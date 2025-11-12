# Example Voxel Material Pack

This is a complete example asset pack for the Fresh Voxel Engine that demonstrates how to create custom voxel textures and materials.

## 📋 What's Included

This example pack shows:
- ✅ Complete manifest file (`voxel_materials.json`) with all voxel types
- ✅ Texture files for all voxel types (32x32 PNG format)
- ✅ Face-specific textures (e.g., grass block with different top/bottom/sides)
- ✅ Material properties (metallic, roughness, transparency, etc.)
- ✅ Proper directory structure

## 📁 Directory Structure

```
example_pack/
├── voxel_materials.json    # Main manifest file (required)
├── textures/               # Texture directory
│   ├── stone.png          # Stone block texture
│   ├── dirt.png           # Dirt block texture
│   ├── grass_top.png      # Grass block - top face
│   ├── grass_side.png     # Grass block - side faces
│   ├── sand.png           # Sand block texture
│   ├── water.png          # Water block texture
│   ├── wood_side.png      # Wood log - side texture
│   ├── wood_top.png       # Wood log - top/bottom texture
│   ├── leaves.png         # Leaf block texture
│   ├── cobblestone.png    # Cobblestone texture
│   ├── planks.png         # Wooden planks texture
│   ├── glass.png          # Glass block texture
│   ├── snow.png           # Snow block texture
│   └── ice.png            # Ice block texture
└── README.md              # This file
```

## 🎨 Manifest File Format

The `voxel_materials.json` file is the heart of your asset pack. Here's the structure:

### Pack Metadata
```json
{
  "name": "Your Pack Name",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "Pack description",
  "priority": 100,
  ...
}
```

**Priority**: Higher numbers = higher priority. When multiple packs are loaded, higher priority packs override lower priority ones.

### Material Definitions

Each material in the `materials` array defines:

```json
{
  "voxelType": "Stone",
  "name": "Stone Block",
  "textures": {
    "all": "textures/stone.png"  // Same texture for all 6 faces
  },
  "materialProperties": {
    "metallic": 0.0,              // 0.0 = non-metallic, 1.0 = fully metallic
    "roughness": 0.9,             // 0.0 = smooth, 1.0 = rough
    "ao": 1.0,                    // Ambient occlusion (0.0 to 1.0)
    "alpha": 1.0                  // Transparency (1.0 = opaque, 0.0 = fully transparent)
  },
  "includeInAtlas": true
}
```

### Face-Specific Textures

For blocks with different textures on different faces (like grass or wood logs):

```json
{
  "voxelType": "Grass",
  "textures": {
    "top": "textures/grass_top.png",
    "bottom": "textures/dirt.png",
    "sides": "textures/grass_side.png"    // Applied to north, south, east, west
  }
}
```

You can also specify individual faces:
```json
{
  "textures": {
    "top": "textures/top.png",
    "bottom": "textures/bottom.png",
    "north": "textures/north.png",
    "south": "textures/south.png",
    "east": "textures/east.png",
    "west": "textures/west.png"
  }
}
```

### Transparent Blocks

For blocks with transparency (water, glass, leaves):

```json
{
  "voxelType": "Water",
  "textures": {
    "all": "textures/water.png"
  },
  "materialProperties": {
    "alpha": 0.6,                 // 60% transparent
    "alphaBlend": true,           // Enable alpha blending
    "roughness": 0.1              // Smooth for reflective water
  }
}
```

For alpha cutoff (like leaves with holes):
```json
{
  "voxelType": "Leaves",
  "materialProperties": {
    "alpha": 0.8,
    "alphaBlend": true,
    "alphaCutoffValue": 0.3       // Pixels below 30% alpha are discarded
  }
}
```

## 🎯 Available Voxel Types

The following voxel types are currently supported:
- `Air` - Invisible blocks
- `Stone` - Basic stone
- `Dirt` - Dirt blocks
- `Grass` - Grass with different top/bottom/sides
- `Sand` - Sandy terrain
- `Water` - Water blocks
- `Wood` - Wood logs
- `Leaves` - Tree leaves
- `Bedrock` - Indestructible bedrock
- `Snow` - Snow blocks
- `Ice` - Ice blocks
- `Cobblestone` - Cobblestone
- `Planks` - Wooden planks
- `Glass` - Glass blocks

## 🖼️ Texture Requirements

### Format
- **File Format**: PNG with RGBA support
- **Resolution**: Any square resolution (16x16, 32x32, 64x64, 128x128, etc.)
- **Channels**: RGB for opaque blocks, RGBA for transparent blocks
- **Compression**: PNG compression is fine

### Best Practices
1. **Keep textures square** (same width and height)
2. **Use consistent resolution** across all textures in a pack
3. **Include alpha channel** for transparent blocks (water, glass, leaves)
4. **Use power-of-2 sizes** for best performance (16, 32, 64, 128, 256)
5. **Avoid extremely large textures** (>256x256) as they impact performance

## 🚀 Using Your Custom Pack

### Installation

1. **Create your pack directory**:
   ```
   asset_packs/my_custom_pack/
   ```

2. **Add your manifest**:
   ```
   asset_packs/my_custom_pack/voxel_materials.json
   ```

3. **Add your textures**:
   ```
   asset_packs/my_custom_pack/textures/
   ```

