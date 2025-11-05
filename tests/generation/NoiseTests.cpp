/**
 * @file NoiseTests.cpp
 * @brief Unit tests for noise generation functions
 */

#include <gtest/gtest.h>
#include "generation/NoiseGenerator.h"
#include <cmath>

using namespace fresh;

/**
 * Test fixture for NoiseGenerator tests
 */
class NoiseGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        seed = 12345;
        noise = std::make_unique<NoiseGenerator>(seed);
    }

    void TearDown() override {
        noise.reset();
    }

    unsigned int seed;
    std::unique_ptr<NoiseGenerator> noise;
};

/**
 * Test Perlin noise determinism
 */
TEST_F(NoiseGeneratorTest, PerlinNoise_SameSeed_ProducesSameResults) {
    // Arrange
    float x = 10.5f, y = 20.3f;
    
    // Act
    float result1 = noise->perlin2D(x, y);
    float result2 = noise->perlin2D(x, y);
    
    // Assert
    EXPECT_FLOAT_EQ(result1, result2);
}

/**
 * Test different seeds produce different results
 */
TEST_F(NoiseGeneratorTest, PerlinNoise_DifferentSeeds_ProducesDifferentResults) {
    // Arrange
    NoiseGenerator noise1(12345);
    NoiseGenerator noise2(54321);
    float x = 10.5f, y = 20.3f;
    
    // Act
    float result1 = noise1.perlin2D(x, y);
    float result2 = noise2.perlin2D(x, y);
    
    // Assert
    EXPECT_NE(result1, result2);
}

/**
 * Test Perlin noise output range
 */
TEST_F(NoiseGeneratorTest, PerlinNoise_OutputRange_WithinExpectedBounds) {
    // Arrange & Act
    int samples = 1000;
    bool allInRange = true;
    
    for (int i = 0; i < samples; i++) {
        float x = static_cast<float>(i) * 0.1f;
        float y = static_cast<float>(i) * 0.1f;
        float value = noise->perlin2D(x, y);
        
        // Perlin noise should be in range [-1, 1]
        if (value < -1.0f || value > 1.0f) {
            allInRange = false;
            break;
        }
    }
    
    // Assert
    EXPECT_TRUE(allInRange) << "Perlin noise output out of expected range [-1, 1]";
}

/**
 * Test noise smoothness (adjacent values should be similar)
 */
TEST_F(NoiseGeneratorTest, PerlinNoise_AdjacentSamples_AreSimilar) {
    // Arrange
    float x = 10.0f, y = 20.0f;
    float step = 0.01f;
    
    // Act
    float value1 = noise->perlin2D(x, y);
    float value2 = noise->perlin2D(x + step, y);
    float difference = std::abs(value1 - value2);
    
    // Assert - very close samples should have small difference
    EXPECT_LT(difference, 0.1f) << "Adjacent noise values differ too much";
}

/**
 * Test fractal noise has more detail than single octave
 */
TEST_F(NoiseGeneratorTest, FractalNoise_MultipleOctaves_HasMoreDetail) {
    // Arrange - use non-integer coordinates to avoid zero values at grid points
    float x = 10.5f, y = 20.5f;
    int octaves = 4;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    
    // Act
    float fractalValue = noise->fractalNoise2D(x, y, octaves, persistence, lacunarity);
    float perlinValue = noise->perlin2D(x, y);
    
    // Assert - they should be different (fractal has more detail)
    EXPECT_NE(fractalValue, perlinValue);
}

/**
 * Test fractal noise determinism
 */
TEST_F(NoiseGeneratorTest, FractalNoise_SameParameters_ProducesSameResults) {
    // Arrange
    float x = 10.0f, y = 20.0f;
    int octaves = 4;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    
    // Act
    float result1 = noise->fractalNoise2D(x, y, octaves, persistence, lacunarity);
    float result2 = noise->fractalNoise2D(x, y, octaves, persistence, lacunarity);
    
    // Assert
    EXPECT_FLOAT_EQ(result1, result2);
}

/**
 * Test 3D Perlin noise
 */
