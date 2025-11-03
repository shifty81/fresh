/**
 * @file TerrainGeneratorTests.cpp
 * @brief Unit tests for the TerrainGenerator class
 */

#include <gtest/gtest.h>
#include "generation/TerrainGenerator.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"

using namespace fresh;

/**
 * Test fixture for TerrainGenerator tests
 */
class TerrainGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<TerrainGenerator>(12345);
    }

    void TearDown() override {
        generator.reset();
    }

    std::unique_ptr<TerrainGenerator> generator;
};

/**
 * Test constructor
 */
TEST_F(TerrainGeneratorTest, Constructor_WithSeed_CreatesGenerator) {
    // Act
    TerrainGenerator gen(54321);
    
    // Assert - should not crash
    SUCCEED();
}

TEST_F(TerrainGeneratorTest, Constructor_Default_CreatesGenerator) {
    // Act
    TerrainGenerator gen;
    
    // Assert - should not crash
    SUCCEED();
}

/**
 * Test height generation
 */
TEST_F(TerrainGeneratorTest, GetHeight_ValidCoordinates_ReturnsPositiveHeight) {
    // Arrange
    int x = 0, z = 0;
    
    // Act
    int height = generator->getHeight(x, z);
    
    // Assert
    EXPECT_GT(height, 0);
    EXPECT_LT(height, CHUNK_HEIGHT);
}

TEST_F(TerrainGeneratorTest, GetHeight_SameSeed_DeterministicResults) {
    // Arrange
    TerrainGenerator gen1(12345);
    TerrainGenerator gen2(12345);
    int x = 10, z = 20;
    
    // Act
    int height1 = gen1.getHeight(x, z);
    int height2 = gen2.getHeight(x, z);
    
    // Assert
    EXPECT_EQ(height1, height2);
}

TEST_F(TerrainGeneratorTest, GetHeight_DifferentSeeds_DifferentResults) {
    // Arrange
    TerrainGenerator gen1(12345);
    TerrainGenerator gen2(54321);
    int x = 10, z = 20;
    
    // Act
    int height1 = gen1.getHeight(x, z);
    int height2 = gen2.getHeight(x, z);
    
    // Assert
    EXPECT_NE(height1, height2);
}

TEST_F(TerrainGeneratorTest, GetHeight_AdjacentCoordinates_SimilarHeights) {
    // Arrange
    int x = 10, z = 10;
    
    // Act
    int height1 = generator->getHeight(x, z);
    int height2 = generator->getHeight(x + 1, z);
    int difference = std::abs(height1 - height2);
    
    // Assert - adjacent positions should have similar heights
    EXPECT_LT(difference, 10) << "Adjacent heights differ by too much";
}

TEST_F(TerrainGeneratorTest, GetHeight_NegativeCoordinates_ReturnsValidHeight) {
    // Arrange
    int x = -10, z = -20;
    
    // Act
    int height = generator->getHeight(x, z);
    
    // Assert
    EXPECT_GT(height, 0);
    EXPECT_LT(height, CHUNK_HEIGHT);
}

/**
 * Test seed setting
 */
TEST_F(TerrainGeneratorTest, SetSeed_ChangesGeneration) {
    // Arrange
    int x = 10, z = 20;
    int height1 = generator->getHeight(x, z);
    
    // Act
    generator->setSeed(99999);
    int height2 = generator->getHeight(x, z);
    
    // Assert
    EXPECT_NE(height1, height2);
}

TEST_F(TerrainGeneratorTest, SetSeed_RepeatedSeed_ProducesSameResults) {
    // Arrange
    int x = 10, z = 20;
    generator->setSeed(11111);
    int height1 = generator->getHeight(x, z);
    
    // Act
    generator->setSeed(11111); // Same seed again
    int height2 = generator->getHeight(x, z);
    
    // Assert
    EXPECT_EQ(height1, height2);
}

/**
 * Test chunk generation
 */
TEST_F(TerrainGeneratorTest, GenerateChunk_ValidChunk_FillsWithTerrain) {
    // Arrange
    Chunk chunk(ChunkPos(0, 0));
    
    // Act
    generator->generateChunk(&chunk);
    
    // Assert - chunk should have some solid blocks
    bool hasSolidBlocks = false;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                if (chunk.getVoxel(x, y, z).type != VoxelType::Air) {
                    hasSolidBlocks = true;
                    break;
                }
            }
            if (hasSolidBlocks) break;
        }
        if (hasSolidBlocks) break;
    }
    
    EXPECT_TRUE(hasSolidBlocks) << "Generated chunk has no solid blocks";
}

TEST_F(TerrainGeneratorTest, GenerateChunk_NullPointer_DoesNotCrash) {
    // Act & Assert - should handle gracefully
    EXPECT_NO_THROW(generator->generateChunk(nullptr));
}

TEST_F(TerrainGeneratorTest, GenerateChunk_SameSeed_ProducesSameChunk) {
    // Arrange
    TerrainGenerator gen1(12345);
    TerrainGenerator gen2(12345);
    Chunk chunk1(ChunkPos(0, 0));
    Chunk chunk2(ChunkPos(0, 0));
    
    // Act
    gen1.generateChunk(&chunk1);
    gen2.generateChunk(&chunk2);
    
    // Assert - spot check some positions
    EXPECT_EQ(chunk1.getVoxel(0, 64, 0).type, chunk2.getVoxel(0, 64, 0).type);
    EXPECT_EQ(chunk1.getVoxel(8, 64, 8).type, chunk2.getVoxel(8, 64, 8).type);
    EXPECT_EQ(chunk1.getVoxel(15, 64, 15).type, chunk2.getVoxel(15, 64, 15).type);
}

