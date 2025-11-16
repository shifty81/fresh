#include "core/Engine.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <limits.h>
#else
    #include <unistd.h>
    #include <linux/limits.h>
#endif

#ifdef _WIN32
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
    #include "ui/native/Win32MenuBar.h"
    #include "ui/native/Win32Toolbar.h"
#else
    #include <GLFW/glfw3.h>
    #include "core/Window.h"
    #include "input/InputManager.h"
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ai/AISystem.h"
#include "core/Logger.h"
#include "editor/EditorGUI.h"
#include "editor/EditorManager.h"
#include "editor/WorldEditor.h"
#include "gameplay/Player.h"
#include "generation/TerrainGenerator.h"
#include "interaction/VoxelInteraction.h"
#include "physics/PhysicsSystem.h"
#include "renderer/GraphicsAPI.h"
#include "renderer/RenderContext.h"
#include "serialization/WorldSerializer.h"
#include "ui/EditorToolbar.h"
#include "ui/HotbarPanel.h"
#include "ui/MainMenu.h"
#include "ui/MainMenuPanel.h"
#include "ui/VoxelToolPalette.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"
#include "voxel/VoxelWorld.h"

#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    #include <GL/glew.h>
#endif

#ifdef _WIN32
    #include "renderer/backends/DirectX11RenderContext.h"
    #include "renderer/backends/DirectX12RenderContext.h"
#endif

namespace fresh
{

// Platform-specific key code definitions
#ifdef _WIN32
    // Win32 virtual key codes
    constexpr int KEY_0 = '0';
    constexpr int KEY_1 = '1';
    constexpr int KEY_2 = '2';
    constexpr int KEY_3 = '3';
    constexpr int KEY_4 = '4';
    constexpr int KEY_5 = '5';
    constexpr int KEY_6 = '6';
    constexpr int KEY_7 = '7';
    constexpr int KEY_8 = '8';
    constexpr int KEY_9 = '9';
    constexpr int KEY_F = 'F';
    constexpr int KEY_Y = 'Y';
    constexpr int KEY_Z = 'Z';
    constexpr int KEY_LEFT_CONTROL = VK_LCONTROL;
    constexpr int KEY_RIGHT_CONTROL = VK_RCONTROL;
    constexpr int KEY_LEFT_SHIFT = VK_LSHIFT;
    constexpr int KEY_RIGHT_SHIFT = VK_RSHIFT;
    constexpr int MOUSE_BUTTON_LEFT = VK_LBUTTON;
    constexpr int MOUSE_BUTTON_RIGHT = VK_RBUTTON;
#else
    // GLFW key codes
    constexpr int KEY_0 = GLFW_KEY_0;
    constexpr int KEY_1 = GLFW_KEY_1;
    constexpr int KEY_2 = GLFW_KEY_2;
    constexpr int KEY_3 = GLFW_KEY_3;
    constexpr int KEY_4 = GLFW_KEY_4;
    constexpr int KEY_5 = GLFW_KEY_5;
    constexpr int KEY_6 = GLFW_KEY_6;
    constexpr int KEY_7 = GLFW_KEY_7;
    constexpr int KEY_8 = GLFW_KEY_8;
    constexpr int KEY_9 = GLFW_KEY_9;
    constexpr int KEY_F = GLFW_KEY_F;
    constexpr int KEY_Y = GLFW_KEY_Y;
    constexpr int KEY_Z = GLFW_KEY_Z;
    constexpr int KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    constexpr int KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    constexpr int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    constexpr int KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    constexpr int MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
    constexpr int MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
#endif

// Rendering constants
namespace
{
constexpr float MAX_INTERACTION_DISTANCE = 5.0f;
constexpr float CROSSHAIR_SIZE = 0.02f;
constexpr float CROSSHAIR_LINE_WIDTH = 2.0f;
constexpr int SHADER_INFO_LOG_SIZE = 512;
#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
[[maybe_unused]] const char* VOXEL_VERTEX_SHADER = "shaders/voxel.vert";
[[maybe_unused]] const char* VOXEL_FRAGMENT_SHADER = "shaders/voxel.frag";
[[maybe_unused]] const char* CROSSHAIR_VERTEX_SHADER = "shaders/crosshair.vert";
[[maybe_unused]] const char* CROSSHAIR_FRAGMENT_SHADER = "shaders/crosshair.frag";
#endif

/**
 * @brief Get the directory path where the executable is located
 * @return Path to the executable's directory, or empty string on failure
 */
std::string getExecutableDirectory()
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH) {
        return "";
    }
    std::string exePath(buffer);
    // Find last slash or backslash
    size_t lastSlash = exePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }
    return "";
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = PATH_MAX;
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        return "";
    }
    std::string exePath(buffer);
    size_t lastSlash = exePath.find_last_of('/');
    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }
    return "";
#else // Linux
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX - 1);
    if (length == -1) {
        return "";
    }
    buffer[length] = '\0';
    std::string exePath(buffer);
    size_t lastSlash = exePath.find_last_of('/');
    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash);
    }
    return "";
#endif
}

} // namespace

Engine::Engine() : m_running(false), m_inGame(false), m_selectedBlockType(VoxelType::Stone), 
                   m_lastCursorCaptured(false), m_rightMouseHeldForCamera(false) {}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    std::cout << "Initializing Fresh Voxel Engine..." << std::endl;
    LOG_INFO_C("Initializing Fresh Voxel Engine...", "Engine");

    // Create renderer using the abstraction layer first to determine API
    // Auto-select best graphics API for the platform
    m_renderer = RenderContextFactory::createBest();
    if (!m_renderer) {
        std::cerr << "Failed to create render context" << std::endl;
        LOG_ERROR_C("Failed to create render context", "Engine");
        return false;
    }
    std::cout << "Render context created: " << getGraphicsAPIName(m_renderer->getAPI())
              << std::endl;
    LOG_INFO_C("Render context created: " + std::string(getGraphicsAPIName(m_renderer->getAPI())),
               "Engine");

    // Determine if we need OpenGL context for the window based on selected API
    bool useOpenGL = (m_renderer->getAPI() == GraphicsAPI::OpenGL);

    // Create window FIRST so we can show the GUI
    m_window = std::make_unique<WindowType>(1280, 720, "Fresh Voxel Engine");
    if (!m_window->initialize(useOpenGL)) {
        std::cerr << "Failed to initialize window" << std::endl;
        LOG_ERROR_C("Failed to initialize window", "Engine");
        return false;
    }
    std::cout << "Window created" << std::endl;
    LOG_INFO_C("Window created", "Engine");

