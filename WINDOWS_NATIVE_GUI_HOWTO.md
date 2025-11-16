# How to Enable Windows Native GUI

## Quick Start

The Fresh Voxel Engine now supports fully native Windows GUI using Win32 APIs instead of GLFW. This provides better Windows integration and a more native experience.

## Enabling Win32 Native Mode

### Option 1: CMake Configuration (Recommended)

Add this to your CMake configuration:

```cmake
cmake -DUSE_WIN32_NATIVE=ON -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -S . -B build
```

### Option 2: Define in Code

Add this to your main.cpp or Engine.cpp before including window headers:

```cpp
#define USE_WIN32_NATIVE
```

## Usage Example

The Win32 classes are designed to be drop-in replacements for GLFW classes:

```cpp
#ifdef USE_WIN32_NATIVE
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
    using WindowType = fresh::Win32Window;
    using InputManagerType = fresh::Win32InputManager;
#else
    #include "core/Window.h"
    #include "input/InputManager.h"
    using WindowType = fresh::Window;
    using InputManagerType = fresh::InputManager;
#endif

// Create window
auto window = std::make_unique<WindowType>(1920, 1080, "Fresh Voxel Engine");
window->initialize(false); // false = use DirectX, true = use OpenGL

// Create input manager
auto inputManager = std::make_unique<InputManagerType>();

#ifdef USE_WIN32_NATIVE
    // Win32: Set up callbacks
    inputManager->initialize(window.get());
    
    window->setKeyCallback([&inputManager](int vk, bool isDown) {
        inputManager->processKeyEvent(vk, isDown);
    });
    
    window->setMouseMoveCallback([&inputManager](int x, int y) {
        inputManager->processMouseMovement(x, y);
    });
    
    window->setMouseButtonCallback([&inputManager](int button, bool isDown) {
        inputManager->processMouseButton(button, isDown);
    });
#else
    // GLFW: Initialize with window handle
    inputManager->initialize(window->getHandle());
#endif

// Main loop (same for both)
while (!window->shouldClose()) {
    inputManager->resetMouseDelta();
    window->pollEvents();
    inputManager->update();
    
    // Your game logic here
    
    // Rendering...
}
```

## ImGui Integration

ImGui will automatically use the Win32 backend when `USE_WIN32_NATIVE` is defined:

```cpp
#include "ui/ImGuiContext.h"

ImGuiContext imguiContext;

#ifdef USE_WIN32_NATIVE
    imguiContext.initializeWin32(window.get(), renderContext.get());
#else
    imguiContext.initialize(window.get(), renderContext.get());
#endif

// In your main loop
imguiContext.newFrame();
// ... ImGui UI code ...
imguiContext.render();
```

## Benefits of Win32 Native

1. **Fully Native**: Direct Windows API calls, no cross-platform abstraction
2. **Better Performance**: No intermediate library layer
3. **Windows Features**: Access to all Windows-specific features
4. **Smaller Binary**: No GLFW dependency
5. **Better Integration**: Native Windows theming and features

## Compatibility

- **Minimum Windows Version**: Windows 10 (v1809 or later recommended)
- **DirectX**: Works with DirectX 11 and DirectX 12 backends
- **OpenGL**: Supported but DirectX is recommended for Windows

## Current Status

✅ **Implemented**:
- Win32Window class with full windowing support
- Win32InputManager with keyboard and mouse input
- ImGui Win32 backend integration
- Input callbacks system
- Window message handling

⏳ **To Be Integrated**:
- Full integration into Engine class (currently uses GLFW)
- Testing and validation on different Windows versions
- Performance benchmarking vs GLFW

## Testing

To test the Win32 native GUI:

1. Build with Win32 support enabled
2. Run the engine
3. Verify:
   - Window creates and displays correctly
   - Mouse and keyboard input works
   - ImGui UI renders properly
   - DirectX rendering works
   - Window resize handles correctly

## Troubleshooting

### Build Errors

If you get errors about missing ImGui Win32 backend:
```bash
# Update vcpkg packages
vcpkg install imgui[win32-binding,dx11-binding,dx12-binding]:x64-windows
```

### Input Not Working

Make sure callbacks are set up:
```cpp
window->setKeyCallback(...);
window->setMouseMoveCallback(...);
window->setMouseButtonCallback(...);
```

### ImGui Not Rendering

Ensure ImGui Win32 backend is initialized:
```cpp
imguiContext.initializeWin32(window.get(), renderContext.get());
```

## See Also

- [WINDOWS_NATIVE_GUI_MIGRATION.md](WINDOWS_NATIVE_GUI_MIGRATION.md) - Detailed migration guide
- [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md) - Other Windows-native features
- [BUILD.md](BUILD.md) - Building the engine
