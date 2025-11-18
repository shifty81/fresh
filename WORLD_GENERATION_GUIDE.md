# Fresh Voxel Engine - World Generation Guide
## Complete Guide to Procedural World Generation Types

> **Document Version:** 1.1.0  
> **Last Updated:** 2025-11-18  
> **Purpose:** Comprehensive guide to all world generation types, including 3D, 2D, and hybrid approaches

> 📘 **See Also:** [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md) - Design document for advanced features inspired by Vintage Story

---

## 📊 Executive Summary

Fresh Voxel Engine supports multiple world generation paradigms:
- **3D Voxel Worlds** - Traditional Minecraft-style infinite terrain
- **2D Voxel Worlds** - Side-scrolling platformer style (NEW!)
- **Hybrid Worlds** - Mixed 2D/3D gameplay
- **Custom Generators** - Fully customizable generation

### Current Implementation Status
- **3D Basic Generation:** ✅ 85% Complete
- **3D Advanced Generation:** 🟨 30% Complete (see [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md))
- **2D Generation:** ❌ 0% Complete (NEW FEATURE)
- **Structure Generation:** ❌ 0% Complete
- **Custom Generators:** ✅ 100% Complete (API exists)

### Future Advanced Features (Design Phase)
See [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md) for detailed design documentation on:
- **Climate Simulation Systems** - Temperature and rainfall-based biomes
- **Geological Layering** - Realistic rock strata and ore distribution
- **Hydrological Systems** - Rivers, lakes, and water flow simulation
- **Structure & Lore Generation** - Procedural ruins and storytelling
- **LOD Rendering** - Distant terrain rendering for larger view distances
- **JSON Moddability** - Data-driven world generation configuration

---

## 🌍 1. 3D Voxel World Generation

### 1.1 Current Implementation (Basic 3D)

#### What's Working
```cpp
// Current TerrainGenerator features
- Perlin noise heightmap generation
- Multi-octave fractal noise
- Basic biome system (height-based)
- 3D cave generation
- Chunk-based streaming (16×256×16)
- Configurable seed
```

#### Example Usage
```cpp
// Create a 3D world with seed
WorldGenerator generator(12345);
generator.setWorldType(WorldType::INFINITE_3D);
generator.setBiomes({BiomeType::PLAINS, BiomeType::MOUNTAINS});
generator.setCaveFrequency(0.3f);

// Generate chunk
Chunk* chunk = generator.generateChunk(chunkX, chunkZ);
```

#### Current Biomes
1. **Plains** - Flat grasslands (height: 64-70)
2. **Hills** - Rolling terrain (height: 70-90)
3. **Mountains** - Steep peaks (height: 90-200)
4. **Valleys** - Low areas (height: 50-64)

---

### 1.2 Advanced 3D Generation (Planned - Phase 8)

#### 3D Density-Based Generation

**What It Is:**
Instead of 2D heightmaps, use 3D density functions to determine voxel placement. This enables:
- Overhangs and floating islands
- Natural arches
- More realistic caves
- Underground structures

**Proposed Implementation:**
```cpp
class DensityGenerator {
public:
    // Generate density at any 3D point
    float getDensity(int x, int y, int z) const {
        float continentalness = getContinentalness(x, z);
        float erosion = getErosion(x, z);
        float peaksValleys = getPeaksValleys(x, z);
        
        // Combine multiple noise layers
        float density = 0.0f;
        density += continentalness * 0.5f;
        density += erosion * 0.3f;
        density += peaksValleys * 0.2f;
        
        // Apply height factor
        float heightFactor = (float)y / 256.0f;
        density -= heightFactor * 2.0f;
        
        return density;
    }
    
    VoxelType getVoxelAt(int x, int y, int z) const {
        float density = getDensity(x, y, z);
        if (density > 0.0f) {
            return VoxelType::Stone; // Solid
        }
        return VoxelType::Air; // Empty
    }
    
private:
    Noise continentalnessNoise;
    Noise erosionNoise;
    Noise peaksValleysNoise;
};
```

#### Biome System 2.0

