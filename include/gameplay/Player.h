#pragma once
#include <glm/glm.hpp>

#include "Camera.h"

namespace fresh
{

class VoxelWorld;
class InputManager;

/**
 * @brief Player controller with FPS movement and physics
 *
 * Handles player movement, jumping, collision detection with voxel terrain,
 * and camera positioning. Integrates with InputManager and VoxelWorld.
 */
class Player
{
public:
    Player();

    /**
     * @brief Update player state (physics, collision)
     * @param deltaTime Time since last frame in seconds
     */
    void update(float deltaTime);

    /**
     * @brief Process input to control player movement
     * @param input Input manager to read controls from
     * @param deltaTime Time since last frame
     */
    void handleInput(const InputManager& input, float deltaTime);

    /**
     * @brief Process mouse movement for camera control
     * @param xOffset Mouse X movement
     * @param yOffset Mouse Y movement
     */
    void handleMouseMovement(float xOffset, float yOffset);

    // Camera access
    Camera& getCamera()
    {
        return camera;
    }
    const Camera& getCamera() const
    {
        return camera;
    }

    // Position
    glm::vec3 getPosition() const
    {
        return position;
    }
    void setPosition(const glm::vec3& pos);

    // World reference
    void setWorld(VoxelWorld* w)
    {
        world = w;
    }

    // State queries
    bool isPlayerGrounded() const
    {
        return isGrounded;
    }
    bool isPlayerSprinting() const
    {
        return isSprinting;
    }
    bool isPlayerCrouching() const
    {
        return isCrouching;
    }

    // Configuration
    void setWalkSpeed(float speed)
    {
        walkSpeed = speed;
    }
    void setSprintSpeed(float speed)
    {
        sprintSpeed = speed;
    }
    void setMouseSensitivity(float sens)
    {
        mouseSensitivity = sens;
    }
    
    // Free flight mode control
    void setFreeFlightMode(bool enabled)
    {
        freeFlightMode = enabled;
    }
    bool isFreeFlightMode() const
    {
        return freeFlightMode;
    }

private:
    /**
     * @brief Apply gravity to player velocity
     */
    void applyGravity(float deltaTime);

    /**
     * @brief Move player in given direction with collision checking
     * @param direction Movement direction (normalized)
     * @param speed Movement speed
     * @param deltaTime Time delta
     */
    void handleMovement(const glm::vec3& direction, float speed, float deltaTime);

    /**
     * @brief Check if position would collide with terrain
     * @param newPos Position to check
     * @return true if collision detected
     */
    bool checkCollision(const glm::vec3& newPos);

    /**
     * @brief Check if player is on ground
     * @return true if grounded
     */
    bool checkGrounded();

    /**
     * @brief Check if there's ground ahead at a position (for crouch edge detection)
     * @param newPos Position to check
     * @return true if ground exists below this position
     */
    bool checkGroundAhead(const glm::vec3& newPos);

    Camera camera;
    VoxelWorld* world = nullptr;

    glm::vec3 position{0.0f, 80.0f, 0.0f};
    glm::vec3 velocity{0.0f};

    // Player dimensions (capsule collision)
    float height = 1.8f;
    float eyeHeight = 1.6f;
    float radius = 0.3f;
    float crouchHeight = 1.4f;

    // Movement speeds (blocks per second)
    float walkSpeed = 4.3f;
    float sprintSpeed = 5.6f;
    float crouchSpeed = 2.0f;
    float jumpVelocity = 8.0f;
    float gravity = 20.0f;
    float mouseSensitivity = 10.0f; // Increased from 1.0 for better responsiveness

    // State
    bool isGrounded = false;
    bool isSprinting = false;
    bool isCrouching = false;
    bool freeFlightMode = true; // New: Enable free-flying camera by default
};

} // namespace fresh
