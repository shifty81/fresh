# Testing Best Practices for Component-Based Architecture

## Overview

Testing is critical for maintaining quality and enabling confident iteration in Fresh Voxel Engine. The Component-Based Architecture makes testing easier by allowing components and systems to be tested in isolation.

## Testing Philosophy

### The Testing Pyramid

```
        /\
       /  \       Few, Slow, Expensive
      / E2E \     End-to-End Tests (Manual playtesting)
     /______\
    /        \
   /Integration\   Medium quantity, Medium speed
  /____________\  Integration Tests (System interactions)
 /              \
/   Unit Tests   \ Many, Fast, Cheap
/_________________\ Unit Tests (Components, individual functions)
```

**Fresh Engine Testing Strategy:**
- **70%** Unit tests (components, utilities)
- **20%** Integration tests (systems working together)
- **10%** End-to-end tests (full gameplay scenarios)

## Unit Testing Components

### Why Test Components?

Components are pure data containers with minimal logic - perfect for unit testing:

✅ **Fast**: Run thousands of tests in seconds
✅ **Reliable**: No dependencies, predictable
✅ **Precise**: Pinpoint exact failures
✅ **Regression Prevention**: Catch bugs introduced by changes

### Example: HealthComponent Tests

```cpp
#include <gtest/gtest.h>
#include "ecs/HealthComponent.h"

TEST(HealthComponentTest, Constructor_InitializesCorrectly)
{
    // Arrange & Act
    HealthComponent health(100.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);
    EXPECT_FLOAT_EQ(health.getMaxHealth(), 100.0f);
    EXPECT_TRUE(health.getIsAlive());
}

TEST(HealthComponentTest, TakeDamage_ReducesHealth)
{
    // Arrange
    HealthComponent health(100.0f);
    
    // Act
    health.takeDamage(30.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 70.0f);
}

TEST(HealthComponentTest, TakeDamage_ExceedsHealth_EntityDies)
{
    // Arrange
    HealthComponent health(100.0f);
    
    // Act
    health.takeDamage(150.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 0.0f);
    EXPECT_FALSE(health.getIsAlive());
}

TEST(HealthComponentTest, Heal_WhenDead_HasNoEffect)
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
```

### AAA Pattern (Arrange-Act-Assert)

All tests follow the same structure:

```cpp
TEST(ComponentTest, Method_Condition_ExpectedResult)
{
    // Arrange: Set up test data
    Component component(initialValues);
    
    // Act: Perform the action being tested
    component.doSomething();
    
    // Assert: Verify the result
    EXPECT_EQ(component.getValue(), expectedValue);
}
```

### Testing Edge Cases

Don't just test the happy path:

```cpp
// Edge case: Zero health
TEST(HealthComponentTest, TakeDamage_AtZeroHealth_StaysAtZero)
{
    HealthComponent health(0.0f);
    health.takeDamage(10.0f);
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 0.0f);
}

// Edge case: Negative damage (should this heal? Or be ignored?)
TEST(HealthComponentTest, TakeDamage_NegativeAmount_Ignored)
{
    HealthComponent health(100.0f);
    health.takeDamage(-10.0f);
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);  // No change
}

// Edge case: Heal beyond max
TEST(HealthComponentTest, Heal_BeyondMax_ClampsToMax)
{
    HealthComponent health(100.0f);
    health.takeDamage(20.0f);
    health.heal(50.0f);
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 100.0f);  // Clamped
}
```

## Integration Testing Systems

### Why Test Systems?

Systems coordinate multiple components - test their interactions:

✅ **Verify component interactions**
✅ **Test event emission**
✅ **Validate system update logic**
✅ **Ensure proper state management**

### Example: CombatSystem Tests

