/**
 * @file raycasting.cpp
 * @brief Demonstrates raycasting for voxel interaction and mouse picking
 * 
 * This example shows:
 * - Casting rays through the voxel world
 * - Finding intersections with voxels
 * - Getting the hit position and normal
 * - Mouse-based voxel selection
 * - Block placement using raycasting
 */

#include <iostream>
#include <memory>
#include "interaction/RaycastSystem.h"
#include "voxel/VoxelWorld.h"
#include "gameplay/Camera.h"
#include <glm/glm.hpp>

int main() {
    std::cout << "=== Fresh Voxel Engine: Raycasting ===" << std::endl;
    std::cout << "This example demonstrates raycasting for voxel interaction." << std::endl;
    
    // Step 1: Setup world with some voxels
    std::cout << "\n[1/5] Creating test world..." << std::endl;
    VoxelWorld world;
    
    // Create a chunk with a simple structure
    auto chunk = std::make_unique<Chunk>(0, 0);
    
    // Create a floor
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            chunk->setVoxel(x, 60, z, VoxelType::Stone);
        }
    }
    
    // Create a wall
    for (int y = 61; y <= 65; y++) {
        for (int x = 5; x <= 10; x++) {
            chunk->setVoxel(x, y, 8, VoxelType::Stone);
        }
    }
    
    // Create a tower
    for (int y = 61; y <= 70; y++) {
        chunk->setVoxel(12, y, 12, VoxelType::Diamond);
    }
    
    world.addChunk(std::move(chunk));
    std::cout << "    ✓ Created world with floor, wall, and tower" << std::endl;
    
    // Step 2: Setup camera and raycast system
    std::cout << "\n[2/5] Initializing camera and raycast system..." << std::endl;
    Camera camera;
    camera.setPosition(glm::vec3(8.0f, 65.0f, 5.0f));
    
    RaycastSystem raycast;
    std::cout << "    ✓ Camera at (8, 65, 5)" << std::endl;
    std::cout << "    ✓ Raycast system ready" << std::endl;
    
    // Step 3: Cast ray forward from camera
    std::cout << "\n[3/5] Casting ray forward..." << std::endl;
    glm::vec3 rayOrigin = camera.getPosition();
    glm::vec3 rayDirection = camera.getForward();
    float maxDistance = 100.0f;
    
    RaycastResult result = raycast.castRay(world, rayOrigin, rayDirection, maxDistance);
    
    if (result.hit) {
        std::cout << "    ✓ Ray hit voxel!" << std::endl;
        std::cout << "      Position: (" << result.position.x << ", " 
                  << result.position.y << ", " << result.position.z << ")" << std::endl;
        std::cout << "      Normal: (" << result.normal.x << ", "
                  << result.normal.y << ", " << result.normal.z << ")" << std::endl;
        std::cout << "      Distance: " << result.distance << " blocks" << std::endl;
        std::cout << "      Voxel type: " << static_cast<int>(result.voxelType) << std::endl;
    } else {
        std::cout << "    ✗ Ray hit nothing (looking at sky)" << std::endl;
    }
    
    // Step 4: Cast ray downward (should hit floor)
    std::cout << "\n[4/5] Casting ray downward..." << std::endl;
    rayDirection = glm::vec3(0.0f, -1.0f, 0.0f); // Straight down
    result = raycast.castRay(world, rayOrigin, rayDirection, maxDistance);
    
    if (result.hit) {
        std::cout << "    ✓ Ray hit floor!" << std::endl;
        std::cout << "      Position: (" << result.position.x << ", " 
                  << result.position.y << ", " << result.position.z << ")" << std::endl;
        std::cout << "      Distance: " << result.distance << " blocks" << std::endl;
        
        // The normal should be pointing up (0, 1, 0)
        std::cout << "      Normal: (" << result.normal.x << ", "
                  << result.normal.y << ", " << result.normal.z << ") (pointing up)" << std::endl;
    }
    
    // Step 5: Demonstrate block placement using raycast
    std::cout << "\n[5/5] Block placement using raycast..." << std::endl;
    
    // Cast ray from a position looking at the wall
    camera.setPosition(glm::vec3(8.0f, 63.0f, 5.0f));
    camera.lookAt(glm::vec3(8.0f, 63.0f, 8.0f)); // Look at wall
    
    rayOrigin = camera.getPosition();
    rayDirection = camera.getForward();
    result = raycast.castRay(world, rayOrigin, rayDirection, maxDistance);
    
    if (result.hit) {
        std::cout << "    ✓ Found target block at wall" << std::endl;
        
        // Calculate placement position (hit position + normal)
        glm::ivec3 placementPos = result.position + result.normal;
        
        std::cout << "    Placement position: (" << placementPos.x << ", "
                  << placementPos.y << ", " << placementPos.z << ")" << std::endl;
        
        // Place a new block
        world.setVoxel(placementPos.x, placementPos.y, placementPos.z, VoxelType::Wood);
        std::cout << "    ✓ Placed Wood block" << std::endl;
        
        // Verify placement
        VoxelType placed = world.getVoxel(placementPos.x, placementPos.y, placementPos.z);
        std::cout << "    Verified: Block type = " << static_cast<int>(placed) << " (Wood)" << std::endl;
    }
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Casting rays through the voxel world" << std::endl;
    std::cout << "  • Detecting ray-voxel intersections" << std::endl;
    std::cout << "  • Getting hit position and surface normal" << std::endl;
    std::cout << "  • Using raycasting for block placement" << std::endl;
    std::cout << "  • Understanding raycast results" << std::endl;
    std::cout << "\nCommon Use Cases:" << std::endl;
    std::cout << "  • Mouse picking (click to select)" << std::endl;
    std::cout << "  • Block placement/removal" << std::endl;
    std::cout << "  • Line of sight checks" << std::endl;
    std::cout << "  • Projectile collision" << std::endl;
    std::cout << "  • Interaction distance checking" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try collision_detection.cpp for physics" << std::endl;
    std::cout << "  • Integrate with player_controller.cpp for interaction" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Raycasting
 *    - Cast a ray from origin in direction
 *    - Check for intersections with voxels
 *    - Returns first intersection (if any)
 *    - DDA algorithm for voxel traversal
 * 
 * 2. Ray Definition
 *    - Origin: Starting point (vec3)
 *    - Direction: Unit vector (normalized)
 *    - Max distance: How far to check
 *    - Efficiently steps through voxel grid
 * 
 * 3. RaycastResult
 *    - hit: Whether ray hit anything
 *    - position: World position of hit voxel
 *    - normal: Surface normal (which face was hit)
 *    - distance: Distance from origin
 *    - voxelType: Type of voxel that was hit
 * 
 * 4. Surface Normals
 *    - (0, 1, 0): Top face
 *    - (0, -1, 0): Bottom face
 *    - (1, 0, 0): Right face (+X)
 *    - (-1, 0, 0): Left face (-X)
 *    - (0, 0, 1): Front face (+Z)
 *    - (0, 0, -1): Back face (-Z)
 * 
 * 5. DDA Algorithm
 *    - Digital Differential Analyzer
 *    - Steps through voxel grid efficiently
 *    - Only checks voxels along ray path
 *    - Much faster than checking all voxels
 * 
 * MOUSE PICKING PATTERN:
 * 
 * // In game loop, on mouse click
 * glm::vec3 rayOrigin = camera.getPosition();
 * glm::vec3 rayDir = camera.getForward();
 * 
 * RaycastResult result = raycast.castRay(world, rayOrigin, rayDir, 100.0f);
 * 
 * if (result.hit) {
 *     // Left click = break block
 *     if (leftClick) {
 *         world.setVoxel(result.position, VoxelType::Air);
 *     }
 *     
 *     // Right click = place block
 *     if (rightClick) {
 *         glm::ivec3 placePos = result.position + result.normal;
 *         world.setVoxel(placePos, selectedVoxelType);
 *     }
 * }
 * 
 * INTERACTION RANGE CHECK:
 * 
 * if (result.hit && result.distance <= playerReach) {
 *     // Allow interaction
 * } else {
 *     // Too far away
 * }
 * 
 * LINE OF SIGHT:
 * 
 * bool hasLineOfSight(vec3 from, vec3 to) {
 *     vec3 direction = normalize(to - from);
 *     float distance = length(to - from);
 *     
 *     RaycastResult result = raycast.castRay(world, from, direction, distance);
 *     return !result.hit; // True if no obstruction
 * }
 * 
 * PERFORMANCE TIPS:
 * 
 * • Limit max distance to reasonable range (50-100 blocks)
 * • Cache results when ray doesn't change
 * • Use early exit when hit found
 * • DDA is already very efficient
 * 
 * COMMON PATTERNS:
 * 
 * 1. Hover highlight:
 *    - Cast ray every frame
 *    - Highlight result.position
 *    - Show placement preview at result.position + result.normal
 * 
 * 2. Mining/Building:
 *    - Cast ray on click
 *    - Check distance limit
 *    - Modify voxel at hit or placement position
 * 
 * 3. AI targeting:
 *    - Cast ray from NPC to player
 *    - Check if result.hit
 *    - If no hit, NPC can see player
 * 
 * DEBUGGING:
 * 
 * • Visualize ray with debug renderer
 * • Draw hit position as sphere
 * • Draw normal as arrow
 * • Log ray origin, direction, result
 */
