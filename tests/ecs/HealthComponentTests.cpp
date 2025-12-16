/**
 * @file HealthComponentTests.cpp
 * @brief Unit tests for HealthComponent demonstrating Component-Based Architecture
 */

#include <gtest/gtest.h>
#include "ecs/HealthComponent.h"
#include "ecs/EntityManager.h"

using namespace fresh::ecs;

/**
 * Test fixture for HealthComponent
 */
class HealthComponentTest : public ::testing::Test
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
 * Test basic health initialization
 */
TEST_F(HealthComponentTest, Constructor_DefaultValues_InitializesCorrectly)
{
    // Arrange & Act
    HealthComponent health(100.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);
    EXPECT_FLOAT_EQ(health.getMaxHealth(), 100.0f);
    EXPECT_FLOAT_EQ(health.getHealthPercentage(), 1.0f);
    EXPECT_TRUE(health.getIsAlive());
}

/**
 * Test taking damage
 */
TEST_F(HealthComponentTest, TakeDamage_ReducesHealth)
{
    // Arrange
    HealthComponent health(100.0f);

    // Act
    health.takeDamage(25.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 75.0f);
    EXPECT_FLOAT_EQ(health.getHealthPercentage(), 0.75f);
    EXPECT_TRUE(health.getIsAlive());
}

/**
 * Test death when health reaches zero
 */
TEST_F(HealthComponentTest, TakeDamage_ExceedsHealth_EntityDies)
{
    // Arrange
    HealthComponent health(100.0f);

    // Act
    health.takeDamage(150.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 0.0f);
    EXPECT_FLOAT_EQ(health.getHealthPercentage(), 0.0f);
    EXPECT_FALSE(health.getIsAlive());
}

/**
 * Test healing
 */
TEST_F(HealthComponentTest, Heal_IncreasesHealth)
{
    // Arrange
    HealthComponent health(100.0f);
    health.takeDamage(50.0f);

    // Act
    health.heal(25.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 75.0f);
    EXPECT_FLOAT_EQ(health.getHealthPercentage(), 0.75f);
}

/**
 * Test healing doesn't exceed max health
 */
TEST_F(HealthComponentTest, Heal_ExceedsMax_ClampsToMax)
{
    // Arrange
    HealthComponent health(100.0f);
    health.takeDamage(25.0f);

    // Act
    health.heal(50.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);
    EXPECT_FLOAT_EQ(health.getHealthPercentage(), 1.0f);
}

/**
 * Test that dead entities cannot be healed
 */
TEST_F(HealthComponentTest, Heal_WhenDead_HasNoEffect)
{
    // Arrange
    HealthComponent health(100.0f);
    health.takeDamage(100.0f);

    // Act
    health.heal(50.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 0.0f);
    EXPECT_FALSE(health.getIsAlive());
}

/**
 * Test revive functionality
 */
TEST_F(HealthComponentTest, Revive_DeadEntity_RestoresFullHealth)
{
    // Arrange
    HealthComponent health(100.0f);
    health.takeDamage(100.0f);

    // Act
    health.revive();

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);
    EXPECT_TRUE(health.getIsAlive());
}

/**
 * Test changing max health
 */
TEST_F(HealthComponentTest, SetMaxHealth_AdjustsCurrentHealth)
{
    // Arrange
    HealthComponent health(100.0f);

    // Act
    health.setMaxHealth(50.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getMaxHealth(), 50.0f);
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 50.0f); // Clamped down
}

/**
 * Test component in EntityManager (demonstrates modularity)
 */
TEST_F(HealthComponentTest, ComponentInECS_CanBeAddedAndRetrieved)
{
    // Arrange
    Entity entity = entityManager->createEntity();

    // Act
    auto* health = entityManager->addComponent(entity, new HealthComponent(100.0f));
    health->takeDamage(30.0f);

    // Assert
    auto* retrieved = entityManager->getComponent<HealthComponent>(entity);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_FLOAT_EQ(retrieved->getCurrentHealth(), 70.0f);
}

/**
 * Test multiple entities with health (demonstrates reusability)
 */
TEST_F(HealthComponentTest, MultipleEntities_EachHaveIndependentHealth)
{
    // Arrange
    Entity entity1 = entityManager->createEntity();
    Entity entity2 = entityManager->createEntity();

    entityManager->addComponent(entity1, new HealthComponent(100.0f));
    entityManager->addComponent(entity2, new HealthComponent(50.0f));

    // Act
    entityManager->getComponent<HealthComponent>(entity1)->takeDamage(25.0f);
    entityManager->getComponent<HealthComponent>(entity2)->takeDamage(10.0f);

    // Assert
    EXPECT_FLOAT_EQ(entityManager->getComponent<HealthComponent>(entity1)->getCurrentHealth(), 75.0f);
    EXPECT_FLOAT_EQ(entityManager->getComponent<HealthComponent>(entity2)->getCurrentHealth(), 40.0f);
}

/**
 * Test that damage when dead has no additional effect
 */
TEST_F(HealthComponentTest, TakeDamage_WhenDead_HasNoEffect)
{
    // Arrange
    HealthComponent health(100.0f);
    health.takeDamage(100.0f);

    // Act
    health.takeDamage(50.0f);

    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 0.0f);
    EXPECT_FALSE(health.getIsAlive());
}
