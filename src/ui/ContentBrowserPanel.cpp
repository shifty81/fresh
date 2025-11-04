#include "ui/ContentBrowserPanel.h"
#include "core/Logger.h"
#include <imgui.h>
#include <filesystem>

namespace fresh {
namespace fs = std::filesystem;

ContentBrowserPanel::ContentBrowserPanel()
    : m_visible(true)
    , m_assetsPath("assets")
    , m_currentPath("assets")
    , m_selectedAsset(nullptr)
    , m_showDeleteDialog(false)
    , m_showRenameDialog(false)
{
    m_searchBuffer[0] = '\0';
    m_renameBuffer[0] = '\0';
}

ContentBrowserPanel::~ContentBrowserPanel() {
}

bool ContentBrowserPanel::initialize(const std::string& assetsPath) {
    m_assetsPath = assetsPath;
    m_currentPath = assetsPath;
    
    // Create assets directory if it doesn't exist
    if (!fs::exists(assetsPath)) {
        fs::create_directories(assetsPath);
    }
    
    refresh();
    
    LOG_INFO_C("Content Browser Panel initialized", "ContentBrowserPanel");
    return true;
}

void ContentBrowserPanel::render() {
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Content Browser", &m_visible);

    // Toolbar
    if (ImGui::Button("< Back")) {
        if (m_currentPath != m_assetsPath) {
            m_currentPath = fs::path(m_currentPath).parent_path().string();
            refresh();
        }
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Refresh")) {
        refresh();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Import...")) {
        // Simple import dialog - in a full implementation, this would use a file dialog
        LOG_INFO_C("Import dialog would open here", "ContentBrowserPanel");
    }
    ImGui::SameLine();
    
    // Search box
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##search", "Search assets...", m_searchBuffer, sizeof(m_searchBuffer));

    ImGui::Separator();

    // Current path breadcrumb
    ImGui::Text("Path: %s", m_currentPath.c_str());
    ImGui::Separator();

    // Asset grid
    renderAssetGrid();

    // Asset details (bottom panel)
    if (m_selectedAsset) {
        ImGui::Separator();
        renderAssetDetails(*m_selectedAsset);
    }

    ImGui::End();
}

void ContentBrowserPanel::renderAssetGrid() {
    float cellSize = 80.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = std::max(1, (int)(panelWidth / cellSize));

    ImGui::Columns(columnCount, nullptr, false);

    std::string searchStr(m_searchBuffer);
    bool hasSearch = !searchStr.empty();

    for (auto& asset : m_assets) {
        // Filter by search
        if (hasSearch && asset.name.find(searchStr) == std::string::npos) {
            continue;
        }

        ImGui::PushID(asset.name.c_str());

        // Asset button
        bool isSelected = (m_selectedAsset == &asset);
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
        }

        // Icon and name
        const char* icon = getAssetIcon(asset.type);
        if (ImGui::Button(icon, ImVec2(cellSize - 10, cellSize - 30))) {
            m_selectedAsset = &asset;
        }

        if (isSelected) {
            ImGui::PopStyleColor();
        }

        // Handle double-click to open
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            if (asset.type == "folder") {
                m_currentPath = asset.path;
                refresh();
            } else {
                LOG_INFO_C("Opening asset: " + asset.name, "ContentBrowserPanel");
            }
        }

        // Asset name
        ImGui::TextWrapped("%s", asset.name.c_str());

