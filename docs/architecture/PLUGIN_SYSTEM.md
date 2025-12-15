# World Generator Plugin System

## Overview

The Fresh Voxel Engine adopts an **editor-first architecture** where world generation is treated as a **pluggable feature**, not the main focus of the engine. This design philosophy reflects that Fresh is primarily a **game editor** for building games from scratch, with world generation being one of many tools available to developers.

## Philosophy: Editor-First, Not Generation-First

Fresh is a **game development platform** built around a professional editor, similar to Unreal Engine or Unity. The editor is the primary application, and world generation modules are plugins that enhance the editor's capabilities:

- ✅ **Editor First**: The editor starts immediately with full UI and tools
- ✅ **Scenes, Not Worlds**: Users create and manage "scenes" (which may contain voxel worlds)
- ✅ **Pluggable Generation**: World generators are optional, extensible plugins
- ✅ **Multiple Scene Types**: 3D voxel, 2D platformer, 2D top-down, empty scenes, custom types

## Plugin Interface

### IWorldGenerator Interface

All world generators implement the `IWorldGenerator` interface:

```cpp
#include "generation/IWorldGenerator.h"

class CustomGenerator : public fresh::IWorldGenerator
{
public:
    std::string getName() const override 
    { 
        return "My Custom Generator"; 
    }
    
    std::string getDescription() const override 
    { 
        return "Generates amazing custom terrain"; 
    }
    
    void generateChunk(fresh::Chunk* chunk) override
    {
        // Your generation logic here
        // Fill chunk with voxels based on your algorithm
    }
    
    void setSeed(int seed) override 
    { 
        m_seed = seed; 
    }
    
    int getHeight(int x, int z) const override
    {
        // Return terrain height at position (optional)
        return calculateHeight(x, z);
    }
    
    bool is3DGenerator() const override { return true; }
    bool is2DGenerator() const override { return false; }

private:
    int m_seed = 0;
    
    int calculateHeight(int x, int z) const
    {
        // Your height calculation
        return 64;
    }
};
```

## Registering Custom Generators

### Method 1: At Engine Startup

Register your generator in the Engine initialization or in a custom plugin loader:

```cpp
#include "generation/IWorldGenerator.h"

// In your plugin or engine initialization code:
fresh::WorldGeneratorFactory::registerGenerator("My Generator", []() {
    return std::make_unique<MyCustomGenerator>();
});
```

### Method 2: Plugin Module (Future Enhancement)

Future versions will support dynamically loaded plugin DLLs:

```cpp
// In your plugin DLL entry point:
extern "C" __declspec(dllexport) void RegisterGenerators()
{
    fresh::WorldGeneratorFactory::registerGenerator("Plugin Generator", []() {
        return std::make_unique<PluginGenerator>();
    });
}
```

## Built-In Generators

Fresh ships with several built-in generators that serve as examples and cover common use cases:

| Generator Name | Type | Description |
|---------------|------|-------------|
| **3D Terrain** | 3D | Perlin noise-based terrain with biomes, caves, and natural features |
| **2D Platformer** | 2D | Side-scrolling terrain (Terraria-style) with layers and caves |
| **2D Top-Down** | 2D | Top-down world (Zelda-style) with terrain and obstacles |
| **Empty** | Both | Blank world with no terrain (clean canvas for manual building) |

## Advanced Generator Features

### Asset Placement

Override `generateChunkWithAssets()` to place complex assets:

```cpp
void generateChunkWithAssets(fresh::Chunk* chunk, fresh::VoxelWorld* world) override
{
    // Generate base terrain
    generateChunk(chunk);
    
    // Place trees, structures, etc.
    if (world) {
        placeTree(chunk, world, 8, getHeight(8, 8), 8);
        placeRuins(chunk, world);
    }
}
```

### Heightmap Access

Implement `getHeight()` for terrain queries:

```cpp
int getHeight(int x, int z) const override
{
    // Provide terrain surface height for editor tools
    float noise = m_noiseGenerator.get(x * 0.01f, z * 0.01f);
    return static_cast<int>(64.0f + noise * 32.0f);
}
```

### Multi-Biome Support

```cpp
enum class Biome { Plains, Desert, Forest, Mountains, Ocean };

Biome getBiome(int x, int z) const
{
    float temperature = m_temperatureNoise.get(x * 0.001f, z * 0.001f);
    float rainfall = m_rainfallNoise.get(x * 0.001f, z * 0.001f);
    
    if (rainfall < 0.3f) return Biome::Desert;
    if (temperature > 0.7f) return Biome::Plains;
    if (temperature < 0.3f) return Biome::Mountains;
    return Biome::Forest;
}
```

## Generator Categories

Generators can be organized by type for the editor UI:

- **Terrain Generators**: Natural landscapes (3D Terrain, Heightmap-based)
- **Structure Generators**: Cities, dungeons, ruins
- **Procedural Generators**: Algorithmic patterns
- **2D Generators**: Platformer and top-down worlds
- **Empty/Manual**: Blank canvas for manual building

## Using Generators from the Editor

