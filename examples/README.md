# Fresh Voxel Engine - Examples

This directory contains practical examples demonstrating the Fresh Voxel Engine's features.

## 📚 Available Examples

### 1. Getting Started
- **[01_hello_engine.cpp](01_hello_engine.cpp)** - Minimal engine initialization
- **[02_create_window.cpp](02_create_window.cpp)** - Open a window with DirectX
- **[03_logging_demo.cpp](03_logging_demo.cpp)** - Using the logging system

### 2. Voxel World
- **[04_create_world.cpp](04_create_world.cpp)** - Create a simple voxel world
- **[05_terrain_generation.cpp](05_terrain_generation.cpp)** - Procedural terrain
- **[06_chunk_system.cpp](06_chunk_system.cpp)** - Understanding chunks

### 3. Player & Interaction
- **[07_player_movement.cpp](07_player_movement.cpp)** - First-person controls
- **[08_place_remove_blocks.cpp](08_place_remove_blocks.cpp)** - Block interaction
- **[09_raycasting.cpp](09_raycasting.cpp)** - Block selection with raycasting

### 4. Advanced Features
- **[10_editor_integration.cpp](10_editor_integration.cpp)** - Using the editor
- **[11_terraforming.cpp](11_terraforming.cpp)** - Terraforming tools
- **[12_save_load_world.cpp](12_save_load_world.cpp)** - World persistence

## 🚀 Using Examples

### Important Note

**The examples directory contains documentation and code snippets demonstrating API usage patterns.** They are intended for learning and reference, not as standalone executables.

**To play the game**: Run the main `FreshVoxelEngine.exe` executable, which contains all features integrated into a complete playable demo.

**To learn the APIs**: Study the example files to understand how each system works.

## 📖 Example Structure

Each example follows this pattern:

```cpp
/**
 * @file example_name.cpp
 * @brief Brief description
 * 
 * This example demonstrates:
 * - Feature A
 * - Feature B
 * - Feature C
 */

#include "core/Engine.h"
// ... other includes

int main() {
    // Initialize engine
    fresh::Engine engine;
    if (!engine.initialize()) {
        return 1;
    }
    
    // Example-specific code here
    // ...
    
    // Run engine
    engine.run();
    
    // Cleanup
    engine.shutdown();
    
    return 0;
}
```

## 🎯 Learning Path

### Beginner (Start Here!)
1. **01_hello_engine.cpp** - Understand basic initialization
2. **02_create_window.cpp** - See graphics in action
3. **03_logging_demo.cpp** - Learn debugging with logs
4. **04_create_world.cpp** - Your first voxel world

### Intermediate
5. **05_terrain_generation.cpp** - Procedural generation
6. **07_player_movement.cpp** - Player controller
7. **08_place_remove_blocks.cpp** - Interactive gameplay
8. **09_raycasting.cpp** - Advanced selection

### Advanced
9. **10_editor_integration.cpp** - Editor architecture
10. **11_terraforming.cpp** - Advanced world editing
11. **12_save_load_world.cpp** - Serialization patterns

## 💡 Tips

- **Read the Comments** - Each example has detailed documentation
- **Experiment** - Modify parameters and observe changes
- **Build Incrementally** - Start simple, add complexity
- **Check Main Docs** - See [../docs/](../docs/) for system details

## 🔗 Resources

- **Main README** - [../README.md](../README.md)
- **Documentation Index** - [../DOCUMENTATION_INDEX.md](../DOCUMENTATION_INDEX.md)
- **Build Instructions** - [../BUILD.md](../BUILD.md)
- **API Reference** - [../docs/api/](../docs/api/)
- **Tutorials** - [../docs/tutorials/](../docs/tutorials/)

## 🆘 Need Help?

- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [Report Issues](https://github.com/shifty81/fresh/issues)
- 📖 [Documentation](https://github.com/shifty81/fresh/tree/main/docs)

---

**Note**: These examples demonstrate the Fresh Voxel Engine API. For a complete playable demo with all features, run the main `FreshVoxelEngine.exe` executable.
