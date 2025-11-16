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

namespace devtools
{
class DebugRenderer;
}

#ifdef _WIN32
class WindowsThemeManager;
class WindowsDialogManager;
class WindowsTaskbarManager;
class WindowsCustomizationPanel;
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
     * @return true if successful
     */
    bool initialize(WindowType* window, IRenderContext* renderContext, VoxelWorld* world = nullptr,
                    WorldEditor* worldEditor = nullptr, InputManagerType* inputManager = nullptr);

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
    std::unique_ptr<devtools::DebugRenderer> m_debugRenderer;
    std::unique_ptr<WorldSerializer> m_worldSerializer;

#ifdef _WIN32
    // Windows-native integration managers
    std::unique_ptr<WindowsThemeManager> m_windowsThemeManager;
    std::unique_ptr<WindowsDialogManager> m_windowsDialogManager;
    std::unique_ptr<WindowsTaskbarManager> m_windowsTaskbarManager;
    std::unique_ptr<WindowsCustomizationPanel> m_windowsCustomizationPanel;
#endif

    // References
    WindowType* m_window;
    IRenderContext* m_renderContext;
    VoxelWorld* m_world;
    WorldEditor* m_worldEditor;

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
