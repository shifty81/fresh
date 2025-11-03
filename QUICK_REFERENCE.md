# Fresh Voxel Engine - Quick Reference

A one-page reference for common tasks and APIs.

## 🚀 Quick Start

```bash
# Clone and build
git clone https://github.com/shifty81/fresh.git
cd fresh && mkdir build && cd build
cmake .. && make -j$(nproc)
./FreshVoxelEngine
```

## 📦 Basic Setup

```cpp
#include "core/Engine.h"
#include "core/Window.h"

// Create window
auto window = std::make_unique<Window>(1920, 1080, "My Game");
window->initialize();

// Initialize engine
Engine engine;
engine.initialize(window.get());

// Main loop
while (engine.isRunning()) {
    engine.update(deltaTime);
    engine.render();
}
```

## 🌍 World Creation

```cpp
#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"

// Create world
VoxelWorld world;

// Generate terrain
TerrainGenerator terrainGen(12345); // seed
auto chunk = terrainGen.generateChunk(0, 0);
world.addChunk(0, 0, std::move(chunk));

// Access voxels
VoxelType voxel = world.getVoxel(x, y, z);
world.setVoxel(x, y, z, VoxelType::Stone);
```

## 🎮 Player & Camera

```cpp
#include "gameplay/Camera.h"
#include "gameplay/Player.h"

// Create camera
Camera camera;
camera.setPosition(glm::vec3(0, 100, 0));
camera.setFOV(70.0f);

// Create player
Player player(&camera);
player.setMoveSpeed(10.0f);

// In game loop
player.update(deltaTime);
camera.update();
```

## 🔧 Terraforming

```cpp
#include "editor/TerraformingSystem.h"

TerraformingSystem terraform(&world);

// Place sphere
terraform.setTool(TerraformTool::FilledSphere);
terraform.setVoxelType(VoxelType::Stone);
terraform.setToolSize(5);
terraform.applyTool(WorldPos(0, 70, 0));

// Undo/Redo
terraform.undo();
terraform.redo();
```

## 📦 Asset Loading

```cpp
#include "assets/ModularAssetSystem.h"

auto& assetSystem = ModularAssetSystem::getInstance();

// Scan for asset packs
assetSystem.scanAndLoadAssetPacks();

// Generate assets for chunk
auto assets = assetSystem.generateAssetsForChunk(
    world, chunkX, chunkZ, seed
);

// Place in world
assetSystem.placeAssetsInWorld(assets, world);
```

## 🎲 Noise Generation

```cpp
#include "generation/NoiseGenerator.h"

NoiseGenerator noise(12345);
noise.setOctaves(6);
noise.setFrequency(0.01f);
noise.setPersistence(0.5f);

// 2D noise
float value = noise.noise2D(x, z);

// 3D noise
float value = noise.noise3D(x, y, z);

// Fractal noise
float value = noise.fractalNoise2D(x, z);
```

## ⚡ Physics

```cpp
#include "physics/PhysicsSystem.h"
#include "physics/CollisionDetection.h"

PhysicsSystem physics;
CollisionDetection collision;

// AABB collision
AABB box1, box2;
if (collision.testAABB(box1, box2)) {
    // Collision!
}

// Raycast
RaycastResult hit = collision.raycast(
    origin, direction, maxDistance, world
);
if (hit.hit) {
    // Hit at hit.position
}
```

## 🎨 Voxel Types

```cpp
enum class VoxelType : uint8_t {
    Air = 0,
    Stone = 1,
    Dirt = 2,
    Grass = 3,
    Sand = 4,
    Water = 5,
    Wood = 6,
    Leaves = 7,
    Coal = 8,
    Iron = 9,
    Gold = 10,
    Diamond = 11,
    Bedrock = 12
};
```

## 🔄 Chunk Streaming

```cpp
#include "voxel/ChunkStreamer.h"

ChunkStreamer streamer(&world, &terrainGen);
streamer.setViewDistance(8); // chunks

// In game loop
streamer.update(playerPosition);
```

## 💾 World Serialization

```cpp
#include "serialization/WorldSerializer.h"

WorldSerializer serializer;

// Save world
serializer.saveWorld(world, "saves/myworld.world");

// Load world
auto loadedWorld = serializer.loadWorld("saves/myworld.world");
```

## 🎯 Input Handling

```cpp
#include "input/InputManager.h"

InputManager input;

// Check key state
if (input.isKeyPressed(GLFW_KEY_W)) {
    // W key pressed
}

// Mouse position
glm::vec2 mousePos = input.getMousePosition();
glm::vec2 mouseDelta = input.getMouseDelta();

// Bind actions
input.bindAction("jump", GLFW_KEY_SPACE);
if (input.isActionPressed("jump")) {
    // Jump!
}
```

## 🎵 Audio

```cpp
#include "audio/AudioEngine.h"

AudioEngine audio;
audio.initialize();

// Play 2D sound
audio.play2D("sounds/click.wav", false); // not looped

// Play 3D sound
audio.play3D("sounds/explosion.wav", position, false);

// Background music
audio.playMusic("music/theme.mp3", true); // looped
audio.setMusicVolume(0.5f);
```

## 🧠 AI & Behavior Trees

