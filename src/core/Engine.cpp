#include "core/Engine.h"
#include "core/Window.h"
#include "core/Logger.h"
#include "renderer/RenderContext.h"
#include "renderer/GraphicsAPI.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "voxel/VoxelTypes.h"
#include "physics/PhysicsSystem.h"
#include "ai/AISystem.h"
#include "editor/EditorGUI.h"
#include "editor/EditorManager.h"
#include "editor/WorldEditor.h"
#include "ui/MainMenu.h"
#include "generation/TerrainGenerator.h"
#include "serialization/WorldSerializer.h"
#include "gameplay/Player.h"
#include "input/InputManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fresh {

Engine::Engine()
    : m_running(false)
    , m_inGame(false)
{
}

Engine::~Engine() {
    shutdown();
}

bool Engine::initialize() {
    std::cout << "Initializing Fresh Voxel Engine..." << std::endl;
    LOG_INFO_C("Initializing Fresh Voxel Engine...", "Engine");
    
    // Create main menu first
    m_mainMenu = std::make_unique<MainMenu>();
    if (!m_mainMenu->initialize()) {
        std::cerr << "Failed to initialize main menu" << std::endl;
        LOG_ERROR_C("Failed to initialize main menu", "Engine");
        return false;
    }
    std::cout << "Main menu initialized" << std::endl;
    LOG_INFO_C("Main menu initialized", "Engine");
    
    // Show main menu and wait for user choice
    m_mainMenu->render();
    
    // Check if user wants to create or load a world
    if (m_mainMenu->shouldCreateNewWorld()) {
        createNewWorld(m_mainMenu->getNewWorldName(), m_mainMenu->getWorldSeed());
    } else if (m_mainMenu->shouldLoadWorld()) {
        loadWorld(m_mainMenu->getLoadWorldName());
    } else {
        // User might be still in menu - we'll handle this in the run loop
        m_running = true;
        return true;
    }
    
    m_mainMenu->clearFlags();
    
    // Create renderer using the abstraction layer first to determine API
    // Auto-select best graphics API for the platform
    m_renderer = RenderContextFactory::createBest();
    if (!m_renderer) {
        std::cerr << "Failed to create render context" << std::endl;
        LOG_ERROR_C("Failed to create render context", "Engine");
        return false;
    }
    std::cout << "Render context created: " << getGraphicsAPIName(m_renderer->getAPI()) << std::endl;
    LOG_INFO_C("Render context created: " + std::string(getGraphicsAPIName(m_renderer->getAPI())), "Engine");
    
    // Determine if we need OpenGL context for the window
    bool useOpenGL = false; // DirectX only, no OpenGL needed
    
    // Create window
    m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
    if (!m_window->initialize(useOpenGL)) {
        std::cerr << "Failed to initialize window" << std::endl;
        LOG_ERROR_C("Failed to initialize window", "Engine");
        return false;
    }
    std::cout << "Window created" << std::endl;
    LOG_INFO_C("Window created", "Engine");
    
    // Create input manager and set up callbacks
    m_inputManager = std::make_unique<InputManager>();
    m_inputManager->initialize(m_window->getHandle());
    setupInputCallbacks();
    std::cout << "Input manager initialized" << std::endl;
    LOG_INFO_C("Input manager initialized", "Engine");
    
    // Initialize renderer with the window
    if (!m_renderer->initialize(m_window.get())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        LOG_ERROR_C("Failed to initialize renderer", "Engine");
        return false;
    }
    std::cout << "Renderer initialized with " << getGraphicsAPIName(m_renderer->getAPI()) << std::endl;
    LOG_INFO_C("Renderer initialized with " + std::string(getGraphicsAPIName(m_renderer->getAPI())), "Engine");
    
    // Create physics system
    m_physics = std::make_unique<PhysicsSystem>();
    if (!m_physics->initialize()) {
        std::cerr << "Failed to initialize physics system" << std::endl;
        LOG_ERROR_C("Failed to initialize physics system", "Engine");
        return false;
    }
    std::cout << "Physics system initialized" << std::endl;
    LOG_INFO_C("Physics system initialized", "Engine");
    
    // Create AI system
    m_aiSystem = std::make_unique<AISystem>();
    if (!m_aiSystem->initialize()) {
        std::cerr << "Failed to initialize AI system" << std::endl;
        LOG_ERROR_C("Failed to initialize AI system", "Engine");
        return false;
    }
    std::cout << "AI system initialized" << std::endl;
    LOG_INFO_C("AI system initialized", "Engine");
    
    // Create world editor
    m_worldEditor = std::make_unique<WorldEditor>();
    if (m_world && !m_worldEditor->initialize(m_world.get())) {
        std::cerr << "Failed to initialize world editor" << std::endl;
        LOG_ERROR_C("Failed to initialize world editor", "Engine");
        return false;
    }
    m_worldEditor->setEnabled(true); // Enable editor by default
    std::cout << "World editor initialized" << std::endl;
    LOG_INFO_C("World editor initialized", "Engine");
    
    // Create comprehensive editor manager
    m_editorManager = std::make_unique<EditorManager>();
    if (!m_editorManager->initialize(m_window.get(), m_renderer.get(), m_world.get(), m_worldEditor.get())) {
        std::cerr << "Failed to initialize editor manager" << std::endl;
        LOG_ERROR_C("Failed to initialize editor manager", "Engine");
        // Continue without full editor if ImGui not available
    } else {
        std::cout << "Editor manager initialized" << std::endl;
        LOG_INFO_C("Editor manager initialized", "Engine");
    }
    
    // Create legacy editor GUI (for backward compatibility)
    m_editor = std::make_unique<EditorGUI>();
    if (!m_editor->initialize(m_worldEditor.get())) {
        std::cerr << "Failed to initialize editor GUI" << std::endl;
        LOG_ERROR_C("Failed to initialize editor GUI", "Engine");
        return false;
    }
    m_editor->setVisible(true); // Show editor UI by default
    std::cout << "Editor GUI initialized" << std::endl;
    LOG_INFO_C("Editor GUI initialized", "Engine");
    
    // Create player
    m_player = std::make_unique<Player>();
    m_player->setWorld(m_world.get());
    // Set player spawn position at a safe height
    m_player->setPosition(glm::vec3(0.0f, 80.0f, 0.0f));
    std::cout << "Player initialized" << std::endl;
    LOG_INFO_C("Player initialized", "Engine");
    
    m_running = true;
    m_inGame = true;
    return true;
}

