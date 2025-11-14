#include "ui/SceneHierarchyPanel.h"

#include <cstring>
#include <functional>

#include "core/Logger.h"
#include "voxel/VoxelWorld.h"

#include <imgui.h>

namespace fresh
{

SceneHierarchyPanel::SceneHierarchyPanel()
    : m_visible(true),
      m_world(nullptr),
      m_rootNode(nullptr),
      m_selectedNode(nullptr),
      m_showRenameDialog(false),
      m_showDeleteDialog(false)
{
    m_rootNode = std::make_shared<HierarchyNode>("Scene Root");
    m_renameBuffer[0] = '\0';
}

SceneHierarchyPanel::~SceneHierarchyPanel() {}

bool SceneHierarchyPanel::initialize(VoxelWorld* world)
{
    if (!world) {
        LOG_ERROR_C("Invalid world pointer", "SceneHierarchyPanel");
        return false;
    }

    m_world = world;
    buildHierarchyFromWorld();

    LOG_INFO_C("Scene Hierarchy Panel initialized", "SceneHierarchyPanel");
    return true;
}

void SceneHierarchyPanel::render()
{
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Scene Hierarchy", &m_visible);

    // Refresh button
    if (ImGui::Button("Refresh")) {
        refresh();
    }
    ImGui::SameLine();

    // Filter/search box
    static char searchBuffer[256] = "";
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##search", "Search...", searchBuffer, sizeof(searchBuffer));

    ImGui::Separator();

    // Render hierarchy tree
    if (m_rootNode) {
        renderNode(m_rootNode.get());
    }

    // Render dialogs
    if (m_showRenameDialog && m_selectedNode) {
        ImGui::OpenPopup("Rename Node");
        if (ImGui::BeginPopupModal("Rename Node", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Enter new name for '%s':", m_selectedNode->name.c_str());
            ImGui::InputText("##rename", m_renameBuffer, sizeof(m_renameBuffer));
            ImGui::Separator();

            if (ImGui::Button("Rename", ImVec2(120, 0))) {
                if (m_renameBuffer[0] != '\0') {
                    if (renameSelectedNode(std::string(m_renameBuffer))) {
                        LOG_INFO_C("Renamed node to: " + std::string(m_renameBuffer),
                                   "SceneHierarchyPanel");
                    }
                }
                m_showRenameDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                m_showRenameDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    if (m_showDeleteDialog && m_selectedNode) {
        ImGui::OpenPopup("Delete Node?");
        if (ImGui::BeginPopupModal("Delete Node?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete '%s'?", m_selectedNode->name.c_str());
            ImGui::Text("This will delete the node and all its children.");
            ImGui::Separator();

            if (ImGui::Button("Delete", ImVec2(120, 0))) {
                if (deleteSelectedNode()) {
                    LOG_INFO_C("Deleted node: " + m_selectedNode->name, "SceneHierarchyPanel");
                }
                m_showDeleteDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                m_showDeleteDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void SceneHierarchyPanel::renderNode(HierarchyNode* node)
{
    if (!node) {
        return;
    }

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (node->selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (node->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    // Node icon based on type
    const char* icon = "○ "; // Default object icon
    if (node->name.find("Chunk") != std::string::npos) {
        icon = "■ "; // Chunk icon
    } else if (node->name.find("World") != std::string::npos) {
        icon = "🌍 "; // World icon
    } else if (node->name.find("Entity") != std::string::npos) {
        icon = "⚡ "; // Entity icon
    }

    bool nodeOpen = ImGui::TreeNodeEx(node, flags, "%s%s", icon, node->name.c_str());

    // Handle selection
    if (ImGui::IsItemClicked()) {
        if (m_selectedNode) {
            m_selectedNode->selected = false;
        }
        node->selected = true;
        m_selectedNode = node;
    }

    // Context menu
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Rename")) {
            m_showRenameDialog = true;
#ifdef _MSC_VER
            strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), node->name.c_str(), _TRUNCATE);
#else
            strncpy(m_renameBuffer, node->name.c_str(), sizeof(m_renameBuffer) - 1);
            m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
#endif
        }
        if (ImGui::MenuItem("Duplicate")) {
            if (duplicateSelectedNode()) {
                LOG_INFO_C("Duplicated node: " + node->name, "SceneHierarchyPanel");
            }
        }
        if (ImGui::MenuItem("Delete")) {
            m_showDeleteDialog = true;
        }
        ImGui::Separator();
        if (ImGui::MenuItem(node->visible ? "Hide" : "Show")) {
            node->visible = !node->visible;
        }
        ImGui::EndPopup();
    }

    // Render children
    if (nodeOpen && !node->children.empty()) {
        for (auto& child : node->children) {
            renderNode(child.get());
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::buildHierarchyFromWorld()
{
    if (!m_world) {
        return;
    }

    // Clear existing hierarchy
    m_rootNode->children.clear();

    // Create "World" node
    auto worldNode = std::make_shared<HierarchyNode>("Voxel World");

    // Add chunks as children
    auto chunksNode = std::make_shared<HierarchyNode>(
        "Chunks (" + std::to_string(m_world->getChunks().size()) + ")");

    // Add a few sample chunks (limited to avoid performance issues)
    int chunkCount = 0;
    for (const auto& pair : m_world->getChunks()) {
        if (chunkCount >= 100) { // Limit to first 100 chunks
            auto moreNode = std::make_shared<HierarchyNode>(
                "... and " + std::to_string(m_world->getChunks().size() - 100) + " more");
            chunksNode->children.push_back(moreNode);
            break;
        }

        const ChunkPos& pos = pair.first;
        auto chunkNode = std::make_shared<HierarchyNode>("Chunk (" + std::to_string(pos.x) + ", " +
                                                         std::to_string(pos.z) + ")");
        chunksNode->children.push_back(chunkNode);
        chunkCount++;
    }

    worldNode->children.push_back(chunksNode);

    // Add entities node (placeholder for future)
    auto entitiesNode = std::make_shared<HierarchyNode>("Entities (0)");
    worldNode->children.push_back(entitiesNode);

    // Add lights node (placeholder for future)
    auto lightsNode = std::make_shared<HierarchyNode>("Lights (0)");
    worldNode->children.push_back(lightsNode);

    m_rootNode->children.push_back(worldNode);
}

void SceneHierarchyPanel::refresh()
{
    buildHierarchyFromWorld();
    LOG_INFO_C("Scene hierarchy refreshed", "SceneHierarchyPanel");
}

bool SceneHierarchyPanel::renameSelectedNode(const std::string& newName)
{
    if (!m_selectedNode || newName.empty()) {
        return false;
    }

    m_selectedNode->name = newName;
    return true;
}

bool SceneHierarchyPanel::duplicateSelectedNode()
{
    if (!m_selectedNode || !m_rootNode) {
        return false;
    }

    // Create a duplicate of the node
    auto duplicatedNode = duplicateNode(m_selectedNode);
    if (!duplicatedNode) {
        return false;
    }

    // Find parent and add duplicate as sibling
    // For simplicity, add to root for now
    m_rootNode->children.push_back(duplicatedNode);

    return true;
}

bool SceneHierarchyPanel::deleteSelectedNode()
{
    if (!m_selectedNode || !m_rootNode) {
        return false;
    }

    // Don't allow deleting root
    if (m_selectedNode == m_rootNode.get()) {
        LOG_WARNING_C("Cannot delete root node", "SceneHierarchyPanel");
        return false;
    }

    // Find and remove the node
    bool removed = findAndRemoveNode(m_rootNode, m_selectedNode);
    if (removed) {
        m_selectedNode = nullptr;
    }

    return removed;
}

bool SceneHierarchyPanel::findAndRemoveNode(std::shared_ptr<HierarchyNode>& parent,
                                            HierarchyNode* target)
{
    if (!parent || !target) {
        return false;
    }

    // Check direct children
    for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
        if (it->get() == target) {
            parent->children.erase(it);
            return true;
        }
    }

    // Recursively search children
    for (auto& child : parent->children) {
        if (findAndRemoveNode(child, target)) {
            return true;
        }
    }

    return false;
}

std::shared_ptr<HierarchyNode> SceneHierarchyPanel::duplicateNode(HierarchyNode* node)
{
    if (!node) {
        return nullptr;
    }

    auto duplicate = std::make_shared<HierarchyNode>(node->name + " (Copy)");
    duplicate->visible = node->visible;
    duplicate->userData = node->userData;

    // Recursively duplicate children
    for (auto& child : node->children) {
        auto childDuplicate = duplicateNode(child.get());
        if (childDuplicate) {
            duplicate->children.push_back(childDuplicate);
        }
    }

    return duplicate;
}

void SceneHierarchyPanel::selectAll()
{
    if (!m_rootNode) {
        return;
    }

    // Recursive function to select all nodes
    std::function<void(HierarchyNode*)> selectNodeAndChildren = [&](HierarchyNode* node) {
        if (!node) return;
        node->selected = true;
        for (auto& child : node->children) {
            selectNodeAndChildren(child.get());
        }
    };

    selectNodeAndChildren(m_rootNode.get());
    LOG_INFO_C("Selected all nodes in scene hierarchy", "SceneHierarchyPanel");
}

void SceneHierarchyPanel::deselectAll()
{
    if (!m_rootNode) {
        return;
    }

    // Recursive function to deselect all nodes
    std::function<void(HierarchyNode*)> deselectNodeAndChildren = [&](HierarchyNode* node) {
        if (!node) return;
        node->selected = false;
        for (auto& child : node->children) {
            deselectNodeAndChildren(child.get());
        }
    };

    deselectNodeAndChildren(m_rootNode.get());
    m_selectedNode = nullptr;
    LOG_INFO_C("Deselected all nodes in scene hierarchy", "SceneHierarchyPanel");
}

} // namespace fresh
