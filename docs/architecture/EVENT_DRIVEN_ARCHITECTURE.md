# Event-Driven Architecture (EDA) Guide

## Overview

Fresh Voxel Engine uses an **Event-Driven Architecture** where systems communicate by sending and receiving events rather than direct function calls. This pattern promotes:

- **Decoupling**: Systems don't need direct references to each other
- **Flexibility**: Easy to add new interactions without modifying existing code
- **Emergent Gameplay**: Complex behaviors arise from simple event interactions
- **Asynchronous Processing**: Events can be queued for later processing

## Core Concepts

### Events

An **Event** is a notification that something happened in the game. Events have:
- A **type** (string identifier like "player_died" or "door_opened")
- Optional **data** (key-value pairs with event details)

### Event System

The **EventSystem** is a singleton that manages event subscriptions and emissions:

```cpp
#include "scripting/EventSystem.h"

auto& eventSystem = EventSystem::getInstance();
```

### Event Flow

```
System A                 EventSystem                System B
   |                          |                         |
   |--- emit("door_opened")-->|                         |
   |                          |----> notify ----------->|
   |                          |                         |
   |                          |<---- subscribe ---------|
```

## Basic Usage

### 1. Subscribing to Events

Systems "listen" for events they're interested in:

```cpp
#include "scripting/EventSystem.h"

// Subscribe to door opened events
int subID = EventSystem::getInstance().subscribe("door_opened", 
    [](const EventData& data) {
        // React to door opening
        int doorID = data.get<int>("doorID");
        std::cout << "Door " << doorID << " was opened!" << std::endl;
    });

// Later, unsubscribe if needed
EventSystem::getInstance().unsubscribe(subID);
```

### 2. Emitting Events

Systems emit events when something happens:

```cpp
// Create event data
EventData data;
data.set("doorID", 123);
data.set("position", glm::vec3(10.0f, 0.0f, 5.0f));

// Emit immediately
EventSystem::getInstance().emit("door_opened", data);
```

### 3. Queuing Events

For events that should be processed next frame:

```cpp
// Queue event (will be processed during processEvents())
EventSystem::getInstance().queueEvent("enemy_spotted", data);

// In main game loop
EventSystem::getInstance().processEvents();  // Process all queued events
```

## Built-in Event Types

### Player Events

```cpp
Events::PLAYER_SPAWNED  // When player enters world
Events::PLAYER_DIED     // When player health reaches zero
```

### World Events

```cpp
Events::CHUNK_LOADED    // When terrain chunk loads
Events::CHUNK_UNLOADED  // When terrain chunk unloads
Events::BLOCK_PLACED    // When voxel block placed
Events::BLOCK_BROKEN    // When voxel block destroyed
```

### Combat Events

```cpp
Events::ENTITY_DAMAGED  // When entity takes damage
Events::ENTITY_HEALED   // When entity is healed
Events::ENTITY_DIED     // When entity health reaches zero
Events::ENTITY_REVIVED  // When dead entity is revived
```

### Environmental Events

```cpp
Events::DOOR_OPENED     // When door opens
Events::DOOR_CLOSED     // When door closes
Events::ENEMY_SPOTTED   // When enemy sees player
Events::NOISE_MADE      // When sound is made (for AI)
Events::TRAP_TRIGGERED  // When trap activates
```

### Item Events

```cpp
Events::ITEM_PICKED_UP  // When item collected
Events::ITEM_USED       // When item consumed/equipped
```

### Game State Events

```cpp
Events::GAME_PAUSED     // When game pauses
Events::GAME_RESUMED    // When game resumes
Events::SCENE_LOADED    // When scene finishes loading
Events::RESOURCE_LOADED // When asset loads
```

## Practical Examples

### Example 1: Combat System

The CombatSystem emits events when entities take damage:

```cpp
void CombatSystem::applyDamage(Entity entity, float amount, Entity source)
{
    auto* health = entityManager->getComponent<HealthComponent>(entity);
    if (!health) return;

    health->takeDamage(amount);

    // Emit damage event for other systems to react
    EventData eventData;
    eventData.set("entity", entity.getId());
    eventData.set("damage", amount);
    eventData.set("health", health->getCurrentHealth());
    if (source.isValid()) {
        eventData.set("source", source.getId());
    }

    EventSystem::getInstance().emit("entity_damaged", eventData);
}
```

