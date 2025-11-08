// Example demonstrating Asset Management System Integration
// This file shows how to use the complete system with C++, C#, and Lua

// ============================================================================
// C++ Example - Using AssetManager
// ============================================================================

#include "assets/AssetManager.h"
#include <iostream>

void CppExample() {
    // Get singleton instance
    auto* assetManager = fresh::AssetManager::GetInstance();
    
    // Initialize with asset root path
    if (!assetManager->Initialize("./Assets")) {
        std::cerr << "Failed to initialize AssetManager" << std::endl;
        return;
    }
    
    // Load assets from Lua configuration
    if (!assetManager->LoadAssetsFromLua("./Assets/asset_config.lua")) {
        std::cerr << "Failed to load Lua configuration" << std::endl;
        return;
    }
    
    // Register callback for when new assets are imported
    assetManager->RegisterImportCallback("TextureLoader",
        [](const std::string& path, const std::string& type) {
            std::cout << "New asset imported: " << path 
                      << " (Type: " << type << ")" << std::endl;
            
            if (type == "Texture") {
                // Example: Load the texture into GPU memory
                // LoadTextureToGPU(path);
            }
        });
    
    // Process a new asset (normally called from C# via P/Invoke)
    bool success = assetManager->ProcessNewAsset(
        "C:/temp/stone_texture.png",
        "Texture"
    );
    
    if (success) {
        std::cout << "Asset imported successfully!" << std::endl;
    }
    
    // Cleanup
    assetManager->Shutdown();
}

// ============================================================================
// Lua Configuration Example
// ============================================================================

/*
-- asset_config.lua
-- This file is automatically updated when assets are imported

Assets = {
    Textures = {
        {"grass", "Textures/grass.png"},
        {"stone", "Textures/stone.png"},
        {"dirt", "Textures/dirt.png"}
    },
    
    Models = {
        {"tree", "Models/tree.obj"},
        {"rock", "Models/rock.obj"}
    },
    
    Scripts = {
        {"init", "Scripts/init.lua"},
        {"gamelogic", "Scripts/gamelogic.lua"}
    },
    
    Audio = {
        {"theme", "Audio/theme.mp3"},
        {"footsteps", "Audio/footsteps.wav"}
    },
    
    Shaders = {
        {"basic", "Shaders/basic.glsl"},
        {"voxel", "Shaders/voxel.hlsl"}
    },
    
    Configs = {
        {"settings", "Configs/settings.json"}
    }
}

return Assets
*/

// ============================================================================
// Reading Lua Configuration in C++
// ============================================================================

#ifdef FRESH_LUA_AVAILABLE
#include <lua.hpp>

void ReadLuaConfigExample() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    // Load the configuration file
    if (luaL_dofile(L, "./Assets/asset_config.lua") != LUA_OK) {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return;
    }
    
    // Access the Assets table
    lua_getglobal(L, "Assets");
    if (!lua_istable(L, -1)) {
        std::cerr << "Assets is not a table" << std::endl;
        lua_close(L);
        return;
    }
    
    // Access Textures subtable
    lua_getfield(L, -1, "Textures");
    if (lua_istable(L, -1)) {
        size_t numTextures = lua_rawlen(L, -1);
        std::cout << "Found " << numTextures << " textures:" << std::endl;
        
        // Iterate through texture entries
        for (size_t i = 1; i <= numTextures; i++) {
            lua_rawgeti(L, -1, i);  // Get the i-th entry
            
            if (lua_istable(L, -1)) {
                // Get name (first element)
                lua_rawgeti(L, -1, 1);
                const char* name = lua_tostring(L, -1);
                lua_pop(L, 1);
                
                // Get path (second element)
                lua_rawgeti(L, -1, 2);
                const char* path = lua_tostring(L, -1);
                lua_pop(L, 1);
                
                std::cout << "  " << name << " -> " << path << std::endl;
            }
            
            lua_pop(L, 1);  // Pop the entry
        }
    }
    lua_pop(L, 1);  // Pop Textures
    
    lua_pop(L, 1);  // Pop Assets
    lua_close(L);
}
#else
void ReadLuaConfigExample() {
    std::cout << "Lua not available - skipping Lua config reading example" << std::endl;
}
#endif // FRESH_LUA_AVAILABLE

