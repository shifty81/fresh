# Iterative Development & Rapid Prototyping Guide

## Overview

Fresh Voxel Engine is designed to support **iterative development** and **rapid prototyping** workflows. The combination of Component-Based Architecture, Event-Driven Architecture, and Data-Driven Design enables developers to:

- Build and test features quickly
- Iterate on gameplay mechanics rapidly
- Gather feedback early and often
- Refactor with confidence
- Minimize wasted development time

## The Iterative Development Cycle

### 1. Prototype Phase: Make It Work

**Goal**: Test the core idea as quickly as possible

```cpp
// Quick prototype: Basic enemy AI
class QuickEnemyAI {
public:
    void update(Entity enemy, Entity player) {
        // Simplest possible implementation
        auto* enemyPos = entityManager->getComponent<TransformComponent>(enemy);
        auto* playerPos = entityManager->getComponent<TransformComponent>(player);
        
        // Move toward player (no pathfinding, no optimization)
        glm::vec3 direction = playerPos->getPosition() - enemyPos->getPosition();
        if (glm::length(direction) > 0) {
            direction = glm::normalize(direction);
            enemyPos->setPosition(enemyPos->getPosition() + direction * 0.1f);
        }
    }
};
```

**At this stage:**
- ❌ Don't worry about performance
- ❌ Don't add polish
- ❌ Don't over-engineer
- ✅ Focus on testing the concept
- ✅ Use placeholder art/sounds
- ✅ Get it playable ASAP

### 2. Feedback Phase: Make It Fun

**Goal**: Identify what works and what doesn't

**Playtest with:**
- Yourself
- Team members
- Target audience (if possible)

**Ask:**
- Is the core mechanic fun?
- What feels good?
- What feels frustrating?
- What's confusing?

**Collect data:**
```cpp
// Add telemetry to prototype
EventSystem::getInstance().subscribe("enemy_defeated", [](const EventData& data) {
    float combatDuration = data.get<float>("duration");
    metrics.recordCombatDuration(combatDuration);
});

// After playtesting
std::cout << "Average combat: " << metrics.getAverageCombatDuration() << "s" << std::endl;
```

### 3. Iteration Phase: Make It Better

**Goal**: Refine based on feedback

**Data-Driven Iteration:**
```json
// Easy to tweak without recompiling
{
  "enemy_zombie": {
    "health": 50.0,      // Was 40 - too easy
    "speed": 2.5,        // Was 3.0 - too fast  
    "damage": 12.0,      // Was 10 - not threatening enough
    "detectionRange": 15.0  // New property based on feedback
  }
}
```

**Component-Based Iteration:**
```cpp
// Add new component without changing existing code
entityManager->addComponent(enemy, new DetectionComponent(15.0f));

// Test with new behavior
// If it works: keep it
// If it doesn't: remove component, no harm done
```

**Event-Based Iteration:**
```cpp
// Add audio feedback based on playtest comments
EventSystem::getInstance().subscribe("enemy_spotted", [](const EventData& data) {
    audioEngine->playSound("alert.wav");  // Added based on feedback
});
```

### 4. Polish Phase: Make It Shine

**Goal**: Refine the implementation

Now that the mechanic is proven fun:
- Optimize performance
- Add visual polish
- Improve code quality
- Add edge case handling
- Write comprehensive tests

```cpp
// Refactored from prototype
class EnemyAISystem : public SystemBase {
public:
    void update(float deltaTime) override {
        // Optimized: spatial partitioning, batch processing
        auto enemies = entityManager->getEntitiesWithComponent<EnemyAIComponent>();
        
        for (const auto& enemy : enemies) {
            updateEnemyBehavior(enemy, deltaTime);
        }
    }
    
private:
    void updateEnemyBehavior(Entity enemy, float deltaTime) {
        // Proper state machine
        // Pathfinding
        // Animation blending
        // Sound positioning
    }
};
```

## Rapid Prototyping Techniques

### Technique 1: Component Composition

**Quickly try different behaviors:**

