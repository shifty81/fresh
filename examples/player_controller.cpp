/**
 * @file player_controller.cpp
 * @brief Demonstrates implementing a first-person player controller
 * 
 * This example shows:
 * - Setting up player movement (WASD)
 * - Implementing jump mechanics
 * - Adding sprint functionality
 * - Collision detection with terrain
 * - Gravity and physics integration
 */

#include <iostream>
#include <memory>
#include "gameplay/Player.h"
#include "gameplay/Camera.h"
#include "voxel/VoxelWorld.h"
#include "input/InputManager.h"
#include "physics/PhysicsSystem.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Player Controller ===" << std::endl;
    std::cout << "This example demonstrates first-person player movement." << std::endl;
    
    // Step 1: Setup world with terrain
    std::cout << "\n[1/5] Creating world with terrain..." << std::endl;
    auto world = std::make_shared<VoxelWorld>();
    
    // Create flat terrain platform
    auto chunk = std::make_unique<Chunk>(0, 0);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y <= 60; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
            chunk->setVoxel(x, 61, z, VoxelType::Grass);
        }
    }
    world->addChunk(std::move(chunk));
    std::cout << "    ✓ Created flat terrain platform" << std::endl;
    
    // Step 2: Create player instance
    std::cout << "\n[2/5] Initializing player..." << std::endl;
    Player player(world);
    
    // Set initial position (above terrain)
    glm::vec3 spawnPos(8.0f, 65.0f, 8.0f);
    player.setPosition(spawnPos);
    std::cout << "    ✓ Player spawned at (" << spawnPos.x << ", " << spawnPos.y << ", " << spawnPos.z << ")" << std::endl;
    
    // Configure player properties
    player.setMoveSpeed(5.0f);        // 5 blocks/second
    player.setSprintMultiplier(1.5f); // 1.5x faster when sprinting
    player.setJumpHeight(1.2f);       // Jump 1.2 blocks high
    std::cout << "    ✓ Player properties configured" << std::endl;
    std::cout << "      Move speed: 5.0 blocks/sec" << std::endl;
    std::cout << "      Sprint multiplier: 1.5x" << std::endl;
    std::cout << "      Jump height: 1.2 blocks" << std::endl;
    
    // Step 3: Setup input system
    std::cout << "\n[3/5] Configuring input..." << std::endl;
    InputManager input;
    
    // Bind movement keys
    input.bindKey("MoveForward", GLFW_KEY_W);
    input.bindKey("MoveBackward", GLFW_KEY_S);
    input.bindKey("MoveLeft", GLFW_KEY_A);
    input.bindKey("MoveRight", GLFW_KEY_D);
    input.bindKey("Jump", GLFW_KEY_SPACE);
    input.bindKey("Sprint", GLFW_KEY_LEFT_SHIFT);
    input.bindKey("Crouch", GLFW_KEY_LEFT_CONTROL);
    std::cout << "    ✓ Movement keys bound (WASD + Space + Shift)" << std::endl;
    
    // Configure mouse look
    input.setMouseSensitivity(0.002f);
    input.setCursorMode(CursorMode::Disabled); // Lock cursor for FPS
    std::cout << "    ✓ Mouse look configured" << std::endl;
    
    // Step 4: Simulate game loop with player movement
    std::cout << "\n[4/5] Simulating player movement..." << std::endl;
    
    float deltaTime = 0.016f; // ~60 FPS
    
    // Simulate forward movement for 2 seconds (120 frames)
    std::cout << "    Simulating forward movement..." << std::endl;
    glm::vec3 startPos = player.getPosition();
    
    for (int frame = 0; frame < 120; frame++) {
        // Simulate W key pressed
        bool moveForward = true;
        
        // Calculate move direction
        glm::vec3 forward = player.getCamera().getForward();
        glm::vec3 velocity(0.0f);
        
        if (moveForward) {
            velocity += forward * player.getMoveSpeed() * deltaTime;
        }
        
        // Apply gravity
        velocity.y -= 9.81f * deltaTime;
        
        // Update player (handles collision, etc.)
        player.update(deltaTime);
        
        // Move player
        player.move(velocity);
    }
    
    glm::vec3 endPos = player.getPosition();
    float distanceMoved = glm::length(endPos - startPos);
    std::cout << "    ✓ Moved " << distanceMoved << " blocks forward" << std::endl;
    std::cout << "      Start: (" << startPos.x << ", " << startPos.y << ", " << startPos.z << ")" << std::endl;
    std::cout << "      End:   (" << endPos.x << ", " << endPos.y << ", " << endPos.z << ")" << std::endl;
    
    // Simulate jump
    std::cout << "\n    Simulating jump..." << std::endl;
    if (player.isGrounded()) {
        player.jump();
        std::cout << "    ✓ Player jumped!" << std::endl;
        
        // Simulate several frames of jump arc
        for (int frame = 0; frame < 30; frame++) {
            player.update(deltaTime);
        }
        
        float maxHeight = player.getPosition().y;
        std::cout << "    Peak height reached: " << maxHeight << " (+" << (maxHeight - endPos.y) << " blocks)" << std::endl;
    }
    
    // Step 5: Player state and properties
    std::cout << "\n[5/5] Player state information..." << std::endl;
    std::cout << "    Position: (" << player.getPosition().x << ", " 
              << player.getPosition().y << ", " << player.getPosition().z << ")" << std::endl;
    std::cout << "    Velocity: (" << player.getVelocity().x << ", "
              << player.getVelocity().y << ", " << player.getVelocity().z << ")" << std::endl;
    std::cout << "    Grounded: " << (player.isGrounded() ? "Yes" : "No") << std::endl;
    std::cout << "    Sprinting: " << (player.isSprinting() ? "Yes" : "No") << std::endl;
    std::cout << "    Crouching: " << (player.isCrouching() ? "Yes" : "No") << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Creating and configuring a player instance" << std::endl;
    std::cout << "  • Setting up movement controls (WASD)" << std::endl;
    std::cout << "  • Implementing jump mechanics" << std::endl;
    std::cout << "  • Player-terrain collision detection" << std::endl;
    std::cout << "  • Integrating with input system" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try camera_system.cpp for advanced camera controls" << std::endl;
    std::cout << "  • See collision_detection.cpp for detailed physics" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Player Class
 *    - Manages player position and state
 *    - Integrates camera for first-person view
 *    - Handles movement input and physics
 *    - Provides collision detection with world
 * 
 * 2. Movement System
 *    - WASD for directional movement
 *    - Relative to camera forward direction
 *    - Speed configurable per player
 *    - Sprint modifier multiplies speed
 * 
 * 3. Physics Integration
 *    - Gravity pulls player down (9.81 m/s²)
 *    - Collision prevents passing through voxels
 *    - Grounded state detection for jumping
 *    - Velocity-based movement
 * 
 * 4. Jump Mechanics
 *    - Only jump when grounded
 *    - Applies upward velocity
 *    - Gravity brings player back down
 *    - Height determined by initial velocity
 * 
 * 5. Player States
 *    - Grounded: Touching terrain
 *    - Sprinting: Moving faster
 *    - Crouching: Moving slower, smaller hitbox
 *    - Falling: In air with downward velocity
 * 
 * MOVEMENT PATTERNS:
 * 
 * 1. Basic Movement:
 *    velocity = forward * moveSpeed * deltaTime;
 *    player.move(velocity);
 * 
 * 2. Sprint:
 *    if (sprintKey.pressed) {
 *        velocity *= sprintMultiplier;
 *    }
 * 
 * 3. Jump:
 *    if (jumpKey.pressed && player.isGrounded()) {
 *        player.jump();
 *    }
 * 
 * 4. Collision Response:
 *    newPos = oldPos + velocity * deltaTime;
 *    if (!world.isSolid(newPos)) {
 *        player.setPosition(newPos);
 *    }
 * 
 * CAMERA INTEGRATION:
 * 
 * • Player contains Camera instance
 * • Camera position = player position + eye offset
 * • Mouse input rotates camera
 * • Movement is relative to camera forward
 * • First-person: camera at eye level
 * • Third-person: camera offset behind player
 * 
 * PHYSICS PARAMETERS:
 * 
 * • Move speed: 4-6 blocks/sec (walking)
 * • Sprint multiplier: 1.3-1.5x
 * • Jump height: 1.0-1.5 blocks
 * • Gravity: 9.81 m/s² (realistic) or custom
 * • Air control: 0.5-1.0 (movement while airborne)
 * 
 * COLLISION DETECTION:
 * 
 * • Player has bounding box (AABB)
 * • Check 8 corners of box against voxels
 * • If any corner in solid voxel, reject move
 * • Slide along surfaces for smooth movement
 * • Ground check: raycast downward
 * 
 * BEST PRACTICES:
 * 
 * • Always multiply movement by deltaTime
 * • Check isGrounded() before allowing jump
 * • Clamp velocity to prevent excessive speeds
 * • Use separate X/Y/Z collision checks for sliding
 * • Interpolate position for smooth movement
 * • Buffer jump input for better feel
 */
