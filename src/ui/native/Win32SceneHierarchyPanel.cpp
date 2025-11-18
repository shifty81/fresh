#ifdef _WIN32

#include "ui/native/Win32SceneHierarchyPanel.h"
#include "ui/native/UnrealStyleTheme.h"
#include "ui/SceneHierarchyPanel.h"
#include "voxel/VoxelWorld.h"
#include "core/Logger.h"
#include <windowsx.h>

namespace fresh
{

// Context menu command IDs
constexpr int CMD_RENAME = 2001;
constexpr int CMD_DUPLICATE = 2002;
constexpr int CMD_DELETE = 2003;
constexpr int CMD_ADD_OBJECT = 2004;

Win32SceneHierarchyPanel::Win32SceneHierarchyPanel()
    : Win32Panel(),
      m_world(nullptr),
      m_treeView(nullptr),
      m_rootNode(nullptr),
      m_selectedNode(nullptr),
      m_selectionCallback(nullptr),
      m_contextMenu(nullptr)
{
}

Win32SceneHierarchyPanel::~Win32SceneHierarchyPanel()
{
    if (m_contextMenu) {
        DestroyMenu(m_contextMenu);
    }
}

bool Win32SceneHierarchyPanel::create(HWND parent, int x, int y, int width, int height, VoxelWorld* world)
{
    m_world = world;
    
    if (!Win32Panel::create(parent, x, y, width, height, L"Scene Hierarchy")) {
        return false;
    }
    
    // Create tree view control
    m_treeView = std::make_unique<Win32TreeView>();
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    
    if (!m_treeView->create(m_hwnd, 0, 0, clientRect.right, clientRect.bottom)) {
        LOG_ERROR_C("Failed to create TreeView control", "Win32SceneHierarchyPanel");
        return false;
    }
    
    // Create context menu
    m_contextMenu = CreatePopupMenu();
    AppendMenuW(m_contextMenu, MF_STRING, CMD_ADD_OBJECT, L"Add Object");
    AppendMenuW(m_contextMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(m_contextMenu, MF_STRING, CMD_RENAME, L"Rename");
    AppendMenuW(m_contextMenu, MF_STRING, CMD_DUPLICATE, L"Duplicate");
    AppendMenuW(m_contextMenu, MF_STRING, CMD_DELETE, L"Delete");
    
    // Build initial hierarchy
    refresh();
    
    LOG_INFO_C("Win32SceneHierarchyPanel created", "Win32SceneHierarchyPanel");
    return true;
}

void Win32SceneHierarchyPanel::refresh()
{
    if (!m_treeView) {
        return;
    }
    
    // Clear existing tree
    m_treeView->clear();
    m_itemToNode.clear();
    m_nodeToItem.clear();
    m_selectedNode = nullptr;
    
    // Build hierarchy from world
    buildHierarchyFromWorld();
}

void Win32SceneHierarchyPanel::buildHierarchyFromWorld()
{
    // Create root node
    m_rootNode = std::make_shared<HierarchyNode>("World");
    
    if (!m_world) {
        // Add placeholder items
        HTREEITEM rootItem = m_treeView->addRootItem("World", m_rootNode.get());
        m_itemToNode[rootItem] = m_rootNode.get();
        m_nodeToItem[m_rootNode.get()] = rootItem;
        return;
    }
    
    // Add root item to tree
    HTREEITEM rootItem = m_treeView->addRootItem("World", m_rootNode.get());
    m_itemToNode[rootItem] = m_rootNode.get();
    m_nodeToItem[m_rootNode.get()] = rootItem;
    
    // Add chunks as children
    // Note: In a real implementation, we would iterate through world chunks
    // For now, add sample hierarchy
    
    // Entities node
    auto entitiesNode = std::make_shared<HierarchyNode>("Entities");
    m_rootNode->children.push_back(entitiesNode);
    addNodeToTree(entitiesNode.get(), rootItem);
    
    // Terrain node
    auto terrainNode = std::make_shared<HierarchyNode>("Terrain");
    m_rootNode->children.push_back(terrainNode);
    addNodeToTree(terrainNode.get(), rootItem);
    
    // Chunks under terrain (sample)
    for (int i = 0; i < 5; ++i) {
        std::string chunkName = "Chunk_" + std::to_string(i);
        auto chunkNode = std::make_shared<HierarchyNode>(chunkName);
        terrainNode->children.push_back(chunkNode);
        
        HTREEITEM terrainItem = m_nodeToItem[terrainNode.get()];
        addNodeToTree(chunkNode.get(), terrainItem);
    }
    
    // Expand root by default
    m_treeView->expandItem(rootItem);
}

void Win32SceneHierarchyPanel::addNodeToTree(HierarchyNode* node, HTREEITEM parentItem)
{
    if (!node || !m_treeView) {
        return;
    }
    
    HTREEITEM item = m_treeView->addChildItem(parentItem, node->name, node);
    m_itemToNode[item] = node;
    m_nodeToItem[node] = item;
    
    // Recursively add children
    for (auto& child : node->children) {
        addNodeToTree(child.get(), item);
    }
}

void Win32SceneHierarchyPanel::onTreeSelectionChanged()
{
    if (!m_treeView) {
        return;
    }
    
    HTREEITEM selectedItem = m_treeView->getSelectedItem();
    
    if (selectedItem && m_itemToNode.find(selectedItem) != m_itemToNode.end()) {
        m_selectedNode = m_itemToNode[selectedItem];
        
        // Mark node as selected
        if (m_selectedNode) {
            m_selectedNode->selected = true;
        }
        
        // Invoke callback
        if (m_selectionCallback) {
            m_selectionCallback(m_selectedNode);
        }
        
        LOG_INFO_C("Node selected: " + (m_selectedNode ? m_selectedNode->name : "nullptr"), "Win32SceneHierarchyPanel");
    } else {
        m_selectedNode = nullptr;
        
        if (m_selectionCallback) {
            m_selectionCallback(nullptr);
        }
    }
}

void Win32SceneHierarchyPanel::showContextMenu(int x, int y)
{
    if (!m_contextMenu) {
        return;
    }
    
    // Enable/disable menu items based on selection
    UINT enableFlags = m_selectedNode ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(m_contextMenu, CMD_RENAME, MF_BYCOMMAND | enableFlags);
    EnableMenuItem(m_contextMenu, CMD_DUPLICATE, MF_BYCOMMAND | enableFlags);
    EnableMenuItem(m_contextMenu, CMD_DELETE, MF_BYCOMMAND | enableFlags);
    
    // Show menu
    POINT pt = { x, y };
    ClientToScreen(m_hwnd, &pt);
    TrackPopupMenu(m_contextMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hwnd, nullptr);
}

bool Win32SceneHierarchyPanel::renameSelectedNode()
{
    if (!m_selectedNode) {
        return false;
    }
    
    // Show input dialog for new name
    // For now, just log
    LOG_INFO_C("Rename requested for: " + m_selectedNode->name, "Win32SceneHierarchyPanel");
    
    // TODO: Implement input dialog and update tree view
    return true;
}

bool Win32SceneHierarchyPanel::duplicateSelectedNode()
{
    if (!m_selectedNode) {
        return false;
    }
    
    LOG_INFO_C("Duplicate requested for: " + m_selectedNode->name, "Win32SceneHierarchyPanel");
    
    // TODO: Implement duplication logic
    return true;
}

bool Win32SceneHierarchyPanel::deleteSelectedNode()
{
    if (!m_selectedNode) {
        return false;
    }
    
    LOG_INFO_C("Delete requested for: " + m_selectedNode->name, "Win32SceneHierarchyPanel");
    
    // TODO: Implement deletion logic and refresh tree
    return true;
}

void Win32SceneHierarchyPanel::selectAll()
{
    // Select all nodes in tree
    // Note: Win32 TreeView doesn't support multi-selection by default
    LOG_INFO_C("Select all requested", "Win32SceneHierarchyPanel");
}

void Win32SceneHierarchyPanel::deselectAll()
{
    if (m_treeView) {
        m_treeView->setSelectedItem(nullptr);
        m_selectedNode = nullptr;
    }
}

HierarchyNode* Win32SceneHierarchyPanel::addNode(const std::string& name, HierarchyNode* parent)
{
    auto newNode = std::make_shared<HierarchyNode>(name);
    
    if (!parent) {
        parent = m_rootNode.get();
    }
    
    if (parent) {
        parent->children.push_back(newNode);
        
        // Add to tree view
        HTREEITEM parentItem = m_nodeToItem[parent];
        addNodeToTree(newNode.get(), parentItem);
        
        // Expand parent
        if (m_treeView) {
            m_treeView->expandItem(parentItem);
        }
    }
    
    return newNode.get();
}

LRESULT Win32SceneHierarchyPanel::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_NOTIFY: {
            NMHDR* nmhdr = (NMHDR*)lParam;
            if (nmhdr->code == TVN_SELCHANGED) {
                onTreeSelectionChanged();
            }
            break;
        }
        
        case WM_CONTEXTMENU: {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            showContextMenu(xPos, yPos);
            return 0;
        }
        
        case WM_COMMAND: {
            int cmdId = LOWORD(wParam);
            
            switch (cmdId) {
                case CMD_RENAME:
                    renameSelectedNode();
                    break;
                case CMD_DUPLICATE:
                    duplicateSelectedNode();
                    break;
                case CMD_DELETE:
                    deleteSelectedNode();
                    break;
                case CMD_ADD_OBJECT:
                    addNode("New Object", m_selectedNode);
                    break;
            }
            break;
        }
        
        case WM_SIZE: {
            // Resize tree view to fill panel
            if (m_treeView) {
                RECT clientRect;
                GetClientRect(m_hwnd, &clientRect);
                // Tree view is accessed via its window handle
                // We would need to expose the HWND from Win32TreeView
                // For now, assume it's handled
            }
            break;
        }
    }
    
    return Win32Panel::handleMessage(msg, wParam, lParam);
}

} // namespace fresh

#endif // _WIN32
