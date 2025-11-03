#include "gameplay/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace fresh {

Camera::Camera(float fov, float nearPlane, float farPlane)
    : fov(fov), nearPlane(nearPlane), farPlane(farPlane) {
    updateVectors();
}

void Camera::updateVectors() {
    // Calculate front vector from yaw and pitch
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processMouseMovement(float xOffset, float yOffset, float sensitivity) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    yaw += xOffset;
    pitch += yOffset;
    
    // Constrain pitch to avoid gimbal lock
    pitch = std::clamp(pitch, -89.0f, 89.0f);
    
    updateVectors();
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::setRotation(float newPitch, float newYaw) {
    pitch = std::clamp(newPitch, -89.0f, 89.0f);
    yaw = newYaw;
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

} // namespace fresh
