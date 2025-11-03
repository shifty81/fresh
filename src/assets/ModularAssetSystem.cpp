#include "assets/ModularAssetSystem.h"
#include "core/ResourceManager.h"
#include "voxel/VoxelWorld.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>

namespace fresh {

// AssetPack implementation
AssetPack::AssetPack(const std::string& packPath) : packPath(packPath) {
}

AssetPack::~AssetPack() {
    unload();
}

bool AssetPack::load() {
    if (loaded) return true;
    
    std::cout << "Loading asset pack from: " << packPath << std::endl;
    
    // Look for manifest file
    std::string manifestPath = packPath + "/manifest.json";
    if (!std::filesystem::exists(manifestPath)) {
        std::cerr << "Manifest not found: " << manifestPath << std::endl;
        return false;
    }
    
    if (!parseManifest(manifestPath)) {
        std::cerr << "Failed to parse manifest: " << manifestPath << std::endl;
        return false;
    }
    
    loaded = true;
    std::cout << "Loaded asset pack: " << name << " v" << version << std::endl;
    std::cout << "  Assets: " << assets.size() << std::endl;
    return true;
}

void AssetPack::unload() {
    if (!loaded) return;
    
    std::cout << "Unloading asset pack: " << name << std::endl;
    assets.clear();
    loaded = false;
}

std::vector<AssetMetadata> AssetPack::getAssetsByBiome(BiomeType biome) const {
    std::vector<AssetMetadata> result;
    for (const auto& asset : assets) {
        if (std::find(asset.allowedBiomes.begin(), asset.allowedBiomes.end(), biome) 
            != asset.allowedBiomes.end() || 
            std::find(asset.allowedBiomes.begin(), asset.allowedBiomes.end(), BiomeType::Any) 
            != asset.allowedBiomes.end()) {
            result.push_back(asset);
        }
    }
    return result;
}

std::vector<AssetMetadata> AssetPack::getAssetsByTag(const std::string& tag) const {
    std::vector<AssetMetadata> result;
    for (const auto& asset : assets) {
        if (std::find(asset.tags.begin(), asset.tags.end(), tag) != asset.tags.end()) {
            result.push_back(asset);
        }
    }
    return result;
}

std::vector<AssetMetadata> AssetPack::getAssetsByRule(PlacementRule rule) const {
    std::vector<AssetMetadata> result;
    for (const auto& asset : assets) {
        if (asset.placementRule == rule) {
            result.push_back(asset);
        }
    }
    return result;
}

bool AssetPack::parseManifest(const std::string& manifestPath) {
    // TODO: Implement JSON parsing
    // For now, create some example assets
    
    name = std::filesystem::path(packPath).filename().string();
    version = "1.0.0";
    author = "Fresh Engine";
    
    // Example tree asset
    AssetMetadata tree;
    tree.name = "Oak_Tree";
    tree.description = "Large oak tree";
    tree.modelPath = packPath + "/models/oak_tree.obj";
    tree.texturePath = packPath + "/textures/oak_tree.png";
    tree.allowedBiomes = {BiomeType::Forest, BiomeType::Plains};
    tree.placementRule = PlacementRule::Clustered;
    tree.spawnChance = 0.15f;
    tree.minDistance = 8.0f;
    tree.minGroupSize = 2;
    tree.maxGroupSize = 8;
    tree.size = glm::vec3(3.0f, 12.0f, 3.0f);
    tree.randomRotation = true;
    tree.placeOnSurface = true;
    tree.tags = {"tree", "vegetation", "natural"};
    assets.push_back(tree);
    
    // Example rock asset
    AssetMetadata rock;
    rock.name = "Boulder";
    rock.description = "Large boulder";
    rock.modelPath = packPath + "/models/boulder.obj";
    rock.texturePath = packPath + "/textures/boulder.png";
    rock.allowedBiomes = {BiomeType::Mountains, BiomeType::Plains};
    rock.placementRule = PlacementRule::Random;
    rock.spawnChance = 0.08f;
    rock.minDistance = 15.0f;
    rock.size = glm::vec3(2.5f, 2.0f, 2.5f);
    rock.randomRotation = true;
    rock.embedInTerrain = true;
    rock.embedDepth = 0.3f;
    rock.tags = {"rock", "natural", "obstacle"};
    assets.push_back(rock);
    
    return true;
}

// ModularAssetSystem implementation
ModularAssetSystem& ModularAssetSystem::getInstance() {
    static ModularAssetSystem instance;
    return instance;
}

void ModularAssetSystem::initialize(const std::string& assetDir) {
    assetDirectory = assetDir;
    
    std::cout << "=== Modular Asset System ===" << std::endl;
    std::cout << "Asset directory: " << assetDirectory << std::endl;
    
    // Register default placement functions
    registerPlacementFunction(PlacementRule::Random, 
        [this](const AssetMetadata& asset, VoxelWorld* world, int cx, int cz, uint32_t seed) {
            return placeRandomly(asset, world, cx, cz, seed);
        });
    
    registerPlacementFunction(PlacementRule::Clustered,
        [this](const AssetMetadata& asset, VoxelWorld* world, int cx, int cz, uint32_t seed) {
            return placeClustered(asset, world, cx, cz, seed);
        });
    
    registerPlacementFunction(PlacementRule::Grid,
        [this](const AssetMetadata& asset, VoxelWorld* world, int cx, int cz, uint32_t seed) {
            return placeGrid(asset, world, cx, cz, seed);
        });
    
    // Scan for asset packs
    scanAndLoadAssetPacks();
}

void ModularAssetSystem::shutdown() {
    std::cout << "Shutting down Modular Asset System..." << std::endl;
    
    for (auto& pack : loadedPacks) {
        pack->unload();
    }
    loadedPacks.clear();
    placedAssetPositions.clear();
}

void ModularAssetSystem::scanAndLoadAssetPacks() {
    if (!std::filesystem::exists(assetDirectory)) {
        std::cout << "Asset directory does not exist, creating: " << assetDirectory << std::endl;
        std::filesystem::create_directories(assetDirectory);
        return;
    }
    
    std::cout << "Scanning for asset packs..." << std::endl;
    
    int packsFound = 0;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(assetDirectory)) {
            if (entry.is_directory()) {
                std::string packPath = entry.path().string();
                if (loadAssetPack(packPath)) {
                    packsFound++;
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error scanning asset directory: " << e.what() << std::endl;
    }
    
    std::cout << "Found and loaded " << packsFound << " asset packs" << std::endl;
}

bool ModularAssetSystem::loadAssetPack(const std::string& packPath) {
    auto pack = std::make_shared<AssetPack>(packPath);
    if (pack->load()) {
        loadedPacks.push_back(pack);
        return true;
    }
    return false;
}

void ModularAssetSystem::unloadAssetPack(const std::string& packName) {
    auto it = std::remove_if(loadedPacks.begin(), loadedPacks.end(),
        [&packName](const std::shared_ptr<AssetPack>& pack) {
            return pack->getName() == packName;
        });
    
    if (it != loadedPacks.end()) {
        loadedPacks.erase(it, loadedPacks.end());
        std::cout << "Unloaded asset pack: " << packName << std::endl;
    }
}

AssetPack* ModularAssetSystem::getAssetPack(const std::string& name) {
    for (auto& pack : loadedPacks) {
        if (pack->getName() == name) {
            return pack.get();
        }
    }
    return nullptr;
}

std::vector<AssetMetadata> ModularAssetSystem::queryAssets(
    BiomeType biome, const std::string& tag, PlacementRule rule) const {
    
    std::vector<AssetMetadata> result;
    
    for (const auto& pack : loadedPacks) {
        const auto& packAssets = pack->getAssets();
        
        for (const auto& asset : packAssets) {
            bool matches = true;
            
            // Filter by biome
            if (biome != BiomeType::Any) {
                bool biomeMatch = std::find(asset.allowedBiomes.begin(), 
                                           asset.allowedBiomes.end(), biome) 
                                 != asset.allowedBiomes.end();
                bool anyBiome = std::find(asset.allowedBiomes.begin(),
                                         asset.allowedBiomes.end(), BiomeType::Any)
                               != asset.allowedBiomes.end();
                matches = matches && (biomeMatch || anyBiome);
            }
            
            // Filter by tag
            if (!tag.empty()) {
                bool tagMatch = std::find(asset.tags.begin(), asset.tags.end(), tag)
                               != asset.tags.end();
                matches = matches && tagMatch;
            }
            
            // Filter by placement rule
            if (rule != PlacementRule::Random) {
                matches = matches && (asset.placementRule == rule);
            }
            
            if (matches) {
                result.push_back(asset);
            }
        }
    }
    
    return result;
}

std::vector<AssetInstance> ModularAssetSystem::generateAssetsForChunk(
    VoxelWorld* world, int chunkX, int chunkZ, uint32_t seed) {
    
    std::vector<AssetInstance> instances;
    if (!world) return instances;
    
    // Create RNG with chunk-specific seed
    std::mt19937 rng(seed ^ (chunkX * 73856093) ^ (chunkZ * 19349663));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Get all available assets
    auto allAssets = queryAssets();
    
    for (const auto& asset : allAssets) {
        // Check spawn chance
        if (dist(rng) > asset.spawnChance) {
            continue;
        }
        
        // Use registered placement function
        auto it = placementFunctions.find(asset.placementRule);
        if (it != placementFunctions.end()) {
            auto assetInstances = it->second(asset, world, chunkX, chunkZ, seed);
            instances.insert(instances.end(), assetInstances.begin(), assetInstances.end());
        }
    }
    
    return instances;
}

void ModularAssetSystem::placeAssetsInWorld(const std::vector<AssetInstance>& instances,
                                           VoxelWorld* world) {
    if (!world) return;
    
    for (const auto& instance : instances) {
        // TODO: Actually place asset in world
        // This would create a visual representation (mesh, entity, etc.)
        std::cout << "Placing asset: " << instance.assetName 
                  << " at (" << instance.position.x << ", " 
                  << instance.position.y << ", " << instance.position.z << ")" << std::endl;
        
        // Track placement
        glm::vec2 pos2D(instance.position.x, instance.position.z);
        placedAssetPositions[instance.assetName].push_back(pos2D);
    }
}

bool ModularAssetSystem::createAssetPackTemplate(const std::string& outputPath) {
    std::cout << "Creating asset pack template at: " << outputPath << std::endl;
    
    // Create directory structure
    std::filesystem::create_directories(outputPath);
    std::filesystem::create_directories(outputPath + "/models");
    std::filesystem::create_directories(outputPath + "/textures");
    
    // Create manifest template
    std::string manifestPath = outputPath + "/manifest.json";
    std::ofstream manifest(manifestPath);
    if (!manifest.is_open()) {
        std::cerr << "Failed to create manifest file" << std::endl;
        return false;
    }
    
    manifest << "{\n";
    manifest << "  \"name\": \"MyAssetPack\",\n";
    manifest << "  \"version\": \"1.0.0\",\n";
    manifest << "  \"author\": \"Your Name\",\n";
    manifest << "  \"description\": \"Description of your asset pack\",\n";
    manifest << "  \"assets\": [\n";
    manifest << "    {\n";
    manifest << "      \"name\": \"ExampleTree\",\n";
    manifest << "      \"description\": \"An example tree asset\",\n";
    manifest << "      \"modelPath\": \"models/tree.obj\",\n";
    manifest << "      \"texturePath\": \"textures/tree.png\",\n";
    manifest << "      \"biomes\": [\"Forest\", \"Plains\"],\n";
    manifest << "      \"placementRule\": \"Clustered\",\n";
    manifest << "      \"spawnChance\": 0.15,\n";
    manifest << "      \"minDistance\": 8.0,\n";
    manifest << "      \"size\": [3.0, 12.0, 3.0],\n";
    manifest << "      \"tags\": [\"tree\", \"vegetation\"]\n";
    manifest << "    }\n";
    manifest << "  ]\n";
    manifest << "}\n";
    manifest.close();
    
    std::cout << "Asset pack template created successfully" << std::endl;
    return true;
}

bool ModularAssetSystem::validateAssetPack(const std::string& packPath) const {
    // Check if directory exists
    if (!std::filesystem::exists(packPath)) {
        std::cerr << "Pack directory does not exist: " << packPath << std::endl;
        return false;
    }
    
    // Check for manifest
    std::string manifestPath = packPath + "/manifest.json";
    if (!std::filesystem::exists(manifestPath)) {
        std::cerr << "Manifest file missing: " << manifestPath << std::endl;
        return false;
    }
    
    // TODO: Validate manifest structure and referenced files
    
    return true;
}

void ModularAssetSystem::printStats() const {
    std::cout << "\n=== Asset System Statistics ===" << std::endl;
    std::cout << "Loaded asset packs: " << loadedPacks.size() << std::endl;
    
    int totalAssets = 0;
    for (const auto& pack : loadedPacks) {
        totalAssets += pack->getAssets().size();
        std::cout << "  " << pack->getName() << " v" << pack->getVersion() 
                  << ": " << pack->getAssets().size() << " assets" << std::endl;
    }
    
    std::cout << "Total assets: " << totalAssets << std::endl;
    std::cout << "Registered placement functions: " << placementFunctions.size() << std::endl;
}

void ModularAssetSystem::registerPlacementFunction(PlacementRule rule, PlacementFunction func) {
    placementFunctions[rule] = func;
    std::cout << "Registered placement function for rule: " << static_cast<int>(rule) << std::endl;
}

// Placement implementations
std::vector<AssetInstance> ModularAssetSystem::placeRandomly(
    const AssetMetadata& asset, VoxelWorld* world, int chunkX, int chunkZ, uint32_t seed) {
    
    std::vector<AssetInstance> instances;
    std::mt19937 rng(seed ^ (chunkX * 73856093) ^ (chunkZ * 19349663));
    std::uniform_real_distribution<float> posDist(0.0f, 16.0f);
    
    const int CHUNK_SIZE = 16;
    glm::vec3 chunkWorldPos(chunkX * CHUNK_SIZE, 0, chunkZ * CHUNK_SIZE);
    
    // Try to place 1-3 instances
    std::uniform_int_distribution<int> countDist(1, 3);
    int attempts = countDist(rng);
    
    for (int i = 0; i < attempts; ++i) {
        glm::vec3 position = chunkWorldPos + glm::vec3(posDist(rng), 0, posDist(rng));
        
        // Get surface height
        // position.y = world->getSurfaceHeight(position.x, position.z);
        position.y = 64.0f; // Placeholder
        
        BiomeType biome = determineBiome(world, position);
        
        if (canPlaceAsset(asset, position, world, biome)) {
            AssetInstance instance;
            instance.assetName = asset.name;
            instance.position = position;
            instance.rotation = asset.randomRotation ? 
                glm::vec3(0, posDist(rng) * 360.0f, 0) : glm::vec3(0);
            instance.biome = biome;
            instances.push_back(instance);
        }
    }
    
    return instances;
}

std::vector<AssetInstance> ModularAssetSystem::placeClustered(
    const AssetMetadata& asset, VoxelWorld* world, int chunkX, int chunkZ, uint32_t seed) {
    
    std::vector<AssetInstance> instances;
    std::mt19937 rng(seed ^ (chunkX * 73856093) ^ (chunkZ * 19349663));
    std::uniform_real_distribution<float> posDist(0.0f, 16.0f);
    std::uniform_int_distribution<int> sizeDist(asset.minGroupSize, asset.maxGroupSize);
    
    const int CHUNK_SIZE = 16;
    glm::vec3 chunkWorldPos(chunkX * CHUNK_SIZE, 0, chunkZ * CHUNK_SIZE);
    
    // Place cluster center
    glm::vec3 clusterCenter = chunkWorldPos + glm::vec3(posDist(rng), 0, posDist(rng));
    clusterCenter.y = 64.0f; // world->getSurfaceHeight(clusterCenter.x, clusterCenter.z);
    
    int clusterSize = sizeDist(rng);
    float clusterRadius = asset.minDistance * 0.5f;
    
    for (int i = 0; i < clusterSize; ++i) {
        // Offset from cluster center
        std::uniform_real_distribution<float> offsetDist(-clusterRadius, clusterRadius);
        glm::vec3 position = clusterCenter;
        position.x += offsetDist(rng);
        position.z += offsetDist(rng);
        position.y = 64.0f; // world->getSurfaceHeight(position.x, position.z);
        
        BiomeType biome = determineBiome(world, position);
        
        if (canPlaceAsset(asset, position, world, biome)) {
            AssetInstance instance;
            instance.assetName = asset.name;
            instance.position = position;
            instance.rotation = asset.randomRotation ?
                glm::vec3(0, posDist(rng) * 360.0f, 0) : glm::vec3(0);
            instance.biome = biome;
            instances.push_back(instance);
        }
    }
    
    return instances;
}

std::vector<AssetInstance> ModularAssetSystem::placeGrid(
    const AssetMetadata& asset, VoxelWorld* world, int chunkX, int chunkZ, uint32_t seed) {
    
    std::vector<AssetInstance> instances;
    const int CHUNK_SIZE = 16;
    glm::vec3 chunkWorldPos(chunkX * CHUNK_SIZE, 0, chunkZ * CHUNK_SIZE);
    
    float spacing = asset.minDistance;
    
    for (float x = 0; x < CHUNK_SIZE; x += spacing) {
        for (float z = 0; z < CHUNK_SIZE; z += spacing) {
            glm::vec3 position = chunkWorldPos + glm::vec3(x, 0, z);
            position.y = 64.0f; // world->getSurfaceHeight(position.x, position.z);
            
            BiomeType biome = determineBiome(world, position);
            
            if (canPlaceAsset(asset, position, world, biome)) {
                AssetInstance instance;
                instance.assetName = asset.name;
                instance.position = position;
                instance.rotation = glm::vec3(0);
                instance.biome = biome;
                instances.push_back(instance);
            }
        }
    }
    
    return instances;
}

bool ModularAssetSystem::canPlaceAsset(const AssetMetadata& asset, 
                                       const glm::vec3& position,
                                      VoxelWorld* world, BiomeType biome) {
    // Check biome compatibility
    bool biomeAllowed = std::find(asset.allowedBiomes.begin(), 
                                  asset.allowedBiomes.end(), biome) 
                       != asset.allowedBiomes.end();
    bool anyBiome = std::find(asset.allowedBiomes.begin(),
                             asset.allowedBiomes.end(), BiomeType::Any)
                   != asset.allowedBiomes.end();
    
    if (!biomeAllowed && !anyBiome) {
        return false;
    }
    
    // Check minimum distance to other assets of same type
    glm::vec2 pos2D(position.x, position.z);
    auto it = placedAssetPositions.find(asset.name);
    if (it != placedAssetPositions.end()) {
        for (const auto& placedPos : it->second) {
            float dist = glm::distance(pos2D, placedPos);
            if (dist < asset.minDistance) {
                return false;
            }
        }
    }
    
    // TODO: Additional checks (terrain slope, water proximity, etc.)
    
    return true;
}

BiomeType ModularAssetSystem::determineBiome(VoxelWorld* world, const glm::vec3& position) {
    // TODO: Query world for actual biome
    // For now, return a placeholder
    return BiomeType::Plains;
}

} // namespace fresh
