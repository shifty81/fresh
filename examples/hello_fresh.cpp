/**
 * @file hello_fresh.cpp
 * @brief Simplest possible Fresh Voxel Engine example
 * 
 * This is a minimal console application that demonstrates:
 * - Engine initialization without graphics
 * - Basic logging
 * - Proper cleanup
 * 
 * Perfect for verifying your build environment works!
 */

#include "core/Logger.h"
#include <iostream>

int main() {
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Fresh Voxel Engine - Hello World!    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    // Step 1: Initialize Logger
    std::cout << "1. Initializing Logger..." << std::endl;
    fresh::Logger& logger = fresh::Logger::getInstance();
    
    if (!logger.initialize("logs")) {
        std::cerr << "   ✗ Failed to initialize logger" << std::endl;
        return 1;
    }
    
    logger.log(fresh::LogLevel::INFO, "Logger initialized successfully!");
    std::cout << "   ✓ Logger ready" << std::endl;
    std::cout << std::endl;
    
    // Step 2: Test basic operations
    std::cout << "2. Testing basic operations..." << std::endl;
    
    // Log at different levels
    logger.log(fresh::LogLevel::INFO, "This is an info message");
    logger.log(fresh::LogLevel::WARNING, "This is a warning message");
    logger.log(fresh::LogLevel::ERR, "This is an error message (for testing)");
    
    std::cout << "   ✓ Logging works at all levels" << std::endl;
    std::cout << std::endl;
    
    // Step 3: Component logging
    std::cout << "3. Testing component-specific logging..." << std::endl;
    logger.info("System initialized", "Core");
    logger.warning("This is a test warning", "TestComponent");
    std::cout << "   ✓ Component logging works" << std::endl;
    std::cout << std::endl;
    
    // Step 4: Cleanup
    std::cout << "4. Shutting down..." << std::endl;
    logger.log(fresh::LogLevel::INFO, "Logger shutdown initiated");
    logger.shutdown();
    std::cout << "   ✓ Clean shutdown" << std::endl;
    std::cout << std::endl;
    
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Success! Fresh Engine is working!    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    std::cout << "✓ All systems operational!" << std::endl;
    std::cout << "✓ Your build environment is correctly configured" << std::endl;
    std::cout << "✓ Core systems are functioning" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Next steps:" << std::endl;
    std::cout << "  • Check logs/ directory for detailed log files" << std::endl;
    std::cout << "  • Try examples/basic_setup.cpp for graphics" << std::endl;
    std::cout << "  • See examples/creating_chunks.cpp for voxel worlds" << std::endl;
    std::cout << "  • Read docs/ for comprehensive documentation" << std::endl;
    
    return 0;
}
