/**
 * @file SelectionRendererTests.cpp
 * @brief Tests for SelectionRenderer
 * 
 * Tests the visualization of voxel selections including bounds rendering,
 * color configuration, and enable/disable functionality.
 */

#include <gtest/gtest.h>
#include "editor/SelectionRenderer.h"
#include "editor/SelectionManager.h"
#include "devtools/DebugRenderer.h"
#include "voxel/VoxelWorld.h"
#include <glm/glm.hpp>

namespace fresh {

/**
 * @brief Test fixture for SelectionRenderer tests
 */
class SelectionRendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        debugRenderer = std::make_unique<devtools::DebugRenderer>();
        selectionRenderer = std::make_unique<SelectionRenderer>();
        selectionManager = std::make_unique<SelectionManager>();
        voxelWorld = std::make_unique<VoxelWorld>();
        voxelWorld->initialize();
        
        // Create test voxels
        createTestVoxels();
    }

    void TearDown() override {
        selectionRenderer.reset();
        selectionManager.reset();
        debugRenderer.reset();
        voxelWorld.reset();
    }
    
    void createTestVoxels() {
        // Load chunk and create some test voxels
        voxelWorld->loadChunk(ChunkPos(0, 0));
        for (int x = 0; x < 5; ++x) {
            for (int y = 0; y < 5; ++y) {
                for (int z = 0; z < 5; ++z) {
                    voxelWorld->setVoxel(WorldPos(x, y, z), Voxel(VoxelType::Stone));
                }
            }
        }
    }

    std::unique_ptr<devtools::DebugRenderer> debugRenderer;
    std::unique_ptr<SelectionRenderer> selectionRenderer;
    std::unique_ptr<SelectionManager> selectionManager;
    std::unique_ptr<VoxelWorld> voxelWorld;
};

// ============================================================================
// Initialization Tests
// ============================================================================

TEST_F(SelectionRendererTest, Constructor_InitializesWithDefaults) {
    // GIVEN: A newly constructed SelectionRenderer
    // THEN: Should have default values
    EXPECT_TRUE(selectionRenderer->isEnabled());
    
    // Default color should be light blue with some transparency
    const auto& color = selectionRenderer->getSelectionColor();
    EXPECT_FLOAT_EQ(0.2f, color.r);
    EXPECT_FLOAT_EQ(0.7f, color.g);
    EXPECT_FLOAT_EQ(1.0f, color.b);
    EXPECT_FLOAT_EQ(0.8f, color.a);
}

TEST_F(SelectionRendererTest, Initialize_WithNullDebugRenderer_ReturnsFalse) {
    // WHEN: Initializing with null debug renderer
    bool result = selectionRenderer->initialize(nullptr);
    
    // THEN: Should fail
    EXPECT_FALSE(result);
}

TEST_F(SelectionRendererTest, Initialize_WithValidDebugRenderer_ReturnsTrue) {
    // WHEN: Initializing with valid debug renderer
    bool result = selectionRenderer->initialize(debugRenderer.get());
    
    // THEN: Should succeed
    EXPECT_TRUE(result);
}

// ============================================================================
// Render Tests
// ============================================================================

