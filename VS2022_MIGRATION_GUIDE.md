# Visual Studio 2022 Migration Guide

## Overview

The Fresh Voxel Engine has been completely rebuilt around **Visual Studio 2022**, **Windows**, **DirectX 11/12**, and **.NET 9**. This guide explains the new architecture and how to work with it.

## What's Changed

### Graphics Architecture
- **Before**: Multi-platform support with OpenGL, DirectX, and Vulkan backends
- **After**: Windows-exclusive with DirectX 11/12 as primary graphics APIs
- **Reason**: Focused development on Windows platform for better DirectX optimization

### Build System
- **Before**: Generic CMake configuration supporting multiple platforms
- **After**: CMake optimized for Visual Studio 2022 and Windows 10/11
- **CMakePresets**: Linux presets removed, Windows presets enhanced

### .NET Integration
- **New**: .NET 9 managed wrapper library for C# development
- **Location**: `dotnet/` directory with P/Invoke bindings
- **Purpose**: Enable game scripting and logic in C# while maintaining native performance

### Dependencies
- **Updated**: ImGui configured with DirectX bindings only (removed OpenGL3 binding)
- **Simplified**: Focus on Windows-specific libraries

## Architecture Diagram

```
┌─────────────────────────────────────────────────────┐
│                 Developer Options                    │
├─────────────────────────┬───────────────────────────┤
│   Native C++ Game       │   C# Managed Game         │
│   (High Performance)    │   (Rapid Development)     │
└───────────┬─────────────┴───────────┬───────────────┘
            │                         │
            │                         ↓
            │         ┌────────────────────────────────┐
            │         │  FreshEngine.Managed.dll       │
            │         │  (.NET 9 P/Invoke Wrapper)     │
            │         └───────────┬────────────────────┘
            │                     │
            ↓                     ↓
┌───────────────────────────────────────────────────┐
│        FreshVoxelEngine (Native C++)              │
│  ┌─────────────────────────────────────────────┐  │
│  │  Core Systems                               │  │
│  │  - Engine, Window, Input, Resources         │  │
│  ├─────────────────────────────────────────────┤  │
│  │  Renderer (DirectX 11/12)                   │  │
│  │  - Shader Manager, Materials, Textures      │  │
│  ├─────────────────────────────────────────────┤  │
│  │  Voxel Engine                               │  │
│  │  - Chunks, Meshing, World Management        │  │
│  ├─────────────────────────────────────────────┤  │
│  │  Game Systems                               │  │
│  │  - Physics, AI, Audio, Networking           │  │
│  └─────────────────────────────────────────────┘  │
└───────────────────────────────────────────────────┘
                        ↓
┌───────────────────────────────────────────────────┐
│              DirectX 11/12 Runtime                │
│           Windows Graphics Stack                  │
└───────────────────────────────────────────────────┘
```

## New File Structure

```
fresh/
├── CMakeLists.txt              # Updated for VS2022/DirectX focus
├── CMakePresets.json           # Windows-only presets
├── vcpkg.json                  # DirectX-focused dependencies
├── generate_vs2022.bat         # Updated generator script
├── build_all.bat               # NEW: Complete build automation
├── check_dotnet.bat            # NEW: .NET SDK verification
├── DOTNET_INTEGRATION.md       # NEW: .NET 9 integration guide
├── VS2022_MIGRATION_GUIDE.md   # NEW: This file
│
├── dotnet/                     # NEW: .NET 9 Managed Wrapper
│   ├── FreshEngine.Managed.csproj
│   ├── EngineInterop.cs        # P/Invoke bindings
│   ├── global.json             # .NET 9 SDK specification
│   └── README.md               # .NET usage guide
│
├── src/
│   ├── renderer/
│   │   └── backends/
│   │       ├── DirectX11RenderContext.cpp
│   │       └── DirectX12RenderContext.cpp
│   └── ... (other systems)
│
└── include/
    └── ... (headers)
```

## Quick Start

### For Native C++ Development

1. **Install Prerequisites**
   ```batch
   - Visual Studio 2022 (with "Desktop development with C++")
   - CMake 3.20+
   - Windows SDK 10.0.19041.0+
   ```

2. **Generate and Build**
   ```batch
   generate_vs2022.bat
   # Opens build/FreshVoxelEngine.sln
   ```

3. **Build in Visual Studio**
   - Open solution
   - Select Release configuration
   - Build (F7)

### For .NET C# Development

1. **Install Additional Prerequisites**
   ```batch
   - .NET 9 SDK (https://dotnet.microsoft.com/download/dotnet/9.0)
   - Visual Studio 2022 (with ".NET desktop development")
   ```

2. **Verify .NET Installation**
   ```batch
   check_dotnet.bat
   ```

3. **Build Everything**
   ```batch
   build_all.bat
   # Builds both native and managed components
   ```

