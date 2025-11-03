/**
 * @file terraforming_tools.cpp
 * @brief Demonstrates using the terraforming system for advanced world editing
 * 
 * This example shows:
 * - Using different terraforming tools (Brush, Sphere, Cube, etc.)
 * - Applying tools with different modes (Place, Remove, Replace)
 * - Undo/Redo functionality
 * - Tool size adjustment
 * - Practical editing workflows
 */

#include <iostream>
#include <memory>
#include "editor/TerraformingSystem.h"
#include "editor/WorldEditor.h"
#include "voxel/VoxelWorld.h"
#include "voxel/VoxelTypes.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Terraforming Tools ===" << std::endl;
    std::cout << "This example demonstrates the powerful terraforming system." << std::endl;
    
    // Step 1: Create world and editor
    std::cout << "\n[1/7] Setting up world and editor..." << std::endl;
    auto world = std::make_shared<VoxelWorld>();
    
    // Create initial terrain
    auto chunk = std::make_unique<Chunk>(0, 0);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y <= 60; y++) {
                chunk->setVoxel(x, y, z, VoxelType::Stone);
            }
        }
    }
    world->addChunk(std::move(chunk));
    
    WorldEditor editor(world);
    TerraformingSystem* terraform = editor.getTerraformingSystem();
    std::cout << "    ✓ World editor initialized" << std::endl;
    std::cout << "    ✓ Terraforming system ready" << std::endl;
    
    // Step 2: Single Block Tool
    std::cout << "\n[2/7] Using Single Block tool..." << std::endl;
    terraform->setTool(TerraformTool::SingleBlock);
    terraform->setVoxelType(VoxelType::Diamond);
    terraform->setEditMode(TerraformMode::Place);
    
    terraform->applyTool(WorldPos(5, 61, 5));
    std::cout << "    ✓ Placed single Diamond block at (5, 61, 5)" << std::endl;
    
    // Step 3: Brush Tool (Natural Sculpting)
    std::cout << "\n[3/7] Using Brush tool..." << std::endl;
    terraform->setTool(TerraformTool::Brush);
    terraform->setVoxelType(VoxelType::Grass);
    terraform->setToolSize(3);
    
    terraform->applyTool(WorldPos(8, 61, 8));
    std::cout << "    ✓ Applied Grass brush (size 3) at (8, 61, 8)" << std::endl;
    std::cout << "    Creates natural, spherical terrain modification" << std::endl;
    
    // Step 4: Filled Sphere Tool
    std::cout << "\n[4/7] Using Filled Sphere tool..." << std::endl;
    terraform->setTool(TerraformTool::FilledSphere);
    terraform->setVoxelType(VoxelType::Stone);
    terraform->setToolSize(5);
    
    terraform->applyTool(WorldPos(12, 65, 12));
    std::cout << "    ✓ Created stone sphere (radius 5) at (12, 65, 12)" << std::endl;
    
    // Step 5: Cube Tool
    std::cout << "\n[5/7] Using Cube tool..." << std::endl;
    terraform->setTool(TerraformTool::Cube);
    terraform->setVoxelType(VoxelType::Wood);
    terraform->setToolSize(4);
    
    terraform->applyTool(WorldPos(3, 61, 10));
    std::cout << "    ✓ Created wooden cube frame (size 4) at (3, 61, 10)" << std::endl;
    std::cout << "    (Hollow cube - only edges)" << std::endl;
    
    // Step 6: Flatten Tool
    std::cout << "\n[6/7] Using Flatten tool..." << std::endl;
    terraform->setTool(TerraformTool::Flatten);
    terraform->setToolSize(5);
    
    WorldPos flattenPos(10, 62, 5);
    terraform->applyTool(flattenPos);
    std::cout << "    ✓ Flattened area (size 5) to height 62" << std::endl;
    std::cout << "    All blocks in area set to same Y level" << std::endl;
    
    // Step 7: Undo/Redo System
    std::cout << "\n[7/7] Demonstrating Undo/Redo..." << std::endl;
    
    // Make a change
    terraform->setTool(TerraformTool::SingleBlock);
    terraform->setVoxelType(VoxelType::Gold);
    terraform->applyTool(WorldPos(7, 61, 7));
    std::cout << "    ✓ Placed Gold block at (7, 61, 7)" << std::endl;
    
    // Undo the change
    if (terraform->canUndo()) {
        terraform->undo();
        std::cout << "    ✓ Undid Gold block placement" << std::endl;
    }
    
    // Redo the change
    if (terraform->canRedo()) {
        terraform->redo();
        std::cout << "    ✓ Redid Gold block placement" << std::endl;
    }
    
    // Show undo history size
    std::cout << "    History size: " << terraform->getHistorySize() << " operations" << std::endl;
    std::cout << "    (Maximum: 100 operations)" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Using different terraforming tools" << std::endl;
    std::cout << "  • Adjusting tool size for different effects" << std::endl;
    std::cout << "  • Changing voxel types for different materials" << std::endl;
    std::cout << "  • Using Undo/Redo for mistake correction" << std::endl;
    std::cout << "\nAvailable Tools:" << std::endl;
    std::cout << "  • SingleBlock - Precise single-block placement" << std::endl;
    std::cout << "  • Brush - Natural, spherical sculpting" << std::endl;
    std::cout << "  • Sphere - Hollow sphere outline" << std::endl;
    std::cout << "  • FilledSphere - Solid sphere" << std::endl;
    std::cout << "  • Cube - Hollow cube frame" << std::endl;
    std::cout << "  • FilledCube - Solid cube" << std::endl;
    std::cout << "  • Line - Straight line between points" << std::endl;
    std::cout << "  • Flatten - Level terrain to height" << std::endl;
    std::cout << "  • Smooth - Average heights for smooth terrain" << std::endl;
    std::cout << "  • Paint - Change block types without geometry change" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try custom_structures.cpp for building complex shapes" << std::endl;
    std::cout << "  • See docs/TERRAFORMING.md for complete documentation" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. TerraformingSystem
 *    - Provides high-level editing tools
 *    - Handles undo/redo automatically
 *    - Works with any VoxelWorld
 *    - Thread-safe for multi-chunk editing
 * 
 * 2. Terraform Tools
 *    - SingleBlock: Precise editing
 *    - Brush: Natural terrain sculpting
 *    - Sphere/FilledSphere: Round structures
 *    - Cube/FilledCube: Rectangular structures
 *    - Line: Connect two points
 *    - Flatten: Level terrain
 *    - Smooth: Blend heights
 *    - Paint: Change materials only
 * 
 * 3. Edit Modes
 *    - Place: Add blocks (default)
 *    - Remove: Delete blocks (set to Air)
 *    - Replace: Change existing blocks only
 * 
 * 4. Tool Size
 *    - Range: 1-10 blocks
 *    - Affects radius for spherical tools
 *    - Affects dimensions for cubic tools
 *    - Larger sizes = slower operations
 * 
 * 5. Undo/Redo System
 *    - Stores up to 100 operations
 *    - Each tool application = 1 operation
 *    - Stored as delta (what changed)
 *    - Memory-efficient compression
 * 
 * PRACTICAL WORKFLOWS:
 * 
 * 1. Creating a Mountain:
 *    - Use Brush tool with Stone
 *    - Large size (8-10) for base
 *    - Smaller size (3-5) for peak
 *    - Smooth tool for natural slopes
 * 
 * 2. Building a House:
 *    - FilledCube for foundation (Stone)
 *    - Cube for walls (Wood)
 *    - FilledCube for roof (Wood)
 *    - SingleBlock for doors/windows (Air)
 * 
 * 3. Creating a Lake:
 *    - FilledSphere in Remove mode (dig hole)
 *    - Paint tool to add Water
 *    - Smooth edges for natural shore
 * 
 * 4. Clearing Area:
 *    - Flatten at desired height
 *    - Or use FilledCube in Remove mode
 * 
 * PERFORMANCE TIPS:
 * 
 * • Large tool sizes are slower (especially FilledSphere/Cube)
 * • Undo/redo is instant (no recalculation)
 * • Batch operations when possible
 * • Consider disabling auto-mesh-update for batch edits
 * 
 * BEST PRACTICES:
 * 
 * • Use appropriate tool for the job
 * • Start with large tools, refine with small
 * • Test with Undo before committing
 * • Save frequently when making major changes
 * • Use Paint mode to experiment with materials
 */
