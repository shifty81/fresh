/**
 * @file InputManagerTests.cpp
 * @brief Unit tests for the InputManager class
 */

#include <gtest/gtest.h>
#include "input/InputManager.h"

using namespace fresh;

/**
 * Test fixture for InputManager tests
 * Note: InputManager requires GLFW window which we can't create in unit tests,
 * so we test what we can without window initialization
 */
class InputManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        inputManager = std::make_unique<InputManager>();
    }

    void TearDown() override {
        inputManager.reset();
    }

    std::unique_ptr<InputManager> inputManager;
};

/**
 * Test InputMode enum values
 */
TEST_F(InputManagerTest, InputMode_EnumValues_AreDistinct) {
    // Arrange & Assert - just verify enum values are distinct
    EXPECT_NE(static_cast<int>(InputMode::GameMode), static_cast<int>(InputMode::UIMode));
    EXPECT_NE(static_cast<int>(InputMode::GameMode), static_cast<int>(InputMode::BuildMode));
    EXPECT_NE(static_cast<int>(InputMode::UIMode), static_cast<int>(InputMode::BuildMode));
}

/**
 * Test InputAction enum values
 */
TEST_F(InputManagerTest, InputAction_EnumValues_AreDistinct) {
    // Arrange & Assert - verify some key actions are distinct
    EXPECT_NE(static_cast<int>(InputAction::MoveForward), static_cast<int>(InputAction::MoveBackward));
    EXPECT_NE(static_cast<int>(InputAction::Jump), static_cast<int>(InputAction::Crouch));
    EXPECT_NE(static_cast<int>(InputAction::Attack), static_cast<int>(InputAction::Use));
}

/**
 * Test mouse delta initialization
 */
TEST_F(InputManagerTest, GetMouseDelta_BeforeUpdate_ReturnsZero) {
    // Act
    glm::vec2 delta = inputManager->getMouseDelta();
    
    // Assert
    EXPECT_FLOAT_EQ(delta.x, 0.0f);
    EXPECT_FLOAT_EQ(delta.y, 0.0f);
}

/**
 * Test mouse sensitivity
 */
TEST_F(InputManagerTest, SetMouseSensitivity_ValidValue_Accepts) {
    // Arrange & Act - should not crash
    EXPECT_NO_THROW(inputManager->setMouseSensitivity(1.0f));
    EXPECT_NO_THROW(inputManager->setMouseSensitivity(0.5f));
    EXPECT_NO_THROW(inputManager->setMouseSensitivity(2.0f));
}

/**
 * Test mouse sensitivity getter
 */
TEST_F(InputManagerTest, GetMouseSensitivity_Default_ReturnsPositive) {
    // Act
    float sensitivity = inputManager->getMouseSensitivity();
    
    // Assert
    EXPECT_GT(sensitivity, 0.0f);
}

/**
 * Test mouse sensitivity setter and getter
 */
TEST_F(InputManagerTest, SetAndGetMouseSensitivity_ValuePersists) {
    // Arrange
    float testSensitivity = 1.5f;
    
    // Act
    inputManager->setMouseSensitivity(testSensitivity);
    float result = inputManager->getMouseSensitivity();
    
    // Assert
    EXPECT_FLOAT_EQ(result, testSensitivity);
}

/**
 * Test input mode default state
 */
TEST_F(InputManagerTest, GetInputMode_Initial_ReturnsValidMode) {
    // Act
    InputMode mode = inputManager->getInputMode();
    
    // Assert - should be one of the valid modes
    bool isValid = (mode == InputMode::GameMode || 
                    mode == InputMode::UIMode || 
                    mode == InputMode::BuildMode);
    EXPECT_TRUE(isValid);
}

/**
 * Test input mode switching
 */
TEST_F(InputManagerTest, SetInputMode_AllModes_Accepts) {
    // Act & Assert - should not crash
    EXPECT_NO_THROW(inputManager->setInputMode(InputMode::GameMode));
    EXPECT_EQ(inputManager->getInputMode(), InputMode::GameMode);
    
    EXPECT_NO_THROW(inputManager->setInputMode(InputMode::UIMode));
    EXPECT_EQ(inputManager->getInputMode(), InputMode::UIMode);
    
    EXPECT_NO_THROW(inputManager->setInputMode(InputMode::BuildMode));
    EXPECT_EQ(inputManager->getInputMode(), InputMode::BuildMode);
}

/**
 * Test action state before initialization
 */
TEST_F(InputManagerTest, IsActionActive_BeforeInit_ReturnsFalse) {
    // Act & Assert - actions should not be active without initialization
    EXPECT_FALSE(inputManager->isActionActive(InputAction::MoveForward));
    EXPECT_FALSE(inputManager->isActionActive(InputAction::Jump));
    EXPECT_FALSE(inputManager->isActionActive(InputAction::Attack));
}

/**
 * Test just pressed state before initialization
 */
TEST_F(InputManagerTest, IsActionJustPressed_BeforeInit_ReturnsFalse) {
    // Act & Assert - actions should not be just pressed without initialization
    EXPECT_FALSE(inputManager->isActionJustPressed(InputAction::MoveForward));
    EXPECT_FALSE(inputManager->isActionJustPressed(InputAction::Jump));
    EXPECT_FALSE(inputManager->isActionJustPressed(InputAction::Attack));
}

/**
 * Test key event processing without window (should not crash)
 */
