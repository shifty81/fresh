#include "editor/EditorManager.h"

#include "core/Logger.h"
#include "core/Window.h"
#include "editor/WorldEditor.h"
#include "editor/SelectionManager.h"
#include "renderer/RenderContext.h"
#include "ui/ConsolePanel.h"
#include "ui/ContentBrowserPanel.h"
#include "ui/EditorMenuBar.h"
#include "ui/EditorToolbar.h"
#include "ui/HotbarPanel.h"
#include "ui/ImGuiContext.h"
#include "ui/InspectorPanel.h"
#include "ui/MainMenuPanel.h"
#include "ui/SceneHierarchyPanel.h"
#include "ui/SettingsPanel.h"
#include "ui/VoxelToolPalette.h"
#include "voxel/VoxelWorld.h"

#include <glm/glm.hpp>

#ifdef _WIN32
    #include "ui/WindowsThemeManager.h"
    #include "ui/WindowsDialogManager.h"
    #include "ui/WindowsTaskbarManager.h"
    #include "ui/WindowsCustomizationPanel.h"
#endif

#ifdef FRESH_IMGUI_AVAILABLE
    #include <imgui.h>
#endif

namespace fresh
{

EditorManager::EditorManager()
    : m_initialized(false),
      m_visible(false),
      m_window(nullptr),
      m_renderContext(nullptr),
      m_world(nullptr),
      m_worldEditor(nullptr),
      m_showSceneHierarchy(true),
      m_showInspector(true),
      m_showContentBrowser(true),
      m_showConsole(true),
      m_showToolPalette(true)
{
}

EditorManager::~EditorManager()
{
    shutdown();
}

bool EditorManager::initialize(Window* window, IRenderContext* renderContext, VoxelWorld* world,
                               WorldEditor* worldEditor, InputManager* inputManager)
{
    if (m_initialized) {
        LOG_WARNING_C("EditorManager already initialized", "EditorManager");
        return true;
    }

    if (!window || !renderContext) {
        LOG_ERROR_C("Invalid parameters for EditorManager", "EditorManager");
        return false;
    }

    m_window = window;
    m_renderContext = renderContext;
    m_world = world;
    m_worldEditor = worldEditor;

#ifdef FRESH_IMGUI_AVAILABLE
    // Initialize ImGui context
    m_imguiContext = std::make_unique<ImGuiContext>();
    if (!m_imguiContext->initialize(window, renderContext)) {
        LOG_ERROR_C("Failed to initialize ImGui context", "EditorManager");
        return false;
    }
    LOG_INFO_C("ImGui context initialized", "EditorManager");

    // Initialize UI panels that require world/worldEditor
    // Only initialize these if world and worldEditor are available
    if (world && worldEditor) {
        m_sceneHierarchy = std::make_unique<SceneHierarchyPanel>();
        if (!m_sceneHierarchy->initialize(world)) {
            LOG_ERROR_C("Failed to initialize Scene Hierarchy panel", "EditorManager");
            return false;
        }

        m_inspector = std::make_unique<InspectorPanel>();
        if (!m_inspector->initialize()) {
            LOG_ERROR_C("Failed to initialize Inspector panel", "EditorManager");
            return false;
        }

        m_menuBar = std::make_unique<EditorMenuBar>();
        if (!m_menuBar->initialize(world, worldEditor)) {
            LOG_ERROR_C("Failed to initialize Menu Bar", "EditorManager");
            return false;
        }

        // Link menu bar to panel visibility flags
        m_menuBar->setSceneHierarchyVisible(&m_showSceneHierarchy);
        m_menuBar->setInspectorVisible(&m_showInspector);
        m_menuBar->setContentBrowserVisible(&m_showContentBrowser);
        m_menuBar->setConsoleVisible(&m_showConsole);
        m_menuBar->setToolPaletteVisible(&m_showToolPalette);

        // Set settings callback to open settings panel
        m_menuBar->setSettingsCallback([this]() {
            if (m_settingsPanel) {
                m_settingsPanel->setVisible(true);
            }
        });

        // Set import assets callback to show import dialog in content browser
        m_menuBar->setImportAssetsCallback([this]() {
            if (m_contentBrowser) {
                m_contentBrowser->showImportDialog();
            }
        });

        // Set select all/deselect all callbacks for scene hierarchy
        m_menuBar->setSelectAllCallback([this]() {
            if (m_sceneHierarchy) {
                m_sceneHierarchy->selectAll();
            }
        });

        m_menuBar->setDeselectAllCallback([this]() {
            if (m_sceneHierarchy) {
                m_sceneHierarchy->deselectAll();
            }
        });

#ifdef _WIN32
        // Set Windows customization callback
        m_menuBar->setWindowsCustomizationCallback([this]() {
            if (m_windowsCustomizationPanel) {
                m_windowsCustomizationPanel->setVisible(true);
            }
        });
#endif

        // Set undo/redo callbacks for terraforming
        m_menuBar->setUndoCallback([this, worldEditor]() {
            if (worldEditor && worldEditor->getTerraformingSystem()) {
                if (worldEditor->getTerraformingSystem()->undo()) {
                    LOG_INFO_C("Undo performed", "EditorManager");
                } else {
                    LOG_INFO_C("Nothing to undo", "EditorManager");
                }
            }
        });

        m_menuBar->setRedoCallback([this, worldEditor]() {
            if (worldEditor && worldEditor->getTerraformingSystem()) {
                if (worldEditor->getTerraformingSystem()->redo()) {
                    LOG_INFO_C("Redo performed", "EditorManager");
                } else {
                    LOG_INFO_C("Nothing to redo", "EditorManager");
                }
            }
        });

        // Set Cut/Copy/Paste/Delete callbacks for selection manager
        m_menuBar->setCutCallback([this, world]() {
            if (m_selectionManager && m_selectionManager->hasSelection()) {
                m_selectionManager->cutToClipboard(world);
                LOG_INFO_C("Cut selection to clipboard", "EditorManager");
            } else {
                LOG_INFO_C("No selection to cut", "EditorManager");
            }
        });

        m_menuBar->setCopyCallback([this, world]() {
            if (m_selectionManager && m_selectionManager->hasSelection()) {
                m_selectionManager->copyToClipboard(world);
                LOG_INFO_C("Copied selection to clipboard", "EditorManager");
            } else {
                LOG_INFO_C("No selection to copy", "EditorManager");
            }
        });

        m_menuBar->setPasteCallback([this, world]() {
            if (m_selectionManager && m_selectionManager->hasClipboardData()) {
                // TODO: Get paste position from camera/cursor
                // For now, paste at origin as placeholder
                m_selectionManager->pasteFromClipboard(glm::ivec3(0, 64, 0), world);
                LOG_INFO_C("Pasted clipboard content", "EditorManager");
            } else {
                LOG_INFO_C("Clipboard is empty", "EditorManager");
            }
        });

        m_menuBar->setDeleteCallback([this, world]() {
            if (m_selectionManager && m_selectionManager->hasSelection()) {
                m_selectionManager->deleteSelected(world);
                LOG_INFO_C("Deleted selection", "EditorManager");
            } else {
                LOG_INFO_C("No selection to delete", "EditorManager");
            }
        });

        m_toolbar = std::make_unique<EditorToolbar>();
        if (!m_toolbar->initialize()) {
            LOG_ERROR_C("Failed to initialize Toolbar", "EditorManager");
            return false;
        }

        m_contentBrowser = std::make_unique<ContentBrowserPanel>();
        if (!m_contentBrowser->initialize("assets")) {
            LOG_ERROR_C("Failed to initialize Content Browser panel", "EditorManager");
            return false;
        }

        m_console = std::make_unique<ConsolePanel>();
        if (!m_console->initialize()) {
            LOG_ERROR_C("Failed to initialize Console panel", "EditorManager");
            return false;
        }

        m_voxelTools = std::make_unique<VoxelToolPalette>();
        if (!m_voxelTools->initialize(worldEditor->getTerraformingSystem())) {
            LOG_ERROR_C("Failed to initialize Voxel Tool Palette", "EditorManager");
            return false;
        }

        // Initialize selection manager
        m_selectionManager = std::make_unique<SelectionManager>();
        LOG_INFO_C("Selection Manager initialized", "EditorManager");
    } else {
        LOG_INFO_C("World and WorldEditor not provided, deferring initialization of world-dependent panels", "EditorManager");
    }

    // Initialize main menu panel
    m_mainMenuPanel = std::make_unique<MainMenuPanel>();
    if (!m_mainMenuPanel->initialize()) {
        LOG_ERROR_C("Failed to initialize Main Menu Panel", "EditorManager");
        return false;
    }

    // Initialize settings panel
    m_settingsPanel = std::make_unique<SettingsPanel>();
    if (!m_settingsPanel->initialize(window, inputManager)) {
        LOG_ERROR_C("Failed to initialize Settings Panel", "EditorManager");
        return false;
    }

    // Initialize hotbar panel (for play mode)
    m_hotbar = std::make_unique<HotbarPanel>();
    if (!m_hotbar->initialize()) {
        LOG_ERROR_C("Failed to initialize Hotbar Panel", "EditorManager");
        return false;
    }
    m_hotbar->setVisible(false); // Hidden by default (shown only in play mode)

#ifdef _WIN32
    // Initialize Windows-native integration features
    LOG_INFO_C("Initializing Windows-native integration features", "EditorManager");
    
    // Initialize Windows Theme Manager
    m_windowsThemeManager = std::make_unique<WindowsThemeManager>();
    if (m_windowsThemeManager->initialize()) {
        // Force dark theme for better readability regardless of Windows settings
        // This ensures consistent dark background with white text
        m_windowsThemeManager->setTheme(WindowsTheme::Dark);
        LOG_INFO_C("Windows Theme Manager initialized with Dark theme", "EditorManager");
    } else {
        LOG_WARNING_C("Failed to initialize Windows Theme Manager", "EditorManager");
    }
    
    // Initialize Windows Dialog Manager
    m_windowsDialogManager = std::make_unique<WindowsDialogManager>();
    void* nativeHandle = window->getNativeWindowHandle();
    if (nativeHandle && m_windowsDialogManager->initialize(nativeHandle)) {
        LOG_INFO_C("Windows Dialog Manager initialized", "EditorManager");
    } else {
        LOG_WARNING_C("Failed to initialize Windows Dialog Manager", "EditorManager");
    }
    
    // Initialize Windows Taskbar Manager
    m_windowsTaskbarManager = std::make_unique<WindowsTaskbarManager>();
    if (nativeHandle && m_windowsTaskbarManager->initialize(nativeHandle)) {
        LOG_INFO_C("Windows Taskbar Manager initialized", "EditorManager");
    } else {
        LOG_WARNING_C("Failed to initialize Windows Taskbar Manager", "EditorManager");
    }
    
    // Initialize Windows Customization Panel
    m_windowsCustomizationPanel = std::make_unique<WindowsCustomizationPanel>();
    if (m_windowsCustomizationPanel->initialize(m_windowsThemeManager.get(),
                                                 m_windowsDialogManager.get(),
                                                 m_windowsTaskbarManager.get())) {
        LOG_INFO_C("Windows Customization Panel initialized", "EditorManager");
    } else {
        LOG_WARNING_C("Failed to initialize Windows Customization Panel", "EditorManager");
    }
#endif

    // Connect scene hierarchy to inspector
    // When a node is selected in the hierarchy, show it in the inspector
    // This would require adding a callback system, but for now we'll handle it in render()

    m_initialized = true;
    LOG_INFO_C("EditorManager initialized successfully", "EditorManager");
    LOG_INFO_C("All editor UI panels initialized", "EditorManager");
    return true;
#else
    LOG_WARNING_C("ImGui not available, editor will run in console mode", "EditorManager");
    m_initialized = true;
    return true;
#endif
}

void EditorManager::beginFrame()
{
    if (!m_initialized) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Always call newFrame() when initialized, regardless of visibility
    // This is required by ImGui - NewFrame() and Render() must always be paired
    if (m_imguiContext) {
        m_imguiContext->newFrame();
    }
#endif
}

void EditorManager::render()
{
    if (!m_initialized || !m_visible) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Setup dockspace
    setupDockspace();

    // Render menu bar (always visible)
    if (m_menuBar) {
        m_menuBar->render();
    }

    // Render toolbar (always visible when editor is visible)
    if (m_toolbar) {
        m_toolbar->render();
    }

    // Render panels based on visibility flags
    if (m_showSceneHierarchy && m_sceneHierarchy) {
        m_sceneHierarchy->render();

        // Connect to inspector
        if (m_inspector) {
            auto* selectedNode = m_sceneHierarchy->getSelectedNode();
            m_inspector->setInspectedNode(selectedNode);
        }
    }

    if (m_showInspector && m_inspector) {
        m_inspector->render();
    }

    if (m_showContentBrowser && m_contentBrowser) {
        m_contentBrowser->render();
    }

    if (m_showConsole && m_console) {
        m_console->render();
    }

    if (m_showToolPalette && m_voxelTools) {
        m_voxelTools->render();
    }

    // Render main menu panel if active
    if (m_mainMenuPanel) {
        m_mainMenuPanel->render();
    }

    // Render settings panel if visible
    if (m_settingsPanel) {
        m_settingsPanel->render();
    }

    // Render hotbar (shown in play mode)
    if (m_hotbar) {
        m_hotbar->render();
    }

#ifdef _WIN32
    // Render Windows customization panel if available
    if (m_windowsCustomizationPanel) {
        m_windowsCustomizationPanel->render();
    }
#endif
#else
    // Console mode fallback
    LOG_INFO_C("Editor running in console mode (ImGui not available)", "EditorManager");
#endif
}

void EditorManager::endFrame()
{
    if (!m_initialized) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Always call render() when initialized, regardless of visibility
    // This is required by ImGui - NewFrame() and Render() must always be paired
    if (m_imguiContext) {
        m_imguiContext->render();
    }
#endif
}

void EditorManager::shutdown()
{
    if (!m_initialized) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Shutdown in reverse order
    m_hotbar.reset();
    m_settingsPanel.reset();
    m_mainMenuPanel.reset();
    m_voxelTools.reset();
    m_console.reset();
    m_contentBrowser.reset();
    m_toolbar.reset();
    m_menuBar.reset();
    m_inspector.reset();
    m_sceneHierarchy.reset();

#ifdef _WIN32
    // Shutdown Windows-native features
    if (m_windowsCustomizationPanel) {
        m_windowsCustomizationPanel.reset();
    }
    if (m_windowsTaskbarManager) {
        m_windowsTaskbarManager->shutdown();
        m_windowsTaskbarManager.reset();
    }
    if (m_windowsDialogManager) {
        m_windowsDialogManager->shutdown();
        m_windowsDialogManager.reset();
    }
    if (m_windowsThemeManager) {
        m_windowsThemeManager->shutdown();
        m_windowsThemeManager.reset();
    }
#endif

    if (m_imguiContext) {
        m_imguiContext->shutdown();
    }
    m_imguiContext.reset();
#endif

    // Clear all pointer references to avoid dangling pointers
    m_window = nullptr;
    m_renderContext = nullptr;
    m_world = nullptr;
    m_worldEditor = nullptr;

    m_initialized = false;
    m_visible = false; // Reset visibility flag on shutdown
    LOG_INFO_C("EditorManager shutdown complete", "EditorManager");
}

bool EditorManager::updateWorld(VoxelWorld* world, WorldEditor* worldEditor)
{
    if (!m_initialized) {
        LOG_ERROR_C("EditorManager not initialized, cannot update world", "EditorManager");
        return false;
    }

    if (!world || !worldEditor) {
        LOG_ERROR_C("Invalid world or worldEditor parameters", "EditorManager");
        return false;
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Update world references
    m_world = world;
    m_worldEditor = worldEditor;

    // Recreate world-dependent panels with new world reference
    // Shutdown and reinitialize only the world-dependent panels
    m_sceneHierarchy.reset();
    m_inspector.reset();
    m_menuBar.reset();
    m_toolbar.reset();
    m_contentBrowser.reset();
    m_console.reset();
    m_voxelTools.reset();

    // Reinitialize world-dependent panels
    m_sceneHierarchy = std::make_unique<SceneHierarchyPanel>();
    if (!m_sceneHierarchy->initialize(world)) {
        LOG_ERROR_C("Failed to initialize Scene Hierarchy panel", "EditorManager");
        return false;
    }

    m_inspector = std::make_unique<InspectorPanel>();
    if (!m_inspector->initialize()) {
        LOG_ERROR_C("Failed to initialize Inspector panel", "EditorManager");
        return false;
    }

    m_menuBar = std::make_unique<EditorMenuBar>();
    if (!m_menuBar->initialize(world, worldEditor)) {
        LOG_ERROR_C("Failed to initialize Menu Bar", "EditorManager");
        return false;
    }

    // Link menu bar to panel visibility flags
    m_menuBar->setSceneHierarchyVisible(&m_showSceneHierarchy);
    m_menuBar->setInspectorVisible(&m_showInspector);
    m_menuBar->setContentBrowserVisible(&m_showContentBrowser);
    m_menuBar->setConsoleVisible(&m_showConsole);
    m_menuBar->setToolPaletteVisible(&m_showToolPalette);

    // Set settings callback to open settings panel
    m_menuBar->setSettingsCallback([this]() {
        if (m_settingsPanel) {
            m_settingsPanel->setVisible(true);
        }
    });

    // Set import assets callback to show import dialog in content browser
    m_menuBar->setImportAssetsCallback([this]() {
        if (m_contentBrowser) {
            m_contentBrowser->showImportDialog();
        }
    });

    // Set select all/deselect all callbacks for scene hierarchy
    m_menuBar->setSelectAllCallback([this]() {
        if (m_sceneHierarchy) {
            m_sceneHierarchy->selectAll();
        }
    });

    m_menuBar->setDeselectAllCallback([this]() {
        if (m_sceneHierarchy) {
            m_sceneHierarchy->deselectAll();
        }
    });

    // Set undo/redo callbacks for terraforming
    m_menuBar->setUndoCallback([this, worldEditor]() {
        if (worldEditor && worldEditor->getTerraformingSystem()) {
            if (worldEditor->getTerraformingSystem()->undo()) {
                LOG_INFO_C("Undo performed", "EditorManager");
            } else {
                LOG_INFO_C("Nothing to undo", "EditorManager");
            }
        }
    });

    m_menuBar->setRedoCallback([this, worldEditor]() {
        if (worldEditor && worldEditor->getTerraformingSystem()) {
            if (worldEditor->getTerraformingSystem()->redo()) {
                LOG_INFO_C("Redo performed", "EditorManager");
            } else {
                LOG_INFO_C("Nothing to redo", "EditorManager");
            }
        }
    });

    m_toolbar = std::make_unique<EditorToolbar>();
    if (!m_toolbar->initialize()) {
        LOG_ERROR_C("Failed to initialize Toolbar", "EditorManager");
        return false;
    }

    m_contentBrowser = std::make_unique<ContentBrowserPanel>();
    if (!m_contentBrowser->initialize("assets")) {
        LOG_ERROR_C("Failed to initialize Content Browser panel", "EditorManager");
        return false;
    }

    m_console = std::make_unique<ConsolePanel>();
    if (!m_console->initialize()) {
        LOG_ERROR_C("Failed to initialize Console panel", "EditorManager");
        return false;
    }

    m_voxelTools = std::make_unique<VoxelToolPalette>();
    if (!m_voxelTools->initialize(worldEditor->getTerraformingSystem())) {
        LOG_ERROR_C("Failed to initialize Voxel Tool Palette", "EditorManager");
        return false;
    }

    LOG_INFO_C("EditorManager updated with new world successfully", "EditorManager");
    return true;
#else
    LOG_WARNING_C("ImGui not available, updateWorld has no effect", "EditorManager");
    return true;
#endif
}

void EditorManager::setupDockspace()
{
#ifdef FRESH_IMGUI_AVAILABLE
    // Create a fullscreen dockspace (requires ImGui docking branch)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    #ifdef IMGUI_HAS_VIEWPORT
    ImGui::SetNextWindowViewport(viewport->ID);
    #endif

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    #ifdef IMGUI_HAS_DOCK
    window_flags |= ImGuiWindowFlags_NoDocking;
    #endif

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    #ifdef IMGUI_HAS_DOCK
    // DockSpace (only available in docking branch)
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    #endif

    ImGui::End();
#endif
}

bool EditorManager::wantCaptureMouse() const
{
#ifdef FRESH_IMGUI_AVAILABLE
    // Only capture input when editor is visible
    if (m_visible && m_imguiContext) {
        return m_imguiContext->wantCaptureMouse();
    }
#endif
    return false;
}

bool EditorManager::wantCaptureKeyboard() const
{
#ifdef FRESH_IMGUI_AVAILABLE
    // Only capture input when editor is visible
    if (m_visible && m_imguiContext) {
        return m_imguiContext->wantCaptureKeyboard();
    }
#endif
    return false;
}

} // namespace fresh
