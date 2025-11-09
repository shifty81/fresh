# Fresh Voxel Engine - Features Guide

This document describes all features available in the Fresh Voxel Engine.

---

## 🎮 Core Gameplay Features

### First-Person Movement
- **WASD Controls** - Standard FPS movement
- **Mouse Look** - Smooth camera rotation
- **Jump** - Space bar for jumping
- **Sprint** - Shift for 2x speed
- **Crouch** - Ctrl for slower, stealthy movement
- **Physics** - Gravity, collision detection, grounded state

### Block Interaction
- **Place Blocks** - Right-click to place (currently Stone)
- **Remove Blocks** - Left-click to break blocks
- **Raycasting** - Accurate block selection
- **Crosshair** - Visual aim indicator
- **Instant Feedback** - Immediate world updates

### World Exploration
- **Procedural Generation** - Infinite terrain using Perlin noise
- **Chunk Streaming** - World loads dynamically around player
- **Biomes** - Hills, valleys, flat areas
- **Caves** - Underground cave systems
- **Natural Features** - Varied terrain with smooth transitions

---

## 🏗️ Voxel World System

### Chunk Management
- **Chunk Size** - 16×256×16 voxels per chunk
- **Dynamic Loading** - Chunks load/unload based on player position
- **Efficient Storage** - Only stores non-air blocks
- **Mesh Generation** - Automatic mesh creation from voxel data
- **Greedy Meshing** - Optimizes rendering performance

### Voxel Types
The engine supports 13+ different voxel types:

1. **Air** - Empty space
2. **Stone** - Gray, solid foundation
3. **Dirt** - Brown earth
4. **Grass** - Green top, brown sides
5. **Sand** - Yellow, beaches
6. **Water** - Blue, transparent (planned)
7. **Wood** - Tree trunks
8. **Leaves** - Green, transparent
9. **Cobblestone** - Textured stone
10. **Planks** - Processed wood
11. **Brick** - Red building material
12. **Ore** - Various mineral types
13. **Glass** - Transparent blocks

### Mesh Generation
- **Face Culling** - Hidden faces not rendered
- **Greedy Meshing** - Combines adjacent faces
- **Vertex Optimization** - Reduced memory usage
- **Dynamic Updates** - Meshes update when world changes

---

## 🎨 Integrated Editor

### Editor Interface
- **Toggle** - Press T to show/hide editor
- **ImGui-Based** - Professional, responsive UI
- **Dockable Panels** - Customizable layout
- **Theme Support** - Dark theme by default

### Editor Panels

#### Menu Bar
- **File** - New, Open, Save, Save As, Exit
- **Edit** - Undo, Redo, Cut, Copy, Paste, Delete
- **Window** - Toggle panels, layouts, reset
- **Build** - Build settings, export
- **Settings** - Editor preferences, graphics
- **Help** - Documentation, about

#### Toolbar
- **Transform Tools**
  - Select (Q) - Choose objects
  - Move (W) - Translate position
  - Rotate (E) - Change orientation
  - Scale (R) - Resize objects
  
- **Editing Tools**
  - Place (B) - Add blocks
  - Remove (X) - Delete blocks
  - Paint (P) - Change block type

#### Scene Hierarchy
- Tree view of world structure
- Shows chunks, entities, lights
- Click to select and edit
- Drag to rearrange

#### Inspector
- Properties of selected object
- Transform (position, rotation, scale)
- Components (physics, rendering)
- Materials and textures
- Live editing with instant preview

#### Content Browser
- Asset management system
- Textures (42 block types, 11 UI)
- Models, sounds, scripts
- Preview and search
- Drag and drop to scene

#### Console
- Log messages (info, warning, error)
- Filter by type and component
- Command execution
- Clear and export functions

#### Voxel Tool Palette
- 10+ terraforming tools
- 13 material types with color coding
- Tool size slider (1-10 blocks)
- Undo/Redo buttons

### Keyboard Shortcuts
```
File Operations:
  Ctrl+N        - New world
  Ctrl+O        - Open world
  Ctrl+S        - Save world
  Ctrl+Shift+S  - Save As

Edit Operations:
  Ctrl+Z        - Undo (100 operations)
  Ctrl+Y        - Redo
  Ctrl+C        - Copy
  Ctrl+V        - Paste
  Delete        - Delete selected

Transform Tools:
  Q             - Select tool
  W             - Move tool
  E             - Rotate tool
  R             - Scale tool

Editing Tools:
  B             - Place blocks
  X             - Remove blocks
  P             - Paint tool

Editor:
  T             - Toggle editor
  Alt           - Temporary UI mode (hold)
```

---

## 🛠️ Terraforming System

### Available Tools

#### Basic Tools
- **Single Block** - Precise single-block placement
- **Brush** - Natural terrain sculpting (spherical)
- **Line** - Draw straight lines between points

#### Shape Tools
- **Sphere** - Hollow sphere
- **Filled Sphere** - Solid sphere (great for hills)
- **Cube** - Hollow cube
- **Filled Cube** - Solid cube (great for buildings)

#### Terrain Tools
- **Flatten** - Level terrain to specific height
- **Smooth** - Smooth terrain by averaging heights
- **Raise** - Elevate terrain
- **Lower** - Dig down into terrain

#### Material Tools
- **Paint** - Change block types without changing geometry
- **Replace** - Replace one block type with another

### Editing Modes
- **Place Mode** - Add blocks to the world
- **Remove Mode** - Remove blocks from the world
- **Replace Mode** - Swap block types