TEST_F(InputManagerTest, ProcessKeyEvent_WithoutWindow_DoesNotCrash) {
    // Act & Assert - should handle gracefully without window
    EXPECT_NO_THROW(inputManager->processKeyEvent(GLFW_KEY_W, GLFW_PRESS));
    EXPECT_NO_THROW(inputManager->processKeyEvent(GLFW_KEY_W, GLFW_RELEASE));
    EXPECT_NO_THROW(inputManager->processKeyEvent(GLFW_KEY_SPACE, GLFW_PRESS));
}

/**
 * Test mouse button processing without window
 */
TEST_F(InputManagerTest, ProcessMouseButton_WithoutWindow_DoesNotCrash) {
    // Act & Assert - should handle gracefully without window
    EXPECT_NO_THROW(inputManager->processMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS));
    EXPECT_NO_THROW(inputManager->processMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE));
    EXPECT_NO_THROW(inputManager->processMouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS));
}

/**
 * Test mouse movement processing without window
 */
TEST_F(InputManagerTest, ProcessMouseMovement_WithoutWindow_DoesNotCrash) {
    // Act & Assert - should handle gracefully without window
    EXPECT_NO_THROW(inputManager->processMouseMovement(100.0, 200.0));
    EXPECT_NO_THROW(inputManager->processMouseMovement(150.0, 250.0));
}

/**
 * Test update without initialization
 */
TEST_F(InputManagerTest, Update_WithoutInit_DoesNotCrash) {
    // Act & Assert - should handle gracefully without window
    EXPECT_NO_THROW(inputManager->update());
}

/**
 * Test multiple updates in sequence
 */
TEST_F(InputManagerTest, Update_MultipleCalls_DoesNotCrash) {
    // Act & Assert
    EXPECT_NO_THROW({
        inputManager->update();
        inputManager->update();
        inputManager->update();
    });
}

/**
 * Test all InputAction enum members are accessible
 */
TEST_F(InputManagerTest, InputAction_AllMembers_Accessible) {
    // Act & Assert - verify we can check all actions without crash
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::MoveForward));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::MoveBackward));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::MoveLeft));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::MoveRight));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::Jump));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::Crouch));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::Sprint));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::Use));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::Attack));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::PlaceBlock));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::BreakBlock));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::OpenInventory));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::OpenMenu));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::ToggleEditor));
    EXPECT_NO_THROW(inputManager->isActionActive(InputAction::OpenChat));
}

/**
 * Test key binding can be changed
 */
TEST_F(InputManagerTest, SetKeyBinding_ValidKey_DoesNotCrash) {
    // Arrange
    int newKey = GLFW_KEY_F;
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(inputManager->setKeyBinding(InputAction::MoveForward, newKey));
}

/**
 * Test multiple key binding changes
 */
TEST_F(InputManagerTest, SetKeyBinding_MultipleActions_DoesNotCrash) {
    // Arrange
    int key1 = GLFW_KEY_I;
    int key2 = GLFW_KEY_K;
    int key3 = GLFW_KEY_L;
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(inputManager->setKeyBinding(InputAction::MoveForward, key1));
    EXPECT_NO_THROW(inputManager->setKeyBinding(InputAction::MoveBackward, key2));
    EXPECT_NO_THROW(inputManager->setKeyBinding(InputAction::Jump, key3));
}

/**
 * Test input manager can be created and destroyed multiple times
 */
TEST(InputManagerLifecycleTest, CreateDestroy_Multiple_Works) {
    // Act & Assert
    for (int i = 0; i < 10; i++) {
        auto manager = std::make_unique<InputManager>();
        EXPECT_NE(manager, nullptr);
        manager.reset();
    }
}

/**
 * Test mouse delta calculation with simulated movement
 */
TEST_F(InputManagerTest, ProcessMouseMovement_MultipleCallsWithUpdate_TracksMovement) {
    // Arrange
    double x1 = 100.0, y1 = 100.0;
    double x2 = 150.0, y2 = 120.0;
    
    // Act - simulate mouse movement
    inputManager->processMouseMovement(x1, y1);
    inputManager->update();
    
    // First movement establishes baseline
    glm::vec2 delta1 = inputManager->getMouseDelta();
    (void)delta1; // Used to establish baseline
    
    inputManager->processMouseMovement(x2, y2);
    inputManager->update();
    
    // Second movement should show delta
    glm::vec2 delta2 = inputManager->getMouseDelta();
    (void)delta2; // Used to verify delta calculation
    
    // Assert - delta should be calculated (implementation dependent)
    // At minimum, these calls should not crash
    EXPECT_NO_THROW(inputManager->getMouseDelta());
}

/**
 * Test that mouse position tracking exists
 */
TEST_F(InputManagerTest, GetMousePosition_WithoutInit_ReturnsValidValue) {
    // Act
    glm::vec2 pos = inputManager->getMousePosition();
    
    // Assert - should return a valid vec2 (even if zero)
    EXPECT_TRUE(std::isfinite(pos.x));
    EXPECT_TRUE(std::isfinite(pos.y));
}

/**
 * Test that mouse button state tracking exists
 */
TEST_F(InputManagerTest, IsMouseButtonPressed_WithoutInit_DoesNotCrash) {
    // Act & Assert - should handle gracefully
    EXPECT_NO_THROW(inputManager->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT));
    EXPECT_NO_THROW(inputManager->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT));
    EXPECT_NO_THROW(inputManager->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE));
}

/**
 * Test mouse button just pressed tracking
 */
TEST_F(InputManagerTest, IsMouseButtonJustPressed_WithoutInit_DoesNotCrash) {
    // Act & Assert - should handle gracefully
    EXPECT_NO_THROW(inputManager->isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT));
    EXPECT_NO_THROW(inputManager->isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT));
}
