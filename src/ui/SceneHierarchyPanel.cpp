#include "ui/SceneHierarchyPanel.h"
#include "voxel/VoxelWorld.h"
#include "core/Logger.h"
#include <imgui.h>

namespace fresh {

SceneHierarchyPanel::SceneHierarchyPanel()
    : m_visible(true)
    , m_world(nullptr)
    , m_rootNode(nullptr)
    , m_selectedNode(nullptr)
{
    m_rootNode = std::make_shared<SceneNode>("Scene Root");
}

SceneHierarchyPanel::~SceneHierarchyPanel() {
}

bool SceneHierarchyPanel::initialize(VoxelWorld* world) {
    if (!world) {
        LOG_ERROR_C("Invalid world pointer", "SceneHierarchyPanel");
        return false;
    }

    m_world = world;
    buildHierarchyFromWorld();
    
    LOG_INFO_C("Scene Hierarchy Panel initialized", "SceneHierarchyPanel");
    return true;
}

void SceneHierarchyPanel::render() {
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

    ImGui::End();
}

void SceneHierarchyPanel::renderNode(SceneNode* node) {
    if (!node) {
        return;
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    
    if (node->selected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    
    if (node->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    // Node icon based on type
    const char* icon = "○ ";  // Default object icon
    if (node->name.find("Chunk") != std::string::npos) {
        icon = "■ ";  // Chunk icon
    } else if (node->name.find("World") != std::string::npos) {
        icon = "🌍 ";  // World icon
    } else if (node->name.find("Entity") != std::string::npos) {
        icon = "⚡ ";  // Entity icon
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
            // TODO: Implement rename dialog
        }
        if (ImGui::MenuItem("Duplicate")) {
            // TODO: Implement duplication
        }
        if (ImGui::MenuItem("Delete")) {
            // TODO: Implement deletion
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

void SceneHierarchyPanel::buildHierarchyFromWorld() {
    if (!m_world) {
        return;
    }

    // Clear existing hierarchy
    m_rootNode->children.clear();

    // Create "World" node
    auto worldNode = std::make_shared<SceneNode>("Voxel World");
    
    // Add chunks as children
    auto chunksNode = std::make_shared<SceneNode>("Chunks (" + std::to_string(m_world->getChunks().size()) + ")");
    
    // Add a few sample chunks (limited to avoid performance issues)
    int chunkCount = 0;
    for (const auto& pair : m_world->getChunks()) {
        if (chunkCount >= 100) {  // Limit to first 100 chunks
            auto moreNode = std::make_shared<SceneNode>("... and " + 
                std::to_string(m_world->getChunks().size() - 100) + " more");
            chunksNode->children.push_back(moreNode);
            break;
        }
        
        const ChunkPos& pos = pair.first;
        auto chunkNode = std::make_shared<SceneNode>(
            "Chunk (" + std::to_string(pos.x) + ", " + std::to_string(pos.z) + ")"
        );
        chunksNode->children.push_back(chunkNode);
        chunkCount++;
    }
    
    worldNode->children.push_back(chunksNode);
    
    // Add entities node (placeholder for future)
    auto entitiesNode = std::make_shared<SceneNode>("Entities (0)");
    worldNode->children.push_back(entitiesNode);
    
    // Add lights node (placeholder for future)
    auto lightsNode = std::make_shared<SceneNode>("Lights (0)");
    worldNode->children.push_back(lightsNode);

    m_rootNode->children.push_back(worldNode);
}

void SceneHierarchyPanel::refresh() {
    buildHierarchyFromWorld();
    LOG_INFO_C("Scene hierarchy refreshed", "SceneHierarchyPanel");
}

} // namespace fresh
