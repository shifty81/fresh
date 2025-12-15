#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace fresh
{

class Chunk;
class VoxelWorld;

/**
 * @brief Interface for world generation plugins
 * 
 * In the editor-first architecture, world generators are treated as plugins.
 * This allows the engine to support multiple generation strategies:
 * - Built-in generators (Terrain3D, Platformer2D, TopDown2D)
 * - Custom user-defined generators
 * - Asset-pack generators
 * - Procedural generators from scripts
 * 
 * The editor is the primary application, and world generation is a pluggable
 * feature that can be extended without modifying the core engine.
 * 
 * Example implementation:
 * @code
 * class CustomTerrainGenerator : public IWorldGenerator {
 * public:
 *     std::string getName() const override { return "Custom Terrain"; }
 *     std::string getDescription() const override { return "My custom generator"; }
 *     
 *     void generateChunk(Chunk* chunk) override {
 *         // Custom generation logic
 *     }
 *     
 *     void setSeed(int seed) override { m_seed = seed; }
 * private:
 *     int m_seed;
 * };
 * @endcode
 */
class IWorldGenerator
{
public:
    virtual ~IWorldGenerator() = default;
    
    /**
     * @brief Get the name of this generator
     * @return Human-readable generator name (e.g., "3D Terrain", "2D Platformer")
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Get a description of this generator
     * @return Human-readable description of what this generator creates
     */
    virtual std::string getDescription() const = 0;
    
    /**
     * @brief Generate terrain for a single chunk
     * @param chunk The chunk to populate with voxel data
     */
    virtual void generateChunk(Chunk* chunk) = 0;
    
    /**
     * @brief Generate terrain and place assets for a chunk
     * @param chunk The chunk to populate
     * @param world The world context for asset placement (optional)
     * 
     * Default implementation calls generateChunk() if not overridden.
     */
    virtual void generateChunkWithAssets(Chunk* chunk, VoxelWorld* world = nullptr)
    {
        (void)world; // Suppress unused parameter warning
        generateChunk(chunk);
    }
    
    /**
     * @brief Set the generation seed for deterministic world generation
     * @param seed Random seed value
     */
    virtual void setSeed(int seed) = 0;
    
    /**
     * @brief Get the height at a world position (for terrain surface)
     * @param x World X coordinate
     * @param z World Z coordinate
     * @return Height value (Y coordinate)
     * 
     * Default implementation returns 0 for non-heightmap generators.
     */
    virtual int getHeight(int x, int z) const
    {
        (void)x; (void)z; // Suppress unused parameter warnings
        return 0;
    }
    
    /**
     * @brief Check if this generator supports 2D worlds
     * @return true if this is a 2D generator
     */
    virtual bool is2DGenerator() const { return false; }
    
    /**
     * @brief Check if this generator supports 3D worlds
     * @return true if this is a 3D generator
     */
    virtual bool is3DGenerator() const { return true; }
};

/**
 * @brief Factory for creating world generator instances
 * 
 * Allows registration of custom generator types at runtime.
 * The editor can enumerate available generators and present them to the user.
 */
class WorldGeneratorFactory
{
public:
    using GeneratorCreator = std::unique_ptr<IWorldGenerator>(*)();
    
    /**
     * @brief Register a generator type
     * @param name Generator name (must be unique)
     * @param creator Factory function to create instances
     */
    static void registerGenerator(const std::string& name, GeneratorCreator creator);
    
    /**
     * @brief Create a generator instance by name
     * @param name Generator name
     * @return Generator instance or nullptr if not found
     */
    static std::unique_ptr<IWorldGenerator> createGenerator(const std::string& name);
    
    /**
     * @brief Get list of all registered generator names
     * @return Vector of generator names
     */
    static std::vector<std::string> getRegisteredGenerators();
    
    /**
     * @brief Register built-in generators
     * 
     * Registers the default generators:
     * - "3D Terrain" (Perlin noise-based terrain)
     * - "2D Platformer" (Side-scrolling terrain)
     * - "2D Top-Down" (Top-down world)
     * - "Empty" (No generation, blank world)
     */
    static void registerBuiltInGenerators();

private:
    static std::unordered_map<std::string, GeneratorCreator>& getRegistry();
};

} // namespace fresh
