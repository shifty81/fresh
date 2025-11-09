# Fresh Voxel Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![.NET 9](https://img.shields.io/badge/.NET-9.0-512BD4.svg)](https://dotnet.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)](https://cmake.org/)
[![Graphics](https://img.shields.io/badge/Graphics-DirectX%2011%2F12-blue.svg)](https://github.com/shifty81/fresh)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://github.com/shifty81/fresh)
[![VS2022](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91.svg)](https://visualstudio.microsoft.com/)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

A modern **Windows-only** voxel-based game engine built with **C++20**, featuring **DirectX 11/12** rendering, **.NET 9 bindings**, **Lua scripting**, procedural terrain generation, AI systems, and an integrated world editor.

**Platform Support:** Windows 10/11 (x64)  
**Graphics APIs:** DirectX 11/12

> 📚 **New to Fresh?** See [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) for a guide to all documentation.

---

## 🎨 Graphics API Support

Fresh Voxel Engine uses DirectX for high-performance Windows-native rendering:

### Windows
- **DirectX 12** - Cutting-edge performance for Windows 10/11 (default)
- **DirectX 11** - Excellent compatibility across Windows versions

The engine **automatically detects and selects** the best available DirectX version at runtime.

---

## 🎮 How to Play

The Fresh Voxel Engine now includes a **playable demo** with basic voxel world exploration and building!

### Quick Start

1. **Build and Run** (see build instructions below)
2. **Main Menu** - Choose "Create New World" or "Load Existing World"
3. **Enter World** - The engine generates a procedurally generated voxel terrain
4. **Explore and Build!**

### Controls

- **WASD** - Move around (standard FPS controls)
- **Mouse** - Look around
- **Space** - Jump
- **Shift** - Sprint (move faster)
- **Ctrl** - Crouch
- **Left Click** - Break/remove blocks
- **Right Click** - Place blocks (Stone by default)
- **ESC** - Exit game

**Full Controls:** See [CONTROLS.md](CONTROLS.md) for complete control documentation.

### Features in Playable Demo

✅ **Procedural World Generation** - Infinite terrain with hills, valleys, caves  
✅ **First-Person Movement** - Smooth WASD + mouse controls with physics  
✅ **Block Interaction** - Place and break blocks with mouse  
✅ **Collision Detection** - Walk on terrain, can't pass through blocks  
✅ **Chunk Streaming** - World loads around you as you explore  
✅ **Crosshair** - Visual aim indicator for building  

---

## 🚀 Quick Start

### 📖 Building on Windows with Visual Studio 2022

**See [BUILD.md](BUILD.md) for complete step-by-step build instructions.**

**Quick build from scratch:**

```batch
# 1. Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# 2. Set up vcpkg for dependencies (one-time setup)
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..

# 3. Generate Visual Studio 2022 solution
generate_vs2022.bat

# 4. Build
cd build
cmake --build . --config Release

# 5. Run
Release\FreshVoxelEngine.exe
```

**Or use Visual Studio GUI:**
1. Run `generate_vs2022.bat`
2. Open `build\FreshVoxelEngine.sln` in Visual Studio 2022
3. Set **FreshVoxelEngine** as startup project (right-click → "Set as Startup Project")
4. Press **F7** to build, **F5** to run

**Prerequisites:**
- Visual Studio 2022 with "Desktop development with C++"
- CMake 3.20+
- Git for Windows
- .NET 9 SDK (optional, for C# bindings)

**Detailed instructions:** [BUILD.md](BUILD.md) provides complete step-by-step guidance including troubleshooting.


## 🔷 .NET 9 Support (Optional C# Bindings)

Fresh Engine provides optional **.NET 9 C# bindings** for managed code development:

- **C# Game Scripting** - Write game logic in modern C# while leveraging native performance
- **P/Invoke Integration** - Seamless interop between managed and native code  
- **Modern .NET Features** - Use latest .NET 9 capabilities
- **Visual Studio Integration** - Full IntelliSense and debugging support

### Building .NET Bindings

If you want C# scripting support:

```batch
# From repository root
cd dotnet
dotnet build -c Release
cd ..
```

The managed assembly will be located at: `dotnet\bin\Release\net9.0-windows\FreshEngine.Managed.dll`

See [dotnet/README.md](dotnet/README.md) for detailed C# usage examples and API documentation.

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

**For complete step-by-step build instructions, see [BUILD.md](BUILD.md).**

### Prerequisites

- **Visual Studio 2022** with "Desktop development with C++" workload
- **CMake 3.20+** (add to PATH during installation)
- **Git for Windows**
- **.NET 9 SDK** (optional, for C# bindings only)

### Quick Build

```batch
# See BUILD.md for detailed instructions
generate_vs2022.bat
```

Dependencies (GLFW, GLM, ImGui, GLEW) are automatically installed via vcpkg during CMake configuration.

### Building on Windows (Visual Studio 2022)

See [BUILD.md](BUILD.md) for complete instructions including:
- vcpkg setup
- CMake configuration
- Visual Studio usage
- Command-line build
- Troubleshooting

## Graphics API Selection

The engine automatically selects the best DirectX version for Windows:
- **Default**: DirectX 12 (best performance on Windows 10+)
- **Fallback**: DirectX 11 (excellent compatibility)

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
│   ├── renderer/              # DirectX rendering system
│   │   ├── backends/          # Graphics API implementations
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
├── shaders/               # HLSL shaders
│   ├── voxel.hlsl        # DirectX shader
│   └── voxel.vert/frag  # Legacy GLSL shaders (if needed)
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

### Completed (Phase 1-5)
- [x] Project structure and build system
- [x] **DirectX 11/12 rendering system**
- [x] **Visual Studio 2022 support**
- [x] Voxel chunk system
- [x] Procedural terrain generation
- [x] Noise functions (Perlin, fractal)
- [x] Basic mesh generation
- [x] Player controller and camera system
- [x] Enhanced input system with action mapping

### In Progress (Phase 6)
- [x] DirectX 11 backend (95% complete - production ready)
- [x] DirectX 12 backend (85% complete - core functionality complete)

### Starting (Phase 7)
- [ ] Procedural voxel character system
- [ ] Character generation and customization
- [ ] Animation system
- [ ] IK (Inverse Kinematics) system
- [ ] Dynamic weapon/tool handling

### Future (Phase 8+)
- [ ] Advanced terrain generation (3D density, caves, overhangs)
- [ ] Enhanced biome system
- [ ] Advanced lighting and shadows
- [ ] Water rendering with transparency
- [ ] Multiplayer support
- [ ] Performance optimizations
- [ ] Comprehensive testing

## Architecture

### DirectX Rendering System
The engine uses DirectX for native Windows performance:

**Supported APIs:**
- **DirectX 11** - High-level API with excellent Windows compatibility
- **DirectX 12** - Low-level API for maximum performance on Windows 10/11

**Features:**
- DirectX 11/12 support with automatic version selection
- DirectX 12 by default with DirectX 11 fallback
- Unified interface for both backends (`IRenderContext`)
- Swapchain and framebuffer management
- Resource creation (buffers, textures, shaders)
- Synchronization and frame pacing

**API Priority:**
DirectX 12 → DirectX 11

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
- **Hold Alt**: Temporarily show cursor for UI interaction (in-game)

## Controls

The engine features an advanced input system with multiple modes and seamless mode switching:

### Standard Controls (Game Mode)
- **WASD**: Move
- **Mouse**: Look around (camera control)
- **Space**: Jump
- **Left Shift**: Sprint
- **Left Ctrl**: Crouch
- **Left Click**: Break block
- **Right Click**: Place block
- **E**: Use/Interact
- **T**: Toggle editor
- **ESC**: Open menu / Exit

### Advanced Input System
- **Hold Alt**: Temporarily switch to UI mode - cursor becomes visible and you can interact with UI elements while still in-game
- **Input Modes**: GameMode (default), UIMode (menus/editor), BuildMode (ship builder)
- **Automatic Mode Switching**: Engine automatically switches modes when opening menus or editor

For complete input system documentation, see [docs/INPUT_SYSTEM.md](docs/INPUT_SYSTEM.md).

---

## ⚠️ Known Limitations

### Graphics APIs
- **DirectX 11** - Production ready (95% complete) ✅
  - All core rendering functionality implemented
  - HLSL shader compilation working
  - Ready for Windows deployment
- **DirectX 12** - Core functionality complete (85% complete) ✅
  - All rendering operations implemented
  - Resource state transitions working
  - Advanced shader features (PSO) pending
  - Texture data upload pending

### Platform Support
- **Windows 10/11** - Primary platform ✅
  - DirectX 11 backend production ready
  - DirectX 12 backend functional
  - Full feature support

### Features
- **ImGui Integration** - Not included by default
  - Editor UI limited to console mode
  - **Installation:** `vcpkg install imgui[glfw-binding,dx11-binding,dx12-binding]:x64-windows`
- **Lua Scripting** - Optional feature, framework exists but incomplete (~40%)
  - **Engine builds and runs without Lua** - Lua-dependent features use stub implementations
  - Need Sol2 library integration for full functionality
  - **Installation:** `vcpkg install luajit:x64-windows` or `vcpkg install lua:x64-windows`
  - **Setup:** Clone sol2 to `external/sol2/` for advanced features
- **Audio** - Only WAV files supported
  - OGG/Vorbis support planned
  - OpenAL integration complete ✅
- **Networking** - Stub implementation only (~10%)
  - Multiplayer not yet functional

### Testing
- **Code Coverage** - ~30% (target: 50%+)
- **Platform Testing** - Windows-focused
  - DirectX 11/12 backends ready for testing

### Performance
- **Memory Usage** - Not yet optimized for large worlds
- **Shader Hot-Reload** - Needs improvement

For the most up-to-date status, see [THIS_STATUS.md](THIS_STATUS.md).

---

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
- 🛠️ [Development Tools](docs/DEVELOPMENT_TOOLS.md) - Code formatting, documentation generation, and release process (NEW!)
- 📚 [Tutorials](docs/tutorials/) - Step-by-step guides
- 🔨 [Examples](examples/) - Sample code and usage
- 🌍 [Terraforming Guide](docs/TERRAFORMING.md) - World editing system
- 🎨 [Editor Integration Guide](docs/EDITOR_INTEGRATION.md) - Comprehensive editor documentation
- 📋 [Editor Implementation Summary](docs/EDITOR_IMPLEMENTATION_SUMMARY.md) - Technical implementation details
- 🎮 [Input System Guide](docs/INPUT_SYSTEM.md) - Input modes, Hold Alt feature, and controls
- 📝 [Logging System](docs/LOGGING.md) - Error tracking and debugging
- 🎨 [Texture System](textures/USAGE.md) - Using placeholder textures and assets
- ⚙️ [GitHub Workflows](WORKFLOWS.md) - CI/CD and automation

---

## Known Limitations

While Fresh Voxel Engine has a solid foundation, there are some known limitations to be aware of:

### Graphics APIs
- **DirectX 11**: Production ready (95% complete) - all core functionality implemented
- **DirectX 12**: Core functionality complete (85% complete) - advanced features pending (PSO, texture upload)

### Systems in Development
- **Lua Scripting**: Framework exists (~40% complete) but needs Sol2 integration for full functionality
- **Networking**: Framework only (~10% complete) - multiplayer features not yet implemented
- **AI System**: Basic framework (~50% complete) - behavior trees need further development

### Testing
- **Test Coverage**: Good coverage for core systems (156 tests passing)
- **Platform Testing**: Windows-focused with DirectX testing

### Recommended for Production
For production use, we recommend:
- Using **DirectX 11 backend** on Windows (production ready, excellent performance)
- Using **DirectX 12 backend** for cutting-edge performance (85% complete)
- Focusing on **single-player experiences** (networking not ready)
- Using **built-in systems** rather than Lua scripting (until Sol2 integration complete)

For accurate, code-verified status of all systems, see [ACTUAL_STATUS.md](ACTUAL_STATUS.md).

---

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
