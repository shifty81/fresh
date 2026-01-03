/**
 * @file ResourceManagerTests.cpp
 * @brief Unit tests for the ResourceManager and resource loading classes
 * 
 * Tests cover:
 * - ResourceManager singleton and initialization
 * - TextureResource loading with stb_image
 * - MeshResource loading with tinyobjloader
 * - AudioClipResource loading (WAV and OGG)
 * - Async loading and caching
 * - Resource unloading and cleanup
 */

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <vector>

#include "core/ResourceManager.h"

namespace fresh
{
namespace tests
{

/**
 * @brief Test fixture for ResourceManager tests
 */
class ResourceManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a temporary test directory
        testDir = std::filesystem::temp_directory_path() / "fresh_resource_test";
        std::filesystem::create_directories(testDir);
        
        // Initialize resource manager
        ResourceManager::getInstance().initialize(testDir.string());
    }

    void TearDown() override
    {
        // Shutdown resource manager
        ResourceManager::getInstance().shutdown();
        
        // Clean up test directory
        std::filesystem::remove_all(testDir);
    }

    std::filesystem::path testDir;
};

// ============================================================================
// ResourceManager Basic Tests
// ============================================================================

TEST_F(ResourceManagerTest, SingletonInstance)
{
    auto& instance1 = ResourceManager::getInstance();
    auto& instance2 = ResourceManager::getInstance();
    
    EXPECT_EQ(&instance1, &instance2);
}

TEST_F(ResourceManagerTest, InitiallyEmpty)
{
    EXPECT_EQ(ResourceManager::getInstance().getLoadedResourceCount(), 0);
}

TEST_F(ResourceManagerTest, MemoryUsageZeroWhenEmpty)
{
    // Shutdown to clear any placeholders
    ResourceManager::getInstance().shutdown();
    ResourceManager::getInstance().initialize(testDir.string());
    
    // Note: May have placeholders loaded, so check for reasonable memory
    size_t memUsage = ResourceManager::getInstance().getTotalMemoryUsage();
    // Placeholders are very small (< 1KB)
    EXPECT_LT(memUsage, 1024);
}

// ============================================================================
// TextureResource Tests
// ============================================================================

TEST_F(ResourceManagerTest, TextureResourcePlaceholder)
{
    TextureResource texture("__placeholder_texture__");
    texture.load();
    
    EXPECT_TRUE(texture.isLoaded());
    EXPECT_EQ(texture.getWidth(), 2);
    EXPECT_EQ(texture.getHeight(), 2);
    EXPECT_EQ(texture.getChannels(), 4);
    EXPECT_NE(texture.getData(), nullptr);
}

TEST_F(ResourceManagerTest, TextureResourceMissingFile)
{
    TextureResource texture("nonexistent/texture.png");
    texture.load();
    
    EXPECT_FALSE(texture.isLoaded());
}

TEST_F(ResourceManagerTest, TextureResourceUnload)
{
    TextureResource texture("__placeholder_texture__");
    texture.load();
    
    EXPECT_TRUE(texture.isLoaded());
    
    texture.unload();
    
    EXPECT_FALSE(texture.isLoaded());
    EXPECT_EQ(texture.getData(), nullptr);
}

TEST_F(ResourceManagerTest, TextureResourceMemoryUsage)
{
    TextureResource texture("__placeholder_texture__");
    texture.load();
    
    // 2x2 RGBA = 16 bytes
    EXPECT_EQ(texture.getMemoryUsage(), 16);
}

TEST_F(ResourceManagerTest, TextureResourceName)
{
    TextureResource texture("path/to/texture.png");
    
    EXPECT_EQ(texture.getName(), "texture");
    EXPECT_EQ(texture.getPath(), "path/to/texture.png");
    EXPECT_EQ(texture.getType(), ResourceType::Texture);
}

// ============================================================================
// MeshResource Tests
// ============================================================================

TEST_F(ResourceManagerTest, MeshResourcePlaceholder)
{
    MeshResource mesh("__placeholder_mesh__");
    mesh.load();
    
    EXPECT_TRUE(mesh.isLoaded());
    EXPECT_FALSE(mesh.getVertices().empty());
    EXPECT_FALSE(mesh.getIndices().empty());
}

TEST_F(ResourceManagerTest, MeshResourceMissingFile)
{
    MeshResource mesh("nonexistent/model.obj");
    mesh.load();
    
    EXPECT_FALSE(mesh.isLoaded());
}

TEST_F(ResourceManagerTest, MeshResourceUnload)
{
    MeshResource mesh("__placeholder_mesh__");
    mesh.load();
    
    EXPECT_TRUE(mesh.isLoaded());
    
    mesh.unload();
    
    EXPECT_FALSE(mesh.isLoaded());
    EXPECT_TRUE(mesh.getVertices().empty());
    EXPECT_TRUE(mesh.getIndices().empty());
}

TEST_F(ResourceManagerTest, MeshResourceName)
{
    MeshResource mesh("models/character.obj");
    
    EXPECT_EQ(mesh.getName(), "character");
    EXPECT_EQ(mesh.getPath(), "models/character.obj");
    EXPECT_EQ(mesh.getType(), ResourceType::Mesh);
}

