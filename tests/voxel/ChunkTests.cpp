/**
 * @file ChunkTests.cpp
 * @brief Unit tests for the Chunk class
 */

#include <gtest/gtest.h>
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"

using namespace fresh;

/**
 * Test fixture for Chunk tests
 */
class ChunkTest : public ::testing::Test {
protected:
    void SetUp() override {
        chunk = std::make_unique<Chunk>(ChunkPos(0, 0));
    }

    void TearDown() override {
        chunk.reset();
    }

    std::unique_ptr<Chunk> chunk;
};

/**
 * Test chunk creation
 */
TEST_F(ChunkTest, Constructor_CreatesChunk_WithCorrectCoordinates) {
    // Arrange & Act
    auto testChunk = std::make_unique<Chunk>(ChunkPos(5, -3));
    
    // Assert
    EXPECT_EQ(testChunk->getPosition().x, 5);
    EXPECT_EQ(testChunk->getPosition().z, -3);
}

/**
 * Test setting and getting voxels
 */
TEST_F(ChunkTest, SetAndGetVoxel_ValidCoordinates_ReturnsCorrectType) {
    // Arrange
    int x = 5, y = 10, z = 8;
    Voxel expectedVoxel(VoxelType::Stone);
    
    // Act
    chunk->setVoxel(x, y, z, expectedVoxel);
    const Voxel& actualVoxel = chunk->getVoxel(x, y, z);
    
    // Assert
    EXPECT_EQ(actualVoxel.type, expectedVoxel.type);
}

/**
 * Test initial voxel state (should be Air)
 */
TEST_F(ChunkTest, GetVoxel_Uninitialized_ReturnsAir) {
    // Arrange
    int x = 3, y = 50, z = 7;
    
    // Act
    const Voxel& voxel = chunk->getVoxel(x, y, z);
    
    // Assert
    EXPECT_EQ(voxel.type, VoxelType::Air);
}

/**
 * Test boundary conditions
 */
TEST_F(ChunkTest, SetVoxel_AtMinimumBounds_Works) {
    // Arrange
    Voxel voxel(VoxelType::Stone);
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(chunk->setVoxel(0, 0, 0, voxel));
    EXPECT_EQ(chunk->getVoxel(0, 0, 0).type, VoxelType::Stone);
}

TEST_F(ChunkTest, SetVoxel_AtMaximumBounds_Works) {
    // Arrange
    Voxel voxel(VoxelType::Stone);
    int maxX = CHUNK_SIZE - 1;
    int maxY = CHUNK_HEIGHT - 1;
    int maxZ = CHUNK_SIZE - 1;
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(chunk->setVoxel(maxX, maxY, maxZ, voxel));
    EXPECT_EQ(chunk->getVoxel(maxX, maxY, maxZ).type, VoxelType::Stone);
}

/**
 * Test multiple voxel operations
 */
TEST_F(ChunkTest, SetMultipleVoxels_DifferentTypes_AllRetainValues) {
    // Arrange
    struct VoxelData {
        int x, y, z;
        VoxelType type;
    };
    
    std::vector<VoxelData> voxels = {
        {0, 0, 0, VoxelType::Stone},
        {5, 10, 5, VoxelType::Dirt},
        {15, 255, 15, VoxelType::Grass},
        {8, 64, 8, VoxelType::Sand}
    };
    
    // Act
    for (const auto& v : voxels) {
        chunk->setVoxel(v.x, v.y, v.z, Voxel(v.type));
    }
    
    // Assert
    for (const auto& v : voxels) {
        EXPECT_EQ(chunk->getVoxel(v.x, v.y, v.z).type, v.type);
    }
}

/**
 * Test chunk filling
 */
TEST_F(ChunkTest, FillChunk_WithStone_AllVoxelsAreStone) {
    // Arrange
    Voxel stoneVoxel(VoxelType::Stone);
    
    // Act - fill entire chunk
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk->setVoxel(x, y, z, stoneVoxel);
            }
        }
    }
    
    // Assert - spot check several locations
    EXPECT_EQ(chunk->getVoxel(0, 0, 0).type, VoxelType::Stone);
    EXPECT_EQ(chunk->getVoxel(7, 64, 7).type, VoxelType::Stone);
    EXPECT_EQ(chunk->getVoxel(15, 128, 15).type, VoxelType::Stone);
    EXPECT_EQ(chunk->getVoxel(15, 255, 15).type, VoxelType::Stone);
}

/**
 * Test chunk dirty flag
 */
TEST_F(ChunkTest, SetVoxel_ChangesVoxel_MarksDirty) {
    // Arrange
    chunk->clearDirty();
    
    // Act
    chunk->setVoxel(5, 10, 5, Voxel(VoxelType::Stone));
    
    // Assert
    EXPECT_TRUE(chunk->isDirty());
}

TEST_F(ChunkTest, ClearDirty_AfterModification_ClearsDirty) {
    // Arrange
    chunk->setVoxel(5, 10, 5, Voxel(VoxelType::Stone));
    ASSERT_TRUE(chunk->isDirty());
    
    // Act
    chunk->clearDirty();
    
    // Assert
    EXPECT_FALSE(chunk->isDirty());
}

/**
 * Test voxel type queries
 */
TEST_F(ChunkTest, IsSolid_SolidVoxel_ReturnsTrue) {
    // Arrange
    chunk->setVoxel(5, 10, 5, Voxel(VoxelType::Stone));
    
    // Act & Assert
    EXPECT_TRUE(chunk->getVoxel(5, 10, 5).isSolid());
}

TEST_F(ChunkTest, IsSolid_AirVoxel_ReturnsFalse) {
    // Arrange
    chunk->setVoxel(5, 10, 5, Voxel(VoxelType::Air));
    
    // Act & Assert
    EXPECT_FALSE(chunk->getVoxel(5, 10, 5).isSolid());
}

TEST_F(ChunkTest, IsTransparent_WaterVoxel_ReturnsTrue) {
    // Arrange
    chunk->setVoxel(5, 10, 5, Voxel(VoxelType::Water));
    
    // Act & Assert
    EXPECT_TRUE(chunk->getVoxel(5, 10, 5).isTransparent());
}

/**
 * Performance test - filling chunk should be fast
 */
TEST(ChunkPerformanceTest, FillChunk_Performance_CompletesQuickly) {
    // Arrange
    auto chunk = std::make_unique<Chunk>(ChunkPos(0, 0));
    Voxel stoneVoxel(VoxelType::Stone);
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Act - fill entire chunk
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk->setVoxel(x, y, z, stoneVoxel);
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert - should complete in under 100ms (adjust based on your requirements)
    EXPECT_LT(duration.count(), 100) << "Filling chunk took too long: " << duration.count() << "ms";
}
