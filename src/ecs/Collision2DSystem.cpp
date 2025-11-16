#include "ecs/Collision2DSystem.h"
#include "ecs/Transform2DComponent.h"
#include "ecs/Collider2DComponent.h"
#include "ecs/RigidBody2DComponent.h"
#include "ecs/EntityManager.h"
#include <algorithm>

namespace fresh
{
namespace ecs
{

Collision2DSystem::Collision2DSystem(EntityManager* manager)
    : SystemBase(manager)
{
}

void Collision2DSystem::initialize()
{
    currentCollisions_.clear();
}

void Collision2DSystem::update(float deltaTime)
{
    (void)deltaTime; // Not used in collision detection
    
    if (!enabled)
        return;

    currentCollisions_.clear();

    // Broad phase: get potential collision pairs
    auto pairs = broadPhase();

    // Narrow phase: detailed collision detection
    for (const auto& pair : pairs)
    {
        Collision2D collision;
        if (narrowPhase(pair.first, pair.second, collision))
        {
            currentCollisions_.push_back(collision);

            // Trigger callbacks
            if (collision.isTrigger)
            {
                for (auto& callback : triggerCallbacks_)
                {
                    callback(collision);
                }
            }
            else
            {
                // Resolve physical collision
                resolveCollision(collision);

                // Call collision callbacks
                for (auto& callback : collisionCallbacks_)
                {
                    callback(collision);
                }
            }
        }
    }
}

void Collision2DSystem::onCollision(CollisionCallback callback)
{
    collisionCallbacks_.push_back(callback);
}

void Collision2DSystem::onTrigger(CollisionCallback callback)
{
    triggerCallbacks_.push_back(callback);
}

std::vector<std::pair<Entity, Entity>> Collision2DSystem::broadPhase()
{
    std::vector<std::pair<Entity, Entity>> pairs;
    
    // Get all entities with colliders
    std::vector<Entity> colliderEntities;
    for (const auto& [id, entity] : entityManager->getAllEntities())
    {
        auto* transform = entity->getComponent<Transform2DComponent>();
        auto* collider = entity->getComponent<Collider2DComponent>();
        
        if (transform && collider && collider->enabled)
        {
            colliderEntities.push_back(Entity(id));
        }
    }
    
    // Check all pairs using AABB overlap
    for (size_t i = 0; i < colliderEntities.size(); i++)
    {
        for (size_t j = i + 1; j < colliderEntities.size(); j++)
        {
            Entity entityA = colliderEntities[i];
            Entity entityB = colliderEntities[j];
            
            auto* entityObjA = entityManager->getEntity(entityA);
            auto* entityObjB = entityManager->getEntity(entityB);
            
            if (!entityObjA || !entityObjB)
                continue;
            
            auto* transformA = entityObjA->getComponent<Transform2DComponent>();
            auto* transformB = entityObjB->getComponent<Transform2DComponent>();
            auto* colliderA = entityObjA->getComponent<Collider2DComponent>();
            auto* colliderB = entityObjB->getComponent<Collider2DComponent>();
            
            // Check collision layer mask
            if ((colliderA->layer & colliderB->mask) == 0 &&
                (colliderB->layer & colliderA->mask) == 0)
            {
                continue;
            }
            
            // AABB overlap test
            auto [minA, maxA] = colliderA->getAABB(transformA->position, transformA->rotation);
            auto [minB, maxB] = colliderB->getAABB(transformB->position, transformB->rotation);
            
            if (maxA.x >= minB.x && minA.x <= maxB.x &&
                maxA.y >= minB.y && minA.y <= maxB.y)
            {
                pairs.push_back({entityA, entityB});
            }
        }
    }
    
    return pairs;
}

bool Collision2DSystem::narrowPhase(Entity entityA, Entity entityB, Collision2D& collision)
{
    auto* entityObjA = entityManager->getEntity(entityA);
    auto* entityObjB = entityManager->getEntity(entityB);
    
    if (!entityObjA || !entityObjB)
        return false;
    
    auto* transformA = entityObjA->getComponent<Transform2DComponent>();
    auto* transformB = entityObjB->getComponent<Transform2DComponent>();
    auto* colliderA = entityObjA->getComponent<Collider2DComponent>();
    auto* colliderB = entityObjB->getComponent<Collider2DComponent>();
    
    if (!transformA || !transformB || !colliderA || !colliderB)
        return false;
    
    collision.entityA = entityA;
    collision.entityB = entityB;
    collision.isTrigger = colliderA->isTrigger || colliderB->isTrigger;
    
    // Dispatch to appropriate collision detection function
    if (colliderA->shapeType == Collider2DComponent::ShapeType::Box &&
        colliderB->shapeType == Collider2DComponent::ShapeType::Box)
    {
        return checkBoxBox(transformA, colliderA, transformB, colliderB, collision);
    }
    else if (colliderA->shapeType == Collider2DComponent::ShapeType::Circle &&
             colliderB->shapeType == Collider2DComponent::ShapeType::Circle)
    {
        return checkCircleCircle(transformA, colliderA, transformB, colliderB, collision);
    }
    else if ((colliderA->shapeType == Collider2DComponent::ShapeType::Box &&
              colliderB->shapeType == Collider2DComponent::ShapeType::Circle) ||
             (colliderA->shapeType == Collider2DComponent::ShapeType::Circle &&
              colliderB->shapeType == Collider2DComponent::ShapeType::Box))
    {
        return checkBoxCircle(transformA, colliderA, transformB, colliderB, collision);
    }
    
    // Other shape combinations not implemented yet
    return false;
}

void Collision2DSystem::resolveCollision(const Collision2D& collision)
{
    auto* entityObjA = entityManager->getEntity(collision.entityA);
    auto* entityObjB = entityManager->getEntity(collision.entityB);
    
    if (!entityObjA || !entityObjB)
        return;
    
    auto* transformA = entityObjA->getComponent<Transform2DComponent>();
    auto* transformB = entityObjB->getComponent<Transform2DComponent>();
    auto* rigidbodyA = entityObjA->getComponent<RigidBody2DComponent>();
    auto* rigidbodyB = entityObjB->getComponent<RigidBody2DComponent>();
    
    if (!transformA || !transformB)
        return;
    
    // Position correction (separate overlapping objects)
    float totalInverseMass = 0.0f;
    if (rigidbodyA && rigidbodyA->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        totalInverseMass += rigidbodyA->inverseMass;
    if (rigidbodyB && rigidbodyB->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        totalInverseMass += rigidbodyB->inverseMass;
    
    if (totalInverseMass > 0.0f)
    {
        glm::vec2 correction = collision.normal * collision.penetration;
        
        if (rigidbodyA && rigidbodyA->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        {
            float moveA = rigidbodyA->inverseMass / totalInverseMass;
            transformA->position -= correction * moveA;
        }
        
        if (rigidbodyB && rigidbodyB->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        {
            float moveB = rigidbodyB->inverseMass / totalInverseMass;
            transformB->position += correction * moveB;
        }
    }
    
    // Impulse resolution (change velocities)
    if (rigidbodyA && rigidbodyB)
    {
        glm::vec2 relativeVelocity = rigidbodyB->velocity - rigidbodyA->velocity;
        float velocityAlongNormal = glm::dot(relativeVelocity, collision.normal);
        
        // Don't resolve if objects are separating
        if (velocityAlongNormal > 0)
            return;
        
        // Calculate restitution (bounciness)
        float restitution = std::min(rigidbodyA->restitution, rigidbodyB->restitution);
        
        // Calculate impulse scalar
        float impulseMagnitude = -(1.0f + restitution) * velocityAlongNormal;
        impulseMagnitude /= totalInverseMass;
        
        glm::vec2 impulse = collision.normal * impulseMagnitude;
        
        // Apply impulses
        if (rigidbodyA->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        {
            rigidbodyA->velocity -= impulse * rigidbodyA->inverseMass;
        }
        
        if (rigidbodyB->bodyType == RigidBody2DComponent::BodyType::Dynamic)
        {
            rigidbodyB->velocity += impulse * rigidbodyB->inverseMass;
        }
    }
}

bool Collision2DSystem::checkBoxBox(const Transform2DComponent* transformA, const Collider2DComponent* colliderA,
                                    const Transform2DComponent* transformB, const Collider2DComponent* colliderB,
                                    Collision2D& collision)
{
    // Simplified AABB collision (assuming axis-aligned for now)
    glm::vec2 posA = transformA->position + colliderA->offset;
    glm::vec2 posB = transformB->position + colliderB->offset;
    
    glm::vec2 delta = posB - posA;
    glm::vec2 overlap = colliderA->boxSize + colliderB->boxSize - glm::abs(delta);
    
    if (overlap.x > 0 && overlap.y > 0)
    {
        // Collision detected
        if (overlap.x < overlap.y)
        {
            collision.normal = glm::vec2(delta.x > 0 ? 1.0f : -1.0f, 0.0f);
            collision.penetration = overlap.x;
        }
        else
        {
            collision.normal = glm::vec2(0.0f, delta.y > 0 ? 1.0f : -1.0f);
            collision.penetration = overlap.y;
        }
        
        collision.point = posA + delta * 0.5f;
        return true;
    }
    
    return false;
}

bool Collision2DSystem::checkCircleCircle(const Transform2DComponent* transformA, const Collider2DComponent* colliderA,
                                         const Transform2DComponent* transformB, const Collider2DComponent* colliderB,
                                         Collision2D& collision)
{
    glm::vec2 posA = transformA->position + colliderA->offset;
    glm::vec2 posB = transformB->position + colliderB->offset;
    
    glm::vec2 delta = posB - posA;
    float distance = glm::length(delta);
    float radiusSum = colliderA->circleRadius + colliderB->circleRadius;
    
    if (distance < radiusSum)
    {
        // Collision detected
        collision.penetration = radiusSum - distance;
        
        if (distance > 0.0001f)
        {
            collision.normal = delta / distance;
        }
        else
        {
            // Circles are at exact same position
            collision.normal = glm::vec2(1.0f, 0.0f);
        }
        
        collision.point = posA + collision.normal * colliderA->circleRadius;
        return true;
    }
    
    return false;
}

bool Collision2DSystem::checkBoxCircle(const Transform2DComponent* transformA, const Collider2DComponent* colliderA,
                                      const Transform2DComponent* transformB, const Collider2DComponent* colliderB,
                                      Collision2D& collision)
{
    // Ensure A is box and B is circle
    if (colliderA->shapeType != Collider2DComponent::ShapeType::Box)
    {
        return checkBoxCircle(transformB, colliderB, transformA, colliderA, collision);
    }
    
    glm::vec2 boxPos = transformA->position + colliderA->offset;
    glm::vec2 circlePos = transformB->position + colliderB->offset;
    
    // Find closest point on box to circle
    glm::vec2 closest = glm::clamp(circlePos, boxPos - colliderA->boxSize, boxPos + colliderA->boxSize);
    
    glm::vec2 delta = circlePos - closest;
    float distance = glm::length(delta);
    
    if (distance < colliderB->circleRadius)
    {
        collision.penetration = colliderB->circleRadius - distance;
        
        if (distance > 0.0001f)
        {
            collision.normal = delta / distance;
        }
        else
        {
            // Circle center is inside box
            // Use direction to nearest edge
            glm::vec2 toEdge = circlePos - boxPos;
            if (std::abs(toEdge.x) > std::abs(toEdge.y))
            {
                collision.normal = glm::vec2(toEdge.x > 0 ? 1.0f : -1.0f, 0.0f);
            }
            else
            {
                collision.normal = glm::vec2(0.0f, toEdge.y > 0 ? 1.0f : -1.0f);
            }
        }
        
        collision.point = closest;
        return true;
    }
    
    return false;
}

} // namespace ecs
} // namespace fresh
