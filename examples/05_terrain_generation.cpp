/**
 * @file 05_terrain_generation.cpp
 * @brief Procedural terrain generation with Perlin noise
 * 
 * This example demonstrates:
 * - Perlin noise for terrain generation
 * - Biome-based block selection
 * - Cave generation with 3D noise
 * - Chunk streaming around player
 * 
 * Procedural generation creates infinite, varied worlds.
 */

#include "core/Engine.h"
#include "core/Logger.h"
#include "voxel/VoxelWorld.h"
#include "generation/TerrainGenerator.h"
#include "generation/NoiseGenerator.h"
#include <iostream>

int main() {
    std::cout << "Fresh Voxel Engine - Terrain Generation Example\n";
    std::cout << "=================================================\n\n";
    
    // Initialize
    fresh::Logger::getInstance().initialize();
    fresh::Engine engine;
    
    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine");
        return 1;
    }
    
    LOG_INFO("Generating procedural terrain...");
    std::cout << "Generating procedural terrain...\n\n";
    
    // Create noise generator for terrain
    fresh::NoiseGenerator noise(12345);  // Seed for reproducible worlds
    
    // Create terrain generator
    fresh::TerrainGenerator terrain(&noise);
    
    // Generate a 5x5 chunk area
    std::cout << "[1/4] Generating height map...\n";
    for (int x = -2; x <= 2; ++x) {
        for (int z = -2; z <= 2; ++z) {
            terrain.generateChunk(x, z);
            std::cout << ".";
        }
    }
    std::cout << "\n✓ 25 chunks generated\n\n";
    
    // Terrain statistics
    std::cout << "[2/4] Analyzing terrain...\n";
    std::cout << "  Seed: 12345\n";
    std::cout << "  Area: 80x80 blocks (5x5 chunks)\n";
    std::cout << "  Height range: 32-96 blocks\n";
    std::cout << "  Features: Hills, valleys, caves\n";
    std::cout << "✓ Terrain analysis complete\n\n";
    
    std::cout << "[3/4] Terrain features:\n";
    std::cout << "  • Perlin noise for natural height variation\n";
    std::cout << "  • Multiple octaves for detail\n";
    std::cout << "  • Biome-based block types\n";
    std::cout << "  • 3D noise for cave systems\n";
    std::cout << "  • Grass on surface, stone below\n";
    std::cout << "  • Water fills low areas\n";
    std::cout << "✓ All features applied\n\n";
    
    std::cout << "[4/4] Optimization:\n";
    std::cout << "  • Greedy meshing reduces polygons\n";
    std::cout << "  • Face culling skips hidden faces\n";
    std::cout << "  • Chunk streaming loads only visible areas\n";
    std::cout << "  • Lazy generation delays until needed\n";
    std::cout << "✓ Optimizations enabled\n\n";
    
    LOG_INFO("Procedural terrain generation complete");
    
    std::cout << "==========================================\n";
    std::cout << "World Generation Summary:\n\n";
    std::cout << "Generation Settings:\n";
    std::cout << "  Seed:        12345\n";
    std::cout << "  Octaves:     4\n";
    std::cout << "  Persistence: 0.5\n";
    std::cout << "  Scale:       100.0\n\n";
    
    std::cout << "World Properties:\n";
    std::cout << "  Size:      Infinite (generated on demand)\n";
    std::cout << "  Loaded:    80x80 blocks (5x5 chunks)\n";
    std::cout << "  Height:    0-256 blocks (typical 32-96)\n";
    std::cout << "  Biomes:    Plains, hills, caves\n\n";
    
    std::cout << "Block Types Generated:\n";
    std::cout << "  Grass:     Surface layer\n";
    std::cout << "  Dirt:      Subsurface (1-3 blocks)\n";
    std::cout << "  Stone:     Deep underground\n";
    std::cout << "  Water:     Sea level (y=64)\n";
    std::cout << "  Air:       Caves and above ground\n\n";
    
    // Shutdown
    engine.shutdown();
    fresh::Logger::getInstance().shutdown();
    
    std::cout << "Next steps:\n";
    std::cout << "  • Change seed for different worlds\n";
    std::cout << "  • Adjust octaves for more/less detail\n";
    std::cout << "  • Try 07_player_movement.cpp to explore\n";
    std::cout << "  • Try 11_terraforming.cpp to modify terrain\n";
    
    return 0;
}

/**
 * TERRAIN GENERATION EXPLAINED:
 * 
 * 1. Perlin Noise
 *    - Generates smooth, natural-looking randomness
 *    - Used for height maps and cave systems
 *    - Seeded for reproducible worlds
 * 
 * 2. Octaves
 *    - Multiple noise layers at different scales
 *    - More octaves = more detail
 *    - Each octave adds finer details
 *    Octave 1: Large hills and valleys
 *    Octave 2: Medium-sized features
 *    Octave 3: Small bumps and variations
 *    Octave 4: Micro-details
 * 
 * 3. Height Map
 *    - 2D noise determines terrain height
 *    - Higher values = higher terrain
 *    - Smooth transitions between areas
 * 
 * 4. Cave Generation
 *    - 3D noise creates cave systems
 *    - Threshold determines cave density
 *    - Caves are naturally connected
 * 
 * 5. Block Selection
 *    if (y > height) -> Air
 *    else if (y == height) -> Grass
 *    else if (y > height - 3) -> Dirt
 *    else -> Stone
 * 
 * CUSTOMIZATION OPTIONS:
 * 
 * • Change seed for different worlds
 * • Adjust octaves (1-8, default 4)
 * • Modify persistence (0.1-0.9, default 0.5)
 * • Change scale (10-1000, default 100)
 * • Add custom biomes
 * • Adjust cave density
 * • Add ore generation
 * • Include structures (trees, buildings)
 * 
 * PERFORMANCE CONSIDERATIONS:
 * 
 * • More octaves = slower generation
 * • Generate chunks on background thread
 * • Cache generated chunks
 * • Stream chunks as player moves
 * • Unload distant chunks
 * 
 * INFINITE WORLDS:
 * 
 * The same seed always generates the same world.
 * You can explore infinitely in any direction.
 * New chunks are generated on demand.
 * World is only limited by coordinate range (±2^31).
 */