**Temperature × Humidity Grid:**
```
              Cold         Temperate      Hot
    Dry    |  Tundra    |  Plains     |  Desert    |
    Normal |  Taiga     |  Forest     |  Savanna   |
    Wet    |  SnowForest|  Jungle     |  Swamp     |
```

**Proposed Implementation:**
```cpp
enum class BiomeType {
    // Cold biomes
    TUNDRA,
    SNOW_PLAINS,
    TAIGA,
    SNOW_FOREST,
    FROZEN_OCEAN,
    
    // Temperate biomes
    PLAINS,
    FOREST,
    DENSE_FOREST,
    HILLS,
    MOUNTAINS,
    
    // Hot biomes
    DESERT,
    HOT_DESERT,
    SAVANNA,
    MESA,
    
    // Wet biomes
    SWAMP,
    JUNGLE,
    RAINFOREST,
    MANGROVE,
    
    // Water biomes
    OCEAN,
    DEEP_OCEAN,
    BEACH,
    RIVER
};

class BiomeGenerator {
public:
    BiomeType getBiome(int x, int z) const {
        float temperature = temperatureNoise.get(x, z);
        float humidity = humidityNoise.get(x, z);
        
        // Use 2D lookup table
        return biomeTable[getTemperatureIndex(temperature)]
                         [getHumidityIndex(humidity)];
    }
    
    float getTemperature(int x, int z) const;
    float getHumidity(int x, int z) const;
    
private:
    Noise temperatureNoise;
    Noise humidityNoise;
    BiomeType biomeTable[3][3]; // Temp × Humidity
};
```

---

### 1.3 Water Bodies & Rivers

#### Ocean Generation
```cpp
class OceanGenerator {
public:
    void generateOcean(Chunk& chunk, int seaLevel = 64) {
        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                int height = heightMap[x][z];
                if (height < seaLevel) {
                    // Fill with water up to sea level
                    for (int y = height; y < seaLevel; y++) {
                        chunk.setVoxel(x, y, z, VoxelType::Water);
                    }
                }
            }
        }
    }
};
```

#### River Generation
**Algorithm:**
1. Start from high points (mountains)
2. Flow downhill following gradient
3. Carve river bed
4. Add water source blocks
5. Connect to ocean/lake

```cpp
class RiverGenerator {
public:
    void generateRiver(World& world, const glm::ivec2& start) {
        std::vector<glm::ivec2> riverPath;
        glm::ivec2 current = start;
        
        // Flow downhill until reaching ocean
        while (getHeight(current) > seaLevel) {
            riverPath.push_back(current);
            current = findLowestNeighbor(current);
        }
        
        // Carve river bed
        for (const auto& pos : riverPath) {
            carveRiverBed(world, pos, riverWidth);
        }
        
        // Add water
        for (const auto& pos : riverPath) {
            addWater(world, pos);
        }
    }
    
private:
    int riverWidth = 3;
    int seaLevel = 64;
};
```

---

### 1.4 Vegetation & Tree Generation

#### L-System Tree Generator
```cpp
class LSystemTree {
public:
    struct Rule {
        char symbol;
        std::string replacement;
    };
    
    void addRule(char symbol, const std::string& replacement) {
        rules[symbol] = replacement;
    }
    
    std::string generate(int iterations) const {
        std::string current = axiom;
        for (int i = 0; i < iterations; i++) {
            current = applyRules(current);
        }
        return current;
    }
    
    void buildTree(World& world, const glm::ivec3& position) {
        std::string lString = generate(4);
        interpretString(world, position, lString);
    }
    
private:
    std::string axiom = "F";
    std::map<char, std::string> rules;
    
    // F = Forward (trunk)
    // [ = Push state
    // ] = Pop state
    // + = Turn right
    // - = Turn left
    // ^ = Pitch up
    // v = Pitch down
};

// Example: Simple tree
LSystemTree oakTree;
oakTree.addRule('F', "FF+[+F-F-F]-[-F+F+F]");
```

