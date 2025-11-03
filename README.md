# Fresh Voxel Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)](https://cmake.org/)
[![Graphics](https://img.shields.io/badge/Graphics-OpenGL%20|%20DirectX-blue.svg)](https://github.com/shifty81/fresh)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

A modern voxel-based game engine built with C++17, featuring **multiple graphics API support** (OpenGL, DirectX 11/12), procedural terrain generation, AI systems, and an integrated world editor.

---

## 🎨 Graphics API Support

Fresh Voxel Engine now supports multiple graphics APIs:

- **OpenGL 4.5+** - Cross-platform, mature ecosystem
- **DirectX 11** - Windows, excellent compatibility
- **DirectX 12** - Windows 10+, cutting-edge performance

The engine **automatically selects the best API** for your platform, or you can manually choose your preferred API.

---

## 🚀 Quick Start

### Windows (Visual Studio 2022)

#### Option 1: Automated Installation (Recommended)

```batch
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated installer (handles everything!)
tools\build_tools\install.bat
```

The automated installer will:
- ✅ Check prerequisites (CMake, Visual Studio)
- ✅ Install vcpkg package manager (optional)
- ✅ Install dependencies (GLFW, GLM)
- ✅ Generate Visual Studio solution
- ✅ Build the project automatically
- ✅ Create shortcuts for easy access

See [tools/build_tools/README.md](tools/build_tools/README.md) for detailed installation options.

#### Option 2: Manual Setup

```batch
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Generate Visual Studio solution
generate_vs2022.bat

# Open in Visual Studio
start build\FreshVoxelEngine.sln
```

See [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) for detailed Visual Studio instructions.

### Linux/macOS

```bash
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake g++ libglfw3-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./FreshVoxelEngine
```

See [GETTING_STARTED.md](GETTING_STARTED.md) for detailed setup instructions.

## Features

### Core Systems (Phase 1-2)
- ✅ CMake-based build system with Visual Studio 2022 support
- ✅ **Multi-API rendering**: OpenGL, DirectX 11/12
- ✅ Automatic graphics API selection
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
- **Graphics API Dependencies** (at least one):
  - **OpenGL 4.5+** drivers (usually pre-installed)
  - **DirectX 11/12** (Windows only, included with Windows SDK)
- GLFW 3.3 or higher (will be downloaded if not found)
- GLM (optional, header-only)

### Windows (Visual Studio 2022)

#### Automated Installation (Easiest)

```batch
# Run the automated installer
tools\build_tools\install.bat
```

This handles all setup automatically! See [tools/build_tools/README.md](tools/build_tools/README.md) for details.

#### Manual Installation

```batch
# Generate Visual Studio solution
generate_vs2022.bat

# Open solution in Visual Studio
start build\FreshVoxelEngine.sln

# Or build from command line
cmake --build build --config Release
```

**Detailed instructions**: See [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)

### Linux/macOS

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake g++ libglfw3-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./FreshVoxelEngine
```

## Graphics API Selection

The engine automatically selects the best graphics API for your platform:
- **Windows**: DirectX 12 → DirectX 11 → OpenGL
- **Linux**: OpenGL
- **macOS**: OpenGL

To manually select a specific API, edit `src/renderer/GraphicsAPI.h` or use environment variables (feature coming soon).

## Project Structure

```
fresh/
├── CMakeLists.txt              # Build configuration
├── CMakePresets.json           # CMake presets for Visual Studio
├── generate_vs2022.bat         # Visual Studio project generator
├── VISUAL_STUDIO_SETUP.md      # Visual Studio setup guide
├── include/                    # Header files
│   ├── core/                  # Engine core (Engine, Window)
│   ├── renderer/              # Multi-API rendering system
│   │   ├── backends/          # Graphics API implementations
│   │   │   ├── OpenGLRenderContext.h
│   │   │   ├── DirectX11RenderContext.h
│   │   │   └── DirectX12RenderContext.h
│   │   ├── GraphicsAPI.h      # API enumeration and selection
│   │   └── RenderContext.h    # Abstract rendering interface
│   ├── voxel/                 # Voxel data structures and world
│   ├── generation/            # Procedural generation (noise, terrain)
│   ├── physics/               # Physics simulation
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
- [x] **Multi-API rendering system** (OpenGL, DirectX 11/12)
- [x] **Visual Studio 2022 support**
- [x] Voxel chunk system
- [x] Procedural terrain generation
- [x] Noise functions (Perlin, fractal)
- [x] Basic mesh generation

### In Progress (Phase 5-6)
- [ ] Complete rendering pipeline for all APIs
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

### Multi-API Rendering System
The engine uses an abstraction layer supporting multiple graphics APIs:

**Supported APIs:**
- **OpenGL 4.5+** - Mature, widely supported cross-platform API
- **DirectX 11** - High-level API with excellent Windows compatibility
- **DirectX 12** - Low-level API for maximum performance on Windows

**Features:**
- Automatic API selection based on platform
- Unified interface for all backends (`IRenderContext`)
- Swapchain and framebuffer management
- Resource creation (buffers, textures, shaders)
- Synchronization and frame pacing

**API Priority:**
- Windows: DirectX 12 → DirectX 11 → OpenGL
- Linux: OpenGL
- macOS: OpenGL

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

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Ways to Contribute

- 🐛 Report bugs via [Issues](https://github.com/shifty81/fresh/issues)
- ✨ Suggest features via [Issues](https://github.com/shifty81/fresh/issues)
- 📝 Improve documentation
- 🔧 Submit pull requests
- 💬 Help others in [Discussions](https://github.com/shifty81/fresh/discussions)

Please read our [Code of Conduct](CODE_OF_CONDUCT.md) before contributing.

## Documentation

- 📖 [Getting Started Guide](GETTING_STARTED.md) - Setup and first steps
- 🏗️ [Architecture Overview](ARCHITECTURE.md) - System design and patterns
- 🗺️ [Development Roadmap](ROADMAP.md) - Future plans and progress
- 📚 [Tutorials](docs/tutorials/) - Step-by-step guides
- 🔨 [Examples](examples/) - Sample code and usage
- 🌍 [Terraforming Guide](docs/TERRAFORMING.md) - World editing system
- ⚙️ [GitHub Workflows](WORKFLOWS.md) - CI/CD and automation

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Learn OpenGL (https://learnopengl.com/)
- Minecraft for voxel inspiration
- Open-source voxel engine projects
