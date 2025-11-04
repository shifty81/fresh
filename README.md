# Fresh Voxel Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![.NET 9](https://img.shields.io/badge/.NET-9.0-512BD4.svg)](https://dotnet.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)](https://cmake.org/)
[![Graphics](https://img.shields.io/badge/Graphics-DirectX%2011%2F12%2FOpenGL-blue.svg)](https://github.com/shifty81/fresh)
[![Cross-Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com/shifty81/fresh)
[![VS2022](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91.svg)](https://visualstudio.microsoft.com/)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

A modern **cross-platform** voxel-based game engine built with **C++20**, featuring **DirectX 11/12** and **OpenGL 4.5+** rendering, **.NET 9 bindings**, procedural terrain generation, AI systems, and an integrated world editor.

**Platform Support:** Windows, Linux, macOS  
**Graphics APIs:** DirectX 11/12 (Windows), OpenGL 4.5+ (All Platforms)

> 📚 **New to Fresh?** See [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) for a guide to all documentation.

---

## 🎨 Graphics API Support

Fresh Voxel Engine automatically selects the best graphics API for your platform:

### Windows
- **DirectX 12** - Native, cutting-edge performance (default)
- **DirectX 11** - Excellent compatibility across Windows versions
- **OpenGL 4.5+** - Optional cross-platform rendering

### Linux
- **OpenGL 4.5+** - Native support with modern drivers
- **Vulkan** - Planned for future release

### macOS
- **OpenGL 4.5+** - Currently supported (deprecated by Apple)
- **Metal** - Planned for future release

The engine **automatically detects and selects** the best available graphics API at runtime.

---

## 🚀 Quick Start

### 📖 New User? Start Here!

**Complete installation in 3 steps:**

1. **Clone the repository**
   ```batch
   git clone https://github.com/shifty81/fresh.git
   cd fresh
   ```

2. **Run the installer**
   ```batch
   install.bat
   ```
   The installer will automatically:
   - Check prerequisites (CMake, Visual Studio 2022)
   - Install vcpkg package manager and dependencies
   - Generate Visual Studio 2022 solution
   - Build the engine
   
   *Estimated time: 10-20 minutes*

3. **Open and run**
   - Open `build\FreshVoxelEngine.sln` in Visual Studio 2022
   - Right-click "FreshVoxelEngine" project → "Set as Startup Project"
   - Press F5 to build and run

**Need help?** See [QUICKSTART_VISUAL.md](QUICKSTART_VISUAL.md) for step-by-step guide with screenshots.

**Lost in documentation?** See [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) to find what you need.

### Alternative Installation Methods

<details>
<summary><b>Option 2: Build All (Includes .NET 9 support)</b></summary>

```batch
# Complete automated build with .NET 9 bindings
build_all.bat
```

For .NET 9 development, run `check_dotnet.bat` to verify .NET SDK installation.

</details>

<details>
<summary><b>Option 3: Manual Setup (Advanced users)</b></summary>

**Note**: Requires manual dependency installation. We recommend Option 1 instead.

```batch
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Generate Visual Studio solution
# This will show warnings if dependencies are missing
generate_vs2022.bat

# Open in Visual Studio
start build\FreshVoxelEngine.sln
```

If you see dependency warnings:
- Run `install.bat` to automatically install dependencies
- Or see [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) for manual installation

</details>

### Linux & macOS

<details>
<summary><b>Linux Installation</b></summary>

```bash
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential cmake libglfw3-dev libglew-dev libglm-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./FreshVoxelEngine
```

For other Linux distributions, see [CROSS_PLATFORM_BUILD.md](CROSS_PLATFORM_BUILD.md).

</details>

<details>
<summary><b>macOS Installation</b></summary>

```bash
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Install dependencies
brew install cmake glfw glew glm

# Build
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run
./FreshVoxelEngine
```

For detailed cross-platform build instructions, see [CROSS_PLATFORM_BUILD.md](CROSS_PLATFORM_BUILD.md).

</details>

---

## 🔷 .NET 9 Support (Windows Only)

Fresh Engine includes **.NET 9 bindings** for managed code development:

- **C# Game Scripting** - Write game logic in modern C# while leveraging native performance
- **P/Invoke Integration** - Seamless interop between managed and native code  
- **Modern .NET Features** - Use latest .NET 9 capabilities
- **Visual Studio Integration** - Full IntelliSense and debugging support

See [dotnet/README.md](dotnet/README.md) for details on using the .NET bindings.

---

## Features

### Integrated Game Engine Editor (NEW! 🎨)
- ✅ **Comprehensive Editor UI** - Professional ImGui-based editor interface
- ✅ **Menu Bar** - Full menu system (File, Edit, Window, Build, Settings, Help)
- ✅ **Toolbar** - Quick-access tools (Select, Move, Rotate, Scale, Place, Remove, Paint)
- ✅ **Scene Hierarchy** - Tree-structure view of all world objects
- ✅ **Inspector Panel** - Dynamic property editor with transform, component, and material editing
- ✅ **Content Browser** - Asset management with preview, search, and import
- ✅ **Console** - Message logging, filtering, and command execution
- ✅ **Voxel Tool Palette** - 10+ terraforming tools (Brush, Sphere, Cube, Flatten, Smooth, etc.)
- ✅ **Material Picker** - 13 voxel types with color-coded buttons
- ✅ **Smart Input Handling** - GUI/world input differentiation
- ✅ **Docking System** - Flexible panel layouts
- ✅ **Keyboard Shortcuts** - Full shortcut support (Ctrl+S, Ctrl+Z, tool shortcuts)

### Core Systems (Phase 1-2)
- ✅ CMake-based build system with Visual Studio 2022 support
- ✅ **DirectX 11/12 rendering** - Native Windows graphics APIs
- ✅ **.NET 9 bindings** - C# interop for managed game development
- ✅ Automatic graphics API selection (DX12 → DX11 fallback)
- ✅ Window management with GLFW
- ✅ Shader system with hot-reloading support
- ✅ **Comprehensive Logging System** - File-based error tracking and debugging

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
- ✅ **Placeholder Textures & Assets** - Ready-to-use voxel and UI textures

## Texture Assets

The engine includes procedurally generated placeholder textures for rapid prototyping:

### Block Textures (42 textures)
- **Basic Blocks**: Stone, Dirt, Sand
- **Natural Materials**: Grass (top/side), Wood, Leaves, Water
- **Crafted Blocks**: Cobblestone, Planks
- **Multiple Resolutions**: 16x16 (low), 32x32 (medium), 64x64 (high)
- **Transparency Support**: Water and leaves with alpha channel
- **Face-Specific Textures**: Grass blocks have different textures per face

### UI Assets (11 textures)
- **Gameplay UI**: Crosshair (2 sizes), cursor
- **Inventory**: Hotbar slots (normal/selected)
- **Menu Elements**: Button backgrounds (normal/hover), panels
- **HUD**: Health indicators (full/empty hearts)
- **Branding**: Logo placeholder

### Using Textures

```cpp
// Load all voxel textures
fresh::VoxelTextureLoader textureLoader(
    fresh::VoxelTextureLoader::TextureResolution::Low
);
textureLoader.loadAllTextures();

// Get texture for a block type
auto stoneTexture = textureLoader.getTexture(fresh::VoxelType::Stone);

// Face-specific textures (for grass)
auto grassTop = textureLoader.getTexture(
    fresh::VoxelType::Grass, 
    fresh::VoxelTextureLoader::BlockFace::Top
);
```

See [textures/USAGE.md](textures/USAGE.md) for complete documentation.

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
- C++20 compatible compiler (MSVC 2019 16.11+ / Visual Studio 2019+ / GCC 10+ / Clang 10+)
- **Graphics API Dependencies**:
  - **DirectX 11/12** (included with Windows SDK)
  - **OpenGL 4.5+** drivers (usually pre-installed)
- GLFW 3.3 or higher (automatically managed via vcpkg.json)
- GLM (automatically managed via vcpkg.json)
- **ImGui** (automatically managed via vcpkg.json) - for editor UI

**Note**: The project includes a `vcpkg.json` manifest file. When using vcpkg with the CMake toolchain file, dependencies (including ImGui) are automatically installed.

### Building on Windows (Visual Studio 2022)

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

## Graphics API Selection

The engine automatically selects the best graphics API for Windows:
- **Default**: DirectX 12 (best performance on Windows 10+)
- **Fallback**: DirectX 11 (excellent compatibility)
- **Legacy**: OpenGL (broadest hardware support)

To manually select a specific API, edit `src/renderer/GraphicsAPI.h` or use environment variables (feature coming soon).

## Project Structure

```
fresh/
├── CMakeLists.txt              # Build configuration
├── CMakePresets.json           # CMake presets for Visual Studio
├── vcpkg.json                  # vcpkg manifest for dependencies
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
├── textures/              # Placeholder textures (NEW!)
│   ├── blocks/           # Block/voxel textures
│   │   ├── stone_*.png   # Stone textures (16x16, 32x32, 64x64)
│   │   ├── dirt_*.png    # Dirt textures
│   │   ├── grass_*.png   # Grass textures (top/side)
│   │   ├── sand_*.png    # Sand textures
│   │   ├── water_*.png   # Water textures (transparent)
│   │   ├── wood_*.png    # Wood textures
│   │   ├── leaves_*.png  # Leaves textures (transparent)
│   │   ├── cobblestone_*.png  # Cobblestone textures
│   │   └── planks_*.png  # Wooden planks textures
│   ├── ui/               # UI element textures
│   │   ├── crosshair.png       # Aiming crosshair
│   │   ├── hotbar_*.png        # Inventory slots
│   │   ├── button_*.png        # Menu buttons
│   │   ├── health_*.png        # Health indicators
│   │   └── logo.png            # Engine logo
│   ├── README.md         # Texture documentation
│   ├── USAGE.md          # Usage guide with examples
│   └── texture_config.json  # Texture mapping configuration
├── examples/              # Example code
│   └── texture_loading_example.cpp  # Texture system demo
├── docs/                  # Documentation
│   ├── TERRAFORMING.md   # Terraforming system guide
│   └── LOGGING.md        # Logging system documentation
├── logs/                  # Application and error logs
│   ├── application_errors_<timestamp>.txt
│   └── Environment/       # Platform-specific error logs
│       └── Windows/
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
- Multiple graphics API support (OpenGL, DirectX 11/12)
- Automatic API selection (DirectX 12 by default)
- Unified interface for all backends (`IRenderContext`)
- Swapchain and framebuffer management
- Resource creation (buffers, textures, shaders)
- Synchronization and frame pacing

**API Priority on Windows:**
DirectX 12 → DirectX 11 → OpenGL

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

## Integrated Editor

Once in a world, the comprehensive game engine editor loads automatically:

### Editor Interface

**Toggle Editor**: Press `T` to show/hide the full editor interface

**Main Components:**
- **Menu Bar**: File operations, editing tools, window management, build options, settings, help
- **Toolbar**: Transform tools (Select, Move, Rotate, Scale) and editing tools (Place, Remove, Paint)
- **Scene Hierarchy**: Browse world structure (Chunks, Entities, Lights)
- **Inspector**: Edit properties of selected objects (Transform, Components, Materials)
- **Content Browser**: Manage assets (textures, models, sounds, scripts)
- **Console**: View logs, errors, and execute commands
- **Voxel Tools**: Terraforming tools with material picker

### Quick Start Editor Guide

**Transform Tools:**
- `W` - Move tool
- `E` - Rotate tool
- `R` - Scale tool

**Editing Tools:**
- `B` - Place blocks
- `X` - Remove blocks
- `P` - Paint (change block type)

**File Operations:**
- `Ctrl+N` - New World
- `Ctrl+O` - Open World
- `Ctrl+S` - Save World

**Edit Operations:**
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste

**Window Management:**
- Toggle panels via Window menu
- Drag panels to dock in different positions
- Save/load custom layouts

For complete editor documentation, see [Editor Integration Guide](docs/EDITOR_INTEGRATION.md).

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
- 🎨 [Editor Integration Guide](docs/EDITOR_INTEGRATION.md) - Comprehensive editor documentation (NEW!)
- 📋 [Editor Implementation Summary](docs/EDITOR_IMPLEMENTATION_SUMMARY.md) - Technical implementation details (NEW!)
- 📝 [Logging System](docs/LOGGING.md) - Error tracking and debugging
- 🎨 [Texture System](textures/USAGE.md) - Using placeholder textures and assets
- ⚙️ [GitHub Workflows](WORKFLOWS.md) - CI/CD and automation

## Logging and Debugging

The engine includes a comprehensive logging system that captures all errors, warnings, and information to help with debugging and development:

- **Application Logs**: Complete activity logs in `logs/application_errors_<timestamp>.txt`
- **Environment Logs**: Platform-specific error logs in `logs/Environment/<Platform>/errors_<timestamp>.txt`
- **Batch Script Logs**: Build errors automatically logged to `logs/`
- **Thread-Safe**: Safe to use from multiple threads
- **Timestamped**: Easy to correlate logs with specific runs

### Quick Example

```cpp
#include "core/Logger.h"

int main() {
    // Initialize logger
    fresh::Logger::getInstance().initialize();
    
    // Log messages
    LOG_INFO("Application started");
    LOG_ERROR("Failed to load texture");
    
    // With component names
    LOG_INFO_C("Renderer initialized", "Renderer");
    
    // Shutdown logger
    fresh::Logger::getInstance().shutdown();
}
```

For complete documentation, see [docs/LOGGING.md](docs/LOGGING.md).

The logs can be uploaded to issue trackers or shared with developers to help steer development and diagnose issues.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Learn OpenGL (https://learnopengl.com/)
- Minecraft for voxel inspiration
- Open-source voxel engine projects
