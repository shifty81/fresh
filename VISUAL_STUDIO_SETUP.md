# Visual Studio 2022 Setup Guide

This guide will help you build and run the Fresh Voxel Engine using Visual Studio 2022.

## Prerequisites

### Required Software

1. **Visual Studio 2022** (Community, Professional, or Enterprise) - version 17.8 or later
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Required workloads:
     - "Desktop development with C++"
     - "Game development with C++" (optional, but recommended)
     - ".NET desktop development" (for .NET 9 bindings)

2. **CMake 3.20 or higher**
   - Download from: https://cmake.org/download/
   - Make sure to add CMake to system PATH during installation

3. **.NET 9 SDK** (for managed bindings)
   - Download from: https://dotnet.microsoft.com/download/dotnet/9.0
   - Required for C# interop layer

4. **Windows SDK** (included with Visual Studio)
   - Required for DirectX 11 and DirectX 12 support
   - Version 10.0.19041.0 or higher recommended
   - Automatically installed with "Desktop development with C++" workload

## Quick Start

### Important: Dependency Management

This project requires several dependencies: GLFW, GLM, and ImGui. You have two options:

**Option A: Automated Setup with vcpkg (Recommended)**
```batch
# This will install vcpkg and all dependencies automatically
install.bat
```
After installation completes, the solution will be ready in `build/FreshVoxelEngine.sln`

