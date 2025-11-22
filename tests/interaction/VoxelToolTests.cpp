/**
 * @file VoxelToolTests.cpp
 * @brief Unit tests for the VoxelTool system
 */

#include <gtest/gtest.h>
#include "interaction/VoxelTool.h"
#include "interaction/ToolManager.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"

using namespace fresh;

/**
 * Test fixture for VoxelTool tests
 */
class VoxelToolTest : public ::testing::Test {
protected:
    void SetUp() override {
        world = std::make_unique<VoxelWorld>();
        world->initialize();
        world->setSeed(12345);
        
        // Load a chunk to work with
        chunk = world->loadChunk(ChunkPos(0, 0));
        ASSERT_NE(chunk, nullptr);
    }

    void TearDown() override {
        world.reset();
    }

    std::unique_ptr<VoxelWorld> world;
    Chunk* chunk;
};

/**
 * Test ConstructionHammer single block placement
 */
TEST_F(VoxelToolTest, ConstructionHammer_PlacesSingleBlock) {
    // Arrange
    ConstructionHammer hammer;
    hammer.initialize(world.get());
    hammer.setBlueprint(ConstructionHammer::Blueprint::SingleBlock);
    
    WorldPos pos(5, 64, 5);
    world->setVoxel(pos, Voxel(VoxelType::Air)); // Ensure starting as air
    
    // Act
    bool result = hammer.useTool(pos, VoxelType::Stone);
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(world->getVoxel(pos).type, VoxelType::Stone);
}

/**
 * Test ConstructionHammer wall blueprint
 */
TEST_F(VoxelToolTest, ConstructionHammer_PlacesWall) {
    // Arrange
    ConstructionHammer hammer;
    hammer.initialize(world.get());
    hammer.setBlueprint(ConstructionHammer::Blueprint::Wall);
    
    WorldPos pos(5, 64, 5);
    
    // Act
    bool result = hammer.useTool(pos, VoxelType::Stone);
    
    // Assert
    EXPECT_TRUE(result);
    
    // Check that wall is 5 blocks high and 3 blocks wide
    int placedBlocks = 0;
    for (int y = 0; y < 5; ++y) {
        for (int x = -1; x <= 1; ++x) {
            WorldPos checkPos(pos.x + x, pos.y + y, pos.z);
            if (world->getVoxel(checkPos).type == VoxelType::Stone) {
                placedBlocks++;
            }
        }
    }
    EXPECT_EQ(placedBlocks, 15); // 5 height * 3 width
}

/**
 * Test ConstructionHammer foundation blueprint
 */
TEST_F(VoxelToolTest, ConstructionHammer_PlacesFoundation) {
    // Arrange
    ConstructionHammer hammer;
    hammer.initialize(world.get());
    hammer.setBlueprint(ConstructionHammer::Blueprint::Foundation);
    
    WorldPos pos(5, 64, 5);
    
    // Act
    bool result = hammer.useTool(pos, VoxelType::Cobblestone);
    
    // Assert
    EXPECT_TRUE(result);
    
    // Check that foundation is 3x3
    int placedBlocks = 0;
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            WorldPos checkPos(pos.x + x, pos.y, pos.z + z);
            if (world->getVoxel(checkPos).type == VoxelType::Cobblestone) {
                placedBlocks++;
            }
        }
    }
    EXPECT_EQ(placedBlocks, 9); // 3x3
}

/**
 * Test Pickaxe single voxel removal
 */
TEST_F(VoxelToolTest, Pickaxe_RemovesSingleVoxel) {
    // Arrange
    Pickaxe pickaxe;
    pickaxe.initialize(world.get());
    pickaxe.setSize(1);
    
    WorldPos pos(5, 64, 5);
    world->setVoxel(pos, Voxel(VoxelType::Stone));
    
    // Act
    bool result = pickaxe.useTool(pos, VoxelType::Air); // voxelType ignored for pickaxe
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(world->getVoxel(pos).type, VoxelType::Air);
}

/**
 * Test Pickaxe does not remove bedrock
 */
TEST_F(VoxelToolTest, Pickaxe_DoesNotRemoveBedrock) {
    // Arrange
    Pickaxe pickaxe;
    pickaxe.initialize(world.get());
    
    WorldPos pos(5, 0, 5);
    world->setVoxel(pos, Voxel(VoxelType::Bedrock));
    
    // Act
    bool result = pickaxe.useTool(pos, VoxelType::Air);
    
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(world->getVoxel(pos).type, VoxelType::Bedrock);
}

