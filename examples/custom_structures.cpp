/**
 * @file custom_structures.cpp
 * @brief Demonstrates building complex structures programmatically
 * 
 * This example shows:
 * - Building structures voxel-by-voxel
 * - Creating reusable structure templates
 * - Rotation and mirroring
 * - Procedural structure generation
 * - Structure blueprints
 */

#include <iostream>
#include <memory>
#include <vector>
#include "voxel/VoxelWorld.h"
#include "voxel/VoxelTypes.h"

/**
 * Structure blueprint - stores relative voxel positions and types
 */
struct StructureBlueprint {
    std::vector<std::tuple<int, int, int, VoxelType>> voxels;
    
    void addVoxel(int x, int y, int z, VoxelType type) {
        voxels.push_back(std::make_tuple(x, y, z, type));
    }
    
    void placeInWorld(VoxelWorld& world, int originX, int originY, int originZ) {
        for (const auto& [x, y, z, type] : voxels) {
            world.setVoxel(originX + x, originY + y, originZ + z, type);
        }
    }
};

/**
 * Create a simple house structure
 */
StructureBlueprint createHouse() {
    StructureBlueprint house;
    
    // Foundation (5x5)
    for (int x = 0; x < 5; x++) {
        for (int z = 0; z < 5; z++) {
            house.addVoxel(x, 0, z, VoxelType::Stone);
        }
    }
    
    // Walls (hollow)
    for (int y = 1; y <= 3; y++) {
        // Front and back walls
        for (int x = 0; x < 5; x++) {
            house.addVoxel(x, y, 0, VoxelType::Wood);
            house.addVoxel(x, y, 4, VoxelType::Wood);
        }
        // Left and right walls
        for (int z = 1; z < 4; z++) {
            house.addVoxel(0, y, z, VoxelType::Wood);
            house.addVoxel(4, y, z, VoxelType::Wood);
        }
    }
    
    // Door (remove blocks)
    house.addVoxel(2, 1, 0, VoxelType::Air);
    house.addVoxel(2, 2, 0, VoxelType::Air);
    
    // Roof (flat)
    for (int x = 0; x < 5; x++) {
        for (int z = 0; z < 5; z++) {
            house.addVoxel(x, 4, z, VoxelType::Wood);
        }
    }
    
    return house;
}

/**
 * Create a tower structure
 */
StructureBlueprint createTower(int height) {
    StructureBlueprint tower;
    
    // Base (3x3)
    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            tower.addVoxel(x, 0, z, VoxelType::Stone);
        }
    }
    
    // Tower shaft (hollow)
    for (int y = 1; y < height; y++) {
        // Corners only for hollow effect
        tower.addVoxel(0, y, 0, VoxelType::Stone);
        tower.addVoxel(2, y, 0, VoxelType::Stone);
        tower.addVoxel(0, y, 2, VoxelType::Stone);
        tower.addVoxel(2, y, 2, VoxelType::Stone);
    }
    
    // Top platform (3x3)
    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            tower.addVoxel(x, height, z, VoxelType::Stone);
        }
    }
    
    return tower;
}

/**
 * Create a bridge structure
 */
StructureBlueprint createBridge(int length) {
    StructureBlueprint bridge;
    
    // Pillars at each end
    for (int y = 0; y < 5; y++) {
        bridge.addVoxel(0, y, 0, VoxelType::Stone);
        bridge.addVoxel(0, y, 2, VoxelType::Stone);
        bridge.addVoxel(length - 1, y, 0, VoxelType::Stone);
        bridge.addVoxel(length - 1, y, 2, VoxelType::Stone);
    }
    
    // Bridge deck
    for (int x = 0; x < length; x++) {
        bridge.addVoxel(x, 5, 0, VoxelType::Wood);
        bridge.addVoxel(x, 5, 1, VoxelType::Wood);
        bridge.addVoxel(x, 5, 2, VoxelType::Wood);
    }
    
    // Railings
    for (int x = 1; x < length - 1; x++) {
        if (x % 2 == 0) { // Every other block
            bridge.addVoxel(x, 6, 0, VoxelType::Wood);
            bridge.addVoxel(x, 6, 2, VoxelType::Wood);
        }
    }
    
    return bridge;
}

