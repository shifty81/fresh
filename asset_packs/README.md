# Fresh Voxel Engine - Asset Packs

This directory contains asset packs for the Fresh Voxel Engine. Asset packs allow you to customize voxel block textures and materials without modifying the engine code.

## 📁 Directory Structure

```
asset_packs/
├── README.md              # This file
├── example_pack/          # Complete example showing all features
│   ├── voxel_materials.json
│   ├── textures/
│   └── README.md
└── your_custom_pack/      # Your custom packs go here
    ├── voxel_materials.json
    └── textures/
```

## 🚀 Quick Start

### 1. Using the Example Pack

The `example_pack` directory contains a fully functional asset pack with:
- All supported voxel types
- Example textures
- Complete manifest with comments
- Comprehensive documentation

**To use it:**

```cpp
#include "assets/VoxelMaterialPack.h"

// Initialize and load packs
auto& packManager = VoxelMaterialPackManager::getInstance();
packManager.initialize("asset_packs");
packManager.scanAndLoadPacks();  // Loads all packs in asset_packs/

// Get textures through the pack manager
auto texture = packManager.getTexture(VoxelType::Stone, "all");
```

### 2. Creating Your Own Pack

**Option A: Start from Example**
```bash
cp -r asset_packs/example_pack asset_packs/my_pack
cd asset_packs/my_pack
# Edit voxel_materials.json and replace textures
```

**Option B: Use Pack Template Generator**
```cpp
// Create a new pack template programmatically
VoxelMaterialPackManager::createPackTemplate("asset_packs/my_new_pack");
```

**Option C: Create from Scratch**
1. Create a directory: `asset_packs/my_pack/`
2. Create manifest: `voxel_materials.json`
3. Create textures folder: `textures/`
4. Add your PNG texture files
5. Reference them in the manifest

## 📋 Manifest File Format

Every asset pack needs a `voxel_materials.json` manifest file:

```json
{
  "name": "My Custom Pack",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "Description of your pack",
  "priority": 100,
  
  "materials": [
    {
      "voxelType": "Stone",
      "name": "Stone Block",
      "textures": {
        "all": "textures/stone.png"
      },
      "materialProperties": {
        "metallic": 0.0,
        "roughness": 0.9,
        "alpha": 1.0
      },
      "includeInAtlas": true
    }
  ]
}
```

See [example_pack/README.md](example_pack/README.md) for detailed documentation.

## 🎨 Texture Guidelines

### Supported Formats
- **Format**: PNG (with or without alpha channel)
- **Resolution**: Any square size (16x16, 32x32, 64x64, etc.)
- **Recommended**: 32x32 for balance of quality and performance

### Texture Types

**Single Texture (All Faces)**
```json
"textures": {
  "all": "textures/stone.png"
}
```

**Face-Specific Textures**
```json
"textures": {
  "top": "textures/grass_top.png",
  "bottom": "textures/dirt.png",
  "sides": "textures/grass_side.png"
}
```

**Individual Faces**
```json
"textures": {
  "top": "textures/top.png",
  "bottom": "textures/bottom.png",
  "north": "textures/north.png",
  "south": "textures/south.png",
  "east": "textures/east.png",
  "west": "textures/west.png"
}
```

## 🔧 Available Voxel Types

You can customize any of these voxel types:

| Voxel Type | Description |
|------------|-------------|
| `Air` | Invisible blocks (usually not textured) |
| `Stone` | Basic stone blocks |
| `Dirt` | Dirt blocks |
| `Grass` | Grass blocks (typically different top/bottom/sides) |
| `Sand` | Sandy terrain |
| `Water` | Water blocks (supports transparency) |
| `Wood` | Wood logs (often different top/sides) |
| `Leaves` | Tree leaves (supports transparency) |
| `Bedrock` | Indestructible bedrock |
| `Snow` | Snow blocks |
| `Ice` | Ice blocks (supports transparency) |
| `Cobblestone` | Cobblestone |
| `Planks` | Wooden planks |
| `Glass` | Glass blocks (supports transparency) |

## 🎯 Material Properties

Control how blocks render with material properties:

### Basic Properties
```json
"materialProperties": {
  "metallic": 0.0,        // 0.0 = non-metal, 1.0 = fully metallic
  "roughness": 0.5,       // 0.0 = smooth/shiny, 1.0 = rough/matte
  "ao": 1.0,              // Ambient occlusion factor (0.0 to 1.0)
  "alpha": 1.0            // Opacity (1.0 = opaque, 0.0 = transparent)
}
```

### Transparency
```json
"materialProperties": {
  "alpha": 0.6,           // 60% transparent
  "alphaBlend": true      // Enable alpha blending
}
```

### Alpha Cutoff (for leaves/vegetation)
```json
"materialProperties": {
  "alpha": 0.8,
  "alphaBlend": true,
  "alphaCutoffValue": 0.3  // Pixels below 30% alpha are discarded
}
```

### PBR Maps (Advanced)
```json
"materialProperties": {
  "normalMap": "textures/stone_normal.png",
  "metallicRoughnessMap": "textures/stone_mr.png",
  "aoMap": "textures/stone_ao.png",
  "emissiveMap": "textures/stone_emissive.png",
  "emissive": [1.0, 0.5, 0.0],
  "emissiveStrength": 2.0
}
```

## 🔄 Pack Priority System

