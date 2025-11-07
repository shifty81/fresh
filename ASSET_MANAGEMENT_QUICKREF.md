# Asset Management System - Quick Reference

## Overview

The Fresh Engine Asset Management System provides a complete solution for dynamic asset loading with C++/C#/Lua integration.

## Key Features

✅ Drag-and-drop asset import  
✅ FileSystemWatcher monitoring  
✅ Automatic type detection  
✅ Lua configuration  
✅ P/Invoke interop  
✅ Cross-platform paths  

## Quick Start

### 1. Build the C++ Engine

```bash
# Using vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### 2. Build the C# Application

```bash
cd dotnet/AssetInterop
dotnet build -c Release
```

### 3. Run the Application

```bash
cd dotnet/AssetInterop/bin/Release/net9.0-windows
./AssetInterop.exe
```

## Usage Examples

### C# - Drag and Drop

```csharp
using FreshEngine.AssetManagement;

var form = new AssetManagementForm("./Assets");
Application.Run(form);
```

### C# - Programmatic Import

```csharp
using var assetManager = new AssetManagerWrapper("./Assets");
bool success = assetManager.ProcessAsset("texture.png", "Texture");
```

### C++ - Asset Loading

```cpp
#include "assets/AssetManager.h"

auto* manager = fresh::AssetManager::GetInstance();
manager->Initialize("./Assets");
manager->LoadAssetsFromLua("./Assets/asset_config.lua");
```

### Lua - Configuration

```lua
Assets = {
    Textures = {
        {"grass", "Textures/grass.png"}
    }
}
return Assets
```

## Supported Asset Types

| Type | Extensions | Folder |
|------|-----------|--------|
| Texture | .png, .jpg, .bmp, .tga, .dds | Textures/ |
| Model | .obj, .fbx, .gltf, .glb | Models/ |
| Script | .lua, .js, .py | Scripts/ |
| Audio | .wav, .mp3, .ogg, .flac | Audio/ |
| Shader | .glsl, .hlsl, .vert, .frag | Shaders/ |
| Config | .json, .xml, .yaml, .ini | Configs/ |

## P/Invoke vs C++/CLI

### Why P/Invoke?

✅ No mixed-mode assemblies  
✅ Works with .NET Core/.NET 5+  
✅ Clear separation  
✅ Standard marshalling  
✅ No special compiler  

### P/Invoke Example

```csharp
[DllImport("FreshVoxelEngine.dll")]
[return: MarshalAs(UnmanagedType.I1)]
public static extern bool AssetManager_ProcessNewAsset(
    IntPtr instance,
    [MarshalAs(UnmanagedType.LPStr)] string sourcePath,
    [MarshalAs(UnmanagedType.LPStr)] string assetType
);
```

### C++/CLI Alternative

```cpp
public ref class AssetInteropWrapper {
    void ProcessAsset(System::String^ path, System::String^ type) {
        std::string nativePath = marshal_as<std::string>(path);
        NativeAssetManager::ProcessAsset(nativePath, nativeType);
    }
};
```

## Architecture Diagram

```
┌─────────────────────────────┐
│  C# UI (WinForms)           │
│  - Drag-and-drop            │
│  - FileSystemWatcher        │
│  - AssetTypeDialog          │
└──────────┬──────────────────┘
           │ P/Invoke
┌──────────▼──────────────────┐
│  C API Exports              │
│  - C-compatible functions   │
│  - String marshalling       │
└──────────┬──────────────────┘
           │
┌──────────▼──────────────────┐
│  C++ AssetManager           │
│  - File operations          │
│  - Directory management     │
│  - Lua integration          │
└──────────┬──────────────────┘
           │ Lua C API
