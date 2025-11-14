#include "voxel/VoxelWorld.h"

#include <iostream>

#include "generation/TerrainGenerator.h"

namespace fresh
{

VoxelWorld::VoxelWorld() : m_renderDistance(8) {}

VoxelWorld::~VoxelWorld() {}

bool VoxelWorld::initialize()
{
    m_terrainGenerator = std::make_unique<TerrainGenerator>(12345);
    return true;
}

void VoxelWorld::update(const WorldPos& playerPos)
{
    (void)playerPos; // Unused - placeholder for future implementation
    // Simple chunk loading/unloading based on player position
    // In a real implementation, this would be more sophisticated
}

Chunk* VoxelWorld::getChunk(const ChunkPos& pos) noexcept
{
    auto it = m_chunks.find(pos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

const Chunk* VoxelWorld::getChunk(const ChunkPos& pos) const noexcept
{
    auto it = m_chunks.find(pos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

Chunk* VoxelWorld::loadChunk(const ChunkPos& pos)
{
    auto existing = getChunk(pos);
    if (existing) {
        return existing;
    }

    auto chunk = std::make_unique<Chunk>(pos);
    m_terrainGenerator->generateChunk(chunk.get());
    chunk->generateMesh();

    Chunk* chunkPtr = chunk.get();
    m_chunks[pos] = std::move(chunk);

    return chunkPtr;
}

void VoxelWorld::unloadChunk(const ChunkPos& pos)
{
    m_chunks.erase(pos);
}

Voxel* VoxelWorld::getVoxel(const WorldPos& pos)
{
    const ChunkPos chunkPos = ChunkPos::fromWorldPos(pos);
    Chunk* chunk = getChunk(chunkPos);

    if (!chunk) {
        return nullptr;
    }

    // Compute local coordinates more efficiently
    // Use modulo for proper wrapping of negative coordinates
    const int localX = ((pos.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    const int localZ = ((pos.z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;

    return &chunk->getVoxel(localX, pos.y, localZ);
}

void VoxelWorld::setVoxel(const WorldPos& pos, const Voxel& voxel)
{
    const ChunkPos chunkPos = ChunkPos::fromWorldPos(pos);
    Chunk* chunk = getChunk(chunkPos);

    if (!chunk) {
        return;
    }

    // Compute local coordinates more efficiently
    const int localX = ((pos.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    const int localZ = ((pos.z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;

    chunk->setVoxel(localX, pos.y, localZ, voxel);
}

void VoxelWorld::setSeed(int seed)
{
    if (m_terrainGenerator) {
        m_terrainGenerator->setSeed(seed);
    }
}

} // namespace fresh
