# Unreal Engine-Inspired Editor Quick Start

This guide answers common questions about using the Fresh Voxel Engine's Unreal Engine-inspired editor interface.

## Quick Navigation

1. [Customizing the Editor Layout](#customizing-the-editor-layout-example)
2. [Interactive Elements (Blueprint Alternative)](#interactive-elements-with-lua-scripting)
3. [Terrain Sculpting (Modeling Mode)](#terrain-sculpting-in-voxel-editing-mode)

---

## Customizing the Editor Layout (Example)

The Fresh Voxel Engine editor provides a flexible layout system similar to Unreal Engine's docking and panel management.

### Example: Creating a Custom Layout for Level Design

**Scenario**: You want a layout optimized for building and decorating levels with maximum viewport space.

**Step-by-Step:**

```
1. Start Fresh Editor
   - Launch FreshVoxelEngine.exe
   - Create or load a world

2. Reset to Default Layout (if needed)
   - Menu: Window → Reset Layout
   - This gives you a clean starting point

3. Customize Panel Visibility
   - Menu: Window → Scene Hierarchy (toggle off)
   - Menu: Window → Content Browser (toggle off)
   - Menu: Window → Console (toggle off)
   - Leave enabled:
     ✓ Inspector (for property editing)
     ✓ Voxel Tool Palette (for building)

4. Arrange Remaining Panels
   - Drag Inspector panel to the right edge
   - Dock it as a vertical panel
   - Drag Voxel Tool Palette to the left edge
   - Dock it as a vertical panel
   - Result: Large viewport in center, tools on sides

5. Resize Panels
   - Drag the panel dividers to adjust widths
   - Make Voxel Tool Palette narrower (just enough for tools)
   - Make Inspector wider (for reading properties)
   - Maximize viewport area

6. Save Your Custom Layout
   - Menu: Window → Layout → Save Layout
   - Layout is automatically saved
   - It will load next time you start the editor
```

**Visual Layout:**
```
┌─────────┬─────────────────────────────────┬─────────────┐
│  Voxel  │                                 │             │
│  Tool   │                                 │  Inspector  │
│ Palette │         Viewport                │   Panel     │
│         │    (3D Scene View)              │             │
│  [*][ ] │                                 │  Position:  │
│  [ ][*] │                                 │  X: 100     │
│  [ ][ ] │                                 │  Y: 64      │
│         │                                 │  Z: 100     │
│ Size: ▓ │                                 │             │
│ [===]   │                                 │  Material:  │
│         │                                 │  [Stone ▼]  │
│ Stone ▓ │                                 │             │
│ Dirt  □ │                                 │  Scale:     │
│ Grass □ │                                 │  1.0        │
└─────────┴─────────────────────────────────┴─────────────┘
```

### Predefined Layout Examples

**1. Default Layout - All Panels**
```
Use When: Starting out, need all tools visible
Panels: Scene Hierarchy, Inspector, Content Browser, Console, Tool Palette
Shortcut: Window → Layout → Default
```

**2. Minimal Layout - Essential Only**
```
Use When: Focused voxel editing, need maximum viewport
Panels: Inspector, Tool Palette only
Shortcut: Window → Layout → Minimal
```

**3. Debugging Layout - Console Focus**
```
Use When: Troubleshooting scripts, testing gameplay
Panels: Scene Hierarchy, Inspector, Console (large)
Shortcut: Window → Layout → Debugging
```

### Quick Layout Tips

**Docking Panels:**
- Drag panel by its title tab
- Blue highlight shows where it will dock
- Drop on edge: splits horizontally/vertically
- Drop on center: creates tabbed group
- Drag outside window: creates floating window

**Keyboard Shortcuts:**
```
F1    - Help documentation
F3    - Debug overlay (performance)
T     - Toggle Tool Palette
```

**Panel Visibility Toggle:**
```
Window menu lists all panels with checkmarks
Click to toggle any panel on/off
```

---

## Interactive Elements with Lua Scripting

The Fresh Voxel Engine uses **Lua scripting** as its equivalent to Unreal Engine's Blueprint visual scripting system.

### Why Lua Instead of Blueprints?

| Unreal Engine Blueprints | Fresh Engine Lua Scripts |
|-------------------------|--------------------------|
| Visual node-based | Text-based code |
| Compiled | Interpreted |
| Editor-based | Any text editor |
| Great for non-programmers | Easy to learn syntax |
| Version control difficult | Git-friendly |
| - | Hot reload supported |

### Example: Creating an Interactive Chest

**Step 1: Write the Lua Script**

Create file: `scripts/gameplay/treasure_chest.lua`

```lua
-- Treasure Chest - Interactive container
TreasureChest = {}

function TreasureChest:new(position, contents)
    local obj = {
        position = position,
        contents = contents or {},
        isOpen = false,
        interactionRadius = 3.0
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function TreasureChest:update(deltaTime)
    -- Get player position
    local playerPos = Engine.getPlayerPosition()
    local distance = self:getDistance(playerPos)
    
    -- Show prompt when player is close
    if distance < self.interactionRadius and not self.isOpen then
        Engine.showPrompt("Press E to open chest")
        
        if Engine.isKeyPressed("E") then
            self:open()
        end
    end
end

function TreasureChest:open()
    if self.isOpen then return end
    
    self.isOpen = true
    print("Opening treasure chest!")
    
    -- Play sound
    Engine.playSound("chest_open.wav", self.position)
    
    -- Give items to player
    for i, item in ipairs(self.contents) do
        Engine.givePlayerItem(item.type, item.amount)
        Engine.showMessage("Found: " .. item.type .. " x" .. item.amount)
    end
    
    -- Spawn particle effect
    Engine.spawnParticles("gold_sparkle", self.position, 30)
    
    -- Update chest appearance
    Engine.setBlockType(self.position, "chest_open")
end

function TreasureChest:getDistance(playerPos)
    local dx = self.position.x - playerPos.x
    local dy = self.position.y - playerPos.y
    local dz = self.position.z - playerPos.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

-- Create chest instance
myChest = TreasureChest:new(
    {x=150, y=64, z=150},
    {
        {type="gold_coins", amount=100},
        {type="health_potion", amount=3},
        {type="magic_sword", amount=1}
    }
)

-- Register for updates
Engine.registerUpdateCallback(function(dt)
    myChest:update(dt)
end)
```

**Step 2: Place in Editor**

```
1. Open Fresh Editor
2. Navigate to Content Browser
3. Browse to scripts/gameplay/
4. Drag treasure_chest.lua to viewport at desired location
5. A chest entity appears at that position

Alternative Method:
1. Menu: Entity → Create Empty Entity
2. Select the entity in Scene Hierarchy
3. In Inspector: Add Component → Script
4. Browse to treasure_chest.lua
5. Script attaches to entity
```

**Step 3: Configure in Inspector**

```
With chest entity selected:
┌────────────────────────┐
│  Inspector             │
├────────────────────────┤
│ Entity: Treasure Chest │
│                        │
│ Transform:             │
│  Position:             │
│   X: 150               │
│   Y: 64                │
│   Z: 150               │
│                        │
│ Components:            │
│  ☑ Script Component    │
│    File: treasure_ch...│
│    [Reload Script]     │
│                        │
│ Custom Properties:     │
│  Contents:             │
│   - gold_coins: 100    │
│   - health_potion: 3   │
│   - magic_sword: 1     │
│                        │
│  Interaction Radius: 3 │
└────────────────────────┘
```

**Step 4: Test**

```
1. Press F5 (Play Mode)
2. Walk to chest location
3. Press E when prompt appears
4. Chest opens and gives items
5. Check Console panel for debug messages
```

### Common Interactive Element Examples

**Auto-Opening Door:**
```lua
-- Opens when player approaches
if distance < 5.0 then
    Engine.animateRotation(doorPos, {x=0, y=90, z=0}, 1.0)
end
```

**Pressure Plate:**
```lua
-- Activates when player steps on it
if playerPos.y <= platePos.y + 0.1 then
    Engine.triggerEvent("plate_pressed")
end
```

**Moving Platform:**
```lua
-- Moves back and forth
local offset = math.sin(Engine.getTime()) * 5.0
Engine.setEntityPosition(platform, {x=startX + offset, y=startY, z=startZ})
```

**Item Spawner:**
```lua
-- Spawns items periodically
if Engine.getTime() - lastSpawn > spawnInterval then
    Engine.spawnItem("health_potion", spawnPos)
    lastSpawn = Engine.getTime()
end
```

### Debugging Scripts

**Console Output:**
```lua
print("Debug: Player distance = " .. distance)
-- Appears in Console panel
```

**Hot Reload:**
```
1. Edit script in text editor
2. Save file
3. In Inspector: click "Reload Script" button
4. Changes apply immediately (no restart needed)
```

**Error Checking:**
```lua
local success, err = pcall(function()
    -- Your code here
end)
if not success then
    print("ERROR: " .. err)
end
```

For complete Lua API reference: `docs/guides/LUA_INTEGRATION_GUIDE.md`

---

## Terrain Sculpting in Voxel Editing Mode

The Fresh Voxel Engine's **Voxel Tool Palette** is equivalent to Unreal Engine's Modeling Mode for terrain and geometry creation.

### Example: Sculpting a Mountain with Valley

This example demonstrates the complete workflow for creating natural terrain.

**Step 1: Open Voxel Tools**

```
1. Launch editor and load/create world
2. Menu: Window → Voxel Tool Palette (if hidden)
3. Or press T to toggle tool palette
4. Panel appears on left side
```

**Step 2: Create Base Mountain Shape**

```
Tool: Brush Tool
Settings:
  - Size: 10 (large brush)
  - Strength: 2.0 (strong effect)
  - Material: Stone

Actions:
1. Click "Brush" in Tool Palette
2. Adjust Size slider to 10
3. Adjust Strength slider to 2.0
4. Select Stone from material picker
5. Click and drag in viewport to build up terrain
6. Create rough cone/pyramid shape
7. Height: ~30 blocks above ground level
8. Base width: ~40 blocks diameter
```

**Visual Result After Step 2:**
```
    /\      <- Rough peak
   /  \
  /    \
 /      \
/        \
──────────  <- Ground level
```

**Step 3: Add Details and Variation**

```
Tool: Brush Tool (smaller)
Settings:
  - Size: 5 (medium brush)
  - Strength: 1.0 (moderate)
  - Material: Stone

Actions:
1. Reduce brush size to 5
2. Add bumps and ridges on mountain sides
3. Create uneven peak (multiple clicks)
4. Add rocky outcroppings
5. Make one side steeper than the other
```

**Step 4: Smooth the Mountain**

```
Tool: Smooth Tool
Settings:
  - Size: 4
  - Strength: 1.5

Actions:
1. Select "Smooth" from Tool Palette
2. Click and drag over harsh edges
3. Focus on slopes and transitions
4. Do 2-3 passes for natural look
5. Keep peak slightly rough (more realistic)
```

**Step 5: Create the Valley**

```
Tool: Sphere Tool (Hollow)
Settings:
  - Radius: 8
  - Material: Air (to remove blocks)

Actions:
1. Select "Sphere" tool (hollow)
2. Set radius to 8
3. Select "Air" as material
4. Click into mountain side at mid-height
5. This removes blocks, creating valley entrance
6. Place 2-3 overlapping spheres for depth

Result: U-shaped valley cutting through mountain
```

**Step 6: Flatten Valley Floor**

```
Tool: Flatten Tool
Settings:
  - Target Height: 68 (slightly above base)
  - Radius: 6

Actions:
1. Select "Flatten" tool
2. Set height to 68
3. Click in valley center
4. Creates flat valley floor
5. Good for placing structures or path
```

**Step 7: Smooth Valley Walls**

```
Tool: Smooth Tool
Settings:
  - Size: 3
  - Strength: 1.0

Actions:
1. Switch to Smooth tool
2. Drag along valley walls
3. Blend floor to walls smoothly
4. Soften any harsh edges
```

**Step 8: Add Material Variation**

```
Tool: Paint Tool
Settings:
  - Size: 6
  - Materials: Dirt, Grass, Snow

Actions:
1. Select "Paint" tool
2. Paint valley floor with Grass (green)
3. Paint lower mountain slopes with Dirt (brown)
4. Paint upper mountain with Stone (gray)
5. Paint peak with Snow (white) for realism
```

**Final Result:**
```
      *       <- Snow-capped peak (white)
     /█\
    /███\     <- Stone upper slopes (gray)
   /█████\
  /▓▓▓▓▓▓▓\   <- Dirt middle slopes (brown)
 /▓▓╔═══╗▓▓\  <- Valley entrance
/▓▓▓║░░░║▓▓▓\ <- Grass valley floor (green)
════╚═══╝════ <- Ground level
```

### Tool Quick Reference

**Brush Tool** - Organic sculpting
```
Best for: Hills, mountains, caves
Shortcut: B
Settings: Size (1-10), Strength (0.1-2.0)
Tip: Lower strength = more control
```

**Sphere Tool** - Round structures
```
Best for: Domes, boulders, excavations
Settings: Radius (1-20), Hollow/Filled
Tip: Use Air material to remove spherical volumes
```

**Cube Tool** - Rectangular structures
```
Best for: Buildings, walls, platforms
Settings: Width, Height, Depth
Tip: Enable grid snap for aligned structures
```

**Flatten Tool** - Level terrain
```
Best for: Building foundations, roads
Settings: Target height, Radius
Tip: Use before placing structures
```

**Smooth Tool** - Blend terrain
```
Best for: Natural transitions, refinement
Settings: Size, Strength (passes)
Tip: Multiple light passes better than one heavy pass
```

**Paint Tool** - Change materials
```
Best for: Texturing, biome transitions
Settings: Brush size, Material selection
Tip: Doesn't change geometry, only appearance
```

### Advanced Sculpting Examples

**Cave System:**
```
1. Place overlapping hollow spheres underground
2. Vary sphere sizes (3-10 blocks)
3. Use Smooth tool on cave walls
4. Paint interior with stone/dirt variation
```

**Cliff Face:**
```
1. Create wall with Cube tool (tall and thin)
2. Use Brush tool to remove random chunks
3. Lower strength for subtle variation
4. Leave some areas smooth, others rough
5. Paint with stone + moss for realism
```

**River Valley:**
```
1. Use Flatten at height 62 for water level
2. Create gently sloping sides with Smooth
3. Paint valley floor with sand
4. Place water blocks manually
5. Add grass on upper banks
```

**Tiered Gardens:**
```
1. Flatten at heights 64, 68, 72 (multiple levels)
2. Smooth edges between tiers
3. Paint with dirt/grass
4. Add stairs between levels (Cube tool)
```

### Keyboard Shortcuts

```
W    - Move tool (transform mode)
E    - Rotate tool (transform mode)
R    - Scale tool (transform mode)
B    - Place voxel tool
X    - Remove voxel tool
P    - Paint tool
T    - Toggle Tool Palette

Ctrl+Z - Undo (up to 100 steps)
Ctrl+Y - Redo
Ctrl+S - Save world

F3   - Performance overlay
F5   - Play Mode (test gameplay)
```

### Performance Tips

**For Large Terrain Edits:**
- Save before major changes
- Use Filled Cube for large solid areas (faster than brush)
- Lower brush strength for real-time sculpting
- Monitor FPS with F3 overlay
- Work in sections, save between

**Optimization:**
- Greedy meshing automatically reduces triangles
- Chunk system loads only visible terrain
- Undo history limited to 100 operations
- Complex terrain = more triangles = lower FPS

### Testing Your Terrain

**Walk Test:**
```
1. Press F5 (Play Mode)
2. Walk around terrain
3. Check for:
   - Walkable slopes (not too steep)
   - No floating blocks
   - No gaps in floor
   - Natural appearance
4. Press Stop to return to editor
```

**Scale Reference:**
```
- Player height: ~2 blocks
- Jump height: ~1.5 blocks
- Comfortable stairs: 1 block height
- Comfortable slopes: ~30-45 degrees
```

---

## Summary

This guide covered three key Unreal Engine-inspired features in Fresh Voxel Engine:

1. **Customizing Editor Layout**
   - Flexible panel docking system
   - Predefined layouts for different workflows
   - Save custom arrangements
   - Optimized for your role (designer, scripter, artist)

2. **Interactive Elements (Lua Scripting)**
   - Lua scripts = Unreal Blueprints alternative
   - Text-based, hot-reloadable
   - Attach to entities via Inspector
   - Full engine API access
   - Debugging via Console

3. **Terrain Sculpting (Voxel Editing Mode)**
   - Comprehensive tool palette
   - Brush, Sphere, Cube, Flatten, Smooth, Paint tools
   - Natural terrain creation workflow
   - Undo/redo support
   - Real-time feedback

### Next Steps

1. **Try the Layout Examples**: Set up a custom layout for your workflow
2. **Run a Lua Script**: Copy one of the example scripts and test it
3. **Build a Terrain**: Follow the mountain valley example
4. **Combine Techniques**: Create a complete scene with terrain and interactive elements

### Full Documentation

- Complete workflow guide: `docs/guides/UNREAL_ENGINE_INSPIRED_WORKFLOW.md`
- Lua scripting API: `docs/guides/LUA_INTEGRATION_GUIDE.md`
- Voxel tools reference: `docs/guides/VOXEL_TOOLS.md`
- Editor integration: `docs/editor/EDITOR_INTEGRATION.md`
- Layout management: `docs/editor/LAYOUT_MANAGEMENT.md`

---

*For questions or issues, see: `docs/community/CONTRIBUTING.md`*