```cpp
// Prototype 1: Aggressive enemy
Entity aggressive = entityManager->createEntity();
entityManager->addComponent(aggressive, new HealthComponent(50.0f));
entityManager->addComponent(aggressive, new MovementComponent(5.0f, 10.0f));  // Fast
entityManager->addComponent(aggressive, new MeleeAttackComponent(15.0f));     // High damage

// Prototype 2: Defensive enemy (same code, different components)
Entity defensive = entityManager->createEntity();
entityManager->addComponent(defensive, new HealthComponent(100.0f));   // More health
entityManager->addComponent(defensive, new MovementComponent(2.0f, 5.0f));   // Slower
entityManager->addComponent(defensive, new ShieldComponent(0.5f));     // Damage reduction

// Test both, see which is more fun, iterate
```

### Technique 2: Event-Driven Experimentation

**Add behaviors without touching core code:**

```cpp
// Experiment: Enemies call for backup when damaged
EventSystem::getInstance().subscribe("entity_damaged", [](const EventData& data) {
    Entity damaged(data.get<Entity::ID>("entity"));
    
    // Is it an enemy?
    if (entityManager->hasComponent<EnemyAIComponent>(damaged)) {
        // Spawn reinforcements
        spawnEnemiesNearby(damaged, 2);
    }
});

// Test it
// Fun? Keep it and formalize
// Not fun? Remove subscription, no code changes needed
```

### Technique 3: Data-Driven Experimentation

**A/B test different configurations:**

```json
// config/experiments/fast_combat.json
{
  "player": {
    "health": 100,
    "damage": 25,      // 2.5x normal
    "attackSpeed": 2.0  // 2x normal
  },
  "enemies": {
    "health_multiplier": 0.5  // Half health
  }
}

// config/experiments/slow_tactical.json
{
  "player": {
    "health": 150,      // More health
    "damage": 8,        // Less damage
    "attackSpeed": 0.5  // Slower attacks
  },
  "enemies": {
    "health_multiplier": 1.5  // More health
  }
}
```

```cpp
// Easy to switch between experiments
loadConfig("config/experiments/fast_combat.json");
// or
loadConfig("config/experiments/slow_tactical.json");

// Playtest both, see which feels better
```

### Technique 4: Incremental Complexity

**Start simple, add complexity only when needed:**

```cpp
// Version 1: Basic AI (10 lines)
void updateAI(Entity enemy) {
    moveTowardPlayer(enemy);
}

// Playtest: Too simple, enemies just run at player

// Version 2: Add detection range (15 lines)
void updateAI(Entity enemy) {
    if (canSeePlayer(enemy)) {
        moveTowardPlayer(enemy);
    }
}

// Playtest: Better, but enemies are stupid

// Version 3: Add state machine (50 lines)
void updateAI(Entity enemy) {
    switch (getState(enemy)) {
        case AIState::Idle: handleIdleState(enemy); break;
        case AIState::Alert: handleAlertState(enemy); break;
        case AIState::Combat: handleCombatState(enemy); break;
    }
}

// Playtest: Much better!

// Version 4: Add pathfinding, tactical positioning, etc. (200+ lines)
// Only add after V3 is proven fun
```

## Testing Strategies for Rapid Iteration

### Unit Tests for Components

Fast to write, fast to run, catch bugs early:

```cpp
TEST(HealthComponentTest, TakeDamage_ReducesHealth) {
    HealthComponent health(100.0f);
    health.takeDamage(30.0f);
    EXPECT_FLOAT_EQ(health.getCurrentHealth(), 70.0f);
}

// Takes milliseconds to run
// Gives immediate feedback
// Prevents regressions during iteration
```

### Integration Tests for Systems

Verify systems work together:

```cpp
TEST(CombatSystemTest, DamageEmitsEvent) {
    CombatSystem combat(entityManager);
    Entity enemy = entityManager->createEntity();
    entityManager->addComponent(enemy, new HealthComponent(100.0f));
    
    bool eventReceived = false;
    EventSystem::getInstance().subscribe("entity_damaged", [&](const EventData& data) {
        eventReceived = true;
    });
    
    combat.applyDamage(enemy, 30.0f);
    
    EXPECT_TRUE(eventReceived);
}
```