### Example 2: Audio System Reacts to Combat

The AudioSystem listens for damage events and plays sounds:

```cpp
class AudioSystem : public SystemBase
{
public:
    void initialize() override
    {
        // Subscribe to damage events
        damageSubID = EventSystem::getInstance().subscribe("entity_damaged",
            [this](const EventData& data) {
                float damage = data.get<float>("damage");
                
                // Play different sounds based on damage amount
                if (damage >= 50.0f) {
                    audioEngine->playSound("impact_heavy.wav");
                } else if (damage >= 20.0f) {
                    audioEngine->playSound("impact_medium.wav");
                } else {
                    audioEngine->playSound("impact_light.wav");
                }
            });
    }

    void shutdown() override
    {
        EventSystem::getInstance().unsubscribe(damageSubID);
    }

private:
    int damageSubID;
    AudioEngine* audioEngine;
};
```

### Example 3: AI Reacts to Noise

Wildlife AI system reacts when player makes noise:

```cpp
class WildlifeAI : public SystemBase
{
public:
    void initialize() override
    {
        // Listen for noise events
        EventSystem::getInstance().subscribe("noise_made",
            [this](const EventData& data) {
                glm::vec3 noisePos = data.get<glm::vec3>("position");
                float volume = data.get<float>("volume");
                
                // Animals within range flee
                auto animals = entityManager->getEntitiesWithComponent<AnimalAIComponent>();
                for (const auto& animal : animals) {
                    auto* ai = entityManager->getComponent<AnimalAIComponent>(animal);
                    auto* transform = entityManager->getComponent<TransformComponent>(animal);
                    
                    float distance = glm::distance(transform->getPosition(), noisePos);
                    float hearingRange = volume * 10.0f;
                    
                    if (distance < hearingRange) {
                        ai->setState(AIState::Fleeing);
                        ai->setFleeTarget(noisePos);
                    }
                }
            });
    }
};
```

### Example 4: Chain Reactions

Events can trigger other events, creating emergent gameplay:

```cpp
// Barrel explodes when it dies
EventSystem::getInstance().subscribe("entity_died", [](const EventData& data) {
    Entity entity(data.get<Entity::ID>("entity"));
    
    // Check if entity has explosive component
    auto* explosive = entityManager->getComponent<ExplosiveComponent>(entity);
    if (explosive) {
        // Emit explosion event
        EventData explosionData;
        explosionData.set("position", transform->getPosition());
        explosionData.set("radius", explosive->getRadius());
        explosionData.set("damage", explosive->getDamage());
        
        EventSystem::getInstance().emit("explosion", explosionData);
    }
});

// Explosion damages nearby entities
EventSystem::getInstance().subscribe("explosion", [](const EventData& data) {
    glm::vec3 position = data.get<glm::vec3>("position");
    float radius = data.get<float>("radius");
    float damage = data.get<float>("damage");
    
    auto entities = entityManager->getEntitiesWithComponents<HealthComponent, TransformComponent>();
    for (const auto& entity : entities) {
        auto* transform = entityManager->getComponent<TransformComponent>(entity);
        float distance = glm::distance(transform->getPosition(), position);
        
        if (distance < radius) {
            // Damage scales with distance
            float damageMultiplier = 1.0f - (distance / radius);
            float actualDamage = damage * damageMultiplier;
            
            // Apply damage (which will emit "entity_damaged" event)
            combatSystem->applyDamage(entity, actualDamage);
        }
    }
});

// Result: Shooting one explosive barrel can cause a chain reaction!
```

## Event Data Patterns

### Simple Event (No Data)

```cpp
EventSystem::getInstance().emit("game_paused");
```

### Event with Single Value

```cpp
EventData data;
data.set("seed", 12345);
EventSystem::getInstance().emit("world_generated", data);
```

### Event with Multiple Values

```cpp
EventData data;
data.set("position", glm::vec3(10.0f, 5.0f, 3.0f));
data.set("velocity", glm::vec3(1.0f, 0.0f, 0.0f));
data.set("entityID", entity.getId());
data.set("timestamp", currentTime);
EventSystem::getInstance().emit("entity_moved", data);
```