### Tool Settings
- **Size** - Adjustable from 1-10 blocks
- **Strength** - 0-100% effect intensity
- **Shape** - Round or square brush
- **Fill** - Hollow or solid

### Undo/Redo
- **Capacity** - Up to 100 operations
- **Speed** - Instant undo/redo
- **Persistence** - History saved with world
- **Smart Tracking** - Only stores changed chunks

---

## 🎮 Main Menu System

### Menu Options
- **Create New World**
  - Enter world name
  - Optional seed (or random)
  - World size selection
  
- **Load Existing World**
  - Browse saved worlds
  - Preview world info
  - Load any .world file
  
- **Settings** (Planned)
  - Graphics quality
  - Audio volume
  - Controls configuration

### World Management
- **Save Location** - saves/ directory
- **File Format** - Binary .world files
- **Compression** - Optional compression
- **Auto-Save** - Configurable intervals

---

## 🎨 Graphics & Rendering

### DirectX Support
- **DirectX 12** - Default, best performance (Windows 10+)
- **DirectX 11** - Automatic fallback, excellent compatibility
- **Auto-Detection** - Engine selects best available API
- **Feature Set** - Full shader support, modern effects

### Rendering Features
- **Voxel Rendering** - Optimized block display
- **Lighting** - Directional light simulation
- **Depth Testing** - Correct occlusion
- **Face Culling** - Back-face culling for performance
- **Shader System** - HLSL shaders for DirectX
- **Hot-Reload** - Shader changes apply instantly

### Performance
- **Target** - 60 FPS
- **Render Distance** - 3-5 chunks (48-80 blocks)
- **Optimization** - Frustum culling, greedy meshing
- **Memory** - Efficient chunk-based memory management

---

## 🔧 Technical Systems

### Physics Engine
- **Gravity** - -9.8 m/s² acceleration
- **Collision Detection** - AABB vs voxels
- **Grounded Detection** - Accurate jumping
- **Friction** - Movement damping

### Input System
- **Multiple Modes** - Game, UI, Editor, Build
- **Mode Switching** - Automatic and manual
- **Hold Alt Feature** - Temporary UI cursor access
- **Action Mapping** - Configurable key bindings

### Audio Engine
- **OpenAL** - 3D positional audio
- **Format Support** - WAV (OGG/Vorbis planned)
- **Sound Effects** - Footsteps, block sounds
- **Music** - Background music support

### Logging System
- **Log Levels** - INFO, WARNING, ERROR, FATAL
- **Component Logging** - Categorized messages
- **File Output** - Timestamped log files
- **Thread-Safe** - Multi-threaded logging
- **Location** - logs/ directory

### Serialization
- **Binary Format** - Efficient world storage
- **Compression** - Optional gzip compression
- **Chunk-Based** - Only saves loaded chunks
- **Metadata** - World name, seed, spawn point

---

## 🔮 Optional Features

### .NET 9 Bindings
- **C# Support** - Write game logic in C#
- **P/Invoke** - Seamless interop
- **Modern .NET** - Latest C# features
- **Visual Studio** - Full IntelliSense support

### Lua Scripting (Framework Ready)
- **Lua Integration** - Embedded Lua interpreter
- **API Bindings** - Access engine from Lua
- **Hot-Reload** - Script changes apply live
- **Status** - ~40% complete, needs Sol2

### Networking (Framework Only)
- **Architecture** - Client-server model
- **Message System** - Network messaging framework
- **Status** - ~10% complete, not functional yet

---

## 📦 Asset System

### Texture System
- **Block Textures** - 42 textures (16x16, 32x32, 64x64)
- **UI Assets** - 11 UI elements
- **Format Support** - PNG with alpha
- **Procedural** - Placeholder textures included
- **Custom** - Easy to add your own

### Asset Management
- **Modular System** - Plugin-based architecture
- **Asset Packs** - Bundle assets together
- **Hot-Reload** - Assets update without restart
- **Content Browser** - Visual asset management

---

## 🎯 Current Limitations

### Platform
- **Windows Only** - DirectX requires Windows 10/11
- **x64** - 64-bit systems only

### Graphics
- **DirectX 11** - Production ready ✅
- **DirectX 12** - Core features complete (85%)

### Features
- **Multiplayer** - Framework only, not functional
- **Lua Scripting** - Framework exists, needs integration
- **Audio Formats** - WAV only, OGG planned

### Performance
- **Large Worlds** - Memory not yet optimized
- **Test Coverage** - ~30% (core systems tested)

---

## 🚀 Running the Engine

### Quick Start
1. Build the project (see BUILD.md)
2. Run `FreshVoxelEngine.exe`
3. Create or load a world
4. Press T to toggle editor
5. Explore and build!

### Controls Summary
- **WASD** - Move
- **Mouse** - Look around
- **Space** - Jump
- **Shift** - Sprint
- **Ctrl** - Crouch
- **Left Click** - Break blocks
- **Right Click** - Place blocks
- **T** - Toggle editor
- **ESC** - Menu / Exit

---

## 📚 Documentation

For detailed information on specific features:

- **[GETTING_STARTED.md](GETTING_STARTED.md)** - Setup and first steps
- **[CONTROLS.md](CONTROLS.md)** - Complete control reference
- **[docs/EDITOR_INTEGRATION.md](docs/EDITOR_INTEGRATION.md)** - Editor guide
- **[docs/TERRAFORMING.md](docs/TERRAFORMING.md)** - Terraforming tools
- **[docs/INPUT_SYSTEM.md](docs/INPUT_SYSTEM.md)** - Input system details
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture

---

**The Fresh Voxel Engine is a complete, playable voxel game engine with professional editing tools!**
