# Fresh Voxel Engine API Reference

> **Status:** 🚧 Under Construction
> 
> This API reference is being built. Check back soon for complete documentation!

## Overview

This directory contains API documentation for Fresh Voxel Engine. The documentation is organized by system/module.

## Available Documentation

### Core Systems
- [ ] [Engine](core/Engine.md) - Main engine class
- [ ] [Window](core/Window.md) - Window management
- [ ] [ResourceManager](core/ResourceManager.md) - Asset management
- [ ] [SceneManager](core/SceneManager.md) - Scene graph
- [ ] [MemoryManager](core/MemoryManager.md) - Memory management

### Rendering
- [ ] [VulkanRenderer](renderer/VulkanRenderer.md) - Main renderer
- [ ] [VulkanDevice](renderer/VulkanDevice.md) - Device management
- [ ] [VulkanPipeline](renderer/VulkanPipeline.md) - Pipeline setup
- [ ] [ShaderManager](renderer/ShaderManager.md) - Shader system

### Voxel World
- [ ] [VoxelWorld](voxel/VoxelWorld.md) - World container
- [ ] [Chunk](voxel/Chunk.md) - Chunk data structure
- [ ] [MeshGenerator](voxel/MeshGenerator.md) - Mesh generation
- [ ] [ChunkStreamer](voxel/ChunkStreamer.md) - Chunk streaming

### Procedural Generation
- [ ] [NoiseGenerator](generation/NoiseGenerator.md) - Noise functions
- [ ] [TerrainGenerator](generation/TerrainGenerator.md) - Terrain generation

### Physics & Collision
- [ ] [PhysicsSystem](physics/PhysicsSystem.md) - Physics simulation
- [ ] [CollisionDetection](physics/CollisionDetection.md) - Collision detection
- [ ] [RaycastSystem](interaction/RaycastSystem.md) - Raycasting

### Gameplay
- [ ] [Player](gameplay/Player.md) - Player controller
- [ ] [Camera](gameplay/Camera.md) - Camera system
- [ ] [InputManager](input/InputManager.md) - Input handling

### World Editing
- [ ] [TerraformingSystem](editor/TerraformingSystem.md) - Terraforming
- [ ] [WorldEditor](editor/WorldEditor.md) - World editing

### AI & Behavior
- [ ] [AISystem](ai/AISystem.md) - AI management
- [ ] [BehaviorTree](ai/BehaviorTree.md) - Behavior trees

### Other Systems
- [ ] [AudioEngine](audio/AudioEngine.md) - Audio system
- [ ] [ScriptingEngine](scripting/ScriptingEngine.md) - Scripting
- [ ] [EventSystem](scripting/EventSystem.md) - Event system
- [ ] [ModularAssetSystem](assets/ModularAssetSystem.md) - Asset packs

## Documentation Format

Each API document follows this structure:

```markdown
# ClassName

## Overview
Brief description of the class and its purpose.

## Header File
`#include "path/to/ClassName.h"`

## Constructor
Constructor signatures and parameters.

## Methods
Detailed method documentation.

## Usage Examples
Code examples showing common usage patterns.

## See Also
Links to related classes.
```

## Quick Reference

### Common Patterns

#### Initializing the Engine

```cpp
#include "core/Engine.h"
#include "core/Window.h"

auto window = std::make_unique<Window>(1920, 1080, "My Game");
window->initialize();

Engine engine;
engine.initialize(window.get());
```

#### Creating a Voxel World

```cpp
#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"

VoxelWorld world;
TerrainGenerator terrainGen(12345);

auto chunk = terrainGen.generateChunk(0, 0);
world.addChunk(0, 0, std::move(chunk));
```

#### Player Movement

```cpp
#include "gameplay/Player.h"
#include "gameplay/Camera.h"

Camera camera;
Player player(&camera);

// In game loop:
player.update(deltaTime);
camera.update();
```

## Generating Documentation

This API reference will be generated from source code using Doxygen.

### Prerequisites

```bash
sudo apt-get install doxygen graphviz
```

### Generate Docs

```bash
doxygen Doxyfile
```

Documentation will be generated in `docs/api/html/`.

## Contributing

Help us improve the API documentation!

1. Add Doxygen comments to source code
2. Create example usage code
3. Write tutorials for complex systems
4. Report unclear or missing documentation

See [CONTRIBUTING.md](../community/CONTRIBUTING.md) for more details.

## Resources

- [Doxygen Documentation](https://www.doxygen.nl/manual/)
- [C++ API Design Best Practices](https://www.apidesign.dev/)
- [Example Code](../../examples/)
- [Tutorials](../tutorials/)

---

**Note:** This is a living document. As the engine evolves, this documentation will be updated to reflect changes.