**Option B: Manual Setup**
If you prefer to manage dependencies manually, see [DEVELOPER_SETUP.md - Installing Prerequisites](DEVELOPER_SETUP.md#installing-prerequisites) for instructions on installing dependencies, then proceed with the methods below.

### Method 1: Using the Batch Script

**Note**: This method requires dependencies to be pre-installed (see above) or vcpkg to be set up.

1. Open a command prompt in the project root directory
2. Run the generation script:
   ```batch
   generate_vs2022.bat
   ```
3. If you see warnings about missing dependencies, you need to either:
   - Run `install.bat` to set up vcpkg and dependencies
   - Or install GLFW, GLM, and ImGui manually (see [DEVELOPER_SETUP.md - Installing Prerequisites](DEVELOPER_SETUP.md#installing-prerequisites))
4. Open `build/FreshVoxelEngine.sln` in Visual Studio 2022
5. Select your configuration (Debug or Release)
6. Press F7 or Ctrl+Shift+B to build

### Method 2: Using CMake GUI

**Note**: Requires dependencies to be pre-installed (see "Dependency Management" section above).

1. Open CMake GUI
2. Set "Where is the source code" to the project root directory
3. Set "Where to build the binaries" to `<project-root>/build`
4. (Optional) To use vcpkg: Set CMAKE_TOOLCHAIN_FILE to `<vcpkg-root>/scripts/buildsystems/vcpkg.cmake`
5. Click "Configure"
6. Select "Visual Studio 17 2022" as the generator
7. Select "x64" as the platform
8. Click "Generate"
9. If you see warnings about missing dependencies, install them (see above)
10. Open `build/FreshVoxelEngine.sln` in Visual Studio 2022

### Method 3: Using CMake Command Line

**Note**: Requires dependencies to be pre-installed (see "Dependency Management" section above).

```batch
mkdir build
cd build
# Without vcpkg (requires manual dependency installation):
cmake -G "Visual Studio 17 2022" -A x64 ..
# Or with vcpkg (recommended):
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

## Project Structure in Visual Studio

After generating the solution, you'll see the following project structure:

```
Solution 'FreshVoxelEngine'
├── FreshVoxelEngine (C++ Executable)
│   ├── Source Files
│   │   ├── core/
│   │   ├── renderer/
│   │   │   └── backends/  (DirectX11, DirectX12)
│   │   ├── voxel/
│   │   ├── generation/
│   │   ├── physics/
│   │   └── ... (other subsystems)
│   └── Header Files
│       └── (mirrors source structure)
├── FreshEngine.Managed (.NET 9 Library)
│   └── C# interop wrappers
└── FreshVoxelEngineTests (Optional, if BUILD_TESTS=ON)
```

## Graphics API Configuration

The engine uses **DirectX as the primary graphics API** for Windows:

### Windows Graphics Stack:
1. **DirectX 12** (default, Windows 10+)
2. **DirectX 11** (fallback)

OpenGL support is available as an optional future enhancement and is not currently built by default.

## Build Configurations

### Debug Configuration
- Optimizations: Disabled
- Debug symbols: Full
- Runtime checks: Enabled
- Useful for development and debugging

### Release Configuration
- Optimizations: Maximum speed (/O2)
- Debug symbols: None (or minimal)
- Runtime checks: Disabled
- Recommended for production builds

## Building from Visual Studio

1. **Build Solution**: `Build > Build Solution` (F7)
2. **Rebuild Solution**: `Build > Rebuild Solution` (Ctrl+Alt+F7)
3. **Clean Solution**: `Build > Clean Solution`
4. **Set Startup Project**: Right-click `FreshVoxelEngine` > `Set as Startup Project`
5. **Run**: Press F5 (with debugging) or Ctrl+F5 (without debugging)

## Building from Command Line

```batch
# Build Debug configuration
cmake --build build --config Debug

# Build Release configuration
cmake --build build --config Release

# Build with multiple processors
cmake --build build --config Release -- /m

# Clean build
cmake --build build --config Release --clean-first
```

## Running the Application

### From Visual Studio
1. Set `FreshVoxelEngine` as the startup project
2. Press F5 to run with debugging
3. Press Ctrl+F5 to run without debugging

### From Command Line
```batch
# Debug build
build\Debug\FreshVoxelEngine.exe

# Release build
build\Release\FreshVoxelEngine.exe
```

## Debugging

### Visual Studio Debugger Features
- **Breakpoints**: Click left margin or press F9
- **Step Over**: F10
- **Step Into**: F11
- **Step Out**: Shift+F11
- **Continue**: F5
- **Watch Variables**: Add to Watch window
- **Call Stack**: View > Call Stack (Ctrl+Alt+C)

### Graphics Debugging

#### DirectX 11/12 Debugging
1. Install **Graphics Tools** from Windows optional features
2. In Visual Studio: `Debug > Graphics > Start Graphics Debugging`
3. Capture frames to analyze GPU state
4. Use PIX for Windows for advanced DirectX debugging

## Common Build Issues

### Issue: "DirectX headers not found"
**Solution**:
- Install Windows SDK via Visual Studio Installer
- Update Visual Studio to latest version
- Check that "Windows SDK" is installed in Visual Studio components
- Verify Windows 10/11 SDK version 10.0.19041.0 or higher is installed

### Issue: ".NET 9 SDK not found"
**Solution**:
- Download and install .NET 9 SDK from https://dotnet.microsoft.com/download/dotnet/9.0
- Restart Visual Studio after installation
- Verify installation with: `dotnet --version` (should show 9.0.x)

### Issue: "GLFW not found"
**Solution**:
- If using vcpkg: Dependencies are declared in `vcpkg.json` and will be automatically installed when using the vcpkg toolchain
- Configure CMake with vcpkg: `cmake -B build -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake`
- Alternatively, manually install: `vcpkg install glfw3:x64-windows glm:x64-windows imgui[glfw-binding,dx11-binding,dx12-binding]:x64-windows`

### Issue: "Link error LNK2019"
**Solution**:
- Clean and rebuild solution
- Check that all source files are included in CMakeLists.txt
- Verify library dependencies are correctly linked

## Performance Optimization

### Release Build Optimizations
The project is configured with the following optimizations for Release builds:
- `/O2` - Maximize Speed
- `/GL` - Whole Program Optimization
- `/LTCG` - Link Time Code Generation

### Profiling
1. Use Visual Studio Profiler: `Debug > Performance Profiler`
2. Available tools:
   - CPU Usage
   - Memory Usage
   - GPU Usage
   - .NET Async

## Testing

To build with tests:

```batch
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON ..
cmake --build . --config Release
ctest -C Release
```

Or in Visual Studio:
1. Build the `FreshVoxelEngineTests` project
2. Run tests via Test Explorer (Test > Test Explorer)

## Advanced Configuration

### Custom CMake Options

```batch
# Enable tests
cmake -DBUILD_TESTS=ON ..

# Set custom install prefix
cmake -DCMAKE_INSTALL_PREFIX="C:/FreshEngine" ..

# Specify OpenGL/DirectX SDK path
cmake -DOpenGL/DirectX_ROOT="C:/OpenGL/DirectXSDK/1.3.xxx.x" ..
```

### Multi-Configuration Build

```batch
# Build both Debug and Release
cmake --build . --config Debug
cmake --build . --config Release
```

## Troubleshooting

### CMake Generation Fails
1. Clear CMake cache: Delete `build` directory
2. Regenerate: Run `generate_vs2022.bat` again
3. Check CMake version: `cmake --version` (should be 3.20+)

### Build Fails
1. Clean solution: `Build > Clean Solution`
2. Rebuild: `Build > Rebuild Solution`
3. Check Output window for detailed error messages
4. Verify all dependencies are installed

### Common Compilation Errors

#### Error: "'OpenGLRenderContext': undeclared identifier"
**Cause**: OpenGL backend is being compiled without proper includes or definitions.

**Solution**:
1. Make sure `FRESH_OPENGL_SUPPORT` and `FRESH_GLEW_AVAILABLE` are defined when OpenGL is enabled
2. Verify GLEW is installed: `vcpkg install glew:x64-windows`
3. Regenerate the solution with vcpkg toolchain

#### Error: "'std::make_unique': no matching overloaded function found"
**Cause**: Missing `<memory>` header include.

**Solution**: This has been fixed in the latest version. Update to the latest code or add `#include <memory>` to files using `std::make_unique`.

#### Error: "'ImGuiConfigFlags_DockingEnable': undeclared identifier"
**Cause**: ImGui was not built with docking support, or the feature flags are not properly defined.

**Solution**:
1. Make sure ImGui is installed with docking support: 
   ```batch
   vcpkg install imgui[docking-experimental,glfw-binding,dx11-binding,dx12-binding]:x64-windows
   ```
2. Regenerate the solution after updating vcpkg packages
3. The engine now automatically guards these features with `#ifdef IMGUI_HAS_DOCK`

#### Error: "'UpdatePlatformWindows': is not a member of 'ImGui'"
**Cause**: Multi-viewport feature is not available in the ImGui build.

**Solution**: 
1. This feature requires ImGui's docking branch
2. Install with: `vcpkg install imgui[docking-experimental]:x64-windows`
3. The engine now guards this with `#ifdef IMGUI_HAS_VIEWPORT` so it will gracefully degrade if not available

### Runtime Issues

#### Error: "Unable to start program ... ALL_BUILD access is denied"
**Cause**: `ALL_BUILD` is a CMake meta-target that builds all projects but is not an executable. Visual Studio is trying to run it as the startup project.

**Solution**:
1. In Solution Explorer, right-click on `FreshVoxelEngine` project
2. Select `Set as Startup Project`
3. The project name should now be **bold**
4. Press F5 or Ctrl+F5 to run

**Visual Guide**:
```
Solution Explorer
└── Solution 'FreshVoxelEngine'
    ├── ALL_BUILD          <- Don't run this (meta-target)
    ├── FreshVoxelEngine   <- Set THIS as startup project (bold = active)
    ├── INSTALL
    └── ZERO_CHECK
```

**Alternative**: Use the toolbar dropdown:
1. Find the "Select Startup Item" dropdown in the toolbar (shows current startup project)
2. Click the dropdown arrow
3. Select `FreshVoxelEngine.exe`

#### Error: "The system cannot find the file specified" when running
**Cause**: Missing DLLs or the executable is not in the expected location.

**Solution**:
1. Make sure you've built the solution first (F7)
2. Check that the executable exists in `build/Debug/` or `build/Release/`
3. If using vcpkg dependencies, make sure DLLs are copied to the output directory
4. Try running from command line to see full error: `build\Debug\FreshVoxelEngine.exe`

### Runtime Crashes
1. Run with debugger (F5) to catch exceptions
2. Check Output window for error messages
3. Enable DirectX validation layers (Debug build)
4. Verify all DLLs are in the executable directory
5. Check that DirectX runtime is up to date

### Dependency Installation Issues

#### vcpkg packages fail to install
**Solution**:
1. Make sure vcpkg is up to date: `cd vcpkg && git pull`
2. Bootstrap vcpkg again: `.\vcpkg\bootstrap-vcpkg.bat`
3. Clear vcpkg cache: `.\vcpkg\vcpkg remove --outdated`
4. Try installing packages one at a time to identify the problem:
   ```batch
   vcpkg install glfw3:x64-windows
   vcpkg install glm:x64-windows
   vcpkg install imgui[docking-experimental,glfw-binding,dx11-binding,dx12-binding]:x64-windows
   ```

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [Visual Studio C++ Documentation](https://docs.microsoft.com/en-us/cpp/)
- [DirectX 11 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews)
- [DirectX 12 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide)
- [.NET 9 Documentation](https://docs.microsoft.com/en-us/dotnet/)
- [P/Invoke Guide](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke)

## Support

For issues and questions:
- GitHub Issues: https://github.com/shifty81/fresh/issues
- Documentation: See `/docs` directory
- Contributing: See `CONTRIBUTING.md`
