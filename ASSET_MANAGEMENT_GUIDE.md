# Asset Management System Integration Guide

This guide provides a comprehensive, in-depth explanation of the **Fresh Engine Asset Management System** that integrates C++, .NET 9.0 (C#), and Lua for dynamic asset loading with drag-and-drop support.

## Table of Contents

1. [System Overview](#system-overview)
2. [Architecture](#architecture)
3. [Components](#components)
4. [Integration Steps](#integration-steps)
5. [Usage Examples](#usage-examples)
6. [API Reference](#api-reference)
7. [Advantages of This Architecture](#advantages-of-this-architecture)

---

## System Overview

The Fresh Engine Asset Management System provides a complete solution for:

- **Drag-and-drop asset import** directly into the running application
- **Automatic file system monitoring** using C# FileSystemWatcher
- **Cross-language integration** between C#, C++, and Lua
- **Lua-based asset configuration** for flexible, runtime-editable settings
- **Type-safe asset organization** with automatic folder structure

### Key Features

✅ Drop files directly onto the application window  
✅ Automatic asset type detection based on file extension  
✅ Manual asset type selection via dialog  
✅ Real-time file system monitoring  
✅ Lua configuration auto-update  
✅ Cross-platform file path handling  
✅ Comprehensive error handling and logging  

---

## Architecture

### Technology Stack

```
┌─────────────────────────────────────────────────────────────┐
│                    C# UI Layer (.NET 9.0)                   │
│  • AssetManagementForm (WinForms with drag-and-drop)       │
│  • FileSystemWatcher (monitors asset directory)            │
│  • AssetTypeDialog (user prompts)                          │
└──────────────────────┬──────────────────────────────────────┘
                       │ P/Invoke
┌──────────────────────▼──────────────────────────────────────┐
│              C API Exports (AssetManagerExports)            │
│  • C-compatible function signatures                         │
│  • String marshalling                                       │
└──────────────────────┬──────────────────────────────────────┘
                       │
┌──────────────────────▼──────────────────────────────────────┐
│                 C++ Core (AssetManager)                     │
│  • File operations (copy, validate)                         │
│  • Directory management                                     │
│  • Lua integration                                          │
└──────────────────────┬──────────────────────────────────────┘
                       │ Lua C API
┌──────────────────────▼──────────────────────────────────────┐
│                  Lua Configuration                          │
│  • asset_config.lua                                         │
│  • Runtime-editable asset metadata                          │
└─────────────────────────────────────────────────────────────┘
```

### Data Flow

1. **User Action**: User drags a file onto the C# WinForms window
2. **Type Detection**: C# detects file type or prompts user
3. **P/Invoke Call**: C# calls native C++ function via P/Invoke
4. **File Processing**: C++ validates, copies file to correct folder
5. **Lua Update**: C++ updates asset_config.lua with new entry
6. **Callback Notification**: C++ fires callbacks to notify engine subsystems

---

## Components

### 1. C++ Core: AssetManager

**Location**: `include/assets/AssetManager.h`, `src/assets/AssetManager.cpp`

The core C++ class that handles all asset management operations:

```cpp
class AssetManager {
public:
    static AssetManager* GetInstance();
    bool Initialize(const std::string& assetRootPath);
    bool ProcessNewAsset(const std::string& sourcePath, const std::string& assetType);
    bool UpdateLuaConfiguration(const std::string& assetPath, ...);
    bool LoadAssetsFromLua(const std::string& configPath);
    // ...
};
```

**Key Responsibilities**:
- Singleton instance management
- File validation and copying
- Directory structure creation
- Lua state management
- Configuration file updates

### 2. C API Exports

**Location**: `include/assets/AssetManagerExports.h`, `src/assets/AssetManagerExports.cpp`

Provides C-compatible exports for P/Invoke:

```cpp
extern "C" {
    EXPORT_API AssetManager* AssetManager_GetInstance();
    EXPORT_API bool AssetManager_Initialize(AssetManager* instance, const char* assetRootPath);
    EXPORT_API bool AssetManager_ProcessNewAsset(AssetManager* instance, 
                                                  const char* sourcePath,
                                                  const char* assetType);
}
```

**Why C API?** C has a stable ABI that allows P/Invoke to call native functions without name mangling issues.

### 3. C# P/Invoke Wrapper

**Location**: `dotnet/AssetInterop/AssetManagerWrapper.cs`

Managed wrapper that marshals data between C# and C++:

```csharp
internal static class NativeAssetMethods
{
    [DllImport("FreshVoxelEngine.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr AssetManager_GetInstance();
    
    [DllImport("FreshVoxelEngine.dll", CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool AssetManager_ProcessNewAsset(IntPtr instance, 
        [MarshalAs(UnmanagedType.LPStr)] string sourcePath,
        [MarshalAs(UnmanagedType.LPStr)] string assetType);
}
```

### 4. C# UI Components

**Location**: `dotnet/AssetInterop/`

#### AssetManagementForm
Main window with drag-and-drop support and FileSystemWatcher integration.

```csharp
public class AssetManagementForm : Form
{
    private void DropPanel_DragDrop(object? sender, DragEventArgs e)
    {
        // Handle dropped files
        string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
        foreach (string file in files)
        {
            ProcessDroppedFile(file);
        }
    }
}
```

#### AssetTypeDialog
Dialog for user to select asset type:

```csharp
public class AssetTypeDialog : Form
{
    public static string? ShowDialog(string fileName)
    {
        // Shows combobox with asset types
        // Auto-detects type based on extension
    }
}
```

### 5. Lua Configuration

**Location**: `Assets/asset_config.lua`

Runtime-editable configuration:

```lua
Assets = {
    Textures = {
        {"grass", "Textures/grass.png"},
        {"stone", "Textures/stone.png"}
    },
    Models = {
        {"tree", "Models/tree.obj"}
    },
    -- ...
}
return Assets
```

---

## Integration Steps

### Step 1: Add Lua Dependency

Update `vcpkg.json`:

```json
{
  "dependencies": [
    {
      "name": "lua",
      "platform": "windows"
    }
  ]
}
```

### Step 2: Update CMakeLists.txt

Add Lua and new source files:

```cmake
# Find Lua
find_package(Lua REQUIRED)

# Add asset management sources
set(ASSET_SOURCES
    src/assets/AssetManager.cpp
    src/assets/AssetManagerExports.cpp
    src/assets/ModularAssetSystem.cpp
)

# Link Lua
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
```

### Step 3: Build C++ Engine

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### Step 4: Build C# Application

```bash
cd dotnet/AssetInterop
dotnet build -c Release
```

### Step 5: Run the Application

```bash
cd dotnet/AssetInterop/bin/Release/net9.0-windows
./AssetInterop.exe
```

Or with custom asset path:
```bash
./AssetInterop.exe "C:\MyGame\Assets"
```

---

## Usage Examples

### Example 1: Drag and Drop

1. Run the AssetInterop.exe application
2. Drag a PNG file onto the window
3. Select "Texture" in the dialog (or it auto-detects)
4. File is copied to `Assets/Textures/`
5. Lua config is updated automatically

### Example 2: Programmatic Asset Loading

```csharp
using FreshEngine.AssetManagement;

var assetManager = new AssetManagerWrapper("./Assets");
bool success = assetManager.ProcessAsset("C:\\temp\\texture.png", "Texture");
```

### Example 3: FileSystemWatcher Integration

The system automatically monitors the Assets folder:

1. Application is running
2. User copies a file directly to `Assets/Models/`
3. FileSystemWatcher detects the change
4. Status log shows: "File Created: Models/newmodel.obj"

### Example 4: Reading Assets from Lua (C++)

```cpp
#include "assets/AssetManager.h"

auto* assetManager = AssetManager::GetInstance();
assetManager->Initialize("./Assets");
assetManager->LoadAssetsFromLua("./Assets/asset_config.lua");

// Lua tables are now accessible via Lua C API
```

### Example 5: Custom Import Callbacks

```cpp
assetManager->RegisterImportCallback("TextureLoader", 
    [](const std::string& path, const std::string& type) {
        if (type == "Texture") {
            // Load texture into GPU memory
            LoadTextureToGPU(path);
        }
    });
```

---

## API Reference

### C++ AssetManager API

```cpp
class AssetManager {
public:
    static AssetManager* GetInstance();
    
    bool Initialize(const std::string& assetRootPath);
    void Shutdown();
    
    bool ProcessNewAsset(const std::string& sourcePath, 
                         const std::string& assetType);
    
    std::string GetAssetTypeFolder(const std::string& assetType) const;
    
    bool UpdateLuaConfiguration(const std::string& assetPath, 
                                const std::string& assetType,
                                const std::string& assetName);
    
    bool LoadAssetsFromLua(const std::string& configPath);
    
    bool ValidateAsset(const std::string& filePath, 
                       const std::string& assetType) const;
    
    std::vector<std::string> GetSupportedAssetTypes() const;
    
    void RegisterImportCallback(const std::string& callbackName, 
                                AssetImportCallback callback);
};
```

### C# AssetManagerWrapper API

```csharp
public class AssetManagerWrapper : IDisposable
{
    public AssetManagerWrapper(string assetRootPath);
    
    public bool ProcessAsset(string sourcePath, string assetType);
    
    public bool LoadAssetsFromLua(string configPath);
    
    public string AssetRootPath { get; }
    
    public void Dispose();
}
```

### Supported Asset Types

| Type | Extensions | Default Folder |
|------|-----------|----------------|
| Texture | .png, .jpg, .jpeg, .bmp, .tga, .dds | Textures/ |
| Model | .obj, .fbx, .gltf, .glb, .dae | Models/ |
| Script | .lua, .js, .py | Scripts/ |
| Audio | .wav, .mp3, .ogg, .flac | Audio/ |
| Shader | .glsl, .hlsl, .vert, .frag | Shaders/ |
| Config | .json, .xml, .yaml, .yml, .ini | Configs/ |

---

## Advantages of This Architecture

### 1. **P/Invoke vs C++/CLI**

We use **P/Invoke** instead of C++/CLI for several advantages:

#### Advantages of P/Invoke:
✅ **No mixed-mode assemblies** - Simpler deployment  
✅ **Better portability** - Works on .NET Core/.NET 5+  
✅ **Clearer separation** - Managed and native code are separate  
✅ **Standard marshalling** - Well-documented attribute-based marshalling  
✅ **No C++/CLI compiler needed** - Just standard C++ and C#  

#### When C++/CLI is Better:
- Complex object hierarchies requiring seamless interop
- Need to pass C++ objects directly to C#
- Heavy back-and-forth communication

#### Why P/Invoke Works Here:
- Simple C function calls with primitive types and strings
- One-way data flow (C# → C++)
- Clear interface boundaries

### 2. **Lua Integration Benefits**

✅ **Runtime editable** - Change asset metadata without recompiling  
✅ **Human readable** - Easy for artists/designers to modify  
✅ **Flexible** - Can add custom properties without code changes  
✅ **Scriptable** - Complex loading logic can be in Lua  
✅ **Industry standard** - Lua is widely used in game development  

### 3. **FileSystemWatcher Benefits**

✅ **Real-time monitoring** - Detects changes immediately  
✅ **Low overhead** - OS-level notification system  
✅ **Flexible filtering** - Can filter by file type, directory  
✅ **Event-driven** - No polling required  

### 4. **Drag-and-Drop Benefits**

✅ **Intuitive UX** - Natural interaction model  
✅ **Fast workflow** - No file browser needed  
✅ **Multi-file support** - Drop multiple files at once  
✅ **Visual feedback** - UI shows drop zones clearly  

---

## Troubleshooting

### Issue: "DllNotFoundException: FreshVoxelEngine.dll"

**Solution**: Ensure the C++ DLL is in the same directory as the C# executable, or in the system PATH.

```bash
# Copy DLL to output directory
cp build/Release/FreshVoxelEngine.dll dotnet/AssetInterop/bin/Release/net9.0-windows/
```

### Issue: Lua configuration not updating

**Solution**: Check file permissions and ensure the asset_config.lua file is not read-only.

### Issue: FileSystemWatcher not detecting changes

**Solution**: 
- Check that the asset directory exists
- Ensure EnableRaisingEvents is true
- Verify the directory is not on a network drive (performance issues)

---

## Next Steps

1. **Integrate with Engine**: Connect import callbacks to your rendering engine
2. **Add Asset Preview**: Show thumbnails in the UI
3. **Implement Asset Hot-Reload**: Automatically reload assets in the running game
4. **Add Asset Database**: Track asset metadata in SQLite
5. **Create Asset Pipeline**: Add import/export filters for different formats

---

## Conclusion

This asset management system provides a professional, extensible solution for dynamic asset loading in a game engine. The combination of C#, C++, and Lua provides the best of all worlds:

- **C#**: Modern UI, easy file system operations
- **C++**: High-performance core, direct hardware access
- **Lua**: Flexible configuration, runtime scripting

The P/Invoke approach provides clean separation while maintaining excellent performance for this use case.

