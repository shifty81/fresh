# Fresh Engine .NET 9 Bindings

This directory contains the managed .NET 9 wrapper for the Fresh Voxel Engine, allowing C# and other .NET languages to interact with the native C++ engine.

## Overview

The .NET 9 bindings provide a managed API layer on top of the native Fresh Voxel Engine. This allows:

- **C# Game Development**: Write game logic in C# while leveraging the high-performance native engine
- **Cross-Language Integration**: Use any .NET language (C#, F#, VB.NET) with the engine
- **Modern .NET Features**: Take advantage of .NET 9's latest features and performance improvements
- **Seamless Interop**: P/Invoke-based communication with the native engine

## Requirements

- **Visual Studio 2022** (version 17.8 or later)
- **.NET 9 SDK** (https://dotnet.microsoft.com/download/dotnet/9.0)
- **Windows 10/11** (x64)
- **Fresh Voxel Engine** (native C++ component)

## Building

### From Visual Studio 2022

1. Open the main `FreshVoxelEngine.sln` solution
2. The .NET project will be included automatically
3. Build the solution (F7)
4. The managed DLL will be output to `dotnet/bin/x64/Release/`

### From Command Line

```bash
cd dotnet
dotnet build -c Release
```

## Usage

```csharp
using FreshEngine.Managed;

// Create and initialize the engine
using var engine = new Engine();

if (engine.Initialize())
{
    // Configure the engine
    engine.WindowTitle = "My Voxel Game";
    engine.SetWindowSize(1920, 1080);
    
    // Run the main game loop
    engine.Run();
    
    // Shutdown
    engine.Shutdown();
}
```

## Architecture

The .NET bindings use P/Invoke to communicate with the native engine:

```
┌─────────────────────────────────┐
│     C# Application              │
│  (Your game logic)              │
└─────────────────────────────────┘
            ↓ (Managed API)
┌─────────────────────────────────┐
│  FreshEngine.Managed.dll        │
│  (P/Invoke wrappers)            │
└─────────────────────────────────┘
            ↓ (P/Invoke)
┌─────────────────────────────────┐
│  FreshVoxelEngine.dll           │
│  (Native C++ engine)            │
└─────────────────────────────────┘
            ↓
┌─────────────────────────────────┐
│  DirectX 11/12                  │
│  (Graphics rendering)           │
└─────────────────────────────────┘
```

## Current Status

**Note**: The native engine currently outputs an `.exe` file. To use these .NET bindings effectively, the native engine needs to be compiled as a DLL with exported C API functions. This is a future enhancement.

### To Enable Full .NET Integration:

1. **Modify CMakeLists.txt** to create a shared library (DLL) in addition to the executable
2. **Add C API exports** in the native engine for key functions
3. **Update P/Invoke declarations** in `EngineInterop.cs` to match the exported API

Example CMakeLists.txt modification:
```cmake
# Add shared library target for .NET interop
add_library(${PROJECT_NAME}_dll SHARED ${ENGINE_SOURCES})
set_target_properties(${PROJECT_NAME}_dll PROPERTIES 
    OUTPUT_NAME "FreshVoxelEngine"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
```

## Features

Current .NET wrapper provides access to:

- ✅ Engine lifecycle (Create, Initialize, Run, Shutdown)
- ✅ Window management (Title, Size)
- 🔄 Rendering (coming soon)
- 🔄 Scene management (coming soon)
- 🔄 Voxel world manipulation (coming soon)
- 🔄 Event system (coming soon)

## Examples

See the `examples/` directory in the root project for C# usage examples (coming soon).

## Performance Considerations

- P/Invoke calls have minimal overhead (~10-20ns per call)
- Batch operations when possible to reduce interop boundary crossings
- Large data transfers should use pinned memory or unsafe code
- The native engine runs on its own thread; callbacks require marshaling

## Contributing

When adding new native engine features that should be exposed to .NET:

1. Add C API export in native code with `extern "C"` and `__declspec(dllexport)`
2. Add P/Invoke declaration in `NativeMethods` class
3. Add managed wrapper method in the appropriate managed class
4. Update this README with the new feature

## License

Same as the main Fresh Voxel Engine project (MIT License).
