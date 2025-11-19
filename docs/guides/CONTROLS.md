# Fresh Voxel Engine - Controls Guide (Editor-First Mode)

## Overview

Fresh Voxel Engine now operates in **Editor-First Mode** by default. This means:
- The editor UI is always visible and accessible
- You start in **Free-Flying Camera Mode** for world building and exploration
- Press **Play** in the editor toolbar to switch to **Play Mode** with normal character physics
- Mouse sensitivity has been increased 10x for better responsiveness

## Mouse Controls

### In-Game Mouse Controls
- **Mouse Movement** - Look around / Camera control (first-person view)
  - **Improved Sensitivity:** Mouse now responds 10x faster than before
- **Left Mouse Button** - Break/remove blocks (aim at block and click)
- **Right Mouse Button** - Place blocks (aim at adjacent surface and click)
- **Mouse Wheel** - (Future: Block type selection)

### Editor Mouse Controls  
The editor is **always visible** and provides:
- **Mouse** - Navigate and interact with all editor UI panels
- **Click** - Select tools, blocks, and options
- **Drag** - Move sliders and adjust values
- **Scroll** - Scroll through lists and options

**Note:** Mouse cursor is shown when interacting with editor UI panels. Camera control works when not hovering over UI.

### Mouse Mode Toggle
- **F Key** - Toggle between camera freelook and GUI mode
  - **Camera Freelook Mode:** Mouse is captured and hidden for camera control
  - **GUI Mode:** Mouse is free and visible for interacting with UI panels
  - Alternative to Alt-hold for persistent mode switching

## Keyboard Controls

### Free-Flying Camera Mode (Default/Editor Mode)

When in **Editor Mode** (default state):
- **W** - Move forward in camera direction
- **S** - Move backward in camera direction
- **A** - Strafe left
- **D** - Strafe right
- **Space** - Move up (ascend)
- **Ctrl** - Move down (descend)
- **Shift** - Move faster (sprint speed)
- **No gravity** - Free 6-degrees-of-freedom movement
- **No collision** - Fly through blocks and terrain

### Play Mode (Via Editor Toolbar)

When in **Play Mode** (click Play button in editor toolbar):
- **W** - Move forward
- **S** - Move backward
- **A** - Strafe left
- **D** - Strafe right
- **Space** - Jump (when on ground)
- **Shift** - Sprint (move faster)
- **Ctrl** - Crouch (move slower, reduced height, **prevents falling off edges**)
- **1-9, 0** - Select hotbar slot (for quick item/tool access)
- **Hold Alt** - Temporarily show cursor to interact with menus while in play mode
- **Gravity enabled** - Normal physics simulation
- **Collision enabled** - Cannot walk through blocks

### Hotbar (Play Mode Only)

In Play Mode, a hotbar with 10 slots appears at the bottom middle of the screen:
- **Keys 1-9** - Select slots 1-9
- **Key 0** - Select slot 10 (the last slot)
- Selected slot is highlighted in blue
- Each slot can be linked to an inventory item or tool
- Press the slot number to quickly equip/use the assigned item

### Action Controls

- **F** - Toggle mouse capture (camera freelook ↔ GUI mode)
- **T** - Disabled (editor is always visible)
- **ESC** - Pause game / Return to menu
- **E** - Use/Interact (future feature)
- **Tab** - Open inventory (future feature)

## Main Menu (Pre-Game)

The initial main menu appears in the console before graphics initialization:
- **Number Keys (1-4)** - Select menu options
- **Enter** - Confirm selection
- **Type Text** - Enter world names and seeds

**Note:** This pre-game menu uses console input. Once in-game, all features support full mouse interaction.

## Block Interaction

- **Left Click** - Break/remove block (aim at block and click)
- **Right Click** - Place block (aim at adjacent surface and click)

**Note:** Currently places Stone blocks by default. Future versions will support block type selection.

## Editor Toolbar

The editor toolbar is **always visible** at the top of the screen and provides:

### Mode Control
- **Play Button** - Switch to Play Mode (normal character with physics)
- **Stop Button** - Return to Editor Mode (free-flying camera)
- **Pause Button** - Pause simulation (future feature)

### Editor Tools (All Mouse-Enabled)
- **Tool Palette** - Click to select terraforming tools (brush, sphere, cube, etc.)
- **Material Picker** - Click to choose block types (stone, dirt, grass, etc.)
- **Tool Size Slider** - Drag to adjust tool size (1-10)
- **Scene Hierarchy** - Click to navigate world structure
- **Content Browser** - Click to browse and manage assets
- **Inspector Panel** - Click to edit properties
- **Console Panel** - Click to execute commands