When multiple packs are loaded, priority determines which pack's materials are used:

```
priority: 0    → Default textures (lowest priority)
priority: 50   → HD texture pack
priority: 100  → Themed pack (highest priority, overrides others)
```

**Example:**
- You have the default textures (priority 0)
- You load an HD pack (priority 50) → HD textures override defaults
- You load a sci-fi theme (priority 100) → Theme overrides HD textures

This allows you to layer packs and mix different styles!

## 💻 API Usage

### Loading Packs

```cpp
#include "assets/VoxelMaterialPack.h"

// Get the pack manager singleton
auto& packManager = VoxelMaterialPackManager::getInstance();

// Initialize with packs directory
packManager.initialize("asset_packs");

// Scan and load all packs in the directory
packManager.scanAndLoadPacks();

// Or load a specific pack
packManager.loadPack("asset_packs/my_pack");
```

### Using Packs

```cpp
// Get texture from highest priority pack
auto texture = packManager.getTexture(VoxelType::Stone, "all");

// Get material definition
const auto* material = packManager.getMaterialDefinition(VoxelType::Grass);
if (material) {
    std::cout << "Material: " << material->name << std::endl;
}

// List loaded packs
const auto& packs = packManager.getLoadedPacks();
for (const auto& pack : packs) {
    std::cout << pack->getName() << " v" << pack->getVersion() 
              << " (priority: " << pack->getPriority() << ")" << std::endl;
}
```

### Texture Atlas

Generate an optimized texture atlas:

```cpp
auto& packs = packManager.getLoadedPacks();
for (auto& pack : packs) {
    // Generate 1024x1024 atlas with 32x32 tiles
    auto atlas = pack->generateTextureAtlas(1024, 32);
    
    // Get UV coordinates for rendering
    auto uvs = pack->getAtlasUVs(VoxelType::Stone, "all");
    // uvs contains [minU, minV, maxU, maxV]
}
```

### Validation

```cpp
// Validate a pack before loading
bool isValid = packManager.validatePack("asset_packs/my_pack");
if (!isValid) {
    std::cerr << "Pack validation failed!" << std::endl;
}

// Print statistics
packManager.printStats();
```

## 📦 Pack Distribution

### Sharing Your Pack

1. **Package your pack**:
   ```bash
   cd asset_packs
   zip -r my_awesome_pack.zip my_awesome_pack/
   ```

2. **Include documentation**:
   - README with installation instructions
   - Screenshots showing your textures in-game
   - License information
   - Credits for any source materials

3. **Share**:
   - Upload to GitHub, itch.io, or your preferred platform
   - Include engine version compatibility

### Installing Community Packs

1. **Download the pack**
2. **Extract to asset_packs/**:
   ```bash
   unzip my_awesome_pack.zip -d asset_packs/
   ```
3. **Restart or reload** the engine

## 🎨 Example Packs

### Minimalist Style
Override just a few blocks for a subtle change:
```json
{
  "name": "Clean Minimalist",
  "priority": 50,
  "materials": [
    { "voxelType": "Stone", "textures": { "all": "textures/smooth_stone.png" } },
    { "voxelType": "Grass", "textures": { "all": "textures/simple_grass.png" } }
  ]
}
```

### Complete Overhaul
Replace all textures with a unique theme:
```json
{
  "name": "Sci-Fi Industrial",
  "priority": 100,
  "materials": [
    // Define all 14 voxel types with sci-fi themed textures
  ]
}
```

### HD Texture Pack
High-resolution textures with PBR materials:
```json
{
  "name": "HD Realistic",
  "priority": 75,
  "materials": [
    {
      "voxelType": "Stone",
      "textures": { "all": "textures/stone_4k.png" },
      "materialProperties": {
        "normalMap": "textures/stone_normal.png",
        "metallicRoughnessMap": "textures/stone_mr.png"
      }
    }
  ]
}
```

## 🐛 Troubleshooting

### Pack Not Loading
- ✅ Check JSON syntax (use a JSON validator)
- ✅ Verify `voxel_materials.json` exists
- ✅ Check console logs for error messages
- ✅ Ensure pack directory is in `asset_packs/`

### Textures Not Showing
- ✅ Verify PNG files exist at paths specified in manifest
- ✅ Check that paths are relative to pack root
- ✅ Ensure PNG format is correct (RGB or RGBA)
- ✅ Verify voxelType names match exactly (case-sensitive)

### Performance Issues
- ✅ Reduce texture resolution (try 32x32 or 64x64)
- ✅ Enable texture atlasing
- ✅ Limit number of transparent blocks
- ✅ Use power-of-2 texture sizes

## 📚 Resources

- **Example Pack**: See [example_pack/README.md](example_pack/README.md) for complete documentation
- **Engine Docs**: Check main project README for API reference
- **Texture Tools**: GIMP, Photoshop, or online tools for creating textures
- **Tileable Textures**: Search for "seamless texture generator" online

## 🤝 Contributing

Have a cool pack to share? Consider contributing to the community:
1. Create your pack
2. Add documentation
3. Submit to the community showcase
4. Help others create their own packs

## 📄 License

The asset pack system is part of the Fresh Voxel Engine (MIT License).
Individual packs may have their own licenses - check each pack's README.

---

**Ready to create your own textures?** Check out the [example_pack](example_pack/) to get started! 🎨
