# Game Engine Editor Integration Guide

## Overview

The Fresh Voxel Engine now includes a comprehensive, integrated game engine editor with a modern GUI interface built on ImGui. This editor provides professional-level tools for world editing, asset management, and game development.

## Features

### General Editor Interface

The editor provides a complete IDE-like experience with multiple panels and tools:

#### 1. **Menu Bar**
Standard menu system with full functionality:
- **File Menu**: New World, Open World, Save, Save As, Import/Export, Recent Worlds
- **Edit Menu**: Undo, Redo, Cut, Copy, Paste, Delete, Select All, Preferences
- **Window Menu**: Toggle panels, Layout management, Reset Layout
- **Build Menu**: Build World, Build and Run, Generate Lightmaps, Optimize
- **Settings Menu**: Graphics API selection, Quality settings, Input/Audio/Editor settings
- **Help Menu**: Documentation, API Reference, Tutorials, Bug Reports, About

#### 2. **Toolbar**
Quick-access toolbar with essential tools:
- **Transform Tools**: Select, Move (W), Rotate (E), Scale (R)
- **Editing Tools**: Place (B), Remove (X), Paint (P)
- **Play Controls**: Play, Pause, Stop buttons for testing

#### 3. **Scene Hierarchy Panel**
Tree-structure view of all objects in the scene:
- Browse world structure (World → Chunks → Entities → Lights)
- Select objects for editing
- Show/hide objects
- Context menu with Rename, Duplicate, Delete options
- Search/filter functionality
- Up to 100 chunks displayed (with more indicator)

#### 4. **Inspector/Details Panel**
Dynamic property editor for selected objects:
- **Transform Properties**: Position, Rotation, Scale with drag controls
- **Component Properties**: Physics, Renderer, Audio Source, Scripts
- **Material Properties**: Shader selection, Color picker, Texture assignment
- **Add Component**: Button with popup menu for adding new components

#### 5. **Content Browser / Asset Manager**
File browser for project assets:
- Navigate asset folders
- Preview assets with icons (textures 🖼️, models 📦, sounds 🔊, scripts 📄)
- Search and filter assets
- Import new assets
- Asset details panel showing name, type, path, size
- Operations: Open, Delete, Rename

#### 6. **Console / Status Bar**
Message logging and command execution:
- Filter messages by type (Info, Warning, Error, Success)
- Timestamped messages
- Auto-scroll option
- Command input for console commands
- Clear history button
- Color-coded messages

### Voxel & World Editing

#### 7. **Voxel Tool Palette**
Comprehensive voxel editing tools:

**Basic Tools:**
- Single Block - Precise single-block placement
- Brush - Natural terrain sculpting with spherical brush
- Sphere - Create hollow spherical structures
- Filled Sphere - Create solid spherical structures

**Shape Tools:**
- Cube - Create hollow cubic structures
- Filled Cube - Create solid cubic structures
- Line - Draw straight lines between points

**Terrain Tools:**
- Flatten - Level terrain to specific height
- Smooth - Smooth terrain by averaging heights
- Paint - Change block types without geometry changes

#### 8. **Material/Texture Picker**
Select from available voxel types:

**Basic Materials:**
- Air, Stone, Dirt, Grass, Sand, Water

**Natural Materials:**
- Wood, Leaves, Snow, Ice

**Crafted Materials:**
- Cobblestone, Planks, Glass

Color-coded buttons for easy identification.

#### 9. **Tool Settings**
Configurable parameters:
- **Tool Size**: Slider from 1-10 blocks
- **Strength**: Brush intensity (0.1-2.0)
- **Procedural Settings**: Noise scale, strength, seed for terrain generation

#### 10. **Quick Actions**
- Undo (Ctrl+Z) / Redo (Ctrl+Y)
- Generate Terrain
- Clear Selection

## Installation & Setup

### Prerequisites

1. **ImGui Library**
   ```bash
   vcpkg install imgui[glfw-binding,opengl3-binding,dx11-binding,dx12-binding]:x64-windows
   ```

2. **CMake 3.20+** and **Visual Studio 2017+**

### Building with Editor

The editor is automatically built when ImGui is available. The build system detects ImGui and enables editor features:

```bash
# Generate Visual Studio solution
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg_root]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Release
```

If ImGui is not found, the engine falls back to console-based editor mode.

## Usage

### Starting the Editor

1. Launch the engine - it will show the main menu
2. Create a new world or load an existing one
3. The editor interface loads automatically with all panels visible

### Keyboard Shortcuts

**File Operations:**
- `Ctrl+N` - New World
- `Ctrl+O` - Open World
- `Ctrl+S` - Save World
- `Ctrl+Shift+S` - Save World As
- `Alt+F4` - Exit

**Editing:**
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Delete` - Delete selection
- `Ctrl+A` - Select All
- `Ctrl+D` - Deselect All

**Tools:**
- `W` - Move tool
- `E` - Rotate tool
- `R` - Scale tool
- `B` - Place tool
- `X` - Remove tool
- `P` - Paint tool

**Playmode:**
- `F5` - Build and Run
- `F7` - Build World

**Help:**
- `F1` - Documentation

### Panel Management

**Window Menu** allows you to:
- Toggle individual panels on/off
- Choose from predefined layouts (Default, Minimal, Debugging)
- Save current layout
- Reset to default layout

**Docking:**
- Drag panel tabs to dock them in different positions
- Create tabbed panel groups
- Split panels horizontally or vertically
- Undock panels into floating windows

### Workflow Examples

#### 1. Creating Terrain

```
1. Select "Voxel Tools" panel
2. Choose "Filled Sphere" or "Brush" tool
3. Set tool size (e.g., 5)
4. Select material (e.g., Stone)
5. Click in viewport to place voxels
6. Use "Smooth" tool to refine
7. Press Ctrl+S to save
```

#### 2. Editing Existing Objects

```
1. Click object in viewport or Scene Hierarchy
2. Inspector panel shows properties
3. Modify transform values
4. Add/remove components
5. Change material settings
6. Changes apply in real-time
```

#### 3. Managing Assets

```
1. Open Content Browser panel
2. Navigate to asset folder
3. Click to select assets
4. View details in bottom panel
5. Double-click to open
6. Use Import button for new assets
```

## Input Handling

The editor implements smart input handling that differentiates between GUI and world interactions:

### Mouse Input
- **GUI Interaction**: When mouse is over any UI panel, input is captured by the UI
- **World Interaction**: When mouse is in viewport, input goes to the 3D world
- **Left Click**: Select/activate in UI, place blocks in world
- **Right Click**: Context menus in UI, remove blocks in world
- **Middle Click/Scroll**: Pan/zoom in UI, camera control in world

### Keyboard Input
- **GUI Focus**: When typing in text boxes, only GUI receives input
- **World Focus**: When viewport is active, shortcuts and controls affect the world
- **Smart Detection**: ImGui's `WantCaptureKeyboard` flag handles this automatically

### Implementation

The `EditorManager` provides these query methods:
```cpp
bool wantCaptureMouse() const;    // Returns true if GUI wants mouse
bool wantCaptureKeyboard() const; // Returns true if GUI wants keyboard
```

Use these in your input processing:
```cpp
if (!editorManager->wantCaptureMouse()) {
    // Process world mouse input
    processWorldMouseInput();
}

if (!editorManager->wantCaptureKeyboard()) {
    // Process world keyboard input
    processWorldKeyboardInput();
}
```

## Architecture

### Component Structure

```
EditorManager (Main Controller)
├── ImGuiContext (ImGui wrapper)
├── SceneHierarchyPanel (Object tree)
├── InspectorPanel (Property editor)
├── EditorMenuBar (Menu system)
├── EditorToolbar (Quick tools)
├── ContentBrowserPanel (Asset manager)
├── ConsolePanel (Logging/commands)
└── VoxelToolPalette (Voxel tools)
```

### Integration Points

The editor integrates with the engine through:
- `VoxelWorld` - Direct world manipulation
- `WorldEditor` - High-level editing operations
- `TerraformingSystem` - Voxel editing tools
- `Window` - GLFW window handle for ImGui
- `IRenderContext` - Graphics API backend

### Graphics API Support

The editor works with all supported graphics APIs:
- **OpenGL 4.5+**: Full support with OpenGL3 ImGui backend
- **DirectX 11**: Planned support (backend structure in place)
- **DirectX 12**: Planned support (backend structure in place)

Currently, OpenGL is fully implemented. DirectX backends will be added in future updates.

## Customization

### Adding New Panels

1. Create panel class inheriting common interface
2. Initialize in `EditorManager::initialize()`
3. Add to `EditorManager::render()`
4. Add visibility flag and menu item

Example:
```cpp
// In EditorManager.h
std::unique_ptr<MyCustomPanel> m_customPanel;
bool m_showCustomPanel;