#ifdef _WIN32
    // Set up native Win32 menu bar
    setupNativeMenuBar();
    // Set up native Win32 toolbar
    setupNativeToolbar();
#endif

    // Initialize renderer with the window
    if (!m_renderer->initialize(m_window.get())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        LOG_ERROR_C("Failed to initialize renderer", "Engine");
        return false;
    }
    std::cout << "Renderer initialized with " << getGraphicsAPIName(m_renderer->getAPI())
              << std::endl;
    LOG_INFO_C("Renderer initialized with " + std::string(getGraphicsAPIName(m_renderer->getAPI())),
               "Engine");

    // Create input manager and set up callbacks
    m_inputManager = std::make_unique<InputManagerType>();
#ifdef _WIN32
    // Win32: Initialize with window pointer and set up callbacks
    m_inputManager->initialize(m_window.get());
    
    m_window->setKeyCallback([this](int vk, bool isDown) {
        m_inputManager->processKeyEvent(vk, isDown);
    });
    
    m_window->setMouseMoveCallback([this](int x, int y) {
        m_inputManager->processMouseMovement(x, y);
    });
    
    m_window->setMouseButtonCallback([this](int button, bool isDown) {
        m_inputManager->processMouseButton(button, isDown);
    });
#else
    // GLFW: Initialize with window handle
    m_inputManager->initialize(m_window->getHandle());
#endif
    m_inputManager->setInputMode(InputMode::UIMode); // Start in UI mode for menu
    setupInputCallbacks();
    std::cout << "Input manager initialized" << std::endl;
    LOG_INFO_C("Input manager initialized", "Engine");

    // Create console-based main menu for backward compatibility
    m_mainMenu = std::make_unique<MainMenu>();
    if (!m_mainMenu->initialize()) {
        std::cerr << "Failed to initialize main menu" << std::endl;
        LOG_ERROR_C("Failed to initialize main menu", "Engine");
        return false;
    }
    std::cout << "Main menu initialized" << std::endl;
    LOG_INFO_C("Main menu initialized", "Engine");

#ifdef FRESH_IMGUI_AVAILABLE
    // Create comprehensive editor manager (requires ImGui) - show immediately
    m_editorManager = std::make_unique<EditorManager>();
    // Initialize with nullptr for world and worldEditor initially
    if (!m_editorManager->initialize(m_window.get(), m_renderer.get(), nullptr, nullptr,
                                     m_inputManager.get())) {
        std::cerr << "Failed to initialize editor manager" << std::endl;
        LOG_ERROR_C("Failed to initialize editor manager", "Engine");
        return false;
    }
    m_editorManager->setVisible(true); // Show editor immediately
    std::cout << "Editor manager initialized" << std::endl;
    LOG_INFO_C("Editor manager initialized", "Engine");
#endif

    m_running = true;
    m_inGame = false; // Start in menu mode
    return true;
}

void Engine::createNewWorld(const std::string& name, int seed, bool is3D)
{
    std::cout << "\nCreating new " << (is3D ? "3D" : "2D") << " world: " << name 
              << " (seed: " << seed << ")" << std::endl;

    // Create voxel world
    m_world = std::make_unique<VoxelWorld>();
    if (!m_world->initialize()) {
        std::cerr << "Failed to initialize voxel world" << std::endl;
        return;
    }
    std::cout << "Voxel world initialized" << std::endl;

    // Set custom seed for terrain generator
    m_world->setSeed(seed);

    // Generate initial chunks around spawn
    std::cout << "Generating initial terrain..." << std::endl;
    int chunkRadius = 3;
    int totalChunks = (2 * chunkRadius + 1) * (2 * chunkRadius + 1);
    int chunksGenerated = 0;

    for (int x = -chunkRadius; x <= chunkRadius; ++x) {
        for (int z = -chunkRadius; z <= chunkRadius; ++z) {
            m_world->loadChunk(ChunkPos(x, z));
            chunksGenerated++;

            if (chunksGenerated % 10 == 0 || chunksGenerated == totalChunks) {
                std::cout << "  Generated " << chunksGenerated << "/" << totalChunks << " chunks..."
                          << std::endl;
            }
        }
    }

    std::cout << "World '" << name << "' created successfully!" << std::endl;
    std::cout << "Total chunks: " << m_world->getChunks().size() << std::endl;

    // Now initialize game systems after world is created
    initializeGameSystems();

    // Save the world
    WorldSerializer serializer;
    std::string savePath = std::string("saves/") + name + ".world";
    if (serializer.saveWorld(m_world.get(), savePath)) {
        std::cout << "World saved to: " << savePath << std::endl;
    }
}

void Engine::loadWorld(const std::string& name)
{
    std::cout << "\nLoading world: " << name << std::endl;

    // Create voxel world
    m_world = std::make_unique<VoxelWorld>();
    if (!m_world->initialize()) {
        std::cerr << "Failed to initialize voxel world" << std::endl;
        return;
    }

    // Load the world from file
    WorldSerializer serializer;
    std::string loadPath = std::string("saves/") + name + ".world";

    if (serializer.loadWorld(m_world.get(), loadPath)) {
        std::cout << "World '" << name << "' loaded successfully!" << std::endl;
    } else {
        std::cerr << "Failed to load world. Creating new world instead..." << std::endl;
        // Fall back to creating a new world
        createNewWorld(name, static_cast<int>(std::time(nullptr)));
        return; // createNewWorld will call initializeGameSystems
    }

    // Now initialize game systems after world is loaded
    initializeGameSystems();
}