TEST_F(TerrainGeneratorTest, GenerateChunk_DifferentChunks_DifferentTerrain) {
    // Arrange
    Chunk chunk1(ChunkPos(0, 0));
    Chunk chunk2(ChunkPos(1, 1));
    
    // Act
    generator->generateChunk(&chunk1);
    generator->generateChunk(&chunk2);
    
    // Assert - chunks at different positions should have different terrain
    bool hasDifference = false;
    for (int x = 0; x < CHUNK_SIZE && !hasDifference; x++) {
        for (int z = 0; z < CHUNK_SIZE && !hasDifference; z++) {
            if (chunk1.getVoxel(x, 64, z).type != chunk2.getVoxel(x, 64, z).type) {
                hasDifference = true;
            }
        }
    }
    
    EXPECT_TRUE(hasDifference) << "Different chunk positions produced identical terrain";
}

/**
 * Test terrain features
 */
TEST_F(TerrainGeneratorTest, GenerateChunk_HasBedrockAtBottom) {
    // Arrange
    Chunk chunk(ChunkPos(0, 0));
    
    // Act
    generator->generateChunk(&chunk);
    
    // Assert - bottom layer should have bedrock (or at least solid blocks)
    bool hasSolidAtBottom = false;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (chunk.getVoxel(x, 0, z).isSolid()) {
                hasSolidAtBottom = true;
                break;
            }
        }
        if (hasSolidAtBottom) break;
    }
    
    EXPECT_TRUE(hasSolidAtBottom) << "No solid blocks found at bottom of chunk";
}

TEST_F(TerrainGeneratorTest, GenerateChunk_HasAirAtTop) {
    // Arrange
    Chunk chunk(ChunkPos(0, 0));
    
    // Act
    generator->generateChunk(&chunk);
    
    // Assert - top layers should have air
    bool hasAirAtTop = false;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (chunk.getVoxel(x, CHUNK_HEIGHT - 1, z).type == VoxelType::Air) {
                hasAirAtTop = true;
                break;
            }
        }
        if (hasAirAtTop) break;
    }
    
    EXPECT_TRUE(hasAirAtTop) << "No air found at top of chunk";
}

TEST_F(TerrainGeneratorTest, GenerateChunk_ContainsMultipleBlockTypes) {
    // Arrange
    Chunk chunk(ChunkPos(0, 0));
    std::set<VoxelType> foundTypes;
    
    // Act
    generator->generateChunk(&chunk);
    
    // Collect all block types
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                foundTypes.insert(chunk.getVoxel(x, y, z).type);
            }
        }
    }
    
    // Assert - should have variety (at least Air and one solid type)
    EXPECT_GE(foundTypes.size(), 2) << "Chunk has insufficient block type variety";
    EXPECT_TRUE(foundTypes.count(VoxelType::Air) > 0) << "Chunk has no air blocks";
}

/**
 * Test terrain height variation
 */
TEST_F(TerrainGeneratorTest, GetHeight_MultiplePositions_HasVariation) {
    // Arrange
    std::vector<int> heights;
    
    // Act - sample heights across a region
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            heights.push_back(generator->getHeight(x, z));
        }
    }
    
    // Assert - should have some variation in heights
    int minHeight = *std::min_element(heights.begin(), heights.end());
    int maxHeight = *std::max_element(heights.begin(), heights.end());
    
    EXPECT_GT(maxHeight - minHeight, 0) << "Terrain has no height variation";
}

/**
 * Test terrain continuity across chunks
 */
TEST_F(TerrainGeneratorTest, GenerateChunk_AdjacentChunks_ContinuousTerrain) {
    // Arrange
    Chunk chunk1(ChunkPos(0, 0));
    Chunk chunk2(ChunkPos(1, 0));
    
    // Act
    generator->generateChunk(&chunk1);
    generator->generateChunk(&chunk2);
    
    // Assert - heights at boundaries should be similar
    int height1 = generator->getHeight(15, 8);  // Right edge of chunk1
    int height2 = generator->getHeight(16, 8);  // Left edge of chunk2
    int difference = std::abs(height1 - height2);
    
    EXPECT_LT(difference, 10) << "Adjacent chunk boundaries have discontinuous terrain";
}

/**
 * Performance test
 */
TEST(TerrainGeneratorPerformanceTest, GenerateChunk_Performance_CompletesQuickly) {
    // Arrange
    TerrainGenerator gen(12345);
    Chunk chunk(ChunkPos(0, 0));
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Act
    gen.generateChunk(&chunk);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert - should generate quickly (adjust threshold as needed)
    EXPECT_LT(duration.count(), 100) << "Chunk generation too slow: " << duration.count() << "ms";
}

TEST(TerrainGeneratorPerformanceTest, GetHeight_Performance_ManySamples) {
    // Arrange
    TerrainGenerator gen(12345);
    int samples = 10000;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Act
    int sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += gen.getHeight(i % 256, i / 256);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert
    EXPECT_LT(duration.count(), 100) << "Height sampling too slow: " << duration.count() << "ms";
    EXPECT_GT(sum, 0); // Use sum to prevent optimization
}
