# vcpkg Setup Guide for Fresh Voxel Engine

This guide helps you set up vcpkg to resolve dependency issues like "Cannot open include file: 'glm/glm.hpp'" or "Cannot open include file: 'GLFW/glfw3.h'".

## Quick Fix (Recommended for Most Users)

If you're getting build errors about missing include files, follow these steps:

```batch
# From the Fresh project root directory
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..
generate_vs2022.bat
```

That's it! The build system will automatically detect vcpkg and install all required dependencies.

## Understanding vcpkg Installation Options

Fresh supports two vcpkg installation locations:

### Option 1: Project Directory (Recommended) ✅

**Location:** `./vcpkg` (inside the fresh project folder)

**Advantages:**
- ✅ Simplest setup - no path confusion
- ✅ Self-contained - everything in one place
- ✅ Works consistently everywhere
- ✅ Easier to understand for beginners

**Setup:**
```batch
cd fresh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..
```

### Option 2: Parent Directory (Alternative)

**Location:** `../vcpkg` (one level above fresh project folder)

**Advantages:**
- ✅ Share vcpkg across multiple projects
- ✅ Save disk space (2-5 GB per project)
- ✅ Avoid re-downloading dependencies

**Setup:**
```batch
cd parent_directory
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..
cd fresh
```

## How Detection Works

The `generate_vs2022.bat` script automatically checks for vcpkg in this order:

1. **First:** Project directory (`./vcpkg`)
2. **Then:** Parent directory (`../vcpkg`)

This means you can use either location, and the script will find it automatically.

## Troubleshooting

### Error: "Cannot open include file: 'glm/glm.hpp'"

**Cause:** vcpkg is not installed or CMake didn't use the vcpkg toolchain.

**Solution:**
1. Install vcpkg in the project directory (see Quick Fix above)
2. Run `generate_vs2022.bat` to regenerate the solution
3. Build the project

### Error: "CMake says 'Not using vcpkg'"

**Cause:** The vcpkg toolchain file wasn't passed to CMake.

**Solution:**
- Use `generate_vs2022.bat` instead of running CMake manually
- The script automatically detects vcpkg and passes the correct path

### Error: "vcpkg not found"

**Cause:** vcpkg is not installed in either location.

**Solution:**
Follow the Quick Fix steps at the top of this guide.

## Verifying Your Setup

To check if vcpkg is properly installed, look for these files:

**Project directory:**
- `./vcpkg/vcpkg.exe` should exist
- `./vcpkg/scripts/buildsystems/vcpkg.cmake` should exist

**Parent directory:**
- `../vcpkg/vcpkg.exe` should exist
- `../vcpkg/scripts/buildsystems/vcpkg.cmake` should exist

## What Gets Installed

When you run `generate_vs2022.bat`, vcpkg will automatically install:
- **GLFW** - Window and input handling
- **GLM** - Mathematics library
- **ImGui** - User interface library
- **GLEW** - OpenGL extension loading (Windows)

This happens during the CMake configuration step and may take 5-15 minutes on first run.

## For Advanced Users

If you need to manually specify the vcpkg toolchain file:

```batch
# For project directory installation
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake ..

# For parent directory installation
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=..\..\vcpkg\scripts\buildsystems\vcpkg.cmake ..
```

However, using `generate_vs2022.bat` is recommended as it handles this automatically.

## Additional Resources

- [BUILD.md](BUILD.md) - Complete build instructions
- [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - Development environment setup
- [QUICK_FIX_BUILD_ERRORS.md](QUICK_FIX_BUILD_ERRORS.md) - Common build error solutions
- [vcpkg documentation](https://github.com/microsoft/vcpkg) - Official vcpkg docs
