# Fresh Voxel Engine - Visual Quick Start Guide

This guide provides step-by-step visual instructions for building and running the Fresh Voxel Engine in Visual Studio 2022.

## 📋 Prerequisites Checklist

Before you begin, make sure you have:

- ✅ **.NET 9 SDK** (Required - Primary development method)
  - Download: https://dotnet.microsoft.com/download/dotnet/9.0
  - Verify: `dotnet --version` (should show 9.0.x)
- ✅ **Visual Studio 2022** (version 17.8 or later)
  - With "Desktop development with C++" workload
  - With ".NET desktop development" workload
  - Windows SDK 10.0.19041.0 or higher
- ✅ **CMake 3.20+** (added to system PATH)
- ✅ **Git** (for version control and vcpkg)

## 🚀 Quick Setup (Automated - Recommended)

### Step 1: Clone the Repository

```bash
git clone https://github.com/shifty81/fresh.git
cd fresh
```

### Step 2: Run Automatic Setup

Open Command Prompt or PowerShell in the project directory and run:

```batch
install.bat
```

This script will:
1. ✅ Install vcpkg (dependency manager)
2. ✅ Install all required libraries (GLFW, GLM, ImGui with docking support)
3. ✅ Generate Visual Studio 2022 solution
4. ✅ Configure the project with all dependencies

**⏱️ Time**: 5-15 minutes (depending on internet speed)

**What you'll see**:
```
================================================
Fresh Voxel Engine - Automated Installation
================================================

[1/4] Checking prerequisites...
✓ CMake found
✓ Visual Studio 2022 found

[2/4] Setting up vcpkg...
✓ vcpkg cloned
✓ vcpkg bootstrapped

[3/4] Installing dependencies...
Installing glfw3...
Installing glm...
Installing imgui with docking support...

[4/4] Generating Visual Studio solution...
✓ Solution generated: build/FreshVoxelEngine.sln

================================================
Installation complete!
================================================
```

### Step 3: Open the Solution

1. Navigate to the `build` folder
2. Double-click `FreshVoxelEngine.sln`
3. Visual Studio 2022 will open

## 🔧 Setting Up Visual Studio (First Time)

### Step 4: Set the Startup Project

**⚠️ CRITICAL STEP** - This fixes the "ALL_BUILD access denied" error

In Solution Explorer:

```
Solution 'FreshVoxelEngine' (4 of 4 projects)
├── ALL_BUILD          ← DON'T run this
├── FreshVoxelEngine   ← RIGHT-CLICK THIS ONE
├── INSTALL
└── ZERO_CHECK
```

**Actions**:
1. **Right-click** on `FreshVoxelEngine` project
2. Select **"Set as Startup Project"**
3. The project name will become **bold**

**Alternative**: Use the toolbar dropdown:
- Look for the green "Start" button in the toolbar
- Click the dropdown arrow next to it
- Select `FreshVoxelEngine.exe`

### Step 5: Select Build Configuration

In the toolbar, you'll see two dropdowns:

```
[Solution Configurations ▼] [Solution Platforms ▼]
```

1. Set **Configuration** to `Release` (for better performance) or `Debug` (for development)
2. Set **Platform** to `x64`

**Recommended for first run**: `Release` + `x64`

### Step 6: Build the Solution

**Keyboard**: Press `F7` or `Ctrl+Shift+B`

**Menu**: `Build > Build Solution`

**What you'll see in Output window**:
```
1>------ Build started: Project: FreshVoxelEngine, Configuration: Release x64 ------
1>  Building Custom Rule C:/Projects/fresh/CMakeLists.txt
1>  main.cpp
1>  Engine.cpp
1>  Window.cpp
...
1>  FreshVoxelEngine.vcxproj -> C:\Projects\fresh\build\Release\FreshVoxelEngine.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

**⏱️ Time**: 2-5 minutes (first build)

### Step 7: Run the Application

**Keyboard**: 
- `F5` - Run with debugger (slower, but catches errors)
- `Ctrl+F5` - Run without debugger (faster)

**Menu**: `Debug > Start Debugging` (F5) or `Debug > Start Without Debugging` (Ctrl+F5)

**Success!** The engine window should open showing the voxel world.

## 🎮 Using the Engine

### Basic Controls

Once the engine is running:

- **W/A/S/D** - Move around
- **Mouse** - Look around
- **Space** - Jump (if implemented)
- **Esc** - Pause/Menu
- **Left Click** - Break block
- **Right Click** - Place block

### Editor UI (ImGui)

If ImGui is available, you'll see:
- **Hierarchy Panel** - Scene objects
- **Inspector** - Object properties
- **Console** - Log messages
- **Content Browser** - Asset management

## 🐛 Troubleshooting

### Problem: "Unable to start program ... ALL_BUILD access is denied"

**Why**: ALL_BUILD is not an executable program - it's a build target.

**Fix**:
1. Right-click `FreshVoxelEngine` in Solution Explorer
2. Click "Set as Startup Project"
3. Try running again (F5)

**Visual indicator**: The startup project appears in **bold** in Solution Explorer.

---

### Problem: Build errors with ImGui (DockingEnable, ViewportsEnable, UpdatePlatformWindows)

**Why**: ImGui needs to be built with docking support enabled.

**Fix**:
```batch
# Reinstall ImGui with docking support
vcpkg remove imgui
vcpkg install imgui[docking-experimental,glfw-binding,dx11-binding,dx12-binding]:x64-windows

