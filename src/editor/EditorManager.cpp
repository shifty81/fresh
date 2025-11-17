#include "editor/EditorManager.h"

#include "core/Logger.h"
#include "ecs/EntityManager.h"
#ifdef _WIN32
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
#else
    #include "core/Window.h"
    #include "input/InputManager.h"
#endif
#include "editor/WorldEditor.h"
#include "editor/SelectionManager.h"
#include "editor/SelectionRenderer.h"
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
      m_entityManager(nullptr),
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

#ifdef FRESH_IMGUI_AVAILABLE
    // Initialize ImGui context
    m_imguiContext = std::make_unique<ImGuiContext>();
#ifdef _WIN32
    if (!m_imguiContext->initializeWin32(window, renderContext)) {
        LOG_ERROR_C("Failed to initialize ImGui context with Win32 backend", "EditorManager");
        return false;
    }
    LOG_INFO_C("ImGui context initialized with Win32 backend", "EditorManager");
#else
    if (!m_imguiContext->initialize(window, renderContext)) {
        LOG_ERROR_C("Failed to initialize ImGui context", "EditorManager");
        return false;
    }
    LOG_INFO_C("ImGui context initialized", "EditorManager");
#endif

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

        // Note: File dialog callbacks will be set up after WindowsDialogManager is initialized
        // See below in Windows-specific initialization section

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
        
        // Initialize world serializer
        m_worldSerializer = std::make_unique<WorldSerializer>();
        LOG_INFO_C("World Serializer initialized", "EditorManager");
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
    
    // Set up file dialog callbacks now that WindowsDialogManager is initialized
    if (m_menuBar && m_windowsDialogManager && m_windowsDialogManager->isInitialized()) {
        // Set Load World callback - Show Open File dialog
        m_menuBar->setLoadWorldCallback([this, world]() {
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
                
                // Use WorldSerializer to load the world
                if (m_worldSerializer && world) {
                    // Clear existing world by unloading all chunks
                    auto& chunks = world->getChunks();
                    std::vector<ChunkPos> chunksToUnload;
                    for (const auto& [pos, chunk] : chunks) {
                        chunksToUnload.push_back(pos);
                    }
                    for (const auto& pos : chunksToUnload) {
                        world->unloadChunk(pos);
                    }
                    
                    // Load the world
                    if (m_worldSerializer->loadWorld(world, worldPath)) {
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
                            "Load World Error",
                            "Failed to load world file.\nPlease check the console for details.",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Error
                        );
                    }
                } else {
                    LOG_ERROR_C("WorldSerializer or World not available", "EditorManager");
                }
            } else {
                LOG_INFO_C("Load World dialog cancelled", "EditorManager");
            }
        });
        
        // Set Save World As callback - Show Save File dialog
        m_menuBar->setSaveWorldAsCallback([this, world]() {
            LOG_INFO_C("Opening Save World As dialog", "EditorManager");
            
            std::vector<FileFilter> filters = {
                {"Fresh World Files", "*.world"},
                {"All Files", "*.*"}
            };
            
            std::string savePath = m_windowsDialogManager->showSaveFileDialog(
                "Save World As", "NewWorld.world", filters);
            
            if (!savePath.empty()) {
                // Ensure .world extension
                if (savePath.find(".world") == std::string::npos) {
                    savePath += ".world";
                }
                
                LOG_INFO_C("User selected save location: " + savePath, "EditorManager");
                
                // Use WorldSerializer to save the world
                if (m_worldSerializer && world) {
                    if (m_worldSerializer->saveWorld(world, savePath)) {
                        m_currentWorldPath = savePath;
                        LOG_INFO_C("World saved successfully to: " + savePath, "EditorManager");
                        
                        m_windowsDialogManager->showMessageBox(
                            "Save World",
                            "World saved successfully!",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Information
                        );
                    } else {
                        LOG_ERROR_C("Failed to save world to: " + savePath, "EditorManager");
                        
                        m_windowsDialogManager->showMessageBox(
                            "Save World Error",
                            "Failed to save world file.\nPlease check the console for details.",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Error
                        );
                    }
                } else {
                    LOG_ERROR_C("WorldSerializer or World not available", "EditorManager");
                }
            } else {
                LOG_INFO_C("Save World As dialog cancelled", "EditorManager");
            }
        });
        
        // Set Save World callback - Quick save to current file
        m_menuBar->setSaveWorldCallback([this, world]() {
            LOG_INFO_C("Quick save triggered", "EditorManager");
            
            // If we have a current world path, save directly
            if (!m_currentWorldPath.empty() && m_worldSerializer && world) {
                if (m_worldSerializer->saveWorld(world, m_currentWorldPath)) {
                    LOG_INFO_C("World saved successfully to: " + m_currentWorldPath, "EditorManager");
                } else {
                    LOG_ERROR_C("Failed to save world to: " + m_currentWorldPath, "EditorManager");
                    
                    m_windowsDialogManager->showMessageBox(
                        "Save World Error",
                        "Failed to save world file.\nPlease check the console for details.",
                        MessageBoxButtons::OK,
                        MessageBoxIcon::Error
                    );
                }
            } else {
                // No current path, trigger Save As dialog
                LOG_INFO_C("No current world path, opening Save As dialog", "EditorManager");
                
                std::vector<FileFilter> filters = {
                    {"Fresh World Files", "*.world"},
                    {"All Files", "*.*"}
                };
                
                std::string savePath = m_windowsDialogManager->showSaveFileDialog(
                    "Save World", "NewWorld.world", filters);
                
                if (!savePath.empty()) {
                    // Ensure .world extension
                    if (savePath.find(".world") == std::string::npos) {
                        savePath += ".world";
                    }
                    
                    if (m_worldSerializer && world) {
                        if (m_worldSerializer->saveWorld(world, savePath)) {
                            m_currentWorldPath = savePath;
                            LOG_INFO_C("World saved successfully to: " + savePath, "EditorManager");
                        } else {
                            LOG_ERROR_C("Failed to save world to: " + savePath, "EditorManager");
                            
                            m_windowsDialogManager->showMessageBox(
                                "Save World Error",
                                "Failed to save world file.\nPlease check the console for details.",
                                MessageBoxButtons::OK,
                                MessageBoxIcon::Error
                            );
                        }
                    }
                }
            }
        });
        
        // Override Import Assets callback to use Windows dialog
        m_menuBar->setImportAssetsCallback([this]() {
            LOG_INFO_C("Opening Import Assets dialog", "EditorManager");
            
            std::vector<FileFilter> filters = {
                {"Image Files", "*.png;*.jpg;*.jpeg;*.bmp;*.tga"},
                {"3D Models", "*.obj;*.fbx;*.gltf;*.glb"},
                {"Audio Files", "*.wav;*.ogg;*.mp3"},
                {"All Files", "*.*"}
            };
            
            auto selectedFiles = m_windowsDialogManager->showOpenFileDialog(
                "Import Assets", filters, true); // multiselect = true
            
            if (!selectedFiles.empty()) {
                LOG_INFO_C("User selected " + std::to_string(selectedFiles.size()) + " file(s) to import", "EditorManager");
                
                // TODO: Call actual asset import function
                // For now, just show confirmation
                std::string fileList;
                for (const auto& file : selectedFiles) {
                    fileList += file + "\n";
                }
                
                m_windowsDialogManager->showMessageBox(
                    "Import Assets",
                    "Asset import will be implemented here.\nSelected files:\n" + fileList,
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );
            } else {
                LOG_INFO_C("Import Assets dialog cancelled", "EditorManager");
            }
        });
        
        LOG_INFO_C("File dialog callbacks configured successfully", "EditorManager");
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

