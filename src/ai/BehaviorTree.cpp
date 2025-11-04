#include "ai/BehaviorTree.h"
#include "ecs/Entity.h"
#include "voxel/VoxelWorld.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <vector>

namespace fresh {

// BehaviorTree implementation
BehaviorTree::BehaviorTree() : rootNode(nullptr) {
}

BehaviorTree::~BehaviorTree() {
    // Note: Nodes are not owned by the tree in this simple implementation
}

void BehaviorTree::setRoot(BehaviorNode* root) {
    rootNode = root;
}

NodeStatus BehaviorTree::tick(Entity* entity, float deltaTime) {
    if (!rootNode) {
        return NodeStatus::Failure;
    }
    return rootNode->execute(entity, deltaTime);
}

// SequenceNode implementation
void SequenceNode::addChild(BehaviorNode* child) {
    children.push_back(child);
}

NodeStatus SequenceNode::execute(Entity* entity, float deltaTime) {
    for (auto* child : children) {
        NodeStatus status = child->execute(entity, deltaTime);
        if (status != NodeStatus::Success) {
            return status; // Return on first non-success
        }
    }
    return NodeStatus::Success;
}

// SelectorNode implementation
void SelectorNode::addChild(BehaviorNode* child) {
    children.push_back(child);
}

NodeStatus SelectorNode::execute(Entity* entity, float deltaTime) {
    for (auto* child : children) {
        NodeStatus status = child->execute(entity, deltaTime);
        if (status != NodeStatus::Failure) {
            return status; // Return on first non-failure
        }
    }
    return NodeStatus::Failure;
}

// WanderBehavior implementation
WanderBehavior::WanderBehavior(VoxelWorld* w) 
    : world(w), wanderTimer(0.0f), targetPosition(0.0f) {
}

NodeStatus WanderBehavior::execute(Entity* entity, float deltaTime) {
    if (!entity) return NodeStatus::Failure;
    
    wanderTimer -= deltaTime;
    
    // Pick a new random target every 3 seconds
    if (wanderTimer <= 0.0f) {
        wanderTimer = 3.0f;
        
        // Generate random position nearby (within 10 blocks)
        glm::vec3 currentPos = entity->getPosition();
        targetPosition = currentPos + glm::vec3(
            glm::linearRand(-10.0f, 10.0f),
            0.0f,
            glm::linearRand(-10.0f, 10.0f)
        );
    }
    
    // Move towards target
    glm::vec3 currentPos = entity->getPosition();
    glm::vec3 direction = targetPosition - currentPos;
    float distance = glm::length(direction);
    
    if (distance < 0.5f) {
        // Reached target
        return NodeStatus::Success;
    }
    
    // Move towards target
    direction = glm::normalize(direction);
    float moveSpeed = 2.0f; // blocks per second
    glm::vec3 newPos = currentPos + direction * moveSpeed * deltaTime;
    entity->setPosition(newPos);
    
    return NodeStatus::Running;
}

// FollowBehavior implementation
FollowBehavior::FollowBehavior(Entity* target, float distance)
    : targetEntity(target), followDistance(distance) {
}

NodeStatus FollowBehavior::execute(Entity* entity, float deltaTime) {
    if (!entity || !targetEntity) {
        return NodeStatus::Failure;
    }
    
    glm::vec3 currentPos = entity->getPosition();
    glm::vec3 targetPos = targetEntity->getPosition();
    
    glm::vec3 direction = targetPos - currentPos;
    float distance = glm::length(direction);
    
    // If within follow distance, stop
    if (distance <= followDistance) {
        return NodeStatus::Success;
    }
    
    // Move towards target
    direction = glm::normalize(direction);
    float moveSpeed = 3.0f; // blocks per second
    glm::vec3 newPos = currentPos + direction * moveSpeed * deltaTime;
    entity->setPosition(newPos);
    
    return NodeStatus::Running;
}

} // namespace fresh
