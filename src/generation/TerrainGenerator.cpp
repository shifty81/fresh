#include "generation/TerrainGenerator.h"

#include <cmath>

#include "voxel/Chunk.h"
#include "voxel/VoxelWorld.h"
#include "assets/ModularAssetSystem.h"

namespace fresh
{

TerrainGenerator::TerrainGenerator() : m_noiseGenerator(0), m_seed(0) {}

TerrainGenerator::TerrainGenerator(int seed) : m_noiseGenerator(seed), m_seed(seed) {}

TerrainGenerator::~TerrainGenerator() {}

void TerrainGenerator::setSeed(int seed)
{
    m_seed = seed;
    m_noiseGenerator.setSeed(seed);
}

int TerrainGenerator::getHeight(int x, int z) const
{
    // Generate terrain height using multi-octave Perlin noise (fractal Brownian motion)
    // Scale factor (0.01f): Controls terrain frequency - lower = larger features
    // Octaves (4): Number of noise layers combined - more = finer detail
    // Persistence (0.5f): Amplitude decay per octave - lower = smoother terrain
    // Lacunarity (2.0f): Frequency multiplier per octave - higher = more detail variation
    float noiseValue = m_noiseGenerator.fractalNoise2D(x * 0.01f, z * 0.01f,
                                                       4,    // octaves
                                                       0.5f, // persistence
                                                       2.0f  // lacunarity
    );

    // Map noise from [-1, 1] to height range [40, 80]
    // This creates terrain between y=40 (valleys) and y=80 (peaks)
    int height = 40 + static_cast<int>((noiseValue + 1.0f) * 20.0f);

    return height;
}

VoxelType TerrainGenerator::getBlockType(int x, int y, int z, int surfaceHeight) const
{
    // Determine block type based on height and position
    // Creates natural terrain layers: grass/dirt on top, stone below, bedrock at bottom
    if (y > surfaceHeight) {
        return VoxelType::Air;
    } else if (y == surfaceHeight) {
        // Top layer
        if (surfaceHeight > 62) {
            return VoxelType::Grass;
        } else if (surfaceHeight > 58) {
            return VoxelType::Sand;
        } else {
            return VoxelType::Stone;
        }
    } else if (y >= surfaceHeight - 3) {
        // Few layers below surface
        if (surfaceHeight > 58) {
            return VoxelType::Dirt;
        } else {
            return VoxelType::Stone;
        }
    } else {
        // Deep underground
        // Add caves using 3D noise
        float caveNoise = m_noiseGenerator.perlin3D(x * 0.05f, y * 0.05f, z * 0.05f);

        if (caveNoise > 0.5f) {
            return VoxelType::Air;
        }

        return VoxelType::Stone;
    }
}

void TerrainGenerator::generateChunk(Chunk* chunk)
{
    if (!chunk) {
        return; // Gracefully handle null pointer
    }

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

void TerrainGenerator::generateChunkWithAssets(Chunk* chunk, VoxelWorld* world)
{
    // First generate the base terrain
    generateChunk(chunk);

    // If world is provided, generate and place assets
    if (world) {
        const ChunkPos& chunkPos = chunk->getPosition();
        
        // Generate trees and foliage directly for testing
        generateTreesAndFoliage(chunk, world);
        
        // Also try to use the modular asset system if asset packs are available
        auto& assetSystem = ModularAssetSystem::getInstance();
        auto assetInstances = assetSystem.generateAssetsForChunk(
            world, 
            chunkPos.x, 
            chunkPos.z, 
            static_cast<uint32_t>(m_seed)
        );

        // Place the generated assets in the world
        assetSystem.placeAssetsInWorld(assetInstances, world);
        
        // Mark chunk as dirty to regenerate mesh with new assets
        chunk->markDirty();
    }
}

void TerrainGenerator::generateTreesAndFoliage(Chunk* chunk, VoxelWorld* world)
{
    (void)world; // Parameter reserved for future cross-chunk tree generation
    
    if (!chunk) {
        return;
    }

    const ChunkPos& chunkPos = chunk->getPosition();
    
    // Use noise to determine tree placement positions
    // This creates natural-looking clusters of trees
    for (int localX = 0; localX < CHUNK_SIZE; ++localX) {
        for (int localZ = 0; localZ < CHUNK_SIZE; ++localZ) {
            // Convert to world coordinates
            int worldX = chunkPos.x * CHUNK_SIZE + localX;
            int worldZ = chunkPos.z * CHUNK_SIZE + localZ;
            
            // Use noise to determine if a tree should spawn here
            // Scale of 0.1 creates clusters of trees
            float treeNoise = m_noiseGenerator.perlin2D(worldX * 0.1f, worldZ * 0.1f);
            
            // Only spawn trees on elevated terrain (grass blocks) with certain noise values
            int surfaceHeight = getHeight(worldX, worldZ);
            
            // Check if surface is grass (good for trees)
            if (surfaceHeight > 62 && surfaceHeight < 75) {
                // Spawn trees with higher probability
                if (treeNoise > 0.3f && treeNoise < 0.8f) {
                    // Generate a tree at this position
                    int treeHeight = 5 + static_cast<int>((treeNoise + 1.0f) * 2.0f); // 5-9 blocks tall
                    int trunkHeight = treeHeight - 3; // Leaves start 3 blocks from top
                    
                    // Place tree trunk (Wood blocks) - directly in chunk
                    for (int y = 1; y <= trunkHeight; ++y) {
                        int worldY = surfaceHeight + y;
                        if (worldY < CHUNK_HEIGHT) {
                            chunk->setVoxel(localX, worldY, localZ, Voxel(VoxelType::Wood));
                        }
                    }
                    
                    // Place leaves (foliage) in a spherical pattern around the top
                    int leavesStartY = surfaceHeight + trunkHeight - 1;
                    int leavesRadius = 2; // Leaves spread 2 blocks in each direction
                    
                    for (int dy = 0; dy <= 4; ++dy) {
                        int worldY = leavesStartY + dy;
                        if (worldY >= CHUNK_HEIGHT) break;
                        
                        // Smaller radius at top and bottom of leaves
                        int currentRadius = leavesRadius;
                        if (dy == 0 || dy == 4) {
                            currentRadius = 1;
                        }
                        
                        for (int dx = -currentRadius; dx <= currentRadius; ++dx) {
                            for (int dz = -currentRadius; dz <= currentRadius; ++dz) {
                                // Skip the trunk position
                                if (dx == 0 && dz == 0 && dy < 3) {
                                    continue;
                                }
                                
                                // Create roughly spherical shape
                                float dist = std::sqrt(dx * dx + dz * dz + dy * dy * 0.5f);
                                if (dist <= leavesRadius + 0.5f) {
                                    int leafX = localX + dx;
                                    int leafZ = localZ + dz;
                                    
                                    // Only place leaves within chunk bounds
                                    if (leafX >= 0 && leafX < CHUNK_SIZE && 
                                        leafZ >= 0 && leafZ < CHUNK_SIZE) {
                                        chunk->setVoxel(leafX, worldY, leafZ, Voxel(VoxelType::Leaves));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace fresh
