# Fresh Voxel Engine - Controls Guide

## Movement Controls

- **W** - Move forward
- **S** - Move backward
- **A** - Strafe left
- **D** - Strafe right
- **Space** - Jump (when on ground)
- **Shift** - Sprint (move faster)
- **Ctrl** - Crouch (move slower, reduced height)
- **Mouse** - Look around (camera control)

## Block Interaction

- **Left Click** - Break/remove block (aim at block and click)
- **Right Click** - Place block (aim at adjacent surface and click)

**Note:** Currently places Stone blocks by default.

## Menu Controls

- **ESC** - Exit game / Close application
- **T** - Toggle editor (if ImGui is available)

## Gameplay Tips

1. **Starting Position:** You spawn at coordinates (0, 80, 0) in the world
2. **Movement:** Use WASD for standard FPS-style movement
3. **Building:** Right-click places blocks, left-click removes them
4. **Exploration:** The world generates terrain as you move around
5. **Collision:** You cannot walk through solid blocks
6. **Physics:** Gravity is active - you'll fall if there's no ground beneath you

## World Creation

When you start the engine:
1. Choose "Create New World" or "Load Existing World" from the main menu
2. Enter a world name (or use default "New World")
3. Optionally enter a seed number (or use random)
4. The world will generate with procedural terrain

## Performance

- **Target FPS:** 60 FPS
- **Render Distance:** ~3-5 chunks in each direction
- **Chunk Size:** 16x256x16 voxels

## Known Limitations

- No inventory system yet (can only place one block type)
- No texture variety (all blocks use same color with lighting)
- Limited UI (console-based menu, no in-game HUD yet)
- Mouse cursor is captured while in-game

## Future Features (Planned)

- [ ] Block type selection (1-9 number keys)
- [ ] In-game HUD and crosshair
- [ ] Block textures
- [ ] Inventory system
- [ ] Sound effects
- [ ] Multiplayer support

---

**Last Updated:** November 2025  
**Version:** 0.3.0 (Playable Demo)