        ImGui::PopID();
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

void ContentBrowserPanel::renderAssetDetails(const AssetInfo& asset) {
    ImGui::Text("Selected: %s", asset.name.c_str());
    ImGui::Text("Type: %s", asset.type.c_str());
    ImGui::Text("Path: %s", asset.path.c_str());
    ImGui::Text("Size: %zu bytes", asset.size);
    
    ImGui::Spacing();
    
    if (ImGui::Button("Open")) {
        LOG_INFO_C("Opening asset: " + asset.name, "ContentBrowserPanel");
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Delete")) {
        m_showDeleteDialog = true;
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Rename")) {
        m_showRenameDialog = true;
        // Pre-fill rename buffer with current name
        strncpy(m_renameBuffer, asset.name.c_str(), sizeof(m_renameBuffer) - 1);
        m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
    }
    
    // Delete confirmation dialog
    if (m_showDeleteDialog) {
        ImGui::OpenPopup("Delete Asset?");
        if (ImGui::BeginPopupModal("Delete Asset?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete '%s'?", asset.name.c_str());
            ImGui::Text("This action cannot be undone.");
            ImGui::Separator();
            
            if (ImGui::Button("Delete", ImVec2(120, 0))) {
                if (deleteSelectedAsset()) {
                    LOG_INFO_C("Deleted asset: " + asset.name, "ContentBrowserPanel");
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
    
    // Rename dialog
    if (m_showRenameDialog) {
        ImGui::OpenPopup("Rename Asset");
        if (ImGui::BeginPopupModal("Rename Asset", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Enter new name for '%s':", asset.name.c_str());
            ImGui::InputText("##rename", m_renameBuffer, sizeof(m_renameBuffer));
            ImGui::Separator();
            
            if (ImGui::Button("Rename", ImVec2(120, 0))) {
                if (m_renameBuffer[0] != '\0') {
                    if (renameSelectedAsset(std::string(m_renameBuffer))) {
                        LOG_INFO_C("Renamed asset to: " + std::string(m_renameBuffer), "ContentBrowserPanel");
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
}

const char* ContentBrowserPanel::getAssetIcon(const std::string& type) {
    if (type == "folder") return "📁";
    if (type == "texture") return "🖼️";
    if (type == "model") return "📦";
    if (type == "sound") return "🔊";
    if (type == "script") return "📄";
    if (type == "scene") return "🎬";
    return "📋";  // Default icon
}

void ContentBrowserPanel::scanAssets(const std::string& path) {
    m_assets.clear();

    if (!fs::exists(path)) {
        return;
    }

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            AssetInfo asset;
            asset.name = entry.path().filename().string();
            asset.path = entry.path().string();

            if (entry.is_directory()) {
                asset.type = "folder";
                asset.size = 0;
            } else {
                asset.size = entry.file_size();
                
                // Determine type from extension
                std::string ext = entry.path().extension().string();
                if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
                    asset.type = "texture";
                } else if (ext == ".obj" || ext == ".fbx" || ext == ".gltf") {
                    asset.type = "model";
                } else if (ext == ".wav" || ext == ".mp3" || ext == ".ogg") {
                    asset.type = "sound";
                } else if (ext == ".lua" || ext == ".cpp" || ext == ".h") {
                    asset.type = "script";
                } else if (ext == ".world" || ext == ".scene") {
                    asset.type = "scene";
                } else {
                    asset.type = "file";
                }
            }

            m_assets.push_back(asset);
        }
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR_C(std::string("Failed to scan assets: ") + e.what(), "ContentBrowserPanel");
    }
}

void ContentBrowserPanel::refresh() {
    scanAssets(m_currentPath);
    m_selectedAsset = nullptr;
}

bool ContentBrowserPanel::deleteSelectedAsset() {
    if (!m_selectedAsset) {
        return false;
    }

    try {
        if (fs::exists(m_selectedAsset->path)) {
            if (m_selectedAsset->type == "folder") {
                fs::remove_all(m_selectedAsset->path);
            } else {
                fs::remove(m_selectedAsset->path);
            }
            refresh();
            return true;
        }
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR_C(std::string("Failed to delete asset: ") + e.what(), "ContentBrowserPanel");
    }
    return false;
}

bool ContentBrowserPanel::renameSelectedAsset(const std::string& newName) {
    if (!m_selectedAsset || newName.empty()) {
        return false;
    }

    try {
        fs::path oldPath(m_selectedAsset->path);
        fs::path newPath = oldPath.parent_path() / newName;
        
        // Check if target already exists
        if (fs::exists(newPath)) {
            LOG_ERROR_C("Asset with name '" + newName + "' already exists", "ContentBrowserPanel");
            return false;
        }
        
        fs::rename(oldPath, newPath);
        refresh();
        return true;
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR_C(std::string("Failed to rename asset: ") + e.what(), "ContentBrowserPanel");
    }
    return false;
}

bool ContentBrowserPanel::importAsset(const std::string& sourcePath) {
    if (sourcePath.empty() || !fs::exists(sourcePath)) {
        LOG_ERROR_C("Source file does not exist: " + sourcePath, "ContentBrowserPanel");
        return false;
    }

    try {
        fs::path source(sourcePath);
        fs::path destination = fs::path(m_currentPath) / source.filename();
        
        // Check if destination already exists
        if (fs::exists(destination)) {
            LOG_ERROR_C("Asset already exists at destination", "ContentBrowserPanel");
            return false;
        }
        
        fs::copy(source, destination, fs::copy_options::recursive);
        refresh();
        LOG_INFO_C("Imported asset: " + source.filename().string(), "ContentBrowserPanel");
        return true;
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR_C(std::string("Failed to import asset: ") + e.what(), "ContentBrowserPanel");
    }
    return false;
}

} // namespace fresh
