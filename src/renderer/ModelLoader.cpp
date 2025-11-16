#include "renderer/ModelLoader.h"
#include "renderer/Material.h"
#include "core/Logger.h"

#include <algorithm>
#include <fstream>
#include <unordered_map>

// Use tinyobjloader for OBJ file loading
#ifdef FRESH_TINYOBJLOADER_AVAILABLE
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#endif

namespace fresh
{

// ============================================================================
// Mesh Implementation
// ============================================================================

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& inds)
    : vertices(verts), indices(inds)
{
    calculateBounds();
}

void Mesh::calculateBounds()
{
    if (vertices.empty()) {
        boundsMin = glm::vec3(0.0f);
        boundsMax = glm::vec3(0.0f);
        return;
    }

    boundsMin = vertices[0].position;
    boundsMax = vertices[0].position;

    for (const auto& vertex : vertices) {
        boundsMin = glm::min(boundsMin, vertex.position);
        boundsMax = glm::max(boundsMax, vertex.position);
    }
}

// ============================================================================
// Model Implementation
// ============================================================================

glm::vec3 Model::getMin() const
{
    if (meshes.empty()) {
        return glm::vec3(0.0f);
    }

    glm::vec3 min = meshes[0]->getMin();
    for (const auto& mesh : meshes) {
        min = glm::min(min, mesh->getMin());
    }
    return min;
}

glm::vec3 Model::getMax() const
{
    if (meshes.empty()) {
        return glm::vec3(0.0f);
    }

    glm::vec3 max = meshes[0]->getMax();
    for (const auto& mesh : meshes) {
        max = glm::max(max, mesh->getMax());
    }
    return max;
}

glm::vec3 Model::getCenter() const
{
    return (getMin() + getMax()) * 0.5f;
}

// ============================================================================
// ModelLoader Implementation
// ============================================================================

std::shared_ptr<Model> ModelLoader::loadModel(const std::string& path, bool generateTangents)
{
    return loadModelWithOptions(path, true, true, generateTangents, false, true);
}

std::shared_ptr<Model> ModelLoader::loadModelWithOptions(
    const std::string& path,
    bool triangulate,
    bool generateNormals,
    bool generateTangents,
    bool flipUVs,
    bool optimizeMeshes)
{
#ifndef FRESH_TINYOBJLOADER_AVAILABLE
    Logger::getInstance().error("Model loading not available - tinyobjloader not found: " + path, "ModelLoader");
    return nullptr;
#else
    // Check if file exists
    std::ifstream file(path);
    if (!file.good()) {
        Logger::getInstance().error("Model file not found: " + path, "ModelLoader");
        return nullptr;
    }
    file.close();

    // Load OBJ file using tinyobjloader
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!warn.empty()) {
        Logger::getInstance().warning("Model loading warning: " + warn, "ModelLoader");
    }

    if (!err.empty()) {
        Logger::getInstance().error("Model loading error: " + err, "ModelLoader");
    }

    if (!ret) {
        Logger::getInstance().error("Failed to load model: " + path, "ModelLoader");
        return nullptr;
    }

    // Create model
    auto model = std::make_shared<Model>();
    model->setPath(path);

    // Process each shape (mesh)
    for (const auto& shape : shapes) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Keep track of unique vertices to avoid duplicates
        std::unordered_map<std::string, uint32_t> uniqueVertices;

        // Process each face
        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Only process triangles (tinyobj can triangulate during load)
            if (fv != 3 && triangulate) {
                Logger::getInstance().warning("Non-triangle face found in mesh, skipping", "ModelLoader");
                indexOffset += fv;
                continue;
            }

            // Process vertices in the face
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                Vertex vertex;

                // Position
                vertex.position = glm::vec3(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                );

                // Normal
                if (idx.normal_index >= 0 && !attrib.normals.empty()) {
                    vertex.normal = glm::vec3(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    );
                } else if (generateNormals) {
                    vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Will be calculated later
                }

                // Texture coordinates
                if (idx.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                    vertex.texCoord = glm::vec2(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        flipUVs ? 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                                : attrib.texcoords[2 * idx.texcoord_index + 1]
                    );
                }

                // Vertex color (if available)
                if (!attrib.colors.empty() && 3 * idx.vertex_index + 2 < attrib.colors.size()) {
                    vertex.color = glm::vec4(
                        attrib.colors[3 * idx.vertex_index + 0],
                        attrib.colors[3 * idx.vertex_index + 1],
                        attrib.colors[3 * idx.vertex_index + 2],
                        1.0f
                    );
                }

                // Create unique vertex key
                std::string key = std::to_string(vertex.position.x) + "_" +
                                  std::to_string(vertex.position.y) + "_" +
                                  std::to_string(vertex.position.z) + "_" +
                                  std::to_string(vertex.texCoord.x) + "_" +
                                  std::to_string(vertex.texCoord.y);

                // Check if vertex already exists
                if (optimizeMeshes && uniqueVertices.count(key) > 0) {
                    indices.push_back(uniqueVertices[key]);
                } else {
                    uint32_t index = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                    indices.push_back(index);
                    if (optimizeMeshes) {
                        uniqueVertices[key] = index;
                    }
                }
            }

            indexOffset += fv;
        }

        // Generate tangents if requested
        if (generateTangents && !vertices.empty()) {
            calculateTangents(vertices, indices);
        }

        // Create mesh
        auto mesh = std::make_shared<Mesh>(vertices, indices);
        
        // Set material (if available)
        // TODO: Process materials from tinyobj::material_t
        
        model->addMesh(mesh);

        Logger::getInstance().info("Loaded mesh with " + std::to_string(vertices.size()) + 
                                  " vertices and " + std::to_string(indices.size()) + " indices", 
                                  "ModelLoader");
    }

    Logger::getInstance().info("Successfully loaded model: " + path + 
                              " (" + std::to_string(model->getMeshes().size()) + " meshes)", 
                              "ModelLoader");

    return model;