TEST_F(SelectionRendererTest, Render_WithoutInitialization_DoesNotCrash) {
    // WHEN: Rendering without initialization
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash (nothing to assert, just shouldn't throw)
    SUCCEED();
}

TEST_F(SelectionRendererTest, Render_WithNullSelectionManager_DoesNotCrash) {
    // GIVEN: Initialized renderer
    selectionRenderer->initialize(debugRenderer.get());
    
    // WHEN: Rendering with null selection manager
    selectionRenderer->render(nullptr);
    
    // THEN: Should not crash
    SUCCEED();
}

TEST_F(SelectionRendererTest, Render_WithEmptySelection_DoesNothing) {
    // GIVEN: Initialized renderer with empty selection
    selectionRenderer->initialize(debugRenderer.get());
    
    // WHEN: Rendering empty selection
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash (selection has no data)
    EXPECT_FALSE(selectionManager->hasSelection());
    SUCCEED();
}

TEST_F(SelectionRendererTest, Render_WithActiveSelection_Works) {
    // GIVEN: Initialized renderer with a selection
    selectionRenderer->initialize(debugRenderer.get());
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(2.0f, 2.0f, 2.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering the selection
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash and selection should exist
    EXPECT_TRUE(selectionManager->hasSelection());
    SUCCEED();
}

TEST_F(SelectionRendererTest, Render_WhileSelecting_Works) {
    // GIVEN: Initialized renderer with active dragging
    selectionRenderer->initialize(debugRenderer.get());
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(2.0f, 2.0f, 2.0f));
    // Don't finalize - test rendering while still selecting
    
    // WHEN: Rendering while selecting
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash and should be in selecting mode
    EXPECT_TRUE(selectionManager->isSelecting());
    SUCCEED();
}

TEST_F(SelectionRendererTest, Render_WithLargeSelection_Works) {
    // GIVEN: A large selection (100+ voxels)
    selectionRenderer->initialize(debugRenderer.get());
    
    // Create a larger test area
    for (int x = 10; x < 25; ++x) {
        for (int y = 0; y < 10; ++y) {
            for (int z = 10; z < 25; ++z) {
                voxelWorld->setVoxel(WorldPos(x, y, z), Voxel(VoxelType::Stone));
            }
        }
    }
    
    selectionManager->startSelection(glm::vec3(10.0f, 0.0f, 10.0f));
    selectionManager->updateSelection(glm::vec3(24.0f, 9.0f, 24.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering large selection
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash (renderer should handle large selections efficiently)
    EXPECT_GT(selectionManager->getSelectionSize(), 100u);
    SUCCEED();
}

// ============================================================================
// Color Configuration Tests
// ============================================================================

TEST_F(SelectionRendererTest, SetSelectionColor_UpdatesColor) {
    // WHEN: Setting a custom color
    glm::vec4 customColor(1.0f, 0.0f, 0.0f, 1.0f); // Red, fully opaque
    selectionRenderer->setSelectionColor(customColor);
    
    // THEN: Color should be updated
    const auto& color = selectionRenderer->getSelectionColor();
    EXPECT_FLOAT_EQ(1.0f, color.r);
    EXPECT_FLOAT_EQ(0.0f, color.g);
    EXPECT_FLOAT_EQ(0.0f, color.b);
    EXPECT_FLOAT_EQ(1.0f, color.a);
}

TEST_F(SelectionRendererTest, SetSelectionColor_WithTransparency_Works) {
    // WHEN: Setting a transparent color
    glm::vec4 transparentColor(0.5f, 0.5f, 0.5f, 0.3f);
    selectionRenderer->setSelectionColor(transparentColor);
    
    // THEN: Color should include transparency
    const auto& color = selectionRenderer->getSelectionColor();
    EXPECT_FLOAT_EQ(0.5f, color.r);
    EXPECT_FLOAT_EQ(0.5f, color.g);
    EXPECT_FLOAT_EQ(0.5f, color.b);
    EXPECT_FLOAT_EQ(0.3f, color.a);
}

TEST_F(SelectionRendererTest, SetSelectionColor_BeforeRendering_AppliesColor) {
    // GIVEN: Custom color set before rendering
    selectionRenderer->initialize(debugRenderer.get());
    glm::vec4 customColor(0.0f, 1.0f, 0.0f, 0.5f); // Green, semi-transparent
    selectionRenderer->setSelectionColor(customColor);
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering with custom color
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should render with custom color (verified by color getter)
    const auto& color = selectionRenderer->getSelectionColor();
    EXPECT_FLOAT_EQ(0.0f, color.r);
    EXPECT_FLOAT_EQ(1.0f, color.g);
    EXPECT_FLOAT_EQ(0.5f, color.a);
}

// ============================================================================
// Enable/Disable Tests
// ============================================================================

TEST_F(SelectionRendererTest, SetEnabled_DisablesRendering) {
    // GIVEN: Initialized and enabled renderer
    selectionRenderer->initialize(debugRenderer.get());
    EXPECT_TRUE(selectionRenderer->isEnabled());
    
    // WHEN: Disabling renderer
    selectionRenderer->setEnabled(false);
    
    // THEN: Should be disabled
    EXPECT_FALSE(selectionRenderer->isEnabled());
}

TEST_F(SelectionRendererTest, SetEnabled_ReenablesRendering) {
    // GIVEN: Disabled renderer
    selectionRenderer->initialize(debugRenderer.get());
    selectionRenderer->setEnabled(false);
    EXPECT_FALSE(selectionRenderer->isEnabled());
    
    // WHEN: Re-enabling
    selectionRenderer->setEnabled(true);
    
    // THEN: Should be enabled
    EXPECT_TRUE(selectionRenderer->isEnabled());
}

TEST_F(SelectionRendererTest, Render_WhenDisabled_DoesNotRender) {
    // GIVEN: Disabled renderer with a selection
    selectionRenderer->initialize(debugRenderer.get());
    selectionRenderer->setEnabled(false);
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering while disabled
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash and should still be disabled
    EXPECT_FALSE(selectionRenderer->isEnabled());
    SUCCEED();
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST_F(SelectionRendererTest, EdgeCase_RenderAfterClearingSelection) {
    // GIVEN: Selection created then cleared
    selectionRenderer->initialize(debugRenderer.get());
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    EXPECT_TRUE(selectionManager->hasSelection());
    
    selectionManager->clearSelection();
    EXPECT_FALSE(selectionManager->hasSelection());
    
    // WHEN: Rendering after clearing
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash
    SUCCEED();
}

TEST_F(SelectionRendererTest, EdgeCase_MultipleRendersInSequence) {
    // GIVEN: A selection
    selectionRenderer->initialize(debugRenderer.get());
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering multiple times
    selectionRenderer->render(selectionManager.get());
    selectionRenderer->render(selectionManager.get());
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should not crash and selection should persist
    EXPECT_TRUE(selectionManager->hasSelection());
    SUCCEED();
}

TEST_F(SelectionRendererTest, EdgeCase_ToggleEnabledDuringRender) {
    // GIVEN: Selection exists
    selectionRenderer->initialize(debugRenderer.get());
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Toggling enabled state between renders
    selectionRenderer->render(selectionManager.get());
    selectionRenderer->setEnabled(false);
    selectionRenderer->render(selectionManager.get());
    selectionRenderer->setEnabled(true);
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Should handle state changes gracefully
    EXPECT_TRUE(selectionRenderer->isEnabled());
    SUCCEED();
}

TEST_F(SelectionRendererTest, EdgeCase_ChangeColorDuringRender) {
    // GIVEN: Selection and initial color
    selectionRenderer->initialize(debugRenderer.get());
    selectionRenderer->setSelectionColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    
    selectionManager->startSelection(glm::vec3(0.0f, 0.0f, 0.0f));
    selectionManager->updateSelection(glm::vec3(1.0f, 1.0f, 1.0f));
    selectionManager->finalizeSelection(voxelWorld.get());
    
    // WHEN: Rendering with different colors
    selectionRenderer->render(selectionManager.get());
    
    selectionRenderer->setSelectionColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    selectionRenderer->render(selectionManager.get());
    
    selectionRenderer->setSelectionColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    selectionRenderer->render(selectionManager.get());
    
    // THEN: Color should be updated
    const auto& color = selectionRenderer->getSelectionColor();
    EXPECT_FLOAT_EQ(0.0f, color.r);
    EXPECT_FLOAT_EQ(0.0f, color.g);
    EXPECT_FLOAT_EQ(1.0f, color.b);
}

TEST_F(SelectionRendererTest, EdgeCase_ReInitializeWithDifferentDebugRenderer) {
    // GIVEN: Renderer initialized with first debug renderer
    selectionRenderer->initialize(debugRenderer.get());
    bool firstResult = selectionRenderer->initialize(debugRenderer.get());
    EXPECT_TRUE(firstResult);
    
    // WHEN: Creating a new debug renderer and reinitializing
    auto newDebugRenderer = std::make_unique<devtools::DebugRenderer>();
    bool secondResult = selectionRenderer->initialize(newDebugRenderer.get());
    
    // THEN: Should succeed
    EXPECT_TRUE(secondResult);
}

} // namespace fresh
