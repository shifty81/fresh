#pragma once

#include <memory>
#include <string>

namespace fresh {

class Window;
class IRenderContext;
class VoxelWorld;
class PhysicsSystem;
class AISystem;
class EditorGUI;
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

private:
    bool m_running;
    bool m_inGame;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<IRenderContext> m_renderer;
    std::unique_ptr<VoxelWorld> m_world;
    std::unique_ptr<PhysicsSystem> m_physics;
    std::unique_ptr<AISystem> m_aiSystem;
    std::unique_ptr<EditorGUI> m_editor;
    std::unique_ptr<MainMenu> m_mainMenu;
    std::unique_ptr<WorldEditor> m_worldEditor;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<InputManager> m_inputManager;
    
    void createNewWorld(const std::string& name, int seed);
    void loadWorld(const std::string& name);
    void setupInputCallbacks();
};

} // namespace fresh