4. **Use in C#**
   ```csharp
   using FreshEngine.Managed;
   
   using var engine = new Engine();
   if (engine.Initialize())
   {
       engine.WindowTitle = "My Game";
       engine.Run();
   }
   ```

## Migration Checklist

If you're updating from an older version:

- [ ] Update Visual Studio to 2022 (17.8+)
- [ ] Install .NET 9 SDK (if using C# features)
- [ ] Delete old `build/` directory
- [ ] Run `generate_vs2022.bat` to regenerate solution
- [ ] Update any custom OpenGL code to DirectX (if applicable)
- [ ] Review new .NET bindings for C# scripting opportunities

## DirectX Configuration

### Default Behavior
The engine automatically selects the best DirectX version:
1. **DirectX 12** (Windows 10+) - Default
2. **DirectX 11** (Fallback)

### Graphics Features
- **DirectX 12**: Modern low-level API, best performance
- **DirectX 11**: Mature API, excellent compatibility
- Both support:
  - Shader Model 5.0+
  - Compute shaders
  - Tessellation
  - Multi-threading

## .NET 9 Integration

### What's Available Now
- ✅ Engine lifecycle (Create, Initialize, Run, Shutdown)
- ✅ Window management (Title, Size)
- ✅ P/Invoke infrastructure
- ✅ Complete documentation

### Coming Soon
- 🔄 Scene management APIs
- 🔄 Voxel manipulation from C#
- 🔄 Camera control
- 🔄 Input handling
- 🔄 Event callbacks

### Enabling Full Integration
To expose more native functions to .NET, see [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) for instructions on:
- Creating C API exports
- Building engine as DLL
- Adding P/Invoke declarations

## Performance Considerations

### Native C++
- **Best for**: Core engine, rendering, physics, real-time systems
- **Performance**: Maximum (no overhead)
- **Use when**: Every frame, tight loops, GPU communication

### .NET C#
- **Best for**: Game logic, AI, UI, scripting, level design
- **Performance**: Excellent (P/Invoke overhead ~10-20ns per call)
- **Use when**: Game state, turn-based logic, event handling

### Best Practice
- Keep hot paths in native C++
- Use C# for high-level logic
- Batch operations across interop boundary
- Cache frequently accessed data

## Troubleshooting

### Visual Studio 2022 Not Found
- Install from https://visualstudio.microsoft.com/
- Ensure "Desktop development with C++" workload is installed
- Re-run `generate_vs2022.bat`

### .NET 9 SDK Not Found
- Install from https://dotnet.microsoft.com/download/dotnet/9.0
- Verify with: `dotnet --version`
- Re-run `check_dotnet.bat`

### DirectX Headers Not Found
- Install Windows SDK via Visual Studio Installer
- Update Visual Studio to latest version
- Check Windows SDK version (need 10.0.19041.0+)

### vcpkg Dependencies Failed
- Run `install.bat` for automatic setup
- Or manually: 
  ```batch
  vcpkg install glfw3:x64-windows glm:x64-windows ^
    imgui[glfw-binding,dx11-binding,dx12-binding]:x64-windows
  ```

## Resources

### Documentation
- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS2022 setup
- [DOTNET_INTEGRATION.md](DOTNET_INTEGRATION.md) - .NET 9 integration guide
- [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - General development setup
- [dotnet/README.md](dotnet/README.md) - .NET bindings overview

### Microsoft Resources
- [DirectX 11 Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d11/)
- [DirectX 12 Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d12/)
- [.NET 9 Documentation](https://docs.microsoft.com/en-us/dotnet/)
- [P/Invoke Guide](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke)

## Support

For issues:
- GitHub Issues: https://github.com/shifty81/fresh/issues
- Check [FAQ.md](FAQ.md) for common questions
- Review [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for solutions

## What's Next

### Short Term
1. Expand .NET API surface (more functions exposed)
2. Add C# example projects
3. Create managed component system
4. Add event callback support

### Long Term
1. Full C# scripting system
2. Hot-reload for C# game logic
3. Visual Studio extensions for Fresh Engine
4. .NET-based editor tools

## Contributing

When contributing to the new architecture:

1. **Native Code**: Follow existing C++17 patterns
2. **.NET Code**: Use modern C# 12 features
3. **Interop**: Add exports for new features (see DOTNET_INTEGRATION.md)
4. **Documentation**: Update both native and .NET docs
5. **Testing**: Test on Windows 10 and 11 with DirectX 11/12

## Conclusion

The Fresh Voxel Engine is now optimized for modern Windows game development with Visual Studio 2022, DirectX 11/12, and optional .NET 9 integration. This focused approach enables:

- ⚡ Better performance (DirectX optimization)
- 🔧 Easier development (focused toolchain)
- 🎯 Modern features (.NET 9 support)
- 📚 Better documentation (single platform)

Welcome to the new architecture!
