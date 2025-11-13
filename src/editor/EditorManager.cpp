#include "editor/EditorManager.h"

#include "core/Logger.h"
#include "core/Window.h"
#include "editor/WorldEditor.h"
#include "renderer/RenderContext.h"
#include "ui/ConsolePanel.h"
#include "ui/ContentBrowserPanel.h"
#include "ui/EditorMenuBar.h"
#include "ui/EditorToolbar.h"
#include "ui/ImGuiContext.h"
#include "ui/InspectorPanel.h"
#include "ui/MainMenuPanel.h"
#include "ui/SceneHierarchyPanel.h"
#include "ui/SettingsPanel.h"
#include "ui/VoxelToolPalette.h"
#include "voxel/VoxelWorld.h"

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
    if (!m_initialized || !m_visible) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
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
#else
    // Console mode fallback
    LOG_INFO_C("Editor running in console mode (ImGui not available)", "EditorManager");
#endif
}

void EditorManager::endFrame()
{
    if (!m_initialized || !m_visible) {
        return;
    }

#ifdef FRESH_IMGUI_AVAILABLE
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
    m_voxelTools.reset();
    m_console.reset();
    m_contentBrowser.reset();
    m_toolbar.reset();
    m_menuBar.reset();
    m_inspector.reset();
    m_sceneHierarchy.reset();

    if (m_imguiContext) {
        m_imguiContext->shutdown();
    }
    m_imguiContext.reset();
#endif

    m_initialized = false;
    LOG_INFO_C("EditorManager shutdown complete", "EditorManager");
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
                                    ImGuiWindowFlags_NoNavFocus;

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
