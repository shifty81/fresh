# Asset Pack Creation Guide

This guide explains how to create custom asset packs for the Fresh Voxel Engine.

## Table of Contents
- [Overview](#overview)
- [Quick Start](#quick-start)
- [Directory Structure](#directory-structure)
- [Manifest File](#manifest-file)
- [Asset Properties](#asset-properties)
- [Placement Rules](#placement-rules)
- [Biome Types](#biome-types)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)

---

## Overview

Asset packs allow you to add custom content to the Fresh Voxel Engine without modifying the engine code. Assets are automatically discovered and loaded at startup, making it easy to create and share custom content.

**Supported Asset Types:**
- 3D Models (.obj, .fbx, .gltf, .glb, .dae)
- Textures (.png, .jpg, .jpeg, .bmp, .tga, .dds)

---

## Quick Start

### 1. Generate Template

The engine can generate a complete template for you:

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

### 2. Add Your Assets

- Place model files in `models/`
- Place texture files in `textures/`

### 3. Configure Manifest

Edit `manifest.json` to define your assets (see [Manifest File](#manifest-file))

### 4. Install

Copy the asset pack folder to your engine's `Assets/` directory. The engine will automatically load it on startup.

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

## Examples

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
