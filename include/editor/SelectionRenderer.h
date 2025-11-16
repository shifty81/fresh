#pragma once

#include <glm/glm.hpp>
#include "editor/SelectionManager.h"

namespace fresh
{

// Forward declarations
namespace devtools
{
class DebugRenderer;
}

/**
 * @brief Renders visual feedback for voxel selection
 * 
 * Shows selection bounds and highlights selected voxels using wireframe rendering.
 */
class SelectionRenderer
{
public:
    SelectionRenderer();
    ~SelectionRenderer();

    /**
     * @brief Initialize the selection renderer
     * @param debugRenderer Pointer to debug renderer for drawing
     * @return true if successful
     */
    bool initialize(devtools::DebugRenderer* debugRenderer);

    /**
     * @brief Render the current selection
     * @param selectionManager Selection manager to visualize
     */
    void render(const SelectionManager* selectionManager);

    /**
     * @brief Set selection color
     * @param color RGBA color for selection outline
     */
    void setSelectionColor(const glm::vec4& color)
    {
        m_selectionColor = color;
    }

    /**
     * @brief Get selection color
     * @return Current selection color
     */
    const glm::vec4& getSelectionColor() const
    {
        return m_selectionColor;
    }

    /**
     * @brief Set if selection should be rendered
     * @param enabled true to enable rendering
     */
    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    /**
     * @brief Check if selection rendering is enabled
     * @return true if enabled
     */
    bool isEnabled() const
    {
        return m_enabled;
    }

private:
    devtools::DebugRenderer* m_debugRenderer;
    glm::vec4 m_selectionColor;
    bool m_enabled;
};

} // namespace fresh
