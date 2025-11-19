# Resource System Guide

> **Fresh Voxel Engine Resource Management**  
> Last Updated: 2025-11-17  
> Version: 0.2.6

## Overview

The Fresh Voxel Engine Resource System provides a comprehensive solution for managing game assets including textures, meshes, audio files, shaders, and more. It features:

- **Async Loading** - Background thread loading without blocking gameplay
- **Reference Counting** - Automatic memory management with shared pointers
- **Resource Caching** - Avoid duplicate loading of the same resource
- **Hot Reloading** - Update resources during development without restart
- **Graceful Degradation** - Placeholder assets for missing files
- **Thread Safety** - Safe concurrent access from multiple threads

---

## Table of Contents

1. [Quick Start](#quick-start)
2. [Loading Resources](#loading-resources)
3. [Async Loading](#async-loading)
4. [Resource Types](#resource-types)
5. [Error Handling](#error-handling)
6. [Creating Custom Resources](#creating-custom-resources)
7. [Performance Tips](#performance-tips)
8. [API Reference](#api-reference)

---

## Quick Start

### Basic Usage

```cpp
#include "core/ResourceManager.h"

// Get singleton instance
ResourceManager& resMgr = ResourceManager::getInstance();

// Initialize with asset directory
resMgr.initialize("Assets/");

// Load a texture (synchronous)
auto texture = resMgr.load<TextureResource>("textures/player.png");

// Use the resource
if (texture.isValid()) {
    int width = texture->getWidth();
    int height = texture->getHeight();
}

// Resources are automatically unloaded when handles go out of scope
```

### Async Loading

```cpp
// Load asynchronously (non-blocking)
auto futureTexture = resMgr.loadAsync<TextureResource>("textures/large_texture.png");

// Do other work...

// Get result when ready
auto texture = futureTexture.get();
```

---

## Loading Resources

### Synchronous Loading

Best for: Small resources, loading screens, immediate needs

```cpp
// Load and wait until complete
auto mesh = resMgr.load<MeshResource>("models/character.obj");
auto audio = resMgr.load<AudioClipResource>("sounds/explosion.wav");
```

**Pros:**
- Simple to use
- Resource available immediately
- Deterministic behavior

**Cons:**
- Blocks calling thread
- Can cause frame drops
- Not suitable for large files

### Asynchronous Loading

Best for: Large resources, streaming, background loading

```cpp
// Start loading in background
std::vector<std::future<ResourceHandle<TextureResource>>> futures;
futures.push_back(resMgr.loadAsync<TextureResource>("level1/texture1.png"));
futures.push_back(resMgr.loadAsync<TextureResource>("level1/texture2.png"));
futures.push_back(resMgr.loadAsync<TextureResource>("level1/texture3.png"));

// Show loading screen while waiting
while (!allReady(futures)) {
    float progress = resMgr.getLoadingProgress();
    renderLoadingScreen(progress);
}

// Get all loaded resources
for (auto& future : futures) {
    auto texture = future.get();
    // Use texture...
}
```

**Pros:**
- Non-blocking
- Better performance
- Smooth frame rates
- Progress tracking

**Cons:**
- More complex
- Need to handle futures
- Resources not immediately available

---

## Resource Types

### TextureResource

**Supported Formats:** PNG, JPG, JPEG, BMP, TGA

```cpp
auto texture = resMgr.load<TextureResource>("textures/wood.png");

if (texture.isValid()) {
    int width = texture->getWidth();
    int height = texture->getHeight();
    int channels = texture->getChannels();
    const unsigned char* data = texture->getData();
}
```

**Properties:**
- `width` - Image width in pixels
- `height` - Image height in pixels
- `channels` - Number of color channels (3=RGB, 4=RGBA)
- `data` - Raw pixel data

### MeshResource

**Supported Formats:** OBJ, FBX, GLTF, GLB (via tinyobjloader)

```cpp
auto mesh = resMgr.load<MeshResource>("models/tree.obj");

if (mesh.isValid()) {
    const auto& vertices = mesh->getVertices();
    const auto& indices = mesh->getIndices();
    // Upload to GPU...
}
```

**Properties:**
- `vertices` - Vertex data (position, normal, UV)
- `indices` - Index buffer for triangle rendering

### AudioClipResource

**Supported Formats:** WAV, OGG, MP3

```cpp
auto clip = resMgr.load<AudioClipResource>("sounds/ambient.ogg");

if (clip.isValid()) {
    const auto& samples = clip->getSamples();
    int sampleRate = clip->getSampleRate();
    int channels = clip->getChannels();
    // Play audio...
}
```

**Properties:**
- `samples` - PCM audio data
- `sampleRate` - Samples per second (typically 44100 or 48000)
- `channels` - 1 for mono, 2 for stereo

---

## Error Handling

### File Not Found

When a resource file doesn't exist, the system automatically provides a placeholder:

```cpp
auto texture = resMgr.load<TextureResource>("missing_file.png");

// texture.isValid() returns true (uses placeholder)
// Console shows: "ERROR: Texture file not found: missing_file.png"
// Console shows: "Using placeholder texture instead"
```

**Placeholder Resources:**
- **Texture** - Pink/magenta checkerboard (indicates missing texture)
- **Mesh** - Simple cube
- **Audio** - Silent audio clip

### Invalid File Format

```cpp
auto texture = resMgr.load<TextureResource>("document.pdf");

// Console shows: "WARNING: Unsupported texture format: .pdf"
// Console shows: "Supported formats: .png, .jpg, .jpeg, .bmp, .tga"
// Uses placeholder texture
```

### Loading Failures

```cpp
try {
    auto mesh = resMgr.load<MeshResource>("corrupted_file.obj");
} catch (const std::exception& e) {
    std::cerr << "Failed to load: " << e.what() << std::endl;
}
```

### Checking Resource Status

```cpp
auto texture = resMgr.load<TextureResource>("texture.png");

if (!texture.isValid()) {
    // Resource failed to load or is not ready
    // (Should not happen with placeholder system)
}

// Check if specific file is currently loading
if (resMgr.isLoading("texture.png")) {
    // Still loading asynchronously
}
```

---

## Creating Custom Resources

### Step 1: Define Resource Class

```cpp
// MyCustomResource.h
class MyCustomResource : public fresh::Resource
{
public:
    MyCustomResource(const std::string& path);
    
    void load() override;
    void unload() override;
    size_t getMemoryUsage() const override;
    
    // Your custom accessors
    const std::string& getData() const { return data; }
    
private:
    std::string data;
};
```

### Step 2: Implement Loading Logic

```cpp
// MyCustomResource.cpp
MyCustomResource::MyCustomResource(const std::string& path)
{
    this->path = path;
    this->name = std::filesystem::path(path).stem().string();
    this->type = ResourceType::Unknown; // Or define custom type
}

void MyCustomResource::load()
{
    // Check file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "ERROR: File not found: " << path << std::endl;
        return;
    }
    
    try {
        // Load your custom format
        std::ifstream file(path);
        std::getline(file, data);
        
        loaded = true;
        std::cout << "Custom resource loaded: " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR loading: " << e.what() << std::endl;
        loaded = false;
    }
}

void MyCustomResource::unload()
{
    data.clear();
    loaded = false;
}

size_t MyCustomResource::getMemoryUsage() const
{
    return data.size();
}
```

### Step 3: Register Custom Loader

```cpp
// In your initialization code
ResourceManager& resMgr = ResourceManager::getInstance();

resMgr.registerLoader(ResourceType::Unknown, 
    [](const std::string& path) {
        return std::make_shared<MyCustomResource>(path);
    }
);
```

### Step 4: Use Custom Resource

```cpp
auto resource = resMgr.load<MyCustomResource>("data/config.dat");

if (resource.isValid()) {
    std::string data = resource->getData();
}
```

---

## Performance Tips

### 1. Preload Common Resources

```cpp
// At game start, load frequently-used resources
void preloadCommonResources()
{
    ResourceManager& resMgr = ResourceManager::getInstance();
    
    // Start async loading
    std::vector<std::future<ResourceHandle<TextureResource>>> futures;
    futures.push_back(resMgr.loadAsync<TextureResource>("ui/button.png"));
    futures.push_back(resMgr.loadAsync<TextureResource>("ui/panel.png"));
    futures.push_back(resMgr.loadAsync<TextureResource>("ui/font.png"));
    
    // Wait for completion
    for (auto& future : futures) {
        future.wait();
    }
}
```

### 2. Unload Unused Resources

```cpp
// Periodically clean up unused resources
void cleanupResources()
{
    ResourceManager& resMgr = ResourceManager::getInstance();
    
    // Unload resources with ref count = 0
    resMgr.unloadUnused();
    
    std::cout << "Memory freed: " 
              << resMgr.getTotalMemoryUsage() / (1024 * 1024) 
              << " MB" << std::endl;
}
```

### 3. Use Resource Caching

The system automatically caches resources. Loading the same file multiple times returns the cached version:

```cpp
// First load - reads from disk
auto tex1 = resMgr.load<TextureResource>("texture.png");

// Second load - returns cached resource (instant)
auto tex2 = resMgr.load<TextureResource>("texture.png");

// tex1 and tex2 point to the same resource
```

### 4. Monitor Memory Usage

```cpp
void checkResourceMemory()
{
    ResourceManager& resMgr = ResourceManager::getInstance();
    
    size_t memoryMB = resMgr.getTotalMemoryUsage() / (1024 * 1024);
    size_t resourceCount = resMgr.getLoadedResourceCount();
    
    std::cout << "Resources: " << resourceCount << std::endl;
    std::cout << "Memory: " << memoryMB << " MB" << std::endl;
    
    if (memoryMB > 1024) {
        std::cout << "WARNING: High memory usage!" << std::endl;
        resMgr.unloadUnused();
    }
}
```

### 5. Level Streaming

```cpp
void loadLevel(int levelNum)
{
    ResourceManager& resMgr = ResourceManager::getInstance();
    
    // Unload previous level resources
    resMgr.unloadType(ResourceType::Texture);
    resMgr.unloadType(ResourceType::Mesh);
    
    // Load new level resources asynchronously
    std::string levelPath = "levels/level" + std::to_string(levelNum) + "/";
    
    std::vector<std::future<ResourceHandle<TextureResource>>> futures;
    futures.push_back(resMgr.loadAsync<TextureResource>(levelPath + "terrain.png"));
    futures.push_back(resMgr.loadAsync<TextureResource>(levelPath + "props.png"));
    
    // Show loading screen...
}
```

---

## API Reference

### ResourceManager

#### Singleton Access

```cpp
static ResourceManager& getInstance();
```

Get the singleton instance of the resource manager.

#### Initialization

```cpp
void initialize(const std::string& assetDirectory);
void shutdown();
```

Initialize/shutdown the resource manager. Call `initialize()` once at startup and `shutdown()` before exit.

#### Loading

```cpp
template <typename T>
ResourceHandle<T> load(const std::string& path);
```

Load a resource synchronously. Blocks until resource is loaded.

```cpp
template <typename T>
std::future<ResourceHandle<T>> loadAsync(const std::string& path);
```

Load a resource asynchronously in background thread. Returns future for result.

#### Unloading

```cpp
void unload(const std::string& path);
void unloadType(ResourceType type);
void unloadUnused();
```

Unload specific resource, all resources of a type, or all unused resources (ref count = 0).

#### Hot Reloading

```cpp
void hotReload(const std::string& path);
```

Reload a resource from disk during development. Useful for shaders and scripts.

#### Querying

```cpp
bool exists(const std::string& path) const;
bool isLoading(const std::string& path) const;
float getLoadingProgress() const;
size_t getTotalMemoryUsage() const;
size_t getLoadedResourceCount() const;
```

Query resource status and statistics.

#### Registration

```cpp
void registerLoader(
    ResourceType type,
    std::function<std::shared_ptr<Resource>(const std::string&)> loader
);
```

Register a custom resource loader function.

#### Utilities

```cpp
void scanDirectory(const std::string& directory, bool recursive = true);
std::vector<std::string> getResourcesOfType(ResourceType type) const;
void printStats() const;
```

Scan directories for assets, query by type, print statistics.

### ResourceHandle<T>

```cpp
T* get() const;                   // Get raw pointer
T* operator->() const;            // Arrow operator
bool isValid() const;             // Check if resource loaded
```

Smart handle for resources with automatic reference counting.

---

## Examples

### Example 1: Loading Screen

```cpp
class LoadingScreen
{
public:
    void loadLevel(const std::string& levelName)
    {
        ResourceManager& resMgr = ResourceManager::getInstance();
        
        // Get list of files to load
        std::vector<std::string> files = getLevelFiles(levelName);
        
        // Start async loading
        for (const auto& file : files) {
            if (file.ends_with(".png")) {
                resMgr.loadAsync<TextureResource>(file);
            } else if (file.ends_with(".obj")) {
                resMgr.loadAsync<MeshResource>(file);
            } else if (file.ends_with(".ogg")) {
                resMgr.loadAsync<AudioClipResource>(file);
            }
        }
        
        // Render progress
        while (resMgr.getLoadingProgress() < 1.0f) {
            renderProgressBar(resMgr.getLoadingProgress());
            
            // Sleep to avoid busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
    
private:
    void renderProgressBar(float progress)
    {
        std::cout << "Loading: " << int(progress * 100) << "%" << std::endl;
    }
};
```

### Example 2: Hot Reload Development

```cpp
class DevelopmentTools
{
public:
    void watchShaderChanges()
    {
        ResourceManager& resMgr = ResourceManager::getInstance();
        
        while (developmentMode) {
            // Check if shader file modified
            if (fileModified("shaders/main.vert")) {
                std::cout << "Shader changed, reloading..." << std::endl;
                resMgr.hotReload("shaders/main.vert");
                rebuildPipeline();
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};
```

---

## Troubleshooting

### Problem: Resource Loading is Slow

**Solution:**
- Use `loadAsync()` for large files
- Preload common resources at startup
- Check disk I/O performance
- Enable resource caching (automatic)

### Problem: High Memory Usage

**Solution:**
- Call `unloadUnused()` periodically
- Unload level-specific resources when changing levels
- Monitor with `getTotalMemoryUsage()`
- Use smaller texture/audio formats

### Problem: File Not Found Errors

**Solution:**
- Check asset directory path in `initialize()`
- Verify file exists in correct location
- Check file path separators (use `/` not `\`)
- Review console error messages

### Problem: Placeholder Resources Showing

**Solution:**
- Check file path is correct
- Verify file format is supported
- Check file permissions
- Look for console error messages
- Verify file is not corrupted

---

## Best Practices

1. **Always initialize at startup**
   ```cpp
   ResourceManager::getInstance().initialize("Assets/");
   ```

2. **Use async loading for large files**
   ```cpp
   auto future = resMgr.loadAsync<TextureResource>("huge_texture.png");
   ```

3. **Check resource validity**
   ```cpp
   if (texture.isValid()) {
       // Use texture
   }
   ```

4. **Clean up when done**
   ```cpp
   resMgr.unloadType(ResourceType::Texture);
   ```

5. **Monitor performance**
   ```cpp
   resMgr.printStats();
   ```

6. **Use descriptive paths**
   ```cpp
   // Good
   "textures/characters/player_diffuse.png"
   
   // Bad
   "img1.png"
   ```

---

## See Also

- [AUDIO_FORMAT_SUPPORT.md](AUDIO_FORMAT_SUPPORT.md) - Audio file formats
- [ASSET_PACK_GUIDE.md](ASSET_PACK_GUIDE.md) - Asset pack system
- [BUILD.md](BUILD.md) - Building the engine

---

**Status**: Complete ✅  
**Version**: 1.0  
**Last Updated**: 2025-11-17  
**Maintainer**: Fresh Voxel Engine Team