void Engine::initializeGameSystems()
{
    // This function sets up all game systems after a world is created/loaded

    // Create physics system
    if (!m_physics) {
        m_physics = std::make_unique<PhysicsSystem>();
        if (!m_physics->initialize()) {
            LOG_ERROR_C("Failed to initialize physics system", "Engine");
            return;
        }
        std::cout << "Physics system initialized" << std::endl;
    }

    // Create AI system
    if (!m_aiSystem) {
        m_aiSystem = std::make_unique<AISystem>();
        if (!m_aiSystem->initialize()) {
            LOG_ERROR_C("Failed to initialize AI system", "Engine");
            return;
        }
        std::cout << "AI system initialized" << std::endl;
    }

    // Create/update world editor
    if (!m_worldEditor) {
        m_worldEditor = std::make_unique<WorldEditor>();
    }
    if (m_world) {
        m_worldEditor->initialize(m_world.get());
        m_worldEditor->setEnabled(true);
        std::cout << "World editor initialized" << std::endl;
    }

    // Create legacy editor GUI (for backward compatibility)
    if (!m_editor) {
        m_editor = std::make_unique<EditorGUI>();
        if (m_worldEditor && !m_editor->initialize(m_worldEditor.get())) {
            LOG_ERROR_C("Failed to initialize editor GUI", "Engine");
        } else {
            m_editor->setVisible(false); // Hide by default
            std::cout << "Editor GUI initialized" << std::endl;
        }
    }

#ifdef FRESH_IMGUI_AVAILABLE
    // Update editor manager with the world instead of shutting down and re-initializing
    if (m_editorManager && m_world && m_worldEditor) {
        // Update the editor manager with world and world editor references
        if (!m_editorManager->updateWorld(m_world.get(), m_worldEditor.get())) {
            LOG_ERROR_C("Failed to update editor manager with world", "Engine");
            LOG_WARNING_C("Editor will be unavailable. Game will continue without editor UI.", "Engine");
            std::cerr << "Warning: Editor manager update failed. Continuing without editor UI." << std::endl;
        } else {
            m_editorManager->setVisible(true);
            std::cout << "Editor manager updated with world" << std::endl;
            
            // Set up play mode callback to toggle between free-flight editor mode and play mode
            auto* toolbar = m_editorManager->getToolbar();
            if (toolbar) {
                toolbar->setPlayModeCallback([this](EditorToolbar::PlayMode mode) {
                    if (m_player) {
                        switch (mode) {
                            case EditorToolbar::PlayMode::Playing:
                                // Enter play mode - switch to normal character with physics
                                m_player->setFreeFlightMode(false);
                                // In play mode, use game mode (cursor hidden unless Alt held)
                                m_inputManager->setInputMode(InputMode::GameMode);
                                // Show hotbar in play mode
                                if (m_editorManager && m_editorManager->getHotbar()) {
                                    m_editorManager->getHotbar()->setVisible(true);
                                }
                                LOG_INFO_C("Entered Play Mode - normal character gameplay", "Engine");
                                break;
                            case EditorToolbar::PlayMode::Stopped:
                            case EditorToolbar::PlayMode::Paused:
                                // Enter editor mode - switch to free-flying camera
                                m_player->setFreeFlightMode(true);
                                // In editor mode, keep UI mode (cursor visible for editor interaction)
                                m_inputManager->setInputMode(InputMode::UIMode);
                                // Hide hotbar in editor mode
                                if (m_editorManager && m_editorManager->getHotbar()) {
                                    m_editorManager->getHotbar()->setVisible(false);
                                }
                                LOG_INFO_C("Entered Editor Mode - free-flying camera", "Engine");
                                break;
                        }
                    }
                });
            }
            
            // Connect voxel tool palette voxel type changes to engine's selected block type
            auto* voxelToolPalette = m_editorManager->getVoxelToolPalette();
            if (voxelToolPalette) {
                voxelToolPalette->setVoxelTypeCallback([this](VoxelType type) {
                    m_selectedBlockType = type;
                    LOG_INFO_C("Selected block type from tool palette: " + 
                              std::to_string(static_cast<int>(type)), "Engine");
                });
            }
        }
    }
#endif

    // Initialize rendering based on API
#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    if (m_renderer->getAPI() == GraphicsAPI::OpenGL) {
        initializeRendering();
    }
#endif

    // DirectX 11 rendering confirmation
    if (m_renderer->getAPI() == GraphicsAPI::DirectX11) {
        std::cout << "DirectX 11 voxel rendering ready" << std::endl;
    }

    // DirectX 12 confirmation
    if (m_renderer->getAPI() == GraphicsAPI::DirectX12) {
        std::cout << "DirectX 12 voxel rendering ready" << std::endl;
    }

    // Create voxel interaction system
    if (!m_voxelInteraction) {
        m_voxelInteraction = std::make_unique<VoxelInteraction>();
    }
    m_voxelInteraction->initialize(m_world.get());
    std::cout << "Voxel interaction initialized" << std::endl;

    // Create player
    if (!m_player) {
        m_player = std::make_unique<Player>();
    }
    m_player->setWorld(m_world.get());
    m_player->setPosition(glm::vec3(0.0f, 80.0f, 0.0f));
    std::cout << "Player initialized" << std::endl;

    // Initialize hotbar with common block types
#ifdef FRESH_IMGUI_AVAILABLE
    if (m_editorManager && m_editorManager->getHotbar()) {
        auto* hotbar = m_editorManager->getHotbar();
        
        // Set up common block types in hotbar slots
        std::vector<VoxelType> commonBlocks = {
            VoxelType::Stone,    // Slot 1
            VoxelType::Dirt,     // Slot 2
            VoxelType::Grass,    // Slot 3
            VoxelType::Wood,     // Slot 4
            VoxelType::Sand,     // Slot 5
            VoxelType::Brick,    // Slot 6
            VoxelType::Glass,    // Slot 7
            VoxelType::Leaves,   // Slot 8
            VoxelType::Water,    // Slot 9
            VoxelType::Snow      // Slot 0
        };
        
        for (size_t i = 0; i < commonBlocks.size() && i < HotbarPanel::HOTBAR_SIZE; ++i) {
            HotbarPanel::HotbarSlot slot;
            slot.isEmpty = false;
            slot.voxelType = commonBlocks[i];
            slot.count = 1;
            hotbar->setSlot(static_cast<int>(i), slot);
        }
        
        // Connect hotbar slot selection to block type change
        hotbar->setSlotActivatedCallback([this](int slotIndex) {
            if (m_editorManager && m_editorManager->getHotbar()) {
                const auto& slot = m_editorManager->getHotbar()->getSlot(slotIndex);
                if (!slot.isEmpty) {
                    m_selectedBlockType = slot.voxelType;
                    LOG_INFO_C("Selected block type: " + std::to_string(static_cast<int>(m_selectedBlockType)), 
                              "Engine");
                }
            }
        });
        
        // Set initial selected block to match slot 0 (Stone)
        if (!commonBlocks.empty()) {
            m_selectedBlockType = commonBlocks[0];
            hotbar->setSelectedSlot(0);
        }
        
        LOG_INFO_C("Hotbar initialized with " + std::to_string(commonBlocks.size()) + " block types", 
                  "Engine");
    }
#endif

    // Keep editor visible and stay in UI mode (editor-first approach)
    m_inputManager->setInputMode(InputMode::UIMode);
#ifdef FRESH_IMGUI_AVAILABLE
    // Keep the editor manager visible when entering game
    if (m_editorManager) {
        m_editorManager->setVisible(true);
    }
#endif
    m_inGame = true;
}