#### Predefined Tree Types
```cpp
enum class TreeType {
    OAK,        // Broad, round canopy
    PINE,       // Tall, conical shape
    BIRCH,      // Thin, tall trunk
    JUNGLE,     // Very tall, dense foliage
    ACACIA,     // Flat-topped
    DARK_OAK,   // Thick trunk, dark leaves
    CHERRY,     // Pink foliage (decorative)
    PALM        // Tropical, coconuts
};

class TreeGenerator {
public:
    void generateTree(World& world, const glm::ivec3& position, 
                      TreeType type, int seed);
                      
private:
    void generateOak(World& world, const glm::ivec3& pos);
    void generatePine(World& world, const glm::ivec3& pos);
    void generateJungle(World& world, const glm::ivec3& pos);
    // ...
};
```

---

### 1.5 Structure Generation

#### Structure Template System
```cpp
struct StructureTemplate {
    std::string name;
    glm::ivec3 size;
    std::vector<VoxelType> voxels;
    std::vector<glm::ivec3> doorPositions;
    std::vector<glm::ivec3> windowPositions;
};

class StructureGenerator {
public:
    void loadTemplate(const std::string& filename);
    void placeStructure(World& world, const glm::ivec3& position,
                       int rotation = 0);
                       
    bool canPlaceStructure(const World& world, 
                          const glm::ivec3& position) const;
                          
private:
    std::map<std::string, StructureTemplate> templates;
};
```

#### Jigsaw Generation
**What It Is:** Modular piece assembly (like Minecraft villages)

```cpp
class JigsawGenerator {
public:
    struct JigsawPiece {
        StructureTemplate structure;
        std::vector<glm::ivec3> connectionPoints;
        std::string pieceType; // "house", "road", "farm"
    };
    
    void generateStructure(World& world, const glm::ivec3& start,
                          const std::string& startPiece) {
        std::queue<GenerationTask> tasks;
        tasks.push({start, startPiece, 0});
        
        while (!tasks.empty() && tasks.front().depth < maxDepth) {
            auto task = tasks.front();
            tasks.pop();
            
            // Place piece
            placePiece(world, task.position, task.pieceName);
            
            // Find connection points
            auto piece = getPiece(task.pieceName);
            for (const auto& connection : piece.connectionPoints) {
                // Try to connect compatible pieces
                auto nextPiece = findCompatiblePiece(piece, connection);
                if (nextPiece) {
                    tasks.push({connection, nextPiece->name, task.depth + 1});
                }
            }
        }
    }
    
private:
    std::vector<JigsawPiece> pieces;
    int maxDepth = 10;
};
```

#### Structure Types
1. **Villages**
   - Houses (various sizes)
   - Roads
   - Farms
   - Well
   - Town center
   
2. **Dungeons**
   - Corridors
   - Rooms
   - Treasure rooms
   - Trap rooms
   - Boss room
   
3. **Ruins**
   - Broken walls
   - Collapsed floors
   - Overgrown structures
   
4. **Towers**
   - Multiple floors
   - Spiral stairs
   - Top platform
   
5. **Temples**
   - Grand entrance
   - Puzzle rooms
   - Treasure vault

---

## 🎮 2. 2D Voxel World Generation (NEW!)

### 2.1 Side-Scrolling Platformer Style

**What It Is:**
Generate a 2D voxel world with depth of 1-3 blocks, perfect for platformer games like Terraria or Starbound.

#### Proposed Implementation
```cpp
class World2DGenerator {
public:
    enum class Style {
        PLATFORMER,     // Terraria-style
        METROIDVANIA,   // Connected areas
        RUNNER,         // Endless runner
        PUZZLE          // Room-based puzzles
    };
    
    void setStyle(Style style) { this->style = style; }
    void setWidth(int width) { this->worldWidth = width; }
    void setHeight(int height) { this->worldHeight = height; }
    void setDepth(int depth) { this->worldDepth = depth; } // Usually 1-3
    
    void generate(World& world);
    
private:
    Style style = Style::PLATFORMER;
    int worldWidth = 1024;   // Blocks wide
    int worldHeight = 256;   // Blocks tall
    int worldDepth = 1;      // Blocks deep (Z-axis)
    
    void generatePlatformer(World& world);
    void generateMetroidvania(World& world);
    void generateRunner(World& world);
    void generatePuzzle(World& world);
};
```

