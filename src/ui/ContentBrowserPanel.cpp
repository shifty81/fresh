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
{
    m_searchBuffer[0] = '\0';
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
        // TODO: Open file dialog for import
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
                // TODO: Open asset in appropriate editor
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
        // TODO: Open in appropriate editor
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Delete")) {
        // TODO: Delete asset with confirmation
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Rename")) {
        // TODO: Rename asset
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

} // namespace fresh
