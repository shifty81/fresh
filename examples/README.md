# Fresh Voxel Engine Examples

This directory contains example code demonstrating various features of the Fresh Voxel Engine.

## Available Examples

### 1. Basic Setup
- [basic_setup.cpp](basic_setup.cpp) - Minimal engine initialization

### 2. Voxel World
- [creating_chunks.cpp](creating_chunks.cpp) - Create and manage chunks
- [terrain_generation.cpp](terrain_generation.cpp) - Procedural terrain generation
- [voxel_interaction.cpp](voxel_interaction.cpp) - Place and remove voxels

### 3. World Editing
- [terraforming_tools.cpp](terraforming_tools.cpp) - Using terraforming system
- [custom_structures.cpp](custom_structures.cpp) - Build custom structures

### 4. Asset System
- [loading_assets.cpp](loading_assets.cpp) - Load custom asset packs
- [creating_asset_pack.cpp](creating_asset_pack.cpp) - Create your own asset pack

### 5. Physics & Interaction
- [raycasting.cpp](raycasting.cpp) - Raycast to interact with voxels
- [collision_detection.cpp](collision_detection.cpp) - Detect collisions

### 6. Player & Camera
- [player_controller.cpp](player_controller.cpp) - Implement player movement
- [camera_system.cpp](camera_system.cpp) - Camera controls

## Building Examples

Each example is a standalone program. To build an example:

```bash
# From the repository root
mkdir build && cd build
cmake ..
make

# Run an example (they will be added to CMakeLists.txt)
./examples/basic_setup
```

## Running Examples

Most examples require:
1. Vulkan-capable GPU and drivers
2. GLFW for window management
3. Shader files in the `shaders/` directory

## Contributing Examples

Want to contribute an example? Great! Please:
1. Create a new `.cpp` file in this directory
2. Add clear comments explaining the code
3. Update this README with a description
4. Submit a pull request

## Example Template

```cpp
/**
 * @file example_name.cpp
 * @brief Brief description of what this example demonstrates
 * 
 * Detailed explanation of the example, what concepts it covers,
 * and what the user should learn from it.
 */

#include <iostream>
#include "core/Engine.h"
// ... other includes

int main() {
    std::cout << "=== Example: [Name] ===" << std::endl;
    
    // Your example code here
    
    std::cout << "Example completed successfully!" << std::endl;
    return 0;
}
```

## Learning Path

Recommended order for learning:

1. **Start Here**: `basic_setup.cpp`
2. **Voxel Basics**: `creating_chunks.cpp` → `voxel_interaction.cpp`
3. **World Generation**: `terrain_generation.cpp`
4. **Player Control**: `player_controller.cpp` → `camera_system.cpp`
5. **World Editing**: `terraforming_tools.cpp` → `custom_structures.cpp`
6. **Advanced**: `loading_assets.cpp` → `collision_detection.cpp`

## Additional Resources

- [Getting Started Guide](../GETTING_STARTED.md)
- [Architecture Documentation](../ARCHITECTURE.md)
- [API Reference](../docs/) (coming soon)
- [Video Tutorials](https://example.com) (coming soon)

## Support

If you have questions about examples:
- Open an issue with the `question` label
- Check existing discussions
- Refer to the main documentation
