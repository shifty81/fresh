/**
 * @file 04_create_world.cpp
 * @brief Create and display a simple voxel world
 * 
 * This example demonstrates:
 * - Creating a voxel world
 * - Generating chunks
 * - Basic rendering
 * - Simple voxel placement
 * 
 * This is the foundation for all voxel-based gameplay.
 */

#include "core/Engine.h"
#include "core/Logger.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"
#include <iostream>

int main() {
    std::cout << "Fresh Voxel Engine - Create World Example\n";
    std::cout << "==========================================\n\n";
    
    // Initialize
    fresh::Logger::getInstance().initialize();
    fresh::Engine engine;
    
    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine");
        return 1;
    }
    
    std::cout << "Creating voxel world...\n";
    
    // Create a voxel world
    fresh::VoxelWorld world;
    
    // Create a simple flat world with a platform
    std::cout << "Generating chunks...\n";
    
    // Generate a 3x3 grid of chunks (48x48 blocks)
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            // Create ground layer at y=0
            for (int bx = 0; bx < 16; ++bx) {
                for (int bz = 0; bz < 16; ++bz) {
                    // Bottom layer - stone
                    world.setVoxel(
                        x * 16 + bx,
                        0,
                        z * 16 + bz,
                        fresh::VoxelType::Stone
                    );
                    
                    // Top layer - grass
                    world.setVoxel(
                        x * 16 + bx,
                        1,
                        z * 16 + bz,
                        fresh::VoxelType::Grass
                    );
                }
            }
        }
    }
    
    // Add a small house made of stone
    std::cout << "Building a small structure...\n";
    for (int y = 2; y < 6; ++y) {
        for (int x = -2; x <= 2; ++x) {
            world.setVoxel(x, y, -2, fresh::VoxelType::Stone);  // Front wall
            world.setVoxel(x, y,  2, fresh::VoxelType::Stone);  // Back wall
        }
        for (int z = -2; z <= 2; ++z) {
            world.setVoxel(-2, y, z, fresh::VoxelType::Stone);  // Left wall
            world.setVoxel( 2, y, z, fresh::VoxelType::Stone);  // Right wall
        }
    }
    
    // Add a roof
    for (int x = -2; x <= 2; ++x) {
        for (int z = -2; z <= 2; ++z) {
            world.setVoxel(x, 6, z, fresh::VoxelType::Wood);
        }
    }
    
    std::cout << "✓ World created with " << world.getChunkCount() << " chunks\n";
    std::cout << "✓ Platform: 48x48 blocks\n";
    std::cout << "✓ Structure: 5x5x4 house\n\n";
    
    LOG_INFO("Voxel world created successfully");
    
    std::cout << "Press ESC to exit...\n";
    std::cout << "Use WASD to move, Mouse to look around\n\n";
    
    // Run the engine (this would display the world)
    // In a real application, this would start the game loop
    // For this example, we just verify creation
    
    std::cout << "World Statistics:\n";
    std::cout << "  Chunks: " << world.getChunkCount() << "\n";
    std::cout << "  Active Voxels: ~" << (48 * 48 * 2 + 100) << "\n";
    std::cout << "  Memory: ~" << (world.getChunkCount() * sizeof(fresh::Chunk)) << " bytes\n\n";
    
    // Shutdown
    engine.shutdown();
    fresh::Logger::getInstance().shutdown();
    
    std::cout << "==========================================\n";
    std::cout << "World creation complete!\n\n";
    std::cout << "Next steps:\n";
    std::cout << "  • Try 05_terrain_generation.cpp for procedural worlds\n";
    std::cout << "  • Try 07_player_movement.cpp to explore the world\n";
    std::cout << "  • Try 08_place_remove_blocks.cpp to modify the world\n";
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Voxel World
 *    - World is divided into 16x256x16 chunks
 *    - Each chunk contains up to 65,536 voxels
 *    - Chunks are generated/loaded as needed
 * 
 * 2. Voxel Types
 *    Air, Stone, Dirt, Grass, Sand, Water, Wood, Leaves, etc.
 *    Each type has different properties and appearance
 * 
 * 3. Coordinates
 *    - World coordinates are (x, y, z)
 *    - Y is up (vertical axis)
 *    - Chunks are identified by chunk coordinates
 * 
 * 4. Mesh Generation
 *    - Chunks automatically generate meshes
 *    - Only visible faces are rendered
 *    - Greedy meshing optimizes performance
 * 
 * WORLD STRUCTURE:
 * 
 * Y
 * ↑   ┌─────┐
 * │   │Wood │  Roof (y=6)
 * │   ├─────┤
 * │   │Stone│  Walls (y=2-5)
 * │   ├─────┤
 * │   │Grass│  Ground (y=1)
 * │   │Stone│  Base (y=0)
 * └────────────→ X
 * 
 * PERFORMANCE TIPS:
 * 
 * • Only load chunks near the player
 * • Unload distant chunks to save memory
 * • Use greedy meshing for efficiency
 * • Cull faces between solid blocks
 */
