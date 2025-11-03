#include "gameplay/Player.h"
#include "input/InputManager.h"
#include "voxel/VoxelWorld.h"
#include "voxel/VoxelTypes.h"
#include <algorithm>

namespace fresh {

Player::Player() {
    // Camera starts at player eye height
    camera.setPosition(position + glm::vec3(0.0f, eyeHeight, 0.0f));
}

void Player::update(float deltaTime) {
    if (!world) return;
    
    // Apply gravity
    applyGravity(deltaTime);
    
    // Apply velocity
    glm::vec3 newPosition = position + velocity * deltaTime;
    
    // Check collision
    if (!checkCollision(newPosition)) {
        position = newPosition;
    } else {
        // Stop vertical movement if we hit something
        velocity.y = 0.0f;
    }
    
    // Update grounded state
    isGrounded = checkGrounded();
    
    // Update camera position to follow player
    float currentEyeHeight = isCrouching ? eyeHeight * 0.8f : eyeHeight;
    camera.setPosition(position + glm::vec3(0.0f, currentEyeHeight, 0.0f));
}

void Player::handleInput(const InputManager& input, float deltaTime) {
    if (!world) return;
    
    // Get camera direction vectors (ignore Y for horizontal movement)
    glm::vec3 forward = glm::normalize(glm::vec3(camera.getFront().x, 0.0f, camera.getFront().z));
    glm::vec3 right = glm::normalize(glm::vec3(camera.getRight().x, 0.0f, camera.getRight().z));
    
    // Calculate movement direction
    glm::vec3 moveDirection{0.0f};
    
    if (input.isActionActive(InputAction::MoveForward)) {
        moveDirection += forward;
    }
    if (input.isActionActive(InputAction::MoveBackward)) {
        moveDirection -= forward;
    }
    if (input.isActionActive(InputAction::MoveRight)) {
        moveDirection += right;
    }
    if (input.isActionActive(InputAction::MoveLeft)) {
        moveDirection -= right;
    }
    
    // Normalize if moving diagonally
    if (glm::length(moveDirection) > 0.0f) {
        moveDirection = glm::normalize(moveDirection);
    }
    
    // Determine speed
    isSprinting = input.isActionActive(InputAction::Sprint) && !isCrouching;
    isCrouching = input.isActionActive(InputAction::Crouch);
    
    float speed = walkSpeed;
    if (isSprinting) {
        speed = sprintSpeed;
    } else if (isCrouching) {
        speed = crouchSpeed;
    }
    
    // Apply movement
    if (glm::length(moveDirection) > 0.0f) {
        handleMovement(moveDirection, speed, deltaTime);
    }
    
    // Jump
    if (input.isActionJustPressed(InputAction::Jump) && isGrounded && !isCrouching) {
        velocity.y = jumpVelocity;
        isGrounded = false;
    }
}

void Player::handleMouseMovement(float xOffset, float yOffset) {
    camera.processMouseMovement(xOffset * mouseSensitivity, -yOffset * mouseSensitivity);
}

void Player::setPosition(const glm::vec3& pos) {
    position = pos;
    camera.setPosition(pos + glm::vec3(0.0f, eyeHeight, 0.0f));
}

void Player::applyGravity(float deltaTime) {
    if (!isGrounded) {
        velocity.y -= gravity * deltaTime;
        // Terminal velocity
        velocity.y = std::max(velocity.y, -50.0f);
    } else {
        // Small downward velocity when grounded to stick to terrain
        velocity.y = -0.1f;
    }
}

void Player::handleMovement(const glm::vec3& direction, float speed, float deltaTime) {
    glm::vec3 movement = direction * speed * deltaTime;
    
    // Try to move in X
    glm::vec3 newPos = position + glm::vec3(movement.x, 0.0f, 0.0f);
    if (!checkCollision(newPos)) {
        position.x = newPos.x;
    }
    
    // Try to move in Z
    newPos = position + glm::vec3(0.0f, 0.0f, movement.z);
    if (!checkCollision(newPos)) {
        position.z = newPos.z;
    }
}

bool Player::checkCollision(const glm::vec3& newPos) {
    if (!world) return false;
    
    // Check player bounding box (simplified cylinder as box)
    float checkHeight = isCrouching ? crouchHeight : height;
    
    // Sample points around player cylinder
    int samples = 8;
    for (int i = 0; i < samples; ++i) {
        float angle = (2.0f * 3.14159f * i) / samples;
        float x = newPos.x + radius * cos(angle);
        float z = newPos.z + radius * sin(angle);
        
        // Check from feet to head
        for (float y = newPos.y; y < newPos.y + checkHeight; y += 0.5f) {
            int blockX = static_cast<int>(std::floor(x));
            int blockY = static_cast<int>(std::floor(y));
            int blockZ = static_cast<int>(std::floor(z));
            
            Voxel* voxelPtr = world->getVoxel(WorldPos(blockX, blockY, blockZ));
            VoxelType voxel = voxelPtr ? voxelPtr->type : VoxelType::Air;
            if (voxel != VoxelType::Air && voxel != VoxelType::Water) {
                return true; // Collision detected
            }
        }
    }
    
    return false;
}

bool Player::checkGrounded() {
    if (!world) return false;
    
    // Check slightly below player feet
    float checkY = position.y - 0.1f;
    
    // Sample points around player cylinder
    int samples = 4;
    for (int i = 0; i < samples; ++i) {
        float angle = (2.0f * 3.14159f * i) / samples;
        float x = position.x + (radius * 0.7f) * cos(angle);
        float z = position.z + (radius * 0.7f) * sin(angle);
        
        int blockX = static_cast<int>(std::floor(x));
        int blockY = static_cast<int>(std::floor(checkY));
        int blockZ = static_cast<int>(std::floor(z));
        
        Voxel* voxelPtr = world->getVoxel(WorldPos(blockX, blockY, blockZ));
        VoxelType voxel = voxelPtr ? voxelPtr->type : VoxelType::Air;
        if (voxel != VoxelType::Air && voxel != VoxelType::Water) {
            return true; // Ground detected
        }
    }
    
    return false;
}

} // namespace fresh
