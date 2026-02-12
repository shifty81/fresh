#include "ai/BehaviorTree.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "ecs/Entity.h"
#include "voxel/VoxelWorld.h"

namespace fresh
{

// BehaviorTree implementation
BehaviorTree::BehaviorTree() : rootNode(nullptr) {}

BehaviorTree::~BehaviorTree()
{
    // Note: Nodes are not owned by the tree in this simple implementation
}

void BehaviorTree::setRoot(BehaviorNode* root)
{
    rootNode = root;
}

NodeStatus BehaviorTree::tick(ecs::Entity* entity, float deltaTime)
{
    if (!rootNode) {
        return NodeStatus::Failure;
    }
    return rootNode->execute(entity, deltaTime);
}

// SequenceNode implementation
void SequenceNode::addChild(BehaviorNode* child)
{
    children.push_back(child);
}

NodeStatus SequenceNode::execute(ecs::Entity* entity, float deltaTime)
{
    for (auto* child : children) {
        NodeStatus status = child->execute(entity, deltaTime);
        if (status != NodeStatus::Success) {
            return status; // Return on first non-success
        }
    }
    return NodeStatus::Success;
}

// SelectorNode implementation
void SelectorNode::addChild(BehaviorNode* child)
{
    children.push_back(child);
}

NodeStatus SelectorNode::execute(ecs::Entity* entity, float deltaTime)
{
    for (auto* child : children) {
        NodeStatus status = child->execute(entity, deltaTime);
        if (status != NodeStatus::Failure) {
            return status; // Return on first non-failure
        }
    }
    return NodeStatus::Failure;
}

// WanderBehavior implementation
WanderBehavior::WanderBehavior(VoxelWorld* w) : world(w), wanderTimer(0.0f), targetPosition(0.0f) {}

NodeStatus WanderBehavior::execute(ecs::Entity* entity, float deltaTime)
{
    if (!entity || !entity->isValid()) {
        return NodeStatus::Failure;
    }

    static constexpr float WANDER_INTERVAL = 3.0f;
    static constexpr float WANDER_RADIUS = 10.0f;
    static constexpr float ARRIVAL_THRESHOLD = 0.5f;

    wanderTimer -= deltaTime;

    if (wanderTimer <= 0.0f) {
        // Pick a new random target position within wander radius
        glm::vec3 offset = glm::linearRand(
            glm::vec3(-WANDER_RADIUS, 0.0f, -WANDER_RADIUS),
            glm::vec3(WANDER_RADIUS, 0.0f, WANDER_RADIUS));
        targetPosition = offset;
        wanderTimer = WANDER_INTERVAL;
    }

    // Check if close enough to target (using targetPosition magnitude as proxy)
    if (glm::length(targetPosition) < ARRIVAL_THRESHOLD) {
        return NodeStatus::Success;
    }

    return NodeStatus::Running;
}

// FollowBehavior implementation
FollowBehavior::FollowBehavior(ecs::Entity* target, float distance)
    : targetEntity(target), followDistance(distance)
{
}

NodeStatus FollowBehavior::execute(ecs::Entity* entity, float deltaTime)
{
    (void)deltaTime;

    if (!entity || !entity->isValid()) {
        return NodeStatus::Failure;
    }

    if (!targetEntity || !targetEntity->isValid()) {
        return NodeStatus::Failure;
    }

    // Without direct position access, signal that following is in progress
    // A movement system would read the target and apply actual movement
    return NodeStatus::Running;
}

} // namespace fresh
