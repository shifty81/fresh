/**
 * @file MovementComponentTests.cpp
 * @brief Unit tests for MovementComponent demonstrating Component-Based Architecture
 */

#include <gtest/gtest.h>
#include "ecs/MovementComponent.h"
#include "ecs/EntityManager.h"
#include <glm/glm.hpp>

using namespace fresh::ecs;

/**
 * Test fixture for MovementComponent
 */
class MovementComponentTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        entityManager = std::make_unique<EntityManager>();
    }

    void TearDown() override
    {
        entityManager.reset();
    }

    std::unique_ptr<EntityManager> entityManager;
};

/**
 * Test basic movement initialization
 */
TEST_F(MovementComponentTest, Constructor_DefaultValues_InitializesCorrectly)
{
    // Arrange & Act
    MovementComponent movement(5.0f, 10.0f);

    // Assert
    EXPECT_FLOAT_EQ(movement.getMaxSpeed(), 5.0f);
    EXPECT_FLOAT_EQ(movement.getAcceleration(), 10.0f);
    EXPECT_FLOAT_EQ(movement.getCurrentSpeed(), 0.0f);
    EXPECT_TRUE(movement.getCanMove());
}

/**
 * Test setting desired direction
 */
TEST_F(MovementComponentTest, SetDesiredDirection_NormalizesVector)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);

    // Act
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 1.0f));

    // Assert
    glm::vec3 dir = movement.getDesiredDirection();
    float length = glm::length(dir);
    EXPECT_NEAR(length, 1.0f, 0.001f); // Should be normalized
}

/**
 * Test movement update increases velocity
 */
TEST_F(MovementComponentTest, Update_WithDirection_IncreasesVelocity)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));

    // Act
    movement.update(0.1f);

    // Assert
    EXPECT_GT(movement.getCurrentSpeed(), 0.0f);
    EXPECT_LE(movement.getCurrentSpeed(), 5.0f); // Should not exceed max speed
}

/**
 * Test movement converges to max speed
 */
TEST_F(MovementComponentTest, Update_OverTime_ConvergesToMaxSpeed)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));

    // Act - update for 1 second
    for (int i = 0; i < 10; ++i) {
        movement.update(0.1f);
    }

    // Assert
    EXPECT_NEAR(movement.getCurrentSpeed(), 5.0f, 0.1f);
}

/**
 * Test stop resets velocity
 */
TEST_F(MovementComponentTest, Stop_ResetsVelocity)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));
    movement.update(0.5f);

    // Act
    movement.stop();

    // Assert
    EXPECT_FLOAT_EQ(movement.getCurrentSpeed(), 0.0f);
}

/**
 * Test disabling movement
 */
TEST_F(MovementComponentTest, SetCanMove_False_StopsMovement)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));
    movement.update(0.5f);

    // Act
    movement.setCanMove(false);
    movement.update(0.1f);

    // Assert
    EXPECT_FLOAT_EQ(movement.getCurrentSpeed(), 0.0f);
    EXPECT_FALSE(movement.getCanMove());
}

/**
 * Test changing max speed
 */
TEST_F(MovementComponentTest, SetMaxSpeed_AffectsMovement)
{
    // Arrange
    MovementComponent movement(5.0f, 100.0f); // High acceleration
    movement.setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));

    // Act
    movement.update(1.0f); // Should reach max speed
    float speedAtMax5 = movement.getCurrentSpeed();

    movement.setMaxSpeed(10.0f);
    movement.update(1.0f); // Should accelerate to new max

    // Assert
    EXPECT_NEAR(speedAtMax5, 5.0f, 0.1f);
    EXPECT_NEAR(movement.getCurrentSpeed(), 10.0f, 0.1f);
}

/**
 * Test component in EntityManager (demonstrates modularity)
 */
TEST_F(MovementComponentTest, ComponentInECS_CanBeAddedAndRetrieved)
{
    // Arrange
    Entity entity = entityManager->createEntity();

    // Act
    auto* movement = entityManager->addComponent(entity, new MovementComponent(5.0f, 10.0f));
    movement->setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));
    movement->update(0.5f);

    // Assert
    auto* retrieved = entityManager->getComponent<MovementComponent>(entity);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_GT(retrieved->getCurrentSpeed(), 0.0f);
}

/**
 * Test multiple entities with independent movement
 */
TEST_F(MovementComponentTest, MultipleEntities_EachHaveIndependentMovement)
{
    // Arrange
    Entity entity1 = entityManager->createEntity();
    Entity entity2 = entityManager->createEntity();

    auto* movement1 = entityManager->addComponent(entity1, new MovementComponent(5.0f, 10.0f));
    auto* movement2 = entityManager->addComponent(entity2, new MovementComponent(3.0f, 5.0f));

    // Act
    movement1->setDesiredDirection(glm::vec3(1.0f, 0.0f, 0.0f));
    movement2->setDesiredDirection(glm::vec3(0.0f, 1.0f, 0.0f));

    movement1->update(1.0f);
    movement2->update(1.0f);

    // Assert
    EXPECT_NEAR(movement1->getCurrentSpeed(), 5.0f, 0.1f);
    EXPECT_NEAR(movement2->getCurrentSpeed(), 3.0f, 0.1f);
}

/**
 * Test zero direction doesn't cause issues
 */
TEST_F(MovementComponentTest, SetDesiredDirection_ZeroVector_HandledSafely)
{
    // Arrange
    MovementComponent movement(5.0f, 10.0f);

    // Act & Assert - should not crash
    EXPECT_NO_THROW(movement.setDesiredDirection(glm::vec3(0.0f, 0.0f, 0.0f)));
    movement.update(0.1f);
    EXPECT_FLOAT_EQ(movement.getCurrentSpeed(), 0.0f);
}
