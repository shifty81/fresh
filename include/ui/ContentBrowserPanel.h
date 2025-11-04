#pragma once

#include <string>
#include <vector>

namespace fresh {

/**
 * @brief Asset information structure
 */
struct AssetInfo {
    std::string name;
    std::string path;
    std::string type;  // "texture", "model", "sound", "script", etc.
    size_t size;
    
    AssetInfo(const std::string& n = "", const std::string& p = "", 
              const std::string& t = "", size_t s = 0)
        : name(n), path(p), type(t), size(s) {}
};

/**
 * @brief Content Browser / Asset Manager Panel
 * 
 * Manages all project assets (textures, models, sounds, scripts),
 * allowing for filtering, searching, importing, and previewing.
 */
class ContentBrowserPanel {
public:
    ContentBrowserPanel();
    ~ContentBrowserPanel();

    /**
     * @brief Initialize the panel
     * @param assetsPath Root path for assets
     * @return true if successful
     */
    bool initialize(const std::string& assetsPath = "assets");

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
     * @brief Refresh asset list
     */
    void refresh();

    /**
     * @brief Delete the currently selected asset
     * @return true if successful
     */
    bool deleteSelectedAsset();

    /**
     * @brief Rename the currently selected asset
     * @param newName New name for the asset
     * @return true if successful
     */
    bool renameSelectedAsset(const std::string& newName);

    /**
     * @brief Import a file into the current directory
     * @param sourcePath Path to the file to import
     * @return true if successful
     */
    bool importAsset(const std::string& sourcePath);

private:
    void scanAssets(const std::string& path);
    void renderAssetGrid();
    void renderAssetDetails(const AssetInfo& asset);
    const char* getAssetIcon(const std::string& type);
    bool showConfirmDialog(const std::string& title, const std::string& message);
    bool showRenameDialog(std::string& outName);

private:
    bool m_visible;
    std::string m_assetsPath;
    std::string m_currentPath;
    std::vector<AssetInfo> m_assets;
    AssetInfo* m_selectedAsset;
    char m_searchBuffer[256];
    
    // Dialog state
    bool m_showDeleteDialog;
    bool m_showRenameDialog;
    char m_renameBuffer[256];
};

} // namespace fresh