---

### 2.2 Terraria-Style Generation

#### Core Algorithm
```cpp
void World2DGenerator::generatePlatformer(World& world) {
    // Phase 1: Surface terrain
    generateSurface(world);
    
    // Phase 2: Underground layers
    generateUnderground(world);
    
    // Phase 3: Caves and tunnels
    generateCaves(world);
    
    // Phase 4: Ore deposits
    generateOres(world);
    
    // Phase 5: Underground structures
    generateDungeons(world);
    
    // Phase 6: Surface features
    generateTrees(world);
    generateBuildings(world);
    
    // Phase 7: Background walls
    generateBackgroundWalls(world);
}
```

#### Surface Generation
```cpp
void World2DGenerator::generateSurface(World& world) {
    const int surfaceLevel = worldHeight / 2;
    
    for (int x = 0; x < worldWidth; x++) {
        // Use Perlin noise for height variation
        float noise = perlinNoise.get(x * 0.01f);
        int height = surfaceLevel + (int)(noise * 30.0f);
        
        // Fill from bottom to surface
        for (int y = 0; y < height; y++) {
            VoxelType type = getVoxelTypeForDepth(y, height);
            
            // Place in all depth layers for 2D
            for (int z = 0; z < worldDepth; z++) {
                world.setVoxel(x, y, z, type);
            }
        }
    }
}

VoxelType World2DGenerator::getVoxelTypeForDepth(int y, int surfaceHeight) {
    int depthFromSurface = surfaceHeight - y;
    
    if (depthFromSurface == 0) return VoxelType::Grass;
    if (depthFromSurface < 3) return VoxelType::Dirt;
    return VoxelType::Stone;
}
```

#### Underground Layers
```cpp
struct Layer {
    int minY, maxY;
    VoxelType primaryType;
    float oreChance;
    std::vector<VoxelType> oreTypes;
};

void World2DGenerator::generateUnderground(World& world) {
    std::vector<Layer> layers = {
        {0, 20, VoxelType::Bedrock, 0.0f, {}},
        {20, 50, VoxelType::DeepStone, 0.05f, {VoxelType::Diamond, VoxelType::Gold}},
        {50, 80, VoxelType::Stone, 0.1f, {VoxelType::Gold, VoxelType::Iron}},
        {80, 120, VoxelType::Stone, 0.15f, {VoxelType::Iron, VoxelType::Coal}}
    };
    
    for (const auto& layer : layers) {
        fillLayer(world, layer);
    }
}
```

#### Cave Generation (2D)
```cpp
void World2DGenerator::generateCaves(World& world) {
    // Use cellular automata for organic caves
    
    // Phase 1: Random seed
    for (int x = 0; x < worldWidth; x++) {
        for (int y = 20; y < worldHeight - 20; y++) {
            if (random() < 0.45f) {
                for (int z = 0; z < worldDepth; z++) {
                    world.setVoxel(x, y, z, VoxelType::Air);
                }
            }
        }
    }
    
    // Phase 2: Cellular automata (4-5 iterations)
    for (int iteration = 0; iteration < 5; iteration++) {
        applyCellularAutomata(world);
    }
}

void World2DGenerator::applyCellularAutomata(World& world) {
    auto worldCopy = world.clone();
    
    for (int x = 1; x < worldWidth - 1; x++) {
        for (int y = 1; y < worldHeight - 1; y++) {
            int solidNeighbors = countSolidNeighbors(worldCopy, x, y);
            
            // Birth rule: 4+ solid neighbors → solid
            // Death rule: <4 solid neighbors → air
            if (solidNeighbors >= 4) {
                world.setVoxel(x, y, 0, VoxelType::Stone);
            } else {
                world.setVoxel(x, y, 0, VoxelType::Air);
            }
        }
    }
}
```

---

