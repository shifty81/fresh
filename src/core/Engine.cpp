#include "core/Engine.h"
#include "core/Window.h"
#include "core/Logger.h"
#include "renderer/RenderContext.h"
#include "renderer/GraphicsAPI.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include "physics/PhysicsSystem.h"
#include "ai/AISystem.h"
#include "editor/EditorGUI.h"
#include "editor/WorldEditor.h"
#include "ui/MainMenu.h"
#include "generation/TerrainGenerator.h"
#include "serialization/WorldSerializer.h"
#include <iostream>
#include <chrono>

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
    
    // Create window
    m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
    if (!m_window->initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        LOG_ERROR_C("Failed to initialize window", "Engine");
        return false;
    }
    std::cout << "Window created" << std::endl;
    LOG_INFO_C("Window created", "Engine");
    
    // Create renderer using the abstraction layer
    // Auto-select best graphics API for the platform
    m_renderer = RenderContextFactory::createBest();
    if (!m_renderer) {
        std::cerr << "Failed to create render context" << std::endl;
        LOG_ERROR_C("Failed to create render context", "Engine");
        return false;
    }
    
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
    
    // Create editor GUI
    m_editor = std::make_unique<EditorGUI>();
    if (!m_editor->initialize(m_worldEditor.get())) {
        std::cerr << "Failed to initialize editor GUI" << std::endl;
        LOG_ERROR_C("Failed to initialize editor GUI", "Engine");
        return false;
    }
    m_editor->setVisible(true); // Show editor UI by default
    std::cout << "Editor GUI initialized" << std::endl;
    LOG_INFO_C("Editor GUI initialized", "Engine");
    
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
    
    while (m_running) {
        // If not in game yet, show menu
        if (!m_inGame) {
            m_mainMenu->render();
            
            if (m_mainMenu->shouldCreateNewWorld()) {
                createNewWorld(m_mainMenu->getNewWorldName(), m_mainMenu->getWorldSeed());
                m_mainMenu->clearFlags();
                
                // Initialize graphics systems now that we have a world
                m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
                if (m_window->initialize()) {
                    m_renderer = RenderContextFactory::createBest();
                    if (m_renderer) {
                        m_renderer->initialize(m_window.get());
                    }
                }
                m_inGame = true;
            } else if (m_mainMenu->shouldLoadWorld()) {
                loadWorld(m_mainMenu->getLoadWorldName());
                m_mainMenu->clearFlags();
                
                // Initialize graphics systems now that we have a world
                m_window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine");
                if (m_window->initialize()) {
                    m_renderer = RenderContextFactory::createBest();
                    if (m_renderer) {
                        m_renderer->initialize(m_window.get());
                    }
                }
                m_inGame = true;
            }
            
            continue;
        }
        
        // Normal game loop
        if (m_window && m_window->shouldClose()) {
            break;
        }
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // FPS counter
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f) {
            std::cout << "FPS: " << frameCount;
            if (m_world) {
                std::cout << " | Chunks: " << m_world->getChunks().size();
            }
            std::cout << std::endl;
            frameCount = 0;
            fpsTimer = 0.0f;
        }
        
        processInput();
        update(deltaTime);
        render();
    }
}

void Engine::shutdown() {
    if (m_renderer) {
        m_renderer->waitIdle();
    }
    
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
}

void Engine::update(float deltaTime) {
    if (!m_world) {
        return;
    }
    
    // Update world (chunk streaming, etc.)
    WorldPos playerPos(0, 64, 0); // TODO: Get actual player position
    m_world->update(playerPos);
    
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
    
    if (!m_renderer->beginFrame()) {
        return;
    }
    
    // Render voxel world
    // TODO: Implement actual rendering of chunks
    
    // Render editor UI
    if (m_editor) {
        m_editor->render();
    }
    
    m_renderer->endFrame();
}

} // namespace fresh