#ifndef FRESH_WIN32_UI
    // Render ImGui menu bar and toolbar (only when not using native Win32 UI)
    // On Windows with FRESH_WIN32_UI, the native Win32MenuBar and Win32Toolbar are used instead
    if (m_menuBar) {
        m_menuBar->render();
    }

    if (m_toolbar) {
        m_toolbar->render();
    }
#endif // !FRESH_WIN32_UI

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

    // Render engine configuration panel if visible
    if (m_engineConfigPanel) {
        m_engineConfigPanel->render();
    }

#ifndef FRESH_WIN32_UI
    // Render ImGui hotbar (shown in play mode)
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
        // TODO: Get paste position from camera/cursor
        // For now, paste at origin as placeholder
        m_selectionManager->pasteFromClipboard(glm::ivec3(0, 64, 0), m_world);
        LOG_INFO_C("Pasted clipboard content", "EditorManager");
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
}

void EditorManager::deselectAll()
{
    if (m_sceneHierarchy) {
        m_sceneHierarchy->deselectAll();
        LOG_INFO_C("Deselected all", "EditorManager");
    }
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
            "Save World As", filters, "world");
        
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
    LOG_INFO_C("Save World As not implemented on this platform", "EditorManager");
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
    LOG_INFO_C("Load World not implemented on this platform", "EditorManager");
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
            if (m_world) {
                // Clear existing world by unloading all chunks
                auto& chunks = m_world->getChunks();
                std::vector<ChunkPos> chunksToUnload;
                for (const auto& [pos, chunk] : chunks) {
                    chunksToUnload.push_back(pos);
                }
                for (const auto& pos : chunksToUnload) {
                    m_world->unloadChunk(pos);
                }
                
                m_currentWorldPath.clear();
                LOG_INFO_C("New world created", "EditorManager");
                
                m_windowsDialogManager->showMessageBox(
                    "New World",
                    "New world created successfully!",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );
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
}

void EditorManager::toggleInspector()
{
    m_showInspector = !m_showInspector;
    LOG_INFO_C("Inspector toggled: " + std::string(m_showInspector ? "shown" : "hidden"), "EditorManager");
}

void EditorManager::toggleContentBrowser()
{
    m_showContentBrowser = !m_showContentBrowser;
    LOG_INFO_C("Content Browser toggled: " + std::string(m_showContentBrowser ? "shown" : "hidden"), "EditorManager");
}

void EditorManager::toggleConsole()
{
    m_showConsole = !m_showConsole;
    LOG_INFO_C("Console toggled: " + std::string(m_showConsole ? "shown" : "hidden"), "EditorManager");
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

void EditorManager::showImportAssets()
{
    if (m_contentBrowser) {
        m_contentBrowser->showImportDialog();
        LOG_INFO_C("Import assets dialog shown", "EditorManager");
    }
}

} // namespace fresh
