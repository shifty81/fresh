cmake_minimum_required(VERSION 3.20)
project(FreshVoxelEngine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================================
# Platform Detection and Graphics API Configuration
# ==============================================================================

# Fresh Voxel Engine is now Windows-only for gaming and professional editor development
# Temporarily allow Linux builds for CI testing purposes
if(NOT WIN32)
    message(WARNING "Fresh Voxel Engine is primarily designed for Windows 10/11 (x64).")
    message(WARNING "Building on Linux for testing purposes only - some features may not work.")
    set(ENABLE_DIRECTX OFF)
    set(ENABLE_OPENGL ON)
else()
    message(STATUS "Configuring Fresh Voxel Engine for Windows Gaming Platform")
    message(STATUS "  - Platform: Windows 10/11 (x64)")
    message(STATUS "  - Primary Renderer: DirectX 12")
    message(STATUS "  - Fallback Renderer: DirectX 11")
    message(STATUS "  - Optional: OpenGL support (for compatibility)")
    message(STATUS "  - Editor: Windows-native Unreal-like game development environment")
    set(ENABLE_DIRECTX ON)
    set(ENABLE_OPENGL ON)
endif()

# Enable graphics backends based on platform
if(WIN32)
    add_definitions(-DFRESH_DIRECTX_SUPPORT)
    add_definitions(-DFRESH_WINDOWS_ONLY)
    add_definitions(-DNOMINMAX)  # Prevent Windows.h from defining min/max macros
    add_definitions(-DUNICODE -D_UNICODE)  # Use Unicode (wide character) Windows APIs
endif()
if(ENABLE_OPENGL)
    add_definitions(-DFRESH_OPENGL_SUPPORT)
endif()

# ==============================================================================
# Dependencies
# ==============================================================================

# Detect if we're using vcpkg for automatic dependency management
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    set(USING_VCPKG TRUE)
    message(STATUS "Using vcpkg for dependency management")
    message(STATUS "Dependencies will be automatically installed from vcpkg.json")
else()
    set(USING_VCPKG FALSE)
    message(STATUS "Not using vcpkg - dependencies must be pre-installed")
endif()

# Try to find GLFW (required for windowing)
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found via find_package, trying pkg-config")
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(GLFW QUIET glfw3)
    endif()
    
    if(NOT GLFW_FOUND)
        message(WARNING "GLFW not found. Please install GLFW 3.3+")
        message(WARNING "  Windows: vcpkg install glfw3:x64-windows")
    endif()
endif()

# GLM for math (header-only)
find_package(glm QUIET)
if(NOT glm_FOUND)
    message(STATUS "GLM not found, will use header-only include if available")
endif()

# OpenGL and GLEW (for OpenGL extension loading on non-Apple platforms)
if(ENABLE_OPENGL)
    find_package(OpenGL QUIET)
    if(OpenGL_FOUND)
        message(STATUS "OpenGL found")
    else()
        message(WARNING "OpenGL not found. OpenGL backend will not be available.")
    endif()
    
    # GLEW is needed on Windows for OpenGL extension loading (optional)
    find_package(GLEW QUIET)
    if(GLEW_FOUND)
        message(STATUS "GLEW found for OpenGL extension loading")
        add_definitions(-DFRESH_GLEW_AVAILABLE)
    else()
        message(WARNING "GLEW not found. OpenGL support will be limited.")
        message(WARNING "  Install via: vcpkg install glew:x64-windows")
    endif()
endif()

# OpenAL for audio
find_package(OpenAL QUIET)
if(NOT OpenAL_FOUND)
    message(WARNING "OpenAL not found. Audio features will be disabled.")
    message(WARNING "  vcpkg install openal-soft:x64-windows")
else()
    message(STATUS "OpenAL found successfully")
endif()

# Vorbis for OGG audio support
find_package(Vorbis QUIET CONFIG)
if(NOT Vorbis_FOUND)
    message(WARNING "libvorbis not found. OGG audio support will be disabled.")
    message(WARNING "  vcpkg install libvorbis:x64-windows")
else()
    message(STATUS "libvorbis found successfully")
endif()

# Lua for scripting
# On Windows with vcpkg, prefer CONFIG mode to get proper target
if(USING_VCPKG)
    # Extract vcpkg root path once for reuse in fallback logic
    if(DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VCPKG_ROOT)
        get_filename_component(VCPKG_ROOT "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
        get_filename_component(VCPKG_ROOT "${VCPKG_ROOT}" DIRECTORY)
        get_filename_component(VCPKG_ROOT "${VCPKG_ROOT}" DIRECTORY)
    endif()
    
    # Try vcpkg's CONFIG mode first on Windows
    # Try LuaJIT first (has better C linkage support on Windows)
    find_package(luajit CONFIG QUIET)
    if(luajit_FOUND OR LUAJIT_FOUND)
        set(Lua_FOUND TRUE)
        set(LUA_FOUND_VIA_CONFIG TRUE)
        set(USING_LUAJIT TRUE)
        message(STATUS "LuaJIT found via vcpkg CONFIG mode")
        
        # Get LuaJIT variables from the config
        if(NOT DEFINED LUA_INCLUDE_DIR AND DEFINED LUAJIT_INCLUDE_DIR)
            set(LUA_INCLUDE_DIR ${LUAJIT_INCLUDE_DIR})
        elseif(NOT DEFINED LUA_INCLUDE_DIR AND DEFINED luajit_INCLUDE_DIR)
            set(LUA_INCLUDE_DIR ${luajit_INCLUDE_DIR})
        endif()
        if(NOT DEFINED LUA_LIBRARIES AND DEFINED LUAJIT_LIBRARIES)
            set(LUA_LIBRARIES ${LUAJIT_LIBRARIES})
        elseif(NOT DEFINED LUA_LIBRARIES AND DEFINED luajit_LIBRARIES)
            set(LUA_LIBRARIES ${luajit_LIBRARIES})
        endif()
        
        # Try to extract include directory from target properties if not already set
        if(NOT DEFINED LUA_INCLUDE_DIR)
            if(TARGET luajit)
                get_target_property(LUAJIT_INCLUDE_DIRS luajit INTERFACE_INCLUDE_DIRECTORIES)
                if(LUAJIT_INCLUDE_DIRS)
                    set(LUA_INCLUDE_DIR ${LUAJIT_INCLUDE_DIRS})
                endif()
            elseif(TARGET luajit::luajit)
                get_target_property(LUAJIT_INCLUDE_DIRS luajit::luajit INTERFACE_INCLUDE_DIRECTORIES)
                if(LUAJIT_INCLUDE_DIRS)
                    set(LUA_INCLUDE_DIR ${LUAJIT_INCLUDE_DIRS})
                endif()
            endif()
        endif()
        
        # Final fallback: try to find lua.h in vcpkg installed directory
        if(NOT DEFINED LUA_INCLUDE_DIR)
            # Try with VCPKG_TARGET_TRIPLET if defined
            if(DEFINED VCPKG_TARGET_TRIPLET)
                if(EXISTS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/luajit.h")
                    set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
                    message(STATUS "  Found LuaJIT headers in vcpkg: ${LUA_INCLUDE_DIR}")
                elseif(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include/luajit.h")
                    set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
                    message(STATUS "  Found LuaJIT headers in build vcpkg: ${LUA_INCLUDE_DIR}")
                # Also check for lua.h as a fallback (in case luajit.h is not present)
                elseif(EXISTS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/lua.h")
                    set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
                    message(STATUS "  Found Lua headers in vcpkg: ${LUA_INCLUDE_DIR}")
                elseif(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include/lua.h")
                    set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
                    message(STATUS "  Found Lua headers in build vcpkg: ${LUA_INCLUDE_DIR}")
                endif()
            endif()
            
            # Fallback: Try common Windows triplet if not found
            if(NOT DEFINED LUA_INCLUDE_DIR AND WIN32)
                if(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include/luajit.h")
                    set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include")
                    message(STATUS "  Found LuaJIT headers in build vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                elseif(EXISTS "${VCPKG_ROOT}/installed/x64-windows/include/luajit.h")
                    set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include")
                    message(STATUS "  Found LuaJIT headers in vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                # Also check for lua.h as a fallback (in case luajit.h is not present)
                elseif(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include/lua.h")
                    set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include")
                    message(STATUS "  Found Lua headers in build vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                elseif(EXISTS "${VCPKG_ROOT}/installed/x64-windows/include/lua.h")
                    set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include")
                    message(STATUS "  Found Lua headers in vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                endif()
            endif()
        endif()
    else()
        # Fall back to standard Lua
        find_package(lua CONFIG QUIET)
        if(lua_FOUND)
            set(Lua_FOUND TRUE)
            set(LUA_FOUND_VIA_CONFIG TRUE)
            message(STATUS "Lua found via vcpkg CONFIG mode")
            
            # Get Lua variables from the config
            # Some vcpkg versions set these, some don't
            if(NOT DEFINED LUA_INCLUDE_DIR AND DEFINED lua_INCLUDE_DIR)
                set(LUA_INCLUDE_DIR ${lua_INCLUDE_DIR})
            endif()
            if(NOT DEFINED LUA_LIBRARIES AND DEFINED lua_LIBRARIES)
                set(LUA_LIBRARIES ${lua_LIBRARIES})
            endif()
            
            # Try to extract include directory from target properties if not already set
            if(NOT DEFINED LUA_INCLUDE_DIR)
                if(TARGET lua)
                    get_target_property(LUA_INCLUDE_DIRS lua INTERFACE_INCLUDE_DIRECTORIES)
                    if(LUA_INCLUDE_DIRS)
                        set(LUA_INCLUDE_DIR ${LUA_INCLUDE_DIRS})
                    endif()
                elseif(TARGET lua::lua)
                    get_target_property(LUA_INCLUDE_DIRS lua::lua INTERFACE_INCLUDE_DIRECTORIES)
                    if(LUA_INCLUDE_DIRS)
                        set(LUA_INCLUDE_DIR ${LUA_INCLUDE_DIRS})
                    endif()
                endif()
            endif()
            
            # Final fallback: try to find lua.h in vcpkg installed directory
            if(NOT DEFINED LUA_INCLUDE_DIR)
                # Try with VCPKG_TARGET_TRIPLET if defined
                if(DEFINED VCPKG_TARGET_TRIPLET)
                    if(EXISTS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/lua.h")
                        set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
                        message(STATUS "  Found Lua headers in vcpkg: ${LUA_INCLUDE_DIR}")
                    elseif(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include/lua.h")
                        set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
                        message(STATUS "  Found Lua headers in build vcpkg: ${LUA_INCLUDE_DIR}")
                    endif()
                endif()
                
                # Fallback: Try common Windows triplet if not found
                if(NOT DEFINED LUA_INCLUDE_DIR AND WIN32)
                    if(EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include/lua.h")
                        set(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/include")
                        message(STATUS "  Found Lua headers in build vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                    elseif(EXISTS "${VCPKG_ROOT}/installed/x64-windows/include/lua.h")
                        set(LUA_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include")
                        message(STATUS "  Found Lua headers in vcpkg (x64-windows): ${LUA_INCLUDE_DIR}")
                    endif()
                endif()
            endif()
        endif()
    endif()
endif()

if(NOT Lua_FOUND)
    # Try standard FindLua module (works with system installations)
    find_package(Lua QUIET)
    if(Lua_FOUND)
        message(STATUS "Lua found via FindLua module")
    endif()
endif()

if(NOT Lua_FOUND)
    message(WARNING "Lua not found. Asset management Lua integration will be disabled.")
    message(WARNING "  vcpkg install luajit:x64-windows")
    message(WARNING "  or vcpkg install lua:x64-windows")
endif()

# Sol2 for modern C++ Lua bindings
if(Lua_FOUND)
    find_package(sol2 QUIET CONFIG)
    if(NOT sol2_FOUND)
        message(WARNING "Sol2 not found. Lua scripting will use basic interface only.")
        message(WARNING "  vcpkg install sol2:x64-windows")
    else()
        message(STATUS "Sol2 found successfully")
        set(SOL2_AVAILABLE TRUE)
    endif()
endif()

# nlohmann-json for JSON parsing
find_package(nlohmann_json QUIET CONFIG)
if(NOT nlohmann_json_FOUND)
    message(WARNING "nlohmann-json not found. Asset pack manifest parsing will be limited.")
    message(WARNING "  vcpkg install nlohmann-json:x64-windows")
else()
    message(STATUS "nlohmann-json found successfully")
endif()

# Native File Dialog Extended for file dialogs
find_package(nfd QUIET CONFIG)
if(NOT nfd_FOUND)
    message(WARNING "Native File Dialog Extended (nfd) not found. File dialogs will not be available.")
    message(WARNING "  vcpkg install nativefiledialog-extended:x64-windows")
else()
    message(STATUS "Native File Dialog Extended found successfully")
endif()

# tinyobjloader for OBJ model loading
find_package(tinyobjloader QUIET CONFIG)
if(NOT tinyobjloader_FOUND)
    message(WARNING "tinyobjloader not found. OBJ model loading will be disabled.")
    message(WARNING "  vcpkg install tinyobjloader:x64-windows")
else()
    message(STATUS "tinyobjloader found successfully")
    set(TINYOBJLOADER_AVAILABLE TRUE)
endif()

message(STATUS "All find_package calls completed")

# ==============================================================================
# Dependency Check Summary
# ==============================================================================

# Check if critical dependencies are available
set(GLFW_AVAILABLE FALSE)
if(glfw3_FOUND OR GLFW_FOUND)
    set(GLFW_AVAILABLE TRUE)
endif()

set(DEPENDENCIES_OK TRUE)
set(MISSING_DEPS "")

if(NOT GLFW_AVAILABLE)
    set(DEPENDENCIES_OK FALSE)
    list(APPEND MISSING_DEPS "GLFW")
endif()

if(NOT glm_FOUND)
    set(DEPENDENCIES_OK FALSE)
    list(APPEND MISSING_DEPS "GLM")
endif()

# OpenGL dependencies (optional for Windows compatibility)
if(ENABLE_OPENGL)
    if(NOT OpenGL_FOUND)
        set(DEPENDENCIES_OK FALSE)
        list(APPEND MISSING_DEPS "OpenGL")
    endif()
    
    # GLEW is required on Windows for OpenGL extension loading
    if(NOT GLEW_FOUND)
        set(DEPENDENCIES_OK FALSE)
        list(APPEND MISSING_DEPS "GLEW")
    endif()
endif()

# If not using vcpkg and dependencies are missing, provide clear guidance
if(NOT USING_VCPKG AND NOT DEPENDENCIES_OK)
    message("")
    message("========================================================================")
    message(STATUS "DEPENDENCY CHECK FAILED")
    message("========================================================================")
    message(STATUS "The following required dependencies are missing:")
    foreach(dep ${MISSING_DEPS})
        message(STATUS "  - ${dep}")
    endforeach()
    message("")
    message(STATUS "To resolve this issue:")
    message("")
    message(STATUS "Use vcpkg for automatic dependency management (RECOMMENDED)")
    message(STATUS "  From the project root directory, run:")
    message(STATUS "     git clone https://github.com/Microsoft/vcpkg.git")
    message(STATUS "     cd vcpkg")
    message(STATUS "     bootstrap-vcpkg.bat")
    message(STATUS "     cd ..")
    message(STATUS "     generate_vs2022.bat")
    message("")
    message(STATUS "Alternative: Install dependencies manually")
    message(STATUS "  Download and install each library, then set CMAKE_PREFIX_PATH")
    message(STATUS "  See BUILD.md for detailed instructions")
    message("")
    message(WARNING "CMake configuration will continue, but compilation will fail without these dependencies!")
    message("========================================================================")
    message("")
elseif(NOT USING_VCPKG AND DEPENDENCIES_OK)
    message("")
    message("========================================================================")
    message(STATUS "All required dependencies found!")
    message("========================================================================")
    message("")
endif()

message(STATUS "Dependency checking completed")

# Enable GLM experimental features
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

message(STATUS "Preparing to define source files...")

# Source files
set(ENGINE_SOURCES
    src/main.cpp
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/Logger.cpp
    src/core/MemoryManager.cpp
    src/core/ResourceManager.cpp
    src/core/SceneManager.cpp
    src/core/Reflection.cpp
    src/renderer/ShaderManager.cpp
    src/renderer/RenderContext.cpp
    src/renderer/Material.cpp
    src/renderer/Texture.cpp
    src/renderer/stb_image_impl.cpp
    src/renderer/ModelLoader.cpp
    src/renderer/TextureManager.cpp
    src/renderer/VoxelTextureLoader.cpp
    src/voxel/Chunk.cpp
    src/voxel/VoxelWorld.cpp
    src/voxel/MeshGenerator.cpp
    src/voxel/ChunkStreamer.cpp
    src/generation/NoiseGenerator.cpp
    src/generation/TerrainGenerator.cpp
    src/generation/World2DGenerator.cpp
    src/generation/WorldGeneratorFactory.cpp
    src/generation/WorldGenerationTask.cpp
    src/physics/PhysicsSystem.cpp
    src/physics/CollisionDetection.cpp
    src/physics/PhysicsComponent.cpp
    src/physics/PhysicsSystemEnhanced.cpp
    src/interaction/RaycastSystem.cpp
    src/interaction/VoxelInteraction.cpp
    src/interaction/VoxelTool.cpp
    src/interaction/ToolManager.cpp
    src/ai/AISystem.cpp
    src/ai/BehaviorTree.cpp
    src/audio/AudioEngine.cpp
    src/scripting/EventSystem.cpp
    src/character/VoxelCharacter.cpp
    src/character/BodyPartLibrary.cpp
    src/character/AnimationClip.cpp
    src/character/AnimationController.cpp
    src/character/IKSolver.cpp
    src/character/CharacterRenderer.cpp
    src/editor/EditorGUI.cpp
    src/editor/WorldEditor.cpp
    src/editor/TerraformingSystem.cpp
    src/editor/BlockBuilder.cpp
    src/editor/SelectionManager.cpp
    src/editor/SelectionRenderer.cpp
    src/editor/TransformGizmo.cpp
    src/editor/FileDialogManager.cpp
    src/editor/LayoutManager.cpp
    src/editor/EditorSettingsDialog.cpp
    src/editor/CameraController.cpp
    src/serialization/WorldSerializer.cpp
    src/ui/MainMenu.cpp
    src/ui/EditorToolbar.cpp
    src/input/InputManager.cpp
    src/gameplay/Camera.cpp
    src/gameplay/Player.cpp
    src/gameplay/TimeManager.cpp
    src/gameplay/SeasonManager.cpp
    src/gameplay/WeatherManager.cpp
    src/gameplay/Raft.cpp
    src/assets/ModularAssetSystem.cpp
    src/assets/VoxelMaterialPack.cpp
    src/assets/AssetManager.cpp
    src/assets/AssetManagerExports.cpp
    src/ecs/EntityManager.cpp
    src/ecs/TransformComponent.cpp
    src/ecs/RendererComponent.cpp
    src/ecs/MaterialComponent.cpp
    src/ecs/Transform2DComponent.cpp
    src/ecs/Sprite2DComponent.cpp
    src/ecs/RigidBody2DComponent.cpp
    src/ecs/Collider2DComponent.cpp
    src/ecs/Animation2DComponent.cpp
    src/ecs/Camera2DComponent.cpp
    src/ecs/Physics2DSystem.cpp
    src/ecs/Collision2DSystem.cpp
    src/ecs/Animation2DSystem.cpp
    src/ecs/Camera2DSystem.cpp
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
    src/scripting/lua/LuaScriptingEngine.cpp
    src/scripting/lua/LuaECSBindings.cpp
    src/scripting/lua/LuaTimeBindings.cpp
)

# Add platform-specific rendering backends
if(ENABLE_DIRECTX)
    list(APPEND ENGINE_SOURCES
        src/renderer/backends/DirectX11RenderContext.cpp
        src/renderer/backends/DirectX12RenderContext.cpp
    )
endif()

if(ENABLE_OPENGL)
    list(APPEND ENGINE_SOURCES
        src/renderer/backends/OpenGLRenderContext.cpp
    )
endif()

# Add Windows-native UI (independent of ImGui)
if(WIN32)
    list(APPEND ENGINE_SOURCES
        # Core Windows components
        src/core/Win32Window.cpp
        src/input/Win32InputManager.cpp
        
        # Native Win32 UI panels and controls
        src/ui/native/Win32Panel.cpp
        src/ui/native/Win32MenuBar.cpp
        src/ui/native/Win32Toolbar.cpp
        src/ui/native/Win32HUD.cpp
        src/ui/native/Win32HUDOverlay.cpp
        src/ui/native/Win32TreeView.cpp
        src/ui/native/Win32ListView.cpp
        src/ui/native/Win32ConsolePanel.cpp
        src/ui/native/Win32InspectorPanel.cpp
        src/ui/native/Win32SceneHierarchyPanel.cpp
        src/ui/native/Win32ContentBrowserPanel.cpp
        src/ui/native/Win32SettingsDialog.cpp
        src/ui/native/Win32TerraformingPanel.cpp
        src/ui/native/Win32ViewportPanel.cpp
        src/ui/native/Win32StatusBar.cpp
        src/ui/NativeMainMenu.cpp
        
        # Windows integration features (theme detection, dialogs, taskbar, etc.)
        src/ui/WindowsThemeManager.cpp
        src/ui/WindowsDialogManager.cpp
        src/ui/WindowsTaskbarManager.cpp
        src/ui/WindowsJumpListManager.cpp
        src/ui/WindowsToastManager.cpp
        src/ui/WindowsDPIManager.cpp
        
        # Note: ImGui-based panels (WindowsCustomizationPanel, MainMenuPanel, SettingsPanel, EngineConfigPanel) 
        # have been archived to archived/imgui-panels/ and are no longer compiled.
        # All UI is now handled through native Win32 panels.
    )
    
    # Define that we're using native Win32 UI
    add_definitions(-DFRESH_WIN32_UI)
    message(STATUS "Win32 native UI enabled")
endif()

# Always include EditorManager (uses native Win32 UI on Windows)
list(APPEND ENGINE_SOURCES
    src/editor/EditorManager.cpp
    src/ui/HotbarPanel.cpp
)
message(STATUS "Windows Native UI enabled")

# Note: ImGui has been completely removed from the project.
# All deprecated ImGui-based panels have been archived to archived/imgui-panels/
# The engine uses Windows Native Win32 UI exclusively. See WINDOWS_NATIVE_ONLY_POLICY.md

# Header files
set(ENGINE_HEADERS
    include/core/Engine.h
    include/core/Window.h
    include/core/Logger.h
    include/core/MemoryManager.h
    include/core/ResourceManager.h
    include/core/SceneManager.h
    include/core/Reflection.h
    include/renderer/ShaderManager.h
    include/voxel/Chunk.h
    include/voxel/VoxelWorld.h
    include/voxel/VoxelTypes.h
    include/voxel/MeshGenerator.h
    include/voxel/ChunkStreamer.h
    include/generation/NoiseGenerator.h
    include/generation/TerrainGenerator.h
    include/generation/IWorldGenerator.h
    include/generation/WorldGenerationTask.h
    include/physics/PhysicsSystem.h
    include/physics/CollisionDetection.h
    include/physics/PhysicsComponent.h
    include/physics/PhysicsSystemEnhanced.h
    include/interaction/RaycastSystem.h
    include/interaction/VoxelInteraction.h
    include/ai/AISystem.h
    include/ai/BehaviorTree.h
    include/audio/AudioEngine.h
    include/scripting/EventSystem.h
    include/editor/EditorGUI.h
    include/editor/WorldEditor.h
    include/editor/TerraformingSystem.h
    include/editor/EditorManager.h
    include/editor/SelectionManager.h
    include/editor/SelectionRenderer.h
    include/serialization/WorldSerializer.h
    include/ui/MainMenu.h
    include/ui/HotbarPanel.h
    include/ui/NativeMainMenu.h
    include/input/InputManager.h
    include/gameplay/Camera.h
    include/gameplay/Player.h
    include/gameplay/TimeManager.h
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
    include/scripting/lua/LuaScriptingEngine.h
    include/scripting/lua/LuaECSBindings.h
    include/scripting/lua/LuaTimeBindings.h
    include/character/VoxelCharacter.h
)

message(STATUS "Source and header files defined. Creating executable target...")

# Create executable
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

message(STATUS "Executable target '${PROJECT_NAME}' created successfully")

# Make it a proper Windows GUI application (no console in Release)
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ==============================================================================
# Link Libraries
# ==============================================================================

# Link GLFW (windowing)
if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
elseif(GLFW_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES})
endif()

# Link GLM (mathematics)
if(glm_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

# Link OpenAL (Audio)
if(OpenAL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
    add_definitions(-DFRESH_OPENAL_AVAILABLE)
    message(STATUS "OpenAL enabled for audio support")
endif()

# Link libvorbis (OGG audio support)
if(Vorbis_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Vorbis::vorbisfile Vorbis::vorbis Ogg::ogg)
    add_definitions(-DFRESH_VORBIS_AVAILABLE)
    message(STATUS "libvorbis enabled for OGG audio support")
endif()

# Link Lua (Scripting)
if(Lua_FOUND)
    add_definitions(-DFRESH_LUA_AVAILABLE)
    
    # CRITICAL FIX: Add Lua include directories FIRST, before linking Lua target
    # This ensures that Sol2 (which is header-only and depends on Lua headers)
    # can find lua.h when its headers are processed during compilation.
    # Sol2's cmake config may not properly propagate Lua's include directories,
    # so we explicitly add them here as early as possible.
    if(DEFINED LUA_INCLUDE_DIR)
        target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
        message(STATUS "Lua include directories added: ${LUA_INCLUDE_DIR}")
    else()
        message(WARNING "LUA_INCLUDE_DIR not defined - this may cause compilation errors with Sol2")
    endif()
    
    if(LUA_FOUND_VIA_CONFIG)
        # Using vcpkg's Lua config - check for different possible target names
        # vcpkg on Windows creates the 'lua' or 'luajit' target
        # Note: Must ensure C linkage is preserved when linking
        if(TARGET luajit)
            target_link_libraries(${PROJECT_NAME} PRIVATE luajit)
            message(STATUS "LuaJIT enabled via CONFIG mode - linked target 'luajit'")
        elseif(TARGET luajit::luajit)
            target_link_libraries(${PROJECT_NAME} PRIVATE luajit::luajit)
            message(STATUS "LuaJIT enabled via CONFIG mode - linked target 'luajit::luajit'")
        elseif(TARGET lua)
            target_link_libraries(${PROJECT_NAME} PRIVATE lua)
            message(STATUS "Lua enabled via CONFIG mode - linked target 'lua'")
        elseif(TARGET lua::lua)
            target_link_libraries(${PROJECT_NAME} PRIVATE lua::lua)
            message(STATUS "Lua enabled via CONFIG mode - linked target 'lua::lua'")
        elseif(TARGET lua::library)
            # Some vcpkg versions use lua::library target
            target_link_libraries(${PROJECT_NAME} PRIVATE lua::library)
            message(STATUS "Lua enabled via CONFIG mode - linked target 'lua::library'")
        else()
            # Fallback: use variables if targets don't exist
            if(DEFINED LUA_LIBRARIES)
                target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
                message(STATUS "Lua enabled via CONFIG mode - using variables")
                message(STATUS "  LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")
                message(STATUS "  LUA_LIBRARIES: ${LUA_LIBRARIES}")
            else()
                message(WARNING "Lua found via CONFIG but no targets or variables available")
                message(WARNING "This may cause linking errors!")
            endif()
        endif()
    else()
        # Using standard FindLua module variables
        if(DEFINED LUA_LIBRARIES)
            target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
        endif()
        message(STATUS "Lua enabled via FindLua module")
        message(STATUS "  LUA_INCLUDE_DIR: ${LUA_INCLUDE_DIR}")
        message(STATUS "  LUA_LIBRARIES: ${LUA_LIBRARIES}")
        if(DEFINED LUA_VERSION_STRING)
            message(STATUS "  LUA_VERSION_STRING: ${LUA_VERSION_STRING}")
        endif()
    endif()
    
    # Add Sol2 for modern C++ Lua binding
    if(SOL2_AVAILABLE)
        # Sol2 is header-only, so we just need to link it
        # Note: LUA_INCLUDE_DIR was already added above, ensuring Sol2 can find lua.h
        target_link_libraries(${PROJECT_NAME} PRIVATE sol2::sol2)
        add_definitions(-DFRESH_SOL2_AVAILABLE)
        message(STATUS "Sol2 enabled - using production Lua integration")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/sol2/include")
        target_include_directories(${PROJECT_NAME} PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/external/sol2/include)
        add_definitions(-DFRESH_SOL2_AVAILABLE)
        message(STATUS "Sol2 found (local) - using production Lua integration")
    else()
        message(STATUS "Sol2 not found - Lua scripting features will be limited")
        message(STATUS "  To enable full Lua support: vcpkg install sol2")
    endif()
    
    message(STATUS "Lua enabled for scripting and modding")
else()
    # No Lua available, use stub implementation
    set(LUA_IMPLEMENTATION_FILE src/scripting/lua/ScriptingEngine.cpp)
endif()

# Link nlohmann-json (JSON parsing for asset manifests)
if(nlohmann_json_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
    add_definitions(-DFRESH_JSON_AVAILABLE)
    message(STATUS "nlohmann-json enabled for asset manifest parsing")
endif()

# Link Native File Dialog Extended (file dialogs)
if(nfd_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE nfd::nfd)
    add_definitions(-DFRESH_NFD_AVAILABLE)
    message(STATUS "Native File Dialog Extended enabled for file operations")
endif()

# Link tinyobjloader (OBJ model loading)
if(TINYOBJLOADER_AVAILABLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader::tinyobjloader)
    add_definitions(-DFRESH_TINYOBJLOADER_AVAILABLE)
    message(STATUS "tinyobjloader enabled for OBJ model loading")
endif()

# Link DirectX (Windows only)
if(WIN32 AND ENABLE_DIRECTX)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        d3d11.lib 
        d3d12.lib 
        dxgi.lib 
        d3dcompiler.lib
    )
    message(STATUS "DirectX libraries linked")
endif()

# Link Win32 UI libraries (Windows only)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        comctl32.lib    # Common controls (toolbars, status bars, etc.)
        user32.lib      # User interface functions
        gdi32.lib       # Graphics Device Interface
        gdiplus.lib     # GDI+ for advanced graphics
        shell32.lib     # Shell integration (file dialogs, etc.)
        ole32.lib       # OLE/COM support
        dwmapi.lib      # Desktop Window Manager (Aero effects, etc.)
    )
    message(STATUS "Win32 UI libraries linked")
endif()

# Link OpenGL and GLEW (cross-platform)
if(ENABLE_OPENGL AND OpenGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
    message(STATUS "OpenGL library linked")
    
    # Link GLEW on Windows and Linux
    if(NOT APPLE AND GLEW_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
        message(STATUS "GLEW library linked")
    endif()
endif()

# Windows-only: MSVC is the primary compiler
if(NOT MSVC)
    message(WARNING "Non-MSVC compiler detected. Fresh Voxel Engine is optimized for MSVC/Visual Studio 2022.")
    message(WARNING "Other compilers may work but are not officially supported.")
endif()

if(MSVC)
    # Windows/MSVC specific options
    # /W4: Warning level 4 (high)
    # /WX: Treat warnings as errors to maintain code quality
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    
    # Release mode optimizations for MSVC
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE 
            /O2          # Maximum optimization (speed)
            /Oi          # Generate intrinsic functions
            /Ot          # Favor fast code
            /GL          # Whole program optimization
            /Gy          # Enable function-level linking
            /GS-         # Disable buffer security checks for performance
            /fp:fast     # Fast floating-point model
            /arch:AVX2   # Enable AVX2 instructions for modern CPUs (2013+)
        )
        target_link_options(${PROJECT_NAME} PRIVATE 
            /LTCG        # Link-time code generation
            /OPT:REF     # Eliminate unreferenced functions/data
            /OPT:ICF     # Enable COMDAT folding
        )
        
        # Windows subsystem optimization for GUI applications
        target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
        
        message(STATUS "Windows Release optimizations enabled:")
        message(STATUS "  - AVX2 SIMD instructions")
        message(STATUS "  - Whole program optimization")
        message(STATUS "  - Fast floating-point model")
        message(STATUS "  - Function-level linking")
    endif()
    
    # Debug mode with optimized debugging experience
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE 
            /ZI          # Edit and Continue support
            /Od          # Disable optimizations
            /RTC1        # Runtime checks
        )
    endif()
endif()

# Copy shaders to build directory
# Handle both single-config (Makefile, Ninja) and multi-config (Visual Studio, Xcode) generators
file(GLOB SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert" 
                       "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag")

# For multi-config generators, copy shaders to each configuration directory
# For single-config generators, copy to the binary directory
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    
    # Add a custom command to copy shaders after building the target
    # This uses generator expressions to handle multi-config generators correctly
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SHADER}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/${SHADER_NAME}"
        COMMENT "Copying shader ${SHADER_NAME} to build directory"
        VERBATIM
    )
endforeach()

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY shaders/ DESTINATION bin/shaders)

# ==============================================================================
# Testing Support
# ==============================================================================

# Option to enable/disable tests
option(BUILD_TESTS "Build unit tests" ON)

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
        tests/test_main.cpp                 # Custom main with better error handling
        tests/voxel/ChunkTests.cpp
        tests/voxel/VoxelWorldTests.cpp
        tests/gameplay/TimeManagerTests.cpp       # Tests for day/night cycle system
        tests/integration/WorldCreationIntegrationTest.cpp
        tests/generation/NoiseTests.cpp
        tests/generation/TerrainGeneratorTests.cpp
        tests/input/InputManagerTests.cpp
        tests/core/EngineInitializationTests.cpp  # Tests for engine initialization sequence
        tests/core/ReflectionTests.cpp            # Tests for reflection system
        tests/core/ResourceManagerTests.cpp       # Tests for resource loading system
        # tests/core/MemoryManagerTests.cpp  # Disabled - tests don't match implementation
        tests/scripting/EventSystemTests.cpp
        tests/rpg/InventoryTests.cpp
        tests/rpg/CraftingSystemTests.cpp
        tests/character/VoxelCharacterTests.cpp
        tests/character/BodyPartLibraryTests.cpp
        tests/character/AnimationClipTests.cpp
        tests/editor/SelectionManagerTests.cpp    # Tests for voxel selection system
        tests/editor/SelectionRendererTests.cpp   # Tests for selection visualization
        tests/ecs/Transform2DTests.cpp            # Tests for 2D transform component
        tests/ecs/Collider2DTests.cpp             # Tests for 2D collider component
        tests/ecs/RigidBody2DTests.cpp            # Tests for 2D rigidbody component
        tests/ecs/HealthComponentTests.cpp        # Tests for health component
        tests/ecs/MovementComponentTests.cpp      # Tests for movement component
        tests/ecs/CombatSystemTests.cpp           # Tests for combat system
        tests/voxelship/VoxelBlockStretchTest.cpp # Tests for voxel block stretching
    )
    
    # Engine source files needed for tests (exclude main.cpp)
    set(TEST_ENGINE_SOURCES
        src/voxel/Chunk.cpp
        src/voxel/VoxelWorld.cpp
        src/voxel/MeshGenerator.cpp
        src/generation/NoiseGenerator.cpp
        src/generation/TerrainGenerator.cpp
        src/generation/World2DGenerator.cpp
        src/core/MemoryManager.cpp
        src/core/Logger.cpp
        src/core/Reflection.cpp
        src/core/ResourceManager.cpp
        src/renderer/stb_image_impl.cpp
        src/scripting/EventSystem.cpp
        src/input/InputManager.cpp
        src/gameplay/TimeManager.cpp
        src/gameplay/Camera.cpp
        src/rpg/Inventory.cpp
        src/rpg/CraftingSystem.cpp
        src/assets/ModularAssetSystem.cpp
        src/character/VoxelCharacter.cpp
        src/character/BodyPartLibrary.cpp
        src/character/AnimationClip.cpp
        src/character/AnimationController.cpp
        src/editor/SelectionManager.cpp
        src/editor/SelectionRenderer.cpp
        src/editor/TransformGizmo.cpp
        src/editor/TerraformingSystem.cpp
        src/editor/FileDialogManager.cpp
        src/devtools/DebugRenderer.cpp
        src/ecs/Transform2DComponent.cpp
        src/ecs/Sprite2DComponent.cpp
        src/ecs/RigidBody2DComponent.cpp
        src/ecs/Collider2DComponent.cpp
        src/ecs/Animation2DComponent.cpp
        src/ecs/Camera2DComponent.cpp
        src/voxelship/VoxelBlock.cpp
    )
    
    # Create test executable
    add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES} ${TEST_ENGINE_SOURCES})
    
    # Include directories for tests
    target_include_directories(${PROJECT_NAME}Tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    # Link test executable with Google Test and engine libraries
    # Note: Using GTest::gtest without gtest_main since we provide our own main
    target_link_libraries(${PROJECT_NAME}Tests PRIVATE
        GTest::gtest
    )
    
    # Link GLFW if needed for tests
    if(glfw3_FOUND)
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE glfw)
    elseif(GLFW_FOUND)
        target_include_directories(${PROJECT_NAME}Tests PRIVATE ${GLFW_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE ${GLFW_LIBRARIES})
    else()
        # Fallback: try to link glfw directly on Linux
        if(UNIX AND NOT APPLE)
            target_link_libraries(${PROJECT_NAME}Tests PRIVATE glfw)
        endif()
    endif()
    
    # Link GLM if found
    if(glm_FOUND)
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE glm::glm)
    endif()
    
    # Link Native File Dialog Extended if found (required for FileDialogManager)
    if(nfd_FOUND)
        target_link_libraries(${PROJECT_NAME}Tests PRIVATE nfd::nfd)
        message(STATUS "Native File Dialog Extended linked to test executable")
    endif()
    
    # Compiler settings for tests (Windows/MSVC)
    target_compile_features(${PROJECT_NAME}Tests PRIVATE cxx_std_20)
    
    if(MSVC)
        # /W4: Warning level 4, /WX: Treat warnings as errors
        target_compile_options(${PROJECT_NAME}Tests PRIVATE /W4 /WX)
    else()
        message(WARNING "Non-MSVC compiler for tests - may not be fully supported")
    endif()
    
    # Register tests with CTest
    include(GoogleTest)
    # Use PRE_TEST discovery mode to avoid running test executable at build time
    # This prevents failures on Windows when vcpkg DLLs aren't in PATH during build
    gtest_discover_tests(${PROJECT_NAME}Tests
        DISCOVERY_MODE PRE_TEST
    )
    
    message(STATUS "Tests configured successfully for Windows")
    message(STATUS "Run tests with: ctest or FreshVoxelEngineTests.exe")
endif()

# ==============================================================================
# Example Programs
# ==============================================================================

# NOTE: All gameplay examples and demos are now integrated directly into the
# main FreshVoxelEngine executable. Run FreshVoxelEngine.exe to access all demos.
# See examples/README.md for a complete list of available in-engine demos.

message(STATUS "Examples are integrated into main engine executable")
message(STATUS "Run FreshVoxelEngine.exe and select from the main menu")
message(STATUS "See examples/README.md for demo documentation")

message(STATUS "")
message(STATUS "========================================================================")
message(STATUS "CMake configuration completed successfully!")
message(STATUS "========================================================================")
message(STATUS "")
