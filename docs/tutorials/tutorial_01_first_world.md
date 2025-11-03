# Tutorial 1: Your First Voxel World

## Introduction

Welcome to the Fresh Voxel Engine! In this tutorial, you'll create your first voxel world with procedural terrain generation. By the end, you'll have a working 3D voxel world that you can explore.

**What you'll learn:**
- Setting up the engine
- Creating a voxel world
- Generating terrain procedurally
- Rendering chunks
- Basic camera controls

**Prerequisites:**
- C++ knowledge (basic to intermediate)
- Fresh Voxel Engine installed and building
- Familiarity with 3D concepts (vectors, matrices)

**Time:** ~30 minutes

## Step 1: Project Setup

First, create a new C++ file for your project:

```cpp
// my_first_world.cpp
#include <iostream>
#include <memory>
#include "core/Engine.h"
#include "core/Window.h"
#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"
#include "gameplay/Camera.h"
#include "gameplay/Player.h"

int main() {
    // We'll add code here
    return 0;
}
```

Add this to your `CMakeLists.txt`:

```cmake
add_executable(MyFirstWorld my_first_world.cpp)
target_link_libraries(MyFirstWorld PRIVATE FreshVoxelEngine)
```

## Step 2: Initialize the Engine

Let's initialize the engine with a window:

```cpp
int main() {
    std::cout << "Creating My First Voxel World!" << std::endl;
    
    try {
        // Create window (1920x1080, adjust to your monitor)
        auto window = std::make_unique<Window>(1920, 1080, "My First Voxel World");
        if (!window->initialize()) {
            std::cerr << "Failed to create window!" << std::endl;
            return 1;
        }
        
        // Initialize engine
        Engine engine;
        if (!engine.initialize(window.get())) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return 1;
        }
        
        std::cout << "Engine initialized successfully!" << std::endl;
        
        // More code will go here...
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

**What's happening:**
- `Window` manages the OS window and Vulkan surface
- `Engine` initializes all core systems (renderer, resource manager, etc.)
- Always check return values for errors!

## Step 3: Create the Voxel World

Now let's create our voxel world:

```cpp
// After engine initialization...

// Create the voxel world
auto world = std::make_unique<VoxelWorld>();
std::cout << "Voxel world created!" << std::endl;

// Create terrain generator with a seed
uint32_t worldSeed = 12345; // Change this for different worlds
TerrainGenerator terrainGen(worldSeed);
std::cout << "Terrain generator ready (seed: " << worldSeed << ")" << std::endl;
```

**Understanding the world:**
- `VoxelWorld` manages all chunks in the world
- Chunks are 16×256×16 voxel sections
- `TerrainGenerator` creates terrain using procedural noise
- The seed determines what terrain is generated

## Step 4: Generate Initial Chunks

Let's generate chunks in a radius around the player spawn:

```cpp
// Generate chunks in a 5x5 grid
std::cout << "Generating terrain..." << std::endl;

const int chunkRadius = 2; // 5x5 grid (-2 to +2)
int chunksGenerated = 0;

for (int x = -chunkRadius; x <= chunkRadius; x++) {
    for (int z = -chunkRadius; z <= chunkRadius; z++) {
        // Generate chunk at position (x, z)
        auto chunk = terrainGen.generateChunk(x, z);
        
        // Add to world
        world->addChunk(x, z, std::move(chunk));
        chunksGenerated++;
    }
}

std::cout << "Generated " << chunksGenerated << " chunks!" << std::endl;
```

**What's happening:**
- We generate a 5×5 grid of chunks (25 total)
- Each chunk is 16×256×16 voxels
- Total voxels: 25 × 16 × 256 × 16 = 1,638,400 voxels!
- Chunks are stored in the world by their (x, z) coordinates

## Step 5: Set Up Camera and Player

Let's add a camera so we can see our world:

```cpp
// Create camera
auto camera = std::make_unique<Camera>();
camera->setPosition(glm::vec3(0.0f, 100.0f, 0.0f)); // Start at y=100
camera->setFOV(70.0f); // Field of view in degrees
camera->setNearFar(0.1f, 1000.0f); // Near and far clip planes

std::cout << "Camera created at position (0, 100, 0)" << std::endl;

// Create player controller
auto player = std::make_unique<Player>(camera.get());
player->setMoveSpeed(10.0f); // 10 units per second
player->setLookSensitivity(0.1f); // Mouse sensitivity

std::cout << "Player controller ready!" << std::endl;
```

**Camera settings:**
- Position: Where the camera is in world space
- FOV: Field of view (wider = see more, narrower = zoomed in)
- Near/Far planes: What distances to render

## Step 6: The Main Game Loop

Now for the main loop that updates and renders everything:

```cpp
std::cout << "Starting main loop. Press ESC to exit." << std::endl;

