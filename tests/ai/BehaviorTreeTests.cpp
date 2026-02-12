#include <gtest/gtest.h>
#include "ai/BehaviorTree.h"
#include "ecs/Entity.h"

using namespace fresh;

/**
 * @brief Test suite for BehaviorTree, including node execution and AI behaviors
 */
class BehaviorTreeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        entity = ecs::Entity(1);
    }

    ecs::Entity entity;
};

// ============================================================================
// BehaviorTree core tests
// ============================================================================

TEST_F(BehaviorTreeTest, EmptyTreeReturnsFailure)
{
    BehaviorTree tree;
    EXPECT_EQ(tree.tick(&entity, 0.016f), NodeStatus::Failure);
}

// Simple behavior that always succeeds
class SucceedBehavior : public BehaviorNode
{
public:
    NodeStatus execute(ecs::Entity* /* entity */, float /* deltaTime */) override
    {
        return NodeStatus::Success;
    }
};

// Simple behavior that always fails
class FailBehavior : public BehaviorNode
{
public:
    NodeStatus execute(ecs::Entity* /* entity */, float /* deltaTime */) override
    {
        return NodeStatus::Failure;
    }
};

TEST_F(BehaviorTreeTest, TreeExecutesRoot)
{
    BehaviorTree tree;
    SucceedBehavior root;
    tree.setRoot(&root);
    EXPECT_EQ(tree.tick(&entity, 0.016f), NodeStatus::Success);
}

// ============================================================================
// SequenceNode tests
// ============================================================================

TEST_F(BehaviorTreeTest, SequenceSucceedsWhenAllChildrenSucceed)
{
    SequenceNode sequence;
    SucceedBehavior child1, child2, child3;
    sequence.addChild(&child1);
    sequence.addChild(&child2);
    sequence.addChild(&child3);

    EXPECT_EQ(sequence.execute(&entity, 0.016f), NodeStatus::Success);
}

TEST_F(BehaviorTreeTest, SequenceFailsOnFirstFailure)
{
    SequenceNode sequence;
    SucceedBehavior child1;
    FailBehavior child2;
    SucceedBehavior child3;
    sequence.addChild(&child1);
    sequence.addChild(&child2);
    sequence.addChild(&child3);

    EXPECT_EQ(sequence.execute(&entity, 0.016f), NodeStatus::Failure);
}

// ============================================================================
// SelectorNode tests
// ============================================================================

TEST_F(BehaviorTreeTest, SelectorSucceedsOnFirstSuccess)
{
    SelectorNode selector;
    FailBehavior child1;
    SucceedBehavior child2;
    FailBehavior child3;
    selector.addChild(&child1);
    selector.addChild(&child2);
    selector.addChild(&child3);

    EXPECT_EQ(selector.execute(&entity, 0.016f), NodeStatus::Success);
}

TEST_F(BehaviorTreeTest, SelectorFailsWhenAllChildrenFail)
{
    SelectorNode selector;
    FailBehavior child1, child2, child3;
    selector.addChild(&child1);
    selector.addChild(&child2);
    selector.addChild(&child3);

    EXPECT_EQ(selector.execute(&entity, 0.016f), NodeStatus::Failure);
}

// ============================================================================
// WanderBehavior tests
// ============================================================================

TEST_F(BehaviorTreeTest, WanderFailsWithNullEntity)
{
    WanderBehavior wander(nullptr);
    EXPECT_EQ(wander.execute(nullptr, 0.016f), NodeStatus::Failure);
}

TEST_F(BehaviorTreeTest, WanderFailsWithInvalidEntity)
{
    WanderBehavior wander(nullptr);
    ecs::Entity invalid;
    EXPECT_EQ(wander.execute(&invalid, 0.016f), NodeStatus::Failure);
}

TEST_F(BehaviorTreeTest, WanderReturnsRunningWithValidEntity)
{
    WanderBehavior wander(nullptr);
    // First tick should pick a new target and return Running
    NodeStatus status = wander.execute(&entity, 0.016f);
    // Should be Running (newly picked target is unlikely to be at origin)
    // or Success if the random target happens to be very close to zero
    EXPECT_TRUE(status == NodeStatus::Running || status == NodeStatus::Success);
}

TEST_F(BehaviorTreeTest, WanderEventuallySucceeds)
{
    WanderBehavior wander(nullptr);
    // After the wander timer expires multiple times, eventually the target
    // should be reached (random chance of zero-ish vector)
    // Test that it doesn't crash over many ticks
    for (int i = 0; i < 100; ++i) {
        NodeStatus status = wander.execute(&entity, 0.1f);
        EXPECT_NE(status, NodeStatus::Failure);
    }
}

// ============================================================================
// FollowBehavior tests
// ============================================================================

TEST_F(BehaviorTreeTest, FollowFailsWithNullEntity)
{
    ecs::Entity target(2);
    FollowBehavior follow(&target, 5.0f);
    EXPECT_EQ(follow.execute(nullptr, 0.016f), NodeStatus::Failure);
}

TEST_F(BehaviorTreeTest, FollowFailsWithNullTarget)
{
    FollowBehavior follow(nullptr, 5.0f);
    EXPECT_EQ(follow.execute(&entity, 0.016f), NodeStatus::Failure);
}

TEST_F(BehaviorTreeTest, FollowFailsWithInvalidTarget)
{
    ecs::Entity invalidTarget;
    FollowBehavior follow(&invalidTarget, 5.0f);
    EXPECT_EQ(follow.execute(&entity, 0.016f), NodeStatus::Failure);
}

TEST_F(BehaviorTreeTest, FollowReturnsRunningWithValidEntities)
{
    ecs::Entity target(2);
    FollowBehavior follow(&target, 5.0f);
    EXPECT_EQ(follow.execute(&entity, 0.016f), NodeStatus::Running);
}
