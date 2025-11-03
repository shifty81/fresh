/**
 * @file texture_loading_example.cpp
 * @brief Example demonstrating how to load and use voxel textures
 * 
 * This example shows:
 * - Loading voxel block textures
 * - Loading UI textures
 * - Using the VoxelTextureLoader helper
 * - Handling face-specific textures (grass blocks)
 */

#include "renderer/TextureManager.h"
#include "renderer/VoxelTextureLoader.h"
#include "voxel/VoxelTypes.h"
#include "core/Logger.h"

#include <iostream>

using namespace fresh;

/**
 * @brief Initialize and load all game textures
 */
bool initializeTextures() {
    std::cout << "=== Fresh Voxel Engine - Texture Loading Example ===" << std::endl;
    
    // Step 1: Initialize the texture manager
    auto& texManager = TextureManager::getInstance();
    texManager.createDefaultTextures();
    std::cout << "Created default textures (white, black, normal)" << std::endl;
    
    // Step 2: Create voxel texture loader
    // Start with low resolution (16x16) for best performance
    VoxelTextureLoader textureLoader(
        VoxelTextureLoader::TextureResolution::Low,
        "textures/blocks"
    );
    
    std::cout << "\nLoading voxel block textures..." << std::endl;
    
    // Step 3: Load all voxel textures
    if (!textureLoader.loadAllTextures()) {
        std::cerr << "ERROR: Failed to load some voxel textures!" << std::endl;
        return false;
    }
    
    std::cout << "✓ All voxel textures loaded successfully" << std::endl;
    
    // Step 4: Load UI textures
    std::cout << "\nLoading UI textures..." << std::endl;
    
    auto crosshair = texManager.loadTexture("textures/ui/crosshair.png");
    if (!crosshair) {
        std::cerr << "WARNING: Crosshair texture not loaded" << std::endl;
    } else {
        std::cout << "✓ Crosshair loaded" << std::endl;
    }
    
    auto hotbarSlot = texManager.loadTexture("textures/ui/hotbar_slot.png");
    if (hotbarSlot) {
        std::cout << "✓ Hotbar slot loaded" << std::endl;
    }
    
    auto hotbarSelected = texManager.loadTexture("textures/ui/hotbar_slot_selected.png");
    if (hotbarSelected) {
        std::cout << "✓ Hotbar selected slot loaded" << std::endl;
    }
    
    auto heartFull = texManager.loadTexture("textures/ui/health_heart_full.png");
    auto heartEmpty = texManager.loadTexture("textures/ui/health_heart_empty.png");
    if (heartFull && heartEmpty) {
        std::cout << "✓ Health indicators loaded" << std::endl;
    }
    
    // Step 5: Demonstrate texture access
    std::cout << "\n=== Testing Texture Access ===" << std::endl;
    
    // Simple blocks (same texture all faces)
    auto stoneTexture = textureLoader.getTexture(VoxelType::Stone);
    if (stoneTexture) {
        std::cout << "✓ Stone texture: " << stoneTexture->getWidth() << "x" 
                  << stoneTexture->getHeight() << " pixels" << std::endl;
    }
    
    auto dirtTexture = textureLoader.getTexture(VoxelType::Dirt);
    if (dirtTexture) {
        std::cout << "✓ Dirt texture accessible" << std::endl;
    }
    
    auto sandTexture = textureLoader.getTexture(VoxelType::Sand);
    if (sandTexture) {
        std::cout << "✓ Sand texture accessible" << std::endl;
    }
    
    // Face-specific block (grass)
    std::cout << "\n--- Grass Block (Face-Specific Textures) ---" << std::endl;
    
    if (textureLoader.hasFaceSpecificTextures(VoxelType::Grass)) {
        std::cout << "Grass block has different textures per face:" << std::endl;
        
        auto grassTop = textureLoader.getTexture(VoxelType::Grass, 
                                                 VoxelTextureLoader::BlockFace::Top);
        if (grassTop) {
            std::cout << "  ✓ Top face: grass_top texture" << std::endl;
        }
        
        auto grassSide = textureLoader.getTexture(VoxelType::Grass, 
                                                  VoxelTextureLoader::BlockFace::North);
        if (grassSide) {
            std::cout << "  ✓ Side faces: grass_side texture" << std::endl;
        }
        
        auto grassBottom = textureLoader.getTexture(VoxelType::Grass, 
                                                    VoxelTextureLoader::BlockFace::Bottom);
        if (grassBottom) {
            std::cout << "  ✓ Bottom face: dirt texture" << std::endl;
        }
    }
    
    // Transparent blocks
    std::cout << "\n--- Transparent Blocks ---" << std::endl;
    
    auto waterTexture = textureLoader.getTexture(VoxelType::Water);
    if (waterTexture) {
        std::cout << "✓ Water texture (transparent)" << std::endl;
    }
    
    auto leavesTexture = textureLoader.getTexture(VoxelType::Leaves);
    if (leavesTexture) {
        std::cout << "✓ Leaves texture (transparent)" << std::endl;
    }
    
    // Natural materials
    std::cout << "\n--- Natural Materials ---" << std::endl;
    
    auto woodTexture = textureLoader.getTexture(VoxelType::Wood);
    if (woodTexture) {
        std::cout << "✓ Wood texture" << std::endl;
    }
    
    // Step 6: Show texture cache info
    std::cout << "\n=== Texture Cache Statistics ===" << std::endl;
    std::cout << "Cached textures: " << texManager.getCacheSize() << std::endl;
    std::cout << "Memory usage: " << (texManager.getMemoryUsage() / 1024) << " KB" << std::endl;
    
    // Step 7: Demonstrate resolution switching
    std::cout << "\n=== Testing Resolution Switching ===" << std::endl;
    std::cout << "Current resolution: " << static_cast<int>(textureLoader.getResolution()) << "x"
              << static_cast<int>(textureLoader.getResolution()) << std::endl;
    
    std::cout << "Switching to medium resolution (32x32)..." << std::endl;
    textureLoader.setResolution(VoxelTextureLoader::TextureResolution::Medium);
    
    if (textureLoader.loadAllTextures()) {
        std::cout << "✓ Reloaded textures at 32x32 resolution" << std::endl;
        
        auto stoneTexture32 = textureLoader.getTexture(VoxelType::Stone);
        if (stoneTexture32) {
            std::cout << "  Stone texture now: " << stoneTexture32->getWidth() << "x" 
                      << stoneTexture32->getHeight() << " pixels" << std::endl;
        }
    }
    
    std::cout << "\n=== Texture System Ready ===" << std::endl;
    std::cout << "All textures loaded and ready for rendering!" << std::endl;
    
    return true;
}

