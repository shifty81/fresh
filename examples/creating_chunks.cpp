/**
 * @file creating_chunks.cpp
 * @brief Demonstrates how to create and manage voxel chunks
 * 
 * This example shows:
 * - Creating individual chunks
 * - Setting voxel types in chunks
 * - Understanding chunk coordinates
 * - Basic chunk management
 * 
 * Chunks are the fundamental building blocks of the voxel world.
 * Each chunk is 16x256x16 voxels.
 */

#include <iostream>
#include <memory>
#include "voxel/Chunk.h"
#include "voxel/VoxelWorld.h"
#include "voxel/VoxelTypes.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Creating Chunks ===" << std::endl;
    std::cout << "This example demonstrates chunk creation and manipulation." << std::endl;
    
    // Step 1: Understand chunk dimensions
    std::cout << "\n[1/5] Understanding chunk dimensions..." << std::endl;
    std::cout << "    Chunk size: " << CHUNK_SIZE << "x" << CHUNK_HEIGHT << "x" << CHUNK_SIZE << std::endl;
    std::cout << "    Total voxels per chunk: " << (CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE) << std::endl;
    
    // Step 2: Create a single chunk
    std::cout << "\n[2/5] Creating a single chunk..." << std::endl;
    int chunkX = 0;
    int chunkZ = 0;
    auto chunk = std::make_unique<Chunk>(chunkX, chunkZ);
    std::cout << "    ✓ Chunk created at world position (" << chunkX << ", " << chunkZ << ")" << std::endl;
    
    // Step 3: Fill chunk with voxels
    std::cout << "\n[3/5] Filling chunk with voxels..." << std::endl;
    
    // Create a simple layered terrain
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            // Bedrock layer at bottom
            chunk->setVoxel(x, 0, z, VoxelType::Bedrock);
            
            // Stone layers (1-50)
            for (int y = 1; y <= 50; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
            
            // Dirt layers (51-62)
            for (int y = 51; y <= 62; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Dirt);
            }
            
            // Grass on top (63)
            chunk->setVoxel(x, 63, z, VoxelType::Grass);
            
            // Air above (64-255)
            for (int y = 64; y < CHUNK_HEIGHT; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Air);
            }
        }
    }
    std::cout << "    ✓ Chunk filled with layered terrain" << std::endl;
    std::cout << "    Layers: Bedrock(0) → Stone(1-50) → Dirt(51-62) → Grass(63) → Air(64+)" << std::endl;
    
    // Step 4: Read voxel data
    std::cout << "\n[4/5] Reading voxel data..." << std::endl;
    VoxelType centerVoxel = chunk->getVoxel(8, 63, 8);
    std::cout << "    Voxel at center top (8, 63, 8): " 
              << static_cast<int>(centerVoxel) << " (Grass)" << std::endl;
    
    VoxelType bottomVoxel = chunk->getVoxel(8, 0, 8);
    std::cout << "    Voxel at center bottom (8, 0, 8): " 
              << static_cast<int>(bottomVoxel) << " (Bedrock)" << std::endl;
    
    // Step 5: Create a VoxelWorld with multiple chunks
    std::cout << "\n[5/5] Creating a world with multiple chunks..." << std::endl;
    VoxelWorld world;
    
    // Create a 3x3 grid of chunks
    for (int cx = -1; cx <= 1; cx++) {
        for (int cz = -1; cz <= 1; cz++) {
            auto newChunk = std::make_unique<Chunk>(cx, cz);
            
            // Fill with simple pattern
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    newChunk->setVoxel(x, 0, z, VoxelType::Stone);
                    newChunk->setVoxel(x, 1, z, VoxelType::Grass);
                }
            }
            
            world.addChunk(std::move(newChunk));
        }
    }
    
    std::cout << "    ✓ Created 3x3 chunk grid (9 chunks total)" << std::endl;
    std::cout << "    World coverage: 48x48 blocks" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • How to create individual chunks" << std::endl;
    std::cout << "  • How to set and get voxel data" << std::endl;
    std::cout << "  • Understanding chunk coordinates" << std::endl;
    std::cout << "  • Creating multi-chunk worlds" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try voxel_interaction.cpp to modify voxels" << std::endl;
    std::cout << "  • See terrain_generation.cpp for procedural generation" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Chunk Coordinates
 *    - Chunks use integer coordinates (chunkX, chunkZ)
 *    - Each chunk represents a 16x256x16 region in world space
 *    - World position = chunkCoord * CHUNK_SIZE
 *    - Example: Chunk (2, -1) covers world X [32-47], Z [-16 to -1]
 * 
 * 2. Voxel Coordinates
 *    - Voxels use local coordinates within the chunk (0-15, 0-255, 0-15)
 *    - Local coords are relative to chunk origin
 *    - To get world position: worldX = chunkX * CHUNK_SIZE + localX
 * 
 * 3. Voxel Types
 *    - VoxelType is an enum defining all block types
 *    - Air (0) = empty space, transparent
 *    - Solid types (Stone, Dirt, etc.) = opaque blocks
 *    - See VoxelTypes.h for complete list
 * 
 * 4. Chunk Memory
 *    - Each chunk stores 65,536 voxels (16 * 256 * 16)
 *    - Uses efficient data structure (typically array or map)
 *    - Empty chunks can be optimized (all air)
 * 
 * 5. VoxelWorld Management
 *    - VoxelWorld manages multiple chunks
 *    - Provides unified interface to query across chunks
 *    - Handles chunk loading/unloading
 * 
 * PERFORMANCE TIPS:
 * 
 * • Use setVoxel sparingly in hot loops
 * • Batch voxel changes when possible
 * • Consider chunk dirty flags for mesh updates
 * • Use empty chunk optimization for all-air chunks
 * 
 * COMMON PATTERNS:
 * 
 * 1. Creating flat terrain:
 *    for (y = 0; y < height; y++)
 *        setVoxel(x, y, z, Stone);
 * 
 * 2. Creating hollow structures:
 *    if (x == 0 || x == 15 || z == 0 || z == 15)
 *        setVoxel(x, y, z, Stone);
 * 
 * 3. Layer-based generation:
 *    for each layer type:
 *        fill range [startY, endY] with type
 */
