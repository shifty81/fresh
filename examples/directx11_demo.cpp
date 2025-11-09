/**
 * @file directx11_demo.cpp
 * @brief Comprehensive demo showcasing DirectX 11 voxel rendering
 * 
 * This demo demonstrates the Fresh Voxel Engine's DirectX 11 rendering capabilities:
 * - Voxel world generation with procedural terrain
 * - First-person camera controls
 * - Block placement and breaking
 * - Real-time chunk streaming
 * - Directional lighting
 * - Performance metrics
 * 
 * Controls:
 * - WASD: Move
 * - Mouse: Look around
 * - Space: Jump
 * - Shift: Sprint
 * - Left Click: Break block
 * - Right Click: Place block
 * - ESC: Exit
 * 
 * Requirements:
 * - Windows 10/11
 * - DirectX 11 compatible GPU
 * - Visual Studio 2022
 */

#include "core/Engine.h"
#include "core/Logger.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;
    
    // Initialize logger
    fresh::Logger::getInstance().initialize();
    
    std::cout << "========================================" << std::endl;
    std::cout << "   Fresh Voxel Engine - DirectX 11 Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "This demo showcases the DirectX 11 rendering backend" << std::endl;
    std::cout << "with full voxel world rendering capabilities." << std::endl;
    std::cout << std::endl;
    
    std::cout << "Features:" << std::endl;
    std::cout << "  * Procedural terrain generation" << std::endl;
    std::cout << "  * First-person movement and camera" << std::endl;
    std::cout << "  * Block interaction (place/break)" << std::endl;
    std::cout << "  * Real-time chunk streaming" << std::endl;
    std::cout << "  * Directional lighting" << std::endl;
    std::cout << "  * Hardware-accelerated DirectX 11 rendering" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD       - Move around" << std::endl;
    std::cout << "  Mouse      - Look around" << std::endl;
    std::cout << "  Space      - Jump" << std::endl;
    std::cout << "  Shift      - Sprint (move faster)" << std::endl;
    std::cout << "  Left Click - Break block" << std::endl;
    std::cout << "  Right Click- Place block" << std::endl;
    std::cout << "  ESC        - Exit" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Starting engine..." << std::endl;
    LOG_INFO("DirectX 11 Demo starting...");
    
    try {
        // Create engine instance
        fresh::Engine engine;
        
        // Initialize engine (will use DirectX 11 on Windows)
        if (!engine.initialize()) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            LOG_ERROR("Failed to initialize engine");
            fresh::Logger::getInstance().shutdown();
            return 1;
        }
        
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "   Engine initialized successfully!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        
        std::cout << "You should now see:" << std::endl;
        std::cout << "  * Blue sky background" << std::endl;
        std::cout << "  * Procedurally generated terrain" << std::endl;
        std::cout << "  * Grass, dirt, and stone blocks" << std::endl;
        std::cout << "  * Smooth directional lighting" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Running main loop..." << std::endl;
        std::cout << "(Press ESC to exit)" << std::endl;
        LOG_INFO("Engine main loop starting...");
        
        // Run the main game loop
        engine.run();
        
        std::cout << std::endl;
        std::cout << "Shutting down..." << std::endl;
        LOG_INFO("Engine shutting down...");
        
        // Cleanup
        engine.shutdown();
        
        std::cout << "Engine shutdown complete." << std::endl;
        std::cout << "Thank you for trying the DirectX 11 demo!" << std::endl;
        LOG_INFO("Engine shutdown complete");
        
        fresh::Logger::getInstance().shutdown();
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        LOG_FATAL(std::string("Fatal error: ") + e.what());
        fresh::Logger::getInstance().shutdown();
        return 1;
    }
}
