# Fresh Voxel Engine - Gameplay Scenarios Guide

This guide provides detailed gameplay scenarios you can test immediately in the Fresh Voxel Engine. All scenarios are fully implemented and playable through the main `FreshVoxelEngine.exe` executable.

## 🎮 Quick Start

Launch the engine:
```bash
# From build directory
.\Release\FreshVoxelEngine.exe

# Or from project root after building
.\build\Release\FreshVoxelEngine.exe
```

## 📋 Gameplay Scenario List

### Scenario 1: First World Creation
**Goal:** Create your first procedurally generated voxel world

**Steps:**
1. Launch FreshVoxelEngine.exe
2. At main menu, click "Create New World"
3. Enter world name: `MyFirstWorld`
4. Enter seed: `12345` (or any number for randomness)
5. Wait for initial chunk generation (~49 chunks)
6. Spawn at coordinates (0, 80, 0) in the sky
7. Fall and land on terrain below

**Expected Results:**
- Smooth terrain generation with hills and valleys
- Different biomes visible (plains, forests, mountains)
- Natural features (trees, rocks) placed automatically
- Player controller active with physics
- Sky rendering with atmospheric effects

**Controls:**
- WASD - Move
- Mouse - Look around
- Space - Jump
- ESC - Return to menu

---

### Scenario 2: World Exploration
**Goal:** Explore diverse terrain and biomes

**Steps:**
1. Load an existing world or create new with seed `424242`
2. Walk in any direction for 5+ chunks
3. Observe automatic chunk loading ahead of you
4. Observe chunk unloading behind you
5. Climb a mountain and look at view distance
6. Enter a cave system if you find one

**Expected Results:**
- New chunks generate seamlessly as you move
- Distant chunks fade in/out smoothly
- FPS remains stable (60+ on modern hardware)
- View distance ~8 chunks in all directions
- Memory usage stays reasonable (~200-500 MB)

**Performance Tips:**
- Press F3 to show debug overlay
- Monitor FPS, chunk count, draw calls
- Adjust render distance if needed

**Biomes to Find:**
- Plains (flat grass with scattered trees)
- Forests (dense tree coverage)
- Mountains (high elevation, rocky)
- Deserts (sand, cacti, warm colors)
- Ocean/Water bodies

---

### Scenario 3: Block Interaction & Building
**Goal:** Break and place blocks to build structures

**Steps:**
1. Load any world
2. Look at a grass block
3. Left-click to break it (adds to inventory)
4. Look at ground where you removed block
5. Right-click to place a block
6. Press number keys 1-9 to change block type
7. Build a simple house (4 walls + roof)

**Expected Results:**
- Blocks break instantly with left-click
- Block particles spawn when breaking (visual feedback)
- Blocks place instantly with right-click
- Mesh updates in real-time (no lag)
- Different block types have different textures

**Block Types (1-9 keys):**
1. Stone - Gray, solid
2. Dirt - Brown, earthy
3. Grass - Green top, dirt sides
4. Sand - Yellow/tan, granular
5. Water - Blue, transparent
6. Wood - Brown, logs
7. Leaves - Green, organic
8. Glass - Light blue, transparent
9. Ore - Gray with colored veins

**Building Ideas:**
- Simple house (5x5x3 blocks)
- Tower (climb up as you build)
- Bridge over water/ravine
- Underground bunker
- Pixel art on ground

---

### Scenario 4: Advanced Terraforming
**Goal:** Use editor tools to sculpt terrain

**Steps:**
1. Load any world
2. Press T to open editor interface
3. Select "Brush Tool" from toolbar
4. Click and drag on terrain to paint
5. Try different brush sizes (slider)
6. Switch to "Sphere Tool"
7. Create a large spherical dome
8. Press Ctrl+Z to undo
9. Press Ctrl+Y to redo

**Expected Results:**
- Editor UI overlays on screen
- Brush paints selected block type
- Sphere creates perfect spheres
- Undo/redo works for up to 100 operations
- Terrain updates smoothly

**Editor Tools:**

**Brush Tool:**
- Paint individual blocks
- Adjustable size (1-10 blocks)
- Hold mouse button to paint continuously

**Sphere Tool:**
- Create spherical structures
- Radius: 1-20 blocks
- Hollow or filled option

