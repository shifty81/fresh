#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
#include <GL/glew.h>
#endif

#include "voxel/VoxelTypes.h"

namespace fresh {

class Window;
class IRenderContext;
class VoxelWorld;
class PhysicsSystem;
class AISystem;
class EditorGUI;
class EditorManager;
class MainMenu;
class WorldEditor;
class Player;
class InputManager;

/**
 * @brief Main engine class that orchestrates all subsystems
 * 
 * This is the central component that manages the lifecycle of the voxel engine,
 * including initialization, main loop, and cleanup of all subsystems.
 */
class Engine {
public:
    Engine();
    ~Engine();

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /**
     * @brief Initialize the engine and all subsystems
     * @return true if initialization was successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Run the main engine loop
     */
    void run();

    /**
     * @brief Shutdown the engine and cleanup resources
     */
    void shutdown();

    /**
     * @brief Check if the engine is running
     * @return true if the engine is running, false otherwise
     */
    bool isRunning() const { return m_running; }

private:
    void processInput();
    void update(float deltaTime);
    void render();
    
    // Rendering helpers
    void initializeRendering();
    void shutdownRendering();
    void renderVoxelWorld();
    std::string loadShaderFile(const std::string& filepath);
    GLuint compileShader(const std::string& source, GLenum shaderType);
    GLuint createShaderProgram(const std::string& vertPath, const std::string& fragPath);

private:
    bool m_running;
    bool m_inGame;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<IRenderContext> m_renderer;
    std::unique_ptr<VoxelWorld> m_world;
    std::unique_ptr<PhysicsSystem> m_physics;
    std::unique_ptr<AISystem> m_aiSystem;
    std::unique_ptr<EditorGUI> m_editor;
#ifdef FRESH_IMGUI_AVAILABLE
    std::unique_ptr<EditorManager> m_editorManager;
#endif
    std::unique_ptr<MainMenu> m_mainMenu;
    std::unique_ptr<WorldEditor> m_worldEditor;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<InputManager> m_inputManager;
    
    // OpenGL rendering state
    GLuint m_shaderProgram = 0;
    std::unordered_map<ChunkPos, GLuint> m_chunkVAOs;
    std::unordered_map<ChunkPos, GLuint> m_chunkVBOs;
    std::unordered_map<ChunkPos, GLuint> m_chunkEBOs;
    std::unordered_map<ChunkPos, size_t> m_chunkIndexCounts;
    
    void createNewWorld(const std::string& name, int seed);
    void loadWorld(const std::string& name);
    void setupInputCallbacks();
    void initializeGameSystems(); // Helper for common initialization
    
    // User data for GLFW callbacks
    struct CallbackUserData {
        InputManager* inputManager;
        Window* window;
    };
    std::unique_ptr<CallbackUserData> m_callbackUserData;
};

} // namespace fresh