#endif // FRESH_TINYOBJLOADER_AVAILABLE
}

void ModelLoader::calculateTangents(std::vector<Vertex>& vertices,
                                   const std::vector<uint32_t>& indices)
{
    // Calculate tangents and bitangents for normal mapping
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i + 0]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;

        // Calculate bitangent
        glm::vec3 bitangent = glm::cross(v0.normal, tangent);
        v0.bitangent = bitangent;
        v1.bitangent = bitangent;
        v2.bitangent = bitangent;
    }
}

// ============================================================================
// Primitive Shape Creation
// ============================================================================

std::shared_ptr<Model> ModelLoader::createCube(float size)
{
    float halfSize = size * 0.5f;
    
    std::vector<Vertex> vertices = {
        // Front face
        {{-halfSize, -halfSize,  halfSize}, {0, 0, 1}, {0, 0}},
        {{ halfSize, -halfSize,  halfSize}, {0, 0, 1}, {1, 0}},
        {{ halfSize,  halfSize,  halfSize}, {0, 0, 1}, {1, 1}},
        {{-halfSize,  halfSize,  halfSize}, {0, 0, 1}, {0, 1}},
        
        // Back face
        {{ halfSize, -halfSize, -halfSize}, {0, 0, -1}, {0, 0}},
        {{-halfSize, -halfSize, -halfSize}, {0, 0, -1}, {1, 0}},
        {{-halfSize,  halfSize, -halfSize}, {0, 0, -1}, {1, 1}},
        {{ halfSize,  halfSize, -halfSize}, {0, 0, -1}, {0, 1}},
        
        // Top face
        {{-halfSize,  halfSize,  halfSize}, {0, 1, 0}, {0, 0}},
        {{ halfSize,  halfSize,  halfSize}, {0, 1, 0}, {1, 0}},
        {{ halfSize,  halfSize, -halfSize}, {0, 1, 0}, {1, 1}},
        {{-halfSize,  halfSize, -halfSize}, {0, 1, 0}, {0, 1}},
        
        // Bottom face
        {{-halfSize, -halfSize, -halfSize}, {0, -1, 0}, {0, 0}},
        {{ halfSize, -halfSize, -halfSize}, {0, -1, 0}, {1, 0}},
        {{ halfSize, -halfSize,  halfSize}, {0, -1, 0}, {1, 1}},
        {{-halfSize, -halfSize,  halfSize}, {0, -1, 0}, {0, 1}},
        
        // Right face
        {{ halfSize, -halfSize,  halfSize}, {1, 0, 0}, {0, 0}},
        {{ halfSize, -halfSize, -halfSize}, {1, 0, 0}, {1, 0}},
        {{ halfSize,  halfSize, -halfSize}, {1, 0, 0}, {1, 1}},
        {{ halfSize,  halfSize,  halfSize}, {1, 0, 0}, {0, 1}},
        
        // Left face
        {{-halfSize, -halfSize, -halfSize}, {-1, 0, 0}, {0, 0}},
        {{-halfSize, -halfSize,  halfSize}, {-1, 0, 0}, {1, 0}},
        {{-halfSize,  halfSize,  halfSize}, {-1, 0, 0}, {1, 1}},
        {{-halfSize,  halfSize, -halfSize}, {-1, 0, 0}, {0, 1}},
    };
    
    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Right
        20, 21, 22, 22, 23, 20  // Left
    };
    
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    auto model = std::make_shared<Model>();
    model->addMesh(mesh);
    model->setPath("primitive:cube");
    
    return model;
}

std::shared_ptr<Model> ModelLoader::createPlane(float width, float height)
{
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    std::vector<Vertex> vertices = {
        {{-halfW, 0, -halfH}, {0, 1, 0}, {0, 0}},
        {{ halfW, 0, -halfH}, {0, 1, 0}, {1, 0}},
        {{ halfW, 0,  halfH}, {0, 1, 0}, {1, 1}},
        {{-halfW, 0,  halfH}, {0, 1, 0}, {0, 1}},
    };
    
    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    auto model = std::make_shared<Model>();
    model->addMesh(mesh);
    model->setPath("primitive:plane");
    
    return model;
}

std::shared_ptr<Model> ModelLoader::createSphere(float radius, int /* segments */)
{
    // TODO: Implement sphere generation
    Logger::getInstance().warning("Sphere primitive not yet implemented", "ModelLoader");
    return createCube(radius * 2.0f);
}

std::shared_ptr<Model> ModelLoader::createCylinder(float radius, float /* height */, int /* segments */)
{
    // TODO: Implement cylinder generation
    Logger::getInstance().warning("Cylinder primitive not yet implemented", "ModelLoader");
    return createCube(radius * 2.0f);
}

std::shared_ptr<Model> ModelLoader::createCone(float radius, float /* height */, int /* segments */)
{
    // TODO: Implement cone generation
    Logger::getInstance().warning("Cone primitive not yet implemented", "ModelLoader");
    return createCube(radius * 2.0f);
}

} // namespace fresh
