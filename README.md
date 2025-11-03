# Fresh Voxel Engine

A modern voxel-based game engine built with C++ and Vulkan, featuring procedural terrain generation, AI systems, and an integrated world editor.

## Features

### Core Systems (Phase 1-2)
- ✅ CMake-based build system
- ✅ Vulkan rendering pipeline with modern graphics
- ✅ Window management with GLFW
- ✅ Shader system with hot-reloading support

### Voxel Engine (Phase 3)
- ✅ Chunk-based world management (16x256x16 chunks)
- ✅ Efficient mesh generation with face culling
- ✅ Multiple voxel types (Air, Stone, Dirt, Grass, Sand, Water, etc.)
- ✅ Greedy meshing algorithm for optimization

### Procedural Generation (Phase 4)
- ✅ Perlin noise implementation
- ✅ Fractal/octave noise for natural terrain
- ✅ Biome-based terrain generation
- ✅ Cave generation using 3D noise
- ✅ Chunk streaming system

### Additional Systems (Phase 5-7)
- ✅ Physics system foundation
- ✅ Raycasting for voxel interaction
- ✅ AI system architecture (Behavior trees)
- ✅ Editor GUI framework (ImGui integration ready)
- ✅ World serialization system (Binary format with compression)
- ✅ **Main Menu System** - Create new worlds or load existing ones
- ✅ **Terraforming System** - Comprehensive world editing tools

## Terraforming Features

The engine includes a powerful terraforming system with:

### Tools
- **Single Block** - Precise single-block placement
- **Brush** - Natural terrain sculpting with spherical brush
- **Sphere/Filled Sphere** - Create spherical structures
- **Cube/Filled Cube** - Create cubic structures
- **Line** - Draw straight lines between points
- **Flatten** - Level terrain to specific height
- **Smooth** - Smooth terrain by averaging heights
- **Paint** - Change block types without geometry changes

### Editing Modes
- **Place** - Add blocks to the world
- **Remove** - Remove blocks from the world
- **Replace** - Replace existing block types

### Features
- **Undo/Redo** - Up to 100 operations in history
- **Adjustable Tool Size** - Scale tools from 1-10 blocks
- **Multiple Voxel Types** - Stone, Dirt, Grass, Sand, Water, Wood, Leaves
- **Chunk-Aware** - Automatically handles cross-chunk editing

See [docs/TERRAFORMING.md](docs/TERRAFORMING.md) for detailed documentation.

## Building

### Prerequisites

- CMake 3.20 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Vulkan SDK 1.2 or higher
- GLFW 3.3 or higher (will be downloaded if not found)
- GLM (optional, header-only)

### Linux/macOS

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake g++ libvulkan-dev libglfw3-dev

# Install Vulkan SDK if not available via package manager
# Download from https://vulkan.lunarg.com/

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./FreshVoxelEngine
```

### Windows

```bash
# Install Vulkan SDK from https://vulkan.lunarg.com/
# Install CMake from https://cmake.org/

# Build with Visual Studio
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run
Release\FreshVoxelEngine.exe
```

## Project Structure

```
fresh/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── core/              # Engine core (Engine, Window)
│   ├── renderer/          # Vulkan rendering system
│   ├── voxel/             # Voxel data structures and world
│   ├── generation/        # Procedural generation (noise, terrain)
│   ├── physics/           # Physics simulation
│   ├── interaction/       # Player interaction (raycasting)
│   ├── ai/                # AI system (behavior trees)
│   ├── editor/            # World editor GUI & Terraforming
│   ├── ui/                # Main menu and UI systems
│   └── serialization/     # Save/load system
├── src/                   # Implementation files
│   └── (mirrors include structure)
├── shaders/               # GLSL shaders
│   ├── voxel.vert        # Vertex shader
│   └── voxel.frag        # Fragment shader
├── docs/                  # Documentation
│   └── TERRAFORMING.md   # Terraforming system guide
└── README.md             # This file
```

## Development Roadmap

### Completed (Phase 1-4)
- [x] Project structure and build system
- [x] Vulkan renderer foundation
- [x] Voxel chunk system
- [x] Procedural terrain generation
- [x] Noise functions (Perlin, fractal)
- [x] Basic mesh generation

### In Progress (Phase 5-6)
- [ ] Complete Vulkan rendering pipeline
- [ ] Camera and player controller
- [ ] Physics collision detection
- [ ] Block placement/destruction
- [ ] AI NPC behavior
- [ ] ImGui integration

### Future (Phase 7-8)
- [ ] Advanced lighting and shadows
- [ ] Water rendering with transparency
- [ ] Advanced shader effects
- [ ] Multiplayer support
- [ ] Advanced AI behaviors
- [ ] Performance optimizations
- [ ] Comprehensive testing

## Architecture

### Vulkan Renderer
The engine uses Vulkan for high-performance graphics rendering. The renderer supports:
- Swapchain management
- Command buffer recording
- Synchronization primitives
- Flexible shader pipeline

### Voxel World
The world is divided into chunks for efficient rendering and streaming:
- Each chunk is 16x256x16 voxels
- Chunks are generated procedurally using noise
- Mesh generation only creates faces between different block types
- Support for transparent and opaque voxels

### Procedural Generation
Terrain is generated using multi-octave Perlin noise:
- Height maps for surface terrain
- 3D noise for cave systems
- Biome-based block type selection
- Configurable seed for reproducible worlds

## Main Menu

When starting the engine, you'll be greeted with a main menu offering:

1. **Create New World** - Generate a fresh world with custom name and seed
   - Enter world name
   - Optionally specify seed (or use random)
   - Generates terrain with procedural generation
   
2. **Load Existing World** - Browse and load saved worlds
   - Lists all available world saves
   - Load any previously saved world for continued editing

3. **Settings** - Configure engine settings (coming soon)

All worlds are saved in the `saves/` directory as `.world` files.

## Terraforming (World Editing)

Once in a world, use the terraforming system to modify terrain:

### Quick Start
```cpp
// In your application code:
TerraformingSystem* terraform = worldEditor->getTerraformingSystem();

// Place a sphere of stone
terraform->setTool(TerraformTool::FilledSphere);
terraform->setVoxelType(VoxelType::Stone);
terraform->setToolSize(5);
terraform->applyTool(WorldPos(0, 70, 0));

// Flatten an area
terraform->setTool(TerraformTool::Flatten);
terraform->setToolSize(10);
terraform->applyTool(WorldPos(10, 64, 10));

// Undo if needed
terraform->undo();
```

### Planned Controls (To be implemented)
- **1-9**: Select terraforming tool
- **Q/E**: Cycle voxel types
- **[/]**: Adjust tool size
- **Left Click**: Apply tool (place mode)
- **Right Click**: Apply tool (remove mode)
- **Ctrl+Z/Y**: Undo/Redo
- **T**: Toggle editor UI

## Controls

(To be implemented)
- WASD: Move
- Mouse: Look around
- Space: Jump
- Left Click: Break block
- Right Click: Place block
- E: Open editor
- ESC: Exit

## Contributing

This is a learning project implementing the roadmap from the initial design document. Contributions, suggestions, and feedback are welcome!

## License

[To be determined]

## Acknowledgments

- Vulkan Tutorial (https://vulkan-tutorial.com/)
- Learn OpenGL (https://learnopengl.com/)
- Minecraft for voxel inspiration
- Open-source voxel engine projects