/**
 * @brief Example of using textures in a render loop
 */
void renderExample(VoxelTextureLoader& textureLoader) {
    std::cout << "\n=== Pseudo Render Loop Example ===" << std::endl;
    
    // Example: Render a stone block
    VoxelType blockType = VoxelType::Stone;
    
    std::cout << "Rendering " << static_cast<int>(blockType) << " block..." << std::endl;
    
    // Get texture for this voxel type
    auto texture = textureLoader.getTexture(blockType);
    
    if (texture && texture->isValid()) {
        // In a real render loop, you would:
        // 1. Bind the texture
        texture->bind(0);  // Bind to texture unit 0
        
        // 2. Draw the voxel mesh
        // ... OpenGL/DirectX draw calls ...
        std::cout << "  - Texture bound to unit 0" << std::endl;
        std::cout << "  - Drawing voxel mesh..." << std::endl;
        
        // 3. Unbind texture
        texture->unbind();
        std::cout << "  - Texture unbound" << std::endl;
    }
    
    // Example: Render grass block with face-specific textures
    blockType = VoxelType::Grass;
    std::cout << "\nRendering grass block with face-specific textures..." << std::endl;
    
    if (textureLoader.hasFaceSpecificTextures(blockType)) {
        // Top face
        auto topTex = textureLoader.getTexture(blockType, VoxelTextureLoader::BlockFace::Top);
        if (topTex) {
            topTex->bind(0);
            std::cout << "  - Drawing top face (grass)" << std::endl;
            topTex->unbind();
        }
        
        // Side faces
        auto sideTex = textureLoader.getTexture(blockType, VoxelTextureLoader::BlockFace::North);
        if (sideTex) {
            sideTex->bind(0);
            std::cout << "  - Drawing side faces (grass/dirt)" << std::endl;
            sideTex->unbind();
        }
        
        // Bottom face
        auto bottomTex = textureLoader.getTexture(blockType, VoxelTextureLoader::BlockFace::Bottom);
        if (bottomTex) {
            bottomTex->bind(0);
            std::cout << "  - Drawing bottom face (dirt)" << std::endl;
            bottomTex->unbind();
        }
    }
}

/**
 * @brief Main example function
 */
int main() {
    // Initialize logger
    Logger::getInstance().initialize();
    
    try {
        // Initialize textures
        if (!initializeTextures()) {
            std::cerr << "Failed to initialize textures" << std::endl;
            Logger::getInstance().shutdown();
            return 1;
        }
        
        // Create texture loader for render examples
        VoxelTextureLoader textureLoader(VoxelTextureLoader::TextureResolution::Low);
        textureLoader.loadAllTextures();
        
        // Run render example
        renderExample(textureLoader);
        
        std::cout << "\n=== Example Complete ===" << std::endl;
        std::cout << "Textures are ready to be integrated into the game!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        Logger::getInstance().shutdown();
        return 1;
    }
    
    // Cleanup
    Logger::getInstance().shutdown();
    return 0;
}
