#include "input/InputManager.h"

namespace fresh {

void InputManager::initialize(GLFWwindow* win) {
    window = win;
    initializeDefaultBindings();
    setCursorMode(true); // Start with captured cursor
}

void InputManager::update() {
    // Clear just-pressed states from previous frame
    keyPressedThisFrame.clear();
    
    // Reset mouse delta
    mouseDelta = glm::vec2(0.0f);
}

void InputManager::processKeyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
        keyPressedThisFrame[key] = true;
    } else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

void InputManager::processMouseMovement(double xpos, double ypos) {
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }
    
    float xOffset = static_cast<float>(xpos - lastMouseX);
    float yOffset = static_cast<float>(ypos - lastMouseY);
    
    lastMouseX = xpos;
    lastMouseY = ypos;
    
    if (cursorCaptured) {
        mouseDelta = glm::vec2(xOffset, yOffset);
    }
}

void InputManager::processMouseButton(int button, int action) {
    if (action == GLFW_PRESS) {
        mouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        mouseButtonStates[button] = false;
    }
}

bool InputManager::isActionActive(InputAction action) const {
    auto it = keyBindings.find(action);
    if (it == keyBindings.end()) return false;
    
    auto keyIt = keyStates.find(it->second);
    if (keyIt == keyStates.end()) return false;
    
    return keyIt->second;
}

bool InputManager::isActionJustPressed(InputAction action) const {
    auto it = keyBindings.find(action);
    if (it == keyBindings.end()) return false;
    
    auto keyIt = keyPressedThisFrame.find(it->second);
    if (keyIt == keyPressedThisFrame.end()) return false;
    
    return keyIt->second;
}

void InputManager::setCursorMode(bool captured) {
    if (!window) return;
    
    cursorCaptured = captured;
    if (captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true; // Reset to avoid jump
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates.find(button);
    if (it == mouseButtonStates.end()) return false;
    return it->second;
}

void InputManager::setKeyBinding(InputAction action, int key) {
    keyBindings[action] = key;
}

void InputManager::initializeDefaultBindings() {
    // WASD movement
    keyBindings[InputAction::MoveForward] = GLFW_KEY_W;
    keyBindings[InputAction::MoveBackward] = GLFW_KEY_S;
    keyBindings[InputAction::MoveLeft] = GLFW_KEY_A;
    keyBindings[InputAction::MoveRight] = GLFW_KEY_D;
    
    // Actions
    keyBindings[InputAction::Jump] = GLFW_KEY_SPACE;
    keyBindings[InputAction::Crouch] = GLFW_KEY_LEFT_CONTROL;
    keyBindings[InputAction::Sprint] = GLFW_KEY_LEFT_SHIFT;
    keyBindings[InputAction::Use] = GLFW_KEY_E;
    
    // Mouse actions (mapped to keys for consistency, actual mouse handled separately)
    keyBindings[InputAction::Attack] = GLFW_MOUSE_BUTTON_LEFT;
    keyBindings[InputAction::PlaceBlock] = GLFW_MOUSE_BUTTON_RIGHT;
    keyBindings[InputAction::BreakBlock] = GLFW_MOUSE_BUTTON_LEFT;
    
    // UI
    keyBindings[InputAction::OpenInventory] = GLFW_KEY_TAB;
    keyBindings[InputAction::OpenMenu] = GLFW_KEY_ESCAPE;
    keyBindings[InputAction::ToggleEditor] = GLFW_KEY_T;
    keyBindings[InputAction::OpenChat] = GLFW_KEY_ENTER;
}

} // namespace fresh
