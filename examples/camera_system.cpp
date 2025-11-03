/**
 * @file camera_system.cpp
 * @brief Demonstrates camera controls and first-person view implementation
 * 
 * This example shows:
 * - Creating and configuring a camera
 * - Mouse look controls
 * - Camera movement and positioning
 * - View and projection matrices
 * - FOV adjustment
 */

#include <iostream>
#include <memory>
#include "gameplay/Camera.h"
#include "input/InputManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    std::cout << "=== Fresh Voxel Engine: Camera System ===" << std::endl;
    std::cout << "This example demonstrates camera controls for first-person view." << std::endl;
    
    // Step 1: Create camera instance
    std::cout << "\n[1/5] Creating camera..." << std::endl;
    Camera camera;
    
    // Set initial position (above ground level)
    camera.setPosition(glm::vec3(0.0f, 65.0f, 0.0f));
    std::cout << "    ✓ Camera created at (0, 65, 0)" << std::endl;
    
    // Configure camera properties
    camera.setFOV(70.0f);              // Field of view in degrees
    camera.setAspectRatio(16.0f / 9.0f); // Widescreen
    camera.setNearPlane(0.1f);         // Near clipping plane
    camera.setFarPlane(1000.0f);       // Far clipping plane (render distance)
    std::cout << "    ✓ Camera configured:" << std::endl;
    std::cout << "      FOV: 70°" << std::endl;
    std::cout << "      Aspect: 16:9" << std::endl;
    std::cout << "      Near/Far: 0.1 - 1000 blocks" << std::endl;
    
    // Step 2: Mouse look controls
    std::cout << "\n[2/5] Simulating mouse look..." << std::endl;
    
    // Initial orientation (looking forward)
    std::cout << "    Initial forward: " << camera.getForward().x << ", "
              << camera.getForward().y << ", " << camera.getForward().z << std::endl;
    
    // Simulate mouse movement (look right)
    float mouseDeltaX = 100.0f;  // Pixels
    float mouseDeltaY = 0.0f;
    float sensitivity = 0.002f;
    
    float yaw = mouseDeltaX * sensitivity;
    float pitch = mouseDeltaY * sensitivity;
    
    camera.rotate(yaw, pitch);
    std::cout << "    ✓ Rotated camera (yaw: " << yaw << ", pitch: " << pitch << ")" << std::endl;
    std::cout << "    New forward: " << camera.getForward().x << ", "
              << camera.getForward().y << ", " << camera.getForward().z << std::endl;
    
    // Look up
    mouseDeltaY = -50.0f;
    pitch = mouseDeltaY * sensitivity;
    camera.rotate(0.0f, pitch);
    std::cout << "    ✓ Looked up (pitch: " << pitch << ")" << std::endl;
    
    // Step 3: Camera vectors
    std::cout << "\n[3/5] Camera orientation vectors..." << std::endl;
    glm::vec3 forward = camera.getForward();
    glm::vec3 right = camera.getRight();
    glm::vec3 up = camera.getUp();
    
    std::cout << "    Forward: (" << forward.x << ", " << forward.y << ", " << forward.z << ")" << std::endl;
    std::cout << "    Right:   (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;
    std::cout << "    Up:      (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
    
    // Step 4: View and projection matrices
    std::cout << "\n[4/5] Generating view and projection matrices..." << std::endl;
    
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projMatrix = camera.getProjectionMatrix();
    
    std::cout << "    ✓ View matrix generated (look-at transformation)" << std::endl;
    std::cout << "    ✓ Projection matrix generated (perspective)" << std::endl;
    
    // These matrices would be sent to shaders for rendering
    std::cout << "    Use in shader:" << std::endl;
    std::cout << "      gl_Position = projection * view * model * vec4(position, 1.0);" << std::endl;
    
    // Step 5: Camera movement
    std::cout << "\n[5/5] Camera movement..." << std::endl;
    
    glm::vec3 startPos = camera.getPosition();
    std::cout << "    Start position: (" << startPos.x << ", " << startPos.y << ", " << startPos.z << ")" << std::endl;
    
    // Move forward
    glm::vec3 moveVec = camera.getForward() * 10.0f;
    camera.move(moveVec);
    std::cout << "    ✓ Moved 10 blocks forward" << std::endl;
    
    // Move right
    moveVec = camera.getRight() * 5.0f;
    camera.move(moveVec);
    std::cout << "    ✓ Moved 5 blocks right" << std::endl;
    
    glm::vec3 endPos = camera.getPosition();
    std::cout << "    End position: (" << endPos.x << ", " << endPos.y << ", " << endPos.z << ")" << std::endl;
    
    float distance = glm::length(endPos - startPos);
    std::cout << "    Total distance moved: " << distance << " blocks" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • Creating and configuring a camera" << std::endl;
    std::cout << "  • Implementing mouse look controls" << std::endl;
    std::cout << "  • Understanding camera vectors (forward, right, up)" << std::endl;
    std::cout << "  • Generating view and projection matrices" << std::endl;
    std::cout << "  • Moving the camera in 3D space" << std::endl;
    std::cout << "\nCamera Properties:" << std::endl;
    std::cout << "  • FOV: Controls zoom (lower = more zoomed)" << std::endl;
    std::cout << "  • Aspect Ratio: Width/Height of viewport" << std::endl;
    std::cout << "  • Near/Far Planes: Rendering distance limits" << std::endl;
    std::cout << "  • Position: Camera location in world" << std::endl;
    std::cout << "  • Rotation: Yaw (horizontal) and Pitch (vertical)" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Integrate with player_controller.cpp for complete FPS" << std::endl;
    std::cout << "  • Try raycasting.cpp for mouse picking" << std::endl;
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Camera Class
 *    - Represents viewpoint in 3D world
 *    - Generates view and projection matrices for rendering
 *    - Handles rotation (yaw, pitch, roll)
 *    - Provides direction vectors
 * 
 * 2. View Matrix
 *    - Transforms world space to camera space
 *    - Created from camera position and orientation
 *    - Used by vertex shader to position geometry
 *    - Essentially "moves world" to camera origin
 * 
 * 3. Projection Matrix
 *    - Transforms camera space to clip space
 *    - Creates perspective effect (distant objects smaller)
 *    - Defined by FOV, aspect ratio, near/far planes
 *    - Orthographic alternative for 2D/UI rendering
 * 
 * 4. Camera Rotation
 *    - Yaw: Rotation around Y axis (left/right)
 *    - Pitch: Rotation around X axis (up/down)
 *    - Roll: Rotation around Z axis (tilt)
 *    - Usually limit pitch to prevent gimbal lock
 * 
 * 5. Direction Vectors
 *    - Forward: Direction camera is looking
 *    - Right: Perpendicular to forward (strafe)
 *    - Up: Perpendicular to forward and right
 *    - All unit vectors (length = 1)
 * 
 * MOUSE LOOK IMPLEMENTATION:
 * 
 * 1. Capture Mouse Delta:
 *    deltaX = currentMouseX - lastMouseX;
 *    deltaY = currentMouseY - lastMouseY;
 * 
 * 2. Apply Sensitivity:
 *    yaw += deltaX * sensitivity;
 *    pitch += deltaY * sensitivity;
 * 
 * 3. Clamp Pitch:
 *    pitch = clamp(pitch, -89°, 89°);
 * 
 * 4. Update Camera:
 *    camera.rotate(yaw, pitch);
 * 
 * FOV EFFECTS:
 * 
 * • Low FOV (40-60°): Zoomed in, tunnel vision
 * • Normal FOV (70-90°): Natural, most games
 * • High FOV (100-120°): Wide angle, fisheye effect
 * • Dynamic FOV: Change based on speed (sprint)
 * 
 * CAMERA MODES:
 * 
 * 1. First-Person:
 *    - Camera at player eye level
 *    - No visible player model
 *    - Direct control
 * 
 * 2. Third-Person:
 *    - Camera offset behind player
 *    - Player model visible
 *    - Collision check for camera
 * 
 * 3. Free Camera:
 *    - No collision
 *    - Full 6DOF movement
 *    - Editor/debug mode
 * 
 * INTEGRATION WITH RENDERING:
 * 
 * // In render loop
 * glm::mat4 view = camera.getViewMatrix();
 * glm::mat4 proj = camera.getProjectionMatrix();
 * 
 * // Send to shader
 * shader.setMat4("view", view);
 * shader.setMat4("projection", proj);
 * 
 * // In vertex shader
 * gl_Position = projection * view * model * vec4(position, 1.0);
 * 
 * PERFORMANCE CONSIDERATIONS:
 * 
 * • Cache matrices when camera doesn't move
 * • Only recalculate on position/rotation change
 * • Use dirty flags to track changes
 * • Frustum culling with projection matrix
 * 
 * COMMON ISSUES:
 * 
 * ❌ Forgetting to lock mouse cursor
 * ❌ Not clamping pitch (can flip upside down)
 * ❌ Wrong sensitivity (too fast/slow)
 * ❌ Updating matrices every frame unnecessarily
 * ❌ Gimbal lock from euler angles
 * 
 * ADVANCED FEATURES:
 * 
 * • Camera shake for effects
 * • Smooth follow for third-person
 * • Look-at target tracking
 * • Cinematic camera paths
 * • Zoom/scope mechanics
 */
