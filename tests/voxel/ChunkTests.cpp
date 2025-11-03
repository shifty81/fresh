/**
 * @file ChunkTests.cpp
 * @brief Unit tests for the Chunk class
 */

#include <gtest/gtest.h>
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"

/**
 * Test fixture for Chunk tests
 */
class ChunkTest : public ::testing::Test {
protected:
    void SetUp() override {
        chunk = std::make_unique<Chunk>(0, 0);
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
    auto testChunk = std::make_unique<Chunk>(5, -3);
    
    // Assert
    EXPECT_EQ(testChunk->getChunkX(), 5);
    EXPECT_EQ(testChunk->getChunkZ(), -3);
}

/**
 * Test setting and getting voxels
 */
TEST_F(ChunkTest, SetAndGetVoxel_ValidCoordinates_ReturnsCorrectType) {
    // Arrange
    int x = 5, y = 10, z = 8;
    VoxelType expectedType = VoxelType::Stone;
    
    // Act
    chunk->setVoxel(x, y, z, expectedType);
    VoxelType actualType = chunk->getVoxel(x, y, z);
    
    // Assert
    EXPECT_EQ(actualType, expectedType);
}

/**
 * Test initial voxel state (should be Air)
 */
TEST_F(ChunkTest, GetVoxel_Uninitialized_ReturnsAir) {
    // Arrange
    int x = 3, y = 50, z = 7;
    
    // Act
    VoxelType type = chunk->getVoxel(x, y, z);
    
    // Assert
    EXPECT_EQ(type, VoxelType::Air);
}

/**
 * Test boundary conditions
 */
TEST_F(ChunkTest, SetVoxel_AtMinimumBounds_Works) {
    // Arrange
    VoxelType type = VoxelType::Diamond;
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(chunk->setVoxel(0, 0, 0, type));
    EXPECT_EQ(chunk->getVoxel(0, 0, 0), type);
}

TEST_F(ChunkTest, SetVoxel_AtMaximumBounds_Works) {
    // Arrange
    VoxelType type = VoxelType::Gold;
    int maxX = CHUNK_SIZE - 1;
    int maxY = CHUNK_HEIGHT - 1;
    int maxZ = CHUNK_SIZE - 1;
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(chunk->setVoxel(maxX, maxY, maxZ, type));
    EXPECT_EQ(chunk->getVoxel(maxX, maxY, maxZ), type);
}

/**
 * Test invalid coordinates handling
 */
TEST_F(ChunkTest, GetVoxel_OutOfBounds_ReturnsAir) {
    // Act
    VoxelType type = chunk->getVoxel(-1, 0, 0);
    
    // Assert - out of bounds should return Air
    EXPECT_EQ(type, VoxelType::Air);
}

TEST_F(ChunkTest, GetVoxel_YTooHigh_ReturnsAir) {
    // Act
    VoxelType type = chunk->getVoxel(5, CHUNK_HEIGHT + 10, 5);
    
    // Assert
    EXPECT_EQ(type, VoxelType::Air);
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
        {0, 0, 0, VoxelType::Bedrock},
        {5, 10, 5, VoxelType::Stone},
        {15, 255, 15, VoxelType::Diamond},
        {8, 64, 8, VoxelType::Grass}
    };
    
    // Act
    for (const auto& v : voxels) {
        chunk->setVoxel(v.x, v.y, v.z, v.type);
    }
    
    // Assert
    for (const auto& v : voxels) {
        EXPECT_EQ(chunk->getVoxel(v.x, v.y, v.z), v.type);
    }
}

/**
 * Test chunk filling
 */
TEST_F(ChunkTest, FillChunk_WithStone_AllVoxelsAreStone) {
    // Arrange
    VoxelType fillType = VoxelType::Stone;
    
    // Act - fill entire chunk
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk->setVoxel(x, y, z, fillType);
            }
        }
    }
    
    // Assert - spot check several locations
    EXPECT_EQ(chunk->getVoxel(0, 0, 0), fillType);
    EXPECT_EQ(chunk->getVoxel(7, 64, 7), fillType);
    EXPECT_EQ(chunk->getVoxel(15, 128, 15), fillType);
    EXPECT_EQ(chunk->getVoxel(15, 255, 15), fillType);
}

/**
 * Test chunk dirty flag
 */
TEST_F(ChunkTest, SetVoxel_ChangesVoxel_MarksDirty) {
    // Arrange
    chunk->clearDirtyFlag();
    
    // Act
    chunk->setVoxel(5, 10, 5, VoxelType::Stone);
    
    // Assert
    EXPECT_TRUE(chunk->isDirty());
}

TEST_F(ChunkTest, ClearDirtyFlag_AfterModification_ClearsDirty) {
    // Arrange
    chunk->setVoxel(5, 10, 5, VoxelType::Stone);
    ASSERT_TRUE(chunk->isDirty());
    
    // Act
    chunk->clearDirtyFlag();
    
    // Assert
    EXPECT_FALSE(chunk->isDirty());
}

/**
 * Test chunk coordinate queries
 */
TEST_F(ChunkTest, GetWorldPosition_LocalCoords_ReturnsCorrectWorldPos) {
    // Arrange
    auto chunk5_3 = std::make_unique<Chunk>(5, -3);
    int localX = 7, localY = 64, localZ = 9;
    
    // Act
    auto worldPos = chunk5_3->getWorldPosition(localX, localY, localZ);
    
    // Assert
    EXPECT_EQ(worldPos.x, 5 * CHUNK_SIZE + 7);  // 87
    EXPECT_EQ(worldPos.y, 64);
    EXPECT_EQ(worldPos.z, -3 * CHUNK_SIZE + 9); // -39
}

/**
 * Test voxel type queries
 */
TEST_F(ChunkTest, IsSolid_SolidVoxel_ReturnsTrue) {
    // Arrange
    chunk->setVoxel(5, 10, 5, VoxelType::Stone);
    
    // Act & Assert
    EXPECT_TRUE(chunk->isSolid(5, 10, 5));
}

TEST_F(ChunkTest, IsSolid_AirVoxel_ReturnsFalse) {
    // Arrange
    chunk->setVoxel(5, 10, 5, VoxelType::Air);
    
    // Act & Assert
    EXPECT_FALSE(chunk->isSolid(5, 10, 5));
}

TEST_F(ChunkTest, IsSolid_WaterVoxel_ReturnsFalse) {
    // Arrange
    chunk->setVoxel(5, 10, 5, VoxelType::Water);
    
    // Act & Assert
    EXPECT_FALSE(chunk->isSolid(5, 10, 5));
}

/**
 * Performance test - filling chunk should be fast
 */
TEST(ChunkPerformanceTest, FillChunk_Performance_CompletesQuickly) {
    // Arrange
    auto chunk = std::make_unique<Chunk>(0, 0);
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Act - fill entire chunk
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert - should complete in under 100ms (adjust based on your requirements)
    EXPECT_LT(duration.count(), 100) << "Filling chunk took too long: " << duration.count() << "ms";
}