void Engine::run()
{
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float fpsTimer = 0.0f;

    // Frame timing constants
    const float TARGET_FPS = 60.0f;
    const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS; // 16.67ms
    const float MAX_DELTA_TIME = 0.1f;                 // Cap at 100ms to prevent physics issues

    while (m_running) {
        // Check if window should close
        if (m_window && m_window->shouldClose()) {
            m_running = false;
            break;
        }

        // Poll events
        if (m_window) {
            m_window->pollEvents();
        }

        // If not in game yet, show GUI menu and handle world creation
        if (!m_inGame) {
#ifdef FRESH_IMGUI_AVAILABLE
            // Set clear color and begin frame for menu rendering
            if (m_renderer) {
                m_renderer->clearColor(0.53f, 0.81f, 0.92f, 1.0f);
                m_renderer->beginFrame();
                m_renderer->setViewport(0, 0, m_renderer->getSwapchainWidth(),
                                        m_renderer->getSwapchainHeight());
            }

            // Render the editor with main menu panel
            if (m_editorManager && m_editorManager->isInitialized()) {
                m_editorManager->beginFrame();
                m_editorManager->render();

                // Check if user wants to create or load a world from GUI
                auto* mainMenuPanel = m_editorManager->getMainMenuPanel();
                bool worldActionRequested = false;
                if (mainMenuPanel) {
                    if (mainMenuPanel->shouldExit()) {
                        // End the current ImGui frame before exiting to avoid frame lifecycle issues
                        m_editorManager->endFrame();
                        
                        // Exit the application
                        m_running = false;
                        mainMenuPanel->clearFlags();
                        worldActionRequested = true;
                    } else if (mainMenuPanel->shouldCreateNewWorld()) {
                        // End the current ImGui frame before world creation to avoid frame lifecycle issues
                        m_editorManager->endFrame();
                        
                        createNewWorld(mainMenuPanel->getNewWorldName(),
                                       mainMenuPanel->getWorldSeed(),
                                       mainMenuPanel->isWorld3D());
                        mainMenuPanel->clearFlags();
                        worldActionRequested = true;
                    } else if (mainMenuPanel->shouldLoadWorld()) {
                        // End the current ImGui frame before world loading to avoid frame lifecycle issues
                        m_editorManager->endFrame();
                        
                        loadWorld(mainMenuPanel->getLoadWorldName());
                        mainMenuPanel->clearFlags();
                        worldActionRequested = true;
                    }
                }

                // Only call endFrame if we didn't already call it above
                if (!worldActionRequested) {
                    m_editorManager->endFrame();
                }
            }

            // End frame
            if (m_renderer) {
                m_renderer->endFrame();
            }
#else
            // Fallback to console menu if ImGui not available
            m_mainMenu->render();
            if (m_mainMenu->shouldCreateNewWorld()) {
                createNewWorld(m_mainMenu->getNewWorldName(), m_mainMenu->getWorldSeed());
                m_mainMenu->clearFlags();
            } else if (m_mainMenu->shouldLoadWorld()) {
                loadWorld(m_mainMenu->getLoadWorldName());
                m_mainMenu->clearFlags();
            }
#endif
            continue;
        }

        // Normal game loop

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Cap deltaTime to prevent huge jumps
        deltaTime = std::min(deltaTime, MAX_DELTA_TIME);

        // FPS counter
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f) {
            std::cout << "FPS: " << frameCount;
            if (m_world) {
                std::cout << " | Chunks: " << m_world->getChunks().size();
            }
            if (m_player) {
                glm::vec3 pos = m_player->getPosition();
                std::cout << " | Pos: (" << static_cast<int>(pos.x) << ", "
                          << static_cast<int>(pos.y) << ", " << static_cast<int>(pos.z) << ")";
            }
            std::cout << std::endl;
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        processInput();
        update(deltaTime);
        render();

        // Frame rate limiting - hybrid sleep approach for better accuracy
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(frameEndTime - currentTime).count();

        if (frameTime < TARGET_FRAME_TIME) {
            float sleepTime = TARGET_FRAME_TIME - frameTime;

            // Sleep for most of the remaining time (leave 1ms for spin-wait)
            if (sleepTime > 0.002f) { // 2ms threshold
                std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime - 0.001f));
            }

            // Busy-wait for the remaining time for precision
            while (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() -
                                                currentTime)
                       .count() < TARGET_FRAME_TIME) {
                // Spin-wait for precise timing
            }
        }
    }
}

void Engine::shutdown()
{
#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    // Cleanup OpenGL rendering resources
    if (m_renderer && m_renderer->getAPI() == GraphicsAPI::OpenGL) {
        shutdownRendering();
    }
#endif

    if (m_renderer) {
        m_renderer->waitIdle();
    }

#ifndef _WIN32
    // Clean up GLFW callback user data
    m_callbackUserData.reset();
#endif

#ifdef FRESH_IMGUI_AVAILABLE
    // Explicitly shutdown editor manager BEFORE resetting renderer
    // This ensures ImGuiContext can safely access render context during shutdown
    if (m_editorManager) {
        m_editorManager->shutdown();
        m_editorManager.reset();
    }
#endif

    m_player.reset();
    m_inputManager.reset();
    m_editor.reset();
    m_aiSystem.reset();
    m_physics.reset();
    m_world.reset();
    m_renderer.reset();
    m_window.reset();

    m_running = false;
}

