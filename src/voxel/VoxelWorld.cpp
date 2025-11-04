#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"
#include <iostream>

namespace fresh {

VoxelWorld::VoxelWorld()
    : m_renderDistance(8)
{
}

VoxelWorld::~VoxelWorld() {
}

bool VoxelWorld::initialize() {
    m_terrainGenerator = std::make_unique<TerrainGenerator>(12345);
    return true;
}

void VoxelWorld::update(const WorldPos& playerPos) {
    (void)playerPos; // Unused - placeholder for future implementation
    // Simple chunk loading/unloading based on player position
    // In a real implementation, this would be more sophisticated
}

Chunk* VoxelWorld::getChunk(const ChunkPos& pos) {
    auto it = m_chunks.find(pos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

Chunk* VoxelWorld::loadChunk(const ChunkPos& pos) {
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

void VoxelWorld::unloadChunk(const ChunkPos& pos) {
    m_chunks.erase(pos);
}

Voxel* VoxelWorld::getVoxel(const WorldPos& pos) {
    ChunkPos chunkPos = ChunkPos::fromWorldPos(pos);
    Chunk* chunk = getChunk(chunkPos);
    
    if (!chunk) {
        return nullptr;
    }
    
    int localX = pos.x - chunkPos.x * CHUNK_SIZE;
    int localZ = pos.z - chunkPos.z * CHUNK_SIZE;
    
    if (localX < 0) localX += CHUNK_SIZE;
    if (localZ < 0) localZ += CHUNK_SIZE;
    
    return &chunk->getVoxel(localX, pos.y, localZ);
}

void VoxelWorld::setVoxel(const WorldPos& pos, const Voxel& voxel) {
    ChunkPos chunkPos = ChunkPos::fromWorldPos(pos);
    Chunk* chunk = getChunk(chunkPos);
    
    if (!chunk) {
        return;
    }
    
    int localX = pos.x - chunkPos.x * CHUNK_SIZE;
    int localZ = pos.z - chunkPos.z * CHUNK_SIZE;
    
    if (localX < 0) localX += CHUNK_SIZE;
    if (localZ < 0) localZ += CHUNK_SIZE;
    
    chunk->setVoxel(localX, pos.y, localZ, voxel);
}

} // namespace fresh