```cpp
#include <gtest/gtest.h>
#include "ecs/CombatSystem.h"
#include "ecs/HealthComponent.h"
#include "scripting/EventSystem.h"

class CombatSystemTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        EventSystem::getInstance().clear();
        entityManager = std::make_unique<EntityManager>();
        combatSystem = std::make_unique<CombatSystem>(entityManager.get());
        combatSystem->initialize();
    }
    
    void TearDown() override
    {
        combatSystem->shutdown();
        combatSystem.reset();
        entityManager.reset();
        EventSystem::getInstance().clear();
    }
    
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<CombatSystem> combatSystem;
};

TEST_F(CombatSystemTest, ApplyDamage_EmitsDamageEvent)
{
    // Arrange
    Entity entity = entityManager->createEntity();
    entityManager->addComponent(entity, new HealthComponent(100.0f));
    
    bool eventReceived = false;
    float receivedDamage = 0.0f;
    
    EventSystem::getInstance().subscribe("entity_damaged", [&](const EventData& data) {
        eventReceived = true;
        receivedDamage = data.get<float>("damage");
    });
    
    // Act
    combatSystem->applyDamage(entity, 25.0f);
    
    // Assert
    EXPECT_TRUE(eventReceived);
    EXPECT_FLOAT_EQ(receivedDamage, 25.0f);
}

TEST_F(CombatSystemTest, Update_DetectsEntityDeath)
{
    // Arrange
    Entity entity = entityManager->createEntity();
    entityManager->addComponent(entity, new HealthComponent(100.0f));
    
    bool deathEventReceived = false;
    EventSystem::getInstance().subscribe("entity_died", [&](const EventData& data) {
        deathEventReceived = true;
    });
    
    // Act
    combatSystem->applyDamage(entity, 100.0f);
    combatSystem->update(0.016f);  // Simulate one frame
    
    // Assert
    EXPECT_TRUE(deathEventReceived);
}
```

### Test Fixtures

Use test fixtures to reduce setup code:

```cpp
class SystemTestBase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        entityManager = std::make_unique<EntityManager>();
        EventSystem::getInstance().clear();
    }
    
    void TearDown() override
    {
        entityManager.reset();
        EventSystem::getInstance().clear();
    }
    
    Entity createTestEntity()
    {
        Entity entity = entityManager->createEntity();
        entityManager->addComponent(entity, new HealthComponent(100.0f));
        entityManager->addComponent(entity, new MovementComponent(5.0f, 10.0f));
        return entity;
    }
    
    std::unique_ptr<EntityManager> entityManager;
};

// Now tests are cleaner
TEST_F(SystemTestBase, TestSomething)
{
    Entity entity = createTestEntity();  // Easy setup
    // Test logic...
}
```

## Event-Driven Testing

### Testing Event Emission

```cpp
TEST(EventSystemTest, Emit_WithData_CallbackReceivesData)
{
    // Arrange
    auto& eventSystem = EventSystem::getInstance();
    int receivedValue = 0;
    
    eventSystem.subscribe("test_event", [&](const EventData& data) {
        receivedValue = data.get<int>("value", -1);
    });
    
    EventData data;
    data.set("value", 42);
    
    // Act
    eventSystem.emit("test_event", data);
    
    // Assert
    EXPECT_EQ(receivedValue, 42);
}
```

### Testing Event Subscribers

```cpp
TEST(AudioSystemTest, ReactsToEntityDamage)
{
    // Arrange
    AudioSystem audioSystem;
    audioSystem.initialize();
    
    // Act - Simulate damage event
    EventData data;
    data.set("entity", 123);
    data.set("damage", 50.0f);
    EventSystem::getInstance().emit("entity_damaged", data);
    
    // Assert - Verify audio system reacted
    EXPECT_TRUE(audioSystem.isSoundPlaying("impact_heavy.wav"));
}
```

### Testing Event Chains

```cpp
TEST(GameplayTest, BarrelExplosion_CausesChainReaction)
{
    // Arrange
    Entity barrel1 = createExplosiveBarrel(glm::vec3(0, 0, 0));
    Entity barrel2 = createExplosiveBarrel(glm::vec3(2, 0, 0));  // Nearby
    
    int explosionCount = 0;
    EventSystem::getInstance().subscribe("explosion", [&](const EventData& data) {
        explosionCount++;
    });
    
    // Act - Destroy first barrel
    combatSystem->applyDamage(barrel1, 100.0f);
    combatSystem->update(0.016f);  // Process death event
    explosionSystem->update(0.016f);  // Process explosion event
    
    // Assert - Both barrels exploded
    EXPECT_EQ(explosionCount, 2);  // Chain reaction!
}
```

## Parameterized Tests

Test the same logic with different inputs:

```cpp
class DamageTestParams : public ::testing::TestWithParam<std::tuple<float, float, float>>
{
protected:
    // health, damage, expectedRemaining
};

TEST_P(DamageTestParams, TakeDamage_Various)
{
    auto [health, damage, expected] = GetParam();
    
    HealthComponent component(health);
    component.takeDamage(damage);
    
    EXPECT_FLOAT_EQ(component.getCurrentHealth(), expected);
}

INSTANTIATE_TEST_SUITE_P(
    DamageScenarios,
    DamageTestParams,
    ::testing::Values(
        std::make_tuple(100.0f, 30.0f, 70.0f),   // Normal damage
        std::make_tuple(100.0f, 150.0f, 0.0f),   // Overkill
        std::make_tuple(50.0f, 50.0f, 0.0f),     // Exact kill
        std::make_tuple(100.0f, 0.0f, 100.0f)    // No damage
    )
);
```

