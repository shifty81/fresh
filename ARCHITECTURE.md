# Fresh Voxel Engine Architecture

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Core Systems](#core-systems)
- [Data Flow](#data-flow)
- [Module Dependencies](#module-dependencies)
- [Design Patterns](#design-patterns)
- [Performance Considerations](#performance-considerations)

## Overview

Fresh Voxel Engine is a modern, high-performance voxel game engine built with C++17 supporting multiple graphics APIs (OpenGL, DirectX 11/12). The architecture is designed to be modular, extensible, and optimized for real-time rendering of massive voxel worlds.

### Key Design Goals

1. **Performance**: 60+ FPS on modern hardware
2. **Scalability**: Support for infinite worlds with chunk streaming
3. **Modularity**: Clean separation of concerns, easy to extend
4. **Flexibility**: Support for custom content through asset packs
5. **Maintainability**: Clear code structure, well-documented

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Application Layer                        │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Main Menu   │  │Game Loop │  │Dev Tools │  │UI System  │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                      Core Engine                            │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Resource    │  │Scene     │  │Event     │  │Memory     │ │
│  │Manager     │  │Manager   │  │System    │  │Manager    │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    Gameplay Systems                         │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Player      │  │Inventory │  │Crafting  │  │Trading    │ │
│  │Controller  │  │System    │  │System    │  │System     │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    World Systems                            │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Voxel World │  │Chunk     │  │Terrain   │  │Modular    │ │
│  │            │  │Streamer  │  │Generator │  │Assets     │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  Simulation Systems                         │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Physics     │  │Collision │  │AI System │  │Networking │ │
│  │System      │  │Detection │  │          │  │           │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                    Platform Layer                           │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│  │Graphics    │  │Input     │  │Audio     │  │Scripting  │ │
│  │APIs        │  │Manager   │  │Engine    │  │Engine     │ │
│  └────────────┘  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
```

## Core Systems

### 1. Rendering System

**Components:**
- `IRenderContext`: Abstract rendering interface
- `OpenGLRenderContext`: OpenGL implementation
- `DirectX11RenderContext`: DirectX 11 implementation (Windows)
- `DirectX12RenderContext`: DirectX 12 implementation (Windows)
- `ShaderManager`: Shader compilation and management

**Responsibilities:**
- Initialize graphics context for selected API
- Manage swapchain and framebuffers
- Execute render passes
- Handle shader compilation
- Manage GPU resources

**Key Features:**
- Multi-API support (OpenGL, DirectX 11/12)
- Automatic API selection based on platform
- Efficient command buffer/context management
- Shader hot-reloading support
- Multi-threaded rendering ready

### 2. Voxel World System

**Components:**
- `VoxelWorld`: World container and chunk management
- `Chunk`: 16x256x16 voxel data structure
- `MeshGenerator`: Converts voxel data to renderable meshes
- `ChunkStreamer`: Dynamic chunk loading/unloading

**Responsibilities:**
- Store and manage voxel data
- Generate optimized meshes
- Stream chunks based on player position
- Handle block placement/removal

**Key Features:**
- Greedy meshing algorithm
- Face culling optimization
- Infinite world support
- 13 voxel types (expandable)

**Chunk Structure:**
```cpp
class Chunk {
    VoxelType voxels[16][256][16];  // 65,536 voxels per chunk
    Mesh mesh;                       // Optimized render mesh
    bool dirty;                      // Needs remeshing?
    glm::ivec3 position;            // World position
};
```

### 3. Procedural Generation

**Components:**
- `NoiseGenerator`: Perlin noise implementation
- `TerrainGenerator`: Biome-based terrain generation

**Responsibilities:**
- Generate realistic terrain
- Create cave systems
- Place biome-specific features
- Ensure deterministic generation (seed-based)

**Generation Pipeline:**
```
Seed → Perlin Noise → Height Map → Biome Selection → 
Voxel Placement → Cave Carving → Asset Placement → Chunk Ready
```

### 4. Physics System

**Components:**
- `PhysicsSystem`: Main physics simulation
- `CollisionDetection`: AABB, sphere, ray collision
- `PhysicsComponent`: Per-entity physics data

**Responsibilities:**
- Simulate gravity and forces
- Detect and resolve collisions
- Raycast for voxel interaction
- Spatial partitioning (broad-phase)

**Collision Pipeline:**
```
Broad Phase (Spatial Hash) → Narrow Phase (Precise Test) → 
Resolution (Separate + Response) → Update Positions
```

### 5. Entity Component System (ECS)

**Components:**
- `EntityManager`: Entity lifecycle management
- `IComponent`: Component interface
- `SystemBase`: System base class

**Responsibilities:**
- Create/destroy entities
- Attach/detach components
- Update systems each frame
- Efficient component storage

**Architecture:**
```cpp
// Entity is just an ID
using Entity = uint64_t;

// Components are data
struct TransformComponent : IComponent {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

// Systems process components
class PhysicsSystem : public SystemBase {
    void update(float deltaTime) override;
};
```

### 6. Resource Management

**Components:**
- `ResourceManager`: Asset loading and caching
- `ModularAssetSystem`: Custom asset pack support

**Responsibilities:**
- Load resources (textures, models, audio)
- Cache frequently used assets
- Hot-reload during development
- Manage asset packs

**Resource Lifecycle:**
```
Load Request → Check Cache → Load from Disk → 
Parse/Decode → Store in Memory → Return Handle
```

### 7. Scripting System

**Components:**
- `ScriptingEngine`: Script execution backend
- `EventSystem`: Pub/sub event communication

**Responsibilities:**
- Execute gameplay scripts
- Expose C++ API to scripts
- Handle game events
- Enable modding

**Event Flow:**
```
Event Emitted → Event Queue → Subscribers Notified → 
Handler Executed → State Updated
```

## Data Flow

### Frame Update Loop

```
┌──────────────────────────────────────────────────────┐
│ 1. Input Processing                                  │
│    - Poll keyboard/mouse                             │
│    - Update InputManager state                       │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 2. Gameplay Update                                   │
│    - Update player controller                        │
│    - Process game logic                              │
│    - Execute scripts                                 │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 3. Physics Simulation                                │
│    - Apply forces                                    │
│    - Detect collisions                               │
│    - Resolve penetrations                            │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 4. World Update                                      │
│    - Stream chunks                                   │
│    - Generate new chunks                             │
│    - Update meshes                                   │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 5. AI Update                                         │
│    - Update behavior trees                           │
│    - Pathfinding                                     │
│    - NPC logic                                       │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 6. Rendering                                         │
│    - Cull invisible chunks                           │
│    - Submit draw calls                               │
│    - Present frame                                   │
└──────────────────────────────────────────────────────┘
```

### Chunk Streaming Flow

```
Player Moves → Update View Position → Calculate Required Chunks →
Priority Queue (Distance Sort) → Background Thread Generation →
Chunk Ready → Upload to GPU → Add to Render List
```

### Voxel Interaction Flow

```
Mouse Click → Raycast from Camera → DDA Algorithm → 
Hit Voxel? → Get Block Type → Perform Action (Place/Remove) →
Mark Chunk Dirty → Regenerate Mesh → Update GPU Buffer
```

## Module Dependencies

```
Application
    ↓
Core (Engine, Window, Memory, Resources, Scene)
    ↓
Gameplay (Player, Inventory, Crafting) ← Input Manager
    ↓
World (VoxelWorld, ChunkStreamer, TerrainGen)
    ↓
Simulation (Physics, AI, Networking)
    ↓
Platform (Renderer, Audio, Scripting)
```

**Dependency Rules:**
- Higher layers can depend on lower layers
- Lower layers NEVER depend on higher layers
- Sibling layers communicate via events or interfaces

## Design Patterns

### 1. Entity Component System (ECS)

**Purpose**: Composition over inheritance for game objects

```cpp
// Traditional inheritance - BAD
class Enemy : public GameObject, public Renderable, 
              public Collidable, public Animated { };

// ECS approach - GOOD
Entity enemy = entityManager.create();
entityManager.addComponent<TransformComponent>(enemy, ...);
entityManager.addComponent<RenderComponent>(enemy, ...);
entityManager.addComponent<CollisionComponent>(enemy, ...);
```

### 2. Command Pattern (Undo/Redo)

**Purpose**: Terraforming undo/redo functionality

```cpp
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class PlaceBlockCommand : public ICommand {
    void execute() override { world.setVoxel(pos, type); }
    void undo() override { world.setVoxel(pos, oldType); }
};
```

### 3. Observer Pattern (Events)

**Purpose**: Decouple systems via event communication

```cpp
// Subscribe to events
eventSystem.subscribe("player_moved", [](const EventData& data) {
    chunkStreamer.updatePlayerPosition(data.get<glm::vec3>("position"));
});

// Emit events
eventSystem.emit("player_moved", {{"position", playerPos}});
```

### 4. Singleton (Managers)

**Purpose**: Global access to core managers (limited use)

```cpp
class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }
private:
    ResourceManager() = default;
};
```

### 5. Object Pool

**Purpose**: Reduce allocation overhead for frequently created objects

```cpp
template<typename T>
class ObjectPool {
public:
    T* acquire();
    void release(T* obj);
private:
    std::vector<T*> m_pool;
};
```

### 6. Factory Pattern

**Purpose**: Centralized object creation

```cpp
class ChunkFactory {
public:
    static std::unique_ptr<Chunk> createChunk(int x, int z, uint32_t seed) {
        auto chunk = std::make_unique<Chunk>();
        terrainGen.generate(*chunk, x, z, seed);
        return chunk;
    }
};
```

## Performance Considerations

### 1. Chunk Streaming

**Strategy**: Load/unload chunks in a circular pattern around the player

```
View Distance = 8 chunks
Active Chunks = π × 8² ≈ 201 chunks
Memory per Chunk = ~1 MB
Total Memory ≈ 201 MB
```

**Optimization:**
- Priority queue based on distance
- Background thread for generation
- Unload distant chunks immediately
- Keep meshes in GPU memory

### 2. Greedy Meshing

**Problem**: Naive approach creates 6 faces per visible voxel
**Solution**: Merge adjacent faces into larger quads

```
Before: 16×16×16 = 4096 voxels × 6 faces = 24,576 faces
After: ~2,000 faces (90% reduction)
```

### 3. Face Culling

**Strategy**: Only generate faces between different block types

```cpp
if (voxel.type != neighbor.type && neighbor.type != VoxelType::Air) {
    generateFace(direction);
}
```

### 4. Spatial Partitioning

**Purpose**: Optimize collision detection

```cpp
// O(n²) - Check all pairs
for (auto& a : entities)
    for (auto& b : entities)
        if (collides(a, b)) { ... }

// O(n) - Spatial hash, check neighbors only
for (auto& entity : entities) {
    auto neighbors = spatialHash.query(entity.bounds);
    for (auto& neighbor : neighbors)
        if (collides(entity, neighbor)) { ... }
}
```

### 5. Memory Management

**Strategies:**
- Object pooling for frequent allocations (chunks, entities)
- Stack allocators for temporary/per-frame allocations
- Reference counting with smart pointers
- Custom allocators for specific subsystems

### 6. Multi-threading

**Parallelizable Tasks:**
- Chunk generation (terrain, meshing)
- Physics simulation (isolated groups)
- Asset loading
- Audio processing

**Synchronization Points:**
- Main thread: Game logic, rendering submission
- Worker threads: Background tasks
- Lockless queues for task communication

## Threading Model

```
┌────────────────────────────────────────────────┐
│ Main Thread                                    │
│ - Game logic                                   │
│ - Input processing                             │
│ - Render command submission                    │
│ - Frame synchronization                        │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│ Chunk Generation Thread                        │
│ - Procedural terrain generation                │
│ - Mesh generation                              │
│ - Asset placement                              │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│ Physics Thread (Optional)                      │
│ - Collision detection                          │
│ - Physics simulation                           │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│ Asset Loading Thread                           │
│ - Load models, textures                        │
│ - Decompress data                              │
│ - Parse files                                  │
└────────────────────────────────────────────────┘
```

## Rendering Pipeline

```
┌──────────────────────────────────────────────────────┐
│ 1. Culling                                           │
│    - Frustum culling (chunks outside view)           │
│    - Occlusion culling (chunks behind others)        │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 2. Command Recording                                 │
│    - Begin render pass                               │
│    - Bind pipeline                                   │
│    - Bind descriptors (textures, uniforms)           │
│    - Draw commands                                   │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 3. Vertex Processing (GPU)                           │
│    - Vertex shader                                   │
│    - Transform to clip space                         │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 4. Rasterization (GPU)                               │
│    - Convert triangles to fragments                  │
│    - Depth testing                                   │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 5. Fragment Processing (GPU)                         │
│    - Fragment shader                                 │
│    - Lighting calculations                           │
│    - Texture sampling                                │
└─────────────────┬────────────────────────────────────┘
                  ▼
┌──────────────────────────────────────────────────────┐
│ 6. Present                                           │
│    - Swap buffers                                    │
│    - Display frame                                   │
└──────────────────────────────────────────────────────┘
```

## Memory Layout

### Chunk Memory Layout

```
Chunk (65,536 voxels):
├── Voxel Data: 64 KB (1 byte per voxel type)
├── Mesh Data: Variable (depends on visible faces)
│   ├── Vertices: Vec3 position, Vec3 normal, Vec2 UV
│   ├── Indices: uint32_t
│   └── Vertex Count: ~8,000 on average
└── Metadata: 128 bytes
Total: ~200-500 KB per chunk (with mesh)
```

### GPU Memory

```
Per Frame:
├── Uniform Buffers: 256 bytes (view/projection matrices)
├── Vertex Buffers: Dynamic (per visible chunk)
└── Index Buffers: Dynamic (per visible chunk)

Persistent:
├── Texture Atlas: 4096×4096 × 4 bytes = 64 MB
├── Shader Modules: ~1 MB
└── Framebuffers: Resolution-dependent
```

## Future Architecture Enhancements

### 1. Level of Detail (LOD)

```
Close Range (0-4 chunks): Full detail
Medium Range (4-8 chunks): Half resolution
Far Range (8-16 chunks): Quarter resolution
Very Far (16+ chunks): Billboards or simplified geometry
```

### 2. Asynchronous Rendering

```
Frame N: Game logic updates
Frame N+1: Render commands recorded
Frame N+2: GPU processes frame
```

### 3. Network Architecture (Multiplayer)

```
Client → Game Server → Sector Servers
                    ↓
              Authority Server
                    ↓
            Persistent Storage
```

## Conclusion

The Fresh Voxel Engine architecture is designed for:
- **High Performance**: Optimized algorithms and data structures
- **Scalability**: Streaming and LOD support infinite worlds
- **Maintainability**: Clear separation of concerns
- **Extensibility**: Modular design, easy to add features

For implementation details, see the source code and additional documentation in the `docs/` directory.
