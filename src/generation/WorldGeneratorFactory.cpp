#include "generation/IWorldGenerator.h"
#include "generation/TerrainGenerator.h"
#include "core/Logger.h"

namespace fresh
{

// Static registry for generator creators
std::unordered_map<std::string, WorldGeneratorFactory::GeneratorCreator>& 
WorldGeneratorFactory::getRegistry()
{
    static std::unordered_map<std::string, GeneratorCreator> registry;
    return registry;
}

void WorldGeneratorFactory::registerGenerator(const std::string& name, GeneratorCreator creator)
{
    auto& registry = getRegistry();
    if (registry.find(name) != registry.end()) {
        LOG_WARNING("WorldGeneratorFactory: Generator '" + name + "' already registered, overwriting");
    }
    registry[name] = creator;
    LOG_INFO("WorldGeneratorFactory: Registered generator '" + name + "'");
}

std::unique_ptr<IWorldGenerator> WorldGeneratorFactory::createGenerator(const std::string& name)
{
    auto& registry = getRegistry();
    auto it = registry.find(name);
    if (it != registry.end()) {
        return it->second();
    }
    LOG_WARNING("WorldGeneratorFactory: Generator '" + name + "' not found");
    return nullptr;
}

std::vector<std::string> WorldGeneratorFactory::getRegisteredGenerators()
{
    auto& registry = getRegistry();
    std::vector<std::string> names;
    names.reserve(registry.size());
    for (const auto& pair : registry) {
        names.push_back(pair.first);
    }
    return names;
}

// Adapter class to make TerrainGenerator conform to IWorldGenerator interface
class TerrainGeneratorAdapter : public IWorldGenerator
{
public:
    TerrainGeneratorAdapter() : m_generator(std::make_unique<TerrainGenerator>()) {}
    explicit TerrainGeneratorAdapter(int seed) : m_generator(std::make_unique<TerrainGenerator>(seed)) {}
    
    std::string getName() const override { return "3D Terrain"; }
    
    std::string getDescription() const override 
    { 
        return "Procedural 3D terrain using Perlin noise with biomes, caves, and natural features"; 
    }
    
    void generateChunk(Chunk* chunk) override
    {
        m_generator->generateChunk(chunk);
    }
    
    void generateChunkWithAssets(Chunk* chunk, VoxelWorld* world = nullptr) override
    {
        m_generator->generateChunkWithAssets(chunk, world);
    }
    
    void setSeed(int seed) override
    {
        m_generator->setSeed(seed);
    }
    
    int getHeight(int x, int z) const override
    {
        return m_generator->getHeight(x, z);
    }
    
    bool is3DGenerator() const override { return true; }
    bool is2DGenerator() const override { return false; }

private:
    std::unique_ptr<TerrainGenerator> m_generator;
};

// Adapter for 2D Platformer style (like Terraria)
class Platformer2DGeneratorAdapter : public IWorldGenerator
{
public:
    Platformer2DGeneratorAdapter() : m_generator(std::make_unique<TerrainGenerator>())
    {
        m_generator->set2DStyle(0); // 0 = Platformer
    }
    
    std::string getName() const override { return "2D Platformer"; }
    
    std::string getDescription() const override 
    { 
        return "Side-scrolling 2D terrain (Terraria-style) with layers and caves"; 
    }
    
    void generateChunk(Chunk* chunk) override
    {
        m_generator->generateChunk(chunk);
    }
    
    void generateChunkWithAssets(Chunk* chunk, VoxelWorld* world = nullptr) override
    {
        m_generator->generateChunkWithAssets(chunk, world);
    }
    
    void setSeed(int seed) override
    {
        m_generator->setSeed(seed);
    }
    
    int getHeight(int x, int z) const override
    {
        return m_generator->getHeight(x, z);
    }
    
    bool is3DGenerator() const override { return false; }
    bool is2DGenerator() const override { return true; }

private:
    std::unique_ptr<TerrainGenerator> m_generator;
};

// Adapter for 2D Top-Down style (like Zelda)
class TopDown2DGeneratorAdapter : public IWorldGenerator
{
public:
    TopDown2DGeneratorAdapter() : m_generator(std::make_unique<TerrainGenerator>())
    {
        m_generator->set2DStyle(1); // 1 = Top-down
    }
    
    std::string getName() const override { return "2D Top-Down"; }
    
    std::string getDescription() const override 
    { 
        return "Top-down 2D world (Zelda-style) with terrain and obstacles"; 
    }
    
    void generateChunk(Chunk* chunk) override
    {
        m_generator->generateChunk(chunk);
    }
    
    void generateChunkWithAssets(Chunk* chunk, VoxelWorld* world = nullptr) override
    {
        m_generator->generateChunkWithAssets(chunk, world);
    }
    
    void setSeed(int seed) override
    {
        m_generator->setSeed(seed);
    }
    
    int getHeight(int x, int z) const override
    {
        return m_generator->getHeight(x, z);
    }
    
    bool is3DGenerator() const override { return false; }
    bool is2DGenerator() const override { return true; }

private:
    std::unique_ptr<TerrainGenerator> m_generator;
};

// Empty generator - creates blank world
class EmptyWorldGenerator : public IWorldGenerator
{
public:
    std::string getName() const override { return "Empty"; }
    
    std::string getDescription() const override 
    { 
        return "Empty world with no terrain generation (blank canvas)"; 
    }
    
    void generateChunk(Chunk* chunk) override
    {
        (void)chunk; // Do nothing - leave chunk empty
    }
    
    void setSeed(int seed) override
    {
        (void)seed; // Empty generator doesn't use seed
    }
    
    bool is3DGenerator() const override { return true; }
    bool is2DGenerator() const override { return true; } // Works for both

private:
    int m_seed = 0;
};

void WorldGeneratorFactory::registerBuiltInGenerators()
{
    // Register 3D terrain generator
    registerGenerator("3D Terrain", []() -> std::unique_ptr<IWorldGenerator> {
        return std::make_unique<TerrainGeneratorAdapter>();
    });
    
    // Register 2D platformer generator
    registerGenerator("2D Platformer", []() -> std::unique_ptr<IWorldGenerator> {
        return std::make_unique<Platformer2DGeneratorAdapter>();
    });
    
    // Register 2D top-down generator
    registerGenerator("2D Top-Down", []() -> std::unique_ptr<IWorldGenerator> {
        return std::make_unique<TopDown2DGeneratorAdapter>();
    });
    
    // Register empty world generator
    registerGenerator("Empty", []() -> std::unique_ptr<IWorldGenerator> {
        return std::make_unique<EmptyWorldGenerator>();
    });
    
    LOG_INFO("WorldGeneratorFactory: Built-in generators registered");
}

} // namespace fresh
