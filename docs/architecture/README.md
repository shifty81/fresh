# Architecture Documentation

This directory contains comprehensive documentation about the architectural patterns and design principles used in Fresh Voxel Engine.

## Core Architecture Patterns

### 1. Component-Based Architecture (CBA)

**File**: [COMPONENT_BASED_ARCHITECTURE.md](COMPONENT_BASED_ARCHITECTURE.md)

Learn about the Entity-Component-System (ECS) pattern that enables:
- Modular, reusable components
- Easy composition of game entities
- Independent testing of behaviors
- Rapid iteration on gameplay features

**Key topics:**
- What are Entities, Components, and Systems?
- Built-in components (Health, Movement, etc.)
- Creating custom components
- Querying entities by component type
- Benefits for development and testing

### 2. Event-Driven Architecture (EDA)

**File**: [EVENT_DRIVEN_ARCHITECTURE.md](EVENT_DRIVEN_ARCHITECTURE.md)

Understand how systems communicate through events:
- Decoupled system interactions
- Emergent gameplay from simple rules
- Easy to add new behaviors
- Testable in isolation

**Key topics:**
- Subscribing to and emitting events
- Built-in event types
- Creating event-driven features
- Event chains and emergent gameplay
- Integration with components

### 3. Data-Driven Design

**File**: [DATA_DRIVEN_DESIGN.md](DATA_DRIVEN_DESIGN.md)

Discover how external configuration files enable rapid iteration:
- Balance game mechanics without recompiling
- Designer-friendly workflow
- A/B testing different configurations
- Modding support

**Key topics:**
- Character stats configuration
- Weapon properties configuration
- Loading and using configs in code
- Hot-reloading for instant updates
- Integration with components and events

## Development Workflows

### Iterative Development & Rapid Prototyping

**File**: [../guides/ITERATIVE_DEVELOPMENT.md](../guides/ITERATIVE_DEVELOPMENT.md)

Master the cycle of building, testing, and iterating:
- Prototype → Feedback → Iteration → Polish
- Rapid prototyping techniques
- Component composition for quick experiments
- Data-driven A/B testing

### Testing Best Practices

**File**: [../guides/TESTING_BEST_PRACTICES.md](../guides/TESTING_BEST_PRACTICES.md)

Write effective tests for components and systems:
- Unit testing components
- Integration testing systems
- Event-driven testing
- Parameterized and combinatorial tests
- Test-Driven Development (TDD)

## How These Patterns Work Together

The three core patterns complement each other:

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  Component-Based Architecture (What entities ARE)       │
│  ├─ HealthComponent                                     │
│  ├─ MovementComponent                                   │
│  └─ TransformComponent                                  │
│                                                         │
└─────────────────────────────────────────────────────────┘
                            │
                            │ Systems process
                            ▼
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  Event-Driven Architecture (How systems COMMUNICATE)    │
│  ├─ emit("entity_damaged")                              │
│  ├─ emit("door_opened")                                 │
│  └─ subscribe and react                                 │
│                                                         │
└─────────────────────────────────────────────────────────┘
                            │
                            │ Events trigger
                            ▼
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  Data-Driven Design (How values are CONFIGURED)         │
│  ├─ character_stats.json                                │
│  ├─ weapon_stats.json                                   │
│  └─ Hot-reload for rapid iteration                      │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

## Quick Start Example

Here's a complete example showing all three patterns working together:

```cpp
// 1. Component-Based: Create entity with components
Entity player = entityManager->createEntity();
entityManager->addComponent(player, new HealthComponent(100.0f));
entityManager->addComponent(player, new MovementComponent(5.0f, 10.0f));

// 2. Event-Driven: Listen for damage events
EventSystem::getInstance().subscribe("entity_damaged", [](const EventData& data) {
    // Play damage sound
    audioEngine->playSound("hit.wav");
    
    // Show damage number
    float damage = data.get<float>("damage");
    showFloatingText("-" + std::to_string(damage));
});

// 3. Data-Driven: Load enemy from config
// In config/character_stats.json:
// {
//   "enemy_zombie": {
//     "health": 50.0,
//     "speed": 2.0,
//     "damage": 10.0
//   }
// }

Entity zombie = createCharacterFromConfig("enemy_zombie", entityManager);

// When zombie damages player...
combatSystem->applyDamage(player, 10.0f);
// → HealthComponent updates
// → "entity_damaged" event emitted
// → Audio and UI systems react automatically
```

