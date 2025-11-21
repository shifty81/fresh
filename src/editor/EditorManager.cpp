#include "editor/EditorManager.h"

// Prevent Windows.h from defining min/max macros (must be before any Windows includes)
#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
#endif

#include <cmath>
#include "core/Logger.h"
#include "ecs/EntityManager.h"
#include "gameplay/Player.h"
#include "gameplay/Camera.h"
#ifdef _WIN32
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
    #include <windows.h>
    #include <filesystem>
#else
    #include "core/Window.h"
    #include "input/InputManager.h"
#endif
#include "editor/WorldEditor.h"
#include "editor/SelectionManager.h"
#include "editor/SelectionRenderer.h"
#include "editor/TransformGizmo.h"
#include "editor/FileDialogManager.h"
#include "editor/LayoutManager.h"
#include "editor/EditorSettingsDialog.h"
#include "editor/CameraController.h"
#include "serialization/WorldSerializer.h"
#include "devtools/DebugRenderer.h"
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
#include "ui/EngineConfigPanel.h"
#include "ui/VoxelToolPalette.h"
#include "voxel/VoxelWorld.h"

#include "ui/VoxelToolPalette.h"
#include "voxel/VoxelWorld.h"

#include <glm/glm.hpp>

#ifdef _WIN32
    #include "ui/WindowsThemeManager.h"
    #include "ui/WindowsDialogManager.h"
    #include "ui/WindowsTaskbarManager.h"
    #include "ui/WindowsCustomizationPanel.h"
    #include "ui/native/Win32InspectorPanel.h"
    #include "ui/native/Win32SceneHierarchyPanel.h"
    #include "ui/native/Win32ContentBrowserPanel.h"
    #include "ui/native/Win32ConsolePanel.h"
    #include "ui/native/Win32HUD.h"
    #include "ui/native/Win32SettingsDialog.h"
    #include "ui/native/Win32TerraformingPanel.h"
    #include "ui/native/Win32ViewportPanel.h"
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
      m_entityManager(nullptr),
      m_player(nullptr),
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

