# Fresh Engine Asset Management System

A complete asset management solution for the Fresh Voxel Engine featuring drag-and-drop import, automatic file monitoring, and Lua-based configuration.

## Features

- 🎯 **Drag-and-Drop Import**: Drop files directly onto the application window
- 👁️ **File System Monitoring**: Automatic detection of changes in asset folders using FileSystemWatcher
- 🎨 **Smart Type Detection**: Automatic asset type detection based on file extension
- 📝 **Lua Configuration**: Runtime-editable asset configuration with automatic updates
- 🔗 **C++/C# Interop**: Seamless integration between managed C# UI and native C++ core
- 📂 **Organized Structure**: Automatic organization into type-specific folders

## Quick Start

### Prerequisites

- .NET 9.0 SDK
- Fresh Voxel Engine C++ DLL (FreshVoxelEngine.dll)
- Windows (for WinForms UI)

### Building

```bash
cd dotnet/AssetInterop
dotnet build -c Release
```

### Running

```bash
# Default asset location (./Assets)
dotnet run

# Custom asset location
dotnet run -- "C:\MyGame\Assets"
```

## Usage

### Drag and Drop

1. Launch the application
2. Drag any supported asset file onto the drop zone
3. Confirm or change the detected asset type
4. File is automatically copied to the correct folder and registered in Lua config

### Browse for Files

1. Click the "Browse for Files..." button
2. Select one or more asset files
3. Confirm the asset type for each file

### Monitor Folder

Enable the "Monitor asset folder for changes" checkbox to automatically detect:
- New files added to the asset directory
- Modified files
- Deleted files
- Renamed files

## Supported Asset Types

| Type | Extensions |
|------|-----------|
| **Texture** | .png, .jpg, .jpeg, .bmp, .tga, .dds |
| **Model** | .obj, .fbx, .gltf, .glb, .dae |
| **Script** | .lua, .js, .py |
| **Audio** | .wav, .mp3, .ogg, .flac |
| **Shader** | .glsl, .hlsl, .vert, .frag, .vs, .fs |
| **Config** | .json, .xml, .yaml, .yml, .ini |

## Architecture

```
C# UI (WinForms)
    ↕ P/Invoke
C API Exports
    ↕
C++ AssetManager
    ↕ Lua C API
Lua Configuration
```

## Project Structure

```
dotnet/AssetInterop/
├── AssetInterop.csproj          # Project file
├── Program.cs                    # Application entry point
├── AssetManagerWrapper.cs        # P/Invoke wrapper for native code
├── AssetManagementForm.cs        # Main UI with drag-and-drop
└── AssetTypeDialog.cs            # Asset type selection dialog
```

## Integration with Your Project

### Using in C# Code

```csharp
using FreshEngine.AssetManagement;

// Create asset manager
var assetManager = new AssetManagerWrapper("./Assets");

// Process an asset
bool success = assetManager.ProcessAsset(
    @"C:\temp\texture.png", 
    "Texture"
);

// Load Lua configuration
assetManager.LoadAssetsFromLua("./Assets/asset_config.lua");
```

### Using in C++ Code

```cpp
#include "assets/AssetManager.h"

// Get singleton instance
auto* assetManager = fresh::AssetManager::GetInstance();

// Initialize
assetManager->Initialize("./Assets");

// Register callback for new assets
assetManager->RegisterImportCallback("MySystem", 
    [](const std::string& path, const std::string& type) {
        std::cout << "New asset: " << path << " (" << type << ")" << std::endl;
    });

// Load Lua configuration
assetManager->LoadAssetsFromLua("./Assets/asset_config.lua");
```

## Lua Configuration Format

The system automatically updates `Assets/asset_config.lua`:

```lua
Assets = {
    Textures = {
        {"grass", "Textures/grass.png"},
        {"stone", "Textures/stone.png"}
    },
    Models = {
        {"tree", "Models/tree.obj"}
    },
    Scripts = {
        {"init", "Scripts/init.lua"}
    },
    -- ...
}

return Assets
```

## P/Invoke vs C++/CLI

This project uses **P/Invoke** instead of C++/CLI for several advantages:

