# Fresh Voxel Engine - GUI System Guide

**Comprehensive guide to the Fresh Editor GUI implementation**

## Table of Contents

1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Windows Native GUI](#windows-native-gui)
4. [ImGui Integration](#imgui-integration)
5. [Editor Panels](#editor-panels)
6. [Dark Theme System](#dark-theme-system)
7. [Transform Gizmo](#transform-gizmo)
8. [Toolbar and Menu Bar](#toolbar-and-menu-bar)
9. [Quick Start](#quick-start)
10. [Current Status](#current-status)
11. [Keyboard Shortcuts](#keyboard-shortcuts)

---

## Overview

The Fresh Voxel Engine features a professional Unreal Engine-like editor with a hybrid GUI system:
- **Windows Native UI**: Native Win32 window management, menu bars, and toolbars
- **ImGui Editor Panels**: Modern, immediate-mode GUI for editor tools
- **DirectX Rendering**: Full integration with DirectX 11/12 graphics pipeline
- **Dark Theme**: Professional dark theme with customizable colors

### Key Features

- ✅ Native Windows window management (replaces GLFW on Windows)
- ✅ Native menu bar with File, Edit, View, Tools, Help menus
- ✅ Native toolbar with icon buttons
- ✅ ImGui panels: Scene Hierarchy, Inspector, Content Browser, Console
- ✅ Transform Gizmo for object manipulation (Move/Rotate/Scale)
- ✅ Selection system for voxel and entity selection
- ✅ Dark theme with customizable colors
- ✅ High DPI support
- ✅ Keyboard shortcuts (W/E/R for gizmo modes)

---

## Architecture

### Dual GUI System

The Fresh Editor uses a **hybrid approach** combining the best of both worlds:

```
┌─────────────────────────────────────────┐
│     Native Windows Window (Win32)       │
│  ┌────────────┐  ┌──────────────────┐  │
│  │Menu Bar    │  │ Toolbar          │  │
│  │(Native)    │  │ (Native)         │  │
│  └────────────┘  └──────────────────┘  │
│  ┌─────────────────────────────────┐   │
│  │  Main Viewport (DirectX)        │   │
│  │  ┌─────────┐  ┌──────────────┐  │   │
│  │  │Hierarchy│  │ Inspector    │  │   │
│  │  │(ImGui)  │  │ (ImGui)      │  │   │
│  │  └─────────┘  └──────────────┘  │   │
│  │  ┌──────────────────────────┐   │   │
│  │  │ Content Browser (ImGui)  │   │   │
│  │  └──────────────────────────┘   │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

### Why Hybrid?

1. **Native Windows Controls**: Better OS integration, familiar UX, native performance
2. **ImGui Panels**: Flexible, easy to customize, perfect for editor tools
3. **DirectX Integration**: Both systems render seamlessly with DirectX 11/12

---

## Windows Native GUI

### Win32Window Class

The `Win32Window` class replaces GLFW on Windows for better native integration:

**Features:**
- Native window creation and management
- DirectX 11/12 swap chain integration
- High DPI awareness (per-monitor DPI v2)
- Native message handling
- Window resizing, maximizing, minimizing
- Full-screen support

**Key Methods:**
```cpp
class Win32Window {
    bool initialize(bool useOpenGL = false);
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    HWND getHandle() const;
    void setTitle(const std::string& title);
};
```

### Win32InputManager Class

Native Windows input handling:

**Features:**
- Keyboard input (WM_KEYDOWN, WM_KEYUP)
- Mouse input (WM_MOUSEMOVE, WM_LBUTTONDOWN, etc.)
- Mouse wheel (WM_MOUSEWHEEL)
- Raw input support for high-precision mouse
- Integration with ImGui input system

**Key Methods:**
```cpp
class Win32InputManager {
    bool isKeyPressed(int keyCode) const;
    bool isMouseButtonPressed(int button) const;
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;
};
```

### Enabling Win32 Native Mode

#### CMake Configuration (Recommended)

```bash
cmake -DUSE_WIN32_NATIVE=ON \
      -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
      -S . -B build
```

#### Code Configuration

```cpp
#ifdef USE_WIN32_NATIVE
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
    auto window = std::make_unique<Win32Window>(1920, 1080, "Fresh");
#else
    #include "core/Window.h"
    auto window = std::make_unique<Window>(1920, 1080, "Fresh");
#endif
```

---

## ImGui Integration

### ImGuiManager Class

The `ImGuiManager` class handles ImGui initialization and rendering:

**Features:**
- DirectX 11/12 backend integration
- Win32 backend for input
- Custom dark theme
- Docking and viewports support
- Font loading and management

**Initialization:**
```cpp
class ImGuiManager {
    bool initialize(HWND hwnd, ID3D11Device* device, 
                   ID3D11DeviceContext* context);
    void newFrame();
    void render();
    void shutdown();
};
```

### ImGui Panels

All editor panels use ImGui for flexibility:

1. **Scene Hierarchy** - Tree view of entities
2. **Inspector** - Property editor for selected objects
3. **Content Browser** - Asset browser with thumbnails
4. **Console** - Log viewer with filtering
5. **Voxel Tool Palette** - Block selection and tools

---

## Editor Panels

### Scene Hierarchy Panel

**File**: `include/ui/SceneHierarchyPanel.h`

Displays world entities in a tree structure:

```cpp
class SceneHierarchyPanel {
    void render();
    void setWorld(World* world);
    Entity* getSelectedEntity() const;
    void selectEntity(Entity* entity);
};
```

**Features:**
- Tree view of entities
- Drag-and-drop support
- Right-click context menu
- Search/filter
- Multi-selection

### Inspector Panel

**File**: `include/ui/InspectorPanel.h`

Property editor for selected entities:

```cpp
class InspectorPanel {
    void render();
    void setEntity(Entity* entity);
    void setVoxel(Voxel* voxel);
};
```

**Features:**
- Transform properties (Position, Rotation, Scale)
- Component properties
- Material properties
- Real-time editing
- Undo/redo integration

### Content Browser Panel

**File**: `include/ui/ContentBrowserPanel.h`

Asset browser with preview:

```cpp
class ContentBrowserPanel {
    void render();
    void setAssetPath(const std::string& path);
    Asset* getSelectedAsset() const;
};
```

**Features:**
- Folder navigation
- Asset thumbnails
- Asset import
- Drag-and-drop to scene
- Asset search

### Console Panel

**File**: `include/ui/ConsolePanel.h`

Log viewer with filtering:

```cpp
class ConsolePanel {
    void render();
    void addMessage(const std::string& message, LogLevel level);
    void clear();
};
```

**Features:**
- Color-coded messages (Info, Warning, Error)
- Filtering by level
- Search functionality
- Timestamps
- Copy to clipboard

---

## Dark Theme System

### Theme Colors

The editor uses a professional dark theme inspired by Unreal Engine and Visual Studio:

**Primary Colors:**
```cpp
// Background colors
WindowBg:          rgb(37, 37, 38)    // Main background
ChildBg:           rgb(45, 45, 48)    // Panel background
PopupBg:           rgb(45, 45, 48)    // Popup background

// Text colors
Text:              rgb(241, 241, 241) // Primary text
TextDisabled:      rgb(128, 128, 128) // Disabled text

// Accent colors
Header:            rgb(51, 153, 255)  // Blue accent
HeaderHovered:     rgb(75, 175, 255)  // Lighter blue
HeaderActive:      rgb(25, 128, 230)  // Darker blue

// Interactive colors
Button:            rgb(51, 153, 255)
ButtonHovered:     rgb(75, 175, 255)
ButtonActive:      rgb(25, 128, 230)
```

### Applying the Theme

```cpp
void ImGuiManager::applyDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Apply all theme colors
    colors[ImGuiCol_WindowBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.945f, 0.945f, 0.945f, 1.0f);
    // ... more colors ...
}
```

### Customization

Users can customize the theme in `Config/EditorSettings.ini`:

```ini
[Theme]
WindowBg=37,37,38
AccentColor=51,153,255
TextColor=241,241,241
```

---

## Transform Gizmo

### Overview

The Transform Gizmo allows visual manipulation of objects in the 3D viewport:

**Modes:**
- **Translate** (W key): Move objects along X/Y/Z axes
- **Rotate** (E key): Rotate objects around X/Y/Z axes
- **Scale** (R key): Scale objects uniformly or per-axis

### TransformGizmo Class

**File**: `include/editor/TransformGizmo.h`

```cpp
class TransformGizmo {
    enum class Mode { Translate, Rotate, Scale };
    
    void setMode(Mode mode);
    Mode getMode() const;
    
    void setTarget(Entity* entity);
    void render(const Camera& camera);
    bool handleInput(const InputManager& input, const Camera& camera);
};
```

### Implementation Details

**Rendering:**
- Uses DebugRenderer for visualization
- Color-coded axes: Red (X), Green (Y), Blue (Z)
- Highlights on hover
- Scales with camera distance

**Interaction:**
- Ray casting for selection
- Dragging for manipulation
- Snap to grid (hold Ctrl)
- Local/World space toggle

### Keyboard Shortcuts

```cpp
// In Engine.cpp processInput()
if (m_inputManager->isKeyJustPressed(KEY_W)) {
    gizmo->setMode(TransformGizmo::Mode::Translate);
    m_editorManager->getToolbar()->setActiveTool(EditorToolbar::Tool::Move);
}
if (m_inputManager->isKeyJustPressed(KEY_E)) {
    gizmo->setMode(TransformGizmo::Mode::Rotate);
    m_editorManager->getToolbar()->setActiveTool(EditorToolbar::Tool::Rotate);
}
if (m_inputManager->isKeyJustPressed(KEY_R)) {
    gizmo->setMode(TransformGizmo::Mode::Scale);
    m_editorManager->getToolbar()->setActiveTool(EditorToolbar::Tool::Scale);
}
```

### Toolbar Integration

The toolbar buttons are synchronized with the gizmo:

```cpp
m_toolbar->setToolCallback([this](EditorToolbar::Tool tool) {
    if (!m_transformGizmo) return;
    switch (tool) {
        case EditorToolbar::Tool::Move:
            m_transformGizmo->setMode(TransformGizmo::Mode::Translate);
            break;
        case EditorToolbar::Tool::Rotate:
            m_transformGizmo->setMode(TransformGizmo::Mode::Rotate);
            break;
        case EditorToolbar::Tool::Scale:
            m_transformGizmo->setMode(TransformGizmo::Mode::Scale);
            break;
    }
});
```

---

## Toolbar and Menu Bar

### Native Menu Bar

**File**: `include/ui/NativeMenuBar.h`

Native Windows menu bar with standard menus:

**Structure:**
```
File
├── New World          (Ctrl+N)
├── Open World         (Ctrl+O)
├── Save World         (Ctrl+S)
├── Save World As      (Ctrl+Shift+S)
├── Import Assets      
├── Export Selection   
├── ─────────────────
├── Recent Worlds      >
├── ─────────────────
└── Exit               (Alt+F4)

Edit
├── Undo               (Ctrl+Z)
├── Redo               (Ctrl+Y)
├── ─────────────────
├── Cut                (Ctrl+X)
├── Copy               (Ctrl+C)
├── Paste              (Ctrl+V)
├── Delete             (Del)
├── ─────────────────
├── Select All         (Ctrl+A)
└── Preferences        

View
├── Scene Hierarchy
├── Inspector
├── Content Browser
├── Console
├── ─────────────────
├── Toolbar
├── Status Bar
└── Layout             >

Tools
├── Transform Gizmo    >
│   ├── Move (W)
│   ├── Rotate (E)
│   └── Scale (R)
├── Terraforming       >
├── Voxel Editor       
└── Asset Manager      

Help
├── Documentation
├── Keyboard Shortcuts
├── ─────────────────
├── Report Bug
└── About
```

### Native Toolbar

**File**: `include/ui/NativeToolbar.h`

Native Windows toolbar with icon buttons:

**Buttons:**
- New, Open, Save (File operations)
- Undo, Redo (Edit operations)
- Move, Rotate, Scale (Transform gizmo)
- Play, Pause, Stop (Playmode)
- Build (Project build)

**Implementation:**
```cpp
class NativeToolbar {
    void create(HWND parent);
    void addButton(int id, const std::string& icon, 
                   const std::string& tooltip);
    void setButtonState(int id, bool enabled);
    void setToolCallback(std::function<void(Tool)> callback);
};
```

---

## Quick Start

### 1. Enable Windows Native GUI

```bash
cmake -DUSE_WIN32_NATIVE=ON \
      -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
      -S . -B build
cmake --build build --config Release
```

### 2. Run the Editor

```bash
cd build/Release
./FreshVoxelEngine.exe
```

### 3. First Time Setup

1. The editor opens with default layout
2. File > New World to create a world
3. Use toolbar or keyboard shortcuts to select tools
4. Use Scene Hierarchy to manage entities
5. Use Inspector to edit properties

### 4. Basic Workflow

**Creating Objects:**
1. Right-click in Scene Hierarchy
2. Select "Create Entity" or use voxel tools
3. Select the new entity
4. Edit properties in Inspector

**Transforming Objects:**
1. Select an entity in viewport or hierarchy
2. Press W (Move), E (Rotate), or R (Scale)
3. Drag gizmo handles to transform
4. Hold Ctrl for grid snapping

**Saving:**
1. File > Save World (Ctrl+S)
2. Choose location and name
3. World saves automatically on exit

---

## Current Status

### ✅ Completed Features

1. **Transform Gizmo** ✅
   - Rendering with DebugRenderer
   - Keyboard shortcuts (W/E/R)
   - Toolbar integration
   - Bidirectional sync

2. **File Dialogs** ✅
   - NFD (Native File Dialog Extended) integration
   - Open/Save World dialogs
   - Import Assets dialog (multi-file)
   - Folder picker

3. **Windows Native GUI** ✅
   - Win32 window management
   - Native menu bar
   - Native toolbar
   - DirectX integration
   - High DPI support

4. **ImGui Panels** ✅
   - Scene Hierarchy
   - Inspector
   - Content Browser
   - Console
   - Voxel Tool Palette

5. **Dark Theme** ✅
   - Professional color scheme
   - Customizable colors
   - Persistent settings

6. **Selection System** ✅
   - SelectionManager
   - SelectionRenderer
   - Box selection
   - Multi-selection

### 🚧 In Progress / Testing Needed

1. **Selection System Testing**
   - Box selection verification
   - Copy/Paste with selections
   - Undo/Redo with selections

### 🔮 Future Enhancements

1. **Layout Management** (1 week)
   - Save/Load workspace layouts
   - Predefined layouts (Default, Minimal, Debugging)
   - Layout switcher in UI

2. **Camera Controls** (1 week)
   - Orthographic views (Top, Front, Side)
   - Focus on Selection (F key)
   - Frame Selection in View
   - Camera bookmarks

3. **Editor Settings Dialog** (1 week)
   - Auto-save interval
   - Grid settings
   - Snap settings
   - Theme customization
   - Font size

4. **Asset Preview System** (1-2 weeks)
   - Zoom/pan for textures
   - 3D model preview with rotation
   - Material preview sphere
   - Audio playback controls

5. **Build Pipeline** (2-3 weeks)
   - Mesh optimization
   - Texture compression
   - Lightmap baking
   - Occlusion culling

---

## Keyboard Shortcuts

### General
- `Ctrl+N` - New World
- `Ctrl+O` - Open World
- `Ctrl+S` - Save World
- `Ctrl+Shift+S` - Save World As

### Edit
- `Ctrl+Z` - Undo
- `Ctrl+Y` or `Ctrl+Shift+Z` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Del` - Delete Selection

### Transform Gizmo
- `W` - Translate Mode
- `E` - Rotate Mode
- `R` - Scale Mode
- `Ctrl` (hold) - Snap to Grid

### Camera
- `F` - Focus on Selection (planned)
- `Alt+LMB` - Orbit Camera
- `Alt+MMB` - Pan Camera
- `Alt+RMB` - Zoom Camera

### Viewport
- `Space` - Play/Pause
- `Esc` - Stop Playing

---

## Related Documentation

- [Editor Integration Guide](../EDITOR_INTEGRATION.md)
- [Input System](../INPUT_SYSTEM.md)
- [Transform Gizmo Implementation](../../TRANSFORM_GIZMO_IMPLEMENTATION.md)
- [Windows Native UI Quick Start](../../WINDOWS_NATIVE_UI_QUICK_START.md)
- [Development Tools](../DEVELOPMENT_TOOLS.md)

---

## Troubleshooting

### GUI Not Showing

1. Check ImGui initialization:
   ```cpp
   if (!m_imguiManager->initialize(hwnd, device, context)) {
       LOG_ERROR("Failed to initialize ImGui");
   }
   ```

2. Verify DirectX device is created
3. Check window handle is valid

### Toolbar Buttons Not Working

1. Verify callback is set:
   ```cpp
   m_toolbar->setToolCallback([this](Tool tool) { ... });
   ```

2. Check button IDs match
3. Enable debug logging

### Gizmo Not Visible

1. Ensure entity is selected
2. Check gizmo is enabled
3. Verify DebugRenderer is initialized
4. Check camera is properly positioned

### High DPI Issues

1. Verify DPI awareness is set in manifest
2. Check ImGui scale factor
3. Update to latest ImGui version

---

**Last Updated**: 2025-11-19  
**Version**: 1.0  
**Status**: Production Ready (95% complete)