### 2.3 Metroidvania-Style Generation

**What It Is:**
Connected areas with different themes, requiring specific abilities to progress.

```cpp
class MetroidvaniaGenerator {
public:
    struct Room {
        glm::ivec2 position;
        glm::ivec2 size;
        RoomType type;
        std::vector<Door> doors;
        AbilityRequired requiredAbility;
    };
    
    enum class RoomType {
        START,
        NORMAL,
        TREASURE,
        BOSS,
        SAVE_POINT,
        ABILITY_UNLOCK
    };
    
    enum class AbilityRequired {
        NONE,
        DOUBLE_JUMP,
        WALL_JUMP,
        DASH,
        SWIM,
        GRAPPLE
    };
    
    void generate(World& world) {
        // 1. Create main path
        std::vector<Room> mainPath = generateMainPath();
        
        // 2. Add side areas
        std::vector<Room> sideAreas = generateSideAreas(mainPath);
        
        // 3. Connect rooms
        connectRooms(mainPath, sideAreas);
        
        // 4. Place abilities
        placeAbilityUnlocks(mainPath, sideAreas);
        
        // 5. Add enemies and traps
        populateRooms(mainPath, sideAreas);
        
        // 6. Build rooms in world
        buildRooms(world, mainPath);
        buildRooms(world, sideAreas);
    }
    
private:
    int roomMinSize = 20;
    int roomMaxSize = 50;
};
```

---

### 2.4 Endless Runner Generation

**What It Is:**
Procedurally generate chunks ahead of the player for infinite side-scrolling.

```cpp
class EndlessRunnerGenerator {
public:
    struct Segment {
        int length;
        float difficulty;
        std::vector<Obstacle> obstacles;
        std::vector<Platform> platforms;
        std::vector<Collectible> collectibles;
    };
    
    Segment generateNextSegment(float currentDifficulty) {
        Segment segment;
        segment.length = random(50, 100);
        segment.difficulty = currentDifficulty;
        
        // Generate obstacles based on difficulty
        int obstacleCount = (int)(currentDifficulty * 10);
        for (int i = 0; i < obstacleCount; i++) {
            segment.obstacles.push_back(generateObstacle(segment.difficulty));
        }
        
        // Add platforms for jumping
        int platformCount = random(3, 8);
        for (int i = 0; i < platformCount; i++) {
            segment.platforms.push_back(generatePlatform());
        }
        
        // Add collectibles
        int collectibleCount = random(5, 15);
        for (int i = 0; i < collectibleCount; i++) {
            segment.collectibles.push_back(generateCollectible());
        }
        
        return segment;
    }
    
    void buildSegment(World& world, const Segment& segment, int startX) {
        // Build floor
        for (int x = 0; x < segment.length; x++) {
            world.setVoxel(startX + x, 0, 0, VoxelType::Stone);
        }
        
        // Place obstacles
        for (const auto& obstacle : segment.obstacles) {
            placeObstacle(world, startX + obstacle.x, obstacle);
        }
        
        // Place platforms
        for (const auto& platform : segment.platforms) {
            placePlatform(world, startX + platform.x, platform);
        }
    }
    
private:
    float difficultyIncrease = 0.01f;
};
```

---

## 🔀 3. Hybrid World Generation

### 3.1 Mixed 2D/3D Gameplay

**What It Is:**
Combine 2D side-scrolling with 3D exploration areas.

```cpp
class HybridWorldGenerator {
public:
    struct Zone {
        glm::ivec3 position;
        glm::ivec3 size;
        ZoneType type;
    };
    
    enum class ZoneType {
        PLATFORMER_2D,  // Side-scrolling area
        EXPLORATION_3D, // Full 3D area
        TRANSITION      // Connects 2D and 3D
    };
    
    void generate(World& world) {
        // Create zones
        std::vector<Zone> zones;
        
        // Main hub (3D)
        zones.push_back({glm::ivec3(0, 64, 0), 
                        glm::ivec3(100, 50, 100), 
                        ZoneType::EXPLORATION_3D});
        
        // Side-scrolling level 1 (2D)
        zones.push_back({glm::ivec3(100, 64, 0), 
                        glm::ivec3(200, 50, 3), 
                        ZoneType::PLATFORMER_2D});
        
        // Generate each zone
        for (const auto& zone : zones) {
            generateZone(world, zone);
        }
        
        // Connect zones with transitions
        createTransitions(world, zones);
    }
    
private:
    World2DGenerator gen2D;
    TerrainGenerator gen3D;
};
```

