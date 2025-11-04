#pragma once

#include <string>
#include <vector>
#include <memory>

namespace fresh {

class VoxelWorld;

/**
 * @brief Represents a node in the scene hierarchy
 */
struct SceneNode {
    std::string name;
    bool visible;
    bool selected;
    std::vector<std::shared_ptr<SceneNode>> children;
    void* userData;  // Can be entity, chunk, or other game object

    SceneNode(const std::string& nodeName = "Object")
        : name(nodeName), visible(true), selected(false), userData(nullptr) {}
};

/**
 * @brief Scene Hierarchy/Outliner Panel
 * 
 * Displays a tree-structure list of all objects in the current scene,
 * allowing for easy selection, organization, and parent/child relationships.
 */
class SceneHierarchyPanel {
public:
    SceneHierarchyPanel();
    ~SceneHierarchyPanel();

    /**
     * @brief Initialize the panel
     * @param world Voxel world to display
     * @return true if successful
     */
    bool initialize(VoxelWorld* world);

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
     * @brief Get the currently selected node
     * @return Pointer to selected node or nullptr
     */
    SceneNode* getSelectedNode() { return m_selectedNode; }

    /**
     * @brief Refresh the scene hierarchy from world data
     */
    void refresh();

private:
    void renderNode(SceneNode* node);
    void buildHierarchyFromWorld();

private:
    bool m_visible;
    VoxelWorld* m_world;
    std::shared_ptr<SceneNode> m_rootNode;
    SceneNode* m_selectedNode;
};

} // namespace fresh
