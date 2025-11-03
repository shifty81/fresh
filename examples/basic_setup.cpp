/**
 * @file basic_setup.cpp
 * @brief Minimal Fresh Voxel Engine setup example
 * 
 * This example demonstrates the absolute minimum code needed to:
 * - Initialize the engine
 * - Create a window
 * - Run the main game loop
 * - Clean up resources
 * 
 * This is the perfect starting point for understanding how the engine works.
 */

#include <iostream>
#include <memory>
#include "core/Engine.h"
#include "core/Window.h"

int main() {
    std::cout << "=== Fresh Voxel Engine: Basic Setup ===" << std::endl;
    std::cout << "This example shows minimal engine initialization." << std::endl;
    
    try {
        // Step 1: Create a window
        // The window manages the OS window and input handling
        std::cout << "\n[1/3] Creating window..." << std::endl;
        auto window = std::make_unique<Window>(1280, 720, "Fresh Voxel Engine - Basic Setup");
        
        if (!window->initialize()) {
            std::cerr << "ERROR: Failed to initialize window!" << std::endl;
            return 1;
        }
        std::cout << "    ✓ Window created: 1280x720" << std::endl;
        
        // Step 2: Initialize the engine
        // The engine manages all core systems
        std::cout << "\n[2/3] Initializing engine..." << std::endl;
        Engine engine;
        
        if (!engine.initialize(window.get())) {
            std::cerr << "ERROR: Failed to initialize engine!" << std::endl;
            return 1;
        }
        std::cout << "    ✓ Engine initialized" << std::endl;
        std::cout << "    ✓ Renderer ready" << std::endl;
        std::cout << "    ✓ Core systems loaded" << std::endl;
        
        // Step 3: Main game loop
        std::cout << "\n[3/3] Starting main loop..." << std::endl;
        std::cout << "    Press ESC or close window to exit" << std::endl;
        
        double lastTime = glfwGetTime();
        int frameCount = 0;
        
        while (engine.isRunning()) {
            // Calculate delta time
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;
            
            // Update engine (processes input, updates systems)
            engine.update(deltaTime);
            
            // Render frame
            engine.render();
            
            // FPS counter (display every second)
            frameCount++;
            if (frameCount % 60 == 0) {
                double fps = 1.0 / deltaTime;
                std::cout << "    FPS: " << static_cast<int>(fps) << std::endl;
            }
        }
        
        // Step 4: Cleanup
        std::cout << "\n[4/4] Shutting down..." << std::endl;
        engine.shutdown();
        window->shutdown();
        std::cout << "    ✓ Engine shut down cleanly" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nWhat you learned:" << std::endl;
    std::cout << "  • How to create a window" << std::endl;
    std::cout << "  • How to initialize the engine" << std::endl;
    std::cout << "  • The structure of the main game loop" << std::endl;
    std::cout << "  • Proper cleanup and resource management" << std::endl;
    std::cout << "\nNext steps:" << std::endl;
    std::cout << "  • Try creating_chunks.cpp to learn about voxel worlds" << std::endl;
    std::cout << "  • See player_controller.cpp to add player movement" << std::endl;
    
    return 0;
}

/**
 * EXPLANATION OF KEY CONCEPTS:
 * 
 * 1. Window Creation
 *    - Window class wraps GLFW functionality
 *    - Manages OS window, input events, and surface
 *    - Must be created before engine initialization
 * 
 * 2. Engine Initialization
 *    - Engine class is the main interface to all systems
 *    - Initializes renderer, resource manager, etc.
 *    - Returns false if initialization fails
 * 
 * 3. Main Game Loop
 *    - Runs at the refresh rate of the display
 *    - Update phase: processes input, updates game state
 *    - Render phase: submits draw calls to GPU
 *    - Delta time tracks time between frames
 * 
 * 4. Resource Cleanup
 *    - ALWAYS call shutdown() before exit
 *    - Ensures graphics resources are properly released
 *    - Window cleanup happens automatically (RAII)
 * 
 * COMMON MISTAKES:
 * 
 * ❌ Forgetting to call engine.initialize()
 * ❌ Not checking return values for errors
 * ❌ Creating window after engine initialization
 * ❌ Forgetting to call shutdown()
 * ❌ Not handling exceptions
 * 
 * CUSTOMIZATION IDEAS:
 * 
 * • Change window size and title
 * • Add custom error handling
 * • Measure frame times and log statistics
 * • Add a fixed time step for physics
 * • Implement a frame rate limiter
 */
