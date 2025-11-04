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

### Windows

```bash
# Update package list
sudo apt-get update

# Install build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git

# Install graphics dependencies
sudo apt-get install -y \
    libglfw3-dev \
    libglm-dev

# Install optional tools
sudo apt-get install -y \
    clang-format \
    clang-tidy \
    cppcheck \
    valgrind \
    gdb
```

### Fedora/RHEL

```bash
sudo dnf install -y \
    cmake \
    gcc-c++ \
    ninja-build \
    git \
    glfw-devel \
    glm-devel
```

### macOS

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake ninja git glfw glm
```

### Windows

1. **Install Visual Studio 2019+** with C++ development tools
   - Download from: https://visualstudio.microsoft.com/

2. **Install CMake**
   - Download from: https://cmake.org/download/
   - Add to PATH during installation

3. **Install vcpkg** (for libraries)
   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   .\vcpkg install glfw3:x64-windows glm:x64-windows
   ```

## Building from Source

### Clone the Repository

```bash
git clone https://github.com/shifty81/fresh.git
cd fresh
```

### Configure Build

#### Linux/macOS (Unix Makefiles)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

#### Linux/macOS (Ninja)

```bash
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

#### Windows (Visual Studio)

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### Build Types

- **Debug**: Full debug symbols, no optimization, assertions enabled
  ```bash
  cmake .. -DCMAKE_BUILD_TYPE=Debug
  ```

- **Release**: Optimized, no debug symbols, assertions disabled
  ```bash
  cmake .. -DCMAKE_BUILD_TYPE=Release
  ```

- **RelWithDebInfo**: Optimized with debug symbols
  ```bash
  cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
  ```

### Compile

#### Linux/macOS

```bash
# Using make
make -j$(nproc)

# Using ninja
ninja
```

#### Windows

```powershell
cmake --build . --config Debug
# or
cmake --build . --config Release
```

### Run

```bash
# Linux/macOS
./FreshVoxelEngine

# Windows
Debug\FreshVoxelEngine.exe
```

## IDE Setup

### Visual Studio Code

1. **Install Extensions**
   - C/C++ (Microsoft)
   - CMake Tools
   - CMake Language Support
   - GitLens

2. **Configure CMake Kit**
   - Press `Ctrl+Shift+P`
   - Select "CMake: Select a Kit"
   - Choose your compiler

3. **Build**
   - Press `F7` or use CMake sidebar

4. **Debug**
   - Set breakpoints
   - Press `F5`

#### Recommended settings.json

```json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.configureOnOpen": true,
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
```

### CLion

1. **Open Project**
   - File → Open → Select repository root

2. **Configure Toolchain**
   - Settings → Build, Execution, Deployment → Toolchains
   - Select compiler and CMake

3. **Build**
   - Click hammer icon or `Ctrl+F9`

4. **Debug**
   - Set breakpoints
   - Click bug icon or `Shift+F9`

### Visual Studio (Windows)

1. **Open Folder**
   - File → Open → Folder
   - Select repository root

2. **Configure CMake**
   - CMakeSettings.json will be auto-generated
   - Customize build configurations if needed

3. **Build**
   - Build → Build All or `Ctrl+Shift+B`

4. **Debug**
   - Set breakpoints
   - Debug → Start Debugging or `F5`

## Running Tests

> **Note**: Test infrastructure is under development

When available:

```bash
cd build
ctest --output-on-failure
```

## Debugging

### GDB (Linux)

```bash
# Build with debug symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

# Run with GDB
gdb ./FreshVoxelEngine

# Common GDB commands
(gdb) run              # Start program
(gdb) break main       # Set breakpoint
(gdb) continue         # Continue execution
(gdb) backtrace        # Show stack trace
(gdb) print variable   # Print variable value
```

### LLDB (macOS)

```bash
lldb ./FreshVoxelEngine

# Common LLDB commands
(lldb) run
(lldb) breakpoint set --name main
(lldb) continue
(lldb) bt              # Backtrace
(lldb) frame variable  # Show variables
```

### Visual Studio Debugger (Windows)

1. Set breakpoints by clicking in the margin
2. Press `F5` to start debugging
3. Use debugging windows: Locals, Watch, Call Stack


## Troubleshooting

### Build Errors

#### "GLFW not found"

**Solution:**
```bash
# Install GLFW
sudo apt-get install libglfw3-dev

# Or specify path
cmake .. -DGLFW_DIR=/path/to/glfw
```

#### Compiler errors with C++17

**Solution:**
```bash
# Ensure C++17 support
cmake .. -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
```

### Runtime Errors

#### Black screen / nothing renders

**Causes:**
- Shaders not compiled
- Swapchain creation failed
- Render pass issues

**Solution:**
```bash
# Ensure shaders are in build directory
ls build/shaders/

# Run with validation layers for errors
export VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation
./FreshVoxelEngine
```

### Performance Issues

#### Low FPS

**Solutions:**
- Build in Release mode
- Reduce chunk count
- Check GPU usage
- Disable validation layers

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_VALIDATION_LAYERS=OFF
```

#### High memory usage

**Solutions:**
- Reduce chunk streaming distance
- Check for memory leaks with Valgrind
- Enable memory profiling

```bash
valgrind --leak-check=full ./FreshVoxelEngine
```

## Development Workflow

### Recommended Workflow

1. **Create feature branch**
   ```bash
   git checkout -b feature/my-feature
   ```

2. **Make changes**
   - Edit code
   - Build frequently
   - Test changes

3. **Format code**
   ```bash
   clang-format -i src/**/*.cpp include/**/*.h
   ```

4. **Run static analysis**
   ```bash
   cppcheck --enable=all src/
   ```

5. **Commit**
   ```bash
   git add .
   git commit -m "Add my feature"
   ```

6. **Push and create PR**
   ```bash
   git push origin feature/my-feature
   ```

## Additional Tools

### Recommended Development Tools

- **Profiler**: Tracy, Valgrind (Callgrind)
- **Memory**: Valgrind (Memcheck), AddressSanitizer
- **Graphics**: RenderDoc, Nsight Graphics
- **Diff**: Meld, Beyond Compare
- **Git GUI**: GitKraken, GitHub Desktop

### Hot Reload

The engine supports shader hot-reloading:

1. Edit shader files in `shaders/`
2. Shaders automatically recompile on save
3. See changes immediately in running application

## Getting Help

- 📖 [Documentation](docs/)
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [Report Issues](https://github.com/shifty81/fresh/issues)
- 📧 Email: dev@freshvoxelengine.org

Happy developing! 🚀
