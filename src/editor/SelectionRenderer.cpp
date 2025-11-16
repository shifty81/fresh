#include "editor/SelectionRenderer.h"
#include "devtools/DebugRenderer.h"
#include "core/Logger.h"

namespace fresh
{

SelectionRenderer::SelectionRenderer()
    : m_debugRenderer(nullptr)
    , m_selectionColor(0.2f, 0.7f, 1.0f, 0.8f) // Light blue with some transparency
    , m_enabled(true)
{
}

SelectionRenderer::~SelectionRenderer() = default;

bool SelectionRenderer::initialize(devtools::DebugRenderer* debugRenderer)
{
    if (debugRenderer == nullptr) {
        Logger::error("Cannot initialize SelectionRenderer with null DebugRenderer");
        return false;
    }

    m_debugRenderer = debugRenderer;
    Logger::info("SelectionRenderer initialized");
    return true;
}

void SelectionRenderer::render(const SelectionManager* selectionManager)
{
    if (!m_enabled || m_debugRenderer == nullptr || selectionManager == nullptr) {
        return;
    }

    // Check if there's a selection to render
    if (!selectionManager->hasSelection()) {
        return;
    }

    glm::ivec3 min, max;
    if (!selectionManager->getSelectionBounds(min, max)) {
        return;
    }

    // Calculate center and size for the bounding box
    glm::vec3 center = glm::vec3(min + max) * 0.5f + glm::vec3(0.5f);
    glm::vec3 size = glm::vec3(max - min) + glm::vec3(1.0f);

    // Draw the selection box
    m_debugRenderer->drawBox(center, size, m_selectionColor, 0.0f);

    // Optionally, draw individual voxel outlines for smaller selections
    const auto& selectedVoxels = selectionManager->getSelectedVoxels();
    if (selectedVoxels.size() < 100) { // Only for small selections to avoid performance issues
        for (const auto& voxelPos : selectedVoxels) {
            glm::vec3 voxelCenter = glm::vec3(voxelPos.x, voxelPos.y, voxelPos.z) + glm::vec3(0.5f);
            glm::vec3 voxelSize = glm::vec3(1.0f);
            
            // Use a slightly different color for individual voxels
            glm::vec4 voxelColor = m_selectionColor;
            voxelColor.a *= 0.5f; // More transparent
            
            m_debugRenderer->drawBox(voxelCenter, voxelSize, voxelColor, 0.0f);
        }
    }
}

} // namespace fresh
