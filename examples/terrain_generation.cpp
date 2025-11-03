/**
 * @file terrain_generation.cpp
 * @brief Demonstrates procedural terrain generation
 * 
 * This example shows how to:
 * - Create a terrain generator
 * - Generate chunks with different biomes
 * - Customize noise parameters
 * - Generate caves and features
 * - Use different seeds for variety
 */

#include <iostream>
#include <memory>
#include "generation/NoiseGenerator.h"
#include "generation/TerrainGenerator.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Terrain Generation ===" << std::endl;
    
    // Step 1: Create noise generator
    std::cout << "\n[1/5] Creating noise generator..." << std::endl;
    NoiseGenerator noiseGen(12345); // Seed for reproducible terrain
    
    // Configure noise parameters
    noiseGen.setOctaves(6);        // More octaves = more detail
    noiseGen.setFrequency(0.01f);   // Lower = smoother terrain
    noiseGen.setPersistence(0.5f);  // How much each octave contributes
    noiseGen.setLacunarity(2.0f);   // Frequency multiplier between octaves
    
    std::cout << "    ✓ Noise generator created" << std::endl;
    std::cout << "      Seed: 12345" << std::endl;
    std::cout << "      Octaves: 6" << std::endl;
    std::cout << "      Frequency: 0.01" << std::endl;
    
    // Step 2: Create terrain generator
    std::cout << "\n[2/5] Creating terrain generator..." << std::endl;
    TerrainGenerator terrainGen(12345);
    std::cout << "    ✓ Terrain generator ready" << std::endl;
    
    // Step 3: Create voxel world
    std::cout << "\n[3/5] Creating voxel world..." << std::endl;
    VoxelWorld world;
    std::cout << "    ✓ World created" << std::endl;
    
    // Step 4: Generate some chunks
    std::cout << "\n[4/5] Generating chunks..." << std::endl;
    
    // Generate a 5x5 area of chunks around origin
    const int chunkRadius = 2;
    int chunksGenerated = 0;
    
    for (int x = -chunkRadius; x <= chunkRadius; x++) {
        for (int z = -chunkRadius; z <= chunkRadius; z++) {
            std::cout << "    Generating chunk (" << x << ", " << z << ")..." << std::endl;
            
            // Generate the chunk
            auto chunk = terrainGen.generateChunk(x, z);
            
            // Add to world
            world.addChunk(x, z, std::move(chunk));
            
            chunksGenerated++;
        }
    }
    
    std::cout << "    ✓ Generated " << chunksGenerated << " chunks" << std::endl;
    
    // Step 5: Analyze generated terrain
    std::cout << "\n[5/5] Analyzing terrain..." << std::endl;
    
    // Sample the terrain at various points
    struct TerrainSample {
        int x, z;
        int height;
        VoxelType surfaceType;
    };
    
    std::vector<TerrainSample> samples = {
        {0, 0, 0, VoxelType::Air},
        {50, 50, 0, VoxelType::Air},
        {-50, 50, 0, VoxelType::Air},
        {50, -50, 0, VoxelType::Air},
    };
    
    for (auto& sample : samples) {
        // Find surface height
        for (int y = 255; y >= 0; y--) {
            VoxelType voxel = world.getVoxel(sample.x, y, sample.z);
            if (voxel != VoxelType::Air) {
                sample.height = y;
                sample.surfaceType = voxel;
                break;
            }
        }
        
        std::cout << "    Position (" << sample.x << ", " << sample.z << "): "
                  << "Height=" << sample.height << ", "
                  << "Surface=" << voxelTypeToString(sample.surfaceType) << std::endl;
    }
    
    // Biome statistics
    std::cout << "\n=== Terrain Statistics ===" << std::endl;
    std::cout << "  Total chunks: " << chunksGenerated << std::endl;
    std::cout << "  Total voxels: " << (chunksGenerated * 16 * 256 * 16) << std::endl;
    std::cout << "  World seed: 12345" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • How to configure noise parameters" << std::endl;
    std::cout << "  • How to generate terrain chunks" << std::endl;
    std::cout << "  • How to add chunks to a world" << std::endl;
    std::cout << "  • How to sample terrain height and type" << std::endl;
    std::cout << "\nExperiment ideas:" << std::endl;
    std::cout << "  • Try different seeds" << std::endl;
    std::cout << "  • Adjust octaves (1-8)" << std::endl;
    std::cout << "  • Change frequency (0.001-0.1)" << std::endl;
    std::cout << "  • Modify persistence (0.1-0.9)" << std::endl;
    
    return 0;
}

/**
 * NOISE PARAMETER GUIDE:
 * 
 * Octaves (1-8):
 *   - More octaves = more detail and complexity
 *   - 1 = smooth, rolling hills
 *   - 4-6 = realistic terrain (recommended)
 *   - 8+ = very rough, mountainous
 * 
 * Frequency (0.001-0.1):
 *   - Controls the "zoom level" of noise
 *   - Lower = larger features, smoother
 *   - 0.01 = good default for terrain
 *   - Higher = smaller features, rougher
 * 
 * Persistence (0.0-1.0):
 *   - How much each octave contributes
 *   - 0.5 = each octave half as strong
 *   - Lower = smoother (low octaves dominate)
 *   - Higher = rougher (high octaves matter more)
 * 
 * Lacunarity (1.5-3.0):
 *   - Frequency multiplier between octaves
 *   - 2.0 = each octave twice the frequency
 *   - Lower = more uniform detail
 *   - Higher = more varying detail
 * 
 * BIOME GENERATION:
 * 
 * The terrain generator uses height and moisture to select biomes:
 * 
 * Height Ranges:
 *   - Below 50: Ocean/Water
 *   - 50-65: Beach/Plains
 *   - 65-90: Forest/Hills
 *   - 90+: Mountains
 * 
 * CAVE GENERATION:
 * 
 * Caves are generated using 3D noise:
 *   - If noise > threshold, create air pocket
 *   - Typically generates at y < 60
 *   - Can create complex tunnel systems
 * 
 * CUSTOMIZATION:
 * 
 * For flat terrain:
 *   octaves = 1
 *   frequency = 0.001
 * 
 * For extreme mountains:
 *   octaves = 8
 *   frequency = 0.02
 *   persistence = 0.7
 * 
 * For smooth rolling hills:
 *   octaves = 3
 *   frequency = 0.005
 *   persistence = 0.4
 */
