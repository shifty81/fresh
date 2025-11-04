# Cross-Platform Build Guide

## Overview

Fresh Voxel Engine now supports multiple platforms with automatic graphics API selection:

- **Windows**: DirectX 11, DirectX 12, OpenGL (optional)
- **Linux**: OpenGL 4.5+
- **macOS**: OpenGL 4.5+ (deprecated by Apple, Metal support planned)

## Platform-Specific Instructions

### Windows

#### Prerequisites
- Visual Studio 2022 or later
- CMake 3.20+
- vcpkg (recommended for dependency management)

#### Option 1: Using vcpkg (Recommended)

```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install

# Install dependencies
vcpkg install glfw3:x64-windows
vcpkg install glm:x64-windows
vcpkg install imgui[glfw-binding,dx11-binding,dx12-binding,opengl3-binding]:x64-windows
vcpkg install glew:x64-windows

# Build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### Option 2: Using generate_vs2022.bat

```bash
# The batch script will guide you through the process
generate_vs2022.bat
```

### Linux

#### Prerequisites
- GCC 9+ or Clang 10+
- CMake 3.20+
- OpenGL 4.5+ compatible drivers

#### Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglew-dev \
    libglm-dev \
    libopenal-dev

# Build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

#### Fedora/RHEL

```bash
# Install dependencies
sudo dnf install -y \
    gcc-c++ \
    cmake \
    glfw-devel \
    glew-devel \
    glm-devel \
    openal-soft-devel

# Build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

#### Arch Linux

```bash
# Install dependencies
sudo pacman -S \
    base-devel \
    cmake \
    glfw-x11 \
    glew \
    glm \
    openal

# Build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### macOS

#### Prerequisites
- Xcode Command Line Tools
- Homebrew (recommended)
- CMake 3.20+

#### Installation

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake glfw glew glm openal-soft

# Build
mkdir build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

## Graphics API Selection

The engine automatically selects the best graphics API for your platform:

- **Windows**: DirectX 12 (preferred) → DirectX 11 → OpenGL
- **Linux**: OpenGL 4.5+
- **macOS**: OpenGL 4.5+

### Force Specific API

You can force a specific graphics API at runtime:

```cpp
// In your code:
auto renderContext = RenderContextFactory::create(GraphicsAPI::OpenGL);
// Or auto-select:
auto renderContext = RenderContextFactory::createBest();
```

### OpenGL on Windows

To enable OpenGL on Windows (in addition to DirectX):

```bash
cmake .. -DUSE_OPENGL_ON_WINDOWS=ON
```

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | OFF | Build unit tests |
| `USE_OPENGL_ON_WINDOWS` | ON | Enable OpenGL backend on Windows |

## Troubleshooting

### OpenGL Version Issues

If you get errors about OpenGL version:

```bash
# Check your OpenGL version
glxinfo | grep "OpenGL version"  # Linux
```

The engine requires OpenGL 4.5+ but can fall back to 4.0+.

### Missing GLEW

**Linux:**
```bash
sudo apt-get install libglew-dev  # Ubuntu/Debian
sudo dnf install glew-devel       # Fedora/RHEL
```

**macOS:**
```bash
brew install glew
```

**Windows (vcpkg):**
```bash
vcpkg install glew:x64-windows
```

### ImGui Not Found

**Linux:**
ImGui is typically not available in package managers. Use vcpkg or build from source:

```bash
# Using vcpkg on Linux
./vcpkg install imgui[glfw-binding,opengl3-binding]
cmake .. -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```

**Windows:**
```bash
vcpkg install imgui[glfw-binding,dx11-binding,dx12-binding,opengl3-binding]:x64-windows
```

### GLFW Issues

Ensure GLFW 3.3+ is installed:

**Linux:**
```bash
pkg-config --modversion glfw3
```

**macOS:**
```bash
brew info glfw
```

## Building Tests

```bash
mkdir build
cd build
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest  # Run tests
```

## Running the Engine

After building:

**Linux/macOS:**
```bash
./FreshVoxelEngine
```

**Windows:**
```bash
.\Release\FreshVoxelEngine.exe
```

## Performance Notes

### Windows
- **DirectX 12**: Best performance, lowest CPU overhead
- **DirectX 11**: Excellent compatibility, good performance
- **OpenGL**: Good cross-platform testing

### Linux
- **OpenGL**: Native support, excellent performance with modern drivers
- Ensure proprietary GPU drivers are installed for best performance

### macOS
- **OpenGL**: Deprecated by Apple but still functional
- Performance may vary; Metal backend planned for future

## IDE Support

### Visual Studio 2022 (Windows)
```bash
generate_vs2022.bat
# Open build/FreshVoxelEngine.sln
```

### Visual Studio Code (All Platforms)
```json
// .vscode/settings.json
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

### CLion (All Platforms)
CLion has built-in CMake support. Just open the project folder.

## Next Steps

- See [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) for development environment setup
- See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines
- See [docs/IMGUI_DIRECTX_INTEGRATION.md](docs/IMGUI_DIRECTX_INTEGRATION.md) for ImGui usage
