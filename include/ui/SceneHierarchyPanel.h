#pragma once

#include <memory>
#include <string>
#include <vector>

namespace fresh
{

class VoxelWorld;

/**
 * @brief Represents a node in the scene hierarchy
 */
struct HierarchyNode {
    std::string name;
    bool visible;
    bool selected;
    std::vector<std::shared_ptr<HierarchyNode>> children;
    void* userData; // Can be entity, chunk, or other game object

    HierarchyNode(const std::string& nodeName = "Object")
        : name(nodeName), visible(true), selected(false), userData(nullptr)
    {
    }
};

/**
 * @brief Scene Hierarchy/Outliner Panel
 *
 * Displays a tree-structure list of all objects in the current scene,
 * allowing for easy selection, organization, and parent/child relationships.
 */
class SceneHierarchyPanel
{
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
     * @brief Get the currently selected node
     * @return Pointer to selected node or nullptr
     */
    HierarchyNode* getSelectedNode()
    {
        return m_selectedNode;
    }

    /**
     * @brief Refresh the scene hierarchy from world data
     */
    void refresh();

    /**
     * @brief Rename the currently selected node
     * @param newName New name for the node
     * @return true if successful
     */
    bool renameSelectedNode(const std::string& newName);

    /**
     * @brief Duplicate the currently selected node
     * @return true if successful
     */
    bool duplicateSelectedNode();

    /**
     * @brief Delete the currently selected node
     * @return true if successful
     */
    bool deleteSelectedNode();

private:
    void renderNode(HierarchyNode* node);
    void buildHierarchyFromWorld();
    bool findAndRemoveNode(std::shared_ptr<HierarchyNode>& parent, HierarchyNode* target);
    std::shared_ptr<HierarchyNode> duplicateNode(HierarchyNode* node);

private:
    bool m_visible;
    VoxelWorld* m_world;
    std::shared_ptr<HierarchyNode> m_rootNode;
    HierarchyNode* m_selectedNode;

    // Dialog state
    bool m_showRenameDialog;
    bool m_showDeleteDialog;
    char m_renameBuffer[256];
};

} // namespace fresh
