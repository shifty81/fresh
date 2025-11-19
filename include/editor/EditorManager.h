#pragma once

#include <memory>
#include <string>

namespace fresh
{

#ifdef _WIN32
class Win32Window;
class Win32InputManager;
using WindowType = Win32Window;
using InputManagerType = Win32InputManager;
#else
class Window;
class InputManager;
using WindowType = Window;
using InputManagerType = InputManager;
#endif

class IRenderContext;
class VoxelWorld;
class WorldEditor;
class ImGuiContext;
class SceneHierarchyPanel;
class InspectorPanel;
class EditorMenuBar;
class EditorToolbar;
class ContentBrowserPanel;
class ConsolePanel;
class VoxelToolPalette;
class MainMenuPanel;
class SettingsPanel;
class EngineConfigPanel;
class HotbarPanel;
class SelectionManager;
class SelectionRenderer;
class WorldSerializer;
class TransformGizmo;
class LayoutManager;
class EditorSettingsDialog;

namespace ecs
{
class EntityManager;
}

namespace devtools
{
class DebugRenderer;
}

#ifdef _WIN32
class WindowsThemeManager;
class WindowsDialogManager;
class WindowsTaskbarManager;
class WindowsCustomizationPanel;
class Win32InspectorPanel;
class Win32SceneHierarchyPanel;
class Win32ContentBrowserPanel;
class Win32ConsolePanel;
class Win32HUD;
class Win32SettingsDialog;
#endif

/**
 * @brief Comprehensive Editor Manager
 *
 * Manages all editor UI panels and coordinates the editor interface.
 * This is the main entry point for the integrated game engine editor.
 */
class EditorManager
{
public:
    EditorManager();
    ~EditorManager();

    // Prevent copying
    EditorManager(const EditorManager&) = delete;
    EditorManager& operator=(const EditorManager&) = delete;

    /**
     * @brief Initialize the editor manager
     * @param window Main window
     * @param renderContext Render context for ImGui
     * @param world Voxel world (optional, can be nullptr - world-dependent panels will be initialized later)
     * @param worldEditor World editor instance (optional, can be nullptr - world-dependent panels will be initialized later)
     * @param inputManager Input manager for settings
     * @param entityManager Entity manager for component editing (optional)
     * @return true if successful
     */
    bool initialize(WindowType* window, IRenderContext* renderContext, VoxelWorld* world = nullptr,
                    WorldEditor* worldEditor = nullptr, InputManagerType* inputManager = nullptr,
                    ecs::EntityManager* entityManager = nullptr);

    /**
     * @brief Begin a new editor frame
     * Call this at the start of each frame
     */
    void beginFrame();

    /**
     * @brief Render all editor UI
     */
    void render();

    /**
     * @brief End the editor frame and submit to GPU
     */
    void endFrame();

    /**
     * @brief Shutdown the editor and cleanup
     */
    void shutdown();

    /**
     * @brief Update world and world editor references
     * @param world New voxel world reference
     * @param worldEditor New world editor reference
     * @return true if successful
     */
    bool updateWorld(VoxelWorld* world, WorldEditor* worldEditor);

    /**
     * @brief Set player reference for camera-based operations
     * @param player Player instance
     */
    void setPlayer(class Player* player)
    {
        m_player = player;
    }

    /**
     * @brief Check if editor is initialized
     * @return true if initialized
     */
    bool isInitialized() const
    {
        return m_initialized;
    }

    /**
     * @brief Toggle editor visibility
     */
    void toggle()
    {
        m_visible = !m_visible;
    }

    /**
     * @brief Set editor visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    /**
     * @brief Check if editor is visible
     * @return true if visible
     */
    bool isVisible() const
    {
        return m_visible;
    }

    /**
     * @brief Check if ImGui wants to capture mouse
     * @return true if ImGui is using the mouse
     */
    bool wantCaptureMouse() const;

    /**
     * @brief Check if ImGui wants to capture keyboard
     * @return true if ImGui is using the keyboard
     */
    bool wantCaptureKeyboard() const;