double lastTime = glfwGetTime();

while (engine.isRunning()) {
    // Calculate delta time
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;
    
    // Update player (handles input, movement)
    player->update(deltaTime);
    
    // Update camera
    camera->update();
    
    // Update engine (processes events, etc.)
    engine.update(deltaTime);
    
    // Render the world
    engine.beginFrame();
    world->render(camera.get());
    engine.endFrame();
}

// Cleanup
std::cout << "Shutting down..." << std::endl;
engine.shutdown();
window->shutdown();
```

**The game loop:**
1. Calculate how much time has passed (delta time)
2. Update player and camera based on input
3. Update engine systems
4. Render the world from the camera's perspective
5. Repeat until user exits (ESC or closes window)

## Step 7: Build and Run

Build your project:

```bash
cd build
cmake --build .
./MyFirstWorld
```

You should see:
- A window opens
- Console output showing initialization steps
- A 3D voxel world with procedurally generated terrain
- WASD to move, mouse to look around

## Congratulations! 🎉

You've created your first voxel world! Here's what you built:

✅ Engine initialization
✅ Voxel world with 25 chunks
✅ Procedural terrain generation
✅ Camera system
✅ Player movement controls
✅ Real-time 3D rendering

## Next Steps

Now that you have a working world, try these enhancements:

### Easy Modifications

1. **Change the world seed** - Generate different terrain
   ```cpp
   uint32_t worldSeed = 99999; // Try different numbers!
   ```

2. **Adjust chunk count** - More or fewer chunks
   ```cpp
   const int chunkRadius = 4; // 9x9 grid (81 chunks)
   ```

3. **Change player speed** - Move faster or slower
   ```cpp
   player->setMoveSpeed(20.0f); // Twice as fast!
   ```

### Intermediate Enhancements

4. **Add chunk streaming** - Load/unload chunks as player moves
   ```cpp
   #include "voxel/ChunkStreamer.h"
   
   ChunkStreamer streamer(world.get(), &terrainGen);
   streamer.setViewDistance(8); // 8 chunks in all directions
   
   // In game loop:
   streamer.update(player->getPosition());
   ```

5. **Display FPS counter** - Monitor performance
   ```cpp
   int frameCount = 0;
   double lastFPSTime = glfwGetTime();
   
   // In game loop:
   frameCount++;
   if (currentTime - lastFPSTime >= 1.0) {
       std::cout << "FPS: " << frameCount << std::endl;
       frameCount = 0;
       lastFPSTime = currentTime;
   }
   ```

### Advanced Features

6. **Add voxel interaction** - Place and break blocks
   ```cpp
   #include "interaction/RaycastSystem.h"
   
   RaycastSystem raycast(world.get());
   
   // On mouse click:
   auto hit = raycast.castRay(camera->getPosition(), 
                               camera->getDirection(), 
                               10.0f);
   if (hit.hit) {
       world->setVoxel(hit.position, VoxelType::Stone);
   }
   ```

7. **Implement day/night cycle** - Coming in future tutorial!

8. **Add multiplayer support** - Coming in future tutorial!

## Common Issues

### Issue: Black screen / nothing renders

**Solution:**
- Check console for errors
- Ensure chunks are being generated
- Verify camera is above ground (y > 64)
- Check Vulkan drivers are installed

### Issue: Low FPS

**Solution:**
- Reduce chunk radius
- Enable VSync in Vulkan settings
- Check GPU usage in task manager
- Ensure Release build (not Debug)

### Issue: Camera moves through blocks

**Solution:**
- Collision detection not implemented yet
- Add physics system (Tutorial 3)
- Use `world->getVoxel()` to check ahead

## Full Example Code

See [examples/my_first_world.cpp](../../examples/my_first_world.cpp) for the complete, working example.

## Additional Resources

- [Tutorial 2: Terraforming and World Editing](tutorial_02_terraforming.md)
- [Tutorial 3: Physics and Collision](tutorial_03_physics.md)
- [Tutorial 4: Custom Asset Packs](tutorial_04_assets.md)
- [Architecture Overview](../ARCHITECTURE.md)
- [API Reference](../api/) (coming soon)

## Get Help

- [GitHub Issues](https://github.com/shifty81/fresh/issues) - Report bugs
- [GitHub Discussions](https://github.com/shifty81/fresh/discussions) - Ask questions
- [Discord](https://discord.gg/...) - Community chat (coming soon)

Happy voxel building! 🎮
