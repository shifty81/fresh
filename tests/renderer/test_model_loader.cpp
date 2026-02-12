#include <gtest/gtest.h>
#include "renderer/ModelLoader.h"
#include <fstream>

using namespace fresh;

/**
 * @brief Test suite for ModelLoader with tinyobjloader
 */
class ModelLoaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a simple OBJ test file (a triangle)
        createTestOBJ();
    }

    void TearDown() override
    {
        // Clean up test files
        std::remove("test_model.obj");
    }

    void createTestOBJ()
    {
        // Create a simple triangle OBJ file
        std::ofstream file("test_model.obj");
        file << "# Simple triangle\n";
        file << "v 0.0 0.0 0.0\n";
        file << "v 1.0 0.0 0.0\n";
        file << "v 0.5 1.0 0.0\n";
        file << "vn 0.0 0.0 1.0\n";
        file << "vn 0.0 0.0 1.0\n";
        file << "vn 0.0 0.0 1.0\n";
        file << "vt 0.0 0.0\n";
        file << "vt 1.0 0.0\n";
        file << "vt 0.5 1.0\n";
        file << "f 1/1/1 2/2/2 3/3/3\n";
        file.close();
    }
};

TEST_F(ModelLoaderTest, LoadValidOBJ)
{
    auto model = ModelLoader::loadModel("test_model.obj", false);
    
    // Verify model was loaded
    ASSERT_NE(model, nullptr);
    
    // Verify path is stored
    EXPECT_EQ(model->getPath(), "test_model.obj");
    
    // Verify model has meshes
    EXPECT_GT(model->getMeshes().size(), 0);
    
    // Verify first mesh has vertices
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->getVertices().size(), 3); // Triangle has 3 vertices
    EXPECT_EQ(mesh->getIndices().size(), 3);  // Triangle has 3 indices
}

TEST_F(ModelLoaderTest, LoadInvalidFile)
{
    auto model = ModelLoader::loadModel("nonexistent.obj", false);
    
    // Verify loading failed
    EXPECT_EQ(model, nullptr);
}

TEST_F(ModelLoaderTest, CreatePrimitiveCube)
{
    auto model = ModelLoader::createCube(1.0f);
    
    // Verify model was created
    ASSERT_NE(model, nullptr);
    
    // Verify model has meshes
    EXPECT_GT(model->getMeshes().size(), 0);
    
    // Verify cube has vertices (24 vertices for 6 faces)
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->getVertices().size(), 24);
    EXPECT_EQ(mesh->getIndices().size(), 36); // 6 faces * 2 triangles * 3 vertices
}

TEST_F(ModelLoaderTest, CreatePrimitivePlane)
{
    auto model = ModelLoader::createPlane(2.0f, 2.0f);
    
    // Verify model was created
    ASSERT_NE(model, nullptr);
    
    // Verify model has meshes
    EXPECT_GT(model->getMeshes().size(), 0);
    
    // Verify plane has vertices (4 vertices for quad)
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->getVertices().size(), 4);
    EXPECT_EQ(mesh->getIndices().size(), 6); // 2 triangles * 3 vertices
}

TEST_F(ModelLoaderTest, MeshBoundingBox)
{
    auto model = ModelLoader::createCube(2.0f);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    // Verify bounding box is correct for 2x2x2 cube
    glm::vec3 min = mesh->getMin();
    glm::vec3 max = mesh->getMax();
    glm::vec3 center = mesh->getCenter();
    
    // Min should be around (-1, -1, -1)
    EXPECT_NEAR(min.x, -1.0f, 0.01f);
    EXPECT_NEAR(min.y, -1.0f, 0.01f);
    EXPECT_NEAR(min.z, -1.0f, 0.01f);
    
    // Max should be around (1, 1, 1)
    EXPECT_NEAR(max.x, 1.0f, 0.01f);
    EXPECT_NEAR(max.y, 1.0f, 0.01f);
    EXPECT_NEAR(max.z, 1.0f, 0.01f);
    
    // Center should be at origin
    EXPECT_NEAR(center.x, 0.0f, 0.01f);
    EXPECT_NEAR(center.y, 0.0f, 0.01f);
    EXPECT_NEAR(center.z, 0.0f, 0.01f);
}

// ============================================================================
// Sphere primitive tests
// ============================================================================