bool EditorManager::initialize(WindowType* window, IRenderContext* renderContext, VoxelWorld* world,
                               WorldEditor* worldEditor, InputManagerType* inputManager,
                               ecs::EntityManager* entityManager)
{
    (void)inputManager;  // Unused parameter
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
    m_entityManager = entityManager;

#ifndef _WIN32
    // Initialize ImGui-based UI panels (non-Windows platforms)
    // These are not available on Windows which uses native Win32 UI instead
    
    // Initialize UI panels that require world/worldEditor
    // Only initialize these if world and worldEditor are available
    if (world && worldEditor) {
        m_sceneHierarchy = std::make_unique<SceneHierarchyPanel>();
        if (!m_sceneHierarchy->initialize(world)) {
            LOG_ERROR_C("Failed to initialize Scene Hierarchy panel", "EditorManager");
            return false;
        }

        m_inspector = std::make_unique<InspectorPanel>();
        if (!m_inspector->initialize(m_entityManager)) {
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

        // Set engine configuration callback to open engine config panel
        m_menuBar->setEngineConfigCallback([this]() {
            if (m_engineConfigPanel) {
                m_engineConfigPanel->setVisible(true);
            }
        });
        
        // Set editor settings callback to open editor settings dialog
        m_menuBar->setEditorSettingsCallback([this]() {
            if (m_editorSettingsDialog) {
                m_editorSettingsDialog->setVisible(true);
            }
        });

        // Set import assets callback to show import dialog in content browser
        m_menuBar->setImportAssetsCallback([this]() {
            if (m_contentBrowser) {
                m_contentBrowser->showImportDialog();
            }
        });

        // Set dialogue editor callback
        m_menuBar->setDialogueEditorCallback([this]() {
            launchDialogueEditor();
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
        
        // Set up layout management callbacks
        m_menuBar->setLoadLayoutCallback([this](const std::string& name) {
            loadLayout(name);
        });
        
        m_menuBar->setSaveLayoutCallback([this]() {
            saveCurrentLayout();
        });
        
        m_menuBar->setResetLayoutCallback([this]() {
            resetLayout();
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
                // Get paste position from camera/player if available
                glm::ivec3 pastePos(0, 64, 0);  // Default position
                
                if (m_player) {
                    // Use player's camera position as the paste location
                    const auto& camera = m_player->getCamera();
                    glm::vec3 cameraPos = camera.getPosition();
                    // Offset slightly forward from camera
                    glm::vec3 forward = camera.getFront();
                    glm::vec3 targetPos = cameraPos + forward * 5.0f;
                    pastePos = glm::ivec3(
                        static_cast<int>(std::floor(targetPos.x)),
                        static_cast<int>(std::floor(targetPos.y)),
                        static_cast<int>(std::floor(targetPos.z))
                    );
                }
                
                m_selectionManager->pasteFromClipboard(pastePos, world);
                LOG_INFO_C("Pasted clipboard content at " + 
                          std::to_string(pastePos.x) + ", " + 
                          std::to_string(pastePos.y) + ", " + 
                          std::to_string(pastePos.z), "EditorManager");
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

        // Note: File dialog callbacks will be set up after WindowsDialogManager is initialized
        // See below in Windows-specific initialization section

        m_toolbar = std::make_unique<EditorToolbar>();
        if (!m_toolbar->initialize()) {
            LOG_ERROR_C("Failed to initialize Toolbar", "EditorManager");
            return false;
        }
        
        // Set up toolbar callbacks for transform gizmo mode switching
        m_toolbar->setToolCallback([this](EditorToolbar::Tool tool) {
            if (!m_transformGizmo) return;
            
            switch (tool) {
                case EditorToolbar::Tool::Move:
                    m_transformGizmo->setMode(TransformGizmo::Mode::Translate);
                    LOG_INFO_C("Toolbar: Transform Gizmo set to Translate mode", "EditorManager");
                    break;
                case EditorToolbar::Tool::Rotate:
                    m_transformGizmo->setMode(TransformGizmo::Mode::Rotate);
                    LOG_INFO_C("Toolbar: Transform Gizmo set to Rotate mode", "EditorManager");
                    break;
                case EditorToolbar::Tool::Scale:
                    m_transformGizmo->setMode(TransformGizmo::Mode::Scale);
                    LOG_INFO_C("Toolbar: Transform Gizmo set to Scale mode", "EditorManager");
                    break;
                default:
                    // Other tools don't affect the transform gizmo
                    break;
            }
        });

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

        // Initialize debug renderer for selection visualization
        m_debugRenderer = std::make_unique<devtools::DebugRenderer>();
        m_debugRenderer->setEnabled(true);
        LOG_INFO_C("Debug Renderer initialized", "EditorManager");

        // Initialize selection manager
        m_selectionManager = std::make_unique<SelectionManager>();
        LOG_INFO_C("Selection Manager initialized", "EditorManager");

        // Initialize selection renderer
        m_selectionRenderer = std::make_unique<SelectionRenderer>();
        if (!m_selectionRenderer->initialize(m_debugRenderer.get())) {
            LOG_ERROR_C("Failed to initialize Selection Renderer", "EditorManager");
            return false;
        }
        LOG_INFO_C("Selection Renderer initialized", "EditorManager");
        
        // Initialize transform gizmo
        m_transformGizmo = std::make_unique<TransformGizmo>();
        m_transformGizmo->setDebugRenderer(m_debugRenderer.get());
        LOG_INFO_C("Transform Gizmo initialized", "EditorManager");
        
        // Initialize world serializer
        m_worldSerializer = std::make_unique<WorldSerializer>();
        LOG_INFO_C("World Serializer initialized", "EditorManager");
        
        // Initialize layout manager
        m_layoutManager = std::make_unique<LayoutManager>();
        m_layoutManager->initialize("configs/layouts.ini");
        
        // Initialize editor settings dialog
        m_editorSettingsDialog = std::make_unique<EditorSettingsDialog>();
        m_editorSettingsDialog->initialize("configs/editor_settings.ini");
        LOG_INFO_C("Editor Settings Dialog initialized", "EditorManager");
        
        // Initialize camera controller
        m_cameraController = std::make_unique<CameraController>();
        // Camera controller will be fully initialized when player is set via setPlayer()
        LOG_INFO_C("Camera Controller created (waiting for player)", "EditorManager");
        
        // Load the current or default layout
        LayoutConfig currentLayout;
        if (m_layoutManager->loadLayout(m_layoutManager->getCurrentLayoutName(), currentLayout)) {
            m_showSceneHierarchy = currentLayout.showSceneHierarchy;
            m_showInspector = currentLayout.showInspector;
            m_showContentBrowser = currentLayout.showContentBrowser;
            m_showConsole = currentLayout.showConsole;
            m_showToolPalette = currentLayout.showToolPalette;
            LOG_INFO_C("Loaded layout: " + m_layoutManager->getCurrentLayoutName(), "EditorManager");
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

    // Initialize engine configuration panel
    m_engineConfigPanel = std::make_unique<EngineConfigPanel>();
    if (!m_engineConfigPanel->initialize(renderContext)) {
        LOG_ERROR_C("Failed to initialize Engine Configuration Panel", "EditorManager");
        return false;
    }

    // Initialize hotbar panel (for play mode)
    m_hotbar = std::make_unique<HotbarPanel>();
    if (!m_hotbar->initialize()) {
        LOG_ERROR_C("Failed to initialize Hotbar Panel", "EditorManager");
        return false;
    }
    m_hotbar->setVisible(false); // Hidden by default (shown only in play mode)
#endif // _WIN32

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
    
#ifdef FRESH_WIN32_UI
    // Initialize native Win32 UI panels (replaces ImGui panels)
    LOG_INFO_C("Initializing native Win32 UI panels", "EditorManager");
    
    // Get Win32 window handle
    Win32Window* win32Window = dynamic_cast<Win32Window*>(window);
    if (win32Window) {
        HWND hwnd = win32Window->getHandle();
        
        // Create native Inspector panel
        m_nativeInspector = std::make_unique<Win32InspectorPanel>();
        if (m_nativeInspector->create(hwnd, 10, 80, 350, 500, m_entityManager)) {
            LOG_INFO_C("Native Win32 Inspector Panel created", "EditorManager");
        }
        
        // Create native Scene Hierarchy panel
        m_nativeSceneHierarchy = std::make_unique<Win32SceneHierarchyPanel>();
        if (m_nativeSceneHierarchy->create(hwnd, 370, 80, 300, 500, world)) {
            // Set selection callback to update inspector
            m_nativeSceneHierarchy->setSelectionCallback([this](HierarchyNode* node) {
                if (m_nativeInspector) {
                    m_nativeInspector->setInspectedNode(node);
                }
            });
            LOG_INFO_C("Native Win32 Scene Hierarchy Panel created", "EditorManager");
        }
        
        // Create native Content Browser panel
        m_nativeContentBrowser = std::make_unique<Win32ContentBrowserPanel>();
        if (m_nativeContentBrowser->create(hwnd, 10, 600, 660, 350, "assets")) {
            LOG_INFO_C("Native Win32 Content Browser Panel created", "EditorManager");
        }
        
        // Create native Console panel
        m_nativeConsole = std::make_unique<Win32ConsolePanel>();
        if (m_nativeConsole->create(hwnd, 680, 600, 600, 350)) {
            LOG_INFO_C("Native Win32 Console Panel created", "EditorManager");
        }
        
        // Create native HUD (for play mode)
        m_nativeHUD = std::make_unique<Win32HUD>();
        if (m_nativeHUD->initialize(hwnd)) {
            m_nativeHUD->setVisible(false); // Hidden by default
            LOG_INFO_C("Native Win32 HUD created", "EditorManager");
        }
        
        // Create native Settings Dialog
        m_nativeSettingsDialog = std::make_unique<Win32SettingsDialog>();
        LOG_INFO_C("Native Win32 Settings Dialog created", "EditorManager");
        
        // Create native Terraforming Panel
        if (worldEditor) {
            m_nativeTerraformingPanel = std::make_unique<Win32TerraformingPanel>();
            if (m_nativeTerraformingPanel->initialize(hwnd, worldEditor)) {
                LOG_INFO_C("Native Win32 Terraforming Panel created", "EditorManager");
            }
        }
        
        // Create native Viewport Panel (central 3D view)
        // Position it to take most of the window, leaving space for UI panels
        // Get window client area size
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;
        
        // Position viewport in center-right area (leaving left for panels)
        int viewportX = 680;  // Start after left panels
        int viewportY = 80;   // Start below menu/toolbar
        int viewportWidth = clientWidth - 680 - 10;  // Fill remaining width
        int viewportHeight = clientHeight - 80 - 10;  // Fill remaining height
        
        m_viewportPanel = std::make_unique<Win32ViewportPanel>();
        if (m_viewportPanel->create(hwnd, viewportX, viewportY, viewportWidth, viewportHeight)) {
            LOG_INFO_C("Native Win32 Viewport Panel created at (" + 
                      std::to_string(viewportX) + ", " + std::to_string(viewportY) + ") " +
                      "size " + std::to_string(viewportWidth) + "x" + std::to_string(viewportHeight), "EditorManager");
        } else {
            LOG_ERROR_C("Failed to create Native Win32 Viewport Panel", "EditorManager");
        }
        
        LOG_INFO_C("All native Win32 UI panels initialized successfully", "EditorManager");
    } else {
        LOG_WARNING_C("Win32Window not available, native panels not created", "EditorManager");
    }
#endif // FRESH_WIN32_UI
    
#endif // _WIN32

    // Connect scene hierarchy to inspector
    // When a node is selected in the hierarchy, show it in the inspector
    // This would require adding a callback system, but for now we'll handle it in render()

    m_initialized = true;
    LOG_INFO_C("EditorManager initialized successfully", "EditorManager");
    LOG_INFO_C("All editor UI panels initialized", "EditorManager");
    return true;
}

void EditorManager::beginFrame()
{
    if (!m_initialized) {
        return;
    }
}

void EditorManager::render()
{
    if (!m_initialized || !m_visible) {
        return;
    }

    // Render panels based on visibility flags
#ifndef FRESH_WIN32_UI
    // Use panels when native Win32 UI is not available
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
#else
    // When using native Win32 UI, native panels are always visible and managed by Win32
    // No need to render them here - they are native windows
    // The native panels were created during initialization and handle their own rendering
#endif // !FRESH_WIN32_UI

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

    // Render engine configuration panel if visible
    if (m_engineConfigPanel) {
        m_engineConfigPanel->render();
    }
    
    // Render editor settings dialog if visible
    if (m_editorSettingsDialog) {
        m_editorSettingsDialog->render();
    }

#ifndef FRESH_WIN32_UI
    // Render hotbar (shown in play mode)
    // On Windows with FRESH_WIN32_UI, use native Win32HUD instead
    if (m_hotbar) {
        m_hotbar->render();
    }
#endif // !FRESH_WIN32_UI

    // Render selection visualization (3D rendering, not UI)
    if (m_selectionRenderer && m_selectionManager) {
        m_selectionRenderer->render(m_selectionManager.get());
    }

    // Update and render debug visualization
    if (m_debugRenderer) {
        m_debugRenderer->render();
    }

#ifdef _WIN32
    // Render Windows customization panel if available
    if (m_windowsCustomizationPanel) {
        m_windowsCustomizationPanel->render();
    }
#endif
}

void EditorManager::endFrame()
{
    if (!m_initialized) {
        return;
    }
}

void EditorManager::shutdown()
{
    if (!m_initialized) {
        return;
    }

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

    // Clear all pointer references to avoid dangling pointers
    m_window = nullptr;
    m_renderContext = nullptr;
    m_world = nullptr;
    m_worldEditor = nullptr;

    m_initialized = false;
    m_visible = false; // Reset visibility flag on shutdown
    LOG_INFO_C("EditorManager shutdown complete", "EditorManager");
}

void EditorManager::setPlayer(Player* player)
{
    m_player = player;
    
    // Initialize camera controller when player is set
    if (m_cameraController && player) {
        m_cameraController->initialize(player);
        LOG_INFO_C("Camera controller initialized with player", "EditorManager");
    }
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
    if (!m_inspector->initialize(m_entityManager)) {
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
    // On Windows with native UI, updating world is simpler
    m_world = world;
    m_worldEditor = worldEditor;
    LOG_INFO_C("EditorManager updated with new world (Windows native UI)", "EditorManager");
    return true;
#endif // FRESH_IMGUI_AVAILABLE
}

bool EditorManager::wantCaptureMouse() const
{
    // For Windows native UI, check if any UI element is capturing mouse
    // For now, return false to allow normal game interaction
    return false;
}

bool EditorManager::wantCaptureKeyboard() const
{
    // For Windows native UI, check if any UI element is capturing keyboard
    // For now, return false to allow normal game interaction
    return false;
}

// ========== Editor Operations Implementation ==========

void EditorManager::undo()
{
    if (m_worldEditor && m_worldEditor->getTerraformingSystem()) {
        if (m_worldEditor->getTerraformingSystem()->undo()) {
            LOG_INFO_C("Undo performed", "EditorManager");
        } else {
            LOG_INFO_C("Nothing to undo", "EditorManager");
        }
    }
}

void EditorManager::redo()
{
    if (m_worldEditor && m_worldEditor->getTerraformingSystem()) {
        if (m_worldEditor->getTerraformingSystem()->redo()) {
            LOG_INFO_C("Redo performed", "EditorManager");
        } else {
            LOG_INFO_C("Nothing to redo", "EditorManager");
        }
    }
}

void EditorManager::cut()
{
    if (m_selectionManager && m_selectionManager->hasSelection()) {
        m_selectionManager->cutToClipboard(m_world);
        LOG_INFO_C("Cut selection to clipboard", "EditorManager");
    } else {
        LOG_INFO_C("No selection to cut", "EditorManager");
    }
}

void EditorManager::copy()
{
    if (m_selectionManager && m_selectionManager->hasSelection()) {
        m_selectionManager->copyToClipboard(m_world);
        LOG_INFO_C("Copied selection to clipboard", "EditorManager");
    } else {
        LOG_INFO_C("No selection to copy", "EditorManager");
    }
}

void EditorManager::paste()
{
    if (m_selectionManager && m_selectionManager->hasClipboardData()) {
        // Get paste position from camera/player if available
        glm::ivec3 pastePos(0, 64, 0);  // Default position
        
        if (m_player) {
            // Use player's camera position as the paste location
            const auto& camera = m_player->getCamera();
            glm::vec3 cameraPos = camera.getPosition();
            // Offset slightly forward from camera
            glm::vec3 forward = camera.getFront();
            glm::vec3 targetPos = cameraPos + forward * 5.0f;
            pastePos = glm::ivec3(
                static_cast<int>(std::floor(targetPos.x)),
                static_cast<int>(std::floor(targetPos.y)),
                static_cast<int>(std::floor(targetPos.z))
            );
        }
        
        m_selectionManager->pasteFromClipboard(pastePos, m_world);
        LOG_INFO_C("Pasted clipboard content at " + 
                  std::to_string(pastePos.x) + ", " + 
                  std::to_string(pastePos.y) + ", " + 
                  std::to_string(pastePos.z), "EditorManager");
    } else {
        LOG_INFO_C("Clipboard is empty", "EditorManager");
    }
}

void EditorManager::deleteSelection()
{
    if (m_selectionManager && m_selectionManager->hasSelection()) {
        m_selectionManager->deleteSelected(m_world);
        LOG_INFO_C("Deleted selection", "EditorManager");
    } else {
        LOG_INFO_C("No selection to delete", "EditorManager");
    }
}

void EditorManager::selectAll()
{
    if (m_sceneHierarchy) {
        m_sceneHierarchy->selectAll();
        LOG_INFO_C("Selected all", "EditorManager");
    }
#ifdef _WIN32
    else if (m_nativeSceneHierarchy) {
        m_nativeSceneHierarchy->selectAll();
        LOG_INFO_C("Selected all (native)", "EditorManager");
    }
#endif
}

void EditorManager::deselectAll()
{
    if (m_sceneHierarchy) {
        m_sceneHierarchy->deselectAll();
        LOG_INFO_C("Deselected all", "EditorManager");
    }
#ifdef _WIN32
    else if (m_nativeSceneHierarchy) {
        m_nativeSceneHierarchy->deselectAll();
        LOG_INFO_C("Deselected all (native)", "EditorManager");
    }
#endif
}

void EditorManager::saveWorld()
{
#ifdef _WIN32
    if (m_worldSerializer && m_world) {
        if (!m_currentWorldPath.empty()) {
            // Save to current path
            if (m_worldSerializer->saveWorld(m_world, m_currentWorldPath)) {
                LOG_INFO_C("World saved successfully to: " + m_currentWorldPath, "EditorManager");
                if (m_windowsDialogManager) {
                    m_windowsDialogManager->showMessageBox(
                        "Save World",
                        "World saved successfully!",
                        MessageBoxButtons::OK,
                        MessageBoxIcon::Information
                    );
                }
            } else {
                LOG_ERROR_C("Failed to save world to: " + m_currentWorldPath, "EditorManager");
                if (m_windowsDialogManager) {
                    m_windowsDialogManager->showMessageBox(
                        "Save World",
                        "Failed to save world!",
                        MessageBoxButtons::OK,
                        MessageBoxIcon::Error
                    );
                }
            }
        } else {
            // No current path, show save as dialog
            saveWorldAs();
        }
    }
#else
    LOG_INFO_C("Save World not implemented on this platform", "EditorManager");
#endif
}

void EditorManager::saveWorldAs()
{
#ifdef _WIN32
    if (m_windowsDialogManager && m_windowsDialogManager->isInitialized() && m_worldSerializer && m_world) {
        LOG_INFO_C("Opening Save World As dialog", "EditorManager");
        
        std::vector<FileFilter> filters = {
            {"Fresh World Files", "*.world"},
            {"All Files", "*.*"}
        };
        
        auto selectedFile = m_windowsDialogManager->showSaveFileDialog(
            "Save World As", "world", filters);
        
        if (!selectedFile.empty()) {
            LOG_INFO_C("User selected save path: " + selectedFile, "EditorManager");
            
            if (m_worldSerializer->saveWorld(m_world, selectedFile)) {
                m_currentWorldPath = selectedFile;
                LOG_INFO_C("World saved successfully to: " + selectedFile, "EditorManager");
                
                m_windowsDialogManager->showMessageBox(
                    "Save World",
                    "World saved successfully!",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );
            } else {
                LOG_ERROR_C("Failed to save world to: " + selectedFile, "EditorManager");
                
                m_windowsDialogManager->showMessageBox(
                    "Save World",
                    "Failed to save world!",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Error
                );
            }
        } else {
            LOG_INFO_C("Save cancelled by user", "EditorManager");
        }
    }
#else
    // Use cross-platform file dialog manager
    if (FileDialogManager::isAvailable() && m_worldSerializer && m_world) {
        LOG_INFO_C("Opening Save World As dialog", "EditorManager");
        
        std::vector<FileDialogManager::Filter> filters = {
            {"Fresh World Files", "world"},
            {"All Files", "*"}
        };
        
        auto selectedFile = FileDialogManager::saveFile(filters, "", "NewWorld.world");
        
        if (!selectedFile.empty()) {
            LOG_INFO_C("User selected save path: " + selectedFile, "EditorManager");
            
            if (m_worldSerializer->saveWorld(m_world, selectedFile)) {
                m_currentWorldPath = selectedFile;
                LOG_INFO_C("World saved successfully to: " + selectedFile, "EditorManager");
            } else {
                LOG_ERROR_C("Failed to save world to: " + selectedFile, "EditorManager");
            }
        } else {
            LOG_INFO_C("Save cancelled by user", "EditorManager");
        }
    } else {
        LOG_INFO_C("Save World As not available (file dialogs not available)", "EditorManager");
    }
#endif
}

void EditorManager::loadWorld()
{
#ifdef _WIN32
    if (m_windowsDialogManager && m_windowsDialogManager->isInitialized() && m_worldSerializer && m_world) {
        LOG_INFO_C("Opening Load World dialog", "EditorManager");
        
        std::vector<FileFilter> filters = {
            {"Fresh World Files", "*.world"},
            {"All Files", "*.*"}
        };
        
        auto selectedFiles = m_windowsDialogManager->showOpenFileDialog(
            "Open World", filters, false);
        
        if (!selectedFiles.empty()) {
            const std::string& worldPath = selectedFiles[0];
            LOG_INFO_C("User selected world file: " + worldPath, "EditorManager");
            
            // Clear existing world by unloading all chunks
            auto& chunks = m_world->getChunks();
            std::vector<ChunkPos> chunksToUnload;
            for (const auto& [pos, chunk] : chunks) {
                chunksToUnload.push_back(pos);
            }
            for (const auto& pos : chunksToUnload) {
                m_world->unloadChunk(pos);
            }
            
            // Load the world
            if (m_worldSerializer->loadWorld(m_world, worldPath)) {
                m_currentWorldPath = worldPath;
                LOG_INFO_C("World loaded successfully from: " + worldPath, "EditorManager");
                
                m_windowsDialogManager->showMessageBox(
                    "Load World",
                    "World loaded successfully!",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );
            } else {
                LOG_ERROR_C("Failed to load world from: " + worldPath, "EditorManager");
                
                m_windowsDialogManager->showMessageBox(
                    "Load World",
                    "Failed to load world!",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Error
                );
            }
        } else {
            LOG_INFO_C("Load cancelled by user", "EditorManager");
        }
    }
#else
    // Use cross-platform file dialog manager
    if (FileDialogManager::isAvailable() && m_worldSerializer && m_world) {
        LOG_INFO_C("Opening Load World dialog", "EditorManager");
        
        std::vector<FileDialogManager::Filter> filters = {
            {"Fresh World Files", "world"},
            {"All Files", "*"}
        };
        
        auto selectedFile = FileDialogManager::openFile(filters);
        
        if (!selectedFile.empty()) {
            LOG_INFO_C("User selected world file: " + selectedFile, "EditorManager");
            
            // Clear existing world by unloading all chunks
            auto& chunks = m_world->getChunks();
            std::vector<ChunkPos> chunksToUnload;
            for (const auto& [pos, chunk] : chunks) {
                chunksToUnload.push_back(pos);
            }
            for (const auto& pos : chunksToUnload) {
                m_world->unloadChunk(pos);
            }
            
            // Load the world
            if (m_worldSerializer->loadWorld(m_world, selectedFile)) {
                m_currentWorldPath = selectedFile;
                LOG_INFO_C("World loaded successfully from: " + selectedFile, "EditorManager");
            } else {
                LOG_ERROR_C("Failed to load world from: " + selectedFile, "EditorManager");
            }
        } else {
            LOG_INFO_C("Load cancelled by user", "EditorManager");
        }
    } else {
        LOG_INFO_C("Load World not available (file dialogs not available)", "EditorManager");
    }
#endif
}

void EditorManager::newWorld()
{
#ifdef _WIN32
    if (m_windowsDialogManager) {
        // Show confirmation dialog if there's unsaved work
        auto result = m_windowsDialogManager->showMessageBox(
            "New World",
            "Create a new world? Any unsaved changes will be lost.",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Question
        );
        
        if (result == MessageBoxResult::Yes) {
            // Show the main menu panel to allow user to configure new world
            if (m_mainMenuPanel) {
                m_mainMenuPanel->setMenuActive(true);
                // Clear any existing flags
                m_mainMenuPanel->clearFlags();
                LOG_INFO_C("Main menu activated for new world creation", "EditorManager");
            }
        } else {
            LOG_INFO_C("New world cancelled by user", "EditorManager");
        }
    }
#else
    LOG_INFO_C("New World not implemented on this platform", "EditorManager");
#endif
}

void EditorManager::toggleSceneHierarchy()
{
    m_showSceneHierarchy = !m_showSceneHierarchy;
    LOG_INFO_C("Scene Hierarchy toggled: " + std::string(m_showSceneHierarchy ? "shown" : "hidden"), "EditorManager");
    
#ifdef FRESH_WIN32_UI
    // Toggle native Win32 panel visibility
    if (m_nativeSceneHierarchy) {
        m_nativeSceneHierarchy->setVisible(m_showSceneHierarchy);
    }
#endif
}

void EditorManager::toggleInspector()
{
    m_showInspector = !m_showInspector;
    LOG_INFO_C("Inspector toggled: " + std::string(m_showInspector ? "shown" : "hidden"), "EditorManager");
    
#ifdef FRESH_WIN32_UI
    // Toggle native Win32 panel visibility
    if (m_nativeInspector) {
        m_nativeInspector->setVisible(m_showInspector);
    }
#endif
}

void EditorManager::toggleContentBrowser()
{
    m_showContentBrowser = !m_showContentBrowser;
    LOG_INFO_C("Content Browser toggled: " + std::string(m_showContentBrowser ? "shown" : "hidden"), "EditorManager");
    
#ifdef FRESH_WIN32_UI
    // Toggle native Win32 panel visibility
    if (m_nativeContentBrowser) {
        m_nativeContentBrowser->setVisible(m_showContentBrowser);
    }
#endif
}

void EditorManager::toggleConsole()
{
    m_showConsole = !m_showConsole;
    LOG_INFO_C("Console toggled: " + std::string(m_showConsole ? "shown" : "hidden"), "EditorManager");
    
#ifdef FRESH_WIN32_UI
    // Toggle native Win32 panel visibility
    if (m_nativeConsole) {
        m_nativeConsole->setVisible(m_showConsole);
    }
#endif
}

void EditorManager::toggleToolPalette()
{
    m_showToolPalette = !m_showToolPalette;
    LOG_INFO_C("Tool Palette toggled: " + std::string(m_showToolPalette ? "shown" : "hidden"), "EditorManager");
}

void EditorManager::showSettings()
{
    if (m_settingsPanel) {
        m_settingsPanel->setVisible(true);
        LOG_INFO_C("Settings panel shown", "EditorManager");
    }
}

void EditorManager::showEngineConfig()
{
    if (m_engineConfigPanel) {
        m_engineConfigPanel->setVisible(true);
        LOG_INFO_C("Engine configuration panel shown", "EditorManager");
    }
}

void EditorManager::showEditorSettings()
{
    if (m_editorSettingsDialog) {
        m_editorSettingsDialog->setVisible(true);
        LOG_INFO_C("Editor settings dialog shown", "EditorManager");
    }
}

void EditorManager::showImportAssets()
{
    if (m_contentBrowser) {
        m_contentBrowser->showImportDialog();
        LOG_INFO_C("Import assets dialog shown", "EditorManager");
    }
#ifdef _WIN32
    else if (m_nativeContentBrowser) {
        m_nativeContentBrowser->showImportDialog();
        LOG_INFO_C("Import assets dialog shown (native)", "EditorManager");
    }
#endif
}

void EditorManager::launchDialogueEditor()
{
#ifdef _WIN32
    LOG_INFO_C("Launching dialogue editor", "EditorManager");
    
    // Path to the dialogue editor executable
    std::string editorPath = "dotnet/DialogueEditor/bin/Release/net9.0-windows/DialogueEditor.exe";
    
    // Check if the editor exists
    if (!std::filesystem::exists(editorPath)) {
        // Try debug build
        editorPath = "dotnet/DialogueEditor/bin/Debug/net9.0-windows/DialogueEditor.exe";
        
        if (!std::filesystem::exists(editorPath)) {
            LOG_ERROR_C("Dialogue editor executable not found. Please build the DialogueEditor project.", "EditorManager");
            
            #ifdef FRESH_IMGUI_AVAILABLE
            // Show error dialog in ImGui if available
            // (This would be implemented in a future update)
            #endif
            
            return;
        }
    }
    
    // Launch the dialogue editor as a separate process
    STARTUPINFOA si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);
    
    // Create the process
    if (CreateProcessA(
        editorPath.c_str(),     // Application path
        nullptr,                // Command line
        nullptr,                // Process handle not inheritable
        nullptr,                // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        nullptr,                // Use parent's environment block
        nullptr,                // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi))                   // Pointer to PROCESS_INFORMATION structure
    {
        LOG_INFO_C("Dialogue editor launched successfully", "EditorManager");
        
        // Close process and thread handles as we don't need them
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        DWORD error = GetLastError();
        LOG_ERROR_C("Failed to launch dialogue editor. Error code: " + std::to_string(error), "EditorManager");
    }
#else
    LOG_WARNING_C("Dialogue editor is only available on Windows", "EditorManager");
#endif
}

void EditorManager::loadLayout(const std::string& name)
{
    if (!m_layoutManager) {
        LOG_WARNING_C("Layout manager not initialized", "EditorManager");
        return;
    }
    
    LayoutConfig config;
    if (m_layoutManager->loadLayout(name, config)) {
        // Apply the layout configuration
        m_showSceneHierarchy = config.showSceneHierarchy;
        m_showInspector = config.showInspector;
        m_showContentBrowser = config.showContentBrowser;
        m_showConsole = config.showConsole;
        m_showToolPalette = config.showToolPalette;
        
        LOG_INFO_C("Layout applied: " + name, "EditorManager");
    } else {
        LOG_ERROR_C("Failed to load layout: " + name, "EditorManager");
    }
}

void EditorManager::saveCurrentLayout(const std::string& name)
{
    if (!m_layoutManager) {
        LOG_WARNING_C("Layout manager not initialized", "EditorManager");
        return;
    }
    
    // Get the current panel visibility states
    LayoutConfig config;
    config.showSceneHierarchy = m_showSceneHierarchy;
    config.showInspector = m_showInspector;
    config.showContentBrowser = m_showContentBrowser;
    config.showConsole = m_showConsole;
    config.showToolPalette = m_showToolPalette;
    
    // Use provided name or current layout name
    std::string layoutName = name.empty() ? m_layoutManager->getCurrentLayoutName() : name;
    
    if (m_layoutManager->saveLayout(layoutName, config)) {
        LOG_INFO_C("Layout saved: " + layoutName, "EditorManager");
    } else {
        LOG_ERROR_C("Failed to save layout: " + layoutName, "EditorManager");
    }
}

void EditorManager::resetLayout()
{
    loadLayout("Default");
}

void EditorManager::setCameraViewMode(const std::string& mode)
{
    if (!m_cameraController) {
        LOG_WARNING_C("Camera controller not initialized", "EditorManager");
        return;
    }
    
    if (!m_player) {
        LOG_WARNING_C("Player not set, cannot initialize camera controller", "EditorManager");
        return;
    }
    
    // Initialize camera controller with player if not already done
    if (!m_player) {
        m_cameraController->initialize(m_player);
    }
    
    // Map string to view mode
    CameraController::ViewMode viewMode;
    if (mode == "Perspective") {
        viewMode = CameraController::ViewMode::Perspective;
    } else if (mode == "Top") {
        viewMode = CameraController::ViewMode::Top;
    } else if (mode == "Front") {
        viewMode = CameraController::ViewMode::Front;
    } else if (mode == "Side" || mode == "Right") {
        viewMode = CameraController::ViewMode::Side;
    } else if (mode == "Bottom") {
        viewMode = CameraController::ViewMode::Bottom;
    } else if (mode == "Back") {
        viewMode = CameraController::ViewMode::Back;
    } else if (mode == "Left") {
        viewMode = CameraController::ViewMode::Left;
    } else {
        LOG_WARNING_C("Unknown camera view mode: " + mode, "EditorManager");
        return;
    }
    
    m_cameraController->setViewMode(viewMode);
}

void EditorManager::focusOnSelection()
{
    if (!m_cameraController) {
        LOG_WARNING_C("Camera controller not initialized", "EditorManager");
        return;
    }
    
    if (!m_player) {
        LOG_WARNING_C("Player not set", "EditorManager");
        return;
    }
    
    // Initialize camera controller with player if not already done
    if (!m_player) {
        m_cameraController->initialize(m_player);
    }
    
    if (!m_selectionManager || !m_selectionManager->hasSelection()) {
        LOG_INFO_C("No selection to focus on", "EditorManager");
        return;
    }
    
    // Get selection center
    const auto& selectedBlocks = m_selectionManager->getSelectedVoxels();
    if (selectedBlocks.empty()) {
        LOG_INFO_C("Selection is empty", "EditorManager");
        return;
    }
    
    // Calculate center of selected blocks
    glm::vec3 center(0.0f);
    for (const auto& block : selectedBlocks) {
        center += glm::vec3(block.x, block.y, block.z);
    }
    center /= static_cast<float>(selectedBlocks.size());
    
    m_cameraController->focusOn(center);
    LOG_INFO_C("Focused camera on selection center", "EditorManager");
}

void EditorManager::frameSelection()
{
    if (!m_cameraController) {
        LOG_WARNING_C("Camera controller not initialized", "EditorManager");
        return;
    }
    
    if (!m_player) {
        LOG_WARNING_C("Player not set", "EditorManager");
        return;
    }
    
    // Initialize camera controller with player if not already done
    if (!m_player) {
        m_cameraController->initialize(m_player);
    }
    
    if (!m_selectionManager || !m_selectionManager->hasSelection()) {
        LOG_INFO_C("No selection to frame", "EditorManager");
        return;
    }
    
    // Get selection bounds
    const auto& selectedBlocks = m_selectionManager->getSelectedVoxels();
    if (selectedBlocks.empty()) {
        LOG_INFO_C("Selection is empty", "EditorManager");
        return;
    }
    
    // Calculate bounding box of selected blocks
    glm::vec3 boundsMin(std::numeric_limits<float>::max());
    glm::vec3 boundsMax(std::numeric_limits<float>::lowest());
    
    for (const auto& block : selectedBlocks) {
        glm::vec3 pos(block.x, block.y, block.z);
        boundsMin = glm::min(boundsMin, pos);
        boundsMax = glm::max(boundsMax, pos);
    }
    
    m_cameraController->frameBox(boundsMin, boundsMax);
    LOG_INFO_C("Framed selection in view", "EditorManager");
}

} // namespace fresh