void Engine::processInput()
{
    // Reset mouse delta from previous frame BEFORE polling new events
    if (m_inputManager) {
        m_inputManager->resetMouseDelta();
    }

    if (m_window) {
        m_window->pollEvents();
    }

    // Update input manager state (handles Alt-hold mode switching)
    if (m_inputManager) {
        m_inputManager->update();

        // Check if GUI wants input (only process world input if GUI doesn't need it)
#ifdef FRESH_IMGUI_AVAILABLE
        bool guiCapturesMouse = m_editorManager && m_editorManager->wantCaptureMouse();
        (void)guiCapturesMouse; // May be unused in this scope
        bool guiCapturesKeyboard = m_editorManager && m_editorManager->wantCaptureKeyboard();
#else
        bool guiCapturesMouse = false;
        (void)guiCapturesMouse; // Suppress unused warning
        bool guiCapturesKeyboard = false;
#endif

        // Handle hotbar key presses (1-0) when not in editor mode and GUI doesn't capture keyboard
#ifdef FRESH_IMGUI_AVAILABLE
        if (!guiCapturesKeyboard && m_editorManager && m_editorManager->getHotbar()) {
            auto* hotbar = m_editorManager->getHotbar();
            if (hotbar->isVisible()) {
                // Check keys 1-9 and 0
                for (int key = KEY_1; key <= KEY_9; ++key) {
                    if (m_inputManager->isKeyJustPressed(key)) {
                        hotbar->handleKeyPress(key);
                    }
                }
                if (m_inputManager->isKeyJustPressed(KEY_0)) {
                    hotbar->handleKeyPress(KEY_0);
                }
            }
        }
        
        // Handle keyboard shortcuts for editor actions (Ctrl+Z, Ctrl+Y, etc.)
        if (!guiCapturesKeyboard) {
            bool ctrlPressed = m_inputManager->isKeyPressed(KEY_LEFT_CONTROL) || 
                              m_inputManager->isKeyPressed(KEY_RIGHT_CONTROL);
            bool shiftPressed = m_inputManager->isKeyPressed(KEY_LEFT_SHIFT) || 
                               m_inputManager->isKeyPressed(KEY_RIGHT_SHIFT);
            
            // Ctrl+Z for Undo
            if (ctrlPressed && !shiftPressed && m_inputManager->isKeyJustPressed(KEY_Z)) {
                if (m_worldEditor && m_worldEditor->getTerraformingSystem()) {
                    if (m_worldEditor->getTerraformingSystem()->undo()) {
                        LOG_INFO_C("Undo performed (Ctrl+Z)", "Engine");
                    } else {
                        LOG_INFO_C("Nothing to undo", "Engine");
                    }
                }
            }
            
            // Ctrl+Y for Redo (or Ctrl+Shift+Z)
            if ((ctrlPressed && m_inputManager->isKeyJustPressed(KEY_Y)) ||
                (ctrlPressed && shiftPressed && m_inputManager->isKeyJustPressed(KEY_Z))) {
                if (m_worldEditor && m_worldEditor->getTerraformingSystem()) {
                    if (m_worldEditor->getTerraformingSystem()->redo()) {
                        LOG_INFO_C("Redo performed (Ctrl+Y)", "Engine");
                    } else {
                        LOG_INFO_C("Nothing to redo", "Engine");
                    }
                }
            }
        }
#endif

        // F key to toggle mouse cursor capture (camera freelook vs GUI mode)
        if (!guiCapturesKeyboard && m_inputManager->isKeyJustPressed(KEY_F)) {
            m_inputManager->toggleCursorCapture();
            m_userToggledCursor = true;  // Track that user explicitly toggled
            if (m_inputManager->isCursorCaptured()) {
                LOG_INFO_C("Mouse captured - Camera freelook enabled", "Engine");
            } else {
                LOG_INFO_C("Mouse released - GUI mode enabled", "Engine");
            }
        }

        // T key disabled - editor is always visible in new editor-first mode
        // Keeping this code commented out for reference
        // if (!guiCapturesKeyboard &&
        //     m_inputManager->isActionJustPressed(InputAction::ToggleEditor)) {
        // #ifdef FRESH_IMGUI_AVAILABLE
        //     if (m_editorManager) {
        //         m_editorManager->toggle();
        //         // Switch to UI mode when editor is visible, game mode when hidden
        //         if (m_editorManager->isVisible()) {
        //             m_inputManager->setInputMode(InputMode::UIMode);
        //         } else {
        //             m_inputManager->setInputMode(InputMode::GameMode);
        //         }
        //         LOG_INFO_C("Editor toggled", "Engine");
        //     }
        // #else
        //     LOG_INFO_C("Editor not available (ImGui required)", "Engine");
        // #endif
        // }

        // Allow ESC to close the window (only if GUI doesn't want keyboard)
        if (!guiCapturesKeyboard && m_inputManager->isActionJustPressed(InputAction::OpenMenu)) {
            // TODO: In future, show pause menu instead of exiting
            // For now, switch to UI mode when menu would open
            m_running = false;
        }
    }
}

void Engine::update(float deltaTime)
{
    if (!m_world) {
        return;
    }

    // Check if GUI wants input before processing player updates
#ifdef FRESH_IMGUI_AVAILABLE
    bool guiCapturesMouse = m_editorManager && m_editorManager->wantCaptureMouse();
    bool guiCapturesKeyboard = m_editorManager && m_editorManager->wantCaptureKeyboard();
#else
    bool guiCapturesMouse = false;
    bool guiCapturesKeyboard = false;
#endif

    // ========================================================================
    // UNREAL-STYLE MOUSE CONTROL SYSTEM
    // ========================================================================
    // In Unreal Engine:
    // - Right Mouse Button (RMB) held = Camera free look (cursor hidden & captured)
    // - RMB released = Cursor visible, can interact with UI/menus
    // - This prevents the cursor from "snapping" to camera control automatically
    // ========================================================================
    
    if (m_inputManager) {
        InputMode currentMode = m_inputManager->getInputMode();
        bool actualCursorCaptured = m_inputManager->isCursorCaptured();
        
        // Check if right mouse button is currently held (for Unreal-style camera control)
        bool rightMousePressed = m_inputManager->isMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        
        // Determine if we should capture cursor based on RMB state
        // Only capture when RMB is held AND GUI doesn't want the mouse
        bool shouldCaptureCursor = false;
        
        if (currentMode == InputMode::UIMode) {
            // In UI mode (editor), only capture cursor when:
            // 1. Right mouse button is held (Unreal-style free look)
            // 2. GUI is not capturing mouse (not hovering over UI elements)
            // 3. User hasn't explicitly toggled cursor with F key
            if (!m_userToggledCursor && !guiCapturesMouse) {
                shouldCaptureCursor = rightMousePressed;
                
                // Track if RMB is being used for camera control
                m_rightMouseHeldForCamera = rightMousePressed;
            } else if (m_userToggledCursor) {
                // User explicitly toggled with F key - respect their choice
                shouldCaptureCursor = actualCursorCaptured;
                
                // Clear user toggle if they start using GUI
                if (guiCapturesMouse) {
                    m_userToggledCursor = false;
                }
            }
        } else if (currentMode == InputMode::GameMode) {
            // In game mode, always capture cursor (traditional FPS style)
            shouldCaptureCursor = true;
            m_userToggledCursor = false;
        }
        
        // Only call setCursorMode if the state actually changed (prevents stuttering)
        if (shouldCaptureCursor != actualCursorCaptured) {
            m_inputManager->setCursorMode(shouldCaptureCursor);
            m_lastCursorCaptured = shouldCaptureCursor;
            
            // Log state changes for debugging
            if (shouldCaptureCursor) {
                LOG_INFO_C("Camera free look enabled (RMB held)", "Engine");
            } else {
                LOG_INFO_C("Cursor released for UI interaction", "Engine");
            }
        } else {
            m_lastCursorCaptured = actualCursorCaptured;
        }
    }

    // In editor-first mode, always allow player input unless GUI captures it
    // (GUI capture means user is actively using a text field, slider, etc.)
    
    // Handle player input (only if GUI doesn't capture input)
    if (m_player && m_inputManager && !guiCapturesMouse && !guiCapturesKeyboard) {
        m_player->handleInput(*m_inputManager, deltaTime);

        // UNREAL-STYLE: Handle mouse movement for camera ONLY when RMB is held
        // This prevents camera from moving when user is trying to use menus
        bool rightMousePressed = m_inputManager->isMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        if (!guiCapturesMouse && (rightMousePressed || m_rightMouseHeldForCamera)) {
            glm::vec2 mouseDelta = m_inputManager->getMouseDelta();
            if (glm::length(mouseDelta) > 0.0f) {
                m_player->handleMouseMovement(mouseDelta.x, mouseDelta.y);
            }
        }

        // Handle block placement/breaking
        if (!guiCapturesMouse && m_voxelInteraction) {
            // Perform raycast to find targeted block
            RayHit hit =
                m_voxelInteraction->performRaycast(m_player->getCamera(), MAX_INTERACTION_DISTANCE);

            // Left click to break block (only when NOT using RMB for camera)
            if (m_inputManager->isMouseButtonPressed(MOUSE_BUTTON_LEFT) && hit.hit && !rightMousePressed) {
                m_voxelInteraction->breakBlock(hit);
            }

            // Right click to place block (DISABLED during camera free look)
            // In Unreal style, RMB is reserved for camera control, not block placement
            // Block placement now requires Left-click with modifier or separate key
            // Note: Keeping legacy behavior for now, but RMB won't place when held for camera
            if (m_inputManager->isMouseButtonPressed(MOUSE_BUTTON_RIGHT) && hit.hit && !m_rightMouseHeldForCamera) {
                m_voxelInteraction->placeBlock(hit, m_selectedBlockType);
            }
        }
    }

    // Update player (physics, collision)
    if (m_player) {
        m_player->update(deltaTime);
    }

    // Update world with actual player position for chunk streaming
    if (m_player) {
        glm::vec3 playerPos = m_player->getPosition();
        m_world->update(WorldPos(static_cast<int>(playerPos.x), static_cast<int>(playerPos.y),
                                 static_cast<int>(playerPos.z)));
    } else {
        // Fallback to default position
        m_world->update(WorldPos(0, 64, 0));
    }

    // Update physics
    if (m_physics) {
        m_physics->update(deltaTime);
    }

    // Update AI
    if (m_aiSystem) {
        m_aiSystem->update(deltaTime);
    }

    // Update world editor
    if (m_worldEditor) {
        m_worldEditor->update(deltaTime);
    }
}

