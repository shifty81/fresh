/**
 * @file voxel_interaction.cpp
 * @brief Demonstrates placing and removing voxels in the world
 * 
 * This example shows:
 * - Placing individual voxels
 * - Removing voxels
 * - World coordinate to chunk coordinate conversion
 * - Raycasting to find target voxels
 * - Basic voxel interaction patterns
 */

#include <iostream>
#include <memory>
#include "voxel/VoxelWorld.h"
#include "voxel/VoxelTypes.h"
#include "interaction/RaycastSystem.h"

/**
 * Helper function to convert world coordinates to chunk coordinates
 */
void worldToChunkCoords(int worldX, int worldY, int worldZ, 
                       int& chunkX, int& chunkZ, 
                       int& localX, int& localY, int& localZ) {
    chunkX = worldX >> 4; // Divide by 16
    chunkZ = worldZ >> 4;
    localX = worldX & 15; // Modulo 16
    localY = worldY;
    localZ = worldZ & 15;
}

int main() {
    std::cout << "=== Fresh Voxel Engine: Voxel Interaction ===" << std::endl;
    std::cout << "This example demonstrates placing and removing voxels." << std::endl;
    
    // Step 1: Create a world with initial terrain
    std::cout << "\n[1/5] Creating world with initial terrain..." << std::endl;
    VoxelWorld world;
    
    // Create a single chunk at origin
    auto chunk = std::make_unique<Chunk>(0, 0);
    
    // Fill with basic terrain (stone platform)
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y <= 5; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
        }
    }
    
    world.addChunk(std::move(chunk));
    std::cout << "    ✓ Created stone platform (16x6x16)" << std::endl;
    
    // Step 2: Place voxels at specific positions
    std::cout << "\n[2/5] Placing voxels..." << std::endl;
    
    // Place a single dirt block
    int worldX = 5, worldY = 6, worldZ = 5;
    world.setVoxel(worldX, worldY, worldZ, VoxelType::Dirt);
    std::cout << "    ✓ Placed Dirt at world position (" << worldX << ", " << worldY << ", " << worldZ << ")" << std::endl;
    
    // Build a small tower
    std::cout << "    Building a 5-block stone tower at (10, 6-10, 10)..." << std::endl;
    for (int y = 6; y <= 10; y++) {
        world.setVoxel(10, y, 10, VoxelType::Stone);
    }
    std::cout << "    ✓ Tower built" << std::endl;
    
    // Create a cross pattern
    std::cout << "    Creating cross pattern at (8, 6, 8)..." << std::endl;
    world.setVoxel(8, 6, 8, VoxelType::Diamond); // Center
    world.setVoxel(7, 6, 8, VoxelType::Gold);    // West
    world.setVoxel(9, 6, 8, VoxelType::Gold);    // East
    world.setVoxel(8, 6, 7, VoxelType::Gold);    // North
    world.setVoxel(8, 6, 9, VoxelType::Gold);    // South
    std::cout << "    ✓ Cross pattern created (Diamond center, Gold edges)" << std::endl;
    
    // Step 3: Query voxel data
    std::cout << "\n[3/5] Querying voxel data..." << std::endl;
    VoxelType voxel1 = world.getVoxel(5, 6, 5);
    VoxelType voxel2 = world.getVoxel(10, 8, 10);
    VoxelType voxel3 = world.getVoxel(8, 6, 8);
    
    std::cout << "    Voxel at (5, 6, 5): " << static_cast<int>(voxel1) << " (Dirt)" << std::endl;
    std::cout << "    Voxel at (10, 8, 10): " << static_cast<int>(voxel2) << " (Stone)" << std::endl;
    std::cout << "    Voxel at (8, 6, 8): " << static_cast<int>(voxel3) << " (Diamond)" << std::endl;
    
    // Step 4: Remove voxels
    std::cout << "\n[4/5] Removing voxels..." << std::endl;
    
    // Remove the dirt block
    world.setVoxel(5, 6, 5, VoxelType::Air);
    std::cout << "    ✓ Removed dirt block at (5, 6, 5)" << std::endl;
    
    // Remove middle of tower
    world.setVoxel(10, 8, 10, VoxelType::Air);
    std::cout << "    ✓ Removed stone block from tower (created gap)" << std::endl;
    
    // Verify removal
    VoxelType removed1 = world.getVoxel(5, 6, 5);
    VoxelType removed2 = world.getVoxel(10, 8, 10);
    std::cout << "    Verified: (5,6,5) = " << static_cast<int>(removed1) << " (Air)" << std::endl;
    std::cout << "    Verified: (10,8,10) = " << static_cast<int>(removed2) << " (Air)" << std::endl;
    
    // Step 5: Coordinate conversion demonstration
    std::cout << "\n[5/5] Coordinate conversion..." << std::endl;
    
    int testWorldX = 25, testWorldZ = -10;
    int chunkX, chunkZ, localX, localY, localZ;
    worldToChunkCoords(testWorldX, 64, testWorldZ, chunkX, chunkZ, localX, localY, localZ);
    
    std::cout << "    World position: (" << testWorldX << ", 64, " << testWorldZ << ")" << std::endl;
    std::cout << "    Chunk: (" << chunkX << ", " << chunkZ << ")" << std::endl;
    std::cout << "    Local: (" << localX << ", " << localY << ", " << localZ << ")" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • How to place voxels at world positions" << std::endl;
    std::cout << "  • How to remove voxels (set to Air)" << std::endl;
    std::cout << "  • How to query voxel types" << std::endl;
    std::cout << "  • Converting between world and chunk coordinates" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try terraforming_tools.cpp for advanced editing" << std::endl;
    std::cout << "  • See raycasting.cpp for mouse-based interaction" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. World Coordinates
 *    - Global position in the infinite world
 *    - Can be negative
 *    - X: west(-) to east(+)
 *    - Y: down(0) to up(255)
 *    - Z: north(-) to south(+)
 * 
 * 2. Chunk Coordinates
 *    - Which chunk contains a world position
 *    - chunkX = worldX / 16 (or worldX >> 4)
 *    - chunkZ = worldZ / 16
 *    - Important: handles negatives correctly with >> operator
 * 
 * 3. Local Coordinates
 *    - Position within a chunk (0-15 for X/Z, 0-255 for Y)
 *    - localX = worldX % 16 (or worldX & 15)
 *    - localZ = worldZ % 16
 * 
 * 4. Placing Voxels
 *    - Use VoxelWorld::setVoxel(worldX, worldY, worldZ, type)
 *    - Automatically finds correct chunk
 *    - Creates chunk if it doesn't exist
 *    - Marks chunk as dirty for mesh regeneration
 * 
 * 5. Removing Voxels
 *    - Set voxel type to VoxelType::Air
 *    - Air is transparent and non-solid
 *    - Removing adjacent voxels exposes new faces
 * 
 * INTERACTION PATTERNS:
 * 
 * 1. Click to Place (in game loop):
 *    if (mouse.leftClick) {
 *        auto hit = raycast(camera.position, camera.forward);
 *        if (hit.hasHit) {
 *            world.setVoxel(hit.position + hit.normal, selectedType);
 *        }
 *    }
 * 
 * 2. Click to Remove:
 *    if (mouse.rightClick) {
 *        auto hit = raycast(camera.position, camera.forward);
 *        if (hit.hasHit) {
 *            world.setVoxel(hit.position, VoxelType::Air);
 *        }
 *    }
 * 
 * 3. Range Check:
 *    if (playerDist(position) < maxReach) {
 *        world.setVoxel(position, type);
 *    }
 * 
 * PERFORMANCE TIPS:
 * 
 * • Batch multiple voxel changes together
 * • Regenerate chunk mesh after all changes
 * • Use local coordinates when modifying many voxels in same chunk
 * • Consider undo/redo system for player edits
 * 
 * ERROR HANDLING:
 * 
 * • Check if Y coordinate is valid (0-255)
 * • Verify chunk exists before modification
 * • Handle edge cases at chunk boundaries
 * • Validate voxel type is in valid range
 */
