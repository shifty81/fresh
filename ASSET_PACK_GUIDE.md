# Asset Pack Creation Guide

This guide explains how to create custom asset packs for the Fresh Voxel Engine.

## Table of Contents
- [Overview](#overview)
- [Asset Pack Types](#asset-pack-types)
  - [3D Model Asset Packs](#3d-model-asset-packs)
  - [Voxel Material/Texture Packs](#voxel-materialtexture-packs)
- [Quick Start](#quick-start)
- [Directory Structure](#directory-structure)
- [Manifest Files](#manifest-files)
- [Asset Properties](#asset-properties)
- [Placement Rules](#placement-rules)
- [Biome Types](#biome-types)
- [Voxel Material Packs](#voxel-material-packs-detailed)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

---

## Overview

Asset packs allow you to add custom content to the Fresh Voxel Engine without modifying the engine code. Assets are automatically discovered and loaded at startup, making it easy to create and share custom content.

**Supported Asset Pack Types:**

1. **3D Model Asset Packs** - Add decorative objects, structures, and props to the world
2. **Voxel Material/Texture Packs** - Customize the appearance of voxel blocks (like Minecraft resource packs)

**Supported File Formats:**
- 3D Models: .obj, .fbx, .gltf, .glb, .dae
- Textures: .png, .jpg, .jpeg, .bmp, .tga, .dds

---

## Asset Pack Types

### 3D Model Asset Packs

3D model asset packs add custom objects and decorations to your voxel world. These include trees, rocks, buildings, and other props that are placed during world generation or by the player.

- **Use Case**: Add custom trees, boulders, structures, decorations
- **Manifest**: `manifest.json`
- **Placement**: Biome-based, rule-based automatic placement

### Voxel Material/Texture Packs

Voxel material packs allow you to customize the textures and materials of voxel blocks themselves, similar to Minecraft resource packs. Change how stone, dirt, grass, and other blocks look.

- **Use Case**: Create custom block textures, themed texture sets (medieval, sci-fi, etc.)
- **Manifest**: `voxel_materials.json` or `materials.json`
- **Application**: Replaces default voxel block textures

---

## Quick Start

### For 3D Model Asset Packs

Generate a template for 3D models:

```cpp
ModularAssetSystem::createAssetPackTemplate("path/to/MyAssetPack");
```

This creates:
```
MyAssetPack/
├── manifest.json       # Configuration file
├── README.md           # Documentation
├── models/             # Model directory
└── textures/           # Texture directory
```

### For Voxel Material/Texture Packs

Generate a template for voxel materials:

```cpp
VoxelMaterialPackManager::createPackTemplate("path/to/MyVoxelPack");
```

This creates:
```
MyVoxelPack/
├── voxel_materials.json   # Voxel material definitions (REQUIRED)
├── README.md              # Documentation
└── textures/              # Block texture files
    ├── stone.png
    ├── grass_top.png
    ├── grass_side.png
    └── ...
```

### Installation

1. Place your asset pack folder in the appropriate directory:
   - 3D Model Packs: `Assets/` directory
   - Voxel Material Packs: Materials directory (configurable)
2. The engine will automatically discover and load packs on startup
3. Check console output for loading confirmation

---

## Directory Structure

Asset packs must follow this structure:

```
YourAssetPack/
├── manifest.json           # REQUIRED: Asset pack configuration
├── README.md               # Optional: Documentation
├── models/                 # 3D model files
│   ├── tree.obj
│   ├── rock.obj
│   └── ...
├── textures/               # Texture files
│   ├── tree.png
│   ├── rock.png
│   └── ...
└── [additional folders]    # Optional: organize as needed
```

**Requirements:**
- `manifest.json` must exist in the root directory
- File paths in manifest must be relative to pack root
- Follow the naming conventions (lowercase, no spaces recommended)

---

## Manifest File

The `manifest.json` file defines your asset pack metadata and all assets.

### Basic Structure

```json
{
  "name": "MyAssetPack",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "Pack description",
  "assets": [
    {
      "name": "AssetName",
      "modelPath": "models/asset.obj",
      "texturePath": "textures/asset.png",
      ...
    }
  ]
}
```

### Pack Metadata

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | string | Yes | Unique identifier for the pack |
| `version` | string | Yes | Version number (e.g., "1.0.0") |
| `author` | string | Yes | Creator name |
| `description` | string | Yes | Brief description |
| `assets` | array | Yes | Array of asset definitions |

---

## Asset Properties

Each asset in the `assets` array supports these properties:

### Required Fields

| Field | Type | Description | Example |
|-------|------|-------------|---------|
| `name` | string | Unique asset identifier | `"OakTree"` |
| `modelPath` | string | Path to model file | `"models/oak.obj"` |
| `texturePath` | string | Path to texture file | `"textures/oak.png"` |

### Optional Fields

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `description` | string | `""` | Asset description |
| `biomes` | array | `["Any"]` | Allowed biomes (see [Biome Types](#biome-types)) |
| `placementRule` | string | `"Random"` | Placement algorithm (see [Placement Rules](#placement-rules)) |
| `spawnChance` | float | `0.1` | Spawn probability (0.0-1.0) |
| `minDistance` | float | `10.0` | Min distance between instances |
| `minGroupSize` | int | `1` | Min cluster size (for Clustered rule) |
| `maxGroupSize` | int | `5` | Max cluster size (for Clustered rule) |
| `size` | array | `[1,1,1]` | Asset dimensions [width, height, depth] |
| `offset` | array | `[0,0,0]` | Position offset [x, y, z] |
| `randomRotation` | bool | `true` | Enable random Y-axis rotation |
| `rotationStep` | float | `90.0` | Rotation angle step (degrees) |
| `placeOnSurface` | bool | `true` | Place on terrain surface |
| `embedInTerrain` | bool | `false` | Partially sink into terrain |
| `embedDepth` | float | `0.0` | How deep to embed (if enabled) |
| `tags` | array | `[]` | Tags for filtering/categorization |
| `customProperties` | object | `{}` | Custom key-value pairs |

---

## Placement Rules

Controls how assets are distributed in the world:

| Rule | Description | Best For |
|------|-------------|----------|
| `Random` | Random placement within biome | General decoration, rocks |
| `Clustered` | Grouped together | Trees, forests, villages |
| `Grid` | Regular grid pattern | Structured layouts, farms |
| `NearWater` | Near water sources | Reeds, docks, water plants |
| `OnHills` | On elevated terrain | Lookout towers, windmills |
| `InCaves` | Underground caves | Crystals, mushrooms, ore |
| `RiverSide` | Along rivers | Riverside vegetation |

### Placement Parameters

```json
{
  "placementRule": "Clustered",
  "spawnChance": 0.15,        // 15% chance per chunk
  "minDistance": 8.0,         // 8 units apart minimum
  "minGroupSize": 2,          // 2-8 assets per cluster
  "maxGroupSize": 8
}
```

---

## Biome Types

Assets can be restricted to specific biomes:

| Biome | Description |
|-------|-------------|
| `Any` | Can spawn anywhere (default) |
| `Forest` | Forested areas |
| `Plains` | Open grasslands |
| `Desert` | Sandy/arid regions |
| `Mountains` | High elevation areas |
| `Swamp` | Wetlands |
| `Tundra` | Cold/snowy regions |
| `Ocean` | Water bodies |
| `Cave` | Underground |

### Examples

```json
// Spawn only in forests
"biomes": ["Forest"]

// Spawn in forests or plains
"biomes": ["Forest", "Plains"]

// Spawn anywhere
"biomes": ["Any"]
```

---

## Voxel Material Packs (Detailed)

Voxel material packs allow you to customize voxel block textures and materials, similar to Minecraft resource packs. This is perfect for creating themed worlds or custom visual styles.

### Creating a Voxel Material Pack

1. **Generate Template**:
   ```cpp
   VoxelMaterialPackManager::createPackTemplate("MyVoxelPack");
   ```

2. **Create Block Textures**:
   - Use pixel art tools (Aseprite, GIMP, Photoshop, etc.)
   - Recommended sizes: 16x16, 32x32, or 64x64 pixels
   - Save as PNG for transparency support
   - Make textures tileable for seamless appearance

3. **Edit voxel_materials.json**:
   - Define which voxel types to customize
   - Specify texture paths for each block type
   - Configure material properties (optional)

### Manifest Structure (`voxel_materials.json`)

```json
{
  "name": "MyVoxelMaterialPack",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "Custom voxel block textures",
  "priority": 0,
  "materials": [
    {
      "voxelType": "Stone",
      "name": "Custom Stone",
      "textures": {
        "all": "textures/stone.png"
      },
      "materialProperties": {
        "roughness": 0.8,
        "metallic": 0.0
      }
    }
  ]
}
```

### Pack Metadata Fields

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | string | Yes | Unique pack identifier |
| `version` | string | Yes | Version number (e.g., "1.0.0") |
| `author` | string | Yes | Creator name |
| `description` | string | No | Brief description |
| `priority` | integer | No | Loading priority (higher = takes precedence), default: 0 |

### Material Definition Fields

#### Required Fields

| Field | Type | Description |
|-------|------|-------------|
| `voxelType` | string | Voxel type name (see [Supported Voxel Types](#supported-voxel-types)) |
| `name` | string | Display name for this material |
| `textures` | object | Texture paths (see below) |

#### Texture Options

**Option 1: Single texture for all faces**
```json
"textures": {
  "all": "textures/stone.png"
}
```

**Option 2: Face-specific textures (like grass blocks)**
```json
"textures": {
  "top": "textures/grass_top.png",
  "bottom": "textures/dirt.png",
  "sides": "textures/grass_side.png"
}
```

Or specify each face individually:
```json
"textures": {
  "top": "textures/grass_top.png",
  "bottom": "textures/dirt.png",
  "north": "textures/grass_side.png",
  "south": "textures/grass_side.png",
  "east": "textures/grass_side.png",
  "west": "textures/grass_side.png"
}
```

#### Optional Material Properties

For PBR (Physically Based Rendering) materials:

```json
"materialProperties": {
  "normalMap": "textures/stone_normal.png",
  "metallicRoughnessMap": "textures/stone_mr.png",
  "aoMap": "textures/stone_ao.png",
  "emissiveMap": "textures/glow.png",
  "metallic": 0.0,
  "roughness": 0.8,
  "ao": 1.0,
  "alpha": 1.0,
  "alphaBlend": false,
  "alphaCutoffValue": 0.5,
  "emissive": [0.0, 0.0, 0.0],
  "emissiveStrength": 1.0
}
```

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `normalMap` | string | - | Path to normal map texture |
| `metallicRoughnessMap` | string | - | Combined metallic/roughness map |
| `aoMap` | string | - | Ambient occlusion map |
| `emissiveMap` | string | - | Emissive (glow) map |
| `metallic` | float | 0.0 | Metallic factor (0.0-1.0) |
| `roughness` | float | 0.5 | Surface roughness (0.0-1.0) |
| `ao` | float | 1.0 | Ambient occlusion factor |
| `alpha` | float | 1.0 | Transparency (1.0 = opaque) |
| `alphaBlend` | boolean | false | Enable alpha blending |
| `alphaCutoffValue` | float | 0.5 | Alpha cutoff threshold |
| `emissive` | array | [0,0,0] | Emissive color [R,G,B] |
| `emissiveStrength` | float | 1.0 | Emissive intensity multiplier |

### Supported Voxel Types

- `Air` - Empty space
- `Stone` - Stone blocks
- `Dirt` - Dirt blocks
- `Grass` - Grass blocks (supports face-specific textures)
- `Sand` - Sand blocks
- `Water` - Water blocks (transparent)
- `Wood` - Wood/log blocks
- `Leaves` - Leaf blocks (transparent)
- `Bedrock` - Bedrock blocks
- `Snow` - Snow blocks
- `Ice` - Ice blocks (transparent)
- `Cobblestone` - Cobblestone blocks
- `Planks` - Wooden plank blocks
- `Glass` - Glass blocks (transparent)

### Multiple Packs and Priority

When multiple voxel material packs are loaded:
- Higher priority packs override lower priority packs
- Same priority: last loaded wins
- Default priority is 0
- Use negative priorities for base packs, positive for overrides

Example:
```json
{
  "name": "BaseTextures",
  "priority": -10,  // Low priority base pack
  ...
}

{
  "name": "HighResOverride",
  "priority": 10,   // High priority override
  ...
}
```

### Texture Guidelines

- **Resolution**: 16x16, 32x32, or 64x64 pixels (power of 2)
- **Format**: PNG (recommended for transparency), JPG, or TGA
- **Tileable**: Textures should tile seamlessly on all sides
- **Transparency**: Use PNG alpha channel for see-through blocks (water, glass, leaves)
- **Consistency**: Keep visual style consistent across all blocks
- **Performance**: Higher resolutions = more memory, balance quality vs performance

### Testing Your Pack

1. Copy pack to materials directory
2. Launch engine - check console for loading messages
3. Create/load world to see your textures
4. Verify all block types display correctly
5. Test transparency on glass/water/leaves
6. Check face-specific textures on grass blocks

---

## Examples

### Example 0: Simple Stone Voxel Material

```json
{
  "name": "CustomStonepack",
  "version": "1.0.0",
  "author": "Alice",
  "description": "Custom stone texture",
  "materials": [
    {
      "voxelType": "Stone",
      "name": "Smooth Stone",
      "textures": {
        "all": "textures/smooth_stone.png"
      },
      "materialProperties": {
        "roughness": 0.6,
        "metallic": 0.0
      }
    }
  ]
}
```

### Example 1: Forest Tree

```json
{
  "name": "OakTree",
  "description": "Large oak tree for forests",
  "modelPath": "models/oak_tree.obj",
  "texturePath": "textures/oak_bark.png",
  "biomes": ["Forest", "Plains"],
  "placementRule": "Clustered",
  "spawnChance": 0.15,
  "minDistance": 8.0,
  "minGroupSize": 3,
  "maxGroupSize": 12,
  "size": [3.0, 15.0, 3.0],
  "randomRotation": true,
  "placeOnSurface": true,
  "tags": ["tree", "vegetation", "wood"]
}
```

### Example 2: Mountain Boulder

```json
{
  "name": "Boulder",
  "description": "Large rock for mountainous terrain",
  "modelPath": "models/boulder.obj",
  "texturePath": "textures/rock_grey.png",
  "biomes": ["Mountains", "Plains"],
  "placementRule": "Random",
  "spawnChance": 0.08,
  "minDistance": 20.0,
  "size": [3.0, 2.5, 3.0],
  "randomRotation": true,
  "placeOnSurface": true,
  "embedInTerrain": true,
  "embedDepth": 0.5,
  "tags": ["rock", "obstacle"]
}
```

### Example 3: Cave Crystal

```json
{
  "name": "GlowCrystal",
  "description": "Glowing crystal found in caves",
  "modelPath": "models/crystal.obj",
  "texturePath": "textures/crystal_blue.png",
  "biomes": ["Cave"],
  "placementRule": "InCaves",
  "spawnChance": 0.05,
  "minDistance": 15.0,
  "size": [1.0, 2.0, 1.0],
  "randomRotation": true,
  "placeOnSurface": true,
  "tags": ["crystal", "glowing", "rare"],
  "customProperties": {
    "lightEmission": "5.0",
    "harvestable": "true"
  }
}
```

### Example 4: Structured Grid

```json
{
  "name": "FencPost",
  "description": "Wooden fence post for farms",
  "modelPath": "models/fence_post.obj",
  "texturePath": "textures/wood_fence.png",
  "biomes": ["Plains", "Forest"],
  "placementRule": "Grid",
  "spawnChance": 1.0,
  "minDistance": 4.0,
  "size": [0.2, 1.5, 0.2],
  "randomRotation": false,
  "rotationStep": 90.0,
  "placeOnSurface": true,
  "tags": ["structure", "fence"]
}
```

---

## Best Practices

### Performance
- **Start with low spawn chances** (0.05-0.15) to avoid overcrowding
- **Use appropriate minDistance** to prevent overlapping
- **Keep poly count reasonable** for models (<5000 triangles recommended)
- **Optimize textures** (1024x1024 or smaller for most assets)

### Design
- **Match biome aesthetics** - desert assets in deserts, etc.
- **Use consistent scale** - relate to voxel size (1 voxel ≈ 1 meter)
- **Test different placements** - try various rules and parameters
- **Use tags effectively** - for easy filtering and organization

### Organization
- **Use descriptive names** - clear, unique identifiers
- **Document custom properties** - explain what they do
- **Version your packs** - increment version when making changes
- **Include README** - document special features or requirements

### Quality
- **Test thoroughly** - verify assets appear correctly
- **Validate manifest** - ensure JSON is properly formatted
- **Check file paths** - all paths must be correct and relative
- **Provide variety** - include multiple assets per biome type

---

## Troubleshooting

### Pack Not Loading

**Problem:** Asset pack doesn't appear in engine

**Solutions:**
- Verify `manifest.json` exists in pack root directory
- Check JSON syntax (use a JSON validator)
- Check console output for specific error messages
- Ensure pack folder is in the correct assets directory

### Assets Not Appearing

**Problem:** Assets defined but not visible in game

**Solutions:**
- Verify file paths are correct and relative to pack root
- Check biome restrictions match terrain
- Increase `spawnChance` for testing (try 1.0)
- Verify models and textures exist at specified paths
- Check that assets aren't being placed outside loaded chunks

### Performance Issues

**Problem:** Game runs slowly with asset pack

**Solutions:**
- Reduce `spawnChance` values
- Increase `minDistance` to reduce density
- Optimize model poly count
- Reduce texture resolution
- Limit number of assets in densely populated biomes

### Validation Errors

**Problem:** Engine reports validation errors

**Solutions:**
- Check console for specific error messages
- Verify all required fields are present
- Check data types (numbers, strings, arrays, booleans)
- Ensure biome and placement rule names are correct (case-sensitive)
- Validate JSON syntax

### Models Not Loading

**Problem:** Models fail to load or appear incorrectly

**Solutions:**
- Verify file format is supported (.obj, .fbx, .gltf, etc.)
- Check model has proper UV mapping
- Ensure model is exported with correct scale
- Verify texture paths in model file are relative or removed
- Test model in 3D viewer before adding to pack

---

## Additional Resources

- [ARCHITECTURE.md](ARCHITECTURE.md) - Engine architecture overview
- [FEATURES.md](FEATURES.md) - Complete feature list
- [PROJECT_STATUS.md](PROJECT_STATUS.md) - Current implementation status
- [CONTRIBUTING.md](CONTRIBUTING.md) - How to contribute

---

## Support

For questions, issues, or contributions:
- Open an issue on GitHub
- Check existing issues and documentation
- Share your asset packs with the community!

---

*Happy asset creation! We can't wait to see what you build.*