### Manual Playtesting

Essential for evaluating fun:

```cpp
// Add debug tools for playtesting
class DebugTools {
public:
    void enableGodMode() {
        // Infinite health for testing
        EventSystem::getInstance().subscribe("entity_damaged", [](const EventData& data) {
            Entity entity(data.get<Entity::ID>("entity"));
            if (isPlayer(entity)) {
                auto* health = entityManager->getComponent<HealthComponent>(entity);
                health->heal(1000.0f);  // Instant heal
            }
        });
    }
    
    void spawnEnemy(const std::string& type) {
        // Quickly spawn enemies for testing
        Entity enemy = createCharacterFromConfig(type, entityManager);
        placeInFrontOfPlayer(enemy);
    }
    
    void adjustGameSpeed(float multiplier) {
        // Speed up/slow down for testing
        Time::setTimeScale(multiplier);
    }
};

// In game, press F1 for debug menu
if (input->wasKeyPressed(Key::F1)) {
    debugTools.enableGodMode();
    debugTools.adjustGameSpeed(2.0f);  // 2x speed for faster iteration
}
```

## Real-World Example: Adding a Dash Mechanic

### Iteration 1: Prototype (1 hour)

```cpp
// Add component
class DashComponent : public IComponent {
public:
    COMPONENT_TYPE(DashComponent)
    
    float dashSpeed = 20.0f;
    float dashDuration = 0.3f;
    float cooldown = 2.0f;
};

// Add to player
entityManager->addComponent(player, new DashComponent());

// Quick implementation in update loop
if (input->wasKeyPressed(Key::Space)) {
    auto* dash = entityManager->getComponent<DashComponent>(player);
    auto* movement = entityManager->getComponent<MovementComponent>(player);
    movement->setMaxSpeed(dash->dashSpeed);  // Hacky but works
}
```

**Playtest feedback:**
- ✅ Dash feels good
- ❌ Too overpowered (no cooldown)
- ❌ Confusing (no visual feedback)

### Iteration 2: Refine (2 hours)

```cpp
// Add proper cooldown tracking
class DashComponent : public IComponent {
public:
    COMPONENT_TYPE(DashComponent)
    
    float dashSpeed = 20.0f;
    float dashDuration = 0.3f;
    float cooldown = 2.0f;
    
    float cooldownRemaining = 0.0f;
    float dashTimeRemaining = 0.0f;
    
    bool canDash() const { return cooldownRemaining <= 0.0f; }
};

// Better system
class DashSystem : public SystemBase {
public:
    void update(float deltaTime) override {
        auto entities = entityManager->getEntitiesWithComponents<DashComponent, MovementComponent>();
        
        for (const auto& entity : entities) {
            auto* dash = entityManager->getComponent<DashComponent>(entity);
            
            // Update cooldown
            if (dash->cooldownRemaining > 0) {
                dash->cooldownRemaining -= deltaTime;
            }
            
            // Handle dash
            if (dash->dashTimeRemaining > 0) {
                dash->dashTimeRemaining -= deltaTime;
                // Boost speed while dashing
            } else {
                // Normal speed
            }
        }
    }
};

// Emit event for visual feedback
if (startedDash) {
    EventData data;
    data.set("entity", entity.getId());
    EventSystem::getInstance().emit("dash_started", data);
}

// Particle system subscribes
EventSystem::getInstance().subscribe("dash_started", [](const EventData& data) {
    particleSystem->emitDashTrail(position);
});
```

**Playtest feedback:**
- ✅ Cooldown works well
- ✅ Visual feedback is good
- ❌ Values need tweaking (too fast/slow)

### Iteration 3: Data-Driven Balance (30 minutes)

```json
// config/abilities.json
{
  "dash": {
    "speed": 20.0,
    "duration": 0.3,
    "cooldown": 2.0,
    "particleEffect": "dash_trail"
  }
}
```