void Engine::createNewWorld(const std::string& name, int seed) {
    std::cout << "\nCreating new world: " << name << " (seed: " << seed << ")" << std::endl;
    
    // Create voxel world
    m_world = std::make_unique<VoxelWorld>();
    if (!m_world->initialize()) {
        std::cerr << "Failed to initialize voxel world" << std::endl;
        return;
    }
    std::cout << "Voxel world initialized" << std::endl;
    
    // Set custom seed for terrain generator
    TerrainGenerator terrainGen(seed);
    
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
                std::cout << "  Generated " << chunksGenerated << "/" << totalChunks << " chunks..." << std::endl;
            }
        }
    }
    
    std::cout << "World '" << name << "' created successfully!" << std::endl;
    std::cout << "Total chunks: " << m_world->getChunks().size() << std::endl;
    
    // Initialize world editor if we have a world
    if (m_world && !m_worldEditor) {
        m_worldEditor = std::make_unique<WorldEditor>();
        m_worldEditor->initialize(m_world.get());
        m_worldEditor->setEnabled(true);
    }
    
    // Save the world
    WorldSerializer serializer;
    std::string savePath = "saves/" + name + ".world";
    if (serializer.saveWorld(m_world.get(), savePath)) {
        std::cout << "World saved to: " << savePath << std::endl;
    }
}

void Engine::loadWorld(const std::string& name) {
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
    }
    
    // Initialize world editor if we have a world
    if (m_world && !m_worldEditor) {
        m_worldEditor = std::make_unique<WorldEditor>();
        m_worldEditor->initialize(m_world.get());
        m_worldEditor->setEnabled(true);
    }
}

void Engine::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float fpsTimer = 0.0f;
    
    // Frame timing constants
    const float TARGET_FPS = 60.0f;
    const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;  // 16.67ms
    const float MAX_DELTA_TIME = 0.1f;  // Cap at 100ms to prevent physics issues
    
    while (m_running) {
        // If not in game yet, show menu
        if (!m_inGame) {
            m_mainMenu->render();
            
            if (m_mainMenu->shouldCreateNewWorld()) {
                createNewWorld(m_mainMenu->getNewWorldName(), m_mainMenu->getWorldSeed());
                m_mainMenu->clearFlags();
                
                // Initialize all game systems (window, input, renderer, player)
                initializeGameSystems();
                m_inGame = true;
            } else if (m_mainMenu->shouldLoadWorld()) {
                loadWorld(m_mainMenu->getLoadWorldName());
                m_mainMenu->clearFlags();
                
                // Initialize all game systems (window, input, renderer, player)
                initializeGameSystems();
                m_inGame = true;
            }
            
            continue;
        }
        
        // Normal game loop
        if (m_window && m_window->shouldClose()) {
            m_running = false;
            break;
        }
        
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
            if (sleepTime > 0.002f) {  // 2ms threshold
                std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime - 0.001f));
            }
            
            // Busy-wait for the remaining time for precision
            while (std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - currentTime).count() < TARGET_FRAME_TIME) {
                // Spin-wait for precise timing
            }
        }
    }
}

