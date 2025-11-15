# Fresh Voxel Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![.NET 9](https://img.shields.io/badge/.NET-9.0-512BD4.svg)](https://dotnet.microsoft.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://github.com/shifty81/fresh)
[![Graphics](https://img.shields.io/badge/Graphics-OpenGL%20%2F%20DirectX-blue.svg)](https://github.com/shifty81/fresh)

A modern **cross-platform** voxel game engine built with **C++20**, featuring **OpenGL** rendering (default) with **DirectX 11/12** support, **.NET 9 bindings**, **Lua scripting**, procedural terrain generation, and an integrated world editor.

> 📚 **New to Fresh?** See [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) for complete documentation guide.

---

## ✨ Features

### Core Engine
- **OpenGL 4.5+ Rendering** - Default, complete cross-platform graphics (Windows/Linux/macOS)
- **DirectX 11/12 Rendering** - Optional Windows-native graphics backends
- **C++20** - Modern C++ with strong type safety and performance
- **.NET 9 Bindings** - Optional C# scripting support
- **Lua Scripting** - Optional scripting engine (framework ready)
- **CMake Build System** - Cross-IDE support with Visual Studio 2022 focus

### Voxel World
- **Chunk-Based World** - Efficient 16×256×16 chunk system
- **Procedural Generation** - Perlin noise terrain with biomes and caves
- **13+ Voxel Types** - Stone, Dirt, Grass, Sand, Water, Wood, and more
- **Dynamic Streaming** - World loads around player as they explore
- **Greedy Meshing** - Optimized rendering with face culling

### Editor & Tools
- **Integrated Editor** - ImGui-based professional editor interface
- **Windows Native Integration** - Native Windows 10/11 themes, dialogs, and taskbar features
- **Terraforming Tools** - Brush, sphere, cube, flatten, smooth, paint
- **Scene Hierarchy** - Visual world structure browser
- **Content Browser** - Asset management system
- **Undo/Redo** - Up to 100 operations in history
- **Hot-Reload** - Shader changes update in real-time

### Gameplay
- **First-Person Controller** - Smooth WASD movement with physics
- **Block Interaction** - Place and remove blocks with mouse
- **Collision Detection** - Physics-based player movement
- **Camera System** - Free-look FPS camera
- **Main Menu** - Create/load worlds with save system

### Additional Systems
- **Audio Engine** - OpenAL-based audio with WAV support
- **Asset Management** - Modular asset system with texture loading
- **Physics System** - Gravity, collision, and raycasting
- **AI Framework** - Behavior tree system (foundation ready)
- **Networking** - Framework in place for multiplayer (stub implementation)
- **Character System** - Voxel character rendering with IK and animation

---

## 🚀 Quick Start

### Prerequisites

#### Windows
- **Windows 10/11** (x64)
- **Visual Studio 2022** with "Desktop development with C++"
- **CMake 3.20+**
- **Git for Windows**
- **.NET 9 SDK** (optional, for C# bindings only)

#### Linux (Ubuntu/Debian)
- **Ubuntu 20.04+** or **Debian 11+**
- **GCC 11+** or **Clang 12+**
- **CMake 3.20+**
- **Git**
- Development libraries: `libglfw3-dev libglm-dev libglew-dev libopenal-dev`

#### macOS
- **macOS 12.0+** (Monterey or later)
- **Xcode Command Line Tools** or **Clang 12+**
- **CMake 3.20+** (via Homebrew)
- **Git**
- Development libraries: `glfw glm openal-soft` (via Homebrew)

### Build and Run

#### Windows: Automated Build (Recommended)

```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated setup script (installs dependencies, builds project)
.\setup-and-build.ps1
```

This script handles everything: vcpkg setup, dependency installation, CMake generation, and building (10-20 minutes first run).

**See [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md) for detailed options.**

#### Windows: Manual Build

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

#### Linux: Quick Build

```bash
# 1. Install dependencies (Ubuntu/Debian)
sudo apt-get install -y build-essential cmake git \
    libglfw3-dev libglm-dev libglew-dev libopenal-dev libgtest-dev

# 2. Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# 3. Build
mkdir build && cd build
cmake ..
cmake --build . --config Release -j$(nproc)

# 4. Run
./FreshVoxelEngine
```

#### macOS: Quick Build

```bash
# 1. Install dependencies (Homebrew)
brew install cmake glfw glm openal-soft googletest

# 2. Clone repository
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

**Mouse Controls (10x More Responsive!):**
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

**Play Mode (Via Editor Toolbar):**
- **WASD** - Move (FPS-style)
- **Space** - Jump
- **Shift** - Sprint
- **Ctrl** - Crouch (prevents falling off edges!)
- **Physics enabled** - Gravity and collision active

**See [CONTROLS.md](CONTROLS.md) for complete controls reference.**

### Getting Started

1. **Launch** - Run FreshVoxelEngine.exe
2. **Main Menu** - Choose "Create New World" or "Load Existing World"
3. **Explore** - Fly around in free-camera mode to explore procedurally generated terrain
4. **Build** - Place and remove blocks with mouse clicks
5. **Edit** - Use the always-visible editor toolbar and panels for advanced features
6. **Test** - Click **Play** in the editor toolbar to test gameplay with normal character physics

**See [examples/README.md](examples/README.md) for 6 playable in-engine demo scenarios.**

**Try the Showcase Demo**: Create a world named "ShowcaseDemo" with seed **12345** to see all engine capabilities! See [examples/DEMO_SHOWCASE.md](examples/DEMO_SHOWCASE.md) for the complete guided demo.

---

## 🎨 Development Languages & Tools

Fresh Voxel Engine supports multiple development approaches:

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
- [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md) - Windows-native features
- [TESTING.md](TESTING.md) - Testing guide
- [WORKFLOWS.md](WORKFLOWS.md) - CI/CD workflows

---

## 🎨 Graphics APIs

Fresh Engine supports multiple graphics APIs across platforms:

### Windows
- **OpenGL 4.5+** - **Default**, complete rendering implementation, cross-platform
- **DirectX 12** - Available, cutting-edge performance (Windows 10/11)
- **DirectX 11** - Available, excellent compatibility fallback

### Linux
- **OpenGL 4.5+** - Primary rendering backend
- GLEW for extension loading

### macOS
- **OpenGL** - Native framework support

The engine **automatically selects** OpenGL as the default on Windows for its complete feature implementation. DirectX backends are available but have some stub implementations. The best API for the platform is selected at runtime.

---

## 🔷 .NET 9 Support (Optional)

Optional C# bindings for managed code development:

```batch
cd dotnet
dotnet build -c Release
```

**See [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) and [dotnet/README.md](dotnet/README.md) for details.**

---

## ⚠️ Current Limitations

### Platform Support
- **Windows 10/11** ✅ Full support (DirectX 11/12 + OpenGL)
- **Linux** ✅ Full support (OpenGL 4.5+)
- **macOS** ✅ Full support (OpenGL)

### Graphics
- **DirectX 11** - Production ready ✅ Full voxel rendering
- **DirectX 12** - Production ready ✅ Full voxel rendering
- **OpenGL** - Production ready ✅ Full voxel rendering (cross-platform)

### Optional Features
- **Lua Scripting** - Framework exists (~40% complete), needs Sol2 integration
- **Networking** - Framework only (~10% complete), multiplayer not functional
- **Audio** - WAV only (OGG/Vorbis planned)

### Testing
- **Code Coverage** - ~30% (core systems well-tested)
- **Platform Testing** - Windows-focused

**For current status, see [PROJECT_STATUS.md](PROJECT_STATUS.md).**

---

## 👥 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Ways to contribute:
- 🐛 Report bugs via [Issues](https://github.com/shifty81/fresh/issues)
- ✨ Suggest features
- 📝 Improve documentation
- 🔧 Submit pull requests
- 💬 Help others in [Discussions](https://github.com/shifty81/fresh/discussions)

Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

---

## 📜 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- [LearnOpenGL](https://learnopengl.com/) - Rendering tutorials
- Minecraft - Voxel game inspiration
- Open-source voxel engine community

---

**Ready to build your voxel world? Get started with [GETTING_STARTED.md](GETTING_STARTED.md)!**