---

## 🛠️ 4. Custom World Generation

### 4.1 Generator API

```cpp
class IWorldGenerator {
public:
    virtual ~IWorldGenerator() = default;
    
    // Override these methods
    virtual void initialize(uint64_t seed) = 0;
    virtual void generateChunk(Chunk& chunk, int chunkX, int chunkZ) = 0;
    virtual BiomeType getBiome(int x, int z) = 0;
    
    // Optional overrides
    virtual void populateChunk(Chunk& chunk, int chunkX, int chunkZ) {}
    virtual void decorateChunk(Chunk& chunk, int chunkX, int chunkZ) {}
};

// Example: Custom flat world generator
class FlatWorldGenerator : public IWorldGenerator {
public:
    void initialize(uint64_t seed) override {
        this->seed = seed;
    }
    
    void generateChunk(Chunk& chunk, int chunkX, int chunkZ) override {
        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                // Bedrock layer
                chunk.setVoxel(x, 0, z, VoxelType::Bedrock);
                
                // Stone layers
                for (int y = 1; y < 60; y++) {
                    chunk.setVoxel(x, y, z, VoxelType::Stone);
                }
                
                // Dirt layers
                for (int y = 60; y < 63; y++) {
                    chunk.setVoxel(x, y, z, VoxelType::Dirt);
                }
                
                // Grass top
                chunk.setVoxel(x, 63, z, VoxelType::Grass);
            }
        }
    }
    
    BiomeType getBiome(int x, int z) override {
        return BiomeType::PLAINS;
    }
    
private:
    uint64_t seed;
};
```

---

### 4.2 Example Custom Generators

#### Sky Island Generator
```cpp
class SkyIslandGenerator : public IWorldGenerator {
public:
    void generateChunk(Chunk& chunk, int chunkX, int chunkZ) override {
        // Generate floating islands at various heights
        
        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                float noise = islandNoise.get(
                    (chunkX * 16 + x) * 0.02f,
                    (chunkZ * 16 + z) * 0.02f
                );
                
                if (noise > 0.5f) {
                    // Create island at this location
                    int islandHeight = 100 + (int)(noise * 50);
                    int islandThickness = 10 + (int)(noise * 20);
                    
                    for (int y = islandHeight - islandThickness; 
                         y < islandHeight; y++) {
                        chunk.setVoxel(x, y, z, VoxelType::Stone);
                    }
                    
                    // Grass on top
                    chunk.setVoxel(x, islandHeight, z, VoxelType::Grass);
                }
            }
        }
    }
    
private:
    Noise islandNoise;
};
```

#### Mushroom World Generator
```cpp
class MushroomWorldGenerator : public IWorldGenerator {
public:
    void generateChunk(Chunk& chunk, int chunkX, int chunkZ) override {
        // Generate flat mycelium ground
        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                for (int y = 0; y < 64; y++) {
                    chunk.setVoxel(x, y, z, VoxelType::Stone);
                }
                chunk.setVoxel(x, 64, z, VoxelType::Mycelium);
            }
        }
    }
    
    void decorateChunk(Chunk& chunk, int chunkX, int chunkZ) override {
        // Add giant mushrooms
        int mushroomCount = random(0, 3);
        for (int i = 0; i < mushroomCount; i++) {
            int x = random(0, 15);
            int z = random(0, 15);
            generateGiantMushroom(chunk, x, 65, z);
        }
    }
    
private:
    void generateGiantMushroom(Chunk& chunk, int x, int y, int z);
};
```

---

## 🎛️ 5. World Generation Configuration

### 5.1 World Presets

