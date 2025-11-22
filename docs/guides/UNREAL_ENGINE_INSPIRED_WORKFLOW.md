# Unreal Engine-Inspired Editor Workflow Guide

This guide explains how to use the Fresh Voxel Engine's Unreal Engine-inspired editor interface for intuitive and efficient game development. The editor provides a modular, customizable layout with several core panels that streamline workflows similar to Unreal Engine.

## Table of Contents

1. [Core Interface Components](#core-interface-components)
2. [Customizing the Editor Layout](#customizing-the-editor-layout)
3. [Interactive Elements with Lua Scripting](#interactive-elements-with-lua-scripting)
4. [Terrain Sculpting and Voxel Editing](#terrain-sculpting-and-voxel-editing)

---

## Core Interface Components

The Fresh Voxel Engine editor is designed with an Unreal Engine-like interface, featuring familiar panels and tools for game developers.

### Menu Bar
Located at the top, this provides access to common application functions:
- **File**: Save, New World, Open World, Import/Export
- **Edit**: Undo, Redo, Cut, Copy, Paste, Delete, Preferences
- **Window**: Open specific editor windows and manage layouts
- **Build**: Build World, Generate Lightmaps, Optimize
- **Settings**: Graphics API selection, Quality settings, Input/Audio configuration
- **Help**: Documentation, API Reference, Tutorials, Bug Reports

### Toolbar
Positioned below the Menu Bar with essential shortcuts:
- **Transform Tools**: Select, Move (W), Rotate (E), Scale (R)
- **Voxel Tools**: Place (B), Remove (X), Paint (P)
- **Play Controls**: Play, Pause, Stop buttons for testing gameplay in-editor
- **Save** and **Content** shortcuts

### Viewport
The central and largest area showing a real-time, interactive 3D view of your voxel world:
- **Placement**: Directly manipulate objects (voxels, NPCs, entities)
- **Navigation**: 
  - Hold Right Mouse Button (RMB) to look around
  - WASD for movement in editor mode (free-flying camera)
  - Mouse wheel for zoom
- **Transform Tools**: Visual gizmos for Move (W), Rotate (E), Scale (R)
- **Snapping**: Grid snapping for precise placement
- **View Modes**: Toggle between different rendering views (press F3 for debug overlay)

### Scene Hierarchy / Outliner
Located in the upper-right area (or customizable position):
- Hierarchical list of all Actors/Entities in the level
- World → Chunks → Entities → Lights structure
- Easy selection, hiding/unhiding, and grouping of objects
- Search and filter functionality
- Context menu with Rename, Duplicate, Delete options
- Organizes objects into folders for complex scenes

### Inspector / Details Panel
Found below the Outliner (default position):
- Context-sensitive panel displaying properties of selected objects
- **Transform Properties**: Position, Rotation, Scale with drag controls
- **Component Properties**: Physics, Renderer, Audio Source, Scripts
- **Material Properties**: Shader selection, color picker, texture assignment
- Collapsible categories (Rendering, Physics, Lighting) for easy navigation

### Content Browser
Central repository for all project assets:
- Navigate asset folders for meshes, textures, materials, audio files, scripts
- **Drag-and-Drop**: Assets can be dragged directly into the Viewport
- Search and filter tools to quickly find specific assets
- Asset preview with icons (textures 🖼️, models 📦, sounds 🔊, scripts 📄)
- Operations: Import, Open, Delete, Rename

### Voxel Tool Palette
Located on the left side (customizable):
- Specialized tools for voxel editing:
  - **Single Block**: Precise single-voxel placement
  - **Brush**: Natural terrain sculpting with spherical brush
  - **Sphere**: Create hollow/filled spherical structures
  - **Cube**: Create rectangular structures
  - **Flatten**: Level terrain to specific height
  - **Smooth**: Smooth terrain by averaging heights
  - **Paint**: Change block types without geometry changes

### Console / Status Bar
Bottom panel for logging and commands:
- Filter messages by type (Info, Warning, Error, Success)
- Timestamped, color-coded messages
- Command input for console commands
- Auto-scroll and clear history options

---

## Customizing the Editor Layout

The Fresh Voxel Engine editor emphasizes customization, allowing you to tailor the workspace to your specific workflow needs.

### Example 1: Switching to Predefined Layouts

The editor comes with three predefined layouts optimized for different tasks:

**Default Layout** - All panels visible
```
1. Open the Window menu
2. Navigate to Layout → Default
3. All panels appear in their standard positions
```

**Minimal Layout** - Essential editing only
```
1. Open the Window menu
2. Navigate to Layout → Minimal
3. Only Inspector and Tool Palette remain visible
4. Perfect for focused voxel editing work
```

**Debugging Layout** - Console-focused
```
1. Open the Window menu
2. Navigate to Layout → Debugging
3. Scene Hierarchy, Inspector, and Console panels visible
4. Ideal for troubleshooting and testing
```

### Example 2: Creating a Custom Layout

You can arrange panels to suit your personal workflow:

```
Step 1: Show/Hide Panels
- Open Window menu
- Toggle individual panels on/off:
  ✓ Scene Hierarchy
  ✓ Inspector
  ✗ Content Browser (hide for more viewport space)
  ✓ Console
  ✓ Tool Palette

Step 2: Arrange Panel Positions
- Drag panel tabs to dock them in different positions
- Split panels horizontally or vertically
- Create tabbed panel groups
- Undock panels into floating windows

Step 3: Save Your Layout
- Window → Layout → Save Layout
- Your configuration is saved automatically
- Layout persists across editor sessions
```

### Example 3: Layout for Different Roles

**For Level Designers:**
```
Optimal Layout:
- Large Viewport (center)
- Tool Palette (left, docked)
- Inspector (right, docked)
- Scene Hierarchy (right, tabbed with Inspector)
- Console (bottom, minimized)

Quick Access:
- Voxel tools always visible
- Quick property editing
- Minimal UI clutter
```

**For Scripters/Programmers:**
```
Optimal Layout:
- Viewport (top center)
- Console (bottom, large)
- Content Browser (left)
- Inspector (right)
- Code editor external

Focus:
- Console for debugging output
- Content Browser for script files
- Inspector for component properties
```

**For World Artists:**
```
Optimal Layout:
- Maximum Viewport size
- Minimal Tool Palette (left, collapsed)
- Small Inspector (right)
- All other panels hidden

Focus:
- Maximum visual feedback
- Quick tool access
- Minimal distractions
```

### Keyboard Shortcuts for Layout

```
Ctrl+N        - New World
Ctrl+O        - Open World
Ctrl+S        - Save World
Ctrl+Shift+S  - Save World As

W             - Move tool
E             - Rotate tool
R             - Scale tool
B             - Place voxel tool
X             - Remove voxel tool
P             - Paint tool

F1            - Documentation (opens this guide)
F3            - Debug overlay (FPS, chunks, performance)
F5            - Build and Run (test gameplay)
```

### Panel Docking System

The editor uses a flexible docking system similar to Unreal Engine:

1. **Drag Tabs**: Click and drag panel tabs to move them
2. **Dock Indicators**: Blue highlight shows where panel will dock
3. **Floating Windows**: Drag panel outside main window to float
4. **Tab Groups**: Drop panel onto existing panel to create tabs
5. **Split Views**: Drop panel on edge to split horizontally/vertically

### Resetting Layout

If your layout becomes problematic:
```
Window → Reset Layout
```
This restores the Default layout configuration.

### Layout Persistence

- Layouts are automatically saved to `configs/layouts.ini`
- Last used layout is restored on editor startup
- Custom layouts persist across application restarts
- Manual save preserves current configuration

---

## Interactive Elements with Lua Scripting

The Fresh Voxel Engine uses **Lua scripting** as an alternative to Unreal Engine's Blueprints system for creating interactive gameplay elements without C++ coding.

### Why Lua Instead of Blueprints?

While Unreal Engine uses visual Blueprints, Fresh Engine uses Lua scripting which provides:
- **Text-based scripting**: More version-control friendly
- **Hot reload**: Changes apply without recompiling
- **Lightweight**: Easy to learn and modify
- **Powerful**: Full access to engine systems
- **Modding-friendly**: Users can create mods easily

### Setting Up Lua Scripting

**Step 1: Enable Lua Support**

Check if Lua is integrated (see `docs/guides/LUA_INTEGRATION_GUIDE.md`):
```bash
# Lua should be installed via vcpkg
vcpkg list | grep sol2
```

**Step 2: Create Script Files**

Scripts are stored in the `scripts/` directory:
```
scripts/
├── gameplay/
│   ├── player_interactions.lua
│   ├── npc_behaviors.lua
│   └── item_pickups.lua
├── world/
│   ├── door_controller.lua
│   ├── trap_trigger.lua
│   └── weather_system.lua
└── utils/
    └── math_helpers.lua
```

### Example 1: Interactive Door

Create a script for a door that opens when the player approaches:

**File**: `scripts/gameplay/door_controller.lua`
```lua
-- Door Controller Script
-- Opens/closes based on player proximity

Door = {}

-- Initialize door properties
function Door:new(position, rotation)
    local obj = {
        position = position,
        rotation = rotation,
        isOpen = false,
        openDistance = 5.0,  -- Distance in blocks to trigger
        animationSpeed = 2.0
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

-- Called every frame
function Door:update(deltaTime)
    -- Get player position from engine
    local playerPos = Engine.getPlayerPosition()
    
    -- Calculate distance to player
    local distance = self:calculateDistance(playerPos, self.position)
    
    -- Open or close based on distance
    if distance < self.openDistance and not self.isOpen then
        self:open()
    elseif distance >= self.openDistance and self.isOpen then
        self:close()
    end
end

-- Open the door
function Door:open()
    self.isOpen = true
    print("Door opening at position: " .. tostring(self.position))
    
    -- Animate door opening
    Engine.animateRotation(self.position, {x=0, y=90, z=0}, self.animationSpeed)
    
    -- Play sound effect
    Engine.playSound("door_open.wav", self.position)
end

-- Close the door
function Door:close()
    self.isOpen = false
    print("Door closing at position: " .. tostring(self.position))
    
    -- Animate door closing
    Engine.animateRotation(self.position, {x=0, y=0, z=0}, self.animationSpeed)
    
    -- Play sound effect
    Engine.playSound("door_close.wav", self.position)
end

-- Helper function to calculate distance
function Door:calculateDistance(pos1, pos2)
    local dx = pos1.x - pos2.x
    local dy = pos1.y - pos2.y
    local dz = pos1.z - pos2.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

-- Create door instance
myDoor = Door:new({x=100, y=64, z=100}, {x=0, y=0, z=0})

-- Register update callback
Engine.registerUpdateCallback(function(deltaTime)
    myDoor:update(deltaTime)
end)
```

**Placing the Door in Editor:**
1. Open Content Browser
2. Navigate to `scripts/gameplay/`
3. Drag `door_controller.lua` into the Viewport at desired location
4. Select the door entity in Scene Hierarchy
5. In Inspector panel, verify script is attached
6. Test with Play button (F5)

### Example 2: Item Pickup System

Create collectible items that respond to player interaction:

**File**: `scripts/gameplay/item_pickup.lua`
```lua
-- Item Pickup System
-- Handles collectible items in the world

ItemPickup = {}

function ItemPickup:new(position, itemType, value)
    local obj = {
        position = position,
        itemType = itemType,  -- "health", "ammo", "key", etc.
        value = value,         -- Amount to give
        pickupRadius = 2.0,
        isCollected = false,
        bobSpeed = 2.0,
        bobHeight = 0.3
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function ItemPickup:update(deltaTime)
    if self.isCollected then
        return
    end
    
    -- Bob up and down for visual effect
    local bobOffset = math.sin(Engine.getTime() * self.bobSpeed) * self.bobHeight
    Engine.setEntityOffset(self.position, {x=0, y=bobOffset, z=0})
    
    -- Rotate slowly
    local rotation = Engine.getTime() * 50  -- Degrees per second
    Engine.setEntityRotation(self.position, {x=0, y=rotation, z=0})
    
    -- Check for player collision
    local playerPos = Engine.getPlayerPosition()
    local distance = self:calculateDistance(playerPos, self.position)
    
    if distance < self.pickupRadius then
        self:collect()
    end
end

function ItemPickup:collect()
    self.isCollected = true
    
    -- Give item to player
    Engine.givePlayerItem(self.itemType, self.value)
    
    -- Play pickup sound
    Engine.playSound("item_pickup.wav", self.position)
    
    -- Show pickup message
    Engine.showMessage("Collected " .. self.itemType .. " +" .. self.value)
    
    -- Spawn particle effect
    Engine.spawnParticles("sparkle", self.position, 20)
    
    -- Remove from world
    Engine.removeEntity(self.position)
    
    print("Item collected: " .. self.itemType .. " at " .. tostring(self.position))
end

function ItemPickup:calculateDistance(pos1, pos2)
    local dx = pos1.x - pos2.x
    local dy = pos1.y - pos2.y
    local dz = pos1.z - pos2.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

-- Create pickup instances
healthPack = ItemPickup:new({x=120, y=65, z=130}, "health", 25)
ammoBox = ItemPickup:new({x=125, y=65, z=135}, "ammo", 50)

-- Register updates
Engine.registerUpdateCallback(function(deltaTime)
    healthPack:update(deltaTime)
    ammoBox:update(deltaTime)
end)
```

### Example 3: NPC Behavior Script

Control NPC interactions and behaviors:

**File**: `scripts/gameplay/npc_trader.lua`
```lua
-- Trader NPC Behavior
-- Allows player to buy/sell items

TraderNPC = {}

function TraderNPC:new(position, name, inventory)
    local obj = {
        position = position,
        name = name,
        inventory = inventory or {},
        interactionRadius = 3.0,
        isTrading = false
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function TraderNPC:update(deltaTime)
    -- Check player distance
    local playerPos = Engine.getPlayerPosition()
    local distance = self:calculateDistance(playerPos, self.position)
    
    -- Show interaction prompt when close
    if distance < self.interactionRadius and not self.isTrading then
        Engine.showPrompt("Press E to trade with " .. self.name)
        
        -- Check for interaction key
        if Engine.isKeyPressed("E") then
            self:startTrade()
        end
    end
end

function TraderNPC:startTrade()
    self.isTrading = true
    print("Trading with " .. self.name)
    
    -- Open trade UI
    Engine.openTradeUI(self.inventory)
    
    -- NPC dialogue
    Engine.showDialogue(self.name, "Welcome! Take a look at my wares.")
end

function TraderNPC:endTrade()
    self.isTrading = false
    Engine.closeTradeUI()
    Engine.showDialogue(self.name, "Safe travels!")
end

function TraderNPC:calculateDistance(pos1, pos2)
    local dx = pos1.x - pos2.x
    local dy = pos1.y - pos2.y
    local dz = pos1.z - pos2.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

-- Create trader NPC
trader = TraderNPC:new(
    {x=100, y=64, z=100},
    "Merchant Marcus",
    {
        {item="Health Potion", price=50, quantity=10},
        {item="Iron Sword", price=200, quantity=1},
        {item="Magic Staff", price=500, quantity=1}
    }
)

-- Register update
Engine.registerUpdateCallback(function(deltaTime)
    trader:update(deltaTime)
end)
```

### Attaching Scripts to Entities in Editor

1. **Select Entity**: Click entity in Viewport or Scene Hierarchy
2. **Inspector Panel**: View entity properties
3. **Add Script Component**: 
   - Click "Add Component" button
   - Select "Script" from dropdown
   - Browse to script file
4. **Configure Parameters**: Set script-specific values in Inspector
5. **Test**: Press F5 to test gameplay with script active

### Debugging Lua Scripts

Use the Console panel for debugging:
```lua
-- Print statements appear in console
print("Debug: Player position is " .. tostring(playerPos))

-- Error handling
local success, error = pcall(function()
    -- Your code here
end)
if not success then
    print("Error: " .. error)
end

-- Inspect variables
Engine.debugVariable("myVar", myVariable)
```

### Hot Reloading Scripts

Scripts can be modified and reloaded without restarting:
1. Edit `.lua` file in your text editor
2. Save the file
3. In editor, select entity with script
4. Inspector panel shows "Reload Script" button
5. Click to apply changes immediately

For more details, see: `docs/guides/LUA_INTEGRATION_GUIDE.md`

---

## Terrain Sculpting and Voxel Editing

The Fresh Voxel Engine's **Voxel Tool Palette** serves as the equivalent to Unreal Engine's Modeling Mode, providing comprehensive tools for sculpting terrain and building structures.

### Accessing Voxel Editing Tools

**Open Tool Palette:**
1. Window menu → Voxel Tool Palette (if hidden)
2. Press `T` to toggle tool palette visibility
3. Panel appears on left side (default position)

**Tool Palette Layout:**
```
┌─────────────────────────┐
│  Voxel Tool Palette     │
├─────────────────────────┤
│  [Selection Mode]       │  ← Current mode
│                         │
│  Basic Tools:           │
│  ● Single Block         │
│  ● Brush                │
│  ● Sphere / Filled      │
│  ● Cube / Filled        │
│                         │
│  Terrain Tools:         │
│  ● Flatten              │
│  ● Smooth               │
│  ● Paint                │
│                         │
│  Settings:              │
│  Size:     [====|   ]   │
│  Strength: [===|    ]   │
│                         │
│  Material Picker:       │
│  [Stone][Dirt][Grass]   │
│  [Sand][Water][Wood]    │
│  [...]                  │
│                         │
│  [Undo] [Redo]          │
└─────────────────────────┘
```

### Tool Reference Guide

#### 1. Single Block Tool
**Purpose**: Precise single-voxel placement
**Best for**: Detailed work, corrections, pixel art

```
Usage:
1. Select "Single Block" tool
2. Choose material from picker
3. Left-click in Viewport to place single voxel
4. Right-click to remove single voxel

Keyboard Shortcut: B
```

#### 2. Brush Tool
**Purpose**: Natural terrain sculpting with spherical brush
**Best for**: Organic terrain, hills, valleys, caves

```
Usage:
1. Select "Brush" tool
2. Adjust Size slider (1-10 blocks)
3. Adjust Strength slider (0.1-2.0)
4. Choose material
5. Click and drag in Viewport to sculpt
6. Hold Shift while dragging to smooth instead

Settings:
- Size: Brush radius (larger = broader strokes)
- Strength: How much material is added/removed
- Noise: Add randomness for organic feel

Tips:
- Lower strength for gradual sculpting
- Higher strength for quick rough work
- Use with Smooth tool for best results
```

#### 3. Sphere Tool
**Purpose**: Create spherical structures (hollow or filled)
**Best for**: Domes, planets, circular structures, excavations

```
Usage:
1. Select "Sphere" or "Filled Sphere"
2. Set radius (1-20 blocks)
3. Choose material
4. Click in Viewport to place sphere at that point

Hollow vs Filled:
- Hollow: Creates shell (good for domes, igloos)
- Filled: Creates solid sphere (good for hills, boulders)

Example Uses:
- Dome buildings
- Planetary terrain
- Cave systems (hollow, air material)
- Boulder formations
```

#### 4. Cube Tool
**Purpose**: Create rectangular structures
**Best for**: Buildings, walls, platforms, foundations

```
Usage:
1. Select "Cube" or "Filled Cube"
2. Set dimensions:
   - Width: X-axis size
   - Height: Y-axis size
   - Depth: Z-axis size
3. Choose material
4. Click to place, click again to set second corner

Snapping:
- Enable grid snap for aligned structures
- Hold Ctrl for freeform placement

Example Uses:
- Building foundations (5×1×5)
- Walls (10×5×1)
- Rooms (8×3×8)
- Platforms
```

#### 5. Flatten Tool
**Purpose**: Level terrain to uniform height
**Best for**: Building foundations, roads, plateaus

```
Usage:
1. Select "Flatten" tool
2. Set target height (Y coordinate)
3. Set radius (area of effect)
4. Click in Viewport to flatten area to that height

Advanced Options:
- Keep High: Only lower tall areas
- Keep Low: Only raise low areas
- Smooth Edges: Gradual transition at edges

Example: Creating a Building Site
1. Select area location
2. Set height = 70 (desired foundation level)
3. Set radius = 8 (16×16 block area)
4. Click to flatten
5. Use Smooth tool on edges
```

#### 6. Smooth Tool
**Purpose**: Smooth terrain by averaging heights
**Best for**: Refining rough terrain, blending transitions

```
Usage:
1. Select "Smooth" tool
2. Set brush size
3. Set strength (number of passes)
4. Click and drag over rough terrain

Tips:
- Multiple passes = smoother result
- Lower strength = more control
- Use after Brush tool for natural look
- Great for fixing harsh edges

Example: Smoothing a Mountain
1. Create rough mountain shape with Brush
2. Switch to Smooth tool
3. Size = 5, Strength = 1.0
4. Drag over mountain slopes
5. Repeat 2-3 times for best results
```

#### 7. Paint Tool
**Purpose**: Change block types without changing geometry
**Best for**: Texturing terrain, changing materials

```
Usage:
1. Select "Paint" tool
2. Choose new material
3. Set brush size
4. Click and drag to paint terrain

Use Cases:
- Converting grass to sand for beach
- Adding snow to mountain tops
- Creating paths (dirt to stone)
- Decorative patterns
- Biome transitions
```

### Complete Terrain Sculpting Workflow

**Example: Creating a Village Building Site**

```
Step 1: Rough Terrain (Brush Tool)
- Select Brush tool
- Size: 8, Strength: 2.0
- Material: Stone
- Create rough hill shape
- Click and drag to build up terrain

Step 2: Flatten Foundation (Flatten Tool)
- Select Flatten tool
- Target height: 70
- Radius: 6 (12×12 area)
- Click center of hill to create flat area

Step 3: Smooth Edges (Smooth Tool)
- Select Smooth tool
- Size: 4, Strength: 1.5
- Drag around edges of foundation
- 2-3 passes for natural slope

Step 4: Add Details (Paint Tool)
- Select Paint tool
- Change to Grass material
- Paint slopes green
- Change to Dirt for paths
- Paint paths to foundation

Step 5: Build Structure (Cube Tool)
- Select Filled Cube tool
- Dimensions: 8×1×8 (foundation layer)
- Material: Stone
- Place on flattened area

Step 6: Add Walls (Cube Tool)
- Select Hollow Cube
- Dimensions: 8×4×8
- Material: Wood Planks
- Stack on foundation

Step 7: Details (Single Block Tool)
- Select Single Block
- Add door opening
- Add window openings
- Add decorative elements
```

### Advanced Sculpting Techniques

#### Creating Caves
```
1. Use Hollow Sphere with Air material
2. Place multiple overlapping spheres underground
3. Vary sizes (radius 3-8) for natural feel
4. Use Smooth tool on cave edges
5. Paint walls with stone/dirt mixture
```

#### Natural Cliffs
```
1. Create rough cliff with Cube tool
2. Use Brush tool to add/remove random sections
3. Lower strength for varied surface
4. Smooth specific areas, leave others rough
5. Paint with stone, add moss with Paint tool
```

#### Terraced Farms
```
1. Use Flatten tool at multiple heights
2. Height 64: First terrace
3. Height 68: Second terrace
4. Height 72: Third terrace
5. Smooth edges between levels
6. Paint with dirt/grass
```

#### Procedural Settings

For terrain generation with noise:
```
Tool Settings → Procedural:
- Noise Scale: Controls feature size
  - Low (0.1): Large, smooth features
  - High (2.0): Small, detailed features
- Noise Strength: Height variation
  - Low (0.5): Subtle variation
  - High (3.0): Dramatic variation
- Seed: Deterministic randomness
  - Change seed for different patterns
```

### Building Tools (Enshrouded-Inspired)

The engine includes specialized building tools:

#### Construction Hammer
Quick building with 7 blueprint types:
- **Walls**: Vertical barriers
- **Foundations**: Ground-level support
- **Columns**: Vertical supports
- **Floors**: Horizontal platforms
- **Stairs**: Vertical access
- **Arches**: Decorative entrances
- **Roofs**: Shelter structures

```
Usage:
1. Select Construction Hammer from Tools
2. Choose blueprint type
3. Select material
4. Click to place, drag for multiple
5. Snaps to grid automatically
```

#### Pickaxe
Mining and removal tool:
- Area operations for fast clearing
- Bedrock protection (can't remove)
- Configurable depth/radius
- Collect removed blocks

```
Usage:
1. Select Pickaxe tool
2. Set operation mode:
   - Single: One block
   - Area: Multiple blocks
   - Layer: Entire layer
3. Click to remove voxels
4. Collected materials added to inventory
```

#### Rake
Terrain leveling and smoothing:
- Flattening for foundations
- Smoothing rough areas
- Precise height control
- Gradual blending

```
Usage:
1. Select Rake tool
2. Set target height
3. Drag over terrain to level
4. Perfect for building preparations
```

See full details: `docs/guides/VOXEL_TOOLS.md`

### Undo/Redo System

All voxel operations support undo/redo:
```
Ctrl+Z: Undo (up to 100 operations)
Ctrl+Y: Redo
```

The system tracks:
- All voxel placements/removals
- Tool operations (brush, sphere, etc.)
- Paint operations
- Transform changes

### Performance Considerations

**Optimization Tips:**
- Use Filled Cube for large solid structures (faster than placing individual blocks)
- Lower brush strength for better performance during real-time sculpting
- Save frequently when doing large terrain modifications
- Use View Distance settings to manage chunk loading
- Press F3 to monitor performance (FPS, triangle count, memory)

**Chunk System:**
- World divided into 16×256×16 chunks
- Only loaded chunks are editable
- Automatic mesh regeneration after edits
- Greedy meshing reduces triangle count by 90%

---

## Testing Your Work

### Play-in-Editor

Test your interactive elements and terrain:
```
1. Press F5 or click Play button
2. Editor switches to Play Mode
3. Test player movement and interactions
4. NPCs become active
5. Lua scripts execute
6. Press Stop to return to editor
```

**Play Mode Features:**
- Physics enabled (gravity, collision)
- All scripts active
- NPC AI behaviors running
- Player controller active
- WASD movement, mouse look
- Interaction with scripted elements

**Editor Mode Features:**
- Free-flying camera (no gravity)
- No collision
- WASD + Space/Ctrl for vertical
- Direct voxel editing
- Transform gizmos for entities

### Debug Overlay

Press F3 to show performance metrics:
- FPS (frames per second)
- Frame time (milliseconds)
- Chunks loaded/visible
- Triangle count
- Draw calls
- Memory usage
- Player position/rotation

### Console Commands

Use Console panel for testing:
```
spawn_npc villager 100 64 100    # Spawn NPC at coordinates
reload_scripts                    # Reload all Lua scripts
tp 200 70 200                     # Teleport player
give_item health_potion 5         # Give items to player
set_time 12.0                     # Set time of day
clear_console                     # Clear console log
```

---

## Summary and Best Practices

### Workflow Checklist

**Starting a New Level:**
- [ ] Create new world or load existing
- [ ] Set up preferred editor layout
- [ ] Enable Tool Palette and Inspector
- [ ] Set appropriate view distance

**Terrain Sculpting:**
- [ ] Use Brush for rough shapes
- [ ] Use Flatten for building areas
- [ ] Use Smooth for natural transitions
- [ ] Use Paint for material variation
- [ ] Test scale by placing player-sized references

**Adding Interactivity:**
- [ ] Create Lua script files in `scripts/` directory
- [ ] Test scripts in isolation first
- [ ] Attach scripts to entities via Inspector
- [ ] Use Console for debugging
- [ ] Test in Play Mode (F5)

**Building Structures:**
- [ ] Flatten foundation area first
- [ ] Use Cube tool for basic shapes
- [ ] Use Single Block for details
- [ ] Add interactive elements (doors, chests)
- [ ] Attach appropriate scripts

**Testing:**
- [ ] Save frequently (Ctrl+S)
- [ ] Test in Play Mode regularly
- [ ] Check Console for errors
- [ ] Monitor performance (F3)
- [ ] Undo if something breaks (Ctrl+Z)

### Common Mistakes to Avoid

1. **Not saving frequently**: Crashes can lose work
2. **Ignoring performance**: Watch triangle count and FPS
3. **Forgetting to test scripts**: Always test in Play Mode
4. **Not using layouts**: Customize for efficiency
5. **Overusing high-strength tools**: Start gentle, increase as needed
6. **Neglecting the Console**: Check for script errors regularly

### Additional Resources

- **Complete Editor Guide**: `docs/editor/EDITOR_INTEGRATION.md`
- **Lua Scripting Guide**: `docs/guides/LUA_INTEGRATION_GUIDE.md`
- **Voxel Tools Reference**: `docs/guides/VOXEL_TOOLS.md`
- **Terraforming Guide**: `docs/guides/TERRAFORMING.md`
- **Controls Reference**: `docs/guides/CONTROLS.md`
- **Layout Management**: `docs/editor/LAYOUT_MANAGEMENT.md`

---

## Conclusion

The Fresh Voxel Engine provides an Unreal Engine-inspired workflow that makes game development intuitive and efficient:

- **Customizable layouts** let you optimize the workspace for your role
- **Lua scripting** provides Blueprint-like functionality for interactive elements
- **Comprehensive voxel tools** enable terrain sculpting similar to Unreal's Modeling Mode

By mastering these three core concepts—layout customization, Lua scripting, and voxel editing—you can create rich, interactive voxel worlds efficiently.

**Next Steps:**
1. Experiment with different layouts to find your optimal setup
2. Try the example Lua scripts and modify them
3. Build a small scene using the voxel tools
4. Test your work in Play Mode
5. Explore the additional documentation for advanced features

Happy developing!
