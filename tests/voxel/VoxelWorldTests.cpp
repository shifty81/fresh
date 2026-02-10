/**
 * @file VoxelWorldTests.cpp
 * @brief Unit tests for the VoxelWorld class
 */

#include <gtest/gtest.h>
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"

using namespace fresh;

/**
 * Test fixture for VoxelWorld tests
 */
class VoxelWorldTest : public ::testing::Test {
protected:
    void SetUp() override {
        world = std::make_unique<VoxelWorld>();
        world->initialize();
    }

    void TearDown() override {
        world.reset();
    }

    std::unique_ptr<VoxelWorld> world;
};

/**
 * Test that setSeed properly updates the terrain generator
 */
TEST_F(VoxelWorldTest, SetSeed_ChangesTerrainGeneration) {
    // Arrange - Create two worlds with different seeds
    VoxelWorld world1;
    world1.initialize();
    world1.setSeed(12345);
    
    VoxelWorld world2;
    world2.initialize();
    world2.setSeed(54321);
    
    // Act - Generate the same chunk position in both worlds
    Chunk* chunk1 = world1.loadChunk(ChunkPos(0, 0));
    Chunk* chunk2 = world2.loadChunk(ChunkPos(0, 0));
    
    // Assert - Chunks should be different due to different seeds
    bool hasDifference = false;
    for (int x = 0; x < CHUNK_SIZE && !hasDifference; x++) {
        for (int y = 50; y < 70 && !hasDifference; y++) {
            for (int z = 0; z < CHUNK_SIZE && !hasDifference; z++) {
                if (chunk1->getVoxel(x, y, z).type != chunk2->getVoxel(x, y, z).type) {
                    hasDifference = true;
                }
            }
        }
    }
    
    EXPECT_TRUE(hasDifference) << "Different seeds should produce different terrain";
}

/**
 * Test that same seed produces same terrain
 */
TEST_F(VoxelWorldTest, SetSeed_SameSeed_ProducesSameTerrain) {
    // Arrange - Create two worlds with the same seed
    VoxelWorld world1;
    world1.initialize();
    world1.setSeed(99999);
    
    VoxelWorld world2;
    world2.initialize();
    world2.setSeed(99999);
    
    // Act - Generate the same chunk position in both worlds
    Chunk* chunk1 = world1.loadChunk(ChunkPos(0, 0));
    Chunk* chunk2 = world2.loadChunk(ChunkPos(0, 0));
    
    // Assert - Chunks should be identical with same seed
    bool allSame = true;
    for (int x = 0; x < CHUNK_SIZE && allSame; x++) {
        for (int y = 0; y < CHUNK_HEIGHT && allSame; y++) {
            for (int z = 0; z < CHUNK_SIZE && allSame; z++) {
                if (chunk1->getVoxel(x, y, z).type != chunk2->getVoxel(x, y, z).type) {
                    allSame = false;
                }
            }
        }
    }
    
    EXPECT_TRUE(allSame) << "Same seeds should produce identical terrain";
}

/**
 * Test that setSeed works after initialization
 */
TEST_F(VoxelWorldTest, SetSeed_AfterInitialization_UpdatesGenerator) {
    // Arrange - Initialize world with default seed, then change it
    world->setSeed(11111);
    
    // Act - Generate chunk
    Chunk* chunk1 = world->loadChunk(ChunkPos(0, 0));
    
    // Assert - Should not crash and should produce valid terrain
    EXPECT_NE(chunk1, nullptr);
    
    bool hasSolidBlocks = false;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                if (chunk1->getVoxel(x, y, z).type != VoxelType::Air) {
                    hasSolidBlocks = true;
                    break;
                }
            }
            if (hasSolidBlocks) break;
        }
        if (hasSolidBlocks) break;
    }
    
    EXPECT_TRUE(hasSolidBlocks) << "Generated chunk should have solid blocks";
}

/**
 * Test that clearAllChunks removes all loaded chunks
 */
TEST_F(VoxelWorldTest, ClearAllChunks_RemovesAllChunks) {
    // Arrange - Load some chunks
    world->loadChunk(ChunkPos(0, 0));
    world->loadChunk(ChunkPos(1, 0));
    world->loadChunk(ChunkPos(0, 1));
    EXPECT_EQ(world->getChunks().size(), 3u);
    
    // Act
    world->clearAllChunks();
    
    // Assert
    EXPECT_EQ(world->getChunks().size(), 0u);
    EXPECT_EQ(world->getChunk(ChunkPos(0, 0)), nullptr);
    EXPECT_EQ(world->getChunk(ChunkPos(1, 0)), nullptr);
    EXPECT_EQ(world->getChunk(ChunkPos(0, 1)), nullptr);
}

/**
 * Test that clearAllChunks works on empty world
 */
TEST_F(VoxelWorldTest, ClearAllChunks_EmptyWorld_NoEffect) {
    // Arrange - No chunks loaded
    EXPECT_EQ(world->getChunks().size(), 0u);
    
    // Act - Should not crash
    world->clearAllChunks();
    
    // Assert
    EXPECT_EQ(world->getChunks().size(), 0u);
}

/**
 * Test that regenerateLoadedChunks keeps the same chunk positions
 */
TEST_F(VoxelWorldTest, RegenerateLoadedChunks_KeepsSamePositions) {
    // Arrange - Load chunks
    world->loadChunk(ChunkPos(0, 0));
    world->loadChunk(ChunkPos(1, 0));
    size_t chunkCount = world->getChunks().size();
    
    // Act
    world->regenerateLoadedChunks();
    
    // Assert - Same number of chunks at same positions
    EXPECT_EQ(world->getChunks().size(), chunkCount);
    EXPECT_NE(world->getChunk(ChunkPos(0, 0)), nullptr);
    EXPECT_NE(world->getChunk(ChunkPos(1, 0)), nullptr);
}

/**
 * Test that regenerateLoadedChunks produces valid terrain
 */
TEST_F(VoxelWorldTest, RegenerateLoadedChunks_ProducesValidTerrain) {
    // Arrange - Load a chunk
    world->loadChunk(ChunkPos(0, 0));
    
    // Act
    world->regenerateLoadedChunks();
    
    // Assert - Regenerated chunk should have solid blocks (terrain)
    Chunk* chunk = world->getChunk(ChunkPos(0, 0));
    ASSERT_NE(chunk, nullptr);
    
    bool hasSolidBlocks = false;
    for (int x = 0; x < CHUNK_SIZE && !hasSolidBlocks; x++) {
        for (int z = 0; z < CHUNK_SIZE && !hasSolidBlocks; z++) {
            for (int y = 0; y < CHUNK_HEIGHT && !hasSolidBlocks; y++) {
                if (chunk->getVoxel(x, y, z).type != VoxelType::Air) {
                    hasSolidBlocks = true;
                }
            }
        }
    }
    
    EXPECT_TRUE(hasSolidBlocks) << "Regenerated chunk should have solid blocks";
}

/**
 * Test that regenerateLoadedChunks works on empty world
 */
TEST_F(VoxelWorldTest, RegenerateLoadedChunks_EmptyWorld_NoEffect) {
    // Arrange - No chunks loaded
    EXPECT_EQ(world->getChunks().size(), 0u);
    
    // Act - Should not crash
    world->regenerateLoadedChunks();
    
    // Assert
    EXPECT_EQ(world->getChunks().size(), 0u);
}