**Cube Tool:**
- Create rectangular structures
- Width, height, depth sliders
- Perfect for buildings

**Flatten Tool:**
- Level terrain to uniform height
- Choose target height
- Smooth edges option

**Smooth Tool:**
- Smooth rough terrain
- Reduce sharp edges
- Multiple passes for best results

**Paint Tool:**
- Change block types without destroying
- Keep terrain shape, change material
- Useful for texturing

---

### Scenario 5: World Saving & Loading
**Goal:** Save your changes and load them later

**Steps:**
1. Create new world: `TestWorld`
2. Make significant changes (build something, terraform)
3. Press Ctrl+S to manually save (or exit to auto-save)
4. Exit to main menu
5. Click "Load Existing World"
6. Select `TestWorld` from list
7. Observe all changes are preserved

**Expected Results:**
- Save completes in < 1 second
- File saved to `saves/TestWorld.world`
- Load time < 2 seconds for small worlds
- All block changes preserved exactly
- Player position saved
- World seed preserved for infinite generation

**Save System Features:**
- Automatic save on exit
- Manual save with Ctrl+S
- Compressed chunk data
- Incremental saves (only changed chunks)
- Backup system (previous save preserved)

---

### Scenario 6: Performance Testing
**Goal:** Test engine performance under load

**Steps:**
1. Create world with seed `0`
2. Press F3 to show performance overlay
3. Sprint forward rapidly (Shift + W)
4. Watch chunk generation performance
5. Build large structures quickly
6. Monitor FPS and memory usage
7. Adjust render distance up/down

**Expected Results:**
- 60+ FPS on modern hardware
- Chunk generation ~50-100ms per chunk
- Memory usage grows with render distance
- Frame time stable (16.67ms @ 60 FPS)
- No stuttering during chunk loads

**Performance Metrics:**
- FPS (frames per second)
- Frame time (milliseconds)
- Chunks loaded/visible
- Draw calls per frame
- Triangle count
- Memory usage (MB)

**Optimization Tips:**
- Reduce render distance for better FPS
- Enable frustum culling (automatic)
- Greedy meshing reduces triangles by 90%
- Chunk streaming happens on background thread

---

### Scenario 7: Cave Exploration
**Goal:** Find and explore underground cave systems

**Steps:**
1. Create world with seed `99999` (good caves)
2. Explore surface to find cave entrance
3. Or dig straight down (not recommended in real gameplay!)
4. Navigate through cave system
5. Observe procedural cave generation
6. Find underground lakes or lava

**Expected Results:**
- Natural cave formations
- Multiple connected chambers
- Varying heights and widths
- Underground water/lava pools
- Atmospheric lighting (if implemented)
- Echo effect in large chambers (if audio implemented)

**Cave Features:**
- Winding passages
- Large caverns
- Small tunnels
- Vertical shafts
- Underground rivers
- Dead ends vs connected systems

---

### Scenario 8: Physics Testing
**Goal:** Test physics system accuracy

**Steps:**
1. Create any world
2. Jump off a cliff or tall structure
3. Observe realistic falling
4. Land and observe impact
5. Try jumping while moving forward
6. Test collision with walls
7. Walk on slopes
8. Jump repeatedly to test consistency

**Expected Results:**
- Gravity: -9.8 m/s² (realistic)
- Jump height: ~1.5 blocks
- No clipping through blocks
- Smooth movement on slopes
- Consistent physics timing
- No floating or weird behavior

**Physics Features:**
- Accurate gravity simulation
- AABB collision detection
- Slope handling
- Jump mechanics
- Sprint speed: 2x normal
- Crouch speed: 0.5x normal

---

### Scenario 9: Multi-Biome World Tour
**Goal:** Explore all available biomes in one world

**Steps:**
1. Create world with seed `0` (balanced biomes)
2. Note your starting biome
3. Walk in each cardinal direction
4. Document biomes you encounter
5. Take mental screenshots of transitions
6. Return to spawn to test navigation

**Biomes Available:**
- **Plains**: Flat grass, scattered trees, rivers
- **Forest**: Dense trees, undergrowth, dark
- **Mountains**: High peaks, steep slopes, snow-capped
- **Desert**: Sand dunes, cacti, hot appearance
- **Ocean**: Large water bodies, beaches

