// Example: Using the Fresh Voxel Engine Logging System
// This example demonstrates the comprehensive logging capabilities

#include "core/Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

// Simulate a subsystem that logs messages
class ExampleSubsystem {
public:
    void initialize() {
        LOG_INFO_C("Initializing example subsystem", "ExampleSubsystem");
        
        // Simulate some initialization work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        LOG_INFO_C("Example subsystem initialized successfully", "ExampleSubsystem");
    }
    
    void performWork() {
        LOG_INFO_C("Starting work...", "ExampleSubsystem");
        
        // Simulate a warning scenario
        LOG_WARNING_C("Resource usage is high (75%)", "ExampleSubsystem");
        
        // Simulate an error scenario
        LOG_ERROR_C("Failed to load optional resource 'optional_texture.png'", "ExampleSubsystem");
        
        LOG_INFO_C("Work completed with warnings", "ExampleSubsystem");
    }
    
    void shutdown() {
        LOG_INFO_C("Shutting down example subsystem", "ExampleSubsystem");
    }
};

int main() {
    std::cout << "=== Fresh Voxel Engine - Logging Example ===" << std::endl;
    std::cout << std::endl;
    
    // Initialize the logging system
    std::cout << "Initializing logger..." << std::endl;
    if (!fresh::Logger::getInstance().initialize()) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        return 1;
    }
    
    // Log application startup
    LOG_INFO("Application started");
    LOG_INFO_C("Fresh Voxel Engine Example Application", "Main");
    
    // Create and use a subsystem
    ExampleSubsystem subsystem;
    subsystem.initialize();
    subsystem.performWork();
    
    // Demonstrate different log levels
    LOG_INFO("Demonstrating different log levels");
    LOG_WARNING("This is a warning - something might be wrong");
    LOG_ERROR("This is an error - something went wrong");
    
    // Simulate a critical error (but don't crash)
    LOG_FATAL("This is a fatal error - but we're handling it gracefully");
    
    // Cleanup
    subsystem.shutdown();
    LOG_INFO("Application shutting down");
    
    // Shutdown logger
    fresh::Logger::getInstance().shutdown();
    
    std::cout << std::endl;
    std::cout << "=== Example Complete ===" << std::endl;
    std::cout << "Check the logs/ directory for output files:" << std::endl;
    std::cout << "  - logs/application_errors_<timestamp>.txt (all logs)" << std::endl;
    std::cout << "  - logs/Environment/<Platform>/errors_<timestamp>.txt (errors only)" << std::endl;
    std::cout << std::endl;
    
    return 0;
}
