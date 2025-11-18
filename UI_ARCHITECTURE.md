# Fresh Engine UI Architecture

## Overview

The Fresh Engine uses **native Windows GUI** as its primary UI system, with optional ImGui support for cross-platform compatibility.

## Primary UI System: Native Win32

### Windows-Native Components

The engine includes a complete suite of native Win32 UI components:

**Location**: `include/ui/native/` and `src/ui/native/`

**Components**:
- `Win32MenuBar` - Native Windows menu bar (HMENU)
- `Win32Toolbar` - Native toolbar with buttons
- `Win32Panel` - Base panel class for UI containers
- `Win32HUD` - Head-up display overlay
- `Win32TreeView` - Hierarchical tree control
- `Win32ListView` - List/grid view control
- `Win32ConsolePanel` - Console output window

**Theme System**:
- `UnrealStyleTheme.h` - Unreal Engine-inspired color palette for Win32
- `WindowsThemeManager` - Manages native Windows theming
- `WindowsDialogManager` - Native file dialogs and message boxes
- `WindowsTaskbarManager` - Windows taskbar integration
- `WindowsCustomizationPanel` - Custom theme editor

### Native Win32 Features

```cpp
// Example: Creating a native menu bar
#include "ui/native/Win32MenuBar.h"
#include "ui/native/UnrealStyleTheme.h"

Win32MenuBar menuBar;
menuBar.create(hwnd);

int fileMenu = menuBar.addMenu("File");
menuBar.addMenuItem(fileMenu, "New", []() { /* ... */ });
menuBar.addMenuItem(fileMenu, "Open", []() { /* ... */ });

// Apply Unreal-style colors
HBRUSH bgBrush = CreateSolidBrush(UnrealStyleTheme::WindowBackground);
```

### Build Configuration

Native Win32 UI is enabled by default on Windows:

```cmake
# From CMakeLists.txt
if(WIN32)
    list(APPEND ENGINE_SOURCES
        src/ui/native/Win32Panel.cpp
        src/ui/native/Win32MenuBar.cpp
        src/ui/native/Win32Toolbar.cpp
        # ... etc
    )
    add_definitions(-DFRESH_WIN32_UI)
endif()
```

**Compilation Flags**:
- `FRESH_WIN32_UI` - Enables native Win32 UI (default on Windows)
- Uses native Windows APIs: `windows.h`, `commctrl.h`, `dwmapi.h`

## Secondary UI System: ImGui (Optional)

### ImGui Integration

ImGui is provided as an **optional** cross-platform UI layer:

**Location**: `include/ui/ImGuiContext.h` and `src/ui/ImGuiContext.cpp`

**Purpose**:
- Cross-platform compatibility (Linux, macOS)
- Rapid prototyping of UI elements
- Developer debugging tools
- Optional overlay UI

**Build Configuration**:

```cmake
# ImGui is optional
find_package(ImGui)
if(ImGui_FOUND)
    add_definitions(-DFRESH_IMGUI_AVAILABLE)
    list(APPEND ENGINE_SOURCES src/ui/ImGuiContext.cpp)
else()
    message(STATUS "ImGui not found - editor UI will be limited")
endif()
```

**Compilation Flags**:
- `FRESH_IMGUI_AVAILABLE` - Only defined if ImGui is found
- All ImGui code is wrapped in `#ifdef FRESH_IMGUI_AVAILABLE`

### Conditional ImGui Usage

The codebase uses conditional compilation for ImGui:

```cpp
#ifdef FRESH_IMGUI_AVAILABLE
    #include <imgui.h>
    
    // ImGui-specific code
    ImGui::Begin("Debug Window");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::End();
#endif
```

**Without ImGui**: Editor still works with native Win32 UI
**With ImGui**: Additional overlay and debug tools available

## UI System Comparison

### Native Win32 vs ImGui

| Feature | Native Win32 | ImGui |
|---------|-------------|-------|
| **Platform** | Windows only | Cross-platform |
| **Performance** | Native, fastest | Good, GPU-accelerated |
| **Look & Feel** | Native Windows | Custom themed |
| **Integration** | OS integration | Standalone |
| **File Dialogs** | Native dialogs | Custom dialogs |
| **Menus** | Native HMENU | Custom rendering |
| **Controls** | Native controls | Custom widgets |
| **Theme** | Windows themes | Custom styling |
| **Accessibility** | Windows built-in | Limited |
| **Status** | **Primary** | **Optional** |

## Editor UI Architecture

### Current Implementation

```
Fresh Engine Editor (Windows)
│
├─ Native Win32 UI (Primary)
│   ├─ Win32Window (HWND)
│   ├─ Win32MenuBar (HMENU) ◄─── Tools → Dialogue Editor menu
│   ├─ Win32Toolbar (Buttons)
│   ├─ Win32HUD (Overlay)
│   ├─ WindowsThemeManager ◄───── UnrealStyleTheme
│   └─ Native panels and controls
│
└─ ImGui Overlay (Optional)
    ├─ Debug windows
    ├─ Performance stats
    └─ Developer tools
```

### Why Both Systems?

1. **Native Win32** (Primary):
   - Best Windows integration
   - Native look and feel
   - Better performance
   - OS-level features (taskbar, themes, etc.)
   - Unreal-style theming via UnrealStyleTheme.h

