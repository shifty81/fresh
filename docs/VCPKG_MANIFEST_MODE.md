# vcpkg Manifest Mode Guide

## Overview

Fresh Voxel Engine uses **vcpkg manifest mode** for dependency management. This means dependencies are defined in `vcpkg.json` and automatically installed during CMake configuration.

## How It Works

### vcpkg.json Manifest File

Our project includes a `vcpkg.json` file that defines all dependencies:

```json
{
  "name": "fresh-voxel-engine",
  "version": "0.1.0",
  "dependencies": [
    {
      "name": "glfw3",
      "platform": "windows"
    },
    {
      "name": "glm",
      "platform": "windows"
    },
    {
      "name": "imgui",
      "features": ["glfw-binding", "opengl3-binding", "dx11-binding", "dx12-binding"],
      "platform": "windows"
    }
  ]
}
```

### Automatic Installation

When you run CMake with the vcpkg toolchain file, vcpkg automatically:
1. Reads `vcpkg.json`
2. Downloads required packages
3. Builds them for your platform
4. Makes them available to CMake

**No manual `vcpkg install` commands needed!**

## Installation Process

### Using install.bat (Recommended)

The automated installation script handles everything:

```batch
tools\build_tools\install.bat
```

This script will:
1. Check prerequisites
2. Install vcpkg (if needed)
3. **Skip manual dependency installation** (manifest mode handles this)
4. Run CMake (dependencies installed automatically here)
5. Build the project

### Manual Installation

If you prefer manual installation:

```batch
# 1. Install vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# 2. Generate Visual Studio project (dependencies installed automatically)
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

**Important:** Do NOT run `vcpkg install glfw3 glm imgui` manually! 
Manifest mode handles this automatically during CMake configuration.

## Common Issues

### Error: "In manifest mode, vcpkg install does not support individual package arguments"

**Problem:** You're trying to install packages manually while vcpkg.json exists.

**Solution:** 
- Delete any manual vcpkg install commands
- Let CMake handle dependency installation automatically
- If using our install.bat, make sure you have the latest version

### Dependencies Not Found During Build

**Problem:** CMake can't find GLFW, GLM, or ImGui.

**Solutions:**

1. **Make sure you're using the vcpkg toolchain file:**
   ```batch
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```

2. **Check vcpkg.json is in project root:**
   - File must be named exactly `vcpkg.json`
   - Must be in the same directory as CMakeLists.txt

3. **Verify vcpkg is working:**
   ```batch
   cd vcpkg
   .\vcpkg version
   ```

4. **Clear CMake cache and try again:**
   ```batch
   rmdir /s /q build
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```

### First Build Is Slow

**This is normal!** The first time you build:
- vcpkg downloads all dependencies (~100-200 MB)
- Builds each dependency from source (5-15 minutes)
- Subsequent builds reuse cached binaries (fast!)

## Advanced Usage

### Adding New Dependencies

To add a new library:

1. Edit `vcpkg.json`:
   ```json
   {
     "dependencies": [
       "existing-lib",
       "new-lib"
     ]
   }
   ```

2. Re-run CMake (will automatically download/build new library):
   ```batch
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```

3. Use the library in CMakeLists.txt:
   ```cmake
   find_package(new-lib REQUIRED)
   target_link_libraries(YourTarget PRIVATE new-lib::new-lib)
   ```

### Checking Installed Packages

View what vcpkg has installed for your project:

```batch
cd build
type vcpkg-manifest-install.log
```

Or check vcpkg's installed packages:

```batch
cd vcpkg
.\vcpkg list
```

### Updating Dependencies

To update to latest versions:

1. Update builtin-baseline in vcpkg.json (if specified)
2. Delete build directory
3. Re-run CMake

Or use vcpkg's update command:

```batch
cd vcpkg
git pull
.\bootstrap-vcpkg.bat
```

## Benefits of Manifest Mode

1. **Reproducible Builds**: Everyone gets same dependency versions
2. **Project-Specific**: Dependencies isolated per project
3. **No Manual Commands**: CMake handles everything
4. **Version Control**: vcpkg.json tracks exact dependencies
5. **CI/CD Friendly**: Automated builds work seamlessly

## Migration from Classic Mode

If you have an old project using classic vcpkg:

### Old Way (Classic Mode)
```batch
vcpkg install glfw3:x64-windows
vcpkg install glm:x64-windows
vcpkg install imgui:x64-windows
# Dependencies installed globally
```

### New Way (Manifest Mode)
```json
// vcpkg.json
{
  "dependencies": ["glfw3", "glm", "imgui"]
}
// CMake automatically installs per-project
```

## Resources

- [vcpkg Manifest Documentation](https://learn.microsoft.com/vcpkg/users/manifests)
- [vcpkg Package Search](https://vcpkg.io/en/packages.html)
- [CMake vcpkg Integration](https://learn.microsoft.com/vcpkg/users/buildsystems/cmake-integration)

## Support

If you encounter issues:

1. Check this guide for common problems
2. Check [DEVELOPER_SETUP.md](../DEVELOPER_SETUP.md)
3. Open an issue on GitHub with:
   - Your vcpkg.json contents
   - CMake output/errors
   - vcpkg version (`vcpkg version`)

## Summary

**Key Points:**
- ✅ Use manifest mode (vcpkg.json)
- ✅ Let CMake install dependencies automatically
- ❌ Don't run `vcpkg install` manually
- ✅ Use vcpkg toolchain file with CMake
- ✅ First build takes time (subsequent builds are fast)
