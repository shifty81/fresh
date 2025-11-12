# Asset Pack Directory Structure Visualization

## 📁 Complete Visual Structure

```
fresh/                                    # Repository root
├── asset_packs/                         # Asset packs directory (NEW!)
│   ├── README.md                        # Complete documentation
│   ├── QUICK_REFERENCE.md              # Quick reference guide
│   ├── .gitignore                      # Git ignore rules for packs
│   │
│   ├── example_pack/                    # Example asset pack ✨
│   │   ├── voxel_materials.json        # Pack manifest (REQUIRED)
│   │   ├── README.md                    # Pack documentation
│   │   └── textures/                    # Texture directory
│   │       ├── stone.png                # Stone block (32x32)
│   │       ├── dirt.png                 # Dirt block
│   │       ├── grass_top.png            # Grass - top face
│   │       ├── grass_side.png           # Grass - side faces
│   │       ├── sand.png                 # Sand block
│   │       ├── water.png                # Water block (transparent)
│   │       ├── wood_side.png            # Wood log - bark texture
│   │       ├── wood_top.png             # Wood log - rings texture
│   │       ├── leaves.png               # Leaf block (transparent)
│   │       ├── cobblestone.png          # Cobblestone
│   │       ├── planks.png               # Wooden planks
│   │       ├── glass.png                # Glass block (transparent)
│   │       ├── snow.png                 # Snow block
│   │       └── ice.png                  # Ice block (transparent)
│   │
│   └── your_custom_pack/                # YOUR PACK GOES HERE! 🎨
│       ├── voxel_materials.json         # Your manifest
│       ├── README.md                    # (Optional) Your documentation
│       └── textures/                    # Your textures
│           ├── stone.png
│           ├── dirt.png
│           └── ...
│
├── include/                              # Engine headers
│   └── assets/
│       ├── VoxelMaterialPack.h          # Asset pack system API
│       └── ModularAssetSystem.h         # Modular asset system
│
├── src/                                  # Engine source
│   ├── assets/
│   │   └── VoxelMaterialPack.cpp        # Asset pack implementation
│   └── renderer/
│       └── VoxelTextureLoader.cpp       # Texture loader (FIXED)
│
└── textures/                             # Default engine textures
    ├── blocks/                           # Built-in block textures
    │   ├── stone_16x16.png
    │   ├── stone_32x32.png
    │   ├── stone_64x64.png
    │   └── ... (more blocks in 3 resolutions)
    └── texture_config.json               # Texture configuration
```

## 🎯 Key Files and Their Purpose

### Required Files for Each Pack

| File | Purpose | Required? |
|------|---------|-----------|
| `voxel_materials.json` | Pack manifest with material definitions | ✅ **YES** |
| `textures/` | Directory containing PNG texture files | ✅ **YES** |
| `README.md` | Documentation for your pack | ⭕ Optional |

### Asset Pack Manifest (`voxel_materials.json`)

```
voxel_materials.json
├── "name"          → Pack name
├── "version"       → Pack version (e.g., "1.0.0")
├── "author"        → Your name
├── "description"   → Pack description
├── "priority"      → Priority (higher = overrides others)
└── "materials"     → Array of material definitions
    └── [
         ├── "voxelType"         → Block type (e.g., "Stone")
         ├── "name"              → Display name
         ├── "textures"          → Texture paths
         │   ├── "all"           → Single texture for all faces
         │   ├── OR "top"        → Top face
         │   ├── "bottom"        → Bottom face
         │   └── "sides"         → Side faces
         ├── "materialProperties" → Visual properties
         │   ├── "metallic"      → Metalness (0.0 to 1.0)
         │   ├── "roughness"     → Surface roughness
         │   ├── "alpha"         → Transparency
         │   └── "alphaBlend"    → Enable blending
         └── "includeInAtlas"    → Use in texture atlas
        ]
```

## 📸 Visual Examples

### Example 1: Simple Pack (Minimal)
```
my_simple_pack/
├── voxel_materials.json    # Only override stone
└── textures/
    └── stone.png           # Your custom stone texture
```

**Manifest:**
```json
{
  "name": "My Simple Pack",
  "materials": [
    {
      "voxelType": "Stone",
      "textures": { "all": "textures/stone.png" }
    }
  ]
}
```

### Example 2: Complete Pack (All Blocks)
```
my_complete_pack/
├── voxel_materials.json    # All 14 voxel types
├── README.md               # Your documentation
├── LICENSE.txt             # Your license
├── preview.png             # Screenshot
└── textures/
    ├── stone.png
    ├── dirt.png
    ├── grass_top.png
    ├── grass_side.png
    └── ... (14 total textures)
```