2. **ImGui** (Optional):
   - Cross-platform editor support
   - Quick debug UI
   - Developer tools
   - Can be disabled without affecting core functionality

## EditorManager UI Usage

The `EditorManager` uses both systems conditionally:

```cpp
// From EditorManager.cpp

#ifdef FRESH_IMGUI_AVAILABLE
    // Initialize ImGui for overlay UI
    m_imguiContext = std::make_unique<ImGuiContext>();
    m_imguiContext->initializeWin32(window, renderContext);
#endif

#ifdef FRESH_WIN32_UI
    // Native Win32 UI is always available on Windows
    // Win32MenuBar and Win32Toolbar are used
#endif
```

### Menu System

The editor has BOTH menu systems:

1. **Win32MenuBar** (Native):
   - `Win32MenuBar` class
   - Native HMENU with callbacks
   - OS-integrated appearance
   - Keyboard shortcuts (Alt+F, etc.)

2. **EditorMenuBar** (ImGui):
   - `EditorMenuBar` class (when ImGui available)
   - ImGui::BeginMainMenuBar()
   - Custom rendering
   - Used for overlay/debug menus

## Dialogue Editor Integration

The dialogue editor menu item was added to:

**Native Win32 Path** (if available):
```cpp
// Would use Win32MenuBar
Win32MenuBar* nativeMenu = getWin32MenuBar();
int toolsMenu = nativeMenu->addMenu("Tools");
nativeMenu->addMenuItem(toolsMenu, "Dialogue Editor\tCtrl+D", 
    []() { launchDialogueEditor(); });
```

**ImGui Path** (current implementation):
```cpp
// Uses EditorMenuBar with ImGui
void EditorMenuBar::renderToolsMenu()
{
    if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Dialogue Editor", "Ctrl+D")) {
            if (m_dialogueEditorCallback) {
                m_dialogueEditorCallback();
            }
        }
        ImGui::EndMenu();
    }
}
```

## Recommendations

### For Windows-First Development

If you want to focus on Windows with native UI:

1. **Disable ImGui** (optional):
   ```cmake
   # In CMakeLists.txt or via CMake option
   option(ENABLE_IMGUI "Enable ImGui UI" OFF)
   ```

2. **Use Native Win32 Components**:
   - Menu bar: `Win32MenuBar`
   - Toolbar: `Win32Toolbar`
   - Panels: `Win32Panel`
   - Theme: `UnrealStyleTheme`

3. **Benefits**:
   - Pure Windows application
   - Native performance
   - Better OS integration
   - Smaller binary (no ImGui lib)

### For Cross-Platform Development

If you want Linux/macOS support:

1. **Keep ImGui** (recommended):
   - Provides portable UI
   - Works on all platforms
   - Easier development

2. **Use Both Systems**:
   - Win32 UI on Windows
   - ImGui on Linux/macOS
   - Conditional compilation

## Migration Path

### To Pure Native Win32

If you want to remove ImGui completely:

1. Replace all `#ifdef FRESH_IMGUI_AVAILABLE` sections
2. Implement Win32 equivalents for ImGui UI
3. Update `EditorMenuBar` to use `Win32MenuBar`
4. Remove ImGui from CMakeLists.txt
5. Remove ImGui dependency from vcpkg/packages

### To ImGui-Only

If you want to use only ImGui:

1. Keep `#ifdef FRESH_IMGUI_AVAILABLE` sections
2. Remove Win32-specific UI code
3. Use ImGui for all UI elements
4. Portable to Linux/macOS

## Current Status

**As Implemented**:
- ✅ Native Win32 UI components exist
- ✅ UnrealStyleTheme for Win32 implemented
- ✅ ImGui integration (optional)
- ✅ Both systems work together
- ✅ Dialogue editor menu in ImGui path

**Recommended**:
- Use native Win32 UI as primary on Windows
- Keep ImGui optional for cross-platform
- UnrealStyleTheme applied to Win32 components
- Dialogue editor accessible from either UI system

## Technical Details

### Win32 Native Controls

```cpp
// HWND-based window
Win32Window* window = new Win32Window();
window->create("Fresh Engine Editor", 1920, 1080);

// HMENU-based menu bar
Win32MenuBar* menuBar = new Win32MenuBar();
menuBar->create(window->getHandle());

// Apply Unreal theme
WindowsThemeManager* themeManager = new WindowsThemeManager();
themeManager->initialize();
themeManager->applyToWindows();  // Apply to native controls
```

### ImGui Overlay

```cpp
#ifdef FRESH_IMGUI_AVAILABLE
// ImGui context
ImGuiContext* imgui = new ImGuiContext();
imgui->initializeWin32(window, renderContext);

// Each frame
imgui->beginFrame();
ImGui::ShowDemoWindow();  // Debug UI
imgui->endFrame();
#endif
```

## Conclusion

The Fresh Engine uses **native Windows GUI** as its primary UI system:
- Native Win32 components for menus, toolbars, panels
- UnrealStyleTheme for professional dark appearance
- Optional ImGui for debug/dev tools and cross-platform support
- Both systems can coexist or Win32 can be used exclusively

The choice of UI system is configurable via CMake options.
