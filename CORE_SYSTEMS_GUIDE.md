# Fresh Voxel Engine - Core Systems Implementation

This document provides an overview of the core systems implemented in the Fresh Voxel Engine according to the requirements specification.

## Table of Contents
1. [Entity-Component System (ECS)](#1-entity-component-system-ecs)
2. [Voxel-Based Architecture](#2-voxel-based-architecture)
3. [Newtonian Physics System](#3-newtonian-physics-system)
4. [Procedural Generation](#4-procedural-generation)
5. [Scripting API](#5-scripting-api)
6. [Networking/Multiplayer](#6-networkingmultiplayer)
7. [Resource and Inventory Management](#7-resource-and-inventory-management)
8. [RPG Elements](#8-rpg-elements)
9. [Development Tools](#9-development-tools)

---

## 1. Entity-Component System (ECS)

### Overview
Flexible architecture for managing game objects and their properties with efficient component storage and retrieval.

### Key Classes

#### `Entity`
**Location**: `include/ecs/Entity.h`
- Represents a game object with a unique identifier (64-bit ID)
- Lightweight handle to access component data
- Methods: `getId()`, `isValid()`

#### `IComponent`
**Location**: `include/ecs/IComponent.h`
- Interface for all components
- Pure data containers (no logic)
- Provides type identification and cloning
- Use `COMPONENT_TYPE()` macro for implementation

#### `EntityManager`
**Location**: `include/ecs/EntityManager.h`
- Manages entities and their components
- Methods:
  - `createEntity()` - Create new entity
  - `destroyEntity()` - Remove entity
  - `addComponent<T>()` - Attach component
  - `getComponent<T>()` - Retrieve component
  - `hasComponent<T>()` - Check for component
  - `getEntitiesWithComponent<T>()` - Query entities

#### `SystemBase`
**Location**: `include/ecs/SystemBase.h`
- Base class for game systems
- System-based update loop for processing entities
- Methods:
  - `update(deltaTime)` - Process entities
  - `initialize()` - Setup system
  - `shutdown()` - Cleanup

### Usage Example
```cpp
// Create entity
EntityManager manager;
Entity entity = manager.createEntity();

// Add components
auto* physics = new PhysicsComponent();
manager.addComponent(entity, physics);

auto* structure = new VoxelStructureComponent();
manager.addComponent(entity, structure);

// Query entities
auto entities = manager.getEntitiesWithComponents<PhysicsComponent, VoxelStructureComponent>();

// System processes entities
class MySystem : public SystemBase {
    void update(float dt) override {
        auto entities = entityManager->getEntitiesWithComponent<PhysicsComponent>();
        for (auto entity : entities) {
            auto* physics = entityManager->getComponent<PhysicsComponent>(entity);
            // Process physics
        }
    }
};
```

---

## 2. Voxel-Based Architecture

### Overview
Arbitrary-sized blocks for flexible ship and station construction with automatic mass property calculation and collision detection.

### Key Classes

#### `VoxelBlock`
**Location**: `include/voxelship/VoxelBlock.h`
- Represents a single voxel with position, size, and material
- Features:
  - Arbitrary dimensions (not fixed-size grid)
  - Material properties (mass, hardness, conductivity)
  - Collision detection between blocks
  - Methods: `intersects()`, `contains()`, `getBounds()`

#### `VoxelStructureComponent`
**Location**: `include/voxelship/VoxelStructureComponent.h`
- Component containing voxel structure data for entities
- Features:
  - **Automatic center of mass calculation**
  - **Total mass calculation**
  - **Inertia tensor computation** for rotational physics
  - Collision detection between structures
- Methods:
  - `getTotalMass()` - Calculate total mass
  - `getCenterOfMass()` - Calculate center of mass
  - `getInertiaTensor()` - Get inertia tensor for rotation
  - `checkCollision()` - Detect collisions

### Usage Example
```cpp
// Create voxel structure
VoxelStructureComponent structure;

// Add blocks
VoxelMaterial steel;
steel.mass = 7850.0f; // kg/m³
steel.hardness = 10.0f;

VoxelBlock block1(glm::vec3(0, 0, 0), glm::vec3(2, 2, 2), steel);
structure.addBlock(block1);

// Get mass properties
float totalMass = structure.getTotalMass();
glm::vec3 centerOfMass = structure.getCenterOfMass();
glm::mat3 inertia = structure.getInertiaTensor();

// Check collision
bool collides = structure.checkCollision(otherStructure, thisPos, otherPos);
```

---

## 3. Newtonian Physics System

### Overview
Realistic physics simulation with forces, acceleration, velocity, linear and rotational motion, drag, and elastic collision response.

### Key Classes

#### `PhysicsComponent`
**Location**: `include/physics/PhysicsComponent.h`
- Component for physics properties
- Features:
  - **Linear motion**: position, velocity, acceleration
  - **Rotational motion**: rotation (quaternion), angular velocity, angular acceleration
  - **Forces and torques**: accumulated per frame
  - **Inertia tensor**: for realistic rotation
  - **Drag**: linear and angular
  - **Collision properties**: restitution (elasticity), friction
- Methods:
  - `addForce()` - Apply force
  - `addForceAtPoint()` - Apply force at point (generates torque)
  - `addTorque()` - Apply torque directly

#### `PhysicsSystem`
**Location**: `include/physics/PhysicsSystemEnhanced.h`
- System that handles physics simulation
- Features:
  - **Newtonian integration**: F = ma for linear, τ = Iα for rotational
  - **Gravity support**
  - **Drag forces**
  - **Elastic collision response** with proper impulse calculation
- Methods:
  - `update(deltaTime)` - Step physics simulation
  - `setGravity()` - Set global gravity
  - `applyElasticCollision()` - Handle collision response

### Usage Example
```cpp
// Setup physics
PhysicsSystem physicsSystem(&entityManager);
physicsSystem.setGravity(glm::vec3(0, -9.81f, 0));

// Create physics entity
Entity ship = entityManager.createEntity();
auto* physics = new PhysicsComponent();
physics->mass = 1000.0f;
physics->position = glm::vec3(0, 100, 0);
physics->restitution = 0.5f; // 50% elastic
entityManager.addComponent(ship, physics);

// Apply forces
physics->addForce(glm::vec3(0, 0, 1000.0f)); // Thrust
physics->addTorque(glm::vec3(0, 10.0f, 0)); // Rotation

// Update physics
physicsSystem.update(deltaTime);
```

---

## 4. Procedural Generation

### Overview
Deterministic galaxy sector generation using seed-based algorithms for consistent generation based on coordinates.

### Key Classes

#### `GalaxyGenerator`
**Location**: `include/galaxy/GalaxyGenerator.h`
- Generates galaxy sectors with asteroids, stations, and ships
- Features:
  - **Seed-based generation**: same seed + coordinates = same result
  - **Configurable density**: asteroids, stations, ships
  - Procedural naming
- Methods:
  - `generateSector(x, y)` - Generate sector
  - `setSeed()` - Set generation seed

#### `GalaxySector`
**Location**: `include/galaxy/GalaxySector.h`
- Represents a sector in the galaxy
- Contains:
  - **Asteroids** with resource types (Iron, Titanium, Naonite, etc.)
  - **Stations** (Trading Post, Repair Dock, Shipyard, etc.)
  - **Ships** with factions and hostility

#### Data Structures
- **AsteroidData**: position, size, resource type, amount
- **StationData**: position, type, name, level
- **ShipData**: position, velocity, type, faction, hostile flag

### Usage Example
```cpp
// Create generator
GalaxyGenerator generator(12345); // seed

// Generate sector
auto sector = generator.generateSector(10, 20);

// Access sector objects
for (const auto& asteroid : sector->getAsteroids()) {
    std::cout << "Asteroid at " << asteroid.position 
              << " has " << asteroid.resourceAmount << " of resource\n";
}

for (const auto& station : sector->getStations()) {
    std::cout << "Station: " << station.name << "\n";
}
```

---

## 5. Scripting API

### Status
**Note**: Lua integration requires external library (NLua or similar). 
The scripting framework is prepared but requires library integration.

### Planned Classes
- **ScriptingEngine**: Manages Lua scripting and mod loading
- **Mod loading system**: Load scripts from mod directories
- **Object registration**: Register C++ objects for Lua access

### Architecture
The engine uses the existing `EventSystem` (in `include/scripting/EventSystem.h`) for event-driven communication, which can be exposed to Lua scripts.

---

## 6. Networking/Multiplayer

### Overview
TCP-based client-server architecture with sector-based multiplayer, server-side sector management, and multi-threaded sector handling for scalability.

### Key Classes

#### `GameServer`
**Location**: `include/networking/GameServer.h`
- Main server for handling multiplayer connections
- Features:
  - TCP socket server
  - Multi-threaded client handling
  - Sector management
  - Broadcasting
- Methods:
  - `start()` - Start server
  - `stop()` - Stop server
  - `update(deltaTime)` - Update server state
  - `broadcastMessage()` - Send to all clients
  - `broadcastToSector()` - Send to sector clients

#### `ClientConnection`
**Location**: `include/networking/ClientConnection.h`
- Represents a connected client
- Features:
  - TCP socket communication
  - Message serialization/deserialization
  - Timeout tracking
- Methods:
  - `sendMessage()` - Send message to client
  - `receiveMessage()` - Receive message from client

#### `SectorServer`
**Location**: `include/networking/SectorServer.h`
- Manages a single sector on the server
- Features:
  - **Multi-threaded**: each sector can run on its own thread
  - Player management within sector
  - Entity management per sector
- Methods:
  - `addPlayer()`, `removePlayer()` - Manage players
  - `update(deltaTime)` - Update sector entities
  - `broadcastToPlayers()` - Send to sector players

#### `NetworkMessage`
**Location**: `include/networking/NetworkMessage.h`
- Message structure for network communication
- Features:
  - Type-safe serialization
  - Multiple data types support (byte, int, float, string)
  - Message types (Connect, Disconnect, EntityUpdate, etc.)

### Usage Example
```cpp
// Start server
GameServer server(7777); // port
server.setMaxClients(100);
server.start();

// Create sector servers
server.createSectorServer(0, 0);
server.createSectorServer(1, 0);

// Update server
server.update(deltaTime);

// Send message
NetworkMessage msg(MessageType::ChatMessage);
msg.writeString("Hello, players!");
server.broadcastMessage(msg);
```

---

## 7. Resource and Inventory Management

### Overview
Multiple resource types with inventory system, capacity limits, crafting system, and subsystem upgrades.

### Key Classes

#### `Inventory`
**Location**: `include/rpg/Inventory.h`
- Manages resource storage with capacity limits
- Resource types: Iron, Titanium, Naonite, Trinium, Xanion, Ogonite, Avorion, Energy, Credits
- Methods:
  - `addResource()` - Add resource (respects capacity)
  - `removeResource()` - Remove resource
  - `hasResource()` - Check if sufficient amount
  - `getTotalUsed()`, `getCapacity()`

#### `InventoryComponent`
**Location**: `include/rpg/InventoryComponent.h`
- Component for entity inventory

#### `CraftingSystem`
**Location**: `include/rpg/CraftingSystem.h`
- Handles crafting of upgrades
- Features:
  - Recipe system
  - Resource requirements
  - Crafting time
- Default recipes: Shields, Weapons, Cargo expansions

#### `SubsystemUpgrade`
**Location**: `include/rpg/CraftingSystem.h`
- Represents a ship upgrade
- Types: Shield, Weapon, Engine, Cargo, Generator, Computer
- Features:
  - Level system
  - Stat bonuses
  - Resource costs

### Usage Example
```cpp
// Create inventory
Inventory inventory(1000.0f); // 1000 capacity

// Add resources
inventory.addResource(ResourceType::Iron, 500.0f);
inventory.addResource(ResourceType::Titanium, 300.0f);

// Crafting
CraftingSystem crafting;
SubsystemUpgrade upgrade;

if (crafting.canCraft("Basic Shield", inventory)) {
    crafting.craft("Basic Shield", inventory, upgrade);
    // upgrade now contains the crafted shield
    float shieldCapacity = upgrade.getStatBonus("shieldCapacity");
}
```

---

## 8. RPG Elements

### Overview
Ship progression with experience and levels, faction relations and reputation system, loot drop system, and trading system with buy/sell mechanics.

### Key Classes

#### `ProgressionComponent`
**Location**: `include/rpg/ProgressionComponent.h`
- Manages entity progression (XP and levels)
- Features:
  - Experience tracking
  - Automatic level-up
  - Exponential XP curve
  - Stat bonuses per level
- Methods:
  - `addExperience()` - Gain XP
  - `getLevel()` - Current level
  - `setStat()`, `getStat()` - Stat management

#### `FactionComponent`
**Location**: `include/rpg/FactionComponent.h`
- Handles faction relations
- Features:
  - Reputation system (-100 to +100)
  - Relation tiers: Hostile, Unfriendly, Neutral, Friendly, Allied
- Methods:
  - `setReputation()`, `getReputation()`
  - `modifyReputation()` - Change reputation
  - `getRelation()` - Get relation tier

#### `LootSystem`
**Location**: `include/rpg/LootSystem.h`
- Generates loot drops
- Features:
  - Loot tables with drop chances
  - Random generation
  - Default tables for asteroids and enemies
- Methods:
  - `registerLootTable()` - Add loot table
  - `generateLoot()` - Generate drops
  - `generateAsteroidLoot()`, `generateEnemyLoot()`

#### `TradingSystem`
**Location**: `include/rpg/TradingSystem.h`
- Manages resource trading
- Features:
  - Buy/sell mechanics
  - Price per unit
  - Available quantity
  - Sell price = 70% of buy price
- Methods:
  - `buyResource()`, `sellResource()`
  - `calculateBuyPrice()`, `calculateSellPrice()`

### Usage Example
```cpp
// Progression
ProgressionComponent progression;
progression.addExperience(100.0f);
int level = progression.getLevel();

// Factions
FactionComponent faction("Player");
faction.setReputation("Pirates", -50.0f); // Hostile
faction.modifyReputation("Federation", 10.0f);
bool hostile = faction.isHostile("Pirates"); // true

// Loot
LootSystem loot;
auto drops = loot.generateAsteroidLoot(seed);
for (const auto& drop : drops) {
    inventory.addResource(drop.resource, drop.amount);
}

// Trading
TradingSystem trading;
float credits = 10000.0f;
trading.buyResource(ResourceType::Iron, 100.0f, inventory, credits);
trading.sellResource(ResourceType::Titanium, 50.0f, inventory, credits);
```

---

## 9. Development Tools

### Overview
Debug visualization, performance profiling, memory tracking, and interactive debug console.

### Key Classes

#### `DevToolsManager`
**Location**: `include/devtools/DevToolsManager.h`
- Manages all development tools
- Central hub for debug features
- Methods:
  - `initialize()`, `shutdown()`
  - `update(deltaTime)`, `render()`
  - `getDebugRenderer()`, `getProfiler()`, etc.

#### `DebugRenderer`
**Location**: `include/devtools/DebugRenderer.h`
- Debug visualization for game objects and physics
- Features:
  - Draw lines, boxes, spheres, arrows, text
  - Persistent or one-frame display
  - Color support
- Methods:
  - `drawLine()`, `drawBox()`, `drawSphere()`
  - `drawArrow()`, `drawText()`

#### `PerformanceProfiler`
**Location**: `include/devtools/PerformanceProfiler.h`
- FPS and frame timing tracking
- Features:
  - FPS calculation
  - Average/min/max frame times
  - Named section profiling
- Methods:
  - `beginFrame()`, `endFrame()`
  - `getFPS()`, `getAverageFrameTime()`
  - `beginSection()`, `endSection()`

#### `MemoryTracker`
**Location**: `include/devtools/MemoryTracker.h`
- Memory usage monitoring (including GPU when available)
- Features:
  - Category-based tracking
  - Peak usage tracking
  - GPU memory queries (platform-specific)
- Methods:
  - `recordAllocation()`, `recordDeallocation()`
  - `getTotalAllocated()`, `getPeakMemoryUsage()`

#### `DebugConsole`
**Location**: `include/devtools/DebugConsole.h`
- Runtime command console (press ` key)
- Features:
  - Command registration
  - Command history
  - Output logging
  - Built-in commands: help, clear, quit
- Methods:
  - `registerCommand()` - Add command
  - `executeCommand()` - Run command
  - `print()` - Output text

### Usage Example
```cpp
// Initialize dev tools
DevToolsManager devTools;
devTools.initialize();

// Debug rendering
auto& debugRender = devTools.getDebugRenderer();
debugRender.drawLine(start, end, glm::vec4(1, 0, 0, 1), 1.0f); // Red line for 1 second
debugRender.drawBox(center, size, glm::vec4(0, 1, 0, 1)); // Green box

// Performance profiling
auto& profiler = devTools.getProfiler();
profiler.beginSection("Physics");
// ... physics code ...
profiler.endSection("Physics");

std::cout << profiler.getReport();

// Memory tracking
auto& memory = devTools.getMemoryTracker();
memory.recordAllocation("Entities", 1024);
std::cout << "Total memory: " << memory.getTotalAllocated() << " bytes\n";

// Debug console
auto& console = devTools.getConsole();
console.registerCommand("spawn", [](const std::vector<std::string>& args) {
    // Spawn entity at location
}, "Spawn entity at location");

console.executeCommand("spawn 0 0 0");
```

---

## Building the Project

### Prerequisites
- CMake 3.20+
- C++17 compiler
- GLM (for math)
- Vulkan SDK (optional, for rendering)

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### CMake Configuration
All new systems are automatically included in the build via `CMakeLists.txt`:
- ECS files in `src/ecs/`
- Voxel ship files in `src/voxelship/`
- Galaxy generation in `src/galaxy/`
- RPG systems in `src/rpg/`
- Networking in `src/networking/`
- Dev tools in `src/devtools/`

---

## System Integration

### Example: Complete Game Entity
```cpp
// Create a ship entity with all systems
Entity ship = entityManager.createEntity();

// Add voxel structure
auto* structure = new VoxelStructureComponent();
// ... add voxel blocks ...
manager.addComponent(ship, structure);

// Add physics (automatically uses mass from structure)
auto* physics = new PhysicsComponent();
physics->mass = structure->getTotalMass();
physics->inertiaTensor = structure->getInertiaTensor();
manager.addComponent(ship, physics);

// Add inventory
auto* inventory = new InventoryComponent(5000.0f);
manager.addComponent(ship, inventory);

// Add progression
auto* progression = new ProgressionComponent();
manager.addComponent(ship, progression);

// Add faction
auto* faction = new FactionComponent("Player");
manager.addComponent(ship, faction);

// Systems process the entity
physicsSystem.update(deltaTime);
craftingSystem.processCrafting(ship);
```

---

## Architecture Overview

The implemented systems follow a modular, data-driven architecture:

1. **ECS Core**: Entities are IDs, components are data, systems are logic
2. **Physics Integration**: PhysicsComponent works with VoxelStructureComponent for accurate mass properties
3. **Procedural Content**: Deterministic generation ensures consistency across clients
4. **Networking**: Sector-based architecture allows distributed server processing
5. **RPG Layer**: Progression, factions, loot, and trading build on top of ECS
6. **Development Tools**: Non-intrusive debugging and profiling

All systems are designed to work together while remaining loosely coupled and independently testable.

---

## Next Steps

### Integration Tasks
1. Complete Lua scripting integration (requires NLua library)
2. Integrate rendering with debug renderer
3. Connect network messages to entity synchronization
4. Implement OpenGL/Vulkan debugger for graphics error detection
5. Add script compiler for hot-reloading

### Testing
Each system can be tested independently:
- Unit tests for ECS operations
- Physics simulation tests
- Generation consistency tests (same seed = same result)
- Network message serialization tests
- RPG system integration tests

---

## License

[To be determined]

## Contact

For questions or contributions, please refer to the repository's issue tracker.