/**
 * Create a pyramid structure
 */
StructureBlueprint createPyramid(int baseSize) {
    StructureBlueprint pyramid;
    
    int currentSize = baseSize;
    int y = 0;
    
    while (currentSize > 0) {
        int offset = (baseSize - currentSize) / 2;
        
        for (int x = 0; x < currentSize; x++) {
            for (int z = 0; z < currentSize; z++) {
                pyramid.addVoxel(offset + x, y, offset + z, VoxelType::Sand);
            }
        }
        
        currentSize -= 2;
        y++;
    }
    
    return pyramid;
}

int main() {
    std::cout << "=== Fresh Voxel Engine: Custom Structures ===" << std::endl;
    std::cout << "This example demonstrates building complex structures." << std::endl;
    
    // Step 1: Create world
    std::cout << "\n[1/5] Creating world..." << std::endl;
    VoxelWorld world;
    
    // Create flat terrain
    auto chunk = std::make_unique<Chunk>(0, 0);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            chunk->setVoxel(x, 60, z, VoxelType::Grass);
            for (int y = 0; y < 60; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Dirt);
            }
        }
    }
    world.addChunk(std::move(chunk));
    std::cout << "    ✓ Created flat terrain at y=60" << std::endl;
    
    // Step 2: Build a house
    std::cout << "\n[2/5] Building house..." << std::endl;
    StructureBlueprint house = createHouse();
    house.placeInWorld(world, 2, 61, 2);
    std::cout << "    ✓ House placed at (2, 61, 2)" << std::endl;
    std::cout << "    Size: 5x4x5 blocks (width x height x depth)" << std::endl;
    std::cout << "    Features: Stone foundation, wooden walls, door" << std::endl;
    
    // Step 3: Build a tower
    std::cout << "\n[3/5] Building tower..." << std::endl;
    StructureBlueprint tower = createTower(10);
    tower.placeInWorld(world, 8, 61, 2);
    std::cout << "    ✓ Tower placed at (8, 61, 2)" << std::endl;
    std::cout << "    Height: 10 blocks" << std::endl;
    std::cout << "    Style: Hollow stone tower with platform" << std::endl;
    
    // Step 4: Build a bridge
    std::cout << "\n[4/5] Building bridge..." << std::endl;
    StructureBlueprint bridge = createBridge(10);
    bridge.placeInWorld(world, 2, 56, 8);
    std::cout << "    ✓ Bridge placed at (2, 56, 8)" << std::endl;
    std::cout << "    Length: 10 blocks" << std::endl;
    std::cout << "    Features: Stone pillars, wooden deck, railings" << std::endl;
    
    // Step 5: Build a pyramid
    std::cout << "\n[5/5] Building pyramid..." << std::endl;
    StructureBlueprint pyramid = createPyramid(9);
    pyramid.placeInWorld(world, 2, 61, 12);
    std::cout << "    ✓ Pyramid placed at (2, 61, 12)" << std::endl;
    std::cout << "    Base: 9x9 blocks" << std::endl;
    std::cout << "    Height: 5 blocks" << std::endl;
    std::cout << "    Material: Sandstone" << std::endl;
    
    // Summary
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Creating structure blueprints" << std::endl;
    std::cout << "  • Building structures voxel-by-voxel" << std::endl;
    std::cout << "  • Reusable structure templates" << std::endl;
    std::cout << "  • Parametric structure generation" << std::endl;
    std::cout << "  • Placing structures at any world position" << std::endl;
    std::cout << "\nStructure Types Built:" << std::endl;
    std::cout << "  • House - Basic dwelling with walls and roof" << std::endl;
    std::cout << "  • Tower - Tall structure with hollow interior" << std::endl;
    std::cout << "  • Bridge - Connects two points" << std::endl;
    std::cout << "  • Pyramid - Layered stepped structure" << std::endl;
    std::cout << "\nDesign Patterns:" << std::endl;
    std::cout << "  • Use blueprints for reusability" << std::endl;
    std::cout << "  • Parametric functions for flexibility" << std::endl;
    std::cout << "  • Relative coordinates for positioning" << std::endl;
    std::cout << "  • Hollow structures for performance" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Add rotation and mirroring to blueprints" << std:endl;
    std::cout << "  • Create structure loading/saving" << std::endl;
    std::cout << "  • Try terraforming_tools.cpp for interactive building" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Structure Blueprints
 *    - Store structure as relative coordinates
 *    - Can be placed anywhere in world
 *    - Reusable templates
 *    - Easy to serialize/deserialize
 * 
 * 2. Relative Coordinates
 *    - Structure defined from origin (0,0,0)
 *    - Origin can be placed anywhere
 *    - Makes structures moveable
 *    - Simplifies rotation/mirroring
 * 
 * 3. Procedural Generation
 *    - Structures generated from parameters
 *    - Example: createTower(height)
 *    - Allows infinite variations
 *    - More flexible than fixed designs
 * 
 * 4. Hollow Structures
 *    - Only place exterior blocks
 *    - Reduces voxel count
 *    - Better performance
 *    - Interior space for gameplay
 * 
 * 5. Modular Building
 *    - Create small reusable pieces
 *    - Combine pieces into larger structures
 *    - Example: wall section, roof section
 *    - More maintainable code
 * 
 * ADVANCED FEATURES TO IMPLEMENT:
 * 
 * 1. Rotation:
 *    void rotate90(int& x, int& z) {
 *        int temp = x;
 *        x = -z;
 *        z = temp;
 *    }
 * 
 * 2. Mirroring:
 *    void mirrorX(int& x, int width) {
 *        x = width - x - 1;
 *    }
 * 
 * 3. Scaling:
 *    void scale(int& x, int& y, int& z, float factor) {
 *        x *= factor;
 *        y *= factor;
 *        z *= factor;
 *    }
 * 
 * 4. Serialization:
 *    - Save blueprint to file
 *    - Load blueprint from file
 *    - Share structures with others
 * 
 * STRUCTURE DESIGN TIPS:
 * 
 * • Plan on paper first
 * • Start with simple shapes
 * • Use symmetry when possible
 * • Consider interior space
 * • Add architectural details
 * • Test different sizes
 * 
 * PERFORMANCE CONSIDERATIONS:
 * 
 * • Minimize voxel count
 * • Use hollow designs
 * • Batch voxel placement
 * • Cache generated blueprints
 * • Generate structures async
 * 
 * COMMON STRUCTURE PATTERNS:
 * 
 * 1. Building:
 *    - Foundation layer
 *    - Wall perimeter
 *    - Interior divisions
 *    - Roof/ceiling
 * 
 * 2. Tower:
 *    - Wide base
 *    - Tapered middle
 *    - Platform top
 *    - Hollow or solid
 * 
 * 3. Bridge:
 *    - Support pillars
 *    - Deck/walkway
 *    - Railings
 *    - Arches (advanced)
 * 
 * 4. Pyramid:
 *    - Layered construction
 *    - Each layer smaller
 *    - Centered stacking
 *    - Optional interior
 * 
 * INTEGRATION WITH WORLD GEN:
 * 
 * // During chunk generation
 * if (shouldPlaceStructure()) {
 *     auto structure = createRandomStructure();
 *     int x = randomX();
 *     int y = getGroundLevel(x, z);
 *     int z = randomZ();
 *     structure.placeInWorld(world, x, y, z);
 * }
 * 
 * BLUEPRINTS VS ASSETS:
 * 
 * • Blueprints: Code-defined, parametric
 * • Assets: File-defined, fixed models
 * • Use blueprints for procedural
 * • Use assets for detailed models
 * • Can combine both approaches
 * 
 * STRUCTURE LIBRARY:
 * 
 * Create library of common structures:
 * - Houses (small, medium, large)
 * - Towers (wizard, guard, bell)
 * - Bridges (wood, stone, suspension)
 * - Walls (fortification)
 * - Gates and doors
 * - Decorative elements
 * 
 * SAVE/LOAD IMPLEMENTATION:
 * 
 * // Save
 * json blueprint = {
 *     {"name", "MyHouse"},
 *     {"voxels", /* array of voxel data */}
 * };
 * 
 * // Load
 * StructureBlueprint bp;
 * for (auto& v : json["voxels"]) {
 *     bp.addVoxel(v["x"], v["y"], v["z"], v["type"]);
 * }
 */
