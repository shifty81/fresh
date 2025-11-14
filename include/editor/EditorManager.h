#pragma once

#include <memory>

namespace fresh
{

class Window;
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
     * @return true if successful
     */
    bool initialize(Window* window, IRenderContext* renderContext, VoxelWorld* world = nullptr,
                    WorldEditor* worldEditor = nullptr, class InputManager* inputManager = nullptr);

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
     * @brief Get the toolbar
     * @return Toolbar pointer
     */
    EditorToolbar* getToolbar() const
    {
        return m_toolbar.get();
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

    // References
    Window* m_window;
    IRenderContext* m_renderContext;
    VoxelWorld* m_world;
    WorldEditor* m_worldEditor;

    // Panel visibility flags
    bool m_showSceneHierarchy;
    bool m_showInspector;
    bool m_showContentBrowser;
    bool m_showConsole;
    bool m_showToolPalette;
};

} // namespace fresh
