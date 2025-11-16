# Fresh Voxel Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![.NET 9](https://img.shields.io/badge/.NET-9.0-512BD4.svg)](https://dotnet.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-0078D4.svg)](https://github.com/shifty81/fresh)
[![Graphics](https://img.shields.io/badge/Graphics-DirectX%2012%2F11-00B2FF.svg)](https://github.com/shifty81/fresh)
[![Editor](https://img.shields.io/badge/Editor-Unreal--like-orange.svg)](https://github.com/shifty81/fresh)

A professional **Windows-exclusive** voxel game development platform built with **C++20**, featuring **DirectX 12/11** rendering, an **Unreal Engine-like editor**, **.NET 9 bindings**, **Lua scripting**, procedural terrain generation, and comprehensive Windows-native integration for game development.

> 📚 **New to Fresh?** See [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) for complete documentation guide.
> 🎮 **Windows Gaming Platform** - Exclusive focus on Windows 10/11 with DirectX and professional game development tools.

---

## ✨ Features

### Professional Game Development Platform
- **Unreal Engine-Like Editor** - Professional ImGui-based editor with comprehensive game development tools
- **Windows-Native GUI** - Fully native Win32 windowing system (optional, replaces GLFW for true Windows-native experience)
- **Windows-Native Integration** - Full Windows 10/11 themes, native dialogs, jump lists, toast notifications, and taskbar features
- **High DPI Support** - Per-monitor DPI awareness for crisp rendering on 4K/5K displays
- **AVX2 Optimized** - SIMD instructions for 15-30% performance improvement on modern CPUs
- **DirectX 12 Primary Renderer** - Cutting-edge graphics with DirectX 12 (Windows 10/11)
- **DirectX 11 Fallback** - Excellent compatibility for wider hardware support
- **Visual Studio 2022 Optimized** - Seamless integration with Microsoft's flagship IDE
- **OpenGL Optional Support** - For compatibility and testing purposes

### Core Engine
- **C++20** - Modern C++ with strong type safety and performance
- **.NET 9 Bindings** - Optional C# scripting support for rapid development
- **Lua Scripting** - Optional scripting engine for gameplay and modding
- **CMake Build System** - Professional build configuration with Visual Studio 2022 focus
- **Windows SDK Integration** - Full access to Windows platform capabilities
- **Compiler Optimizations** - Whole program optimization, link-time code generation, fast floating-point

### Voxel World
- **Chunk-Based World** - Efficient 16×256×16 chunk system
- **Procedural Generation** - Perlin noise terrain with biomes and caves
- **13+ Voxel Types** - Stone, Dirt, Grass, Sand, Water, Wood, and more
- **Dynamic Streaming** - World loads around player as they explore
- **Greedy Meshing** - Optimized rendering with face culling

### Editor & Tools (Unreal Engine-Like Experience)
- **Professional Game Editor** - Comprehensive ImGui-based editor for complete game development
- **Windows Native Integration** - Native Windows 10/11 themes, dialogs, and taskbar features
- **Terraforming Tools** - Brush, sphere, cube, flatten, smooth, paint for world building
- **NPC Creation System** - Easy-to-use interface for adding NPCs (villagers, traders, enemies)
  - Visual NPC placement in world editor
  - Dropdown behavior selection (Friendly, Trader, Hostile, Patrol, Guard, etc.)
  - Configure AI behaviors without coding
  - Use existing character, AI, and behavior tree systems
- **Scene Hierarchy** - Visual world structure and entity browser
- **Content Browser** - Complete asset management system
- **Entity Inspector** - Edit properties of selected NPCs and objects
- **Undo/Redo** - Up to 100 operations in history
- **Hot-Reload** - Shader and script changes update in real-time
- **Play-in-Editor** - Test gameplay instantly without leaving editor

### Gameplay & NPC Systems
- **First-Person Controller** - Smooth WASD movement with physics
- **NPC System** - Villagers, wandering traders, enemies with AI behaviors
  - Behavior Tree AI for intelligent NPC actions
  - Easy editor integration for non-programmers
  - Configurable behaviors: Friendly, Trader, Hostile, Patrol, Guard, Wander
- **Block Interaction** - Place and remove blocks with mouse
- **Collision Detection** - Physics-based player and NPC movement
- **Camera System** - Free-look FPS camera
- **Main Menu** - Create/load worlds with save system

### Additional Systems
- **Audio Engine** - OpenAL-based audio with WAV support
- **Asset Management** - Modular asset system with texture loading
- **Physics System** - Gravity, collision, and raycasting for players and NPCs
- **AI Framework** - Behavior tree system for NPC intelligence
- **Character System** - Voxel character rendering with IK and animation
- **Networking** - Framework in place for multiplayer (stub implementation)

---

## 🚀 Quick Start

### Prerequisites

**Windows 10/11 Only** - Fresh Voxel Engine is exclusively designed for Windows gaming and development.

- **Windows 10/11** (x64) - Required
- **Visual Studio 2022** with "Desktop development with C++" - Required
- **CMake 3.20+** - Required
- **Git for Windows** - Required
- **.NET 9 SDK** (optional, for C# scripting bindings)
- **Windows 10 SDK** (included with Visual Studio) - For DirectX support

### Build and Run
- **Git**
- Development libraries: `glfw glm openal-soft` (via Homebrew)

### Build and Run

#### Automated Build (Recommended)

```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated setup script (installs dependencies, builds project)
.\setup-and-build.ps1
```

This script handles everything: vcpkg setup, dependency installation, CMake generation, and building (10-20 minutes first run).

**See [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md) for detailed options.**

#### Manual Build

```batch
# 1. Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# 2. Set up vcpkg (one-time setup)
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..

# 3. Generate Visual Studio solution
generate_vs2022.bat

# 4. Build
cd build
cmake --build . --config Release

# 5. Run
Release\FreshVoxelEngine.exe
```

**For complete step-by-step instructions, see [BUILD.md](BUILD.md).**

---
git clone https://github.com/shifty81/fresh.git
cd fresh

# 3. Build
mkdir build && cd build
cmake ..
cmake --build . --config Release -j$(sysctl -n hw.ncpu)

# 4. Run
./FreshVoxelEngine
```

**For complete step-by-step instructions, see [BUILD.md](BUILD.md).**

---

## 🎮 How to Play

### Controls

**Mouse Controls:**
- **Mouse Movement** - Look around (first-person camera with improved sensitivity)
- **Left Click** - Break blocks
- **Right Click** - Place blocks
- **Editor UI** - Click, drag, and interact with all editor panels

**Editor Mode (Default - Free-Flying Camera):**
- **WASD** - Move in camera direction
- **Space** - Fly up (ascend)
- **Ctrl** - Fly down (descend)
- **Shift** - Move faster
- **No gravity or collision** - Full 3D freedom
- **Click NPCs** - Select and edit NPC properties in inspector

**Play Mode (Via Editor Toolbar):**
- **WASD** - Move (FPS-style)
- **Space** - Jump
- **Shift** - Sprint
- **Ctrl** - Crouch (prevents falling off edges!)
- **Physics enabled** - Gravity and collision active
- **Interact with NPCs** - Talk to villagers, trade with merchants, combat enemies

**See [CONTROLS.md](CONTROLS.md) for complete controls reference.**

### Getting Started with the Editor

1. **Launch** - Run FreshVoxelEngine.exe
2. **Main Menu** - Choose "Create New World" or "Load Existing World"
3. **Explore** - Fly around in free-camera mode to explore procedurally generated terrain
4. **Build** - Place and remove blocks with mouse clicks
5. **Add NPCs** - Use the Entity menu to spawn villagers, traders, or enemies
6. **Configure NPCs** - Select an NPC and use the Inspector panel to:
   - Choose behavior type (Friendly, Trader, Hostile, Patrol, Guard, Wander)
   - Set patrol routes or guard positions
   - Configure trading inventory (for traders)
   - Set hostility and combat parameters (for enemies)
7. **Edit** - Use the always-visible editor toolbar and panels for advanced features
8. **Test** - Click **Play** in the editor toolbar to test gameplay with normal physics and active NPCs

**See [examples/README.md](examples/README.md) for 6 playable in-engine demo scenarios.**

**Try the Showcase Demo**: Create a world named "ShowcaseDemo" with seed **12345** to see all engine capabilities! See [examples/DEMO_SHOWCASE.md](examples/DEMO_SHOWCASE.md) for the complete guided demo.

---

## 🎨 Development Languages & Tools

Fresh Voxel Engine supports multiple development approaches for Windows:

### Primary: C++20 Core Engine
- **Modern C++ features**: Concepts, ranges, coroutines
- **High performance**: Direct hardware access
- **Full engine control**: All features available
- **Build with**: CMake + Visual Studio 2022

### Optional: .NET 9 C# Bindings
- **Managed development**: Memory safety, easier debugging
- **Rapid iteration**: Hot reload, faster compile times
- **Rich ecosystem**: NuGet packages, LINQ, async/await
- **Interop**: P/Invoke to native engine
- **See**: [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md)

### Optional: Lua Scripting
- **Gameplay modding**: Custom behaviors and logic
- **Hot reload**: Change scripts without recompiling
- **Easy learning**: Simple syntax for non-programmers
- **Event system**: Hook into engine events
- **See**: [LUA_INTEGRATION_GUIDE.md](LUA_INTEGRATION_GUIDE.md)

### Tools: Python 3.8+
- **Asset pipeline**: Texture atlas generation
- **Build verification**: Environment checking
- **World analysis**: Debug saved worlds
- **Cross-platform**: Works on Windows, Linux, macOS
- **See**: [tools/README.md](tools/README.md)

**Choose your language based on your needs:**
- Performance-critical code → C++20
- Game logic & rapid development → .NET 9 C#
- Modding & scripting → Lua
- Tools & automation → Python

---

## 📁 Project Structure

```
fresh/
├── CMakeLists.txt          # Build configuration
├── vcpkg.json              # Dependency manifest
├── generate_vs2022.bat     # Visual Studio generator
├── setup-and-build.ps1     # Automated setup script
│
├── include/                # Header files
│   ├── core/              # Engine core (Engine, Logger, Window)
│   ├── renderer/          # DirectX 11/12 rendering
│   ├── voxel/             # Voxel world and chunks
│   ├── generation/        # Procedural terrain generation
│   ├── physics/           # Physics and collision
│   ├── editor/            # World editor and terraforming
│   ├── ui/                # ImGui editor UI
│   ├── gameplay/          # Player and camera
│   ├── audio/             # Audio engine
│   ├── character/         # Voxel character system
│   ├── assets/            # Asset management
│   └── ...                # Other systems
│
├── src/                   # Implementation files
│   └── (mirrors include/ structure)
│
├── shaders/               # HLSL shaders for DirectX
├── textures/              # Placeholder textures (42 blocks + 11 UI)
├── sounds/                # Audio files
├── Assets/                # Game assets
├── examples/              # In-engine gameplay demo documentation
├── docs/                  # Detailed documentation
├── dotnet/                # .NET 9 C# bindings
├── tests/                 # Unit tests
└── tools/                 # Python development tools
```

---

## 📖 Documentation

### Getting Started
- [README.md](README.md) - This file
- [GETTING_STARTED.md](GETTING_STARTED.md) - Detailed setup guide
- [BUILD.md](BUILD.md) - Complete build instructions
- [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md) - Automated setup documentation

### Features & Systems
- [CONTROLS.md](CONTROLS.md) - Complete control reference
- [docs/EDITOR_INTEGRATION.md](docs/EDITOR_INTEGRATION.md) - Editor usage guide
- [docs/TERRAFORMING.md](docs/TERRAFORMING.md) - Terraforming tools
- [docs/INPUT_SYSTEM.md](docs/INPUT_SYSTEM.md) - Input system documentation
- [docs/AUDIO_SYSTEM.md](docs/AUDIO_SYSTEM.md) - Audio engine guide
- [docs/LOGGING.md](docs/LOGGING.md) - Logging system
- [textures/USAGE.md](textures/USAGE.md) - Texture system guide

### Development
- [ARCHITECTURE.md](ARCHITECTURE.md) - System architecture
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
- [docs/CPP_GUIDELINES.md](docs/CPP_GUIDELINES.md) - C++ coding standards
- [docs/DEVELOPMENT_TOOLS.md](docs/DEVELOPMENT_TOOLS.md) - Dev tools guide

### Status & Planning
- [PROJECT_STATUS.md](PROJECT_STATUS.md) - Current status
- [ROADMAP.md](ROADMAP.md) - Development roadmap
- [CHANGELOG.md](CHANGELOG.md) - Version history

### Advanced
- [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) - .NET 9 bindings
- [LUA_INTEGRATION_GUIDE.md](LUA_INTEGRATION_GUIDE.md) - Lua scripting
- [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md) - Windows-native features (Jump Lists, DPI, etc.)
- [WINDOWS_NATIVE_GUI_HOWTO.md](WINDOWS_NATIVE_GUI_HOWTO.md) - **NEW:** Native Win32 GUI (replaces GLFW)
- [WINDOWS_NATIVE_GUI_MIGRATION.md](WINDOWS_NATIVE_GUI_MIGRATION.md) - **NEW:** Migration guide from GLFW to Win32
- [WINDOWS_OPTIMIZATION_SUMMARY.md](WINDOWS_OPTIMIZATION_SUMMARY.md) - Performance optimizations guide
- [TESTING.md](TESTING.md) - Testing guide
- [WORKFLOWS.md](WORKFLOWS.md) - CI/CD workflows

---

## 🎨 Graphics APIs

Fresh Engine uses DirectX as the primary graphics API for Windows gaming:

### DirectX Rendering (Primary)
- **DirectX 12** - Primary renderer for Windows 10/11, cutting-edge performance
- **DirectX 11** - Excellent compatibility fallback for wider hardware support
- **Native Windows integration** - Optimal performance and features

### OpenGL (Optional)
- **OpenGL 4.5+** - Available for compatibility and testing
- **GLEW** - For extension loading

The engine automatically selects **DirectX 12** as the primary renderer on Windows 10/11, with automatic fallback to DirectX 11 if needed. OpenGL is available as an optional backend.

---

## 🔷 .NET 9 Support (Optional)

Optional C# bindings for managed code development on Windows:

```batch
cd dotnet
dotnet build -c Release
```

**See [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) and [dotnet/README.md](dotnet/README.md) for details.**

---

## ⚠️ Current Status

### Platform Support
- **Windows 10/11 (x64)** ✅ **EXCLUSIVE PLATFORM** - Full support with DirectX 12/11
- **Linux** ❌ Not supported (archived)
- **macOS** ❌ Not supported (archived)

### Graphics
- **DirectX 12** - Production ready ✅ Full voxel rendering (Windows 10/11)
- **DirectX 11** - Production ready ✅ Full voxel rendering (Windows compatibility)
- **OpenGL** - Available ✅ Optional compatibility backend

### Editor & NPC Features
- **Unreal-like Editor** - ✅ Professional ImGui-based game development interface
- **NPC System** - ✅ Integrated with character, AI, and behavior tree systems
- **NPC Editor Integration** - 🚧 In development - Easy dropdown-based behavior configuration
- **Visual NPC Placement** - 🚧 In development - Click-to-place NPCs in world editor
- **Behavior Types** - 🚧 In development - Friendly, Trader, Hostile, Patrol, Guard, Wander

### Optional Features
- **Lua Scripting** - Framework exists (~40% complete), needs Sol2 integration
- **Networking** - Framework only (~10% complete), multiplayer not functional
- **Audio** - WAV support (OGG/Vorbis planned)

### Testing
- **Code Coverage** - ~30% (core systems well-tested)
- **Platform Testing** - Windows 10/11 exclusive

**For current status, see [PROJECT_STATUS.md](PROJECT_STATUS.md).**

---

## 👥 Contributing

We welcome contributions to the Windows-exclusive gaming platform! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Ways to contribute:
- 🐛 Report bugs via [Issues](https://github.com/shifty81/fresh/issues)
- ✨ Suggest features for Windows gaming
- 📝 Improve documentation
- 🔧 Submit pull requests
- 💬 Help others in [Discussions](https://github.com/shifty81/fresh/discussions)
- 🎮 Test the NPC editor and behavior systems

Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

---

## 📜 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- Microsoft - DirectX API and Visual Studio 2022
- Unreal Engine - Editor inspiration
- Minecraft - Voxel game inspiration
- Windows gaming community

---

**Ready to build your voxel game on Windows? Get started with [GETTING_STARTED.md](GETTING_STARTED.md)!**

**Build games with the Unreal-like editor - No coding required for NPCs and gameplay!**
