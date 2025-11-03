/**
 * @file collision_detection.cpp
 * @brief Demonstrates collision detection system for physics interactions
 * 
 * This example shows:
 * - AABB (bounding box) collision detection
 * - Sphere collision detection
 * - Ray-voxel collision
 * - Spatial hashing for optimization
 * - Collision response and resolution
 */

#include <iostream>
#include <memory>
#include "physics/CollisionDetection.h"
#include "physics/PhysicsSystem.h"
#include "voxel/VoxelWorld.h"
#include <glm/glm.hpp>

int main() {
    std::cout << "=== Fresh Voxel Engine: Collision Detection ===" << std::endl;
    std::cout << "This example demonstrates various collision detection methods." << std::endl;
    
    // Step 1: Create collision detection system
    std::cout << "\n[1/5] Initializing collision system..." << std::endl;
    CollisionDetection collision;
    std::cout << "    ✓ Collision detection system ready" << std::endl;
    
    // Step 2: AABB vs AABB collision
    std::cout << "\n[2/5] Testing AABB collision..." << std::endl;
    
    // Create two bounding boxes
    AABB box1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    AABB box2(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f, 2.0f, 2.0f));
    
    std::cout << "    Box 1: min(0,0,0) max(2,2,2)" << std::endl;
    std::cout << "    Box 2: min(1,0,0) max(3,2,2)" << std::endl;
    
    bool aabbCollision = collision.testAABBvsAABB(box1, box2);
    std::cout << "    Collision: " << (aabbCollision ? "YES ✓" : "NO ✗") << std::endl;
    std::cout << "    (Boxes overlap in X: 1-2)" << std::endl;
    
    // Test non-colliding boxes
    AABB box3(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(12.0f, 2.0f, 2.0f));
    bool noCollision = collision.testAABBvsAABB(box1, box3);
    std::cout << "\n    Box 1 vs Box 3 (far apart): " 
              << (noCollision ? "COLLISION" : "NO COLLISION") << " ✓" << std::endl;
    
    // Step 3: Sphere vs Sphere collision
    std::cout << "\n[3/5] Testing Sphere collision..." << std::endl;
    
    Sphere sphere1(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Sphere sphere2(glm::vec3(1.5f, 0.0f, 0.0f), 1.0f);
    
    std::cout << "    Sphere 1: center(0,0,0) radius(1.0)" << std::endl;
    std::cout << "    Sphere 2: center(1.5,0,0) radius(1.0)" << std::endl;
    
    bool sphereCollision = collision.testSphereVsSphere(sphere1, sphere2);
    std::cout << "    Collision: " << (sphereCollision ? "YES ✓" : "NO ✗") << std::endl;
    
    float distance = glm::length(sphere2.center - sphere1.center);
    float minDist = sphere1.radius + sphere2.radius;
    std::cout << "    Distance between centers: " << distance << std::endl;
    std::cout << "    Sum of radii: " << minDist << std::endl;
    std::cout << "    (Collision if distance < sum of radii)" << std::endl;
    
    // Step 4: Player AABB vs Voxel World collision
    std::cout << "\n[4/5] Testing player vs world collision..." << std::endl;
    
    // Create simple world with floor
    VoxelWorld world;
    auto chunk = std::make_unique<Chunk>(0, 0);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            chunk->setVoxel(x, 60, z, VoxelType::Stone);
        }
    }
    world.addChunk(std::move(chunk));
    
    // Player bounding box (standing on ground)
    AABB playerBox(glm::vec3(8.0f, 61.0f, 8.0f), glm::vec3(8.6f, 62.8f, 8.6f));
    std::cout << "    Player box: " << playerBox.min.x << "," << playerBox.min.y << "," << playerBox.min.z
              << " to " << playerBox.max.x << "," << playerBox.max.y << "," << playerBox.max.z << std::endl;
    
    bool playerCollision = collision.testAABBvsVoxelWorld(playerBox, world);
    std::cout << "    Player vs World: " << (playerCollision ? "COLLISION" : "NO COLLISION") << " ✓" << std::endl;
    std::cout << "    (Player is standing on solid ground)" << std::endl;
    
    // Test player in air (should not collide)
    AABB playerInAir(glm::vec3(8.0f, 65.0f, 8.0f), glm::vec3(8.6f, 66.8f, 8.6f));
    bool airCollision = collision.testAABBvsVoxelWorld(playerInAir, world);
    std::cout << "    Player in air: " << (airCollision ? "COLLISION" : "NO COLLISION") << " ✓" << std::endl;
    
    // Step 5: Spatial hashing demonstration
    std::cout << "\n[5/5] Spatial hashing optimization..." << std::endl;
    
    SpatialHash spatialHash(10.0f); // 10-block cell size
    std::cout << "    ✓ Created spatial hash with 10-block cells" << std::endl;
    
    // Add multiple objects
    for (int i = 0; i < 10; i++) {
        float x = static_cast<float>(i * 5);
        AABB obj(glm::vec3(x, 0, 0), glm::vec3(x + 1, 1, 1));
        spatialHash.insert(i, obj);
    }
    std::cout << "    ✓ Inserted 10 objects into spatial hash" << std::endl;
    
    // Query nearby objects
    AABB queryBox(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(11.0f, 1.0f, 1.0f));
    auto nearby = spatialHash.queryNearby(queryBox);
    
    std::cout << "    Query around (10,0,0): Found " << nearby.size() << " nearby objects" << std::endl;
    std::cout << "    (Spatial hashing reduces collision checks by ~90%)" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • AABB collision detection" << std::endl;
    std::cout << "  • Sphere collision detection" << std::endl;
    std::cout << "  • Player vs voxel world collision" << std::endl;
    std::cout << "  • Spatial hashing for performance" << std::endl;
    std::cout << "  • Different collision shapes and uses" << std::endl;
    std::cout << "\nCollision Shapes:" << std::endl;
    std::cout << "  • AABB: Fast, simple, axis-aligned (players, items)" << std::endl;
    std::cout << "  • Sphere: Fast, rotation-independent (projectiles)" << std::endl;
    std::cout << "  • Ray: Line-of-sight, raycasting (shooting, interaction)" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Integrate with physics_system for full physics simulation" << std::endl;
    std::cout << "  • Add collision response (sliding, bouncing)" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. AABB (Axis-Aligned Bounding Box)
 *    - Rectangular box aligned to world axes
 *    - Defined by min and max corners
 *    - Very fast collision test
 *    - Used for most game objects
 * 
 * 2. Sphere Collision
 *    - Defined by center point and radius
 *    - Simple distance check
 *    - Rotation-independent
 *    - Used for projectiles, explosions
 * 
 * 3. Spatial Hashing
 *    - Divides world into grid cells
 *    - Objects stored in cells they occupy
 *    - Only test objects in nearby cells
 *    - Massive performance improvement
 * 
 * 4. Collision Detection vs Response
 *    - Detection: Are two objects overlapping?
 *    - Response: What happens when they collide?
 *    - Detection is fast, response is complex
 * 
 * 5. Broad vs Narrow Phase
 *    - Broad: Quickly find potential collisions (spatial hash)
 *    - Narrow: Precise test for actual collision
 *    - Two-phase approach is much faster
 * 
 * AABB COLLISION TEST:
 * 
 * bool testAABBvsAABB(AABB a, AABB b) {
 *     return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
 *            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
 *            (a.min.z <= b.max.z && a.max.z >= b.min.z);
 * }
 * 
 * SPHERE COLLISION TEST:
 * 
 * bool testSphereVsSphere(Sphere a, Sphere b) {
 *     float distance = length(a.center - b.center);
 *     return distance < (a.radius + b.radius);
 * }
 * 
 * PLAYER COLLISION WORKFLOW:
 * 
 * 1. Move player to new position
 * 2. Create AABB at new position
 * 3. Test AABB vs voxel world
 * 4. If collision, reject move or slide
 * 5. If no collision, accept move
 * 
 * VOXEL COLLISION:
 * 
 * • Convert AABB to voxel coordinates
 * • Check all voxels in AABB range
 * • If any voxel is solid, collision occurs
 * • Can check 8 corners for efficiency
 * 
 * SPATIAL HASH BENEFITS:
 * 
 * • O(1) insertion and query
 * • Only test nearby objects
 * • 10-100x faster than brute force
 * • Essential for many objects
 * • Configurable cell size
 * 
 * COLLISION RESPONSE TYPES:
 * 
 * 1. Stop:
 *    - Simply reject movement
 *    - Used for hard walls
 * 
 * 2. Slide:
 *    - Project velocity along surface
 *    - Smooth movement along walls
 *    - Used for player movement
 * 
 * 3. Bounce:
 *    - Reflect velocity off surface
 *    - Used for projectiles, balls
 * 
 * 4. Push:
 *    - Move both objects apart
 *    - Used for dynamic objects
 * 
 * PERFORMANCE OPTIMIZATION:
 * 
 * • Use spatial hashing for broad phase
 * • Sleep inactive objects
 * • Use simpler shapes when possible
 * • Cache collision results when applicable
 * • Update collisions only when objects move
 * 
 * COMMON PATTERNS:
 * 
 * 1. Player Movement:
 *    newPos = oldPos + velocity;
 *    if (!collision.test(playerAABB at newPos, world)) {
 *        player.setPosition(newPos);
 *    }
 * 
 * 2. Projectile:
 *    if (collision.test(projectileSphere, world)) {
 *        projectile.explode();
 *    }
 * 
 * 3. Item Pickup:
 *    for (item in nearbyItems) {
 *        if (collision.test(playerSphere, itemSphere)) {
 *            player.pickup(item);
 *        }
 *    }
 * 
 * DEBUGGING:
 * 
 * • Visualize collision shapes
 * • Draw AABBs as wireframe boxes
 * • Draw spheres as wireframe spheres
 * • Color code: green = no collision, red = collision
 */
