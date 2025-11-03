cmake_minimum_required(VERSION 3.20)
project(FreshVoxelEngine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Graphics API Dependencies
# OpenGL is available on most systems
message(STATUS "OpenGL backend will be available (system OpenGL)")
add_definitions(-DFRESH_OPENGL_SUPPORT)

# DirectX is only available on Windows
if(WIN32)
    message(STATUS "DirectX 11/12 backends will be available (Windows)")
    add_definitions(-DFRESH_DIRECTX_SUPPORT)
endif()

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

# Enable GLM experimental features
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# Source files
set(ENGINE_SOURCES
    src/main.cpp
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/Logger.cpp
    src/core/MemoryManager.cpp
    src/core/ResourceManager.cpp
    src/core/SceneManager.cpp
    src/renderer/ShaderManager.cpp
    src/renderer/RenderContext.cpp
    src/renderer/Material.cpp
    src/renderer/Texture.cpp
    src/renderer/TextureManager.cpp
    src/renderer/VoxelTextureLoader.cpp
    src/renderer/backends/OpenGLRenderContext.cpp
    src/voxel/Chunk.cpp
    src/voxel/VoxelWorld.cpp
    src/voxel/MeshGenerator.cpp
    src/voxel/ChunkStreamer.cpp
    src/generation/NoiseGenerator.cpp
    src/generation/TerrainGenerator.cpp
    src/physics/PhysicsSystem.cpp
    src/physics/CollisionDetection.cpp
    src/physics/PhysicsComponent.cpp
    src/physics/PhysicsSystemEnhanced.cpp
    src/interaction/RaycastSystem.cpp
    src/ai/AISystem.cpp
    src/ai/BehaviorTree.cpp
    src/audio/AudioEngine.cpp
    src/scripting/EventSystem.cpp
    src/editor/EditorGUI.cpp
    src/editor/WorldEditor.cpp
    src/editor/TerraformingSystem.cpp
    src/serialization/WorldSerializer.cpp
    src/ui/MainMenu.cpp
    src/input/InputManager.cpp
    src/gameplay/Camera.cpp
    src/gameplay/Player.cpp
    src/assets/ModularAssetSystem.cpp
    src/ecs/EntityManager.cpp
    src/voxelship/VoxelBlock.cpp
    src/voxelship/VoxelStructureComponent.cpp
    src/galaxy/GalaxySector.cpp
    src/galaxy/GalaxyGenerator.cpp
    src/rpg/Inventory.cpp
    src/rpg/InventoryComponent.cpp
    src/rpg/CraftingSystem.cpp
    src/rpg/ProgressionComponent.cpp
    src/rpg/FactionComponent.cpp
    src/rpg/LootSystem.cpp
    src/rpg/TradingSystem.cpp
    src/networking/NetworkMessage.cpp
    src/networking/ClientConnection.cpp
    src/networking/GameServer.cpp
    src/networking/SectorServer.cpp
    src/devtools/DebugRenderer.cpp
    src/devtools/PerformanceProfiler.cpp
    src/devtools/MemoryTracker.cpp
    src/devtools/DebugConsole.cpp
    src/devtools/DevToolsManager.cpp
    src/scripting/lua/ScriptingEngine.cpp
)

# Add DirectX backends on Windows
if(WIN32)
    list(APPEND ENGINE_SOURCES
        src/renderer/backends/DirectX11RenderContext.cpp
        src/renderer/backends/DirectX12RenderContext.cpp
    )
endif()

# Header files
set(ENGINE_HEADERS
    include/core/Engine.h
    include/core/Window.h
    include/core/Logger.h
    include/core/MemoryManager.h
    include/core/ResourceManager.h
    include/core/SceneManager.h
    include/renderer/ShaderManager.h
    include/voxel/Chunk.h
    include/voxel/VoxelWorld.h
    include/voxel/VoxelTypes.h
    include/voxel/MeshGenerator.h
    include/voxel/ChunkStreamer.h
    include/generation/NoiseGenerator.h
    include/generation/TerrainGenerator.h
    include/physics/PhysicsSystem.h
    include/physics/CollisionDetection.h
    include/physics/PhysicsComponent.h
    include/physics/PhysicsSystemEnhanced.h
    include/interaction/RaycastSystem.h
    include/ai/AISystem.h
    include/ai/BehaviorTree.h
    include/audio/AudioEngine.h
    include/scripting/EventSystem.h
    include/editor/EditorGUI.h
    include/editor/WorldEditor.h
    include/editor/TerraformingSystem.h
    include/serialization/WorldSerializer.h
    include/ui/MainMenu.h
    include/input/InputManager.h
    include/gameplay/Camera.h
    include/gameplay/Player.h
    include/assets/ModularAssetSystem.h
    include/ecs/Entity.h
    include/ecs/IComponent.h
    include/ecs/EntityManager.h
    include/ecs/SystemBase.h
    include/voxelship/VoxelBlock.h
    include/voxelship/VoxelStructureComponent.h
    include/galaxy/GalaxySector.h
    include/galaxy/GalaxyGenerator.h
    include/rpg/Inventory.h
    include/rpg/InventoryComponent.h
    include/rpg/CraftingSystem.h
    include/rpg/ProgressionComponent.h
    include/rpg/FactionComponent.h
    include/rpg/LootSystem.h
    include/rpg/TradingSystem.h
    include/networking/NetworkMessage.h
    include/networking/ClientConnection.h
    include/networking/GameServer.h
    include/networking/SectorServer.h
    include/devtools/DebugRenderer.h
    include/devtools/PerformanceProfiler.h
    include/devtools/MemoryTracker.h
    include/devtools/DebugConsole.h
    include/devtools/DevToolsManager.h
    include/scripting/lua/ScriptingEngine.h
)

# Create executable
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Windows-specific: Make it a proper Windows GUI application (no console in Release)
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
endif()

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
# Link OpenGL (required for OpenGL backend)
find_package(OpenGL QUIET)
if(OpenGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
else()
    # Fallback for Windows if CMake can't find OpenGL package
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
    endif()
endif()

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

# Link DirectX on Windows (system libraries)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        d3d11.lib 
        d3d12.lib 
        dxgi.lib 
        d3dcompiler.lib
    )
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

# ==============================================================================
# Testing Support
# ==============================================================================

# Option to enable/disable tests
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    message(STATUS "Building tests enabled")
    
    # Enable testing
    enable_testing()
    
    # Find Google Test
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        message(STATUS "Google Test not found, fetching from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
    
    # Test source files
    set(TEST_SOURCES
        tests/voxel/ChunkTests.cpp
        tests/generation/NoiseTests.cpp
        tests/generation/TerrainGeneratorTests.cpp
        tests/core/MemoryManagerTests.cpp
        tests/scripting/EventSystemTests.cpp
    )
    
    # Engine source files needed for tests (exclude main.cpp)
    set(TEST_ENGINE_SOURCES
        src/voxel/Chunk.cpp
        src/generation/NoiseGenerator.cpp
        src/generation/TerrainGenerator.cpp
        src/core/MemoryManager.cpp
        src/core/Logger.cpp
        src/scripting/EventSystem.cpp
    )
    
    # Create test executable
    add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES} ${TEST_ENGINE_SOURCES})
    
    # Include directories for tests
    target_include_directories(${PROJECT_NAME}Tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    # Link test executable with Google Test and engine libraries
    target_link_libraries(${PROJECT_NAME}Tests PRIVATE
        GTest::gtest
        GTest::gtest_main
    )
    
    # Link GLFW if needed for tests
    if(glfw3_FOUND)
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE glfw)
    elseif(GLFW_FOUND)
        target_include_directories(${PROJECT_NAME}Tests PRIVATE ${GLFW_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE ${GLFW_LIBRARIES})
    endif()
    
    # Link GLM if found
    if(glm_FOUND)
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE glm::glm)
    endif()
    
    # Compiler settings for tests
    target_compile_features(${PROJECT_NAME}Tests PRIVATE cxx_std_17)
    
    if(MSVC)
        target_compile_options(${PROJECT_NAME}Tests PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME}Tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}Tests)
    
    message(STATUS "Tests configured successfully")
    message(STATUS "Run tests with: ctest or ./FreshVoxelEngineTests")
endif()
