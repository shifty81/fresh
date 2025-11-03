#pragma once
#include <map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace fresh {

/**
 * @brief Input actions that can be mapped to keys
 */
enum class InputAction {
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
    Crouch,
    Sprint,
    Use,
    Attack,
    PlaceBlock,
    BreakBlock,
    OpenInventory,
    OpenMenu,
    ToggleEditor,
    OpenChat
};

/**
 * @brief Manages keyboard, mouse, and controller input
 * 
 * Provides action-based input system with rebindable keys.
 * Tracks key states, mouse movement, and button presses.
 */
class InputManager {
public:
    /**
     * @brief Initialize input manager with GLFW window
     * @param window GLFW window handle
     */
    void initialize(GLFWwindow* window);
    
    /**
     * @brief Update input state (call once per frame)
     */
    void update();
    
    /**
     * @brief Process keyboard event (called by GLFW callback)
     * @param key GLFW key code
     * @param action GLFW action (press/release)
     */
    void processKeyEvent(int key, int action);
    
    /**
     * @brief Process mouse movement (called by GLFW callback)
     * @param xpos Mouse X position
     * @param ypos Mouse Y position
     */
    void processMouseMovement(double xpos, double ypos);
    
    /**
     * @brief Process mouse button event (called by GLFW callback)
     * @param button Mouse button code
     * @param action GLFW action (press/release)
     */
    void processMouseButton(int button, int action);
    
    /**
     * @brief Check if action is currently active
     * @param action Action to check
     * @return true if active
     */
    bool isActionActive(InputAction action) const;
    
    /**
     * @brief Check if action was just pressed this frame
     * @param action Action to check
     * @return true if just pressed
     */
    bool isActionJustPressed(InputAction action) const;
    
    /**
     * @brief Get mouse movement delta for this frame
     * @return Mouse delta as vec2
     */
    glm::vec2 getMouseDelta() const { return mouseDelta; }
    
    /**
     * @brief Set mouse sensitivity multiplier
     */
    void setMouseSensitivity(float sens) { sensitivity = sens; }
    float getMouseSensitivity() const { return sensitivity; }
    
    /**
     * @brief Set cursor capture mode
     * @param captured If true, hides and locks cursor
     */
    void setCursorMode(bool captured);
    
    /**
     * @brief Check if mouse button is pressed
     * @param button GLFW mouse button code
     * @return true if pressed
     */
    bool isMouseButtonPressed(int button) const;
    
    /**
     * @brief Bind key to action
     * @param action Action to bind
     * @param key GLFW key code
     */
    void setKeyBinding(InputAction action, int key);
    
private:
    void initializeDefaultBindings();
    
    GLFWwindow* window = nullptr;
    std::map<InputAction, int> keyBindings;
    std::map<int, bool> keyStates;
    std::map<int, bool> keyPressedThisFrame;
    std::map<int, bool> mouseButtonStates;
    
    glm::vec2 mouseDelta{0.0f};
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    float sensitivity = 1.0f;
    bool cursorCaptured = false;
};

} // namespace fresh