void Engine::shutdown() {
    if (m_renderer) {
        m_renderer->waitIdle();
    }
    
    // Clean up GLFW callback user data
    if (m_callbackUserData) {
        delete m_callbackUserData;
        m_callbackUserData = nullptr;
    }
    
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

void Engine::processInput() {
    if (m_window) {
        m_window->pollEvents();
    }
    
    // Update input manager state
    if (m_inputManager) {
        m_inputManager->update();
        
        // Check if GUI wants input (only process world input if GUI doesn't need it)
        bool guiCapturesMouse = m_editorManager && m_editorManager->wantCaptureMouse();
        bool guiCapturesKeyboard = m_editorManager && m_editorManager->wantCaptureKeyboard();
        
        // Toggle editor with 'T' key (only if GUI doesn't want keyboard)
        if (!guiCapturesKeyboard && m_inputManager->isActionJustPressed(InputAction::ToggleEditor)) {
            if (m_editorManager) {
                m_editorManager->toggle();
                LOG_INFO_C("Editor toggled", "Engine");
            }
        }
        
        // Allow ESC to close the window (only if GUI doesn't want keyboard)
        if (!guiCapturesKeyboard && m_inputManager->isActionJustPressed(InputAction::OpenMenu)) {
            m_running = false;
        }
    }
}

void Engine::update(float deltaTime) {
    if (!m_world) {
        return;
    }
    
    // Check if GUI wants input before processing player updates
    bool guiCapturesMouse = m_editorManager && m_editorManager->wantCaptureMouse();
    bool guiCapturesKeyboard = m_editorManager && m_editorManager->wantCaptureKeyboard();
    
    // Handle player input (only if GUI doesn't capture input)
    if (m_player && m_inputManager && !guiCapturesMouse && !guiCapturesKeyboard) {
        m_player->handleInput(*m_inputManager, deltaTime);
        
        // Handle mouse movement for camera
        glm::vec2 mouseDelta = m_inputManager->getMouseDelta();
        if (glm::length(mouseDelta) > 0.0f) {
            m_player->handleMouseMovement(mouseDelta.x, mouseDelta.y);
        }
    }
    
    // Update player (physics, collision)
    if (m_player) {
        m_player->update(deltaTime);
    }
    
    // Update world with actual player position for chunk streaming
    if (m_player) {
        glm::vec3 playerPos = m_player->getPosition();
        m_world->update(WorldPos(
            static_cast<int>(playerPos.x),
            static_cast<int>(playerPos.y),
            static_cast<int>(playerPos.z)
        ));
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

void Engine::render() {
    if (!m_renderer) {
        return;
    }
    
    // Set clear color (sky blue) before beginning frame
    m_renderer->clearColor(0.53f, 0.81f, 0.92f, 1.0f);
    
    if (!m_renderer->beginFrame()) {
        return;
    }
    
    // Set viewport to match window size
    m_renderer->setViewport(0, 0, m_renderer->getSwapchainWidth(), m_renderer->getSwapchainHeight());
    
    // Render voxel world with DirectX
    if (m_world) {
        // TODO: Implement DirectX-based voxel rendering
        // For now, we'll keep the immediate mode OpenGL as a reference
        // This needs to be replaced with proper DirectX draw calls
    }
    
    // Begin editor frame (ImGui) before rendering editor UI
    if (m_editorManager) {
        m_editorManager->beginFrame();
    }
    
    // Render comprehensive editor UI
    if (m_editorManager && m_editorManager->isVisible()) {
        m_editorManager->render();
    }
    
    // End editor frame (render ImGui)
    if (m_editorManager) {
        m_editorManager->endFrame();
    }
    
    m_renderer->endFrame();
    
    // DirectX handles buffer swapping in endFrame()
}

void Engine::initializeGameSystems() {
    // This helper is called after world creation to initialize all game systems
    
    // Create renderer first to determine API
    if (!m_renderer) {
        m_renderer = RenderContextFactory::createBest();
        std::cout << "Render context created: " << getGraphicsAPIName(m_renderer->getAPI()) << std::endl;
    }
    
    bool useOpenGL = false; // DirectX only
    
    if (!m_window) {
        m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
        if (!m_window->initialize(useOpenGL)) {
            std::cerr << "Failed to initialize window in game systems" << std::endl;
            return;
        }
    }
    
    // Create input manager and set up callbacks
    if (!m_inputManager) {
        m_inputManager = std::make_unique<InputManager>();
        m_inputManager->initialize(m_window->getHandle());
        setupInputCallbacks();
    }
    
    // Initialize renderer with window
    if (m_renderer && m_window) {
        m_renderer->initialize(m_window.get());
    }
    
    // Create player
    if (!m_player && m_world) {
        m_player = std::make_unique<Player>();
        m_player->setWorld(m_world.get());
        m_player->setPosition(glm::vec3(0.0f, 80.0f, 0.0f));
    }
}

void Engine::setupInputCallbacks() {
    if (!m_window || !m_inputManager) {
        return;
    }
    
    GLFWwindow* window = m_window->getHandle();
    
    // Clean up old user data if it exists
    if (m_callbackUserData) {
        delete m_callbackUserData;
    }
    
    // Allocate user data for callbacks
    m_callbackUserData = new CallbackUserData{m_inputManager.get(), m_window.get()};
    glfwSetWindowUserPointer(window, m_callbackUserData);
    
    // Keyboard callback
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        (void)scancode; // Unused
        (void)mods; // Unused
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

} // namespace fresh
