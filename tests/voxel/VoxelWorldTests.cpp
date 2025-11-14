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
