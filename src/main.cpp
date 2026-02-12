#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "core/Engine.h"
#include "core/Logger.h"

int main(int argc, char* argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

#ifdef _WIN32
    // Enable per-monitor DPI awareness to prevent Windows from scaling the
    // application.  Without this, logical and physical pixel coordinates can
    // diverge which causes panel layout gaps and misaligned rendering.
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
        // V2 not available (older Windows), try V1
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    }
#endif

    // Initialize logger first
    fresh::Logger::getInstance().initialize();

    std::cout << "Fresh Voxel Engine - Starting..." << std::endl;
    LOG_INFO("Fresh Voxel Engine starting...");

    try {
        fresh::Engine engine;

        if (!engine.initialize()) {
            std::cerr << "Failed to initialize engine" << std::endl;
            LOG_ERROR("Failed to initialize engine");
            fresh::Logger::getInstance().shutdown();
            return EXIT_FAILURE;
        }

        std::cout << "Engine initialized successfully" << std::endl;
        LOG_INFO("Engine initialized successfully");
        std::cout << "Running main loop..." << std::endl;

        engine.run();

        std::cout << "Shutting down..." << std::endl;
        LOG_INFO("Engine shutting down...");
        engine.shutdown();

        std::cout << "Engine shutdown complete" << std::endl;
        LOG_INFO("Engine shutdown complete");

        // Shutdown logger
        fresh::Logger::getInstance().shutdown();
        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        LOG_FATAL(std::string("Fatal error: ") + e.what());
        fresh::Logger::getInstance().shutdown();
        return EXIT_FAILURE;
    }
}
