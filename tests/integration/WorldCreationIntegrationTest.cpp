/**
 * @file WorldCreationIntegrationTest.cpp
 * @brief Integration tests for world creation with custom seeds
 */

#include <gtest/gtest.h>
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"
#include "generation/TerrainGenerator.h"
#include <memory>

using namespace fresh;

/**
 * Test fixture for world creation integration tests
 */
class WorldCreationIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

/**
 * Simulate the world creation flow from Engine::createNewWorld
 * This tests the bug fix where the custom seed was ignored
 */
TEST_F(WorldCreationIntegrationTest, CreateNewWorld_WithCustomSeed_UsesCorrectSeed) {
    // Arrange - Simulate two world creations with different seeds
    const int seed1 = 12345;
    const int seed2 = 54321;
    
    // World 1
    auto world1 = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(world1->initialize());
    world1->setSeed(seed1);
    
    // World 2
    auto world2 = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(world2->initialize());
    world2->setSeed(seed2);
    
    // Act - Generate initial chunks around spawn (simulating Engine::createNewWorld)
    const int chunkRadius = 3;
    
    // Generate chunks for world 1
    for (int x = -chunkRadius; x <= chunkRadius; ++x) {
        for (int z = -chunkRadius; z <= chunkRadius; ++z) {
            world1->loadChunk(ChunkPos(x, z));
        }
    }
    
    // Generate chunks for world 2
    for (int x = -chunkRadius; x <= chunkRadius; ++x) {
        for (int z = -chunkRadius; z <= chunkRadius; ++z) {
            world2->loadChunk(ChunkPos(x, z));
        }
    }
    
    // Assert - Verify that the worlds are different (proving seeds were used)
    Chunk* chunk1 = world1->getChunk(ChunkPos(0, 0));
    Chunk* chunk2 = world2->getChunk(ChunkPos(0, 0));
    
    ASSERT_NE(chunk1, nullptr);
    ASSERT_NE(chunk2, nullptr);
    
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
    
    EXPECT_TRUE(hasDifference) << "Different seeds should produce different worlds";
}

/**
 * Test that the same seed produces consistent worlds
 * This verifies deterministic world generation
 */
TEST_F(WorldCreationIntegrationTest, CreateNewWorld_SameSeed_ProducesIdenticalWorlds) {
    // Arrange
    const int seed = 99999;
    
    // World 1
    auto world1 = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(world1->initialize());
    world1->setSeed(seed);
    
    // World 2
    auto world2 = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(world2->initialize());
    world2->setSeed(seed);
    
    // Act - Generate the same chunks
    const int chunkRadius = 2;
    for (int x = -chunkRadius; x <= chunkRadius; ++x) {
        for (int z = -chunkRadius; z <= chunkRadius; ++z) {
            world1->loadChunk(ChunkPos(x, z));
            world2->loadChunk(ChunkPos(x, z));
        }
    }
    
    // Assert - All chunks should be identical
    for (int cx = -chunkRadius; cx <= chunkRadius; ++cx) {
        for (int cz = -chunkRadius; cz <= chunkRadius; ++cz) {
            Chunk* chunk1 = world1->getChunk(ChunkPos(cx, cz));
            Chunk* chunk2 = world2->getChunk(ChunkPos(cx, cz));
            
            ASSERT_NE(chunk1, nullptr);
            ASSERT_NE(chunk2, nullptr);
            
            // Check all voxels in the chunk
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int y = 0; y < CHUNK_HEIGHT; y++) {
                    for (int z = 0; z < CHUNK_SIZE; z++) {
                        EXPECT_EQ(chunk1->getVoxel(x, y, z).type, chunk2->getVoxel(x, y, z).type)
                            << "Mismatch at chunk (" << cx << "," << cz << ") position ("
                            << x << "," << y << "," << z << ")";
                    }
                }
            }
        }
    }
}

/**
 * Test the bug that was fixed: verify old behavior would have failed
 * Before the fix, VoxelWorld always used seed 12345 regardless of input
 */
TEST_F(WorldCreationIntegrationTest, BugFix_CustomSeedIsNotIgnored) {
    // Arrange - Create world with non-default seed
    const int customSeed = 88888;  // Different from the hardcoded 12345
    
    auto worldWithCustomSeed = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(worldWithCustomSeed->initialize());
    worldWithCustomSeed->setSeed(customSeed);
    
    auto worldWithDefaultSeed = std::make_unique<VoxelWorld>();
    ASSERT_TRUE(worldWithDefaultSeed->initialize());
    worldWithDefaultSeed->setSeed(12345);  // The old hardcoded seed
    
    // Act - Generate same chunk position
    worldWithCustomSeed->loadChunk(ChunkPos(0, 0));
    worldWithDefaultSeed->loadChunk(ChunkPos(0, 0));
    
    Chunk* customChunk = worldWithCustomSeed->getChunk(ChunkPos(0, 0));
    Chunk* defaultChunk = worldWithDefaultSeed->getChunk(ChunkPos(0, 0));
    
    // Assert - They should be different (proving custom seed is used)
    bool hasDifference = false;
    for (int x = 0; x < CHUNK_SIZE && !hasDifference; x++) {
        for (int y = 50; y < 70 && !hasDifference; y++) {
            for (int z = 0; z < CHUNK_SIZE && !hasDifference; z++) {
                if (customChunk->getVoxel(x, y, z).type != defaultChunk->getVoxel(x, y, z).type) {
                    hasDifference = true;
                }
            }
        }
    }
    
    EXPECT_TRUE(hasDifference) 
        << "Custom seed should produce different terrain than default seed (bug was that custom seed was ignored)";
}

/**
 * Test world generation with multiple different seeds
 */
TEST_F(WorldCreationIntegrationTest, MultipleSeeds_ProduceUniqueWorlds) {
    // Arrange - Create worlds with different seeds
    std::vector<int> seeds = {111, 222, 333, 444, 555};
    std::vector<std::unique_ptr<VoxelWorld>> worlds;
    
    for (int seed : seeds) {
        auto world = std::make_unique<VoxelWorld>();
        ASSERT_TRUE(world->initialize());
        world->setSeed(seed);
        world->loadChunk(ChunkPos(0, 0));
        worlds.push_back(std::move(world));
    }
    
    // Assert - Each world should be different from the others
    for (size_t i = 0; i < worlds.size(); i++) {
        for (size_t j = i + 1; j < worlds.size(); j++) {
            Chunk* chunk_i = worlds[i]->getChunk(ChunkPos(0, 0));
            Chunk* chunk_j = worlds[j]->getChunk(ChunkPos(0, 0));
            
            bool hasDifference = false;
            for (int x = 0; x < CHUNK_SIZE && !hasDifference; x++) {
                for (int y = 50; y < 70 && !hasDifference; y++) {
                    for (int z = 0; z < CHUNK_SIZE && !hasDifference; z++) {
                        if (chunk_i->getVoxel(x, y, z).type != chunk_j->getVoxel(x, y, z).type) {
                            hasDifference = true;
                        }
                    }
                }
            }
            
            EXPECT_TRUE(hasDifference) 
                << "Worlds with seeds " << seeds[i] << " and " << seeds[j] << " should be different";
        }
    }
}
