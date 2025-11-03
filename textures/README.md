# Fresh Voxel Engine - Texture Assets

This directory contains placeholder texture assets for the Fresh Voxel Engine.

## Block Textures

All block textures are procedurally generated placeholder assets inspired by popular block games like Minecraft. They are designed to be recognizable and functional for prototype development.

### Available Textures

Each texture is available in three resolutions: 16x16, 32x32, and 64x64 pixels.

#### Basic Blocks
- **stone** - Gray stone texture with noise variation
- **dirt** - Brown earth texture
- **sand** - Sandy yellow texture with grain variation

#### Grass Blocks
- **grass_top** - Green grass top texture
- **grass_side** - Combined dirt and grass side texture

#### Natural Materials
- **wood** - Wood log texture with vertical grain
- **leaves** - Leafy green texture with transparency
- **water** - Blue water texture with transparency (for water blocks)

#### Crafted Blocks
- **cobblestone** - Stone brick pattern texture
- **planks** - Wooden planks with horizontal pattern

#### Special
- **default** - Default fallback texture (16x16 stone)

## Texture Format

- **Format**: PNG with RGBA channels
- **Sizes**: 16x16, 32x32, 64x64 pixels
- **Transparency**: Supported for water and leaves
- **Naming**: `<name>_<size>x<size>.png`

## Usage

```cpp
// Example: Load a texture using the TextureManager
auto& texManager = fresh::TextureManager::getInstance();
auto stoneTexture = texManager.loadTexture("textures/blocks/stone_16x16.png");
```

## Future Improvements

These are placeholder textures. For production quality:
- Add more detailed textures
- Create texture atlases for better performance
- Add normal maps for lighting effects
- Add animated textures (water, lava)
- Add biome-specific texture variations

## License

These procedurally generated textures are part of the Fresh Voxel Engine and are provided under the MIT License. They are suitable for use as placeholder assets during development.

## Credits

Generated using Python PIL (Pillow) library with procedural algorithms inspired by classic voxel game textures.