TEST_F(NoiseGeneratorTest, PerlinNoise3D_ValidInput_ReturnsValue) {
    // Arrange
    float x = 10.0f, y = 20.0f, z = 30.0f;
    
    // Act
    float result = noise->perlin3D(x, y, z);
    
    // Assert
    EXPECT_GE(result, -1.0f);
    EXPECT_LE(result, 1.0f);
}

/**
 * Test 3D noise differs along all axes
 */
TEST_F(NoiseGeneratorTest, PerlinNoise3D_DifferentAxes_ProducesDifferentResults) {
    // Arrange - use non-integer coordinates to avoid zero values at grid points
    float base = 10.5f;
    float offset = 5.3f;
    
    // Act
    float value1 = noise->perlin3D(base, base, base);
    float value2 = noise->perlin3D(base + offset, base, base);
    float value3 = noise->perlin3D(base, base + offset, base);
    float value4 = noise->perlin3D(base, base, base + offset);
    
    // Assert - all should be different
    EXPECT_NE(value1, value2);
    EXPECT_NE(value1, value3);
    EXPECT_NE(value1, value4);
}

/**
 * Test octaves affect detail level
 */
TEST_F(NoiseGeneratorTest, FractalNoise_MoreOctaves_MoreDetail) {
    // Arrange - use non-integer coordinates to avoid zero values at grid points
    float x = 10.5f, y = 20.5f;
    
    // Act
    float noise1Octave = noise->fractalNoise2D(x, y, 1, 0.5f, 2.0f);
    float noise4Octaves = noise->fractalNoise2D(x, y, 4, 0.5f, 2.0f);
    
    // Assert - they should be different
    EXPECT_NE(noise1Octave, noise4Octaves);
}

/**
 * Test persistence affects amplitude
 */
TEST_F(NoiseGeneratorTest, FractalNoise_DifferentPersistence_AffectsAmplitude) {
    // Arrange - use non-integer coordinates to avoid zero values at grid points
    float x = 10.5f, y = 20.5f;
    
    // Act
    float lowPersistence = noise->fractalNoise2D(x, y, 4, 0.1f, 2.0f);
    float highPersistence = noise->fractalNoise2D(x, y, 4, 0.9f, 2.0f);
    
    // Assert - they should be different
    EXPECT_NE(lowPersistence, highPersistence);
}

/**
 * Test lacunarity affects frequency
 */
TEST_F(NoiseGeneratorTest, FractalNoise_DifferentLacunarity_AffectsFrequency) {
    // Arrange
    float x = 10.0f, y = 20.0f;
    
    // Act
    float lowLacunarity = noise->fractalNoise2D(x, y, 4, 0.5f, 1.5f);
    float highLacunarity = noise->fractalNoise2D(x, y, 4, 0.5f, 3.0f);
    
    // Assert - they should be different
    EXPECT_NE(lowLacunarity, highLacunarity);
}

/**
 * Test noise continuity (no sudden jumps)
 */
TEST_F(NoiseGeneratorTest, PerlinNoise_Continuity_NoSuddenJumps) {
    // Arrange
    float step = 0.001f;
    int samples = 100;
    bool continuous = true;
    
    // Act
    for (int i = 0; i < samples - 1; i++) {
        float x = static_cast<float>(i) * step;
        float value1 = noise->perlin2D(x, 0.0f);
        float value2 = noise->perlin2D(x + step, 0.0f);
        float difference = std::abs(value1 - value2);
        
        // Very small step should have very small difference
        if (difference > 0.01f) {
            continuous = false;
            break;
        }
    }
    
    // Assert
    EXPECT_TRUE(continuous) << "Noise function has discontinuities";
}

/**
 * Performance test
 */
TEST(NoisePerformanceTest, PerlinNoise_Performance_GeneratesQuickly) {
    // Arrange
    NoiseGenerator noise(12345);
    int samples = 10000;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Act
    float sum = 0.0f;
    for (int i = 0; i < samples; i++) {
        sum += noise.perlin2D(i * 0.1f, i * 0.1f);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert - should generate 10000 samples quickly
    EXPECT_LT(duration.count(), 100) << "Noise generation too slow: " << duration.count() << "ms";
    
    // Use sum to prevent optimization
    EXPECT_NE(sum, 0.0f);
}