### Event with Complex Types

```cpp
struct DamageInfo {
    float physicalDamage;
    float fireDamage;
    float iceDamage;
};

EventData data;
data.set("target", targetEntity.getId());
data.set("source", sourceEntity.getId());
data.set("damageInfo", DamageInfo{10.0f, 5.0f, 0.0f});
EventSystem::getInstance().emit("complex_damage", data);
```

## Benefits for Development & Testing

### 1. Decoupling Systems

Systems don't need to know about each other:

```cpp
// BAD: Direct coupling
class WildlifeSystem {
    EnemyAISystem* enemyAI;  // Direct dependency
    
    void onNoiseMade() {
        enemyAI->alertNearbyEnemies();  // Tight coupling
    }
};

// GOOD: Event-driven decoupling
class WildlifeSystem {
    void onNoiseMade() {
        EventData data;
        data.set("position", position);
        EventSystem::getInstance().emit("noise_made", data);
        // WildlifeSystem doesn't know or care who listens
    }
};

// EnemyAISystem subscribes independently
class EnemyAISystem {
    void initialize() {
        EventSystem::getInstance().subscribe("noise_made", [](const EventData& data) {
            // React to noise
        });
    }
};
```

### 2. Easy to Add New Features

Want NPCs to react to doors opening? Just subscribe:

```cpp
EventSystem::getInstance().subscribe("door_opened", [](const EventData& data) {
    // NPCs investigate nearby door openings
    glm::vec3 doorPos = data.get<glm::vec3>("position");
    alertNearbyNPCs(doorPos);
});
```

No need to modify door code or NPC code!

### 3. Emergent Gameplay

Complex interactions from simple rules:

```cpp
// Rule 1: Breaking blocks makes noise
EventSystem::getInstance().emit("noise_made", noiseData);

// Rule 2: Noise alerts enemies
// Rule 3: Noise scares wildlife
// Rule 4: Loud noise wakes sleeping NPCs

// Emergent behavior: Player breaks block → 
//   enemies investigate + animals flee + NPCs wake up
```

### 4. Testable in Isolation

Test systems by simulating events:

```cpp
TEST(AudioSystemTest, PlaysDamageSound)
{
    AudioSystem audioSystem;
    audioSystem.initialize();
    
    // Simulate damage event
    EventData data;
    data.set("damage", 50.0f);
    EventSystem::getInstance().emit("entity_damaged", data);
    
    // Verify sound was played
    EXPECT_TRUE(audioSystem.wasSoundPlayed("impact_heavy.wav"));
}
```

No need to set up combat, entities, or health components!

## Immediate vs. Queued Events

### Immediate Emission

Use `emit()` for events that must be processed immediately:

```cpp
// Critical state change that other systems need to react to NOW
EventSystem::getInstance().emit("entity_died", data);
```

### Queued Events

Use `queueEvent()` for events to process next frame:

```cpp
// Collect events during frame
EventSystem::getInstance().queueEvent("block_placed", data);
EventSystem::getInstance().queueEvent("item_picked_up", data);

// Process all at once (in game loop)
EventSystem::getInstance().processEvents();
```

Benefits of queuing:
- **Consistent timing**: All events processed at same point in frame
- **Safe iteration**: No modifying collections during iteration
- **Performance**: Batch processing more efficient

## Event Priority & Ordering

Events are processed in the order they were queued:

```cpp
EventSystem::getInstance().queueEvent("event1", data);
EventSystem::getInstance().queueEvent("event2", data);
EventSystem::getInstance().queueEvent("event3", data);

EventSystem::getInstance().processEvents();
// Fires: event1, then event2, then event3
```

For priority events, use immediate emission.

## Best Practices

### ✅ DO

- **Use descriptive event names**: "door_opened" not "event1"
- **Include relevant data**: Everything listeners might need
- **Document event contracts**: What data does event contain?
- **Clean up subscriptions**: Unsubscribe in shutdown()
- **Use queued events for batching**: Better performance
- **Test event handlers in isolation**: Mock event data

### ❌ DON'T

- **Don't emit events in tight loops**: Can cause performance issues
- **Don't assume listener order**: Multiple listeners, any order
- **Don't modify shared state in listeners**: Can cause race conditions
- **Don't use events for return values**: Events are fire-and-forget
- **Don't subscribe in hot paths**: Subscribe once in initialization