**Biome Transitions:**
- Gradual height changes
- Vegetation blending
- Color palette shifts
- Natural boundaries (rivers, cliffs)

---

### Scenario 10: Creative Building Challenge
**Goal:** Build something impressive with editor tools

**Challenge Ideas:**

**Beginner:**
- Simple house (5×5×3)
- Dirt bridge across water
- Small tower (climb while building)

**Intermediate:**
- Castle with walls and towers
- Underground base with rooms
- Pixel art on ground (use glass blocks)
- Floating island (terraform + build)

**Advanced:**
- Entire village with multiple buildings
- Giant statue or monument
- Roller coaster track (visual only)
- Maze with dead ends
- Replica of famous building

**Tips:**
- Use editor for large-scale terrain work
- Use manual building for details
- Save frequently (Ctrl+S)
- Use undo/redo liberally
- Plan before building
- Reference images help

---

## 🎯 Gameplay Achievements

Try to complete these achievements:

### Explorer
- ✅ Create your first world
- ✅ Walk 1000 blocks from spawn
- ✅ Find all 5 biome types
- ✅ Discover a cave system
- ✅ Reach height 128 (mountain top)

### Builder
- ✅ Place 100 blocks
- ✅ Break 100 blocks
- ✅ Build a house
- ✅ Build a tower 20 blocks tall
- ✅ Use all 9 block types

### Engineer
- ✅ Open the editor (Press T)
- ✅ Use all 6 editor tools
- ✅ Terraform an area 50×50 blocks
- ✅ Use undo/redo successfully
- ✅ Save and load a world

### Performance Enthusiast
- ✅ Achieve 60 FPS
- ✅ Load 100+ chunks
- ✅ Generate 200+ chunks
- ✅ Monitor performance with F3
- ✅ Optimize settings for your hardware

---

## 🛠️ Technical Details

### World Generation Parameters

**Seed-based Generation:**
- Seed determines all terrain
- Same seed = same world
- 32-bit signed integer range
- Default seed: 0

**Chunk System:**
- Chunk size: 16×256×16 blocks
- Total voxels per chunk: 65,536
- View distance: 8 chunks (configurable)
- Active chunks: ~201 (circular pattern)

**Generation Speed:**
- New chunk: 50-150ms
- Mesh generation: 20-50ms
- Total: ~100-200ms per chunk
- Background threaded (non-blocking)

### Performance Targets

**Minimum Specs:**
- CPU: Intel i5 / AMD Ryzen 5
- GPU: GTX 1060 / RX 580
- RAM: 8 GB
- Target: 30 FPS @ 1080p

**Recommended Specs:**
- CPU: Intel i7 / AMD Ryzen 7
- GPU: RTX 2060 / RX 5700
- RAM: 16 GB
- Target: 60 FPS @ 1080p

**High-End Specs:**
- CPU: Intel i9 / AMD Ryzen 9
- GPU: RTX 3080 / RX 6800 XT
- RAM: 32 GB
- Target: 120+ FPS @ 1440p

---

## 🐛 Troubleshooting

### Low FPS
- Reduce render distance
- Close other applications
- Update graphics drivers
- Check CPU/GPU usage

### Crashes
- Check logs in `logs/` directory
- Verify dependencies installed
- Try reducing view distance
- Report with error log

### Missing Features
- Verify you're running latest build
- Check feature availability in README
- Some features may be optional (ImGui, Lua)

### Build Issues
- Run `python tools/build_verifier.py`
- See [BUILD.md](../BUILD.md)
- Check vcpkg installation

---

## 📚 Additional Resources

- **Examples Documentation**: [examples/README.md](README.md)
- **Architecture Guide**: [../ARCHITECTURE.md](../ARCHITECTURE.md)
- **Build Instructions**: [../BUILD.md](../BUILD.md)
- **API Documentation**: [../docs/api/README.md](../docs/api/README.md)
- **.NET Development**: [../DOTNET_INTEGRATION.md](../DOTNET_INTEGRATION.md)
- **Lua Scripting**: [../LUA_INTEGRATION_GUIDE.md](../LUA_INTEGRATION_GUIDE.md)
- **Python Tools**: [../tools/README.md](../tools/README.md)

---

**Ready to play?** Choose a scenario and start exploring Fresh Voxel Engine!

**Want more challenges?** Try combining scenarios or creating your own!