```cpp
struct WorldPreset {
    std::string name;
    std::string description;
    WorldType type;
    GeneratorSettings settings;
};

std::vector<WorldPreset> presets = {
    {
        "Default",
        "Standard 3D world with varied terrain",
        WorldType::INFINITE_3D,
        {
            .seed = 0,
            .biomes = {BiomeType::PLAINS, BiomeType::FOREST, BiomeType::MOUNTAINS},
            .caveFrequency = 0.3f,
            .structureFrequency = 0.05f,
            .worldHeight = 256
        }
    },
    {
        "Superflat",
        "Completely flat world for building",
        WorldType::FLAT,
        {
            .seed = 0,
            .flatLayers = {
                {VoxelType::Bedrock, 1},
                {VoxelType::Stone, 59},
                {VoxelType::Dirt, 3},
                {VoxelType::Grass, 1}
            }
        }
    },
    {
        "Sky Islands",
        "Floating islands in the sky",
        WorldType::SKY_ISLANDS,
        {
            .seed = 0,
            .islandFrequency = 0.1f,
            .islandMinHeight = 100,
            .islandMaxHeight = 200
        }
    },
    {
        "Terraria Style",
        "2D side-scrolling world",
        WorldType::PLATFORMER_2D,
        {
            .seed = 0,
            .worldWidth = 1024,
            .worldHeight = 256,
            .worldDepth = 1,
            .dungeonCount = 3
        }
    },
    {
        "Cave World",
        "Everything is underground",
        WorldType::CAVE_WORLD,
        {
            .seed = 0,
            .caveFrequency = 0.8f,
            .surfaceLevel = 0
        }
    }
};
```

---

### 5.2 Configuration Files

**Example: world_config.json**
```json
{
  "worldName": "My Custom World",
  "worldType": "INFINITE_3D",
  "seed": 12345,
  "settings": {
    "biomes": [
      "PLAINS",
      "FOREST",
      "MOUNTAINS",
      "DESERT",
      "OCEAN"
    ],
    "generation": {
      "caveFrequency": 0.3,
      "structureFrequency": 0.05,
      "treeFrequency": 0.2,
      "oreFrequency": 0.1
    },
    "worldSize": {
      "height": 256,
      "renderDistance": 8
    },
    "advanced": {
      "use3DDensity": true,
      "enableRivers": true,
      "enableStructures": true,
      "enableFoliage": true
    }
  }
}
```

---

## 📊 6. Performance Optimization

### 6.1 Multi-threaded Generation

```cpp
class ThreadedWorldGenerator {
public:
    void initialize(int threadCount = 4) {
        for (int i = 0; i < threadCount; i++) {
            workers.emplace_back([this]() { workerThread(); });
        }
    }
    
    void queueChunk(int chunkX, int chunkZ, int priority) {
        std::lock_guard<std::mutex> lock(queueMutex);
        chunkQueue.push({chunkX, chunkZ, priority});
        cv.notify_one();
    }
    
    Chunk* getGeneratedChunk(int chunkX, int chunkZ) {
        std::lock_guard<std::mutex> lock(completedMutex);
        auto key = std::make_pair(chunkX, chunkZ);
        if (completedChunks.count(key)) {
            auto chunk = completedChunks[key];
            completedChunks.erase(key);
            return chunk;
        }
        return nullptr;
    }
    
private:
    void workerThread() {
        while (running) {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return !chunkQueue.empty() || !running; });
            
            if (!running) break;
            
            auto task = chunkQueue.top();
            chunkQueue.pop();
            lock.unlock();
            
            // Generate chunk
            auto chunk = new Chunk();
            generator->generateChunk(*chunk, task.x, task.z);
            
            // Store result
            std::lock_guard<std::mutex> completedLock(completedMutex);
            completedChunks[{task.x, task.z}] = chunk;
        }
    }
    
    std::vector<std::thread> workers;
    std::priority_queue<ChunkTask> chunkQueue;
    std::map<std::pair<int, int>, Chunk*> completedChunks;
    std::mutex queueMutex;
    std::mutex completedMutex;
    std::condition_variable cv;
    bool running = true;
};
```

