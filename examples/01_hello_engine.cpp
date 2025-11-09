/**
 * @file 01_hello_engine.cpp
 * @brief Minimal Fresh Voxel Engine initialization example
 * 
 * This example demonstrates:
 * - Basic engine initialization
 * - Logger setup
 * - Proper shutdown sequence
 * 
 * Perfect for verifying your build environment works!
 */

#include "core/Engine.h"
#include "core/Logger.h"
#include <iostream>

int main() {
    std::cout << "Fresh Voxel Engine - Hello World Example\n";
    std::cout << "==========================================\n\n";
    
    // Step 1: Initialize the logger
    std::cout << "[1/3] Initializing logger...\n";
    fresh::Logger::getInstance().initialize();
    LOG_INFO("Logger initialized successfully");
    std::cout << "✓ Logger ready\n\n";
    
    // Step 2: Create and initialize the engine
    std::cout << "[2/3] Initializing engine...\n";
    fresh::Engine engine;
    
    if (!engine.initialize()) {
        std::cerr << "✗ Failed to initialize engine\n";
        LOG_ERROR("Engine initialization failed");
        fresh::Logger::getInstance().shutdown();
        return 1;
    }
    
    LOG_INFO("Engine initialized successfully");
    std::cout << "✓ Engine ready\n";
    std::cout << "✓ DirectX rendering initialized\n";
    std::cout << "✓ All systems operational\n\n";
    
    // Step 3: Clean shutdown
    std::cout << "[3/3] Shutting down...\n";
    engine.shutdown();
    LOG_INFO("Engine shutdown complete");
    fresh::Logger::getInstance().shutdown();
    std::cout << "✓ Clean shutdown\n\n";
    
    std::cout << "==========================================\n";
    std::cout << "Success! Engine is working correctly.\n";
    std::cout << "\nNext steps:\n";
    std::cout << "  • Try 02_create_window.cpp to see graphics\n";
    std::cout << "  • Check logs/ directory for detailed logs\n";
    std::cout << "  • Read the documentation in docs/\n";
    
    return 0;
}

/**
 * KEY CONCEPTS:
 * 
 * 1. Logger Initialization
 *    - Always initialize logger first
 *    - Provides debugging and error tracking
 *    - Logs saved to logs/ directory
 * 
 * 2. Engine Initialization
 *    - Engine class manages all core systems
 *    - Returns false if initialization fails
 *    - Sets up renderer, resource manager, etc.
 * 
 * 3. Clean Shutdown
 *    - Always call shutdown() before exit
 *    - Ensures proper resource cleanup
 *    - Prevents memory leaks
 * 
 * WHAT HAPPENS:
 * - Logger starts and writes to log files
 * - Engine initializes DirectX renderer
 * - Core systems are loaded and verified
 * - Everything shuts down cleanly
 * 
 * COMMON ISSUES:
 * ❌ DirectX not available - need Windows 10/11
 * ❌ Missing dependencies - run vcpkg install
 * ❌ Build errors - see BUILD.md for help
 */
