# Asset Pack Quick Reference

## 📋 Minimal Pack Structure

```
my_pack/
├── voxel_materials.json    # Required
└── textures/
    └── stone.png          # At least one texture
```

## 🔧 Minimal Manifest

```json
{
  "name": "My Pack",
  "version": "1.0.0",
  "materials": [
    {
      "voxelType": "Stone",
      "textures": { "all": "textures/stone.png" }
    }
  ]
}
```

## 📐 Texture Configurations

### Same texture on all faces
```json
"textures": { "all": "textures/stone.png" }
```

### Different top/bottom (like grass)
```json
"textures": {
  "top": "textures/grass_top.png",
  "bottom": "textures/dirt.png",
  "sides": "textures/grass_side.png"
}
```

### Different top/sides (like wood logs)
```json
"textures": {
  "top": "textures/wood_rings.png",
  "bottom": "textures/wood_rings.png",
  "sides": "textures/wood_bark.png"
}
```

### All six faces different
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

## 🎨 Common Material Presets

### Solid Opaque Block
```json
"materialProperties": {
  "metallic": 0.0,
  "roughness": 0.9,
  "alpha": 1.0
}
```

### Shiny Metal
```json
"materialProperties": {
  "metallic": 1.0,
  "roughness": 0.2,
  "alpha": 1.0
}
```

### Transparent (Water/Glass)
```json
"materialProperties": {
  "metallic": 0.0,
  "roughness": 0.1,
  "alpha": 0.6,
  "alphaBlend": true
}
```

### Alpha Cutout (Leaves)
```json
"materialProperties": {
  "alpha": 1.0,
  "alphaBlend": true,
  "alphaCutoffValue": 0.5
}
```

### Glowing/Emissive
```json
"materialProperties": {
  "emissive": [1.0, 0.8, 0.2],
  "emissiveStrength": 3.0
}
```

## 🎯 Voxel Type Names (Copy-Paste Ready)

```
Air
Stone
Dirt
Grass
Sand
Water
Wood
Leaves
Bedrock
Snow
Ice
Cobblestone
Planks
Glass
```

## ⚡ Quick Commands

### Create from example
```bash
cp -r asset_packs/example_pack asset_packs/my_pack
cd asset_packs/my_pack
# Edit voxel_materials.json
# Replace textures in textures/ folder
```

### Package for distribution
```bash
cd asset_packs
zip -r my_pack.zip my_pack/
```

### Validate JSON
```bash
python -m json.tool voxel_materials.json
```

## 📏 Recommended Texture Sizes

| Use Case | Resolution | File Size |
|----------|-----------|-----------|
| Low-end / Mobile | 16x16 | ~1-2 KB |
| Balanced | 32x32 | ~2-4 KB |
| High Quality | 64x64 | ~4-8 KB |
| Ultra HD | 128x128 | ~10-20 KB |

## 🔍 Troubleshooting Checklist

- [ ] `voxel_materials.json` is valid JSON
- [ ] All texture paths exist
- [ ] Texture paths are relative to pack root
- [ ] PNG files are valid RGB or RGBA
- [ ] VoxelType names match exactly (case-sensitive)
- [ ] Transparent blocks have `alphaBlend: true`
- [ ] Texture files are square (same width/height)

## 🚀 Testing Your Pack

```cpp
#include "assets/VoxelMaterialPack.h"

auto& mgr = VoxelMaterialPackManager::getInstance();
mgr.initialize("asset_packs");

// Validate
if (mgr.validatePack("asset_packs/my_pack")) {
    mgr.loadPack("asset_packs/my_pack");
} else {
    // Check logs for errors
}
```

## 💡 Pro Tips

1. **Test incrementally** - Add one material at a time
2. **Use consistent naming** - Keep texture names organized
3. **Include README** - Document your pack for users
4. **Version your pack** - Update version number for releases
5. **Credit sources** - If using textures from elsewhere, give credit
6. **Screenshot everything** - Show your textures in action
7. **Keep textures tileable** - Make sure edges match for seamless blocks

## 🎨 Texture Creation Tools

- **GIMP** - Free, full-featured (https://www.gimp.org/)
- **Photoshop** - Industry standard
- **Aseprite** - Great for pixel art
- **Online** - Search "seamless texture generator"

## 📚 Full Documentation

See [README.md](README.md) for complete documentation and examples.

---

**Happy pack creating!** 🎨
