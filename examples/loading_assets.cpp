/**
 * @file loading_assets.cpp
 * @brief Demonstrates loading and using custom asset packs
 * 
 * This example shows:
 * - Loading asset packs from disk
 * - Querying assets by biome and tags
 * - Generating assets for chunks
 * - Placing assets in the world
 * - Auto-discovery of asset packs
 */

#include <iostream>
#include <memory>
#include "assets/ModularAssetSystem.h"
#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Loading Assets ===" << std::endl;
    std::cout << "This example demonstrates the modular asset system." << std::endl;
    
    // Step 1: Initialize asset system
    std::cout << "\n[1/5] Initializing asset system..." << std::endl;
    ModularAssetSystem& assetSystem = ModularAssetSystem::getInstance();
    std::cout << "    ✓ Asset system ready" << std::endl;
    
    // Step 2: Scan and load asset packs
    std::cout << "\n[2/5] Scanning for asset packs..." << std::endl;
    assetSystem.scanAndLoadAssetPacks("assets/");
    
    auto packs = assetSystem.getLoadedPacks();
    std::cout << "    ✓ Loaded " << packs.size() << " asset pack(s)" << std::endl;
    
    for (const auto& pack : packs) {
        std::cout << "      • " << pack.name << " v" << pack.version << std::endl;
        std::cout << "        Assets: " << pack.assets.size() << std::endl;
    }
    
    // Step 3: Query assets by criteria
    std::cout << "\n[3/5] Querying assets..." << std::endl;
    
    // Get all forest assets
    auto forestAssets = assetSystem.queryAssets(BiomeType::Forest);
    std::cout << "    Forest assets: " << forestAssets.size() << std::endl;
    for (const auto& asset : forestAssets) {
        std::cout << "      • " << asset->name << std::endl;
    }
    
    // Get all assets with "tree" tag
    auto treeAssets = assetSystem.queryAssets(BiomeType::Any, "tree");
    std::cout << "\n    Assets tagged 'tree': " << treeAssets.size() << std::endl;
    for (const auto& asset : treeAssets) {
        std::cout << "      • " << asset->name << std::endl;
    }
    
    // Step 4: Generate assets for a chunk
    std::cout << "\n[4/5] Generating assets for chunk..." << std::endl;
    
    auto world = std::make_shared<VoxelWorld>();
    TerrainGenerator terrainGen(12345); // Seed for consistency
    
    int chunkX = 0, chunkZ = 0;
    
    // Generate terrain first
    auto chunk = std::make_unique<Chunk>(chunkX, chunkZ);
    terrainGen.generateChunk(chunk.get(), chunkX, chunkZ);
    world->addChunk(std::move(chunk));
    std::cout << "    ✓ Generated terrain for chunk (0, 0)" << std::endl;
    
    // Generate assets for this chunk
    auto generatedAssets = assetSystem.generateAssetsForChunk(world, chunkX, chunkZ, 12345);
    std::cout << "    ✓ Generated " << generatedAssets.size() << " asset instances" << std::endl;
    
    // Show what was generated
    for (const auto& instance : generatedAssets) {
        std::cout << "      • " << instance.asset->name 
                  << " at (" << instance.position.x << ", " 
                  << instance.position.y << ", " << instance.position.z << ")" << std::endl;
    }
    
    // Step 5: Place assets in world
    std::cout << "\n[5/5] Placing assets in world..." << std::endl;
    
    int placedCount = assetSystem.placeAssetsInWorld(generatedAssets, world);
    std::cout << "    ✓ Placed " << placedCount << " assets in world" << std::endl;
    std::cout << "    (Assets are now part of the voxel world)" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Auto-scanning and loading asset packs" << std::endl;
    std::cout << "  • Querying assets by biome and tags" << std::endl;
    std::cout << "  • Generating asset instances for chunks" << std::endl;
    std::cout << "  • Placing assets into the voxel world" << std::endl;
    std::cout << "  • Integration with terrain generation" << std::endl;
    std::cout << "\nAsset System Features:" << std::endl;
    std::cout << "  • Drop-in asset packs (just add to assets/ folder)" << std::endl;
    std::cout << "  • Automatic discovery and loading" << std::endl;
    std::cout << "  • Biome-specific placement rules" << std::endl;
    std::cout << "  • Smart distance enforcement" << std::endl;
    std::cout << "  • 7 placement rules (Random, Clustered, Grid, etc.)" << std::endl;
    std::cout << "\nPlacement Rules:" << std::endl;
    std::cout << "  • Random - Scattered placement" << std::endl;
    std::cout << "  • Clustered - Groups of assets (forests, rocks)" << std::endl;
    std::cout << "  • Grid - Regular patterns" << std::endl;
    std::cout << "  • NearWater - Close to water sources" << std::endl;
    std::cout << "  • OnHills - Elevated terrain" << std::endl;
    std::cout << "  • InCaves - Underground placement" << std::endl;
    std::cout << "  • RiverSide - Along rivers" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try creating_asset_pack.cpp to make your own" << std::endl;
    std::cout << "  • See COMPLETE_STATUS.md for asset system details" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Asset Packs
 *    - Self-contained folders with manifest.json
 *    - Contains models, textures, metadata
 *    - Drop into assets/ folder for auto-loading
 *    - Version controlled
 * 
 * 2. Manifest Format
 *    - JSON file defining all assets
 *    - Specifies models, textures, placement rules
 *    - Configures spawn chances, biomes, tags
 *    - See COMPLETE_STATUS.md for schema
 * 
 * 3. Asset Discovery
 *    - Automatic scanning of assets/ directory
 *    - Loads all valid manifests
 *    - Validates asset definitions
 *    - Reports errors for invalid packs
 * 
 * 4. Placement Rules
 *    - Control where/how assets spawn
 *    - Biome restrictions
 *    - Distance enforcement
 *    - Group clustering
 *    - Rotation and scaling
 * 
 * 5. World Integration
 *    - Assets generate during world creation
 *    - Placed as actual voxels in world
 *    - Become part of terrain
 *    - Can be modified like any voxel
 * 
 * ASSET PACK STRUCTURE:
 * 
 * MyAssetPack/
 * ├── manifest.json       # Asset definitions
 * ├── models/
 * │   ├── tree.obj
 * │   └── rock.obj
 * └── textures/
 *     ├── tree.png
 *     └── rock.png
 * 
 * MANIFEST EXAMPLE:
 * 
 * {
 *   "name": "NaturePack",
 *   "version": "1.0.0",
 *   "author": "Your Name",
 *   "assets": [
 *     {
 *       "name": "Oak_Tree",
 *       "modelPath": "models/oak.obj",
 *       "texturePath": "textures/oak.png",
 *       "biomes": ["Forest", "Plains"],
 *       "placementRule": "Clustered",
 *       "spawnChance": 0.2,
 *       "minDistance": 5.0,
 *       "groupSize": [3, 8],
 *       "tags": ["tree", "nature"]
 *     }
 *   ]
 * }
 * 
 * QUERYING ASSETS:
 * 
 * // By biome
 * auto forestAssets = assetSystem.queryAssets(BiomeType::Forest);
 * 
 * // By tag
 * auto trees = assetSystem.queryAssets(BiomeType::Any, "tree");
 * 
 * // By biome and tag
 * auto desertRocks = assetSystem.queryAssets(BiomeType::Desert, "rock");
 * 
 * GENERATION WORKFLOW:
 * 
 * 1. Generate base terrain
 * 2. Determine biome for chunk
 * 3. Query assets for that biome
 * 4. Apply placement rules
 * 5. Generate asset instances
 * 6. Place in world as voxels
 * 
 * PLACEMENT RULES DETAIL:
 * 
 * • Random: Uniform distribution, respects minDistance
 * • Clustered: Groups near seed points, natural look
 * • Grid: Regular spacing, good for structures
 * • NearWater: Within range of water voxels
 * • OnHills: Height threshold, slope check
 * • InCaves: Underground, air pockets
 * • RiverSide: Near flowing water
 * 
 * SPAWN CHANCE:
 * 
 * • Value 0.0-1.0 (0-100%)
 * • Higher = more frequent
 * • 0.1 = ~10% of chunks
 * • 1.0 = every chunk (if space)
 * • Combined with placement rule
 * 
 * DISTANCE ENFORCEMENT:
 * 
 * • Prevents assets spawning too close
 * • minDistance in blocks
 * • Checked against all nearby assets
 * • Ensures natural spacing
 * 
 * PERFORMANCE:
 * 
 * • Assets generated once per chunk
 * • Cached after placement
 * • No runtime overhead
 * • Efficient voxel conversion
 * 
 * BEST PRACTICES:
 * 
 * • Use appropriate spawn chances (0.1-0.3)
 * • Set reasonable minDistance (5-15 blocks)
 * • Tag assets for easy querying
 * • Specify correct biomes
 * • Test with different seeds
 * • Keep models optimized
 * 
 * CREATING CUSTOM PACKS:
 * 
 * 1. Create folder: assets/MyPack/
 * 2. Add manifest.json
 * 3. Add models/ and textures/ folders
 * 4. Copy in your assets
 * 5. Engine auto-loads on next run
 * 
 * See creating_asset_pack.cpp for template generation.
 */
