#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fresh
{

/**
 * @brief First-person camera system for the player
 *
 * Handles view and projection matrices, mouse look controls,
 * and camera positioning. Works in conjunction with Player class.
 */
class Camera
{
public:
    /**
     * @brief Construct a new Camera
     * @param fov Field of view in degrees
     * @param nearPlane Near clipping plane distance
     * @param farPlane Far clipping plane distance
     */
    Camera(float fov = 75.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);

    /**
     * @brief Update camera direction vectors based on pitch and yaw
     */
    void updateVectors();

    /**
     * @brief Process mouse movement input for look controls
     * @param xOffset Mouse X offset
     * @param yOffset Mouse Y offset
     * @param sensitivity Mouse sensitivity multiplier
     */
    void processMouseMovement(float xOffset, float yOffset, float sensitivity = 0.002f);

    /**
     * @brief Set camera position
     * @param pos New position
     */
    void setPosition(const glm::vec3& pos);

    /**
     * @brief Set camera rotation
     * @param pitch Pitch angle in degrees (-89 to 89)
     * @param yaw Yaw angle in degrees
     */
    void setRotation(float pitch, float yaw);

    /**
     * @brief Get view matrix for rendering
     * @return View matrix
     */
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    /**
     * @brief Get projection matrix for rendering
     * @param aspectRatio Width/Height ratio of viewport
     * @return Projection matrix
     */
    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspectRatio) const;

    // Getters
    [[nodiscard]] glm::vec3 getPosition() const noexcept
    {
        return position;
    }
    [[nodiscard]] glm::vec3 getFront() const noexcept
    {
        return front;
    }
    [[nodiscard]] glm::vec3 getRight() const noexcept
    {
        return right;
    }
    [[nodiscard]] glm::vec3 getUp() const noexcept
    {
        return up;
    }
    [[nodiscard]] float getPitch() const noexcept
    {
        return pitch;
    }
    [[nodiscard]] float getYaw() const noexcept
    {
        return yaw;
    }
    [[nodiscard]] float getFOV() const noexcept
    {
        return fov;
    }

    // Setters
    void setFOV(float newFov)
    {
        fov = newFov;
    }

private:
    glm::vec3 position{0.0f, 80.0f, 0.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    float pitch = 0.0f; // Rotation around right axis (look up/down)
    float yaw = -90.0f; // Rotation around up axis (look left/right)
    float fov;          // Field of view in degrees
    float nearPlane;    // Near clipping plane
    float farPlane;     // Far clipping plane
};

} // namespace fresh
