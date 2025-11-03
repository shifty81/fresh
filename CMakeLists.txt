cmake_minimum_required(VERSION 3.20)
project(FreshVoxelEngine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Try to find GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found via find_package, trying pkg-config")
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(GLFW QUIET glfw3)
    endif()
    
    if(NOT GLFW_FOUND)
        message(WARNING "GLFW not found. Please install GLFW 3.3+ or set GLFW path manually.")
        message(WARNING "On Ubuntu/Debian: sudo apt-get install libglfw3-dev")
        message(WARNING "On Fedora: sudo dnf install glfw-devel")
        message(WARNING "On macOS: brew install glfw")
    endif()
endif()

# GLM for math
find_package(glm QUIET)
if(NOT glm_FOUND)
    message(STATUS "GLM not found, will use header-only include if available")
endif()

# Source files
set(ENGINE_SOURCES
    src/main.cpp
    src/core/Engine.cpp
    src/core/Window.cpp
    src/renderer/VulkanRenderer.cpp
    src/renderer/VulkanDevice.cpp
    src/renderer/VulkanPipeline.cpp
    src/renderer/ShaderManager.cpp
    src/voxel/Chunk.cpp
    src/voxel/VoxelWorld.cpp
    src/voxel/MeshGenerator.cpp
    src/generation/NoiseGenerator.cpp
    src/generation/TerrainGenerator.cpp
    src/physics/PhysicsSystem.cpp
    src/interaction/RaycastSystem.cpp
    src/ai/AISystem.cpp
    src/ai/BehaviorTree.cpp
    src/editor/EditorGUI.cpp
    src/editor/WorldEditor.cpp
    src/editor/TerraformingSystem.cpp
    src/serialization/WorldSerializer.cpp
    src/ui/MainMenu.cpp
)

# Header files
set(ENGINE_HEADERS
    include/core/Engine.h
    include/core/Window.h
    include/renderer/VulkanRenderer.h
    include/renderer/VulkanDevice.h
    include/renderer/VulkanPipeline.h
    include/renderer/ShaderManager.h
    include/voxel/Chunk.h
    include/voxel/VoxelWorld.h
    include/voxel/VoxelTypes.h
    include/voxel/MeshGenerator.h
    include/generation/NoiseGenerator.h
    include/generation/TerrainGenerator.h
    include/physics/PhysicsSystem.h
    include/interaction/RaycastSystem.h
    include/ai/AISystem.h
    include/ai/BehaviorTree.h
    include/editor/EditorGUI.h
    include/editor/WorldEditor.h
    include/editor/TerraformingSystem.h
    include/serialization/WorldSerializer.h
    include/ui/MainMenu.h
)

# Create executable
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Vulkan::Vulkan
)

# Link GLFW
if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
elseif(GLFW_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})
endif()

# Link GLM if found
if(glm_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy shaders to build directory
file(GLOB SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert" 
                       "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    configure_file(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY shaders/ DESTINATION bin/shaders)
