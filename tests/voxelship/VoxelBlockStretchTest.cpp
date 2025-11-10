#include <gtest/gtest.h>

#include "voxelship/VoxelBlock.h"

using namespace fresh::voxelship;

/**
 * Test block stretching functionality with structural integrity
 */
class VoxelBlockStretchTest : public ::testing::Test
{
protected:
    VoxelMaterial testMaterial;
    
    void SetUp() override
    {
        testMaterial.name = "TestMaterial";
        testMaterial.mass = 100.0f;
        testMaterial.hardness = 1.0f;
        testMaterial.conductivity = 0.5f;
        testMaterial.maxStretchRatio = 5.0f;
    }
};

// Test basic block creation
TEST_F(VoxelBlockStretchTest, BasicCreation)
{
    glm::vec3 pos(10.0f, 20.0f, 30.0f);
    glm::vec3 size(2.0f, 3.0f, 4.0f);
    
    VoxelBlock block(pos, size, testMaterial);
    
    EXPECT_EQ(block.getPosition(), pos);
    EXPECT_EQ(block.getSize(), size);
    EXPECT_EQ(block.getMaterial().name, "TestMaterial");
}

// Test valid stretching within limits
TEST_F(VoxelBlockStretchTest, ValidStretch)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 initialSize(1.0f, 1.0f, 1.0f);
    
    VoxelBlock block(pos, initialSize, testMaterial);
    
    // Stretch to 3x in one dimension (within 5.0 limit)
    glm::vec3 newSize(3.0f, 1.0f, 1.0f);
    
    EXPECT_TRUE(block.canStretch(newSize));
    EXPECT_TRUE(block.stretch(newSize));
    EXPECT_EQ(block.getSize(), newSize);
}

// Test stretching at maximum limit
TEST_F(VoxelBlockStretchTest, StretchAtMaxLimit)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 initialSize(1.0f, 1.0f, 1.0f);
    
    VoxelBlock block(pos, initialSize, testMaterial);
    
    // Stretch exactly to 5x limit
    glm::vec3 newSize(5.0f, 1.0f, 1.0f);
    
    EXPECT_TRUE(block.canStretch(newSize));
    EXPECT_TRUE(block.stretch(newSize));
    EXPECT_FLOAT_EQ(block.getStretchRatio(), 5.0f);
}

// Test invalid stretching beyond limits
TEST_F(VoxelBlockStretchTest, InvalidStretchBeyondLimit)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 initialSize(1.0f, 1.0f, 1.0f);
    
    VoxelBlock block(pos, initialSize, testMaterial);
    
    // Try to stretch beyond 5x limit
    glm::vec3 invalidSize(6.0f, 1.0f, 1.0f);
    
    EXPECT_FALSE(block.canStretch(invalidSize));
    EXPECT_FALSE(block.stretch(invalidSize));
    
    // Block size should remain unchanged
    EXPECT_EQ(block.getSize(), initialSize);
}

// Test structural integrity check
TEST_F(VoxelBlockStretchTest, StructuralIntegrity)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 initialSize(2.0f, 2.0f, 2.0f);
    
    VoxelBlock block(pos, initialSize, testMaterial);
    
    // Initial block is structurally sound
    EXPECT_TRUE(block.isStructurallySound());
    
    // Stretch within limits - still sound
    glm::vec3 validSize(8.0f, 2.0f, 2.0f); // 4x stretch
    block.stretch(validSize);
    EXPECT_TRUE(block.isStructurallySound());
}

// Test stretch ratio calculation
TEST_F(VoxelBlockStretchTest, StretchRatioCalculation)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 initialSize(1.0f, 1.0f, 1.0f);
    
    VoxelBlock block(pos, initialSize, testMaterial);
    
    // Initial stretch ratio should be 1.0
    EXPECT_FLOAT_EQ(block.getStretchRatio(), 1.0f);
    
    // Stretch to 2x in X
    block.stretch(glm::vec3(2.0f, 1.0f, 1.0f));
    EXPECT_FLOAT_EQ(block.getStretchRatio(), 2.0f);
    
    // Stretch to 3x in Y (max ratio is now 3.0)
    block.stretch(glm::vec3(2.0f, 3.0f, 1.0f));
    EXPECT_FLOAT_EQ(block.getStretchRatio(), 3.0f);
}
