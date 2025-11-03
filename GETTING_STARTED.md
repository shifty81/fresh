# Fresh Voxel Engine - Quick Start Implementation Guide

## 🎯 What This Is

**Fresh** is a standalone C++ game engine built from scratch with:
- **Language**: Pure C++17 (no Unity, no Unreal)
- **Graphics**: Vulkan API for modern rendering
- **Architecture**: Custom ECS-like design
- **Purpose**: Voxel-based multiplayer survival/building game

## 🚀 Current Status vs. Requirements

### ✅ Already Implemented (Foundation)
- Core engine loop and architecture
- Vulkan renderer foundation
- Chunk-based voxel world (16x256x16)
- Procedural terrain with Perlin noise
- 13 voxel types
- Terraforming tools (10 different tools)
- World save/load system
- Basic AI and physics frameworks

### ❌ Critical Missing Features (Needed for Playable Game)
1. **Player Controller** - No movement or camera yet
2. **Chunk Streaming** - World doesn't load/unload dynamically
3. **Input System** - No keyboard/mouse handling
4. **Inventory** - No item management
5. **Resource Gathering** - Can't collect materials
6. **Multiplayer** - No networking
7. **UI** - No ImGui integration yet

## 📋 Implementation Priority Queue

Based on the requirements, here's what needs to be built **NOW** in priority order:

### PHASE 1: Make it Playable (Week 1-2) 🔥 CRITICAL
- [ ] First-person camera with mouse look
- [ ] WASD movement with collision
- [ ] Chunk streaming (infinite world)
- [ ] Input system
- [ ] Build/break blocks with mouse

### PHASE 2: Core Gameplay (Week 3-4)
- [ ] Inventory system
- [ ] Resource nodes (trees, rocks, ores)
- [ ] Gathering mechanics
- [ ] ImGui integration for UI
- [ ] Hotbar system

### PHASE 3: Profession System (Week 5-6)
- [ ] 12 profession classes
- [ ] Lobby/profession selection
- [ ] Profession abilities
- [ ] XP/leveling system

### PHASE 4: Environment (Week 7-8)
- [ ] Day/night cycle
- [ ] Dynamic lighting
- [ ] Weather system (clear, rain, fog)
- [ ] Multiple biomes

### PHASE 5: Multiplayer (Week 9-12)
- [ ] Custom networking library (UDP-based)
- [ ] Host/join lobby
- [ ] Player synchronization
- [ ] Chat system
- [ ] Support 12 players

### PHASE 6: Dungeons & Content (Week 13+)
- [ ] Dungeon generation
- [ ] NPCs and mobs
- [ ] Crafting system
- [ ] Building system

## 🔨 Let's Start Building - Phase 1 Implementation

### Step 1: Player Controller (This is what we'll implement NOW)

Create the following files:

#### `include/gameplay/Camera.h`
```cpp
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fresh {

class Camera {
public:
    Camera(float fov = 75.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
    
    void updateVectors();
    void processMouseMovement(float xOffset, float yOffset, float sensitivity = 0.002f);
    void setPosition(const glm::vec3& pos);
    void setRotation(float pitch, float yaw);
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::vec3 getRight() const { return right; }
    glm::vec3 getUp() const { return up; }
    
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }
    
private:
    glm::vec3 position{0.0f, 80.0f, 0.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    
    float pitch = 0.0f;
    float yaw = -90.0f;
    float fov;
    float nearPlane;
    float farPlane;
};

} // namespace fresh
```

#### `include/gameplay/Player.h`
```cpp
#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

namespace fresh {

class VoxelWorld;
class InputManager;

class Player {
public:
    Player();
    
    void update(float deltaTime);
    void handleInput(const InputManager& input, float deltaTime);
    void handleMouseMovement(float xOffset, float yOffset);
    
    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
    
    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3& pos);
    
    void setWorld(VoxelWorld* w) { world = w; }
    
private:
    void applyGravity(float deltaTime);
    void handleMovement(const glm::vec3& direction, float speed, float deltaTime);
    bool checkCollision(const glm::vec3& newPos);
    
    Camera camera;
    VoxelWorld* world = nullptr;
    
    glm::vec3 position{0.0f, 80.0f, 0.0f};
    glm::vec3 velocity{0.0f};
    
    // Player dimensions
    float height = 1.8f;
    float eyeHeight = 1.6f;
    float radius = 0.3f;
    
    // Movement
    float walkSpeed = 4.3f;
    float sprintSpeed = 5.6f;
    float jumpVelocity = 8.0f;
    float gravity = 20.0f;
    
    // State
    bool isGrounded = false;
    bool isSprinting = false;
    bool isCrouching = false;
};

} // namespace fresh
```