┌──────────▼──────────────────┐
│  Lua Configuration          │
│  - asset_config.lua         │
│  - Runtime-editable         │
└─────────────────────────────┘
```

## File Structure

```
fresh/
├── include/assets/
│   ├── AssetManager.h              # Core asset manager
│   └── AssetManagerExports.h       # C API exports
├── src/assets/
│   ├── AssetManager.cpp
│   ├── AssetManagerExports.cpp
│   └── ModularAssetSystem.cpp
├── dotnet/AssetInterop/
│   ├── AssetInterop.csproj
│   ├── Program.cs                  # Entry point
│   ├── AssetManagerWrapper.cs      # P/Invoke wrapper
│   ├── AssetManagementForm.cs      # Main UI
│   ├── AssetTypeDialog.cs          # Type selector
│   └── README.md
├── Assets/
│   └── asset_config.lua            # Auto-updated config
├── examples/
│   └── asset_management_example.cpp
└── ASSET_MANAGEMENT_GUIDE.md       # Full documentation
```

## API Reference

### AssetManager (C++)

```cpp
class AssetManager {
    static AssetManager* GetInstance();
    bool Initialize(const std::string& assetRootPath);
    bool ProcessNewAsset(const std::string& source, const std::string& type);
    bool UpdateLuaConfiguration(...);
    bool LoadAssetsFromLua(const std::string& configPath);
    void RegisterImportCallback(const std::string& name, callback);
};
```

### AssetManagerWrapper (C#)

```csharp
public class AssetManagerWrapper : IDisposable {
    public AssetManagerWrapper(string assetRootPath);
    public bool ProcessAsset(string sourcePath, string assetType);
    public bool LoadAssetsFromLua(string configPath);
    public string AssetRootPath { get; }
}
```

## Common Tasks

### Add New Asset Type

1. Update `assetTypeExtensions_` in `AssetManager.cpp`
2. Add to `AssetTypeDialog.cs` ComboBox items
3. Add table to `asset_config.lua`

### Register Import Callback

```cpp
assetManager->RegisterImportCallback("MySystem",
    [](const std::string& path, const std::string& type) {
        if (type == "Texture") {
            LoadTextureToGPU(path);
        }
    });
```

### Monitor Directory Changes

```csharp
var watcher = new FileSystemWatcher(assetPath) {
    NotifyFilter = NotifyFilters.FileName | NotifyFilters.LastWrite,
    IncludeSubdirectories = true,
    EnableRaisingEvents = true
};
watcher.Created += OnAssetCreated;
```

## Troubleshooting

### DLL Not Found
- Ensure FreshVoxelEngine.dll is in the same directory as AssetInterop.exe
- Check system PATH

### Lua Config Not Updating
- Verify file permissions (not read-only)
- Check asset root path exists

### FileSystemWatcher Not Working
- Ensure directory exists
- Check EnableRaisingEvents = true
- Avoid network drives

## Documentation

- **Full Guide**: [ASSET_MANAGEMENT_GUIDE.md](ASSET_MANAGEMENT_GUIDE.md)
- **C# Project**: [dotnet/AssetInterop/README.md](dotnet/AssetInterop/README.md)
- **Examples**: [examples/asset_management_example.cpp](examples/asset_management_example.cpp)

## Advantages Summary

### P/Invoke Benefits
- ✅ Simpler deployment (no mixed-mode)
- ✅ .NET Core/.NET 5+ compatible
- ✅ Clear managed/native separation
- ✅ Standard marshalling
- ✅ No special compiler

### Lua Benefits
- ✅ Runtime editable
- ✅ Human readable
- ✅ Flexible properties
- ✅ Scriptable logic
- ✅ Industry standard

### FileSystemWatcher Benefits
- ✅ Real-time monitoring
- ✅ Low overhead
- ✅ Flexible filtering
- ✅ Event-driven (no polling)

### Drag-and-Drop Benefits
- ✅ Intuitive UX
- ✅ Fast workflow
- ✅ Multi-file support
- ✅ Visual feedback

## License

MIT License - See repository root

## Contributing

See CONTRIBUTING.md for guidelines