## Benefits for Complex Games

These architectural patterns specifically address the problem statement's goals:

### 1. Modularity & Decoupling
- **Components** separate different aspects of entities
- **Events** decouple system communication
- **Data files** separate configuration from code

### 2. Ease of Development
- Add new features by creating components
- Modify behavior by subscribing to events
- Balance gameplay by editing JSON files

### 3. Ease of Testing
- Test components in isolation (unit tests)
- Test system interactions (integration tests)
- Simulate events for testing (no full game state needed)

### 4. Rapid Iteration
- Component composition for quick prototypes
- Event subscriptions for experimental features
- Hot-reload configs for instant feedback

### 5. Emergent Gameplay
- Simple event rules → complex interactions
- Example: noise → alert enemies + scare wildlife + wake NPCs

## Practical Examples

### Example 1: Add a Dash Ability (30 minutes)

```cpp
// Step 1: Create component (5 min)
class DashComponent : public IComponent {
    COMPONENT_TYPE(DashComponent)
    float dashSpeed, cooldown;
};

// Step 2: Add to player (1 min)
entityManager->addComponent(player, new DashComponent());

// Step 3: Handle input and emit event (10 min)
if (input->wasKeyPressed(Key::Space) && dash->canDash()) {
    EventSystem::getInstance().emit("dash_started", data);
}

// Step 4: Other systems react automatically (5 min)
EventSystem::getInstance().subscribe("dash_started", [](const EventData& data) {
    particleSystem->emitDashTrail();
    audioEngine->playSound("whoosh.wav");
});

// Step 5: Balance values in JSON (10 min of playtesting)
{
  "dash": {
    "speed": 18.0,
    "duration": 0.3,
    "cooldown": 2.0
  }
}
```

### Example 2: Explosive Barrels with Chain Reactions (1 hour)

```cpp
// Component (15 min)
class ExplosiveComponent : public IComponent {
    COMPONENT_TYPE(ExplosiveComponent)
    float damage, radius;
};

// Death triggers explosion (15 min)
EventSystem::getInstance().subscribe("entity_died", [](const EventData& data) {
    Entity entity(data.get<Entity::ID>("entity"));
    auto* explosive = entityManager->getComponent<ExplosiveComponent>(entity);
    if (explosive) {
        EventSystem::getInstance().emit("explosion", explosionData);
    }
});

// Explosion damages nearby entities (30 min)
EventSystem::getInstance().subscribe("explosion", [](const EventData& data) {
    // Find entities in radius, apply damage
    // Damaged barrels also die → more explosions!
    // Chain reaction emerges automatically from simple rules
});
```

## Other Architecture Documents

- [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md) - World generator plugin architecture
- [SCENE_MANAGEMENT.md](SCENE_MANAGEMENT.md) - Editor-first scene workflow
- [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) - .NET 9 C# bindings
- [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md) - Native Windows features
- [UI_ARCHITECTURE.md](UI_ARCHITECTURE.md) - Native UI system design

## Contributing

When adding new features:

1. **Consider components first**: Can this be a component?
2. **Use events for communication**: Systems shouldn't directly call each other
3. **Make values configurable**: If it might change, put it in JSON
4. **Write tests**: Unit tests for components, integration tests for systems
5. **Document patterns**: Update this documentation

## Further Reading

- [Getting Started Guide](../getting-started/GETTING_STARTED.md)
- [Development Tools](../community/DEVELOPMENT_TOOLS.md)
- [Coding Standards](../community/CPP_GUIDELINES.md)
- [Project Status](../status/PROJECT_STATUS.md)

## Questions?

Check the [main documentation](../../README.md) or ask in [Discussions](https://github.com/shifty81/fresh/discussions).