    /**
     * @brief Get the ImGui context
     * @return ImGui context pointer
     */
    ImGuiContext* getImGuiContext() const
    {
        return m_imguiContext.get();
    }

    /**
     * @brief Get the main menu panel
     * @return Main menu panel pointer
     */
    MainMenuPanel* getMainMenuPanel() const
    {
        return m_mainMenuPanel.get();
    }

    /**
     * @brief Get the settings panel
     * @return Settings panel pointer
     */
    SettingsPanel* getSettingsPanel() const
    {
        return m_settingsPanel.get();
    }

    /**
     * @brief Get the scene hierarchy panel
     * @return Scene hierarchy panel pointer
     */
    SceneHierarchyPanel* getSceneHierarchy() const
    {
        return m_sceneHierarchy.get();
    }

    /**
     * @brief Get the engine configuration panel
     * @return Engine configuration panel pointer
     */
    EngineConfigPanel* getEngineConfigPanel() const
    {
        return m_engineConfigPanel.get();
    }

    /**
     * @brief Get the toolbar
     * @return Toolbar pointer
     */
    EditorToolbar* getToolbar() const
    {
        return m_toolbar.get();
    }

    /**
     * @brief Get the hotbar panel
     * @return Hotbar panel pointer
     */
    HotbarPanel* getHotbar() const
    {
        return m_hotbar.get();
    }

    /**
     * @brief Get the voxel tool palette
     * @return Voxel tool palette pointer
     */
    VoxelToolPalette* getVoxelToolPalette() const
    {
        return m_voxelTools.get();
    }

    /**
     * @brief Get the selection manager
     * @return Selection manager pointer
     */
    SelectionManager* getSelectionManager() const
    {
        return m_selectionManager.get();
    }

    /**
     * @brief Get the selection renderer
     * @return Selection renderer pointer
     */
    SelectionRenderer* getSelectionRenderer() const
    {
        return m_selectionRenderer.get();
    }

    /**
     * @brief Get the transform gizmo
     * @return Transform gizmo pointer
     */
    TransformGizmo* getTransformGizmo() const
    {
        return m_transformGizmo.get();
    }

#ifdef _WIN32
    /**
     * @brief Get the Windows customization panel
     * @return Windows customization panel pointer (nullptr if not on Windows)
     */
    WindowsCustomizationPanel* getWindowsCustomizationPanel() const
    {
        return m_windowsCustomizationPanel.get();
    }
#endif

    // ========== Editor Operations (for native menu integration) ==========
    
    /**
     * @brief Perform undo operation
     */
    void undo();
    
    /**
     * @brief Perform redo operation
     */
    void redo();
    
    /**
     * @brief Cut selection to clipboard
     */
    void cut();
    
    /**
     * @brief Copy selection to clipboard
     */
    void copy();
    
    /**
     * @brief Paste from clipboard
     */
    void paste();
    
    /**
     * @brief Delete selected items
     */
    void deleteSelection();
    
    /**
     * @brief Select all items
     */
    void selectAll();
    
    /**
     * @brief Deselect all items
     */
    void deselectAll();
    
    /**
     * @brief Save current world
     */
    void saveWorld();
    
    /**
     * @brief Show save world dialog
     */
    void saveWorldAs();
    
    /**
     * @brief Show load world dialog
     */
    void loadWorld();
    
    /**
     * @brief Show new world dialog
     */
    void newWorld();
    
    /**
     * @brief Toggle scene hierarchy panel visibility
     */
    void toggleSceneHierarchy();
    
    /**
     * @brief Toggle inspector panel visibility
     */
    void toggleInspector();
    
    /**
     * @brief Toggle content browser panel visibility
     */
    void toggleContentBrowser();
    
    /**
     * @brief Toggle console panel visibility
     */
    void toggleConsole();
    
    /**
     * @brief Toggle tool palette panel visibility
     */
    void toggleToolPalette();
    
    /**
     * @brief Show settings panel
     */
    void showSettings();
    
    /**
     * @brief Show engine configuration panel
     */
    void showEngineConfig();
    
    /**
     * @brief Show editor settings dialog
     */
    void showEditorSettings();
    
