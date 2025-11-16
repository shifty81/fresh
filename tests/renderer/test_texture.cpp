#include <gtest/gtest.h>
#include "renderer/Texture.h"
#include <fstream>

using namespace fresh;

/**
 * @brief Test suite for Texture loading with stb_image
 */
class TextureTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a simple 2x2 PNG test image programmatically
        // This is a minimal valid PNG with a 2x2 red square
        createTestPNG();
    }

    void TearDown() override
    {
        // Clean up test file
        std::remove("test_texture.png");
    }

    void createTestPNG()
    {
        // Minimal 2x2 red PNG image (base64 decoded binary)
        // This is a valid PNG file that can be loaded by stb_image
        const unsigned char pngData[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, // PNG signature
            0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, // IHDR chunk
            0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, // 2x2 dimensions
            0x08, 0x02, 0x00, 0x00, 0x00, 0xFD, 0xD4, 0x9A,
            0x73, 0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41, // IDAT chunk
            0x54, 0x08, 0xD7, 0x63, 0xF8, 0xCF, 0xC0, 0x00,
            0x00, 0x03, 0x01, 0x01, 0x00, 0x18, 0xDD, 0x8D,
            0xB4, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, // IEND chunk
            0x44, 0xAE, 0x42, 0x60, 0x82
        };

        std::ofstream file("test_texture.png", std::ios::binary);
        file.write(reinterpret_cast<const char*>(pngData), sizeof(pngData));
        file.close();
    }
};

TEST_F(TextureTest, LoadFromValidPNG)
{
    Texture texture;
    
    // Load the test PNG
    bool loaded = texture.loadFromFile("test_texture.png", false);
    
    // Verify loading succeeded
    EXPECT_TRUE(loaded);
    
    // Verify dimensions
    EXPECT_EQ(texture.getWidth(), 2);
    EXPECT_EQ(texture.getHeight(), 2);
    
    // Verify format (we request RGBA)
    EXPECT_EQ(texture.getFormat(), TextureFormat::RGBA8);
    
    // Verify texture is marked as valid
    EXPECT_TRUE(texture.isValid());
    
    // Verify path is stored
    EXPECT_EQ(texture.getPath(), "test_texture.png");
}

TEST_F(TextureTest, LoadFromInvalidFile)
{
    Texture texture;
    
    // Try to load non-existent file
    bool loaded = texture.loadFromFile("nonexistent.png", false);
    
    // Verify loading failed
    EXPECT_FALSE(loaded);
    
    // Verify texture is not valid
    EXPECT_FALSE(texture.isValid());
}

TEST_F(TextureTest, LoadFromInvalidPath)
{
    Texture texture;
    
    // Try to load with empty path
    bool loaded = texture.loadFromFile("", false);
    
    // Verify loading failed
    EXPECT_FALSE(loaded);
}
