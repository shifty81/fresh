#!/bin/bash

# Asset Pack Template Generator for Fresh Voxel Engine
# Usage: ./create_pack.sh my_pack_name

set -e

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if pack name provided
if [ -z "$1" ]; then
    echo -e "${YELLOW}Usage: $0 <pack_name>${NC}"
    echo "Example: $0 my_custom_pack"
    exit 1
fi

PACK_NAME="$1"
PACK_DIR="asset_packs/${PACK_NAME}"

# Check if pack already exists
if [ -d "$PACK_DIR" ]; then
    echo -e "${YELLOW}Error: Pack '${PACK_NAME}' already exists at ${PACK_DIR}${NC}"
    exit 1
fi

echo -e "${BLUE}Creating asset pack: ${PACK_NAME}${NC}"

# Create directory structure
mkdir -p "${PACK_DIR}/textures"

# Create manifest file
cat > "${PACK_DIR}/voxel_materials.json" << 'EOF'
{
  "name": "PACK_NAME_PLACEHOLDER",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "Your custom voxel material pack",
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
        "ao": 1.0,
        "alpha": 1.0
      },
      "includeInAtlas": true
    }
  ]
}
EOF

# Replace placeholder with actual pack name
sed -i "s/PACK_NAME_PLACEHOLDER/${PACK_NAME}/g" "${PACK_DIR}/voxel_materials.json"

# Create README
cat > "${PACK_DIR}/README.md" << EOF
# ${PACK_NAME}

## Description

Add your pack description here.

## Installation

1. Copy this folder to \`asset_packs/${PACK_NAME}/\`
2. Load in the engine:
   \`\`\`cpp
   auto& mgr = VoxelMaterialPackManager::getInstance();
   mgr.initialize("asset_packs");
   mgr.loadPack("asset_packs/${PACK_NAME}");
   \`\`\`

## Textures

List your custom textures here:
- Stone: Custom stone texture

## Credits

Add credits and license information here.

## Version History

### 1.0.0
- Initial release
EOF

# Copy a sample texture from example pack if it exists
if [ -f "asset_packs/example_pack/textures/stone.png" ]; then
    cp "asset_packs/example_pack/textures/stone.png" "${PACK_DIR}/textures/"
    echo -e "${GREEN}✓ Copied sample stone texture${NC}"
fi

echo -e "${GREEN}✓ Asset pack created successfully!${NC}"
echo ""
echo -e "${BLUE}Pack location:${NC} ${PACK_DIR}"
echo -e "${BLUE}Next steps:${NC}"
echo "  1. Edit ${PACK_DIR}/voxel_materials.json"
echo "  2. Add your texture files to ${PACK_DIR}/textures/"
echo "  3. Update README.md with your pack information"
echo ""
echo -e "${GREEN}Happy texture crafting! 🎨${NC}"