## Data-Driven Testing

Test configuration loading:

```cpp
TEST(ConfigLoaderTest, LoadCharacterStats_ValidFile_ReturnsCorrectData)
{
    // Arrange
    const char* testConfig = R"({
        "character_types": [
            {
                "id": "test_character",
                "health": 123.0,
                "maxSpeed": 4.5
            }
        ]
    })";
    
    // Write test config to temp file
    std::ofstream file("test_config.json");
    file << testConfig;
    file.close();
    
    // Act
    auto stats = CharacterStatsLoader::loadStats("test_config.json");
    
    // Assert
    EXPECT_EQ(stats["test_character"].health, 123.0f);
    EXPECT_EQ(stats["test_character"].maxSpeed, 4.5f);
    
    // Cleanup
    std::remove("test_config.json");
}
```

## Combinatorial Testing

Test combinations of components:

```cpp
// Test all combinations of health and movement states
struct ComponentCombo {
    bool hasHealth;
    bool hasMovement;
    bool shouldProcess;
};

class ComboTest : public ::testing::TestWithParam<ComponentCombo> {};

TEST_P(ComboTest, System_ProcessesCorrectEntities)
{
    auto combo = GetParam();
    
    Entity entity = entityManager->createEntity();
    if (combo.hasHealth) {
        entityManager->addComponent(entity, new HealthComponent(100.0f));
    }
    if (combo.hasMovement) {
        entityManager->addComponent(entity, new MovementComponent(5.0f, 10.0f));
    }
    
    // System that requires BOTH components
    auto entities = entityManager->getEntitiesWithComponents<HealthComponent, MovementComponent>();
    
    bool wasProcessed = std::find(entities.begin(), entities.end(), entity) != entities.end();
    EXPECT_EQ(wasProcessed, combo.shouldProcess);
}

INSTANTIATE_TEST_SUITE_P(
    ComponentCombinations,
    ComboTest,
    ::testing::Values(
        ComponentCombo{true, true, true},    // Both → process
        ComponentCombo{true, false, false},  // Only health → don't process
        ComponentCombo{false, true, false},  // Only movement → don't process
        ComponentCombo{false, false, false}  // Neither → don't process
    )
);
```

## Mock Objects

For testing systems with external dependencies:

```cpp
// Mock audio engine
class MockAudioEngine : public IAudioEngine
{
public:
    MOCK_METHOD(void, playSound, (const std::string& filename), (override));
    MOCK_METHOD(bool, isSoundPlaying, (const std::string& filename), (const, override));
};

TEST(AudioSystemTest, PlaysSoundOnDamage)
{
    // Arrange
    MockAudioEngine mockAudio;
    AudioSystem audioSystem(&mockAudio);
    
    // Expect playSound to be called with correct file
    EXPECT_CALL(mockAudio, playSound("impact_heavy.wav"))
        .Times(1);
    
    // Act
    EventData data;
    data.set("damage", 50.0f);
    EventSystem::getInstance().emit("entity_damaged", data);
    
    // Assert - Mock verifies playSound was called
}
```

## Performance Testing

Test that systems perform well:

```cpp
TEST(PerformanceTest, CombatSystem_Handles1000Entities)
{
    // Arrange
    const int entityCount = 1000;
    for (int i = 0; i < entityCount; ++i) {
        Entity entity = entityManager->createEntity();
        entityManager->addComponent(entity, new HealthComponent(100.0f));
    }
    
    // Act
    auto startTime = std::chrono::high_resolution_clock::now();
    
    combatSystem->update(0.016f);  // One frame
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Assert - Should complete in less than 16ms (60 FPS)
    EXPECT_LT(duration.count(), 16);
}
```

## Test Organization

### File Structure

```
tests/
  ecs/
    HealthComponentTests.cpp
    MovementComponentTests.cpp
    CombatSystemTests.cpp
    Transform2DTests.cpp
  integration/
    CombatIntegrationTest.cpp
    EventDrivenGameplayTest.cpp
  performance/
    EntityManagerPerformanceTest.cpp
    SystemUpdatePerformanceTest.cpp
```

### Naming Conventions

```cpp
// Test Suite: ComponentName + "Test"
class HealthComponentTest : public ::testing::Test {};

// Test Case: Method_Condition_ExpectedResult
TEST(HealthComponentTest, TakeDamage_ReducesHealth)
TEST(HealthComponentTest, TakeDamage_WhenDead_HasNoEffect)
TEST(HealthComponentTest, Heal_BeyondMax_ClampsToMax)
```

