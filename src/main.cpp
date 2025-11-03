#include "core/Engine.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::cout << "Fresh Voxel Engine - Starting..." << std::endl;
    
    try {
        fresh::Engine engine;
        
        if (!engine.initialize()) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return EXIT_FAILURE;
        }
        
        std::cout << "Engine initialized successfully" << std::endl;
        std::cout << "Running main loop..." << std::endl;
        
        engine.run();
        
        std::cout << "Shutting down..." << std::endl;
        engine.shutdown();
        
        std::cout << "Engine shutdown complete" << std::endl;
        return EXIT_SUCCESS;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
