/**
 * @file creating_asset_pack.cpp
 * @brief Demonstrates creating a custom asset pack
 * 
 * This example shows:
 * - Creating an asset pack folder structure
 * - Writing a manifest.json file
 * - Defining assets with placement rules
 * - Testing your asset pack
 * - Best practices for asset creation
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include "assets/ModularAssetSystem.h"

namespace fs = std::filesystem;

/**
 * Create directory if it doesn't exist
 */
bool createDirectory(const std::string& path) {
    try {
        if (!fs::exists(path)) {
            fs::create_directories(path);
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Write manifest.json template
 */
void createManifestTemplate(const std::string& packPath, const std::string& packName) {
    std::string manifestPath = packPath + "/manifest.json";
    std::ofstream manifest(manifestPath);
    
    manifest << "{\n";
    manifest << "  \"name\": \"" << packName << "\",\n";
    manifest << "  \"version\": \"1.0.0\",\n";
    manifest << "  \"author\": \"Your Name\",\n";
    manifest << "  \"description\": \"Description of your asset pack\",\n";
    manifest << "  \"assets\": [\n";
    manifest << "    {\n";
    manifest << "      \"name\": \"Example_Tree\",\n";
    manifest << "      \"modelPath\": \"models/tree.obj\",\n";
    manifest << "      \"texturePath\": \"textures/tree.png\",\n";
    manifest << "      \"biomes\": [\"Forest\", \"Plains\"],\n";
    manifest << "      \"placementRule\": \"Clustered\",\n";
    manifest << "      \"spawnChance\": 0.2,\n";
    manifest << "      \"minDistance\": 5.0,\n";
    manifest << "      \"groupSize\": [3, 8],\n";
    manifest << "      \"randomRotation\": true,\n";
    manifest << "      \"embedInTerrain\": true,\n";
    manifest << "      \"tags\": [\"tree\", \"nature\", \"foliage\"]\n";
    manifest << "    },\n";
    manifest << "    {\n";
    manifest << "      \"name\": \"Example_Rock\",\n";
    manifest << "      \"modelPath\": \"models/rock.obj\",\n";
    manifest << "      \"texturePath\": \"textures/rock.png\",\n";
    manifest << "      \"biomes\": [\"Mountains\", \"Desert\"],\n";
    manifest << "      \"placementRule\": \"Random\",\n";
    manifest << "      \"spawnChance\": 0.15,\n";
    manifest << "      \"minDistance\": 8.0,\n";
    manifest << "      \"randomRotation\": true,\n";
    manifest << "      \"tags\": [\"rock\", \"stone\"]\n";
    manifest << "    }\n";
    manifest << "  ]\n";
    manifest << "}\n";
    
    manifest.close();
    std::cout << "    ✓ Created manifest template: " << manifestPath << std::endl;
}

/**
 * Create README for the asset pack
 */
void createPackReadme(const std::string& packPath, const std::string& packName) {
    std::string readmePath = packPath + "/README.md";
    std::ofstream readme(readmePath);
    
    readme << "# " << packName << " Asset Pack\n\n";
    readme << "## Description\n";
    readme << "This asset pack contains [describe what's in your pack].\n\n";
    readme << "## Contents\n";
    readme << "- **Models**: OBJ format 3D models\n";
    readme << "- **Textures**: PNG format textures\n\n";
    readme << "## Installation\n";
    readme << "1. Copy this folder to `assets/` in your Fresh Voxel Engine directory\n";
    readme << "2. The engine will automatically detect and load the pack\n";
    readme << "3. Start the engine and create/load a world\n\n";
    readme << "## Assets\n";
    readme << "### Example_Tree\n";
    readme << "- **Biomes**: Forest, Plains\n";
    readme << "- **Placement**: Clustered groups\n";
    readme << "- **Spawn Chance**: 20%\n\n";
    readme << "### Example_Rock\n";
    readme << "- **Biomes**: Mountains, Desert\n";
    readme << "- **Placement**: Random scattered\n";
    readme << "- **Spawn Chance**: 15%\n\n";
    readme << "## Customization\n";
    readme << "Edit `manifest.json` to:\n";
    readme << "- Add more assets\n";
    readme << "- Change spawn chances\n";
    readme << "- Adjust placement rules\n";
    readme << "- Modify biome restrictions\n\n";
    readme << "## Credits\n";
    readme << "Created by: Your Name\n";
    readme << "License: [Your License]\n";
    
    readme.close();
    std::cout << "    ✓ Created README: " << readmePath << std::endl;
}

/**
 * Create a placeholder text file for models/textures
 */
void createPlaceholder(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

int main() {
    std::cout << "=== Fresh Voxel Engine: Creating Asset Pack ===" << std::endl;
    std::cout << "This example creates a template asset pack structure." << std::endl;
    
    // Step 1: Get pack name from user (or use default)
    std::cout << "\n[1/5] Asset pack configuration..." << std::endl;
    std::string packName = "MyAssetPack";
    std::cout << "    Pack name: " << packName << std::endl;
    std::cout << "    (You can change this in the code)" << std::endl;
    
    // Step 2: Create directory structure
    std::cout << "\n[2/5] Creating directory structure..." << std::endl;
    std::string basePath = "assets/" + packName;
    
    createDirectory(basePath);
    std::cout << "    ✓ Created: " << basePath << "/" << std::endl;
    
    createDirectory(basePath + "/models");
    std::cout << "    ✓ Created: " << basePath << "/models/" << std::endl;
    
    createDirectory(basePath + "/textures");
    std::cout << "    ✓ Created: " << basePath << "/textures/" << std::endl;
    
    // Step 3: Create manifest.json
    std::cout << "\n[3/5] Creating manifest template..." << std::endl;
    createManifestTemplate(basePath, packName);
    
    // Step 4: Create README
    std::cout << "\n[4/5] Creating documentation..." << std::endl;
    createPackReadme(basePath, packName);
    
    // Step 5: Create placeholder files
    std::cout << "\n[5/5] Creating placeholder files..." << std::endl;
    
    createPlaceholder(basePath + "/models/README.txt", 
        "Place your .obj model files here.\n\n"
        "Tips:\n"
        "- Keep models low-poly for performance\n"
        "- Use consistent scale (1 unit = 1 voxel)\n"
        "- Include normals and UV coordinates\n"
        "- Test models in a 3D viewer first\n");
    std::cout << "    ✓ Created: models/README.txt" << std::endl;
    
    createPlaceholder(basePath + "/textures/README.txt",
        "Place your texture files here (.png, .jpg).\n\n"
        "Tips:\n"
        "- Use power-of-2 dimensions (256x256, 512x512)\n"
        "- Keep file sizes reasonable (<1MB)\n"
        "- Use PNG for transparency\n"
        "- Match texture names to manifest.json\n");
    std::cout << "    ✓ Created: textures/README.txt" << std::endl;
    
    // Summary
    std::cout << "\n=== Asset pack template created successfully! ===" << std::endl;
    std::cout << "\nCreated structure:" << std::endl;
    std::cout << "  " << basePath << "/" << std::endl;
    std::cout << "  ├── manifest.json (asset definitions)" << std::endl;
    std::cout << "  ├── README.md (documentation)" << std::endl;
    std::cout << "  ├── models/ (place .obj files here)" << std::endl;
    std::cout << "  │   └── README.txt" << std::endl;
    std::cout << "  └── textures/ (place .png files here)" << std::endl;
    std::cout << "      └── README.txt" << std::endl;
    
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  1. Add your 3D models to the models/ folder" << std::endl;
    std::cout << "  2. Add your textures to the textures/ folder" << std::endl;
    std::cout << "  3. Edit manifest.json to define your assets" << std::endl;
    std::cout << "  4. Test by running the engine" << std::endl;
    std::cout << "  5. Adjust spawn chances and placement rules" << std::endl;
    
    std::cout << "\nManifest fields explained:" << std::endl;
    std::cout << "  • name: Unique identifier for the asset" << std::endl;
    std::cout << "  • modelPath: Path to .obj file (relative to pack)" << std::endl;
    std::cout << "  • texturePath: Path to texture file" << std::endl;
    std::cout << "  • biomes: Which biomes this asset spawns in" << std::endl;
    std::cout << "  • placementRule: How asset is distributed" << std::endl;
    std::cout << "  • spawnChance: 0.0-1.0 (probability per chunk)" << std::endl;
    std::cout << "  • minDistance: Minimum blocks between instances" << std::endl;
    std::cout << "  • tags: Keywords for querying assets" << std::endl;
    
    std::cout << "\nPlacement rules:" << std::endl;
    std::cout << "  • Random - Scattered uniformly" << std::endl;
    std::cout << "  • Clustered - Groups (forests, rock fields)" << std::endl;
    std::cout << "  • Grid - Regular pattern" << std::endl;
    std::cout << "  • NearWater - Close to water" << std::endl;
    std::cout << "  • OnHills - Elevated terrain" << std::endl;
    std::cout << "  • InCaves - Underground" << std::endl;
    std::cout << "  • RiverSide - Along rivers" << std::endl;
    
    std::cout << "\nTesting your pack:" << std::endl;
    std::cout << "  1. Ensure pack is in assets/ folder" << std::endl;
    std::cout << "  2. Run the engine" << std::endl;
    std::cout << "  3. Create a new world" << std::endl;
    std::cout << "  4. Look for your assets in the world" << std::endl;
    std::cout << "  5. Check console for loading messages" << std::endl;
    
    std::cout << "\nFor more information:" << std::endl;
    std::cout << "  • See loading_assets.cpp for usage" << std::endl;
    std::cout << "  • See COMPLETE_STATUS.md for full documentation" << std::endl;
    std::cout << "  • Check examples/ for more asset examples" << std::endl;
    
    return 0;
}

/**
 * ASSET PACK CREATION GUIDE:
 * 
 * 1. PLANNING YOUR PACK
 *    - Decide theme (nature, structures, decorations)
 *    - List assets you want to create
 *    - Determine target biomes
 *    - Plan spawn frequencies
 * 
 * 2. CREATING 3D MODELS
 *    - Use Blender, Maya, or similar
 *    - Keep poly count low (< 1000 triangles)
 *    - Scale appropriately (1 unit = 1 block)
 *    - Export as .obj with normals and UVs
 * 
 * 3. CREATING TEXTURES
 *    - Match model UV layout
 *    - Use appropriate resolution
 *    - Optimize file size
 *    - Use PNG for alpha channel
 * 
 * 4. WRITING MANIFEST
 *    - One entry per asset
 *    - Correct file paths
 *    - Appropriate spawn chances
 *    - Meaningful tags
 * 
 * 5. TESTING
 *    - Load in engine
 *    - Check console for errors
 *    - Create test world
 *    - Verify assets appear
 *    - Adjust parameters
 * 
 * MANIFEST SCHEMA:
 * 
 * {
 *   "name": "string",              // Pack name
 *   "version": "string",           // Semantic version
 *   "author": "string",            // Your name
 *   "description": "string",       // Brief description
 *   "assets": [
 *     {
 *       "name": "string",          // Unique asset name
 *       "modelPath": "string",     // Path to .obj file
 *       "texturePath": "string",   // Path to texture
 *       "biomes": ["string"],      // List of biome names
 *       "placementRule": "string", // Placement algorithm
 *       "spawnChance": number,     // 0.0-1.0
 *       "minDistance": number,     // Blocks
 *       "groupSize": [min, max],   // For clustered
 *       "randomRotation": bool,    // Random Y rotation
 *       "embedInTerrain": bool,    // Sink into ground
 *       "alignToSurface": bool,    // Rotate to match slope
 *       "tags": ["string"]         // Search keywords
 *     }
 *   ]
 * }
 * 
 * BIOME NAMES:
 * - Forest
 * - Plains
 * - Desert
 * - Mountains
 * - Swamp
 * - Tundra
 * - Ocean
 * - Cave
 * 
 * SPAWN CHANCE GUIDELINES:
 * - 0.05-0.10: Rare (special items)
 * - 0.10-0.20: Uncommon (rocks, bushes)
 * - 0.20-0.40: Common (trees, grass)
 * - 0.40+: Very common (might be too much)
 * 
 * MIN DISTANCE GUIDELINES:
 * - 3-5: Dense placement (grass, flowers)
 * - 5-10: Normal placement (trees, rocks)
 * - 10-20: Sparse placement (large structures)
 * - 20+: Very rare/special
 * 
 * GROUP SIZE (for Clustered):
 * - [2, 4]: Small groups
 * - [3, 8]: Medium groups (forests)
 * - [5, 15]: Large groups
 * 
 * BEST PRACTICES:
 * 
 * • Start small - 2-3 assets first
 * • Test frequently
 * • Use version control
 * • Document your work
 * • Share with community
 * • Accept feedback
 * • Iterate and improve
 * 
 * COMMON MISTAKES:
 * 
 * ❌ Wrong file paths in manifest
 * ❌ Models not scaled correctly
 * ❌ Textures too large
 * ❌ Spawn chance too high
 * ❌ MinDistance too small
 * ❌ Forgetting to test
 * ❌ Invalid JSON syntax
 * 
 * SHARING YOUR PACK:
 * 
 * 1. Test thoroughly
 * 2. Write good README
 * 3. Include license
 * 4. Create preview images
 * 5. Package as ZIP
 * 6. Share on community forums
 * 7. Accept contributions
 */
