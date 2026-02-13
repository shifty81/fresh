# Fresh Voxel Engine - Project Roadmap & Status Assessment

**Last Updated**: February 2026  
**Engine Version**: 0.1.0 (Alpha)

This document provides an honest assessment of what is implemented, what is partially working, and what needs to be completed for the engine to be fully functional — particularly around viewport rendering, play mode, and editor UI integration.

---

## Table of Contents

1. [Critical Issues](#critical-issues)
2. [Viewport & Rendering Status](#viewport--rendering-status)
3. [Play Mode Status](#play-mode-status)
4. [Editor UI Wiring Audit](#editor-ui-wiring-audit)
5. [System-by-System Status](#system-by-system-status)
6. [Priority Roadmap](#priority-roadmap)
7. [Architecture Notes](#architecture-notes)

---

## Critical Issues

### 🔴 Viewport Blank During World Generation

**Problem**: When creating a new world, the viewport shows nothing during chunk generation because `m_isGeneratingWorld = true` blocks `renderEditor()` from calling `renderVoxelWorld()`.

**Location**: `src/core/Engine.cpp` lines 555-584 (`createNewWorld()`)

**Root Cause**: Chunk generation is synchronous and blocks the render loop. During generation (which loads 49 chunks = 7x7 radius), the `m_isGeneratingWorld` flag prevents any 3D rendering, resulting in a blank/frozen viewport.

**Fix Required**:
- [ ] Move chunk generation to a background thread
- [ ] Render already-loaded chunks while generation continues
- [ ] Add a loading progress indicator to the viewport
- [ ] Consider async chunk loading (load nearest chunks first, expand outward)

### 🔴 Swap Chain Initialization Timing

**Problem**: Viewport swap chain may fail to initialize if the viewport panel has zero dimensions when `setViewportWindow()` + `recreateSwapChain()` are called.

**Location**: `src/core/Engine.cpp` lines 393-464

**Current Mitigation**: The engine retries swap chain creation every frame in `updateEditor()`, but this retry can be fragile.

**Fix Required**:
- [ ] Add explicit "viewport ready" signal from Win32ViewportPanel after first valid resize
- [ ] Guarantee minimum viewport size before attempting swap chain creation
- [ ] Add user-visible error message if swap chain fails after N retries

### 🔴 Scripting System Not Wired

**Problem**: The Lua scripting system (Sol2/LuaJIT) has the framework but the backend is mostly stubs. Scripts cannot actually be loaded or executed in the current state.

**Location**: `src/scripting/lua/LuaScriptingEngine.cpp`, `src/scripting/EventSystem.cpp`

**Fix Required**:
- [ ] Wire Sol2 bindings to actual engine APIs (entity creation, world manipulation)
- [ ] Implement script file loading and execution
- [ ] Connect EventSystem to scripting backend
- [ ] Add script reload capability for development iteration

---

## Viewport & Rendering Status

### DirectX 11 Backend ✅ Fully Functional
| Feature | Status | Notes |
|---------|--------|-------|
| Device/swap chain creation | ✅ Working | FLIP_DISCARD mode |
| Viewport window targeting | ✅ Working | `setViewportWindow()` + `recreateSwapChain()` |
| Voxel world rendering | ✅ Working | Per-chunk mesh generation, index buffers |
| View/projection matrices | ✅ Working | Camera integration |
| Shader compilation | ✅ Working | HLSL vertex + pixel shaders |
| Resize handling | ✅ Working | Swap chain recreation on resize |
| Texture rendering | ❌ Not implemented | `Texture.cpp` has 6+ TODOs |
| Material system | ❌ Not implemented | `ModelLoader.cpp` material processing missing |

### DirectX 12 Backend ⚠️ Partially Functional
| Feature | Status | Notes |
|---------|--------|-------|
| Device/command queue | ✅ Working | Pipeline state objects created |
| Swap chain | ✅ Working | FLIP_DISCARD mode |
| Voxel rendering | ⚠️ Partially working | Implementation exists but less tested |
| Shader compilation | ❌ Stub only | `createShader()` returns empty shader object |
| Texture upload | ❌ Not implemented | "Not yet implemented" warning |

### OpenGL Backend ✅ Fully Functional
| Feature | Status | Notes |
|---------|--------|-------|
| Context creation | ✅ Working | OpenGL 4.5+ |
| Voxel rendering | ✅ Working | glClear + render calls |
| Shader compilation | ✅ Working | GLSL shaders |
| Viewport integration | ⚠️ N/A | OpenGL doesn't use Win32 viewport targeting |

### What Needs Work for Viewport
- [ ] **Texture GPU operations** (6 TODOs in `Texture.cpp`): `bindTexture()`, `unbindTexture()`, GPU upload
- [ ] **DX12 shader compilation**: Currently returns empty stubs
- [ ] **Material processing**: `ModelLoader.cpp` doesn't process .mtl files
- [ ] **Async world generation**: To prevent viewport blanking during new world creation
- [ ] **Loading screen**: Show progress indicator during world generation

---

## Play Mode Status

### Current Flow ✅ Implemented
1. `enterPlayMode()` → Creates separate `GamePlayWindow`
2. Redirects renderer to game window via `setViewportWindow()` + `recreateSwapChain()`
3. Hides editor panels, shows game window
4. `exitPlayMode()` → Closes game window, restores editor viewport

### Game Systems Active During Play Mode ✅ Working
| System | Status | Notes |
|--------|--------|-------|
| Player movement (WASD) | ✅ Working | With sprint, crouch, edge prevention |
| Camera (mouse look) | ✅ Working | First-person with sensitivity |
| Gravity & physics | ✅ Working | Terminal velocity, grounding checks |
| Collision detection | ✅ Working | Voxel-based bounding box (8 samples) |
| Block placement/removal | ✅ Working | Mouse click interaction |
| AI system (NPC behaviors) | ✅ Working | Behavior trees update |
| Day/night cycle | ✅ Working | TimeManager updates |
| Stamina system | ✅ Working | Regen/consumption |

### What Needs Work for Play Mode
- [ ] **Inventory system in play mode**: Hotbar displays but item usage may not be fully connected
- [ ] **NPC interaction UI**: Trading/dialogue prompts need play-mode integration
- [ ] **Audio in play mode**: Sound effects for actions (mining, placing, footsteps)
- [ ] **Pause menu**: No in-game pause menu exists
- [ ] **HUD completeness**: Health/stamina bars display but may need polish
- [ ] **Save/load during gameplay**: Auto-save and manual save from play mode

---

## Editor UI Wiring Audit

### Fully Implemented (21/22 methods) ✅
| Method | Status |
|--------|--------|
| `undo()` / `redo()` | ✅ Connected to TerraformingSystem |
| `cut()` / `copy()` / `paste()` | ✅ Connected to SelectionManager |
| `deleteSelection()` | ✅ Connected to SelectionManager |
| `selectAll()` / `deselectAll()` | ✅ Connected to SceneHierarchy |
| `saveWorld()` / `saveWorldAs()` | ✅ WorldSerializer + Windows dialogs |
| `loadWorld()` | ✅ WorldSerializer + Windows dialogs |
| `newWorld()` | ✅ NativeMainMenu create dialog |
| `newProject()` / `openProject()` | ✅ ProjectManager + FileDialogManager |
| `showSettings()` | ✅ Win32SettingsDialog |
| `showEditorSettings()` | ✅ EditorSettingsDialog |
| `showImportAssets()` | ✅ ContentBrowser import dialog |
| `launchDialogueEditor()` | ✅ Launches external .NET app |
| `loadLayout()` / `saveCurrentLayout()` / `resetLayout()` | ✅ LayoutManager |
| `setCameraViewMode()` | ✅ CameraController |
| `focusOnSelection()` / `frameSelection()` | ✅ CameraController + SelectionManager |
| `toggleSceneHierarchy/Inspector/ContentBrowser/Console` | ✅ Panel visibility |
| `enterPlayMode()` / `exitPlayMode()` | ✅ Engine callbacks |

### Not Implemented
| Method | Status | Reason |
|--------|--------|--------|
| `showEngineConfig()` | ❌ Intentionally disabled | No EngineConfigPanel UI exists |

### New Addition
| Method | Status |
|--------|--------|
| `toggleLLMAssistant()` | ✅ New - Toggles AI assistant panel |

---

## System-by-System Status

### Core Engine ✅ 90% Complete
- [x] Game loop with proper timing (hybrid sleep + spin-wait)
- [x] Editor/play mode separation
- [x] Window management (Win32)
- [x] Input handling (keyboard, mouse)
- [x] Logger system with listeners
- [x] Resource manager
- [x] Scene serialization (JSON)
- [ ] Hot-reload for scripts/assets

### Voxel System ✅ 80% Complete
- [x] 16×256×16 chunk system
- [x] Greedy meshing with face culling
- [x] Infinite chunk streaming
- [x] Block type system (40+ types)
- [x] Chunk dirty flagging and regeneration
- [ ] Transparent block rendering (water, glass)
- [ ] Block lighting system
- [ ] Water physics simulation
- [ ] Level-of-detail for distant chunks

### Terrain Generation ✅ 75% Complete
- [x] Perlin/Simplex noise generation
- [x] Heightmap terrain
- [x] Multi-biome support
- [x] Tree/structure placement
- [ ] Cave generation
- [ ] Ore distribution per biome
- [ ] Custom generation plugins
- [ ] Async generation (currently blocks render)

### Editor ✅ 85% Complete
- [x] Unreal-style layout with all major panels
- [x] Terraforming tools (10 tool types)
- [x] Construction hammer (7 blueprints)
- [x] Scene hierarchy with tree view
- [x] Inspector with property editing
- [x] Content browser
- [x] Console with command input
- [x] Transform gizmo (move/rotate/scale)
- [x] Undo/redo (100-step history)
- [x] Layout persistence
- [x] AI Assistant panel (NEW)
- [ ] Multi-select in viewport
- [ ] Prefab system
- [ ] Drag-and-drop asset placement

### ECS (Entity-Component-System) ✅ 85% Complete
- [x] Entity creation/destruction
- [x] Component registration/retrieval
- [x] 2D physics/collision components
- [x] Animation, sprite, camera components
- [x] Health, movement, combat components
- [ ] 3D physics integration with ECS
- [ ] Spatial partitioning optimization

### AI System ⚠️ 60% Complete
- [x] Behavior tree framework (Sequence, Selector nodes)
- [x] Wander and Follow behaviors
- [x] NPC type system (Friendly, Hostile, Trader, etc.)
- [x] AISystem update loop
- [ ] Pathfinding (A* or navigation mesh)
- [ ] Line-of-sight detection
- [ ] Advanced behaviors (flee, investigate, alert others)
- [x] LLM-powered assistant for developers (NEW)

### Scripting ⚠️ 40% Complete
- [x] LuaJIT/Sol2 dependency configured
- [x] ScriptingEngine interface defined
- [x] EventSystem shell
- [ ] **Sol2 bindings to engine APIs** (CRITICAL)
- [ ] Script file loading and execution
- [ ] Script hot-reload
- [ ] Python bindings beyond stubs

### Audio ✅ 95% Complete
- [x] OpenAL integration
- [x] WAV file loading
- [x] Vorbis (OGG) codec support
- [x] 3D positional audio
- [x] Listener management
- [ ] Music streaming
- [ ] Audio effects processing

### Networking ⚠️ 10% Complete
- [x] Socket infrastructure
- [x] Server accept connections
- [ ] **Message protocol implementation**
- [ ] Player synchronization
- [ ] Chunk streaming over network
- [ ] Client prediction and interpolation

### RPG Systems ✅ 75% Complete
- [x] Inventory system
- [x] Crafting system
- [x] Trading system framework
- [x] Faction component
- [x] Progression component
- [x] Loot system
- [ ] Quest system
- [ ] Dialogue tree execution
- [ ] Experience/leveling

### Physics ✅ 70% Complete
- [x] Custom collision detection
- [x] Gravity and velocity
- [x] Grounding checks
- [x] Player collision (voxel-based)
- [ ] Rigid body dynamics
- [ ] Spatial partitioning (optimization)
- [ ] Projectile physics

---

## Priority Roadmap

### Phase 1: Core Playability (Immediate)
These items are needed for the engine to be usable for basic game testing:

1. **🔴 Fix viewport blank during world generation**
   - Move chunk generation to background thread
   - Render progressively loaded chunks
   
2. **🔴 Ensure swap chain initializes reliably**
   - Add viewport ready signaling
   - Minimum size enforcement
   
3. **🟠 Complete texture GPU operations**
   - Implement `Texture.cpp` GPU upload/bind
   - Enable textured voxel rendering

4. **🟠 Add block lighting system**
   - Ambient occlusion per-vertex
   - Sunlight propagation
   - Torch/light block emission

### Phase 2: Scripting & Interactivity (High Priority)
5. **🔴 Wire Lua scripting backend**
   - Sol2 bindings for entity, world, input APIs
   - Script file loading from project directory
   - Enable `onUpdate()`, `onInit()` lifecycle hooks

6. **🟠 NPC interaction system**
   - Dialogue display in play mode
   - Trading UI
   - Pathfinding (A* on voxel grid)

7. **🟠 Complete play mode experience**
   - Pause menu
   - In-game save/load
   - Sound effects for core actions

### Phase 3: Polish & Tools (Medium Priority)
8. **🟡 Editor improvements**
   - Multi-select in 3D viewport
   - Prefab save/load system
   - Asset drag-and-drop

9. **🟡 DX12 shader compilation**
   - Implement `createShader()` properly
   - HLSL compilation for DX12 pipeline

10. **🟡 Transparent block rendering**
    - Water, glass, leaves transparency
    - Proper depth sorting

### Phase 4: Advanced Features (Future)
11. Multiplayer networking protocol
12. Advanced terrain generation (caves, custom plugins)
13. Quest system implementation
14. Level-of-detail for distant chunks
15. Audio effects and music streaming

---

## Architecture Notes

### Render Pipeline Flow
```
Engine::run()
  └─ render()
       ├─ renderEditor() [editor mode]
       │    ├─ setViewport()
       │    ├─ DX11/DX12/GL: renderVoxelWorld(world, player)
       │    ├─ EditorManager::render() [overlay UI]
       │    └─ endFrame() [present]
       │
       └─ renderGamePlayWindow() [play mode]
            ├─ Separate window swap chain
            ├─ renderVoxelWorld(world, player)
            └─ HUD overlay
```

### Panel Layout (Unreal Engine Style)
```
┌─────────────────────────────────────────────────────┐
│  Menu Bar + Toolbar (80px)                           │
├──────┬────────────────────────────┬──────────────────┤
│ Left │    Center Viewport         │  Right (350px)   │
│ Tool │    (DirectX renders here)  │ ┌──────────────┐ │
│ Panel│                            │ │ Scene Hier.  │ │
│ 220px│                            │ │ (40% height) │ │
│      │                            │ ├──────────────┤ │
│      │                            │ │ Inspector    │ │
│      │                            │ │ (60% height) │ │
├──────┴──────────────┬─────────────┴──────────────────┤
│ Content Browser     │ Console / AI Assistant          │
│ (700px, 220px)      │ (dynamic width, 220px)          │
├─────────────────────┴────────────────────────────────┤
│ Status Bar                                            │
└──────────────────────────────────────────────────────┘
```

### Key Files for Each System
| System | Header | Implementation |
|--------|--------|----------------|
| Engine Loop | `include/core/Engine.h` | `src/core/Engine.cpp` |
| Viewport | `include/ui/native/Win32ViewportPanel.h` | `src/ui/native/Win32ViewportPanel.cpp` |
| DX11 Renderer | `include/renderer/backends/DirectX11RenderContext.h` | `src/renderer/backends/DirectX11RenderContext.cpp` |
| Editor Manager | `include/editor/EditorManager.h` | `src/editor/EditorManager.cpp` |
| Player | `include/gameplay/Player.h` | `src/gameplay/Player.cpp` |
| Voxel World | `include/voxel/VoxelWorld.h` | `src/voxel/VoxelWorld.cpp` |
| AI/LLM | `include/ai/LLMClient.h` | `src/ai/LLMClient.cpp` |
| Lua Scripting | `include/scripting/lua/LuaScriptingEngine.h` | `src/scripting/lua/LuaScriptingEngine.cpp` |
