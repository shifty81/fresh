# Visual Studio Setup Guide

This guide will help you build and run the Fresh Voxel Engine using Visual Studio 2022.

## Prerequisites

### Required Software

1. **Visual Studio 2022** (Community, Professional, or Enterprise)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Required workloads:
     - "Desktop development with C++"
     - "Game development with C++" (optional, but recommended)

2. **CMake 3.20 or higher**
   - Download from: https://cmake.org/download/
   - Make sure to add CMake to system PATH during installation

3. **Vulkan SDK** (for Vulkan backend)
   - Download from: https://vulkan.lunarg.com/
   - Version 1.2 or higher
   - Includes validation layers and debugging tools

### Optional Dependencies

4. **Windows SDK** (usually included with Visual Studio)
   - Required for DirectX 11 and DirectX 12 support
   - Version 10.0.19041.0 or higher recommended

## Quick Start

### Method 1: Using the Batch Script (Recommended)

1. Open a command prompt in the project root directory
2. Run the generation script:
   ```batch
   generate_vs2022.bat
   ```
3. Open `build/FreshVoxelEngine.sln` in Visual Studio 2022
4. Select your configuration (Debug or Release)
5. Press F7 or Ctrl+Shift+B to build

### Method 2: Using CMake GUI

1. Open CMake GUI
2. Set "Where is the source code" to the project root directory
3. Set "Where to build the binaries" to `<project-root>/build`
4. Click "Configure"
5. Select "Visual Studio 17 2022" as the generator
6. Select "x64" as the platform
7. Click "Generate"
8. Open `build/FreshVoxelEngine.sln` in Visual Studio 2022

### Method 3: Using CMake Command Line

```batch
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Project Structure in Visual Studio

After generating the solution, you'll see the following project structure:

```
Solution 'FreshVoxelEngine'
├── FreshVoxelEngine (Executable)
│   ├── Source Files
│   │   ├── core/
│   │   ├── renderer/
│   │   │   └── backends/  (OpenGL, DirectX11, DirectX12, Vulkan)
│   │   ├── voxel/
│   │   ├── generation/
│   │   ├── physics/
│   │   └── ... (other subsystems)
│   └── Header Files
│       └── (mirrors source structure)
└── FreshVoxelEngineTests (Optional, if BUILD_TESTS=ON)
```

## Graphics API Configuration

The engine automatically selects the best available graphics API for your platform:

### Windows Priority Order:
1. **DirectX 12** (default, Windows 10+)
2. **DirectX 11** (fallback)
3. **OpenGL 4.5+**
4. **Vulkan**

To manually select a graphics API, modify `src/renderer/GraphicsAPI.h`:

```cpp
// Change the selectBestGraphicsAPI() function
inline GraphicsAPI selectBestGraphicsAPI() {
#ifdef _WIN32
    return GraphicsAPI::DirectX11;  // or DirectX12, OpenGL, Vulkan
#else
    return GraphicsAPI::Vulkan;
#endif
}
```

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

#### Vulkan Debugging
1. Enable validation layers in Debug builds (automatic)
2. Check Output window for Vulkan validation messages
3. Use RenderDoc for detailed GPU debugging

## Common Build Issues

### Issue: "Cannot find Vulkan SDK"
**Solution**: 
- Install Vulkan SDK from https://vulkan.lunarg.com/
- Set `VULKAN_SDK` environment variable
- Restart Visual Studio

### Issue: "DirectX headers not found"
**Solution**:
- Install Windows SDK via Visual Studio Installer
- Update Visual Studio to latest version
- Check that "Windows SDK" is installed in Visual Studio components

### Issue: "GLFW not found"
**Solution**:
- GLFW will be automatically downloaded by CMake
- If issues persist, manually install: `vcpkg install glfw3:x64-windows`

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

# Specify Vulkan SDK path
cmake -DVulkan_ROOT="C:/VulkanSDK/1.3.xxx.x" ..
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

### Runtime Crashes
1. Run with debugger (F5) to catch exceptions
2. Check Output window for error messages
3. Enable DirectX/Vulkan validation layers (Debug build)
4. Verify all DLLs are in the executable directory

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [Visual Studio C++ Documentation](https://docs.microsoft.com/en-us/cpp/)
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [DirectX 11 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews)
- [DirectX 12 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide)
- [Learn OpenGL](https://learnopengl.com/)

## Support

For issues and questions:
- GitHub Issues: https://github.com/shifty81/fresh/issues
- Documentation: See `/docs` directory
- Contributing: See `CONTRIBUTING.md`