    /**
     * @brief Show import assets dialog
     */
    void showImportAssets();
    
    /**
     * @brief Launch dialogue editor
     * Opens the dialogue editor tool for creating and editing game dialogues
     */
    void launchDialogueEditor();
    
    /**
     * @brief Load a workspace layout
     * @param name Layout name ("Default", "Minimal", "Debugging", or custom name)
     */
    void loadLayout(const std::string& name);
    
    /**
     * @brief Save the current workspace layout
     * @param name Layout name (if empty, saves to current layout)
     */
    void saveCurrentLayout(const std::string& name = "");
    
    /**
     * @brief Reset to default layout
     */
    void resetLayout();
    
    /**
     * @brief Get the layout manager
     * @return Layout manager pointer
     */
    LayoutManager* getLayoutManager() const
    {
        return m_layoutManager.get();
    }
    
    /**
     * @brief Get the editor settings dialog
     * @return Editor settings dialog pointer
     */
    EditorSettingsDialog* getEditorSettingsDialog() const
    {
        return m_editorSettingsDialog.get();
    }

private:
    void setupDockspace();

private:
    bool m_initialized;
    bool m_visible;

    // ImGui context
    std::unique_ptr<ImGuiContext> m_imguiContext;

    // UI Panels
    std::unique_ptr<SceneHierarchyPanel> m_sceneHierarchy;
    std::unique_ptr<InspectorPanel> m_inspector;
    std::unique_ptr<EditorMenuBar> m_menuBar;
    std::unique_ptr<EditorToolbar> m_toolbar;
    std::unique_ptr<ContentBrowserPanel> m_contentBrowser;
    std::unique_ptr<ConsolePanel> m_console;
    std::unique_ptr<VoxelToolPalette> m_voxelTools;
    std::unique_ptr<MainMenuPanel> m_mainMenuPanel;
    std::unique_ptr<SettingsPanel> m_settingsPanel;
    std::unique_ptr<EngineConfigPanel> m_engineConfigPanel;
    std::unique_ptr<HotbarPanel> m_hotbar;
    std::unique_ptr<SelectionManager> m_selectionManager;
    std::unique_ptr<SelectionRenderer> m_selectionRenderer;
    std::unique_ptr<TransformGizmo> m_transformGizmo;
    std::unique_ptr<devtools::DebugRenderer> m_debugRenderer;
    std::unique_ptr<WorldSerializer> m_worldSerializer;
    std::unique_ptr<LayoutManager> m_layoutManager;
    std::unique_ptr<EditorSettingsDialog> m_editorSettingsDialog;

#ifdef _WIN32
    // Windows-native integration managers
    std::unique_ptr<WindowsThemeManager> m_windowsThemeManager;
    std::unique_ptr<WindowsDialogManager> m_windowsDialogManager;
    std::unique_ptr<WindowsTaskbarManager> m_windowsTaskbarManager;
    std::unique_ptr<WindowsCustomizationPanel> m_windowsCustomizationPanel;
    
    // Native Win32 UI panels (used when FRESH_WIN32_UI is defined)
    std::unique_ptr<Win32InspectorPanel> m_nativeInspector;
    std::unique_ptr<Win32SceneHierarchyPanel> m_nativeSceneHierarchy;
    std::unique_ptr<Win32ContentBrowserPanel> m_nativeContentBrowser;
    std::unique_ptr<Win32ConsolePanel> m_nativeConsole;
    std::unique_ptr<Win32HUD> m_nativeHUD;
    std::unique_ptr<Win32SettingsDialog> m_nativeSettingsDialog;
#endif

    // References
    WindowType* m_window;
    IRenderContext* m_renderContext;
    VoxelWorld* m_world;
    WorldEditor* m_worldEditor;
    ecs::EntityManager* m_entityManager;
    class Player* m_player;

    // Panel visibility flags
    bool m_showSceneHierarchy;
    bool m_showInspector;
    bool m_showContentBrowser;
    bool m_showConsole;
    bool m_showToolPalette;
    
    // World file path
    std::string m_currentWorldPath;
};

} // namespace fresh
