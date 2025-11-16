#include "ecs/Camera2DSystem.h"
#include "ecs/Camera2DComponent.h"
#include "ecs/Transform2DComponent.h"
#include "ecs/EntityManager.h"
#include <algorithm>

namespace fresh
{
namespace ecs
{

Camera2DSystem::Camera2DSystem(EntityManager* manager)
    : SystemBase(manager)
{
}

void Camera2DSystem::initialize()
{
    // Find primary camera
    primaryCamera_ = Entity();
    
    for (const auto& [id, entity] : entityManager->getAllEntities())
    {
        auto* camera = entity->getComponent<Camera2DComponent>();
        if (camera && camera->isPrimary)
        {
            primaryCamera_ = Entity(id);
            break;
        }
    }
}

void Camera2DSystem::update(float deltaTime)
{
    if (!enabled)
        return;

    // Update all cameras
    for (auto& [id, entity] : entityManager->getAllEntities())
    {
        auto* camera = entity->getComponent<Camera2DComponent>();
        auto* transform = entity->getComponent<Transform2DComponent>();
        
        if (!camera || !transform)
            continue;
        
        // Clamp zoom
        camera->zoom = std::clamp(camera->zoom, camera->minZoom, camera->maxZoom);
        
        // Update follow behavior
        if (camera->followTarget.isValid())
        {
            updateFollow(camera, transform, deltaTime);
        }
        
        // Apply bounds
        if (camera->useBounds)
        {
            applyBounds(camera, transform);
        }
    }
}

Entity Camera2DSystem::getPrimaryCamera() const
{
    return primaryCamera_;
}

void Camera2DSystem::setScreenAspectRatio(float aspectRatio)
{
    for (auto& [id, entity] : entityManager->getAllEntities())
    {
        auto* camera = entity->getComponent<Camera2DComponent>();
        if (camera)
        {
            camera->aspectRatio = aspectRatio;
        }
    }
}

void Camera2DSystem::updateFollow(Camera2DComponent* camera,
                                  Transform2DComponent* cameraTransform,
                                  float deltaTime)
{
    auto* targetEntity = entityManager->getEntity(camera->followTarget);
    if (!targetEntity)
    {
        camera->followTarget = Entity(); // Invalid target
        return;
    }
    
    auto* targetTransform = targetEntity->getComponent<Transform2DComponent>();
    if (!targetTransform)
    {
        camera->followTarget = Entity(); // Target has no transform
        return;
    }
    
    // Calculate target position
    glm::vec2 targetPos = targetTransform->position + camera->followOffset;
    
    // Apply smoothing
    if (camera->followSmoothing > 0.0f)
    {
        float smoothFactor = 1.0f - std::pow(camera->followSmoothing, deltaTime * 60.0f);
        cameraTransform->position = glm::mix(cameraTransform->position, targetPos, smoothFactor);
    }
    else
    {
        // Instant follow
        cameraTransform->position = targetPos;
    }
}

void Camera2DSystem::applyBounds(Camera2DComponent* camera,
                                Transform2DComponent* cameraTransform)
{
    // Calculate camera extents
    float effectiveSize = camera->orthographicSize / camera->zoom;
    float halfWidth = effectiveSize * camera->aspectRatio;
    float halfHeight = effectiveSize;
    
    // Clamp position to bounds
    glm::vec2 minPos = camera->boundsMin + glm::vec2(halfWidth, halfHeight);
    glm::vec2 maxPos = camera->boundsMax - glm::vec2(halfWidth, halfHeight);
    
    cameraTransform->position.x = std::clamp(cameraTransform->position.x, minPos.x, maxPos.x);
    cameraTransform->position.y = std::clamp(cameraTransform->position.y, minPos.y, maxPos.y);
}

} // namespace ecs
} // namespace fresh