```cpp
// Load from config
auto dashConfig = loadAbilityConfig("dash");
auto* dash = new DashComponent();
dash->dashSpeed = dashConfig["speed"];
dash->dashDuration = dashConfig["duration"];
dash->cooldown = dashConfig["cooldown"];
```

**Iterate on JSON values:**
- Try speed: 15.0 → Too slow
- Try speed: 25.0 → Too fast
- Try speed: 18.0 → Perfect!
- Try cooldown: 1.5 → Too spammy
- Try cooldown: 2.5 → Too restrictive
- Try cooldown: 2.0 → Just right!

**Total time**: 3.5 hours from idea to polished feature

## Best Practices for Rapid Iteration

### ✅ DO

- **Start with the simplest implementation**
- **Get it playable as fast as possible**
- **Test early and often**
- **Use data-driven configs for values**
- **Add components for new behaviors**
- **Use events for system communication**
- **Write tests as you go**
- **Iterate based on feedback, not assumptions**
- **Add debug tools to speed up testing**
- **Don't be afraid to throw away code**

### ❌ DON'T

- **Don't optimize prematurely**
- **Don't add features "just in case"**
- **Don't worry about perfect code in prototypes**
- **Don't skip playtesting**
- **Don't get attached to your first implementation**
- **Don't add complexity before proving the concept**
- **Don't hardcode values that should be configurable**
- **Don't ignore negative feedback**

## Measuring Development Velocity

Track how quickly you can iterate:

```
Iteration Cycle Time = Time from idea → playable prototype

Good velocity:
- Simple feature: 1-4 hours
- Medium feature: 4-8 hours
- Complex feature: 1-2 days

If it's taking longer, ask:
- Am I over-engineering?
- Am I adding unnecessary features?
- Do I need better tools/workflows?
```

## Tools for Rapid Iteration

### In-Engine Console

```cpp
class DeveloperConsole {
public:
    void executeCommand(const std::string& command) {
        if (command.starts_with("spawn ")) {
            std::string enemyType = command.substr(6);
            spawnEnemy(enemyType);
        }
        else if (command.starts_with("set_health ")) {
            float health = std::stof(command.substr(11));
            player->getComponent<HealthComponent>()->setMaxHealth(health);
        }
        // etc.
    }
};

// In game: Press ~ to open console
// > spawn enemy_zombie
// > set_health 1000
// > give_item sword_steel
```

### Hot Reload

```cpp
// Automatically reload configs when changed
configWatcher.watch("config/*.json", []() {
    reloadAllConfigs();
    std::cout << "Configs reloaded!" << std::endl;
});

// Edit JSON file
// Save
// See changes immediately in game!
```

### Time Manipulation

```cpp
// Speed up boring parts for testing
if (input->wasKeyPressed(Key::F2)) {
    Time::setTimeScale(5.0f);  // 5x speed
}

// Slow down for precise testing
if (input->wasKeyPressed(Key::F3)) {
    Time::setTimeScale(0.1f);  // Slow motion
}
```

## Conclusion

The architecture of Fresh Voxel Engine is specifically designed to support rapid iteration:

1. **Component-Based Architecture**: Easy to add/remove/modify behaviors
2. **Event-Driven Architecture**: Decouple systems for independent testing
3. **Data-Driven Design**: Change values without recompiling
4. **Automated Testing**: Catch regressions quickly
5. **Debug Tools**: Speed up manual testing

This enables the **build → test → iterate** cycle to be as fast as possible, letting you focus on making fun games rather than fighting with code.

## Further Reading

- [COMPONENT_BASED_ARCHITECTURE.md](../architecture/COMPONENT_BASED_ARCHITECTURE.md)
- [EVENT_DRIVEN_ARCHITECTURE.md](../architecture/EVENT_DRIVEN_ARCHITECTURE.md)
- [DATA_DRIVEN_DESIGN.md](../architecture/DATA_DRIVEN_DESIGN.md)
- [TESTING.md](../testing/TESTING.md)
