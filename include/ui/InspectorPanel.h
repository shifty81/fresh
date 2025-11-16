#pragma once

namespace fresh
{

struct HierarchyNode;

namespace ecs
{
class EntityManager;
}

/**
 * @brief Inspector/Details Panel
 *
 * Displays and allows editing of properties for the currently selected object,
 * including position, scale, components, scripts, and texture settings.
 */
class InspectorPanel
{
public:
    InspectorPanel();
    ~InspectorPanel();

    /**
     * @brief Initialize the panel
     * @param entityManager Optional entity manager for component editing
     * @return true if successful
     */
    bool initialize(ecs::EntityManager* entityManager = nullptr);

    /**
     * @brief Render the panel UI
     */
    void render();

    /**
     * @brief Set panel visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    /**
     * @brief Check if panel is visible
     * @return true if visible
     */
    bool isVisible() const
    {
        return m_visible;
    }

    /**
     * @brief Set the object to inspect
     * @param node Scene node to inspect
     */
    void setInspectedNode(HierarchyNode* node)
    {
        m_inspectedNode = node;
    }

private:
    void renderTransformProperties();
    void renderComponentProperties();
    void renderMaterialProperties();
    void renderPhysicsProperties();
    void renderRendererProperties();

private:
    bool m_visible;
    HierarchyNode* m_inspectedNode;
    ecs::EntityManager* m_entityManager;
};

} // namespace fresh