void Engine::render()
{
    if (!m_renderer) {
        return;
    }

    // Set clear color (sky blue) before beginning frame
    m_renderer->clearColor(0.53f, 0.81f, 0.92f, 1.0f);

    if (!m_renderer->beginFrame()) {
        return;
    }

    // Set viewport to match window size
    m_renderer->setViewport(0, 0, m_renderer->getSwapchainWidth(),
                            m_renderer->getSwapchainHeight());

#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    // Clear depth buffer
    if (m_renderer->getAPI() == GraphicsAPI::OpenGL) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render voxel world
        if (m_world) {
            renderVoxelWorld();
        }

        // Render crosshair overlay
        renderCrosshair();
    }
#endif

    // DirectX 11 rendering path
#ifdef _WIN32
    if (m_renderer->getAPI() == GraphicsAPI::DirectX11) {
        // Render voxel world using DirectX 11
        if (m_world && m_player) {
            auto* dx11Context = dynamic_cast<DirectX11RenderContext*>(m_renderer.get());
            if (dx11Context) {
                dx11Context->renderVoxelWorld(m_world.get(), m_player.get());
            }
        }
    }

    // DirectX 12 rendering path
    if (m_renderer->getAPI() == GraphicsAPI::DirectX12) {
        // Render voxel world using DirectX 12
        if (m_world && m_player) {
            auto* dx12Context = dynamic_cast<DirectX12RenderContext*>(m_renderer.get());
            if (dx12Context) {
                dx12Context->renderVoxelWorld(m_world.get(), m_player.get());
            }
        }
    }
#endif

#ifdef FRESH_IMGUI_AVAILABLE
    // Begin editor frame (ImGui) before rendering editor UI
    if (m_editorManager && m_editorManager->isInitialized()) {
        m_editorManager->beginFrame();
    }

    // Render comprehensive editor UI
    if (m_editorManager && m_editorManager->isInitialized() && m_editorManager->isVisible()) {
        m_editorManager->render();
    }

    // End editor frame (render ImGui)
    if (m_editorManager && m_editorManager->isInitialized()) {
        m_editorManager->endFrame();
    }
#endif // FRESH_IMGUI_AVAILABLE

    m_renderer->endFrame();
}

void Engine::setupInputCallbacks()
{
    if (!m_window || !m_inputManager) {
        return;
    }

#ifdef _WIN32
    // Win32: Callbacks are already set up in initialize() with lambdas
    // No additional setup needed
    LOG_INFO_C("Win32 input callbacks already configured", "Engine");
#else
    // GLFW: Set up traditional GLFW callbacks
    GLFWwindow* window = m_window->getHandle();

    // Create user data for callbacks
    m_callbackUserData = std::make_unique<CallbackUserData>();
    m_callbackUserData->inputManager = m_inputManager.get();
    m_callbackUserData->window = m_window.get();
    glfwSetWindowUserPointer(window, m_callbackUserData.get());

    // Keyboard callback
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        (void)scancode; // Unused
        (void)mods;     // Unused
        CallbackUserData* data = static_cast<CallbackUserData*>(glfwGetWindowUserPointer(win));
        if (data && data->inputManager) {
            data->inputManager->processKeyEvent(key, action);
        }
    });

    // Mouse movement callback
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        CallbackUserData* data = static_cast<CallbackUserData*>(glfwGetWindowUserPointer(win));
        if (data && data->inputManager) {
            data->inputManager->processMouseMovement(xpos, ypos);
        }
    });

    // Mouse button callback
    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        (void)mods; // Unused
        CallbackUserData* data = static_cast<CallbackUserData*>(glfwGetWindowUserPointer(win));
        if (data && data->inputManager) {
            data->inputManager->processMouseButton(button, action);
        }
    });

    // Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        CallbackUserData* data = static_cast<CallbackUserData*>(glfwGetWindowUserPointer(win));
        if (data && data->window) {
            // Update window dimensions (Window class handles this internally via its own tracking)
            // The renderer will need to handle the resize in its next frame
            (void)width;
            (void)height;
        }
    });