// ============================================================================
// C# Example - Asset Management Form with Drag-and-Drop
// ============================================================================

/*
using System;
using System.Windows.Forms;
using FreshEngine.AssetManagement;

public class ExampleUsage
{
    public static void Main()
    {
        Application.EnableVisualStyles();
        Application.SetHighDpiMode(HighDpiMode.SystemAware);
        
        // Create and show the asset management form
        using var form = new AssetManagementForm("./Assets");
        Application.Run(form);
    }
}
*/

// ============================================================================
// C# Example - Programmatic Asset Import
// ============================================================================

/*
using FreshEngine.AssetManagement;

public class ProgrammaticImportExample
{
    public void ImportAssets()
    {
        // Create asset manager wrapper
        using var assetManager = new AssetManagerWrapper("./GameAssets");
        
        // Import a texture
        bool textureSuccess = assetManager.ProcessAsset(
            @"C:\Downloads\grass_texture.png",
            "Texture"
        );
        
        if (textureSuccess)
        {
            Console.WriteLine("Texture imported successfully!");
        }
        
        // Import a model
        bool modelSuccess = assetManager.ProcessAsset(
            @"C:\Downloads\tree_model.obj",
            "Model"
        );
        
        if (modelSuccess)
        {
            Console.WriteLine("Model imported successfully!");
        }
        
        // Load the updated Lua configuration
        assetManager.LoadAssetsFromLua("./GameAssets/asset_config.lua");
    }
}
*/

// ============================================================================
// C# Example - Custom Drag-and-Drop Integration
// ============================================================================

/*
using System;
using System.IO;
using System.Windows.Forms;
using FreshEngine.AssetManagement;

public class GameEditorForm : Form
{
    private AssetManagerWrapper assetManager;
    
    public GameEditorForm()
    {
        // Initialize asset manager
        assetManager = new AssetManagerWrapper("./EditorAssets");
        
        // Setup drag-and-drop on main form
        this.AllowDrop = true;
        this.DragEnter += OnDragEnter;
        this.DragDrop += OnDragDrop;
        
        Text = "Game Editor - Drop Assets Here";
        Size = new System.Drawing.Size(800, 600);
    }
    
    private void OnDragEnter(object sender, DragEventArgs e)
    {
        if (e.Data.GetDataPresent(DataFormats.FileDrop))
        {
            e.Effect = DragDropEffects.Copy;
        }
    }
    
    private void OnDragDrop(object sender, DragEventArgs e)
    {
        string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
        
        foreach (string file in files)
        {
            // Show asset type dialog
            string fileName = Path.GetFileName(file);
            string? assetType = AssetTypeDialog.ShowDialog(fileName);
            
            if (!string.IsNullOrEmpty(assetType))
            {
                // Import the asset
                if (assetManager.ProcessAsset(file, assetType))
                {
                    MessageBox.Show(
                        $"Successfully imported: {fileName}",
                        "Asset Import",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Information
                    );
                    
                    // Reload in your game engine
                    ReloadAsset(file, assetType);
                }
            }
        }
    }
    
    private void ReloadAsset(string path, string type)
    {
        // Your custom code to reload the asset in the game engine
        Console.WriteLine($"Reloading {type}: {path}");
    }
    
    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            assetManager?.Dispose();
        }
        base.Dispose(disposing);
    }
}
*/

// ============================================================================
// C# Example - FileSystemWatcher Integration
// ============================================================================