1. **File > New Scene** opens the scene creation dialog
2. Select scene dimension (3D or 2D)
3. Choose generator from dropdown (future enhancement)
4. Configure seed and options
5. Click "Create" to generate the scene

## Best Practices

### 1. Make Generators Deterministic

Always use the seed for random generation:

```cpp
void setSeed(int seed) override
{
    m_seed = seed;
    m_noiseGenerator.setSeed(seed);
    m_random.seed(seed);
}
```

### 2. Support Incremental Generation

Generate chunks independently - don't rely on other chunks being loaded:

```cpp
void generateChunk(Chunk* chunk) override
{
    ChunkPos pos = chunk->getPosition();
    
    // Use world position, not chunk-relative position
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            int worldX = pos.x * 16 + x;
            int worldZ = pos.z * 16 + z;
            int height = getHeight(worldX, worldZ);
            
            // Generate based on world coordinates
            for (int y = 0; y < height; ++y) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
        }
    }
}
```

### 3. Optimize for Performance

Chunk generation happens on-demand during gameplay:

```cpp
// Cache expensive calculations
void generateChunk(Chunk* chunk) override
{
    auto heightmap = precomputeHeightmap(chunk);
    
    // Use cached data for fast generation
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            int height = heightmap[x][z];
            fillColumn(chunk, x, z, height);
        }
    }
}
```

### 4. Provide Useful Metadata

```cpp
std::string getName() const override 
{ 
    return "Fantasy Terrain"; 
}

std::string getDescription() const override 
{ 
    return "Medieval fantasy terrain with castles, villages, and magical forests.\n"
           "Features: Biomes, structures, dungeons, ore distribution.\n"
           "Best for: RPG and adventure games."; 
}
```

## Future Enhancements

The plugin system will be extended to support:

- **Dynamic DLL Loading**: Load generators from external DLLs
- **Lua-Based Generators**: Write generators in Lua scripts
- **Visual Node Editors**: Create generators using visual scripting
- **Generator Marketplace**: Share and download generators from community
- **Generator Presets**: Save and load generator configurations
- **Real-Time Preview**: Preview generation before creating scene

## Example: Complete Custom Generator

```cpp
#include "generation/IWorldGenerator.h"
#include "generation/NoiseGenerator.h"

class IslandGenerator : public fresh::IWorldGenerator
{
public:
    IslandGenerator() : m_seed(0)
    {
        m_noiseGenerator.setSeed(m_seed);
    }
    
    std::string getName() const override { return "Island World"; }
    
    std::string getDescription() const override 
    { 
        return "Generates scattered islands surrounded by ocean"; 
    }
    
    void setSeed(int seed) override
    {
        m_seed = seed;
        m_noiseGenerator.setSeed(seed);
    }
    
    void generateChunk(fresh::Chunk* chunk) override
    {
        fresh::ChunkPos pos = chunk->getPosition();
        
        for (int x = 0; x < 16; ++x) {
            for (int z = 0; z < 16; ++z) {
                int worldX = pos.x * 16 + x;
                int worldZ = pos.z * 16 + z;
                
                // Distance from origin
                float dist = std::sqrt(worldX * worldX + worldZ * worldZ);
                float islandFactor = 1.0f - (dist / 1000.0f);
                
                // Generate height with island falloff
                int height = getHeight(worldX, worldZ);
                if (islandFactor > 0.0f) {
                    height = static_cast<int>(height * islandFactor);
                }
                
                // Fill terrain
                for (int y = 0; y < 256; ++y) {
                    fresh::VoxelType type;
                    if (y < 32) {
                        type = fresh::VoxelType::Stone; // Bedrock
                    } else if (y < height - 3) {
                        type = fresh::VoxelType::Stone;
                    } else if (y < height) {
                        type = fresh::VoxelType::Dirt;
                    } else if (y == height && height > 32) {
                        type = fresh::VoxelType::Grass;
                    } else if (y <= 32) {
                        type = fresh::VoxelType::Water;
                    } else {
                        type = fresh::VoxelType::Air;
                    }
                    chunk->setVoxel(x, y, z, type);
                }
            }
        }
    }
    
    int getHeight(int x, int z) const override
    {
        float noise = m_noiseGenerator.get(x * 0.01f, z * 0.01f);
        return static_cast<int>(40.0f + noise * 20.0f);
    }
    
    bool is3DGenerator() const override { return true; }
    bool is2DGenerator() const override { return false; }

private:
    int m_seed;
    fresh::NoiseGenerator m_noiseGenerator;
};

// Register the generator
void registerIslandGenerator()
{
    fresh::WorldGeneratorFactory::registerGenerator("Island World", []() {
        return std::make_unique<IslandGenerator>();
    });
}
```

## Summary

The plugin system treats world generation as an **optional, extensible feature** of the editor:

- ✅ Generators are plugins, not core features
- ✅ Easy to create custom generators
- ✅ Built-in generators serve as examples
- ✅ Editor remains the primary focus
- ✅ Supports any generation strategy (noise, L-systems, cellular automata, etc.)

This design makes Fresh a true **game editor platform** where world generation is just one of many tools available to game developers.