```cpp
#include "ai/BehaviorTree.h"
#include "ai/AISystem.h"

// Create behavior tree
auto tree = std::make_unique<BehaviorTree>();

// Add nodes
tree->addNode(BehaviorNode::Sequence {
    BehaviorNode::Condition("enemyInRange"),
    BehaviorNode::Action("attack")
});

// Execute
BehaviorState state = tree->execute(entity);
```

## 📊 Resource Management

```cpp
#include "core/ResourceManager.h"

auto& resources = ResourceManager::getInstance();

// Load texture
auto texture = resources.loadTexture("textures/stone.png");

// Load model
auto model = resources.loadModel("models/tree.obj");

// Hot reload
resources.enableHotReload(true);
```

## 🧩 Entity Component System

```cpp
#include "ecs/EntityManager.h"

EntityManager entities;

// Create entity
Entity entity = entities.create();

// Add components
entities.addComponent<TransformComponent>(entity, position);
entities.addComponent<RenderComponent>(entity, model);
entities.addComponent<PhysicsComponent>(entity, mass);

// Query entities
auto view = entities.view<TransformComponent, PhysicsComponent>();
for (auto entity : view) {
    auto& transform = entities.getComponent<TransformComponent>(entity);
    auto& physics = entities.getComponent<PhysicsComponent>(entity);
    // Update...
}
```

## 📝 Events

```cpp
#include "scripting/EventSystem.h"

auto& events = EventSystem::getInstance();

// Subscribe
events.subscribe("playerMoved", [](const EventData& data) {
    auto pos = data.get<glm::vec3>("position");
    // Handle event...
});

// Emit
EventData data;
data.set("position", playerPosition);
events.emit("playerMoved", data);
```

## 💾 Memory Management

```cpp
#include "core/MemoryManager.h"

auto& memory = MemoryManager::getInstance();

// Track allocation
void* ptr = memory.allocate(size, "MySystem");
memory.deallocate(ptr, "MySystem");

// Get statistics
auto stats = memory.getStats();
std::cout << "Total allocated: " << stats.totalAllocated << std::endl;
```

## 🎬 Scene Management

```cpp
#include "core/SceneManager.h"

SceneManager scenes;

// Create scene
auto scene = scenes.createScene("MainScene");

// Add nodes
auto root = scene->getRootNode();
auto node = root->createChild("Player");
node->setPosition(glm::vec3(0, 100, 0));

// Switch scenes
scenes.loadScene("MainScene");
```

## ⚙️ Configuration

```cpp
// CMake build types
cmake .. -DCMAKE_BUILD_TYPE=Debug     // Development
cmake .. -DCMAKE_BUILD_TYPE=Release   // Production

// Enable/disable features
cmake .. -DENABLE_VALIDATION_LAYERS=ON
cmake .. -DENABLE_HOT_RELOAD=ON
```

## 🐛 Debugging

```cpp
// Enable Vulkan validation
#define ENABLE_VALIDATION_LAYERS

// Log levels
#include <iostream>
std::cout << "Info: " << message << std::endl;
std::cerr << "Error: " << message << std::endl;

// Assert
#include <cassert>
assert(condition && "Error message");
```

## 📐 Common Types

```cpp
// Math types (GLM)
glm::vec2   // 2D vector
glm::vec3   // 3D vector  
glm::vec4   // 4D vector
glm::mat4   // 4x4 matrix
glm::quat   // Quaternion

// Engine types
Entity      // uint64_t
ChunkPos    // glm::ivec2
WorldPos    // glm::ivec3
VoxelType   // uint8_t enum
```

## 🔑 Key Constants

```cpp
// Chunk dimensions
const int CHUNK_SIZE_X = 16;
const int CHUNK_SIZE_Y = 256;
const int CHUNK_SIZE_Z = 16;

// Voxel count per chunk
const int VOXELS_PER_CHUNK = 16 * 256 * 16; // 65,536

// Default render distance
const int DEFAULT_VIEW_DISTANCE = 8; // chunks
```

## 📚 Common Includes

```cpp
#include "core/Engine.h"              // Main engine
#include "core/Window.h"              // Window management
#include "voxel/VoxelWorld.h"         // Voxel world
#include "generation/TerrainGenerator.h"  // Terrain
#include "gameplay/Player.h"          // Player
#include "gameplay/Camera.h"          // Camera
#include "input/InputManager.h"       // Input
#include "physics/PhysicsSystem.h"    // Physics
#include "editor/TerraformingSystem.h"// World editing
```

## 🔗 Useful Links

- [Full Documentation](docs/)
- [Tutorials](docs/tutorials/)
- [Examples](examples/)
- [API Reference](docs/api/)
- [GitHub Repo](https://github.com/shifty81/fresh)

## 💡 Tips

- Always check return values for errors
- Use smart pointers (unique_ptr, shared_ptr)
- Enable validation layers in debug builds
- Profile before optimizing
- Use RAII for resource management
- Keep update and render separate
- Prefer const references for parameters

## 🎯 Best Practices

```cpp
// ✅ Good: RAII
auto window = std::make_unique<Window>(800, 600, "Game");

// ❌ Bad: Raw pointer
Window* window = new Window(800, 600, "Game");
// ... (might forget to delete)

// ✅ Good: Smart pointers
std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();

// ✅ Good: Const correctness
void render(const Camera& camera) const;

// ✅ Good: Move semantics
world.addChunk(x, z, std::move(chunk));
```

---

For detailed information, see the full documentation in [docs/](docs/).