### Example 3: HD Pack with PBR
```
my_hd_pack/
├── voxel_materials.json
└── textures/
    ├── stone.png              # Albedo/diffuse
    ├── stone_normal.png       # Normal map
    ├── stone_mr.png           # Metallic + roughness
    ├── stone_ao.png           # Ambient occlusion
    └── ... (4 maps per block type)
```

## 🎨 Texture Organization Patterns

### Pattern 1: Flat Structure (Simple)
```
textures/
├── stone.png
├── dirt.png
└── grass.png
```

### Pattern 2: Organized by Type (Recommended)
```
textures/
├── stone.png
├── dirt.png
├── grass_top.png
├── grass_side.png
└── grass_bottom.png
```

### Pattern 3: PBR Subdirectories (Advanced)
```
textures/
├── stone/
│   ├── albedo.png
│   ├── normal.png
│   ├── metallic.png
│   └── roughness.png
└── dirt/
    └── ...
```

## 🚀 Quick Setup Workflow

```
Step 1: Copy Example Pack
┌─────────────────────────────────┐
│ cp -r asset_packs/example_pack  │
│       asset_packs/my_pack       │
└─────────────────────────────────┘
              ↓
Step 2: Edit Manifest
┌─────────────────────────────────┐
│ Edit voxel_materials.json       │
│ - Change name, author, version  │
│ - Modify materials as needed    │
└─────────────────────────────────┘
              ↓
Step 3: Replace Textures
┌─────────────────────────────────┐
│ Replace PNG files in textures/  │
│ - Keep same names OR            │
│ - Update paths in manifest      │
└─────────────────────────────────┘
              ↓
Step 4: Test Your Pack
┌─────────────────────────────────┐
│ Load in engine and test         │
│ - Check textures display        │
│ - Verify transparency works     │
│ - Test face-specific textures   │
└─────────────────────────────────┘
              ↓
Step 5: Share (Optional)
┌─────────────────────────────────┐
│ Package and distribute          │
│ - Add README with instructions  │
│ - Include screenshots           │
│ - Specify license               │
└─────────────────────────────────┘
```

## 📊 File Size Reference

| Resolution | Typical Size | Use Case |
|------------|--------------|----------|
| 16x16 | 1-2 KB | Mobile, low-end |
| 32x32 | 2-4 KB | Balanced (recommended) |
| 64x64 | 4-8 KB | High quality |
| 128x128 | 10-20 KB | Ultra HD |
| 256x256 | 40-80 KB | Extreme detail |

**Pack Size Estimates:**
- **Minimal pack** (1-3 blocks): ~10 KB
- **Complete pack** (14 blocks, 32x32): ~50-70 KB
- **HD pack** (14 blocks, 128x128): ~200-300 KB
- **PBR pack** (14 blocks, 4 maps each): ~1-2 MB

## 🎯 Where Things Go

```
┌──────────────────────────────────────────────────────────┐
│                    REPOSITORY ROOT                        │
├──────────────────────────────────────────────────────────┤
│                                                           │
│  asset_packs/          ← YOUR CUSTOM PACKS GO HERE       │
│  ├── example_pack/     ← Reference this for examples     │
│  ├── README.md         ← Read this first                 │
│  └── my_pack/          ← Create your pack here           │
│                                                           │
│  textures/             ← Default engine textures         │
│  └── blocks/           ← Built-in textures (3 sizes)     │
│                                                           │
│  include/assets/       ← API headers                     │
│  └── VoxelMaterialPack.h  ← Asset pack system          │
│                                                           │
│  src/assets/           ← Implementation                  │
│  └── VoxelMaterialPack.cpp                              │
│                                                           │
└──────────────────────────────────────────────────────────┘
```

## 💡 Common Questions

**Q: Where do I put my custom textures?**
A: `asset_packs/your_pack_name/textures/`

**Q: What format should textures be?**
A: PNG format (RGB or RGBA for transparency)

**Q: Can I override just one block?**
A: Yes! Define only the blocks you want to change.

**Q: How do I use multiple packs?**
A: Set different priority values. Higher priority overrides lower.

**Q: Where are the default textures?**
A: In `textures/blocks/` (16x16, 32x32, 64x64)

**Q: Do I need to rebuild the engine?**
A: No! Just add your pack and load it at runtime.

## 📚 Next Steps

1. **Explore** the example pack: `asset_packs/example_pack/`
2. **Read** the documentation: `asset_packs/README.md`
3. **Quick reference**: `asset_packs/QUICK_REFERENCE.md`
4. **Create** your first pack: Copy example_pack and customize!

---

**Ready to create your custom textures?** 🎨

Start with the example pack and make it your own!
