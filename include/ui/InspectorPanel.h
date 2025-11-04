#pragma once

namespace fresh {

struct SceneNode;

/**
 * @brief Inspector/Details Panel
 * 
 * Displays and allows editing of properties for the currently selected object,
 * including position, scale, components, scripts, and texture settings.
 */
class InspectorPanel {
public:
    InspectorPanel();
    ~InspectorPanel();

    /**
     * @brief Initialize the panel
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Render the panel UI
     */
    void render();

    /**
     * @brief Set panel visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible) { m_visible = visible; }

    /**
     * @brief Check if panel is visible
     * @return true if visible
     */
    bool isVisible() const { return m_visible; }

    /**
     * @brief Set the object to inspect
     * @param node Scene node to inspect
     */
    void setInspectedNode(SceneNode* node) { m_inspectedNode = node; }

private:
    void renderTransformProperties();
    void renderComponentProperties();
    void renderMaterialProperties();

private:
    bool m_visible;
    SceneNode* m_inspectedNode;
};

} // namespace fresh
