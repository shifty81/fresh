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

NodeStatus BehaviorTree::tick(ecs::Entity* entity, float deltaTime) {
    if (!rootNode) {
        return NodeStatus::Failure;
    }
    return rootNode->execute(entity, deltaTime);
}

// SequenceNode implementation
void SequenceNode::addChild(BehaviorNode* child) {
    children.push_back(child);
}

NodeStatus SequenceNode::execute(ecs::Entity* entity, float deltaTime) {
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

NodeStatus SelectorNode::execute(ecs::Entity* entity, float deltaTime) {
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

NodeStatus WanderBehavior::execute(ecs::Entity* entity, float deltaTime) {
    (void)entity;  // Unused for now - needs proper ECS integration
    (void)deltaTime;
    (void)world;
    // TODO: Implement proper ECS-based wandering
    return NodeStatus::Running;
}

// FollowBehavior implementation
FollowBehavior::FollowBehavior(ecs::Entity* target, float distance)
    : targetEntity(target), followDistance(distance) {
}

NodeStatus FollowBehavior::execute(ecs::Entity* entity, float deltaTime) {
    (void)entity;  // Unused for now - needs proper ECS integration
    (void)deltaTime;
    // TODO: Implement proper ECS-based following
    return NodeStatus::Running;
}

} // namespace fresh
