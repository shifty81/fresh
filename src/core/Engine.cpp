#include "core/Engine.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ai/AISystem.h"
#include "core/Logger.h"
#include "core/Window.h"
#include "editor/EditorGUI.h"
#include "editor/EditorManager.h"
#include "editor/WorldEditor.h"
#include "gameplay/Player.h"
#include "generation/TerrainGenerator.h"
#include "input/InputManager.h"
#include "interaction/VoxelInteraction.h"
#include "physics/PhysicsSystem.h"
#include "renderer/GraphicsAPI.h"
#include "renderer/RenderContext.h"
#include "serialization/WorldSerializer.h"
#include "ui/MainMenu.h"
#include "ui/MainMenuPanel.h"
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

// Rendering constants
namespace
{
constexpr float MAX_INTERACTION_DISTANCE = 5.0f;
constexpr float CROSSHAIR_SIZE = 0.02f;
constexpr float CROSSHAIR_LINE_WIDTH = 2.0f;
constexpr int SHADER_INFO_LOG_SIZE = 512;
#ifdef FRESH_OPENGL_SUPPORT
const char* VOXEL_VERTEX_SHADER = "shaders/voxel.vert";
const char* VOXEL_FRAGMENT_SHADER = "shaders/voxel.frag";
const char* CROSSHAIR_VERTEX_SHADER = "shaders/crosshair.vert";
const char* CROSSHAIR_FRAGMENT_SHADER = "shaders/crosshair.frag";
#endif
} // namespace

Engine::Engine() : m_running(false), m_inGame(false), m_selectedBlockType(VoxelType::Stone) {}

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
    m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
    if (!m_window->initialize(useOpenGL)) {
        std::cerr << "Failed to initialize window" << std::endl;
        LOG_ERROR_C("Failed to initialize window", "Engine");
        return false;
    }
    std::cout << "Window created" << std::endl;
    LOG_INFO_C("Window created", "Engine");

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
    m_inputManager = std::make_unique<InputManager>();
    m_inputManager->initialize(m_window->getHandle());
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

void Engine::createNewWorld(const std::string& name, int seed)
{
    std::cout << "\nCreating new world: " << name << " (seed: " << seed << ")" << std::endl;

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
    std::string savePath = "saves/" + name + ".world";
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
    std::string loadPath = "saves/" + name + ".world";

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
    // Re-initialize editor manager with the world
    if (m_editorManager && m_world && m_worldEditor) {
        // Update the editor manager with world and world editor references
        m_editorManager->shutdown();
        if (!m_editorManager->initialize(m_window.get(), m_renderer.get(), m_world.get(),
                                         m_worldEditor.get(), m_inputManager.get())) {
            LOG_ERROR_C("Failed to re-initialize editor manager with world", "Engine");
            LOG_WARNING_C("Editor will be unavailable. Game will continue without editor UI.", "Engine");
            std::cerr << "Warning: Editor manager re-initialization failed. Continuing without editor UI." << std::endl;
        } else {
            m_editorManager->setVisible(true);
            std::cout << "Editor manager updated with world" << std::endl;
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

    // Switch to game mode and hide the editor/menu
    m_inputManager->setInputMode(InputMode::GameMode);
#ifdef FRESH_IMGUI_AVAILABLE
    // Hide the editor manager (including main menu) when entering game
    if (m_editorManager) {
        m_editorManager->setVisible(false);
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
                                       mainMenuPanel->getWorldSeed());
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

    // Clean up GLFW callback user data
    m_callbackUserData.reset();

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

        // Toggle editor with 'T' key (only if GUI doesn't want keyboard)
        if (!guiCapturesKeyboard &&
            m_inputManager->isActionJustPressed(InputAction::ToggleEditor)) {
#ifdef FRESH_IMGUI_AVAILABLE
            if (m_editorManager) {
                m_editorManager->toggle();
                // Switch to UI mode when editor is visible, game mode when hidden
                if (m_editorManager->isVisible()) {
                    m_inputManager->setInputMode(InputMode::UIMode);
                } else {
                    m_inputManager->setInputMode(InputMode::GameMode);
                }
                LOG_INFO_C("Editor toggled", "Engine");
            }
#else
            LOG_INFO_C("Editor not available (ImGui required)", "Engine");
#endif
        }

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

    // Check if we're in game mode (not UI/menu mode)
    bool inGameMode = (m_inputManager->getInputMode() == InputMode::GameMode);

    // Handle player input (only if GUI doesn't capture input and we're in game mode)
    if (m_player && m_inputManager && !guiCapturesMouse && !guiCapturesKeyboard && inGameMode) {
        m_player->handleInput(*m_inputManager, deltaTime);

        // Handle mouse movement for camera
        glm::vec2 mouseDelta = m_inputManager->getMouseDelta();
        if (glm::length(mouseDelta) > 0.0f) {
            m_player->handleMouseMovement(mouseDelta.x, mouseDelta.y);
        }

        // Handle block placement/breaking
        if (m_voxelInteraction) {
            // Perform raycast to find targeted block
            RayHit hit =
                m_voxelInteraction->performRaycast(m_player->getCamera(), MAX_INTERACTION_DISTANCE);

            // Left click to break block
            if (m_inputManager->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && hit.hit) {
                m_voxelInteraction->breakBlock(hit);
            }

            // Right click to place block
            if (m_inputManager->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) && hit.hit) {
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

    GLFWwindow* window = m_window->getHandle();

    // Create user data for callbacks
    m_callbackUserData = std::make_unique<CallbackUserData>(m_inputManager.get(), m_window.get());
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
}

#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)

std::string Engine::loadShaderFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR_C("Failed to open shader file: " + filepath, "Engine");
        LOG_ERROR_C("Make sure to run the executable from the build directory where shaders/ folder exists", "Engine");
        LOG_ERROR_C("The shaders are copied to: <build_directory>/shaders/", "Engine");
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
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

} // namespace fresh
