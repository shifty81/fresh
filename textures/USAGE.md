# Using Placeholder Textures in Fresh Voxel Engine

This guide explains how to use the procedurally generated placeholder textures in your Fresh Voxel Engine project.

## Quick Start

### 1. Initialize Texture Manager

First, create default textures when your engine starts:

```cpp
#include "renderer/TextureManager.h"

// In your initialization code
auto& texManager = fresh::TextureManager::getInstance();
texManager.createDefaultTextures();
```

### 2. Load Voxel Textures

Use the `VoxelTextureLoader` helper class:

```cpp
#include "renderer/VoxelTextureLoader.h"

// Create loader with desired resolution
fresh::VoxelTextureLoader textureLoader(
    fresh::VoxelTextureLoader::TextureResolution::Low  // 16x16
);

// Load all textures
if (!textureLoader.loadAllTextures()) {
    LOG_ERROR("Failed to load some textures");
}
```

### 3. Get Textures for Voxel Types

```cpp
// Get texture for a stone block
auto stoneTexture = textureLoader.getTexture(fresh::VoxelType::Stone);

// Get face-specific texture for grass block
auto grassTop = textureLoader.getTexture(
    fresh::VoxelType::Grass, 
    fresh::VoxelTextureLoader::BlockFace::Top
);

auto grassSide = textureLoader.getTexture(
    fresh::VoxelType::Grass,
    fresh::VoxelTextureLoader::BlockFace::North
);
```

## Texture Resolutions

Three resolution levels are available:

```cpp
// Low resolution (16x16) - Good performance, retro look
textureLoader.setResolution(fresh::VoxelTextureLoader::TextureResolution::Low);

// Medium resolution (32x32) - Balanced
textureLoader.setResolution(fresh::VoxelTextureLoader::TextureResolution::Medium);

// High resolution (64x64) - Better quality
textureLoader.setResolution(fresh::VoxelTextureLoader::TextureResolution::High);

// Reload after changing resolution
textureLoader.loadAllTextures();
```

## Available Voxel Types

All standard voxel types have textures:

- `VoxelType::Stone` - Gray stone
- `VoxelType::Dirt` - Brown dirt
- `VoxelType::Grass` - Green grass (top) with dirt sides/bottom
- `VoxelType::Sand` - Yellow sand
- `VoxelType::Water` - Blue water (transparent)
- `VoxelType::Wood` - Wood log
- `VoxelType::Leaves` - Green leaves (transparent)

## Rendering with Textures

### Basic Rendering

```cpp
// In your render loop
auto texture = textureLoader.getTexture(voxelType);
if (texture) {
    texture->bind(0);  // Bind to texture unit 0
    // ... draw your voxel mesh ...
    texture->unbind();
}
```

### Face-Specific Rendering (for Grass blocks)

```cpp
if (textureLoader.hasFaceSpecificTextures(voxelType)) {
    // Render each face with its specific texture
    auto topTex = textureLoader.getTexture(voxelType, BlockFace::Top);
    auto sideTex = textureLoader.getTexture(voxelType, BlockFace::North);
    auto bottomTex = textureLoader.getTexture(voxelType, BlockFace::Bottom);
    
    // Render top face
    topTex->bind(0);
    // ... draw top face ...
    
    // Render side faces
    sideTex->bind(0);
    // ... draw side faces ...
    
    // Render bottom face
    bottomTex->bind(0);
    // ... draw bottom face ...
} else {
    // Simple case: same texture all faces
    auto tex = textureLoader.getTexture(voxelType);
    tex->bind(0);
    // ... draw all faces ...
}
```

## UI Textures

UI assets are separate from voxel textures:

```cpp
// Load crosshair
auto crosshair = texManager.loadTexture("textures/ui/crosshair.png");

// Load hotbar elements
auto hotbarSlot = texManager.loadTexture("textures/ui/hotbar_slot.png");
auto hotbarSelected = texManager.loadTexture("textures/ui/hotbar_slot_selected.png");

// Load health indicators
auto heartFull = texManager.loadTexture("textures/ui/health_heart_full.png");
auto heartEmpty = texManager.loadTexture("textures/ui/health_heart_empty.png");
```

## Direct Texture Loading

If you prefer direct loading without the helper class:

```cpp
auto& texManager = fresh::TextureManager::getInstance();

// Load specific texture
auto stoneTexture = texManager.loadTexture("textures/blocks/stone_16x16.png");

// The texture manager caches textures automatically
// Subsequent calls with the same path return the cached texture
auto stoneTex2 = texManager.loadTexture("textures/blocks/stone_16x16.png");
// stoneTex2 points to the same texture as stoneTexture
```

## Performance Tips

1. **Preload textures** - Load all textures at startup to avoid hitches during gameplay
2. **Use texture caching** - TextureManager automatically caches loaded textures
3. **Choose appropriate resolution** - Start with 16x16 for best performance
4. **Batch rendering** - Group voxels by texture type to minimize texture switches

## Troubleshooting

### Textures not loading

Check that the textures directory exists:
```bash
ls -la textures/blocks/
ls -la textures/ui/
```

### Missing texture files

Regenerate textures using the provided Python scripts:
```bash
python3 tools/generate_textures.py
python3 tools/generate_ui_assets.py
```

### Wrong texture resolution

Verify the resolution setting:
```cpp
auto res = textureLoader.getResolution();
LOG_INFO("Current resolution: " + std::to_string(static_cast<int>(res)));
```

## Next Steps

- Implement texture atlas for better performance
- Add texture animation (water, lava)
- Create biome-specific texture variations
- Add normal maps for advanced lighting
- Implement mipmapping for distant blocks

## Example: Complete Initialization

```cpp
#include "renderer/TextureManager.h"
#include "renderer/VoxelTextureLoader.h"
#include "core/Logger.h"

bool initializeTextures() {
    // Initialize texture manager
    auto& texManager = fresh::TextureManager::getInstance();
    texManager.createDefaultTextures();
    
    // Create voxel texture loader
    fresh::VoxelTextureLoader textureLoader(
        fresh::VoxelTextureLoader::TextureResolution::Low
    );
    
    // Load all voxel textures
    if (!textureLoader.loadAllTextures()) {
        LOG_ERROR("Failed to load voxel textures");
        return false;
    }
    
    // Load UI textures
    auto crosshair = texManager.loadTexture("textures/ui/crosshair.png");
    if (!crosshair) {
        LOG_WARNING("Crosshair texture not loaded");
    }
    
    LOG_INFO("All textures initialized successfully");
    return true;
}
```

---

For more information, see:
- [Texture System Documentation](../docs/TEXTURES.md)
- [Rendering Pipeline](../docs/RENDERING.md)
- [Asset Management](../docs/ASSETS.md)