---

### 6.2 GPU Compute Shaders (Future)

```glsl
// Compute shader for noise generation
#version 450

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (binding = 0, r32f) writeonly uniform image3D densityField;

uniform vec3 offset;
uniform float scale;
uniform int octaves;

float perlinNoise(vec3 p) {
    // GPU-optimized Perlin noise
    // ...
}

void main() {
    ivec3 pos = ivec3(gl_GlobalInvocationID.xyz);
    vec3 worldPos = vec3(pos) + offset;
    
    float density = 0.0;
    float frequency = scale;
    float amplitude = 1.0;
    
    for (int i = 0; i < octaves; i++) {
        density += perlinNoise(worldPos * frequency) * amplitude;
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    imageStore(densityField, pos, vec4(density));
}
```

---

## 🎯 7. Implementation Roadmap

### Phase 1: 2D World Generation (4-6 weeks)
1. **Week 1-2:** Basic 2D generator
   - Heightmap generation
   - Underground layers
   - Cave generation
   
2. **Week 3-4:** Terraria-style features
   - Ore deposits
   - Underground structures
   - Background walls
   
3. **Week 5-6:** Advanced 2D
   - Metroidvania generator
   - Endless runner generator
   - Room-based generation

### Phase 2: Advanced 3D (8-12 weeks)
1. **Week 1-3:** 3D density system
2. **Week 4-6:** Biome system 2.0
3. **Week 7-9:** Rivers and oceans
4. **Week 10-12:** Structure generation

### Phase 3: Vegetation (4-6 weeks)
1. **Week 1-2:** L-system trees
2. **Week 3-4:** Foliage system
3. **Week 5-6:** Surface decoration

### Phase 4: Optimization (4-6 weeks)
1. **Week 1-2:** Multi-threading
2. **Week 3-4:** LOD system
3. **Week 5-6:** GPU compute shaders

---

## 📚 8. References & Learning Resources

### Algorithms
- **Perlin Noise:** Ken Perlin's original paper
- **Simplex Noise:** Improved Perlin noise
- **Cellular Automata:** Cave generation
- **L-Systems:** Procedural tree generation
- **Voronoi Diagrams:** Biome boundaries

### Tutorials
- [Red Blob Games - Procedural Generation](https://www.redblobgames.com/)
- [Procedural Generation Wiki](http://pcg.wikidot.com/)
- [Amit's Game Programming](http://theory.stanford.edu/~amitp/)

### Examples from Other Games
- **Minecraft:** 3D voxel terrain
- **Terraria:** 2D side-scrolling
- **Starbound:** 2D with parallax
- **No Man's Sky:** Procedural planets
- **Dwarf Fortress:** Complex simulation
- **Vintage Story:** Advanced systems-based generation (see [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md))

---

## 🎉 Conclusion

Fresh Voxel Engine now has a **comprehensive world generation framework** supporting:

1. **3D Voxel Worlds** - Current system + advanced features planned
2. **2D Voxel Worlds** - NEW! Terraria/Metroidvania style
3. **Hybrid Worlds** - Mix 2D and 3D gameplay
4. **Custom Generators** - Fully extensible API

The implementation roadmap provides a clear path from current basic generation to advanced multi-paradigm world creation. With these features, Fresh can support a wide variety of game types and play styles!

**For advanced features and future enhancements**, see [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md) which details:
- Systems-based climate simulation
- Geological layering and ore distribution
- River and lake generation
- Structure placement with lore
- LOD rendering for distant terrain
- Moddability through JSON configuration

---

**Document Status:** ✅ Complete  
**Next Review:** 2025-12-15  
**Owner:** Development Team  
**Related Docs:** 
- [ADVANCED_WORLD_GENERATION.md](ADVANCED_WORLD_GENERATION.md) - Advanced features design
- [ROADMAP.md](ROADMAP.md) - Overall project roadmap
- [COMPREHENSIVE_ROADMAP.md](COMPREHENSIVE_ROADMAP.md) - Detailed feature planning
