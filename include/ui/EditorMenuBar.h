#pragma once

#include <functional>
#include <string>

namespace fresh
{

class VoxelWorld;
class WorldEditor;

/**
 * @brief Main menu bar for the editor
 *
 * Provides standard menu options: File, Edit, Window, Build, Settings, Help
 */
class EditorMenuBar
{
public:
    // Menu action callbacks
    using MenuCallback = std::function<void()>;

    EditorMenuBar();
    ~EditorMenuBar();

    /**
     * @brief Initialize the menu bar
     * @param world Current world
     * @param editor World editor instance
     * @return true if successful
     */
    bool initialize(VoxelWorld* world, WorldEditor* editor);

    /**
     * @brief Render the menu bar
     */
    void render();

    /**
     * @brief Set callback for menu actions
     */
    void setNewWorldCallback(MenuCallback callback)
    {
        m_newWorldCallback = callback;
    }
    void setSaveWorldCallback(MenuCallback callback)
    {
        m_saveWorldCallback = callback;
    }
    void setSaveWorldAsCallback(MenuCallback callback)
    {
        m_saveWorldAsCallback = callback;
    }
    void setLoadWorldCallback(MenuCallback callback)
    {
        m_loadWorldCallback = callback;
    }
    void setExitCallback(MenuCallback callback)
    {
        m_exitCallback = callback;
    }
    void setUndoCallback(MenuCallback callback)
    {
        m_undoCallback = callback;
    }
    void setRedoCallback(MenuCallback callback)
    {
        m_redoCallback = callback;
    }
    void setCutCallback(MenuCallback callback)
    {
        m_cutCallback = callback;
    }
    void setCopyCallback(MenuCallback callback)
    {
        m_copyCallback = callback;
    }
    void setPasteCallback(MenuCallback callback)
    {
        m_pasteCallback = callback;
    }
    void setDeleteCallback(MenuCallback callback)
    {
        m_deleteCallback = callback;
    }
    void setSettingsCallback(MenuCallback callback)
    {
        m_settingsCallback = callback;
    }
    void setEngineConfigCallback(MenuCallback callback)
    {
        m_engineConfigCallback = callback;
    }
    void setImportAssetsCallback(MenuCallback callback)
    {
        m_importAssetsCallback = callback;
    }
    void setDialogueEditorCallback(MenuCallback callback)
    {
        m_dialogueEditorCallback = callback;
    }
    void setSelectAllCallback(MenuCallback callback)
    {
        m_selectAllCallback = callback;
    }
    void setDeselectAllCallback(MenuCallback callback)
    {
        m_deselectAllCallback = callback;
    }
    void setLoadLayoutCallback(std::function<void(const std::string&)> callback)
    {
        m_loadLayoutCallback = callback;
    }
    void setSaveLayoutCallback(MenuCallback callback)
    {
        m_saveLayoutCallback = callback;
    }
    void setResetLayoutCallback(MenuCallback callback)
    {
        m_resetLayoutCallback = callback;
    }
#ifdef _WIN32
    void setWindowsCustomizationCallback(MenuCallback callback)
    {
        m_windowsCustomizationCallback = callback;
    }
#endif

private:
    void renderFileMenu();
    void renderEditMenu();
    void renderWindowMenu();
    void renderToolsMenu();
    void renderBuildMenu();
    void renderSettingsMenu();
    void renderHelpMenu();

private:
    VoxelWorld* m_world;
    WorldEditor* m_editor;

    // Menu callbacks
    MenuCallback m_newWorldCallback;
    MenuCallback m_saveWorldCallback;
    MenuCallback m_saveWorldAsCallback;
    MenuCallback m_loadWorldCallback;
    MenuCallback m_exitCallback;
    MenuCallback m_undoCallback;
    MenuCallback m_redoCallback;
    MenuCallback m_cutCallback;
    MenuCallback m_copyCallback;
    MenuCallback m_pasteCallback;
    MenuCallback m_deleteCallback;
    MenuCallback m_settingsCallback;
    MenuCallback m_engineConfigCallback;
    MenuCallback m_importAssetsCallback;
    MenuCallback m_dialogueEditorCallback;
    MenuCallback m_selectAllCallback;
    MenuCallback m_deselectAllCallback;
    std::function<void(const std::string&)> m_loadLayoutCallback;
    MenuCallback m_saveLayoutCallback;
    MenuCallback m_resetLayoutCallback;
#ifdef _WIN32
    MenuCallback m_windowsCustomizationCallback;
#endif

    // Window visibility flags (shared with other systems)
    bool* m_showSceneHierarchy;
    bool* m_showInspector;
    bool* m_showContentBrowser;
    bool* m_showConsole;
    bool* m_showToolPalette;

public:
    // Setters for window visibility flags
    void setSceneHierarchyVisible(bool* visible)
    {
        m_showSceneHierarchy = visible;
    }
    void setInspectorVisible(bool* visible)
    {
        m_showInspector = visible;
    }
    void setContentBrowserVisible(bool* visible)
    {
        m_showContentBrowser = visible;
    }
    void setConsoleVisible(bool* visible)
    {
        m_showConsole = visible;
    }
    void setToolPaletteVisible(bool* visible)
    {
        m_showToolPalette = visible;
    }
};

} // namespace fresh