// In EditorManager.cpp initialize()
m_customPanel = std::make_unique<MyCustomPanel>();
m_customPanel->initialize();

// In EditorManager.cpp render()
if (m_showCustomPanel && m_customPanel) {
    m_customPanel->render();
}
```

### Styling

ImGui style is configured in `ImGuiContext::initialize()`:
- Window rounding: 5.0f
- Frame rounding: 3.0f
- Scrollbar rounding: 3.0f
- Dark theme with blue accents

Modify `ImGuiStyle` values to customize appearance.

### Adding Menu Items

Edit `EditorMenuBar::render*Menu()` functions to add new menu items:
```cpp
if (ImGui::MenuItem("My Action", "Ctrl+M")) {
    if (m_myActionCallback) {
        m_myActionCallback();
    }
}
```

## Performance Considerations

- **Scene Hierarchy**: Limited to displaying first 100 chunks to avoid performance issues
- **Console**: Maximum 1000 messages with automatic cleanup
- **ImGui Rendering**: Optimized with immediate mode paradigm
- **Docking**: Minimal overhead, hardware accelerated

## Troubleshooting

### Editor Not Appearing
- Check that ImGui is installed via vcpkg
- Verify `FRESH_IMGUI_AVAILABLE` is defined during compilation
- Check console for ImGui initialization errors

### Panels Not Responding
- Ensure window has focus
- Check visibility flags in Window menu
- Try resetting layout

### Graphics API Issues
- OpenGL: Ensure drivers support OpenGL 4.5+
- DirectX: DirectX 11/12 ImGui backends are planned for future updates
- Check console for backend initialization messages

## Future Enhancements

### Planned Features
- GIMP-like image editor plugin for texture editing
- Visual scripting system
- Animation timeline
- Particle effect editor
- Sound wave editor
- Material graph editor
- Heightmap generator
- Blueprint system

### DirectX Support
ImGui backends for DirectX 11 and DirectX 12 will be fully implemented to match OpenGL functionality.

## API Reference

### EditorManager

Main editor coordinator class.

**Methods:**
- `bool initialize(Window*, IRenderContext*, VoxelWorld*, WorldEditor*)` - Initialize editor
- `void beginFrame()` - Start new frame
- `void render()` - Render all UI
- `void endFrame()` - Finish frame and render
- `void shutdown()` - Cleanup
- `bool wantCaptureMouse()` - Check if GUI wants mouse
- `bool wantCaptureKeyboard()` - Check if GUI wants keyboard

### ImGuiContext

Wrapper for ImGui initialization and rendering.

**Methods:**
- `bool initialize(Window*, IRenderContext*)` - Initialize ImGui
- `void newFrame()` - Begin new ImGui frame
- `void render()` - Render ImGui
- `void shutdown()` - Cleanup ImGui

### Panel Classes

All panels implement a common interface:
- `bool initialize(...)` - Initialize with required dependencies
- `void render()` - Render panel UI
- `void setVisible(bool)` - Show/hide panel
- `bool isVisible()` - Check visibility

## Contributing

When contributing editor features:
1. Follow the existing panel structure
2. Use ImGui immediate mode paradigm
3. Integrate with EditorManager
4. Add menu items in EditorMenuBar
5. Document new features
6. Test with all graphics APIs

## License

Part of Fresh Voxel Engine - MIT License

## Resources

- [ImGui Documentation](https://github.com/ocornut/imgui)
- [Fresh Engine Documentation](../README.md)
- [Architecture Overview](../ARCHITECTURE.md)
- [Contributing Guide](../CONTRIBUTING.md)
