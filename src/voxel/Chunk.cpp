#include "voxel/Chunk.h"
#include "voxel/MeshGenerator.h"
#include <iostream>

namespace fresh {

Chunk::Chunk(const ChunkPos& position)
    : m_position(position)
    , m_voxels(CHUNK_VOLUME)
    , m_dirty(true)
{
    // Initialize all voxels to air
    for (auto& voxel : m_voxels) {
        voxel = Voxel(VoxelType::Air);
    }
}

Chunk::~Chunk() {
}

Voxel& Chunk::getVoxel(int x, int y, int z) {
    return m_voxels[getIndex(x, y, z)];
}

const Voxel& Chunk::getVoxel(int x, int y, int z) const {
    return m_voxels[getIndex(x, y, z)];
}

void Chunk::setVoxel(int x, int y, int z, const Voxel& voxel) {
    m_voxels[getIndex(x, y, z)] = voxel;
    m_dirty = true;
}

void Chunk::generateMesh() {
    if (!m_dirty) {
        return;
    }
    
    m_meshVertices.clear();
    m_meshIndices.clear();
    
    MeshGenerator generator;
    generator.generateSimpleMesh(this, m_meshVertices, m_meshIndices);
    
    m_dirty = false;
}

} // namespace fresh