### Advantages of P/Invoke
✅ No mixed-mode assemblies - simpler deployment  
✅ Works with .NET 5+ and .NET Core  
✅ Clear separation between managed and native code  
✅ Standard marshalling with attributes  
✅ No C++/CLI compiler needed  

### When to Use C++/CLI
- Complex object hierarchies
- Need to pass C++ objects directly to C#
- Heavy bidirectional communication

### Why P/Invoke Works Here
- Simple C function calls
- One-way data flow (C# → C++)
- Clear interface boundaries
- String and primitive marshalling only

## API Reference

### AssetManagerWrapper

```csharp
public class AssetManagerWrapper : IDisposable
{
    // Constructor
    public AssetManagerWrapper(string assetRootPath);
    
    // Process a new asset
    public bool ProcessAsset(string sourcePath, string assetType);
    
    // Load assets from Lua config
    public bool LoadAssetsFromLua(string configPath);
    
    // Get asset root path
    public string AssetRootPath { get; }
    
    // IDisposable
    public void Dispose();
}
```

### AssetTypeDialog

```csharp
public class AssetTypeDialog : Form
{
    // Show dialog and get selected type
    public static string? ShowDialog(string fileName);
    
    // Get selected type after dialog closes
    public string SelectedAssetType { get; }
}
```

### AssetManagementForm

```csharp
public class AssetManagementForm : Form
{
    // Constructor
    public AssetManagementForm(string assetRootPath);
}
```

## Examples

### Example 1: Custom Asset Import Handler

```csharp
class CustomAssetImporter
{
    private AssetManagerWrapper assetManager;
    
    public CustomAssetImporter()
    {
        assetManager = new AssetManagerWrapper("./MyAssets");
    }
    
    public void ImportTexture(string path)
    {
        if (assetManager.ProcessAsset(path, "Texture"))
        {
            Console.WriteLine($"Texture imported: {path}");
            // Reload texture in rendering engine
            ReloadTexture(path);
        }
    }
}
```

### Example 2: Batch Import

```csharp
class BatchImporter
{
    public static void ImportFolder(string folderPath)
    {
        var assetManager = new AssetManagerWrapper("./Assets");
        
        foreach (var file in Directory.GetFiles(folderPath))
        {
            string ext = Path.GetExtension(file).ToLower();
            string? type = DetectType(ext);
            
            if (type != null)
            {
                assetManager.ProcessAsset(file, type);
            }
        }
    }
}
```

### Example 3: Custom UI Integration

```csharp
public class GameEditorForm : Form
{
    private AssetManagerWrapper assetManager;
    
    public GameEditorForm()
    {
        assetManager = new AssetManagerWrapper("./GameAssets");
        
        // Enable drag-and-drop on main form
        this.AllowDrop = true;
        this.DragEnter += OnDragEnter;
        this.DragDrop += OnDragDrop;
    }
    
    private void OnDragDrop(object sender, DragEventArgs e)
    {
        var files = (string[])e.Data.GetData(DataFormats.FileDrop);
        foreach (var file in files)
        {
            // Auto-detect and import
            string? type = AssetTypeDialog.ShowDialog(Path.GetFileName(file));
            if (type != null)
            {
                assetManager.ProcessAsset(file, type);
            }
        }
    }
}
```

## Troubleshooting

### DLL Not Found

Ensure `FreshVoxelEngine.dll` is in one of these locations:
- Same directory as the executable
- In system PATH
- In the application's working directory

### Lua Configuration Not Updating

- Check file permissions (not read-only)
- Verify the asset root path exists
- Check console output for error messages

### File System Watcher Not Working

- Ensure the directory exists
- Check that EnableRaisingEvents is true
- Avoid network drives (performance issues)

## See Also

- [ASSET_MANAGEMENT_GUIDE.md](../../ASSET_MANAGEMENT_GUIDE.md) - Complete integration guide
- [C++ AssetManager API](../../include/assets/AssetManager.h)
- [Lua Integration](../../include/scripting/lua/ScriptingEngine.h)

## License

MIT License - See LICENSE file in repository root

## Contributing

Contributions welcome! Please follow the coding standards in CONTRIBUTING.md

