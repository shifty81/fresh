# Developer Setup Guide

This guide will help you set up a development environment for Fresh Voxel Engine on Windows.

## Table of Contents

- [System Requirements](#system-requirements)
- [Installing Prerequisites](#installing-prerequisites)
- [Building from Source](#building-from-source)
- [IDE Setup](#ide-setup)
- [Running Tests](#running-tests)
- [Debugging](#debugging)
- [Troubleshooting](#troubleshooting)

## System Requirements

### Minimum Requirements

- **OS**: Windows 10 (64-bit)
- **CPU**: 4-core processor (2.5 GHz+)
- **RAM**: 8 GB
- **GPU**: DirectX 11+ or OpenGL 4.5+ capable
- **Storage**: 2 GB free space

### Recommended Requirements

- **OS**: Windows 11 (64-bit)
- **CPU**: 8-core processor (3.5 GHz+)
- **RAM**: 16 GB
- **GPU**: Modern GPU with DirectX 12 support (RTX 2060, RX 5700, or better)
- **Storage**: 10 GB free space (for builds and assets)

## Installing Prerequisites

### Required Software

1. **Visual Studio 2022** (recommended) or Visual Studio 2019
   - Download from: https://visualstudio.microsoft.com/
   - Select "Desktop development with C++" workload
   - Includes Windows SDK (required for DirectX 11/12)

2. **CMake 3.20 or higher**
   - Download from: https://cmake.org/download/
   - Add to PATH during installation

3. **Git for Windows**
   - Download from: https://git-scm.com/download/win

### Direct Build Instructions

**For complete step-by-step instructions, see [BUILD.md](BUILD.md).**

Fresh Voxel Engine uses a straightforward build process with Visual Studio 2022:

```batch
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Set up vcpkg in project directory (one-time)
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..

# Generate Visual Studio solution
generate_vs2022.bat

# Build
cd build
cmake --build . --config Release
```

See [BUILD.md](BUILD.md) for detailed instructions with troubleshooting.

### Understanding vcpkg Setup

Fresh Voxel Engine uses vcpkg for dependency management. The build system supports two vcpkg installation locations:

1. **Project Directory (Recommended)**: `./vcpkg` 
   - Self-contained within the project
   - Simplest setup with no path confusion
   - Each project has its own vcpkg installation
   - Example: If Fresh is in `C:\Projects\fresh`, vcpkg is in `C:\Projects\fresh\vcpkg`

2. **Parent Directory (Alternative)**: `../vcpkg`
   - Shared across multiple projects
   - Saves disk space (dependencies are cached once)
   - Avoids re-downloading packages for each project
   - Example: If Fresh is in `C:\Projects\fresh`, vcpkg is in `C:\Projects\vcpkg`

The `generate_vs2022.bat` script automatically detects and uses vcpkg in either location, with project directory taking precedence.

**Verify Your Setup**: Run `verify_vcpkg.bat` to check if vcpkg is properly installed and configured.

### Option 2: Manual Setup with vcpkg

**Recommended Approach (Project-Local vcpkg)**

This approach keeps vcpkg within the project directory for simplicity:

```batch
# Clone Fresh
git clone https://github.com/shifty81/fresh.git
cd fresh

# Install vcpkg locally in the project
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
cd ..

# Generate Visual Studio solution
generate_vs2022.bat
```

**Alternative: Shared vcpkg in Parent Directory**

If you prefer to share vcpkg across multiple projects:

```batch
# Navigate to parent directory (one level above where you'll clone fresh)
cd C:\Projects  # or your preferred location

# Install vcpkg for package management (shared location)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install

# Clone Fresh in the same parent directory
cd ..
git clone https://github.com/shifty81/fresh.git
cd fresh

# Dependencies will be automatically installed via vcpkg.json manifest
# The build scripts will automatically find vcpkg in ../vcpkg
generate_vs2022.bat
```

**Note**: 
- The project now includes a `vcpkg.json` manifest file that automatically manages dependencies
- Build scripts check for vcpkg in the project directory first (`./vcpkg`), then the parent directory (`../vcpkg`)
- The project directory approach is recommended for simplicity and avoiding path-related issues
- When using vcpkg with the toolchain file, dependencies (GLFW, GLM, ImGui) will be automatically installed during CMake configuration

## Building from Source

### Using Visual Studio (Recommended)

#### Method 1: Visual Studio GUI

```batch
# Generate solution
generate_vs2022.bat

# Open in Visual Studio
start build\FreshVoxelEngine.sln
```

In Visual Studio:
1. Select build configuration (Debug/Release)
2. Build → Build Solution (Ctrl+Shift+B)
3. Set FreshVoxelEngine as startup project
4. Debug → Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

#### Method 2: Command Line

```batch
# Generate solution
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release

# Run
build\Debug\FreshVoxelEngine.exe
```

### Build Configurations

- **Debug**: Full debugging symbols, no optimizations
- **Release**: Optimized build, minimal debugging info
- **RelWithDebInfo**: Optimized with debugging symbols
- **MinSizeRel**: Optimized for size

## IDE Setup

### Visual Studio 2022

Fresh Voxel Engine is designed to work seamlessly with Visual Studio 2022.

#### Opening the Project

```batch
# Generate and open solution
generate_vs2022.bat
```

Or manually:
```batch
cmake -B build -G "Visual Studio 17 2022" -A x64
start build\FreshVoxelEngine.sln
```

#### Recommended Extensions

- **C++ Insights**: Better code visualization
- **Visual Assist**: Enhanced IntelliSense
- **CodeMaid**: Code cleanup and formatting
- **ReSharper C++**: Advanced refactoring tools

#### Project Configuration

The solution is pre-configured with:
- C++20 standard
- Multi-threaded DLL runtime (/MD)
- Windows SDK for DirectX support
- Optimized build settings

### Visual Studio Code

While Visual Studio is recommended, VS Code can also be used:

#### Required Extensions

- C/C++ (Microsoft)
- CMake Tools
- CMake

#### Configuration

1. Open the project folder in VS Code
2. CMake Tools will auto-detect the CMakeLists.txt
3. Select kit: "Visual Studio Community 2022 Release - amd64"
4. Build using CMake Tools panel (bottom toolbar)

## Running Tests

### Enable Testing

```batch
# Configure with tests enabled
cmake -B build -DBUILD_TESTS=ON

# Build
cmake --build build --config Debug

# Run tests
cd build
ctest -C Debug --output-on-failure
```

### Visual Studio Test Explorer

1. Build the solution with tests enabled
2. Test → Test Explorer
3. Run All Tests

### Individual Test Execution

```batch
# Run specific test executable
build\Debug\FreshVoxelEngineTests.exe
```

## Debugging

### Visual Studio Debugger

#### Basic Debugging

1. Set breakpoints (F9)
2. Start debugging (F5)
3. Use standard debug controls:
   - Step Over (F10)
   - Step Into (F11)
   - Step Out (Shift+F11)
   - Continue (F5)

#### Debug Windows

- **Locals**: View local variables
- **Autos**: View automatically tracked variables
- **Watch**: Monitor specific expressions
- **Call Stack**: View function call hierarchy
- **Immediate Window**: Execute code during debugging

#### Graphics Debugging

For DirectX debugging:
1. Install Graphics Tools from Windows SDK
2. Debug → Graphics → Start Diagnostics
3. Capture frames for analysis

### Performance Profiling

#### Visual Studio Profiler

1. Debug → Performance Profiler
2. Select profiling tools:
   - **CPU Usage**: Identify hotspots
   - **Memory Usage**: Track allocations
   - **GPU Usage**: DirectX performance

#### PIX (for DirectX)

1. Download PIX from Microsoft Store
2. Launch and attach to FreshVoxelEngine.exe
3. Capture GPU traces and analyze performance

## Troubleshooting

### Common Issues

#### CMake Configuration Fails

**Problem**: CMake can't find Visual Studio

**Solution**:
```batch
# Specify generator explicitly
cmake -B build -G "Visual Studio 17 2022" -A x64
```

#### DirectX Compilation Errors

**Problem**: Missing DirectX headers

**Solution**:
- Ensure Windows SDK is installed with Visual Studio
- Reinstall "Windows 10 SDK" component
- Check CMakeLists.txt for DirectX library links

#### GLFW/GLM Not Found

**Problem**: CMake can't find dependencies

**Solution** (if using vcpkg):
```batch
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

Or use automated installer:
```batch
tools\build_tools\install.bat
```

#### Build Hangs or Crashes

**Problem**: Out of memory during compilation

**Solution**:
- Close other applications
- Use Release configuration (less memory)
- Build with fewer parallel jobs:
  ```batch
  cmake --build build -- /maxcpucount:2
  ```

#### Runtime Crashes

**Problem**: Application crashes on startup

**Solutions**:
1. Check GPU drivers are up to date
2. Verify DirectX runtime is installed
3. Run in Debug mode to see error messages
4. Check logs in `logs/` directory

### Getting Help

If you encounter issues:
1. Check the [FAQ](FAQ.md)
2. Review [closed issues](https://github.com/shifty81/fresh/issues?q=is%3Aissue+is%3Aclosed)
3. Open a [new issue](https://github.com/shifty81/fresh/issues/new) with:
   - Windows version
   - Visual Studio version
   - Error messages
   - Log files from `logs/` directory

## Additional Resources

- [BUILD.md](BUILD.md) - Complete step-by-step build instructions
- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS setup
- [tools/README.md](tools/README.md) - Tools directory overview
- [ARCHITECTURE.md](ARCHITECTURE.md) - Engine architecture overview
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
