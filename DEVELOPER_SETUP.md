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

### Option 1: Automated Setup (Recommended)

**Easy Installation (Choose Your Method)**

```batch
# Clone the repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Option A: Run the launcher (lets you choose between PowerShell and Batch)
install.bat

# Option B: Run PowerShell installer directly (Recommended)
powershell -ExecutionPolicy Bypass -File tools\build_tools\install.ps1

# Option C: Run Batch installer directly (Classic method)
tools\build_tools\install.bat
```

**Why PowerShell is Recommended:**
- Better progress visibility during long operations (vcpkg installation, CMake configuration)
- More reliable error handling and reporting
- Real-time output from CMake and build processes
- Clearer status messages and colored output
- Less likely to appear frozen during dependency installation

The installer (both versions) will:
- Check for required software (CMake, Visual Studio 2022)
- Install vcpkg package manager (optional, but recommended)
- Configure vcpkg in manifest mode
- Generate Visual Studio 2022 solution files
- Automatically install dependencies (GLFW, GLM, ImGui) during CMake
- Build the project in Release configuration
- Create convenient shortcut scripts

### Option 2: Manual Setup with vcpkg

```batch
# Install vcpkg for package management
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install

# Clone and build Fresh
cd ..
git clone https://github.com/shifty81/fresh.git
cd fresh

# Dependencies will be automatically installed via vcpkg.json manifest
# Generate Visual Studio solution with vcpkg toolchain
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

**Note**: The project now includes a `vcpkg.json` manifest file. When using vcpkg with the toolchain file, dependencies (GLFW and GLM) will be automatically installed. Manual installation with `vcpkg install` is no longer required.

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
- C++17 standard
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

- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS setup
- [Build Tools README](tools/build_tools/README.md) - Automated installer docs
- [ARCHITECTURE.md](ARCHITECTURE.md) - Engine architecture overview
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
