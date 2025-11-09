#pragma once

#include <memory>

namespace fresh
{

class WorldEditor;
class TerraformingSystem;

/**
 * @brief ImGui-based editor interface
 *
 * Provides the graphical user interface for the world editor,
 * including terraforming tools, world settings, and debug info.
 */
class EditorGUI
{
public:
    EditorGUI();
    ~EditorGUI();

    /**
     * @brief Initialize editor GUI
     * @param worldEditor World editor instance
     * @return true if successful
     */
    bool initialize(WorldEditor* worldEditor);

    /**
     * @brief Render editor interface
     */
    void render();

    /**
     * @brief Toggle editor visibility
     */
    void toggle()
    {
        m_visible = !m_visible;
    }

    /**
     * @brief Set editor visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    /**
     * @brief Check if editor is visible
     * @return true if visible, false otherwise
     */
    bool isVisible() const
    {
        return m_visible;
    }

private:
    void renderTerraformingPanel();
    void renderWorldInfoPanel();
    void renderToolSelector();
    void renderMaterialSelector();

private:
    WorldEditor* m_worldEditor;
    bool m_visible;
    bool m_showTerraformingPanel;
    bool m_showWorldInfo;
};

} // namespace fresh
