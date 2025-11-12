# Fresh Voxel Engine - Controls Guide

## Mouse Controls

### In-Game Mouse Controls
- **Mouse Movement** - Look around / Camera control (first-person view)
- **Left Mouse Button** - Break/remove blocks (aim at block and click)
- **Right Mouse Button** - Place blocks (aim at adjacent surface and click)
- **Mouse Wheel** - (Future: Block type selection)

### Editor Mouse Controls  
When the editor is open (press **T** to toggle):
- **Mouse** - Navigate and interact with all editor UI panels
- **Click** - Select tools, blocks, and options
- **Drag** - Move sliders and adjust values
- **Scroll** - Scroll through lists and options

**Note:** Mouse cursor is automatically shown when editor is active, and captured during gameplay.

## Keyboard Controls

### Movement Controls

- **W** - Move forward
- **S** - Move backward
- **A** - Strafe left
- **D** - Strafe right
- **Space** - Jump (when on ground)
- **Shift** - Sprint (move faster)
- **Ctrl** - Crouch (move slower, reduced height)

### Action Controls

- **T** - Toggle editor (shows/hides editor UI with mouse support)
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

## Editor Controls (Mouse & Keyboard)

### Opening the Editor
- Press **T** to toggle the editor on/off
- Mouse cursor automatically appears when editor is visible

### Editor Tools (All Mouse-Enabled)
- **Tool Palette** - Click to select terraforming tools (brush, sphere, cube, etc.)
- **Material Picker** - Click to choose block types (stone, dirt, grass, etc.)
- **Tool Size Slider** - Drag to adjust tool size (1-10)
- **Scene Hierarchy** - Click to navigate world structure
- **Content Browser** - Click to browse and manage assets
- **Inspector Panel** - Click to edit properties
- **Console Panel** - Click to execute commands

### Quick Actions
- **Hold Alt** - Temporarily show mouse cursor in-game without opening editor
- **T** - Toggle full editor interface

## Gameplay Tips

1. **Starting Position:** You spawn at coordinates (0, 80, 0) in the world
2. **Movement:** Use WASD for standard FPS-style movement
3. **Building:** Right-click places blocks, left-click removes them
4. **Exploration:** The world generates terrain as you move around
5. **Collision:** You cannot walk through solid blocks
6. **Physics:** Gravity is active - you'll fall if there's no ground beneath you
7. **Mouse Look:** Move mouse to look around freely in first-person view
8. **Editor Access:** Press T anytime to access advanced tools with full mouse support

## World Creation

When you start the engine:
1. The console menu appears (use number keys to navigate)
2. Choose "Create New World" (press 1) or "Load Existing World" (press 2)
3. Enter a world name using keyboard
4. Optionally enter a seed number (or leave blank for random)
5. The world will generate and you'll spawn with full mouse+keyboard controls

## Performance

- **Target FPS:** 60 FPS
- **Render Distance:** ~3-5 chunks in each direction
- **Chunk Size:** 16x256x16 voxels

## Input Modes

The engine automatically switches between input modes:

### Game Mode (Default)
- Mouse cursor is hidden and captured
- Mouse controls camera (look around)
- Left/Right click for block interaction
- WASD for movement

### UI Mode (Editor Open)
- Mouse cursor is visible and free
- Mouse interacts with UI elements
- Click buttons, drag sliders, select items
- Press T to return to Game Mode

### Temporary UI Mode
- Hold **Alt** to temporarily show cursor without opening full editor
- Useful for quick adjustments
- Release Alt to return to Game Mode

## Known Limitations

- Pre-game console menu is keyboard-only (appears before graphics initialization)
- No inventory system yet (can only place one block type)
- No in-game HUD overlay yet (use editor for advanced features)
- Mouse cursor capture may behave differently on different operating systems

## Future Features (Planned)

- [ ] Graphical main menu with full mouse support
- [ ] Block type selection with mouse wheel or hotbar
- [ ] In-game HUD with mouse-clickable elements
- [ ] Inventory system with drag-and-drop
- [ ] Context menus (right-click for options)
- [ ] Mouse-based selection and multi-block operations

---

**Last Updated:** January 2025  
**Version:** 0.4.0 (Full Mouse Support for In-Game Features)

## Summary of Mouse Support

✅ **Fully Supported:**
- Camera control (mouse look)
- Block placement and breaking (left/right click)
- Editor UI (all panels, buttons, sliders, lists)
- Temporary cursor access (hold Alt)

⚠️ **Keyboard Only:**
- Pre-game console menu (before graphics initialization)

**Note:** Once the game starts, all features provide comprehensive mouse and keyboard support.