/*
using System;
using System.IO;
using FreshEngine.AssetManagement;

public class AssetMonitor
{
    private FileSystemWatcher watcher;
    private AssetManagerWrapper assetManager;
    
    public AssetMonitor(string assetPath)
    {
        assetManager = new AssetManagerWrapper(assetPath);
        
        watcher = new FileSystemWatcher(assetPath)
        {
            NotifyFilter = NotifyFilters.FileName | NotifyFilters.LastWrite,
            IncludeSubdirectories = true,
            EnableRaisingEvents = true
        };
        
        watcher.Created += OnAssetCreated;
        watcher.Changed += OnAssetChanged;
        watcher.Deleted += OnAssetDeleted;
    }
    
    private void OnAssetCreated(object sender, FileSystemEventArgs e)
    {
        Console.WriteLine($"New asset detected: {e.FullPath}");
        
        // Automatically import new assets
        string? assetType = DetectAssetType(e.FullPath);
        if (assetType != null)
        {
            assetManager.ProcessAsset(e.FullPath, assetType);
        }
    }
    
    private void OnAssetChanged(object sender, FileSystemEventArgs e)
    {
        Console.WriteLine($"Asset modified: {e.FullPath}");
        // Trigger hot-reload in your engine
    }
    
    private void OnAssetDeleted(object sender, FileSystemEventArgs e)
    {
        Console.WriteLine($"Asset deleted: {e.FullPath}");
        // Update internal references
    }
    
    private string? DetectAssetType(string filePath)
    {
        string ext = Path.GetExtension(filePath).ToLower();
        return ext switch
        {
            ".png" or ".jpg" or ".jpeg" => "Texture",
            ".obj" or ".fbx" or ".gltf" => "Model",
            ".lua" or ".js" => "Script",
            ".wav" or ".mp3" or ".ogg" => "Audio",
            ".glsl" or ".hlsl" => "Shader",
            _ => null
        };
    }
    
    public void Dispose()
    {
        watcher?.Dispose();
        assetManager?.Dispose();
    }
}
*/

// ============================================================================
// P/Invoke vs C++/CLI Comparison
// ============================================================================

/*
ADVANTAGES OF P/INVOKE (Used in this project):
=============================================

1. Simpler Deployment
   - No mixed-mode assemblies
   - Native and managed DLLs are separate
   - Easier to debug and maintain

2. Better .NET Core/.NET 5+ Support
   - C++/CLI has limited .NET Core support
   - P/Invoke works on all .NET platforms

3. Clear Separation
   - Explicit marshalling with attributes
   - Clear interface boundaries
   - Standard C calling convention

4. No Special Compiler
   - Just use standard C++ compiler
   - No need for /clr flag

5. Better for Simple Interop
   - Function calls with primitive types
   - String marshalling is straightforward
   - One-way data flow (C# -> C++)

WHEN TO USE C++/CLI:
===================

1. Complex Object Hierarchies
   - Need to pass C++ objects to C#
   - Requires wrapper classes

2. Bidirectional Communication
   - Heavy C++ to C# callbacks
   - Complex data structures

3. Legacy Codebase
   - Already using C++/CLI
   - Migration would be costly

EXAMPLE P/INVOKE (This Project):
================================

// C++ (Exports.h)
extern "C" {
    __declspec(dllexport) bool ProcessAsset(const char* path, const char* type);
}

// C# (Wrapper.cs)
[DllImport("FreshEngine.dll")]
[return: MarshalAs(UnmanagedType.I1)]
public static extern bool ProcessAsset(
    [MarshalAs(UnmanagedType.LPStr)] string path,
    [MarshalAs(UnmanagedType.LPStr)] string type
);

EXAMPLE C++/CLI (Alternative):
==============================

// C++/CLI (Wrapper.cpp)
public ref class AssetInteropWrapper
{
public:
    void ProcessAsset(System::String^ path, System::String^ type)
    {
        std::string nativePath = msclr::interop::marshal_as<std::string>(path);
        std::string nativeType = msclr::interop::marshal_as<std::string>(type);
        NativeAssetManager::ProcessAsset(nativePath, nativeType);
    }
};

// C# (Usage.cs)
var wrapper = new AssetInteropWrapper();
wrapper.ProcessAsset("texture.png", "Texture");
*/

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Fresh Engine Asset Management Examples" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "C++ Example:" << std::endl;
    std::cout << "------------" << std::endl;
    CppExample();
    std::cout << std::endl;
    std::cout << "Lua Reading Example:" << std::endl;
    std::cout << "-------------------" << std::endl;
    ReadLuaConfigExample();
    std::cout << std::endl;
    std::cout << "For C# examples, see the dotnet/AssetInterop project" << std::endl;
    return 0;
}