### Quick Actions
- **Hold Alt** - Temporarily show mouse cursor focus on UI without losing camera control

## Gameplay Tips

1. **Starting Position:** You spawn at coordinates (0, 80, 0) in the world
2. **Default Mode:** Free-flying camera mode for world building and exploration
3. **Building:** Right-click places blocks, left-click removes them
4. **Exploration:** The world generates terrain as you move around
5. **Mouse Look:** Move mouse to look around freely - 10x more responsive than before!
6. **Editor Always On:** Editor UI is always visible - use Play button to test gameplay
7. **Crouch Safety:** In Play Mode, crouching prevents you from falling off block edges
8. **Free Flight:** In Editor Mode, use Space/Ctrl to fly up/down with no gravity

## World Creation

When you start the engine:
1. The console menu appears (use number keys to navigate)
2. Choose "Create New World" (press 1) or "Load Existing World" (press 2)
3. Enter a world name using keyboard
4. Optionally enter a seed number (or leave blank for random)
5. The world will generate and you'll spawn in **Editor Mode** with the editor UI visible

## Performance

- **Target FPS:** 60 FPS
- **Render Distance:** ~3-5 chunks in each direction
- **Chunk Size:** 16x256x16 voxels

## Modes

The engine now has two main modes accessible via the editor toolbar:

### Editor Mode (Default)
- **Camera:** Free-flying camera with 6DOF movement
- **Gravity:** Disabled
- **Collision:** Disabled (fly through blocks)
- **Purpose:** World building, exploration, asset placement
- **Controls:** W/A/S/D + Space/Ctrl for 3D movement
- **Editor UI:** Fully visible and interactive

### Play Mode (Via Toolbar)
- **Camera:** First-person character controller
- **Gravity:** Enabled
- **Collision:** Enabled (realistic physics)
- **Purpose:** Testing gameplay, character interactions
- **Controls:** W/A/S/D + Space to jump, Ctrl to crouch
- **Crouch Feature:** Prevents falling off edges for safety
- **Hotbar:** 10-slot quick access bar at bottom (keys 1-0)
- **Alt-Hold:** Temporarily access menus without exiting play mode
- **Cursor:** Hidden for mouse look, shown when Alt held
- **Editor UI:** Accessible via Alt key when needed

## Known Limitations

- Pre-game console menu is keyboard-only (appears before graphics initialization)
- T key is disabled (editor is always visible)

## New Features

- ✅ **Mouse Mode Toggle (F Key)** - Toggle between camera freelook and GUI mode
- ✅ **Smart Mouse Look** - Works in both editor and play mode when not using UI
- ✅ **Dynamic Cursor** - Shows/hides automatically based on context
- ✅ **Alt-Hold for Menus** - Access UI in play mode without stopping gameplay
- ✅ **Hotbar System** - 10-slot quick access bar for items and tools (keys 1-0)
- ✅ **10x Mouse Sensitivity** - Much more responsive camera control
- ✅ **Editor Always On** - No need to toggle, always accessible
- ✅ **Free-Flying Camera** - Default mode for world building
- ✅ **Play/Stop Buttons** - Easy switching between editor and play modes
- ✅ **Crouch Edge Detection** - Safety feature prevents falling off blocks in play mode
- ✅ **6DOF Movement** - Full 3D movement in editor mode
- ✅ **Persistent UI** - Editor panels always available

## Future Features (Planned)

- [ ] Graphical main menu with full mouse support
- [ ] Inventory system with drag-and-drop to hotbar
- [ ] Block type selection with mouse wheel
- [ ] Hotbar item/tool linking system
- [ ] Context menus (right-click for options)
- [ ] Mouse-based selection and multi-block operations
- [ ] Save/load camera positions
- [ ] Bookmarks for quick navigation

---

**Last Updated:** January 2025  
**Version:** 0.4.0 (Full Mouse Support for In-Game Features)

## Summary of Mouse Support

✅ **Fully Supported:**
- Camera control (mouse look)
- Mouse mode toggle (F key for camera freelook ↔ GUI mode)
- Block placement and breaking (left/right click)
- Editor UI (all panels, buttons, sliders, lists)
- Temporary cursor access (hold Alt)

⚠️ **Keyboard Only:**
- Pre-game console menu (before graphics initialization)

**Note:** Once the game starts, all features provide comprehensive mouse and keyboard support.