## Running Tests

### Command Line

```bash
# Build tests
cmake --build build --config Release --target FreshVoxelEngineTests

# Run all tests
./build/Release/FreshVoxelEngineTests

# Run specific test suite
./build/Release/FreshVoxelEngineTests --gtest_filter="HealthComponentTest.*"

# Run specific test
./build/Release/FreshVoxelEngineTests --gtest_filter="HealthComponentTest.TakeDamage_ReducesHealth"

# Verbose output
./build/Release/FreshVoxelEngineTests --gtest_print_time=1
```

### Visual Studio

1. Build solution (F7)
2. Open Test Explorer (Test → Test Explorer)
3. Click "Run All" or run specific tests
4. View results in Test Explorer window

## Best Practices

### ✅ DO

- **Write tests as you code**: Don't wait until the end
- **Test edge cases**: Zero, negative, maximum values
- **Use descriptive test names**: Reader should know what's tested
- **Follow AAA pattern**: Arrange, Act, Assert
- **Keep tests independent**: Each test should run alone
- **Test one thing per test**: Easier to debug failures
- **Use test fixtures**: Reduce setup duplication
- **Mock external dependencies**: Keep tests fast
- **Run tests frequently**: Catch bugs early

### ❌ DON'T

- **Don't test implementation details**: Test behavior, not internals
- **Don't write flaky tests**: Tests should be deterministic
- **Don't skip cleanup**: Always reset state in TearDown
- **Don't test framework code**: Trust that Google Test works
- **Don't make tests depend on each other**: Order shouldn't matter
- **Don't use production data**: Use test-specific data
- **Don't ignore failing tests**: Fix or remove, don't skip

## Test-Driven Development (TDD)

### Red-Green-Refactor Cycle

```
1. RED: Write a failing test
2. GREEN: Write minimal code to pass
3. REFACTOR: Improve code while keeping tests green
```

**Example:**

```cpp
// Step 1: RED - Write failing test
TEST(StaminaComponentTest, Consume_ReducesStamina)
{
    StaminaComponent stamina(100.0f);
    stamina.consume(30.0f);
    EXPECT_FLOAT_EQ(stamina.getCurrentStamina(), 70.0f);
}

// Compile error: StaminaComponent doesn't exist yet

// Step 2: GREEN - Minimal implementation
class StaminaComponent : public IComponent
{
public:
    StaminaComponent(float max) : maxStamina(max), currentStamina(max) {}
    COMPONENT_TYPE(StaminaComponent)
    
    void consume(float amount) {
        currentStamina -= amount;
    }
    
    float getCurrentStamina() const { return currentStamina; }
    
private:
    float maxStamina;
    float currentStamina;
};

// Test passes!

// Step 3: REFACTOR - Improve implementation
void consume(float amount) {
    currentStamina = std::max(0.0f, currentStamina - amount);  // Don't go negative
}

// Test still passes!
```

## CI/CD Integration

Tests run automatically on every commit:

```yaml
# .github/workflows/tests.yml
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build
        run: cmake --build build --config Release
      - name: Run tests
        run: ./build/Release/FreshVoxelEngineTests --gtest_output=xml:test-results.xml
      - name: Publish results
        uses: EnricoMi/publish-unit-test-result-action@v1
        with:
          files: test-results.xml
```

## Measuring Test Quality

### Code Coverage

Track which code is tested:

```bash
# Run tests with coverage
cmake -DCMAKE_BUILD_TYPE=Coverage ..
make coverage

# View coverage report
# Target: >80% coverage for critical code
```

### Test Metrics

- **Test count**: More tests = more confidence
- **Test speed**: Faster tests = run more often
- **Test stability**: No flaky tests allowed
- **Coverage**: % of code exercised by tests

## Example: Complete Test Suite

See these files for complete examples:
- `tests/ecs/HealthComponentTests.cpp`
- `tests/ecs/MovementComponentTests.cpp`
- `tests/ecs/CombatSystemTests.cpp`
- `tests/scripting/EventSystemTests.cpp`

## Further Reading

- [Google Test Documentation](https://google.github.io/googletest/)
- [COMPONENT_BASED_ARCHITECTURE.md](../architecture/COMPONENT_BASED_ARCHITECTURE.md)
- [EVENT_DRIVEN_ARCHITECTURE.md](../architecture/EVENT_DRIVEN_ARCHITECTURE.md)
- [ITERATIVE_DEVELOPMENT.md](ITERATIVE_DEVELOPMENT.md)
