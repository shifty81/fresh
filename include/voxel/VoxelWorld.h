#pragma once

#include "Chunk.h"
#include <unordered_map>
#include <memory>
#include <vector>

namespace fresh {

class TerrainGenerator;

/**
 * @brief Manages the entire voxel world
 * 
 * Handles chunk loading/unloading, world updates, and provides
 * interface for voxel manipulation.
 */
class VoxelWorld {
public:
    VoxelWorld();
    ~VoxelWorld();

    /**
     * @brief Initialize the world
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Update the world (chunk streaming, etc.)
     * @param playerPos Player position for chunk loading
     */
    void update(const WorldPos& playerPos);

    /**
     * @brief Get chunk at position
     * @param pos Chunk position
     * @return Pointer to chunk or nullptr if not loaded
     */
    Chunk* getChunk(const ChunkPos& pos);

    /**
     * @brief Load chunk at position
     * @param pos Chunk position
     * @return Pointer to loaded chunk
     */
    Chunk* loadChunk(const ChunkPos& pos);

    /**
     * @brief Unload chunk at position
     * @param pos Chunk position
     */
    void unloadChunk(const ChunkPos& pos);

    /**
     * @brief Get voxel at world position
     * @param pos World position
     * @return Pointer to voxel or nullptr
     */
    Voxel* getVoxel(const WorldPos& pos);

    /**
     * @brief Set voxel at world position
     * @param pos World position
     * @param voxel Voxel to set
     */
    void setVoxel(const WorldPos& pos, const Voxel& voxel);

    /**
     * @brief Get all loaded chunks
     * @return Map of chunk positions to chunks
     */
    [[nodiscard]] const std::unordered_map<ChunkPos, std::unique_ptr<Chunk>>& getChunks() const noexcept {
        return m_chunks;
    }

    /**
     * @brief Set render distance
     * @param distance Render distance in chunks
     */
    void setRenderDistance(int distance) { m_renderDistance = distance; }

private:
    std::unordered_map<ChunkPos, std::unique_ptr<Chunk>> m_chunks;
    std::unique_ptr<TerrainGenerator> m_terrainGenerator;
    int m_renderDistance;
};

} // namespace fresh
