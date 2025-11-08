#include "voxel/MeshGenerator.h"
#include "voxel/Chunk.h"
#include <iostream>

namespace fresh {

MeshGenerator::MeshGenerator() {
}

MeshGenerator::~MeshGenerator() {
}

void MeshGenerator::generateChunkMesh(const Chunk* chunk, 
                                     std::vector<float>& vertices,
                                     std::vector<uint32_t>& indices) {
    // Entry point for chunk mesh generation
    // Future: Implement greedy meshing optimization for reduced triangle count
    // Current: Uses simple face-by-face meshing with basic culling
    generateSimpleMesh(chunk, vertices, indices);
}

void MeshGenerator::generateSimpleMesh(const Chunk* chunk,
                                      std::vector<float>& vertices,
                                      std::vector<uint32_t>& indices) {
    // Simple mesh generation with face culling
    // Iterates through all voxels and generates faces for solid blocks
    // Only adds faces that are adjacent to transparent/air blocks (culling optimization)
    vertices.clear();
    indices.clear();
    
    uint32_t vertexCount = 0;
    
    // Iterate in Y-Z-X order for better cache locality during rendering
    for (int y = 0; y < CHUNK_HEIGHT; ++y) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                const Voxel& voxel = chunk->getVoxel(x, y, z);
                
                if (!voxel.isSolid()) {
                    continue;
                }
                
                // Check each face and add if neighbor is air
                // +X face
                if (x == CHUNK_SIZE - 1 || !chunk->getVoxel(x + 1, y, z).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 0;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
                
                // -X face
                if (x == 0 || !chunk->getVoxel(x - 1, y, z).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 1;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
                
                // +Y face
                if (y == CHUNK_HEIGHT - 1 || !chunk->getVoxel(x, y + 1, z).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 2;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
                
                // -Y face
                if (y == 0 || !chunk->getVoxel(x, y - 1, z).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 3;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
                
                // +Z face
                if (z == CHUNK_SIZE - 1 || !chunk->getVoxel(x, y, z + 1).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 4;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
                
                // -Z face
                if (z == 0 || !chunk->getVoxel(x, y, z - 1).isOpaque()) {
                    Face face;
                    face.x = x;
                    face.y = y;
                    face.z = z;
                    face.width = 1;
                    face.height = 1;
                    face.direction = 5;
                    face.type = voxel.type;
                    addFace(face, vertices, indices);
                    vertexCount += 4;
                }
            }
        }
    }
}

void MeshGenerator::addFace(const Face& face,
                            std::vector<float>& vertices,
                            std::vector<uint32_t>& indices) {
    float x = static_cast<float>(face.x);
    float y = static_cast<float>(face.y);
    float z = static_cast<float>(face.z);
    
    uint32_t startIndex = static_cast<uint32_t>(vertices.size() / 6); // 6 floats per vertex (pos + normal)
    
    // Color based on block type (not currently used in vertex data, but may be used for texturing/lighting)
    float r = 0.5f, g = 0.5f, b = 0.5f;
    switch (face.type) {
        case VoxelType::Grass:
            r = 0.2f; g = 0.8f; b = 0.2f;
            break;
        case VoxelType::Dirt:
            r = 0.6f; g = 0.4f; b = 0.2f;
            break;
        case VoxelType::Stone:
            r = 0.5f; g = 0.5f; b = 0.5f;
            break;
        default:
            break;
    }
    // Suppress warnings - colors reserved for future use
    (void)r; (void)g; (void)b;
    
    // Add vertices based on face direction
    switch (face.direction) {
        case 0: // +X
            vertices.insert(vertices.end(), {
                x + 1, y,     z,     1.0f, 0.0f, 0.0f,
                x + 1, y + 1, z,     1.0f, 0.0f, 0.0f,
                x + 1, y + 1, z + 1, 1.0f, 0.0f, 0.0f,
                x + 1, y,     z + 1, 1.0f, 0.0f, 0.0f
            });
            break;
        case 1: // -X
            vertices.insert(vertices.end(), {
                x, y,     z,     -1.0f, 0.0f, 0.0f,
                x, y,     z + 1, -1.0f, 0.0f, 0.0f,
                x, y + 1, z + 1, -1.0f, 0.0f, 0.0f,
                x, y + 1, z,     -1.0f, 0.0f, 0.0f
            });
            break;
        case 2: // +Y
            vertices.insert(vertices.end(), {
                x,     y + 1, z,     0.0f, 1.0f, 0.0f,
                x,     y + 1, z + 1, 0.0f, 1.0f, 0.0f,
                x + 1, y + 1, z + 1, 0.0f, 1.0f, 0.0f,
                x + 1, y + 1, z,     0.0f, 1.0f, 0.0f
            });
            break;
        case 3: // -Y
            vertices.insert(vertices.end(), {
                x,     y, z,     0.0f, -1.0f, 0.0f,
                x + 1, y, z,     0.0f, -1.0f, 0.0f,
                x + 1, y, z + 1, 0.0f, -1.0f, 0.0f,
                x,     y, z + 1, 0.0f, -1.0f, 0.0f
            });
            break;
        case 4: // +Z
            vertices.insert(vertices.end(), {
                x,     y,     z + 1, 0.0f, 0.0f, 1.0f,
                x + 1, y,     z + 1, 0.0f, 0.0f, 1.0f,
                x + 1, y + 1, z + 1, 0.0f, 0.0f, 1.0f,
                x,     y + 1, z + 1, 0.0f, 0.0f, 1.0f
            });
            break;
        case 5: // -Z
            vertices.insert(vertices.end(), {
                x,     y,     z, 0.0f, 0.0f, -1.0f,
                x,     y + 1, z, 0.0f, 0.0f, -1.0f,
                x + 1, y + 1, z, 0.0f, 0.0f, -1.0f,
                x + 1, y,     z, 0.0f, 0.0f, -1.0f
            });
            break;
    }
    
    // Add indices for two triangles
    indices.insert(indices.end(), {
        startIndex, startIndex + 1, startIndex + 2,
        startIndex, startIndex + 2, startIndex + 3
    });
}

} // namespace fresh