## Performance Considerations

Events are lightweight but not free:

```cpp
// BAD: Event per voxel (millions per second)
for (auto& voxel : voxels) {
    EventSystem::getInstance().emit("voxel_updated", data);  // Too many!
}

// GOOD: Batch event
EventData data;
data.set("voxelCount", voxels.size());
data.set("region", regionBounds);
EventSystem::getInstance().emit("region_updated", data);  // One event
```

## Integration with Components

Events and components work together perfectly:

```cpp
// Component stores data
class DoorComponent : public IComponent {
public:
    bool isOpen() const { return open; }
    void open() { 
        open = true;
        
        // Emit event when state changes
        EventData data;
        data.set("doorID", entity.getId());
        data.set("position", transform->getPosition());
        EventSystem::getInstance().emit("door_opened", data);
    }
    
private:
    bool open = false;
};
```

See [COMPONENT_BASED_ARCHITECTURE.md](COMPONENT_BASED_ARCHITECTURE.md) for component details.

## Example: Complete Event-Driven Feature

Here's a complete example showing how to add a trap system using events:

```cpp
// 1. Define trap component
class TrapComponent : public IComponent {
public:
    COMPONENT_TYPE(TrapComponent)
    
    float getDamage() const { return damage; }
    float getTriggerRadius() const { return triggerRadius; }
    bool isTriggered() const { return triggered; }
    void trigger() { triggered = true; }
    
private:
    float damage = 50.0f;
    float triggerRadius = 2.0f;
    bool triggered = false;
};

// 2. Create trap system
class TrapSystem : public SystemBase {
public:
    void update(float deltaTime) override {
        auto traps = entityManager->getEntitiesWithComponents<TrapComponent, TransformComponent>();
        auto entities = entityManager->getEntitiesWithComponent<TransformComponent>();
        
        for (const auto& trap : traps) {
            auto* trapComp = entityManager->getComponent<TrapComponent>(trap);
            auto* trapTransform = entityManager->getComponent<TransformComponent>(trap);
            
            if (trapComp->isTriggered()) continue;
            
            // Check for entities in range
            for (const auto& entity : entities) {
                if (entity == trap) continue;
                
                auto* transform = entityManager->getComponent<TransformComponent>(entity);
                float distance = glm::distance(trapTransform->getPosition(), transform->getPosition());
                
                if (distance < trapComp->getTriggerRadius()) {
                    // Trigger trap!
                    trapComp->trigger();
                    
                    EventData data;
                    data.set("trap", trap.getId());
                    data.set("victim", entity.getId());
                    data.set("damage", trapComp->getDamage());
                    data.set("position", trapTransform->getPosition());
                    
                    EventSystem::getInstance().emit("trap_triggered", data);
                }
            }
        }
    }
};

// 3. Other systems react
// Audio system plays trap sound
EventSystem::getInstance().subscribe("trap_triggered", [](const EventData& data) {
    audioEngine->playSound("trap_spring.wav");
});

// Particle system shows trap effect
EventSystem::getInstance().subscribe("trap_triggered", [](const EventData& data) {
    glm::vec3 pos = data.get<glm::vec3>("position");
    particleSystem->emitBurst(pos, "trap_particles");
});

// Combat system applies damage
EventSystem::getInstance().subscribe("trap_triggered", [](const EventData& data) {
    Entity victim(data.get<Entity::ID>("victim"));
    float damage = data.get<float>("damage");
    combatSystem->applyDamage(victim, damage);
});
```

Notice how trap system doesn't know about audio, particles, or combat - it just emits an event!

## Testing Event-Driven Code

See test examples:
- `tests/scripting/EventSystemTests.cpp` - Event system unit tests
- `tests/ecs/CombatSystemTests.cpp` - Event integration tests

## Further Reading

- [COMPONENT_BASED_ARCHITECTURE.md](COMPONENT_BASED_ARCHITECTURE.md) - How components work with events
- [DATA_DRIVEN_DESIGN.md](DATA_DRIVEN_DESIGN.md) - Loading event configurations
- [EMERGENT_GAMEPLAY.md](EMERGENT_GAMEPLAY.md) - Creating complex behaviors from simple events