TEST_F(ModelLoaderTest, CreatePrimitiveSphere)
{
    auto model = ModelLoader::createSphere(1.0f, 16);
    
    ASSERT_NE(model, nullptr);
    EXPECT_GT(model->getMeshes().size(), 0);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    // UV sphere with 16 segments and 8 rings: (8+1)*(16+1) = 153 vertices
    EXPECT_GT(mesh->getVertices().size(), 0);
    EXPECT_GT(mesh->getIndices().size(), 0);
    
    // Indices should be divisible by 3 (triangles)
    EXPECT_EQ(mesh->getIndices().size() % 3, 0);
    
    // Path should be set
    EXPECT_EQ(model->getPath(), "primitive:sphere");
}

TEST_F(ModelLoaderTest, SphereBoundingBox)
{
    float radius = 2.0f;
    auto model = ModelLoader::createSphere(radius, 32);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    // Bounding box should approximately match radius
    glm::vec3 min = mesh->getMin();
    glm::vec3 max = mesh->getMax();
    
    EXPECT_NEAR(min.x, -radius, 0.1f);
    EXPECT_NEAR(min.y, -radius, 0.1f);
    EXPECT_NEAR(min.z, -radius, 0.1f);
    EXPECT_NEAR(max.x, radius, 0.1f);
    EXPECT_NEAR(max.y, radius, 0.1f);
    EXPECT_NEAR(max.z, radius, 0.1f);
}

TEST_F(ModelLoaderTest, SphereNormalsAreUnit)
{
    auto model = ModelLoader::createSphere(1.0f, 16);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    for (const auto& v : mesh->getVertices()) {
        float len = glm::length(v.normal);
        EXPECT_NEAR(len, 1.0f, 0.01f);
    }
}

// ============================================================================
// Cylinder primitive tests
// ============================================================================

TEST_F(ModelLoaderTest, CreatePrimitiveCylinder)
{
    auto model = ModelLoader::createCylinder(1.0f, 2.0f, 16);
    
    ASSERT_NE(model, nullptr);
    EXPECT_GT(model->getMeshes().size(), 0);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    EXPECT_GT(mesh->getVertices().size(), 0);
    EXPECT_GT(mesh->getIndices().size(), 0);
    EXPECT_EQ(mesh->getIndices().size() % 3, 0);
    EXPECT_EQ(model->getPath(), "primitive:cylinder");
}

TEST_F(ModelLoaderTest, CylinderBoundingBox)
{
    float radius = 1.0f;
    float height = 3.0f;
    auto model = ModelLoader::createCylinder(radius, height, 32);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    glm::vec3 min = mesh->getMin();
    glm::vec3 max = mesh->getMax();
    
    EXPECT_NEAR(min.y, -height / 2.0f, 0.01f);
    EXPECT_NEAR(max.y, height / 2.0f, 0.01f);
    EXPECT_NEAR(min.x, -radius, 0.1f);
    EXPECT_NEAR(max.x, radius, 0.1f);
}

// ============================================================================
// Cone primitive tests
// ============================================================================

TEST_F(ModelLoaderTest, CreatePrimitiveCone)
{
    auto model = ModelLoader::createCone(1.0f, 2.0f, 16);
    
    ASSERT_NE(model, nullptr);
    EXPECT_GT(model->getMeshes().size(), 0);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    EXPECT_GT(mesh->getVertices().size(), 0);
    EXPECT_GT(mesh->getIndices().size(), 0);
    EXPECT_EQ(mesh->getIndices().size() % 3, 0);
    EXPECT_EQ(model->getPath(), "primitive:cone");
}

TEST_F(ModelLoaderTest, ConeBoundingBox)
{
    float radius = 1.5f;
    float height = 4.0f;
    auto model = ModelLoader::createCone(radius, height, 32);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    
    glm::vec3 min = mesh->getMin();
    glm::vec3 max = mesh->getMax();
    
    // Apex at halfHeight, base at -halfHeight
    EXPECT_NEAR(max.y, height / 2.0f, 0.01f);
    EXPECT_NEAR(min.y, -height / 2.0f, 0.01f);
    EXPECT_NEAR(min.x, -radius, 0.1f);
    EXPECT_NEAR(max.x, radius, 0.1f);
}

TEST_F(ModelLoaderTest, SphereMinSegments)
{
    // Test with very low segment count - should still produce valid geometry
    auto model = ModelLoader::createSphere(1.0f, 1);
    ASSERT_NE(model, nullptr);
    
    auto mesh = model->getMeshes()[0];
    ASSERT_NE(mesh, nullptr);
    EXPECT_GT(mesh->getVertices().size(), 0);
    EXPECT_GT(mesh->getIndices().size(), 0);
}
