/**
 * @file 03_logging_demo.cpp
 * @brief Comprehensive logging system demonstration
 * 
 * This example demonstrates:
 * - Different log levels (INFO, WARNING, ERROR, FATAL)
 * - Component-specific logging
 * - Log file organization
 * - Debugging techniques
 * 
 * The logging system is essential for debugging and development.
 */

#include "core/Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Fresh Voxel Engine - Logging System Demo\n";
    std::cout << "==========================================\n\n";
    
    // Initialize logger
    fresh::Logger::getInstance().initialize();
    
    std::cout << "Demonstrating logging at different levels...\n\n";
    
    // 1. Info messages - general information
    std::cout << "[1] Info Messages\n";
    LOG_INFO("Application started");
    LOG_INFO_C("Renderer initialized", "Renderer");
    LOG_INFO_C("Loading textures...", "ResourceManager");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "✓ Info messages logged\n\n";
    
    // 2. Warning messages - potential issues
    std::cout << "[2] Warning Messages\n";
    LOG_WARNING("Texture not found, using fallback");
    LOG_WARNING_C("Low memory detected", "MemoryManager");
    LOG_WARNING_C("Frame rate dropped below 30 FPS", "Performance");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "✓ Warning messages logged\n\n";
    
    // 3. Error messages - recoverable errors
    std::cout << "[3] Error Messages\n";
    LOG_ERROR("Failed to load shader");
    LOG_ERROR_C("Could not connect to server", "Network");
    LOG_ERROR_C("Audio device not available", "Audio");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "✓ Error messages logged\n\n";
    
    // 4. Fatal messages - critical errors
    std::cout << "[4] Fatal Message (for demonstration only)\n";
    LOG_FATAL("Critical system failure - this is just a test");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "✓ Fatal message logged\n\n";
    
    // 5. Component-based logging
    std::cout << "[5] Component-Based Logging\n";
    LOG_INFO_C("Chunk loaded at (0, 0, 0)", "VoxelWorld");
    LOG_INFO_C("Player spawned", "PlayerController");
    LOG_INFO_C("Physics system ready", "PhysicsEngine");
    LOG_INFO_C("AI system initialized", "AISystem");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "✓ Component messages logged\n\n";
    
    // Shutdown
    std::cout << "Shutting down logger...\n";
    fresh::Logger::getInstance().shutdown();
    
    std::cout << "\n==========================================\n";
    std::cout << "Logging demo complete!\n\n";
    std::cout << "Check the logs/ directory for:\n";
    std::cout << "  • application_errors_<timestamp>.txt - All logs\n";
    std::cout << "  • Environment/Windows/ - Platform-specific logs\n\n";
    std::cout << "Tips:\n";
    std::cout << "  • Use INFO for general information\n";
    std::cout << "  • Use WARNING for potential issues\n";
    std::cout << "  • Use ERROR for recoverable errors\n";
    std::cout << "  • Use FATAL for critical failures\n";
    std::cout << "  • Add component names for better organization\n";
    
    return 0;
}

/**
 * LOGGING BEST PRACTICES:
 * 
 * 1. Log Levels
 *    INFO    - Normal operations, milestones
 *    WARNING - Potential issues, fallbacks used
 *    ERROR   - Errors that don't crash the app
 *    FATAL   - Critical errors, app will terminate
 * 
 * 2. Component Names
 *    - Use consistent component names
 *    - Examples: "Renderer", "Physics", "Audio"
 *    - Makes filtering logs easier
 * 
 * 3. Message Content
 *    ✓ Include context: "Failed to load texture: stone.png"
 *    ✗ Vague: "Error occurred"
 *    ✓ Include values: "FPS: 45 (target: 60)"
 *    ✗ Missing details: "Low FPS"
 * 
 * 4. When to Log
 *    ✓ Initialization/shutdown
 *    ✓ Resource loading
 *    ✓ Errors and warnings
 *    ✓ State changes
 *    ✗ Every frame (too much data)
 *    ✗ Inner loop iterations
 * 
 * LOG FILE ORGANIZATION:
 * 
 * logs/
 *   ├── application_errors_<timestamp>.txt  // Main log
 *   └── Environment/
 *       └── Windows/
 *           └── errors_<timestamp>.txt      // Platform-specific
 * 
 * USING LOGS FOR DEBUGGING:
 * 
 * 1. Reproduce the issue
 * 2. Check latest log file
 * 3. Look for ERROR or FATAL messages
 * 4. Check timestamps to correlate events
 * 5. Add more logging if needed
 */
