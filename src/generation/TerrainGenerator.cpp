#include "generation/TerrainGenerator.h"
#include "voxel/Chunk.h"
#include <cmath>

namespace fresh {

TerrainGenerator::TerrainGenerator()
    : m_noiseGenerator(0)
    , m_seed(0)
{
}

TerrainGenerator::TerrainGenerator(int seed)
    : m_noiseGenerator(seed)
    , m_seed(seed)
{
}

TerrainGenerator::~TerrainGenerator() {
}

void TerrainGenerator::setSeed(int seed) {
    m_seed = seed;
    m_noiseGenerator.setSeed(seed);
}

int TerrainGenerator::getHeight(int x, int z) const {
    // Use noise to generate height
    float noiseValue = m_noiseGenerator.fractalNoise2D(
        x * 0.01f,
        z * 0.01f,
        4,      // octaves
        0.5f,   // persistence
        2.0f    // lacunarity
    );
    
    // Map noise from [-1, 1] to height range [40, 80]
    int height = 40 + static_cast<int>((noiseValue + 1.0f) * 20.0f);
    
    return height;
}

VoxelType TerrainGenerator::getBlockType(int x, int y, int z, int surfaceHeight) const {
    if (y > surfaceHeight) {
        return VoxelType::Air;
    }
    else if (y == surfaceHeight) {
        // Top layer
        if (surfaceHeight > 62) {
            return VoxelType::Grass;
        } else if (surfaceHeight > 58) {
            return VoxelType::Sand;
        } else {
            return VoxelType::Stone;
        }
    }
    else if (y >= surfaceHeight - 3) {
        // Few layers below surface
        if (surfaceHeight > 58) {
            return VoxelType::Dirt;
        } else {
            return VoxelType::Stone;
        }
    }
    else {
        // Deep underground
        // Add caves using 3D noise
        float caveNoise = m_noiseGenerator.perlin3D(
            x * 0.05f,
            y * 0.05f,
            z * 0.05f
        );
        
        if (caveNoise > 0.5f) {
            return VoxelType::Air;
        }
        
        return VoxelType::Stone;
    }
}

void TerrainGenerator::generateChunk(Chunk* chunk) {
    const ChunkPos& chunkPos = chunk->getPosition();
    
    // Generate terrain for each column in the chunk
    for (int localX = 0; localX < CHUNK_SIZE; ++localX) {
        for (int localZ = 0; localZ < CHUNK_SIZE; ++localZ) {
            // Convert to world coordinates
            int worldX = chunkPos.x * CHUNK_SIZE + localX;
            int worldZ = chunkPos.z * CHUNK_SIZE + localZ;
            
            // Get height at this position
            int surfaceHeight = getHeight(worldX, worldZ);
            
            // Generate vertical column
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                VoxelType type = getBlockType(worldX, y, worldZ, surfaceHeight);
                chunk->setVoxel(localX, y, localZ, Voxel(type));
            }
        }
    }
    
    chunk->markDirty();
}

} // namespace fresh