# Regenerate the solution
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

Then rebuild in Visual Studio.

---

### Problem: "std::make_unique no matching overloaded function"

**Why**: Missing `<memory>` header include.

**Fix**: This has been fixed in the latest version. Make sure you have the latest code:
```bash
git pull origin main
```

---

### Problem: "Cannot open include file: 'GL/glew.h'"

**Why**: GLEW library is not installed (needed for OpenGL support).

**Fix**:
```batch
vcpkg install glew:x64-windows
```

Then regenerate and rebuild.

---

### Problem: Build succeeds but nothing happens when running

**Check**:
1. Look in `build/Release/` - is `FreshVoxelEngine.exe` there?
2. Try running from command line:
   ```batch
   cd build\Release
   FreshVoxelEngine.exe
   ```
3. Check for missing DLLs in the error message
4. Verify graphics drivers are up to date

---

### Problem: "DirectX device creation failed"

**Why**: Graphics drivers are outdated or DirectX runtime is missing.

**Fix**:
1. Update graphics drivers (NVIDIA/AMD/Intel)
2. Install DirectX End-User Runtime: https://www.microsoft.com/en-us/download/details.aspx?id=35
3. Restart your computer

---

## 📚 Next Steps

Now that you have the engine running:

1. **Try the Examples**: Check the `examples/` folder for sample code
   - `examples/basic_setup.cpp` - Minimal setup
   - `examples/creating_chunks.cpp` - Chunk creation
   - `examples/terrain_generation.cpp` - Procedural terrain
   - `examples/player_controller.cpp` - First-person movement

2. **Read the Documentation**:
   - [ARCHITECTURE.md](ARCHITECTURE.md) - Engine architecture overview
   - [CORE_SYSTEMS_GUIDE.md](CORE_SYSTEMS_GUIDE.md) - Core systems explained
   - [GETTING_STARTED.md](GETTING_STARTED.md) - Development guide
   - [FAQ.md](FAQ.md) - Frequently asked questions

3. **Explore the API**:
   - [docs/api/](docs/api/) - API documentation
   - [docs/tutorials/](docs/tutorials/) - Step-by-step tutorials

4. **Customize**: 
   - Modify `src/main.cpp` to change engine behavior
   - Add your own voxel types in `include/voxel/VoxelTypes.h`
   - Create custom terrain in `src/generation/TerrainGenerator.cpp`

## 🎯 Visual Summary

```
┌─────────────────────────────────────────────────────────────┐
│  Fresh Voxel Engine - Build & Run Process                   │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. install.bat  →  2. Open .sln  →  3. Set Startup Project │
│     (Automated)      (VS 2022)        (FreshVoxelEngine)    │
│                                                              │
│                ↓                                             │
│                                                              │
│  4. Select Release + x64  →  5. Build (F7)  →  6. Run (F5)  │
│     (Configuration)           (2-5 min)         (Success!)  │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

## 💡 Pro Tips

### Faster Builds
- Use `Release` configuration for faster execution
- Enable parallel builds: `Tools > Options > Projects and Solutions > Build and Run > Maximum parallel builds`
- Use an SSD for your project directory

### Better Debugging
- Use `Debug` configuration for detailed error messages
- Enable "Break on Exception": `Debug > Windows > Exception Settings` → Check "C++ Exceptions"
- Use breakpoints (F9) to pause execution and inspect variables

### Efficient Development
- Use Live++ or Hot Reload for faster iteration (VS 2022 feature)
- Keep the Output window visible to catch build warnings
- Use `Ctrl+K, Ctrl+C` to comment and `Ctrl+K, Ctrl+U` to uncomment code blocks

## 🆘 Getting Help

If you encounter issues not covered here:

1. **Check existing documentation**:
   - [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS setup
   - [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - General development setup
   - [FAQ.md](FAQ.md) - Common questions

2. **Search for similar issues**:
   - GitHub Issues: https://github.com/shifty81/fresh/issues
   - Check closed issues for solutions

3. **Ask for help**:
   - Open a new GitHub issue with:
     - Your VS version
     - CMake version
     - Full error message
     - Steps to reproduce

## ✅ Success Checklist

After following this guide, you should have:

- ✅ Fresh Voxel Engine compiled successfully
- ✅ Engine window opens and displays voxel world
- ✅ No compilation errors
- ✅ Correct startup project set (FreshVoxelEngine in bold)
- ✅ Understanding of basic controls and UI

**Congratulations!** You're ready to start developing with the Fresh Voxel Engine! 🎉
