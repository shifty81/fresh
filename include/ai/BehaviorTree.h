#pragma once

namespace fresh {

class Entity;
class VoxelWorld;

/**
 * @brief Basic behavior tree node types
 */
enum class NodeStatus {
    Success,
    Failure,
    Running
};

/**
 * @brief Base node for behavior tree
 */
class BehaviorNode {
public:
    virtual ~BehaviorNode() = default;
    virtual NodeStatus execute(Entity* entity, float deltaTime) = 0;
};

/**
 * @brief Behavior tree for AI decision making
 */
class BehaviorTree {
public:
    BehaviorTree();
    ~BehaviorTree();
    
    /**
     * @brief Set the root node of the behavior tree
     */
    void setRoot(BehaviorNode* root);
    
    /**
     * @brief Execute the behavior tree
     */
    NodeStatus tick(Entity* entity, float deltaTime);
    
private:
    BehaviorNode* rootNode;
};

/**
 * @brief Sequence node - executes children in order until one fails
 */
class SequenceNode : public BehaviorNode {
public:
    void addChild(BehaviorNode* child);
    NodeStatus execute(Entity* entity, float deltaTime) override;
    
private:
    std::vector<BehaviorNode*> children;
};

/**
 * @brief Selector node - executes children until one succeeds
 */
class SelectorNode : public BehaviorNode {
public:
    void addChild(BehaviorNode* child);
    NodeStatus execute(Entity* entity, float deltaTime) override;
    
private:
    std::vector<BehaviorNode*> children;
};

/**
 * @brief Example behavior: Wander randomly
 */
class WanderBehavior : public BehaviorNode {
public:
    WanderBehavior(VoxelWorld* world);
    NodeStatus execute(Entity* entity, float deltaTime) override;
    
private:
    VoxelWorld* world;
    float wanderTimer;
    glm::vec3 targetPosition;
};

/**
 * @brief Example behavior: Follow target
 */
class FollowBehavior : public BehaviorNode {
public:
    FollowBehavior(Entity* target, float followDistance = 5.0f);
    NodeStatus execute(Entity* entity, float deltaTime) override;
    
private:
    Entity* targetEntity;
    float followDistance;
};

} // namespace fresh
