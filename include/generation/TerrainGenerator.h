#pragma once

#include "NoiseGenerator.h"
#include "voxel/VoxelTypes.h"

namespace fresh
{

class Chunk;
class VoxelWorld;

/**
 * @brief Generates terrain using procedural algorithms
 *
 * Uses noise functions to create natural-looking terrain with
 * different biomes, caves, and structures.
 */
class TerrainGenerator
{
public:
    TerrainGenerator();
    explicit TerrainGenerator(int seed);
    ~TerrainGenerator();

    /**
     * @brief Generate terrain for a chunk
     * @param chunk Chunk to generate terrain for
     */
    void generateChunk(Chunk* chunk);

    /**
     * @brief Generate terrain and assets for a chunk
     * @param chunk Chunk to generate for
     * @param world VoxelWorld for asset placement (optional)
     */
    void generateChunkWithAssets(Chunk* chunk, VoxelWorld* world = nullptr);

    /**
     * @brief Set generation seed
     * @param seed Random seed
     */
    void setSeed(int seed);

    /**
     * @brief Get height at world position
     * @param x World X coordinate
     * @param z World Z coordinate
     * @return Height value
     */
    [[nodiscard]] int getHeight(int x, int z) const;

private:
    VoxelType getBlockType(int x, int y, int z, int surfaceHeight) const;
    void generateTreesAndFoliage(Chunk* chunk, VoxelWorld* world);

private:
    NoiseGenerator m_noiseGenerator;
    int m_seed;
};

} // namespace fresh
