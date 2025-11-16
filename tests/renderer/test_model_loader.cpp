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