#### `include/input/InputManager.h`
```cpp
#pragma once
#include <map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace fresh {

enum class InputAction {
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
    Crouch,
    Sprint,
    Use,
    Attack,
    PlaceBlock,
    BreakBlock,
    OpenInventory,
    OpenMenu,
    ToggleEditor
};

class InputManager {
public:
    void initialize(GLFWwindow* window);
    void update();
    void processKeyEvent(int key, int action);
    void processMouseMovement(double xpos, double ypos);
    void processMouseButton(int button, int action);
    
    bool isActionActive(InputAction action) const;
    bool isActionJustPressed(InputAction action) const;
    glm::vec2 getMouseDelta() const { return mouseDelta; }
    
    void setMouseSensitivity(float sens) { sensitivity = sens; }
    float getMouseSensitivity() const { return sensitivity; }
    
    void setCursorMode(bool captured);
    
private:
    GLFWwindow* window = nullptr;
    std::map<InputAction, int> keyBindings;
    std::map<int, bool> keyStates;
    std::map<int, bool> keyPressedThisFrame;
    std::map<int, bool> mouseButtonStates;
    
    glm::vec2 mouseDelta{0.0f};
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    float sensitivity = 1.0f;
    
    void initializeDefaultBindings();
};

} // namespace fresh
```

#### `include/voxel/ChunkStreamer.h`
```cpp
#pragma once
#include <glm/glm.hpp>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>

namespace fresh {

class VoxelWorld;
class Chunk;

struct ChunkLoadRequest {
    glm::ivec2 chunkPos;
    int priority;
    
    bool operator<(const ChunkLoadRequest& other) const {
        return priority > other.priority; // Lower priority value = higher priority
    }
};

class ChunkStreamer {
public:
    ChunkStreamer(VoxelWorld* world);
    ~ChunkStreamer();
    
    void update(const glm::vec3& playerPosition);
    void setViewDistance(int chunks);
    int getViewDistance() const { return viewDistance; }
    
    void shutdown();
    
private:
    void determineChunksToLoad(const glm::vec3& playerPos);
    void determineChunksToUnload(const glm::vec3& playerPos);
    void processLoadQueue();
    
    // Background generation
    void generationThreadFunc();
    
    VoxelWorld* world;
    int viewDistance = 8; // chunks
    int maxLoadedChunks = 1000;
    
    std::priority_queue<ChunkLoadRequest> loadQueue;
    std::vector<glm::ivec2> unloadQueue;
    
    // Threading
    std::thread generationThread;
    std::mutex queueMutex;
    std::atomic<bool> shouldRun{true};
    
    glm::ivec2 lastPlayerChunk{0, 0};
};

} // namespace fresh
```

### File Structure After Phase 1

```
include/
├── gameplay/
│   ├── Camera.h          ← NEW
│   ├── Player.h          ← NEW
│   └── Inventory.h       ← Phase 2
├── input/
│   └── InputManager.h    ← NEW
└── voxel/
    └── ChunkStreamer.h   ← NEW

src/
├── gameplay/
│   ├── Camera.cpp        ← NEW
│   ├── Player.cpp        ← NEW
│   └── Inventory.cpp     ← Phase 2
├── input/
│   └── InputManager.cpp  ← NEW
└── voxel/
    └── ChunkStreamer.cpp ← NEW
```

## 🎮 Testing After Phase 1

You should be able to:
1. Start the game
2. Move with WASD
3. Look around with mouse
4. Jump with Space
5. Sprint with Shift
6. See chunks loading as you move
7. Break/place blocks with mouse clicks

## 📊 Progress Tracking

Use this checklist to track implementation:

```markdown
## Phase 1: Playable Foundation
- [ ] Camera.h/cpp implemented
- [ ] Player.h/cpp implemented
- [ ] InputManager.h/cpp implemented
- [ ] ChunkStreamer.h/cpp implemented
- [ ] Integrated into Engine.cpp
- [ ] Collision detection working
- [ ] Chunk streaming working
- [ ] Mouse capture working
- [ ] Tested and playable

## Phase 2: Inventory & Resources
- [ ] Item.h/cpp
- [ ] Inventory.h/cpp
- [ ] ResourceNode.h/cpp
- [ ] GatheringSystem.h/cpp
- [ ] ImGui integrated
- [ ] Inventory UI working

## Phase 3: Professions
- [ ] Profession base class
- [ ] 12 profession implementations
- [ ] Lobby system
- [ ] Profession selection UI

## Phase 4: Environment
- [ ] TimeOfDay system
- [ ] SkyRenderer
- [ ] WeatherSystem
- [ ] BiomeManager expanded

## Phase 5: Networking
- [ ] NetworkManager
- [ ] Packet serialization
- [ ] Player sync
- [ ] Chat system

## Phase 6: Content
- [ ] DungeonGenerator
- [ ] NPC system
- [ ] Crafting system
```

## 🔧 Build Instructions

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install cmake g++ libvulkan-dev libglfw3-dev libglm-dev

# Build
cd /home/runner/work/fresh/fresh
mkdir -p build
cd build
cmake ..
make -j$(nproc)

# Run
./FreshVoxelEngine
```

## 📚 Architecture Principles

### 1. **Pure C++** - No external engines
- Custom everything (physics, rendering, networking)
- Full control over performance
- Learning-focused implementation

### 2. **Modular Design**
- Each system is independent
- Easy to test and modify
- Clear interfaces between systems

### 3. **Performance First**
- Chunk-based rendering
- LOD system for distant terrain
- Efficient networking
- Multi-threaded generation

### 4. **Multiplayer Ready**
- Server-authoritative design
- Prediction and reconciliation
- Efficient state synchronization

## 🎯 Next Steps

1. **NOW**: Implement Phase 1 (Player Controller)
2. **Next**: Implement Phase 2 (Inventory)
3. **Then**: Follow the roadmap in ROADMAP.md

Let's start building! 🚀
