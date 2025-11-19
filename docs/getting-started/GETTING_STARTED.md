# Fresh Voxel Engine - Getting Started Guide

Welcome to Fresh Voxel Engine! This guide will help you get up and running quickly.

## 🎯 What is Fresh?

**Fresh** is a professional **Windows-exclusive** voxel game development platform featuring:
- **Language**: Modern C++20
- **Graphics**: DirectX 12/11 (primary) with optional OpenGL support
- **Windowing**: Native Win32 window management on Windows
- **Editor**: Unreal Engine-like ImGui-based editor with Windows native integration
- **Purpose**: Professional game development with comprehensive tooling

## ⚡ Quick Start

### Prerequisites

Before you begin, ensure you have:
- **Windows 10/11** (x64) - Required
- **Visual Studio 2022** with "Desktop development with C++" workload
- **CMake 3.20+**
- **Git for Windows**
- **.NET 9 SDK** (optional, for C# scripting bindings)

### Installation

**Option 1: Automated Build (Recommended)**

```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated setup script
.\setup-and-build.ps1
```

This single command handles everything: vcpkg setup, dependency installation, CMake generation, and building.

**See [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md) for detailed options.**

**Option 2: Manual Build**

See [BUILD.md](BUILD.md) for complete step-by-step manual build instructions.

## 🎮 First Run

After building, launch the engine:

```batch
cd build\Release
FreshVoxelEngine.exe
```

### Main Menu

On first launch, you'll see the main menu with options to:
1. **Create New World** - Generate a new procedural world
2. **Load Existing World** - Continue from a saved world
3. **Settings** - Configure graphics, audio, and controls
4. **Exit** - Close the application

### Creating Your First World

1. Click **"Create New World"**
2. Enter a world name (e.g., "MyFirstWorld")
3. Set a seed (optional - leave blank for random)
4. Click **"Create"**
5. The world will generate and you'll spawn in the editor

## 🎨 Editor Overview

The Fresh Editor provides a professional game development environment:

### Editor Mode (Default)

When you first enter a world, you're in **Editor Mode**:
- **Free-flying camera** - Move in any direction with full 3D freedom
- **No physics** - Fly through blocks, no gravity
- **Full editor UI** - All panels and tools visible
- **WASD** - Move in camera direction
- **Space** - Fly up (ascend)
- **Ctrl** - Fly down (descend)
- **Shift** - Move faster
- **Mouse** - Look around

### Editor Panels

The editor features multiple ImGui panels:
- **Scene Hierarchy** - Browse world entities and structure
- **Inspector** - Edit properties of selected objects
- **Content Browser** - Asset management and browsing
- **Console** - View logs and execute commands
- **Voxel Tool Palette** - Terraforming and editing tools
- **Hotbar** - Quick access to blocks and tools

### Play Mode

Click the **Play** button in the toolbar to test gameplay:
- **First-person controller** - Physics-based movement
- **Gravity enabled** - Fall and jump realistically
- **Collision detection** - Walk on terrain, can't fly through blocks
- **WASD** - Move forward/back/left/right
- **Space** - Jump
- **Shift** - Sprint
- **Ctrl** - Crouch (prevents falling off edges)

Click **Stop** to return to Editor Mode.

## 🔨 Basic Controls

### Mouse Controls
- **Mouse Movement** - Look around (first-person camera)
- **Left Click** - Break/remove blocks
- **Right Click** - Place blocks
- **Scroll Wheel** - Change selected block type (in hotbar)
- **Click UI Elements** - Interact with editor panels

### Keyboard Controls

**Editor Mode:**
- **WASD** - Fly in camera direction
- **Space** - Ascend
- **Ctrl** - Descend
- **Shift** - Fly faster
- **Tab** - Toggle editor UI visibility
- **Esc** - Open menu/settings

**Play Mode:**
- **WASD** - Walk (FPS-style movement)
- **Space** - Jump
- **Shift** - Sprint (run faster)
- **Ctrl** - Crouch
- **Esc** - Pause/return to editor

**See [CONTROLS.md](CONTROLS.md) for complete controls reference.**

## 🏗️ Building and Terraforming

### Placing Blocks

1. Select a block type from the hotbar (1-9 keys or scroll wheel)
2. Aim at a surface with your mouse
3. Right-click to place the block

### Breaking Blocks

1. Aim at a block you want to remove
2. Left-click to break it

### Advanced Terraforming Tools

Access the **Voxel Tool Palette** panel for advanced tools:
- **Brush Tool** - Paint terrain with selected voxel
- **Sphere Tool** - Create/remove spherical areas
- **Cube Tool** - Create/remove cubic areas
- **Flatten Tool** - Level terrain to a specific height
- **Smooth Tool** - Smooth rough terrain
- **Paint Tool** - Change existing voxel types

**See [docs/TERRAFORMING.md](docs/TERRAFORMING.md) for detailed tool usage.**

## 🎬 Example Scenarios

The `examples/` directory contains documentation for 6 playable in-engine demo scenarios:

1. **Simple Scene** - Basic terrain and building
2. **Combat Arena** - Fighting mechanics test
3. **Trading Post** - NPC interaction demo
4. **Dungeon Crawler** - Exploration gameplay
5. **Creative Building** - Advanced construction
6. **Showcase Demo** - Complete feature demonstration

**See [examples/README.md](examples/README.md) for details.**

### Try the Showcase Demo

Create a world named "ShowcaseDemo" with seed **12345** to experience all engine features!

**See [examples/DEMO_SHOWCASE.md](examples/DEMO_SHOWCASE.md) for the guided demo.**

## 🧩 Next Steps

### For Players
- Experiment with different world seeds
- Try all the terraforming tools
- Build structures and explore biomes
- Test Play Mode to experience gameplay physics

### For Developers
- Read [ARCHITECTURE.md](ARCHITECTURE.md) to understand the codebase
- Check [PROJECT_STATUS.md](PROJECT_STATUS.md) for current development status
- Review [CONTRIBUTING.md](CONTRIBUTING.md) to contribute to the project
- Explore [ROADMAP.md](ROADMAP.md) for planned features

### Advanced Features
- **C# Scripting**: See [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md)
- **Lua Scripting**: See [LUA_INTEGRATION_GUIDE.md](LUA_INTEGRATION_GUIDE.md)
- **Windows Integration**: See [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md)
- **Editor Features**: See [EDITOR_FEATURES_STATUS.md](EDITOR_FEATURES_STATUS.md)

## 📚 Additional Documentation

- [README.md](README.md) - Project overview and features
- [BUILD.md](BUILD.md) - Complete build instructions
- [CONTROLS.md](CONTROLS.md) - Complete control reference
- [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) - Complete documentation guide
- [FAQ.md](FAQ.md) - Frequently asked questions

## 💬 Getting Help

If you encounter issues:
1. Check [FAQ.md](FAQ.md) for common questions
2. Search [GitHub Issues](https://github.com/shifty81/fresh/issues)
3. Create a new issue with details about your problem
4. Join [GitHub Discussions](https://github.com/shifty81/fresh/discussions)

## 🚀 Ready to Build?

You now have everything you need to start creating with Fresh Voxel Engine!

**Happy building! 🎮**
