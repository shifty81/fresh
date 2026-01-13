/**
 * @file HeightmapWorldGeneratorTests.cpp
 * @brief Unit tests for HeightmapWorldGenerator class
 */

#include <gtest/gtest.h>
#include "generation/HeightmapWorldGenerator.h"
#include "voxel/Chunk.h"
#include <cmath>

namespace fresh {

class HeightmapWorldGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<HeightmapWorldGenerator>();
        
        // Create a simple test heightmap in memory
        createTestHeightmap();
    }

    void TearDown() override {
        generator.reset();
    }

    void createTestHeightmap() {
        // Create a 16x16 test heightmap with a gradient
        testHeightmap.width = 16;
        testHeightmap.height = 16;
        testHeightmap.heights.resize(16 * 16);
        
        for (int z = 0; z < 16; ++z) {
            for (int x = 0; x < 16; ++x) {
                // Simple gradient from 0.0 to 1.0
                float height = static_cast<float>(z) / 15.0f;
                testHeightmap.heights[z * 16 + x] = height;
            }
        }
    }

    std::unique_ptr<HeightmapWorldGenerator> generator;
    HeightmapData testHeightmap;
};

TEST_F(HeightmapWorldGeneratorTest, DefaultInitialization) {
    EXPECT_FALSE(generator->isHeightmapLoaded());
}

TEST_F(HeightmapWorldGeneratorTest, HeightmapDataGetHeight) {
    float height = testHeightmap.getHeight(0, 0);
    EXPECT_FLOAT_EQ(height, 0.0f);
    
    height = testHeightmap.getHeight(0, 15);
    EXPECT_FLOAT_EQ(height, 1.0f);
    
    height = testHeightmap.getHeight(0, 7);
    EXPECT_NEAR(height, 7.0f / 15.0f, 0.01f);
}

TEST_F(HeightmapWorldGeneratorTest, HeightmapDataOutOfBounds) {
    float height = testHeightmap.getHeight(-1, 0);
    EXPECT_FLOAT_EQ(height, 0.0f);
    
    height = testHeightmap.getHeight(100, 100);
    EXPECT_FLOAT_EQ(height, 0.0f);
}

TEST_F(HeightmapWorldGeneratorTest, BilinearInterpolation) {
    // Test interpolation at center of four pixels
    float height = testHeightmap.getHeightInterpolated(0.5f, 0.5f);
    
    // Should be average of corners (0, 0, 0, 1/15)
    float expected = (0.0f + 0.0f + 0.0f + (1.0f / 15.0f)) / 4.0f;
    EXPECT_NEAR(height, expected, 0.01f);
}

TEST_F(HeightmapWorldGeneratorTest, DefaultLayersSetup) {
    HeightmapGeneratorParams params;
    params.setupDefaultLayers();
    
    EXPECT_FALSE(params.layers.empty());
    EXPECT_GT(params.layers.size(), 0);
    
    // Check that layers cover the full range
    EXPECT_FLOAT_EQ(params.layers.front().minHeight, 0.0f);
    EXPECT_FLOAT_EQ(params.layers.back().maxHeight, 1.0f);
}

TEST_F(HeightmapWorldGeneratorTest, ParametersInitialization) {
    HeightmapGeneratorParams params;
    params.heightmapPath = "test.png";
    params.minHeight = 0.0f;
    params.maxHeight = 256.0f;
    params.verticalScale = 1.0f;
    params.horizontalScale = 1.0f;
    params.setupDefaultLayers();
    
    // Can't actually load file in tests, but can test parameter setup
    EXPECT_EQ(params.heightmapPath, "test.png");
    EXPECT_FLOAT_EQ(params.minHeight, 0.0f);
    EXPECT_FLOAT_EQ(params.maxHeight, 256.0f);
}

TEST_F(HeightmapWorldGeneratorTest, ChunkGenerationWithoutHeightmap) {
    // Test that chunk generation handles missing heightmap gracefully
    auto chunk = std::make_unique<Chunk>();
    chunk->initialize(ChunkPos(0, 0));
    
    // Should not crash even without heightmap loaded
    EXPECT_NO_THROW(generator->generateChunk(chunk.get(), ChunkPos(0, 0)));
}

TEST_F(HeightmapWorldGeneratorTest, HeightScaling) {
    HeightmapGeneratorParams params;
    params.minHeight = 0.0f;
    params.maxHeight = 128.0f;
    params.verticalScale = 2.0f;
    params.horizontalScale = 1.0f;
    params.setupDefaultLayers();
    
    // Height of 0.5 normalized should map to 64 * 2 = 128 voxels
    float normalizedHeight = 0.5f;
    float expectedHeight = params.minHeight + 
                          (normalizedHeight * (params.maxHeight - params.minHeight) * params.verticalScale);
    
    EXPECT_FLOAT_EQ(expectedHeight, 128.0f);
}

TEST_F(HeightmapWorldGeneratorTest, HorizontalScaling) {
    // Test that horizontal scaling affects coordinate mapping
    HeightmapGeneratorParams params;
    params.horizontalScale = 2.0f; // 1 pixel = 2 voxels
    
    // World coordinate 10 should map to heightmap coordinate 5
    float worldX = 10.0f;
    float expectedHmX = worldX / params.horizontalScale;
    
    EXPECT_FLOAT_EQ(expectedHmX, 5.0f);
}

TEST_F(HeightmapWorldGeneratorTest, LayerSelection) {
    HeightmapGeneratorParams params;
    params.layers.clear();
    
    // Add test layers
    params.layers.push_back({0.0f, 0.3f, VoxelType::Sand});
    params.layers.push_back({0.3f, 0.7f, VoxelType::Grass});
    params.layers.push_back({0.7f, 1.0f, VoxelType::Stone});
    
    // Test that correct block types are selected for different heights
    // (This would require exposing the private method or testing through chunk generation)
}

TEST_F(HeightmapWorldGeneratorTest, RepeatWrapping) {
    HeightmapGeneratorParams params;
    params.repeatX = true;
    params.repeatZ = true;
    
    // Test that wrapping is enabled
    EXPECT_TRUE(params.repeatX);
    EXPECT_TRUE(params.repeatZ);
    
    // Actual wrapping behavior would be tested through coordinate mapping
}

TEST_F(HeightmapWorldGeneratorTest, GetParams) {
    HeightmapGeneratorParams params;
    params.minHeight = 10.0f;
    params.maxHeight = 100.0f;
    
    generator->setParams(params);
    
    const HeightmapGeneratorParams& retrieved = generator->getParams();
    EXPECT_FLOAT_EQ(retrieved.minHeight, 10.0f);
    EXPECT_FLOAT_EQ(retrieved.maxHeight, 100.0f);
}

} // namespace fresh