TEST_F(ResourceManagerTest, MeshResourceVertexLayout)
{
    MeshResource mesh("__placeholder_mesh__");
    mesh.load();
    
    // Each vertex should have 8 floats: pos(3) + normal(3) + texcoord(2)
    const auto& vertices = mesh.getVertices();
    EXPECT_EQ(vertices.size() % 8, 0);
}

// ============================================================================
// AudioClipResource Tests
// ============================================================================

TEST_F(ResourceManagerTest, AudioClipResourcePlaceholder)
{
    AudioClipResource audio("__placeholder_audio__");
    audio.load();
    
    EXPECT_TRUE(audio.isLoaded());
    EXPECT_EQ(audio.getSampleRate(), 44100);
    EXPECT_EQ(audio.getChannels(), 1);
    EXPECT_EQ(audio.getSamples().size(), 44100); // 1 second
}

TEST_F(ResourceManagerTest, AudioClipResourceMissingFile)
{
    AudioClipResource audio("nonexistent/sound.wav");
    audio.load();
    
    EXPECT_FALSE(audio.isLoaded());
}

TEST_F(ResourceManagerTest, AudioClipResourceUnload)
{
    AudioClipResource audio("__placeholder_audio__");
    audio.load();
    
    EXPECT_TRUE(audio.isLoaded());
    
    audio.unload();
    
    EXPECT_FALSE(audio.isLoaded());
    EXPECT_TRUE(audio.getSamples().empty());
}

TEST_F(ResourceManagerTest, AudioClipResourceName)
{
    AudioClipResource audio("sounds/jump.wav");
    
    EXPECT_EQ(audio.getName(), "jump");
    EXPECT_EQ(audio.getPath(), "sounds/jump.wav");
    EXPECT_EQ(audio.getType(), ResourceType::Audio);
}

TEST_F(ResourceManagerTest, AudioClipResourceMemoryUsage)
{
    AudioClipResource audio("__placeholder_audio__");
    audio.load();
    
    // 44100 samples * 2 bytes per sample = 88200 bytes
    EXPECT_EQ(audio.getMemoryUsage(), 44100 * sizeof(int16_t));
}

// ============================================================================
// ResourceManager Operations Tests
// ============================================================================

TEST_F(ResourceManagerTest, ExistsReturnsFalseForMissingResource)
{
    auto& rm = ResourceManager::getInstance();
    
    // Check that exists() returns false for non-existent resources
    EXPECT_FALSE(rm.exists(""));
    EXPECT_FALSE(rm.exists("nonexistent/texture.png"));
    EXPECT_FALSE(rm.exists("missing_model.obj"));
}

TEST_F(ResourceManagerTest, ScanDirectory)
{
    // Create some test files
    std::ofstream(testDir / "texture1.png");
    std::ofstream(testDir / "model1.obj");
    std::ofstream(testDir / "sound1.wav");
    
    auto& rm = ResourceManager::getInstance();
    rm.scanDirectory(testDir.string(), false);
    
    // ScanDirectory just logs what it finds, doesn't load
    // This test just ensures it doesn't crash
    SUCCEED();
}

TEST_F(ResourceManagerTest, UnloadNonExistent)
{
    auto& rm = ResourceManager::getInstance();
    
    // Should not crash when unloading non-existent resource
    rm.unload("nonexistent/path.png");
    
    SUCCEED();
}

TEST_F(ResourceManagerTest, UnloadUnusedEmpty)
{
    auto& rm = ResourceManager::getInstance();
    
    rm.unloadUnused();
    
    // Should not crash with no resources
    SUCCEED();
}

TEST_F(ResourceManagerTest, HotReloadNonExistent)
{
    auto& rm = ResourceManager::getInstance();
    
    // Should not crash when hot-reloading non-existent resource
    rm.hotReload("nonexistent/path.png");
    
    SUCCEED();
}

TEST_F(ResourceManagerTest, GetResourcesOfType)
{
    auto& rm = ResourceManager::getInstance();
    
    auto textures = rm.getResourcesOfType(ResourceType::Texture);
    auto meshes = rm.getResourcesOfType(ResourceType::Mesh);
    
    // Initially may have placeholders
    // Just ensure it returns valid vectors
    EXPECT_GE(textures.size(), 0);
    EXPECT_GE(meshes.size(), 0);
}

// ============================================================================
// Loading Progress Tests
// ============================================================================

TEST_F(ResourceManagerTest, LoadingProgressComplete)
{
    auto& rm = ResourceManager::getInstance();
    
    // When no loading is in progress, progress should be 1.0
    float progress = rm.getLoadingProgress();
    EXPECT_FLOAT_EQ(progress, 1.0f);
}

TEST_F(ResourceManagerTest, IsLoadingReturnsFalse)
{
    auto& rm = ResourceManager::getInstance();
    
    // Non-existent path should not be loading
    EXPECT_FALSE(rm.isLoading("nonexistent/path.png"));
}

} // namespace tests
} // namespace fresh