/**
 * Test Pickaxe area removal
 */
TEST_F(VoxelToolTest, Pickaxe_RemovesArea) {
    // Arrange
    Pickaxe pickaxe;
    pickaxe.initialize(world.get());
    pickaxe.setSize(2); // 2 block radius
    
    WorldPos centerPos(5, 64, 5);
    
    // Place blocks in area
    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            for (int z = -2; z <= 2; ++z) {
                WorldPos pos(centerPos.x + x, centerPos.y + y, centerPos.z + z);
                world->setVoxel(pos, Voxel(VoxelType::Stone));
            }
        }
    }
    
    // Act
    bool result = pickaxe.useTool(centerPos, VoxelType::Air);
    
    // Assert
    EXPECT_TRUE(result);
    
    // Check that blocks within radius are removed
    // Center and immediate neighbors should be air
    EXPECT_EQ(world->getVoxel(centerPos).type, VoxelType::Air);
    EXPECT_EQ(world->getVoxel(WorldPos(centerPos.x + 1, centerPos.y, centerPos.z)).type, VoxelType::Air);
    EXPECT_EQ(world->getVoxel(WorldPos(centerPos.x, centerPos.y + 1, centerPos.z)).type, VoxelType::Air);
}

/**
 * Test Rake leveling
 */
TEST_F(VoxelToolTest, Rake_LevelsGround) {
    // Arrange
    Rake rake;
    rake.initialize(world.get());
    rake.setSize(2);
    
    WorldPos pos(5, 64, 5);
    
    // Create uneven terrain
    world->setVoxel(WorldPos(pos.x, pos.y + 2, pos.z), Voxel(VoxelType::Stone));
    world->setVoxel(WorldPos(pos.x + 1, pos.y + 3, pos.z), Voxel(VoxelType::Stone));
    
    // Act
    bool result = rake.useTool(pos, VoxelType::Grass);
    
    // Assert
    EXPECT_TRUE(result);
    
    // The terrain should be flattened to the target height (pos.y)
    // Check that blocks above pos.y in the radius are removed
    EXPECT_EQ(world->getVoxel(WorldPos(pos.x, pos.y + 2, pos.z)).type, VoxelType::Air);
}

/**
 * Test ToolManager initialization
 */
TEST_F(VoxelToolTest, ToolManager_Initializes) {
    // Arrange & Act
    ToolManager manager;
    bool result = manager.initialize(world.get());
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_NE(manager.getActiveTool(), nullptr);
}

/**
 * Test ToolManager tool selection
 */
TEST_F(VoxelToolTest, ToolManager_SelectsTool) {
    // Arrange
    ToolManager manager;
    manager.initialize(world.get());
    
    // Act
    bool result = manager.selectTool(VoxelToolType::Pickaxe);
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(manager.getActiveToolType(), VoxelToolType::Pickaxe);
    EXPECT_NE(manager.getPickaxe(), nullptr);
}

/**
 * Test ToolManager tool cycling
 */
TEST_F(VoxelToolTest, ToolManager_CyclesTools) {
    // Arrange
    ToolManager manager;
    manager.initialize(world.get());
    EXPECT_EQ(manager.getActiveToolType(), VoxelToolType::ConstructionHammer);
    
    // Act & Assert
    manager.cycleNextTool();
    EXPECT_EQ(manager.getActiveToolType(), VoxelToolType::Pickaxe);
    
    manager.cycleNextTool();
    EXPECT_EQ(manager.getActiveToolType(), VoxelToolType::Rake);
    
    manager.cycleNextTool();
    EXPECT_EQ(manager.getActiveToolType(), VoxelToolType::ConstructionHammer);
}

/**
 * Test ToolManager active tool usage
 */
TEST_F(VoxelToolTest, ToolManager_UsesActiveTool) {
    // Arrange
    ToolManager manager;
    manager.initialize(world.get());
    manager.selectTool(VoxelToolType::ConstructionHammer);
    
    auto hammer = manager.getConstructionHammer();
    hammer->setBlueprint(ConstructionHammer::Blueprint::SingleBlock);
    
    WorldPos pos(5, 64, 5);
    world->setVoxel(pos, Voxel(VoxelType::Air));
    
    // Act
    bool result = manager.useActiveTool(pos, VoxelType::Wood);
    
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(world->getVoxel(pos).type, VoxelType::Wood);
}