#endif
}

#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)

std::string Engine::loadShaderFile(const std::string& filepath)
{
    // Try multiple paths to find the shader file
    std::vector<std::string> searchPaths;
    
    // 1. Try relative to current working directory (for development builds)
    searchPaths.push_back(filepath);
    
    // 2. Try relative to executable directory (for installed/deployed builds)
    std::string exeDir = getExecutableDirectory();
    if (!exeDir.empty()) {
        searchPaths.push_back(exeDir + "/" + filepath);
    }
    
    // Try each path
    for (const auto& path : searchPaths) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            LOG_INFO_C("Loaded shader from: " + path, "Engine");
            return buffer.str();
        }
    }
    
    // If we get here, shader file was not found in any location
    LOG_ERROR_C("Failed to open shader file: " + filepath, "Engine");
    LOG_ERROR_C("Searched in the following locations:", "Engine");
    for (const auto& path : searchPaths) {
        LOG_ERROR_C("  - " + path, "Engine");
    }
    LOG_ERROR_C("Make sure the shaders/ folder exists in one of these locations", "Engine");
    return "";
}

GLuint Engine::compileShader(const std::string& source, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[SHADER_INFO_LOG_SIZE];
        glGetShaderInfoLog(shader, SHADER_INFO_LOG_SIZE, nullptr, infoLog);
        LOG_ERROR_C(std::string("Shader compilation failed: ") + infoLog, "Engine");
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Engine::createShaderProgram(const std::string& vertPath, const std::string& fragPath)
{
    std::string vertSource = loadShaderFile(vertPath);
    std::string fragSource = loadShaderFile(fragPath);

    if (vertSource.empty() || fragSource.empty()) {
        return 0;
    }

    GLuint vertShader = compileShader(vertSource, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragSource, GL_FRAGMENT_SHADER);

    if (vertShader == 0 || fragShader == 0) {
        if (vertShader)
            glDeleteShader(vertShader);
        if (fragShader)
            glDeleteShader(fragShader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Check linking
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[SHADER_INFO_LOG_SIZE];
        glGetProgramInfoLog(program, SHADER_INFO_LOG_SIZE, nullptr, infoLog);
        LOG_ERROR_C(std::string("Shader program linking failed: ") + infoLog, "Engine");
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

void Engine::initializeRendering()
{
    if (m_renderer->getAPI() != GraphicsAPI::OpenGL) {
        LOG_INFO_C("Rendering initialization skipped - not using OpenGL", "Engine");
        return;
    }

    // Create shader program
    m_shaderProgram = createShaderProgram(VOXEL_VERTEX_SHADER, VOXEL_FRAGMENT_SHADER);
    if (m_shaderProgram == 0) {
        LOG_ERROR_C("Failed to create voxel shader program", "Engine");
        return;
    }

    // Create crosshair shader program
    m_crosshairShader = createShaderProgram(CROSSHAIR_VERTEX_SHADER, CROSSHAIR_FRAGMENT_SHADER);
    if (m_crosshairShader == 0) {
        LOG_ERROR_C("Failed to create crosshair shader program", "Engine");
        // Continue without crosshair
    }

    LOG_INFO_C("OpenGL voxel rendering initialized", "Engine");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable back-face culling for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Create crosshair geometry (simple cross)
    float crosshairVertices[] = {// Horizontal line
                                 -CROSSHAIR_SIZE, 0.0f, CROSSHAIR_SIZE, 0.0f,
                                 // Vertical line
                                 0.0f, -CROSSHAIR_SIZE, 0.0f, CROSSHAIR_SIZE};

    glGenVertexArrays(1, &m_crosshairVAO);
    glGenBuffers(1, &m_crosshairVBO);

    glBindVertexArray(m_crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Engine::shutdownRendering()
{
    if (m_renderer->getAPI() != GraphicsAPI::OpenGL) {
        return;
    }

    // Delete crosshair buffers
    if (m_crosshairVAO) {
        glDeleteVertexArrays(1, &m_crosshairVAO);
        m_crosshairVAO = 0;
    }
    if (m_crosshairVBO) {
        glDeleteBuffers(1, &m_crosshairVBO);
        m_crosshairVBO = 0;
    }

    // Delete all VAOs/VBOs/EBOs
    for (auto& pair : m_chunkVAOs) {
        glDeleteVertexArrays(1, &pair.second);
    }
    for (auto& pair : m_chunkVBOs) {
        glDeleteBuffers(1, &pair.second);
    }
    for (auto& pair : m_chunkEBOs) {
        glDeleteBuffers(1, &pair.second);
    }

    m_chunkVAOs.clear();
    m_chunkVBOs.clear();
    m_chunkEBOs.clear();
    m_chunkIndexCounts.clear();

    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    if (m_crosshairShader) {
        glDeleteProgram(m_crosshairShader);
        m_crosshairShader = 0;
    }
}

void Engine::renderVoxelWorld()
{
    if (!m_world || !m_player || m_shaderProgram == 0) {
        return;
    }

    glUseProgram(m_shaderProgram);

    // Calculate matrices
    glm::mat4 view = m_player->getCamera().getViewMatrix();
    float aspectRatio = static_cast<float>(m_renderer->getSwapchainWidth()) /
                        static_cast<float>(m_renderer->getSwapchainHeight());
    glm::mat4 projection = m_player->getCamera().getProjectionMatrix(aspectRatio);

    // Render each chunk
    for (const auto& chunkPair : m_world->getChunks()) {
        const ChunkPos& chunkPos = chunkPair.first;
        Chunk* chunk = chunkPair.second.get();

        if (!chunk)
            continue;

        // Check if we need to upload mesh data to GPU
        // Upload if chunk is dirty (modified) OR if we haven't created GPU buffers yet
        bool needsUpload = chunk->isDirty() || (m_chunkVAOs.find(chunkPos) == m_chunkVAOs.end());

        if (needsUpload) {
            // Generate mesh if dirty
            if (chunk->isDirty()) {
                chunk->generateMesh();
                chunk->clearDirty();
            }

            // Update OpenGL buffers
            const auto& vertices = chunk->getMeshVertices();
            const auto& indices = chunk->getMeshIndices();

            if (vertices.empty() || indices.empty()) {
                continue;
            }

            // Create or get VAO/VBO/EBO
            GLuint& vao = m_chunkVAOs[chunkPos];
            GLuint& vbo = m_chunkVBOs[chunkPos];
            GLuint& ebo = m_chunkEBOs[chunkPos];

            if (vao == 0) {
                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);
                glGenBuffers(1, &ebo);
            }

            glBindVertexArray(vao);

            // Upload vertex data
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
                         GL_STATIC_DRAW);

            // Upload index data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),
                         GL_STATIC_DRAW);

            // Set up vertex attributes
            // Position (vec3)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

            // Normal (vec3)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                  (void*)(3 * sizeof(float)));

            glBindVertexArray(0);

            m_chunkIndexCounts[chunkPos] = indices.size();
        }

        // Render the chunk if it has a VAO
        if (m_chunkVAOs.find(chunkPos) != m_chunkVAOs.end()) {
            GLuint vao = m_chunkVAOs[chunkPos];
            size_t indexCount = m_chunkIndexCounts[chunkPos];

            if (indexCount == 0)
                continue;

            // Calculate model matrix (chunk position)
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,
                                   glm::vec3(chunkPos.x * CHUNK_SIZE, 0, chunkPos.z * CHUNK_SIZE));

            // Calculate MVP matrix
            glm::mat4 mvp = projection * view * model;

            // Set uniform
            GLint mvpLoc = glGetUniformLocation(m_shaderProgram, "modelViewProj");
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

            // Draw
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    glUseProgram(0);
}

void Engine::renderCrosshair()
{
    if (m_crosshairVAO == 0 || m_crosshairShader == 0) {
        return;
    }

    // Disable depth test for 2D overlay
    glDisable(GL_DEPTH_TEST);

    // Use crosshair shader
    glUseProgram(m_crosshairShader);

    // Draw crosshair
    glBindVertexArray(m_crosshairVAO);
    glLineWidth(CROSSHAIR_LINE_WIDTH);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);

    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
}

#endif // FRESH_OPENGL_SUPPORT && FRESH_GLEW_AVAILABLE

#ifdef _WIN32
void Engine::setupNativeMenuBar()
{
    if (!m_window) {
        LOG_ERROR_C("Cannot setup menu bar: window not initialized", "Engine");
        return;
    }

    auto* menuBar = m_window->getMenuBar();
    if (!menuBar) {
        LOG_ERROR_C("Failed to create menu bar", "Engine");
        return;
    }

    // Create File menu
    int fileMenu = menuBar->addMenu("File");
    menuBar->addMenuItem(fileMenu, "New World...", [this]() {
        LOG_INFO_C("New World menu item clicked", "Engine");
        // TODO: Show new world dialog
    });
    menuBar->addMenuItem(fileMenu, "Open World...", [this]() {
        LOG_INFO_C("Open World menu item clicked", "Engine");
        // TODO: Show open world dialog
    });
    menuBar->addMenuItem(fileMenu, "Save World", [this]() {
        LOG_INFO_C("Save World menu item clicked", "Engine");
        // TODO: Save current world
    });
    menuBar->addSeparator(fileMenu);
    menuBar->addMenuItem(fileMenu, "Exit", [this]() {
        LOG_INFO_C("Exit menu item clicked", "Engine");
        m_running = false;
    });

    // Create Edit menu
    int editMenu = menuBar->addMenu("Edit");
    menuBar->addMenuItem(editMenu, "Undo", [this]() {
        LOG_INFO_C("Undo menu item clicked", "Engine");
        // TODO: Implement undo
    });
    menuBar->addMenuItem(editMenu, "Redo", [this]() {
        LOG_INFO_C("Redo menu item clicked", "Engine");
        // TODO: Implement redo
    });
    menuBar->addSeparator(editMenu);
    menuBar->addMenuItem(editMenu, "Settings...", [this]() {
        LOG_INFO_C("Settings menu item clicked", "Engine");
        // TODO: Show settings dialog
    });

    // Create View menu
    int viewMenu = menuBar->addMenu("View");
    menuBar->addMenuItem(viewMenu, "Toggle Fullscreen", [this]() {
        LOG_INFO_C("Toggle Fullscreen menu item clicked", "Engine");
        // TODO: Toggle fullscreen
    });
    menuBar->addMenuItem(viewMenu, "Reset Camera", [this]() {
        LOG_INFO_C("Reset Camera menu item clicked", "Engine");
        // TODO: Reset camera position
    });

    // Create Help menu
    int helpMenu = menuBar->addMenu("Help");
    menuBar->addMenuItem(helpMenu, "Documentation", [this]() {
        LOG_INFO_C("Documentation menu item clicked", "Engine");
        // TODO: Open documentation
    });
    menuBar->addMenuItem(helpMenu, "About", [this]() {
        LOG_INFO_C("About menu item clicked", "Engine");
        // TODO: Show about dialog
    });

    LOG_INFO_C("Native Win32 menu bar initialized", "Engine");
}

void Engine::setupNativeToolbar()
{
    if (!m_window) {
        LOG_ERROR_C("Cannot setup toolbar: window not initialized", "Engine");
        return;
    }

    auto* toolbar = m_window->getToolbar();
    if (!toolbar) {
        LOG_ERROR_C("Failed to create toolbar", "Engine");
        return;
    }

    // Add toolbar buttons
    // Note: For actual icons, you would load them from resources
    // For now, using nullptr for icons - buttons will show as text
    
    // File operations
    toolbar->addButton(5001, "New", nullptr, [this]() {
        LOG_INFO_C("New button clicked", "Engine");
        // TODO: New world dialog
    });
    
    toolbar->addButton(5002, "Open", nullptr, [this]() {
        LOG_INFO_C("Open button clicked", "Engine");
        // TODO: Open world dialog
    });
    
    toolbar->addButton(5003, "Save", nullptr, [this]() {
        LOG_INFO_C("Save button clicked", "Engine");
        // TODO: Save current world
    });
    
    toolbar->addSeparator();
    
    // Edit operations
    toolbar->addButton(5004, "Undo", nullptr, [this]() {
        LOG_INFO_C("Undo button clicked", "Engine");
        // TODO: Implement undo
    });
    
    toolbar->addButton(5005, "Redo", nullptr, [this]() {
        LOG_INFO_C("Redo button clicked", "Engine");
        // TODO: Implement redo
    });
    
    toolbar->addSeparator();
    
    // View operations
    toolbar->addButton(5006, "Camera", nullptr, [this]() {
        LOG_INFO_C("Camera button clicked", "Engine");
        // TODO: Reset camera
    });
    
    toolbar->addButton(5007, "Fullscreen", nullptr, [this]() {
        LOG_INFO_C("Fullscreen button clicked", "Engine");
        // TODO: Toggle fullscreen
    });

    LOG_INFO_C("Native Win32 toolbar initialized", "Engine");
}
#endif

} // namespace fresh