4. **Load in code**:
   ```cpp
   #include "assets/VoxelMaterialPack.h"
   
   auto& packManager = VoxelMaterialPackManager::getInstance();
   packManager.initialize("asset_packs");
   packManager.scanAndLoadPacks();  // Automatically finds and loads all packs
   
   // Or load a specific pack:
   packManager.loadPack("asset_packs/my_custom_pack");
   ```

### Pack Priority

When multiple packs are loaded:
- Higher `priority` values override lower ones
- If two packs define the same voxel type, the higher priority wins
- Default priority is 0 if not specified

Example use case:
- Base pack: priority 0 (default textures)
- HD texture pack: priority 50 (higher quality)
- Custom theme pack: priority 100 (your custom style)

## 🎨 Creating Custom Textures

### Quick Start

1. **Copy this example pack**:
   ```bash
   cp -r asset_packs/example_pack asset_packs/my_pack
   ```

2. **Edit the manifest**:
   - Change `name`, `author`, `description`
   - Adjust `priority` if needed

3. **Replace textures**:
   - Edit PNG files in `textures/` directory
   - Keep the same filenames or update paths in manifest

4. **Test your pack**:
   ```bash
   # Build and run the engine with your pack
   ```

### Advanced: PBR Materials

For advanced rendering, you can add PBR (Physically-Based Rendering) maps:

```json
{
  "voxelType": "Stone",
  "textures": {
    "all": "textures/stone.png"
  },
  "materialProperties": {
    "normalMap": "textures/stone_normal.png",           // Bump/normal mapping
    "metallicRoughnessMap": "textures/stone_mr.png",    // Metallic + roughness
    "aoMap": "textures/stone_ao.png",                   // Ambient occlusion
    "emissiveMap": "textures/stone_emissive.png",       // Glow map
    "emissive": [1.0, 0.5, 0.0],                        // Emissive color (RGB)
    "emissiveStrength": 2.0                             // Glow intensity
  }
}
```

## 📦 Texture Atlas

The engine can automatically generate a texture atlas from your pack for optimized rendering:

```cpp
auto& packManager = VoxelMaterialPackManager::getInstance();
auto pack = packManager.getLoadedPacks()[0];

// Generate 1024x1024 atlas with 32x32 tiles
auto atlas = pack->generateTextureAtlas(1024, 32);

// Get UV coordinates for a specific block face
auto uvs = pack->getAtlasUVs(VoxelType::Stone, "all");
```

## 🔧 Validation

Validate your pack before distribution:

```cpp
auto& packManager = VoxelMaterialPackManager::getInstance();
bool isValid = packManager.validatePack("asset_packs/my_pack");
if (!isValid) {
    // Check logs for validation errors
}
```

## 💡 Tips and Tricks

### Tileable Textures
Make sure your textures tile seamlessly:
- The left edge should match the right edge
- The top edge should match the bottom edge
- Use texture creation tools with "seamless" or "tile" options

### Testing
- Test with different lighting conditions
- Check transparency rendering order
- Verify face-specific textures orient correctly

### Performance
- Use texture atlases for better performance
- Keep resolution reasonable (32x32 or 64x64 is usually sufficient)
- Minimize the number of transparent blocks

### Distribution
- Include a README with your pack
- Credit source materials if applicable
- Specify compatible engine versions
- Provide screenshots of your pack in use

## 📚 Examples

### Minimalist Pack
Only override a few blocks:
```json
{
  "name": "Minimalist Stone",
  "priority": 50,
  "materials": [
    {
      "voxelType": "Stone",
      "textures": { "all": "textures/smooth_stone.png" }
    }
  ]
}
```

### High-Detail Pack
Add PBR materials for enhanced visuals:
```json
{
  "voxelType": "Stone",
  "textures": { "all": "textures/stone_albedo.png" },
  "materialProperties": {
    "normalMap": "textures/stone_normal.png",
    "metallicRoughnessMap": "textures/stone_mr.png",
    "metallic": 0.0,
    "roughness": 0.9
  }
}
```

### Themed Pack (Sci-Fi)
Create a unique aesthetic:
```json
{
  "name": "Sci-Fi Industrial",
  "materials": [
    {
      "voxelType": "Stone",
      "name": "Metal Plating",
      "textures": { "all": "textures/metal_plates.png" },
      "materialProperties": {
        "metallic": 0.8,
        "roughness": 0.3
      }
    }
  ]
}
```

## 🐛 Troubleshooting

### Pack Not Loading
- Check `voxel_materials.json` syntax with a JSON validator
- Verify file paths are relative to pack root
- Check console logs for error messages

### Textures Not Appearing
- Ensure PNG files exist at specified paths
- Verify texture file format (PNG with correct channels)
- Check that voxelType names match exactly

### Transparency Issues
- Set `alphaBlend: true` for transparent materials
- Use `alphaCutoffValue` for leaves-style transparency
- Ensure PNG files have alpha channel

### Performance Issues
- Reduce texture resolution
- Use texture atlases
- Limit number of transparent blocks

## 📄 License

This example pack is provided under the MIT License as part of the Fresh Voxel Engine.
Feel free to use it as a template for your own custom packs.

## 🤝 Contributing

Found an issue or have a suggestion? Please contribute to the Fresh Voxel Engine project!

---

**Happy texture crafting!** 🎨✨
