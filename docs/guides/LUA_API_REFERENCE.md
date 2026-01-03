# Lua API Reference

## Overview

This document provides a complete reference for the Fresh Voxel Engine Lua API. All engine systems are accessible from Lua scripts, enabling powerful modding and scripting capabilities.

## Getting Started

To use the Lua API in your scripts:

```lua
-- Access global objects (provided by engine)
-- world - VoxelWorld instance
-- player - Player instance  
-- input - InputManager instance

-- Use the API
local pos = player:get_position()
world:set_voxel(pos.x, pos.y, pos.z, VoxelType.Diamond)
```

## Math Types

### Vec3

3D floating-point vector.

#### Constructors

```lua
local v1 = Vec3.new()              -- (0, 0, 0)
local v2 = Vec3.new(1, 2, 3)       -- (1, 2, 3)
```

#### Properties

```lua
v.x  -- X component (float)
v.y  -- Y component (float)
v.z  -- Z component (float)
```

#### Operations

```lua
-- Arithmetic
local sum = v1 + v2           -- Vector addition
local diff = v1 - v2          -- Vector subtraction
local scaled = v1 * 2.0       -- Scalar multiplication

-- Methods
local len = v:length()        -- Get length/magnitude
local normalized = v:normalize()  -- Get normalized vector
local dot = v1:dot(v2)        -- Dot product
local cross = v1:cross(v2)    -- Cross product
local dist = v1:distance(v2)  -- Distance between vectors
```

### IVec3

3D integer vector (for voxel coordinates).

#### Constructors

```lua
local coord = IVec3.new(10, 64, 20)
```

#### Properties

```lua
coord.x  -- X component (int)
coord.y  -- Y component (int)
coord.z  -- Z component (int)
```

## Voxel Types

### VoxelType Enum

Available voxel types:

```lua
VoxelType.Air       -- 0
VoxelType.Stone     -- 1
VoxelType.Dirt      -- 2
VoxelType.Grass     -- 3
VoxelType.Sand      -- 4
VoxelType.Water     -- 5
VoxelType.Wood      -- 6
VoxelType.Leaves    -- 7
VoxelType.Coal      -- 8
VoxelType.Iron      -- 9
VoxelType.Gold      -- 10
VoxelType.Diamond   -- 11
VoxelType.Bedrock   -- 12
```

## VoxelWorld

World manipulation and queries.

### Methods

#### get_voxel(x, y, z)

Get voxel type at world position.

```lua
local voxel = world:get_voxel(10, 64, 10)
if voxel == VoxelType.Grass then
    print("Found grass!")
end
```

#### set_voxel(x, y, z, voxelType)

Set voxel type at world position.

```lua
-- Place a diamond block
world:set_voxel(10, 65, 10, VoxelType.Diamond)

-- Remove a block (set to air)
world:set_voxel(10, 66, 10, VoxelType.Air)
```

#### is_solid(x, y, z)

Check if voxel is solid (not air).

```lua
if world:is_solid(x, y, z) then
    print("Solid block at position")
end
```

#### is_transparent(x, y, z)

Check if voxel is transparent (air, water, leaves).

```lua
if world:is_transparent(x, y, z) then
    print("Transparent block")
end
```

### Examples

```lua
-- Create a stone pillar
for y = 64, 74 do
    world:set_voxel(0, y, 0, VoxelType.Stone)
end

-- Create a cube
function create_cube(x, y, z, size, voxelType)
    for dx = 0, size - 1 do
        for dy = 0, size - 1 do
            for dz = 0, size - 1 do
                world:set_voxel(x + dx, y + dy, z + dz, voxelType)
            end
        end
    end
end

create_cube(10, 64, 10, 5, VoxelType.Gold)
```

## Player

Player state and control.

### Methods

#### get_position()

Get player world position.

```lua
local pos = player:get_position()
print("Player at: " .. pos.x .. ", " .. pos.y .. ", " .. pos.z)
```

#### set_position(vec3)

Teleport player to position.

```lua
local newPos = Vec3.new(0, 100, 0)
player:set_position(newPos)
```

#### get_velocity()

Get player velocity vector.

```lua
local vel = player:get_velocity()
print("Speed: " .. vel:length())
```

#### is_grounded()

Check if player is on the ground.

```lua
if player:is_grounded() then
    print("On ground")
end
```

#### is_flying()

Check if player is in fly mode.

```lua
if player:is_flying() then
    print("Flying!")
end
```

#### set_flying(enabled)

Enable/disable fly mode.

```lua
player:set_flying(true)   -- Enable flying
player:set_flying(false)  -- Disable flying
```

#### get_camera()

Get player's camera.

```lua
local camera = player:get_camera()
local lookDir = camera:get_front()
```

## Camera

Camera state and properties.

### Methods

#### get_position()

Get camera world position.

```lua
local camPos = camera:get_position()
```

#### get_front()

Get camera forward direction (normalized).

```lua
local forward = camera:get_front()
-- Use for raycasting, looking direction, etc.
```

#### get_up()

Get camera up direction (normalized).

```lua
local up = camera:get_up()
```

#### get_right()

Get camera right direction (normalized).

```lua
local right = camera:get_right()
```

#### get_fov()

Get field of view in degrees.

```lua
local fov = camera:get_fov()  -- e.g., 75.0
```

#### set_fov(degrees)

Set field of view.

```lua
camera:set_fov(90.0)  -- Wider FOV
camera:set_fov(60.0)  -- Narrow FOV
```

#### get_near_plane()

Get near clipping plane distance.

```lua
local near = camera:get_near_plane()
```

#### get_far_plane()

Get far clipping plane distance.

```lua
local far = camera:get_far_plane()
```

## InputManager

Input state queries.

### Methods

#### is_key_pressed(key)

Check if key was pressed this frame.

```lua
-- GLFW key codes (examples)
-- 'W' = 87, 'A' = 65, 'S' = 83, 'D' = 68
-- Space = 32, Enter = 257, Escape = 256

if input:is_key_pressed(32) then  -- Space
    print("Jump!")
end
```

#### is_key_down(key)

Check if key is currently held down.

```lua
if input:is_key_down(87) then  -- W key
    -- Move forward
end
```

#### is_key_up(key)

Check if key was released this frame.

```lua
if input:is_key_up(32) then
    print("Released space")
end
```

#### is_mouse_button_pressed(button)

Check if mouse button pressed (0=left, 1=right, 2=middle).

```lua
if input:is_mouse_button_pressed(0) then
    print("Left click!")
end
```

#### get_mouse_position()

Get current mouse position (Vec2).

```lua
local mousePos = input:get_mouse_position()
print("Mouse: " .. mousePos.x .. ", " .. mousePos.y)
```

#### get_mouse_delta()

Get mouse movement delta since last frame.

```lua
local delta = input:get_mouse_delta()
-- Use for camera rotation, etc.
```

## Utility Functions

### log(message)

Log info message to console.

```lua
log("Hello from Lua!")
```

### log_warning(message)

Log warning message.

```lua
log_warning("This is a warning")
```

### log_error(message)

Log error message.

```lua
log_error("Something went wrong")
```

### get_time()

Get current engine time in seconds.

```lua
local time = get_time()
print("Engine running for: " .. time .. " seconds")
```

## Example Scripts

### Place Blocks Around Player

```lua
function place_ring_around_player()
    local pos = player:get_position()
    local radius = 5
    
    for angle = 0, 360, 30 do
        local rad = math.rad(angle)
        local x = pos.x + radius * math.cos(rad)
        local z = pos.z + radius * math.sin(rad)
        local y = pos.y
        
        world:set_voxel(
            math.floor(x), 
            math.floor(y), 
            math.floor(z), 
            VoxelType.Diamond
        )
    end
    
    log("Created diamond ring!")
end
```

### Build a House

```lua
function build_house(x, y, z)
    local width = 8
    local height = 5
    local depth = 8
    
    -- Floor
    for dx = 0, width do
        for dz = 0, depth do
            world:set_voxel(x + dx, y, z + dz, VoxelType.Wood)
        end
    end
    
    -- Walls
    for dy = 1, height do
        -- Front and back walls
        for dx = 0, width do
            world:set_voxel(x + dx, y + dy, z, VoxelType.Stone)
            world:set_voxel(x + dx, y + dy, z + depth, VoxelType.Stone)
        end
        
        -- Side walls
        for dz = 0, depth do
            world:set_voxel(x, y + dy, z + dz, VoxelType.Stone)
            world:set_voxel(x + width, y + dy, z + dz, VoxelType.Stone)
        end
    end
    
    -- Door
    world:set_voxel(x + width / 2, y + 1, z, VoxelType.Air)
    world:set_voxel(x + width / 2, y + 2, z, VoxelType.Air)
    
    log("House built!")
end

-- Build at player location
local pos = player:get_position()
build_house(math.floor(pos.x), math.floor(pos.y), math.floor(pos.z))
```

### Raycast to Place Blocks

```lua
function place_block_at_look()
    local cam = player:get_camera()
    local pos = cam:get_position()
    local dir = cam:get_front()
    
    -- Raycast forward
    local distance = 10
    for i = 1, distance do
        local x = math.floor(pos.x + dir.x * i)
        local y = math.floor(pos.y + dir.y * i)
        local z = math.floor(pos.z + dir.z * i)
        
        if world:is_solid(x, y, z) then
            -- Found solid block, place in front of it
            local px = math.floor(pos.x + dir.x * (i - 1))
            local py = math.floor(pos.y + dir.y * (i - 1))
            local pz = math.floor(pos.z + dir.z * (i - 1))
            
            world:set_voxel(px, py, pz, VoxelType.Diamond)
            log("Placed diamond!")
            return
        end
    end
    
    log("No block in range")
end
```

### Distance-Based Effects

```lua
function explode_nearby_blocks()
    local pos = player:get_position()
    local radius = 5
    
    for dx = -radius, radius do
        for dy = -radius, radius do
            for dz = -radius, radius do
                local x = math.floor(pos.x + dx)
                local y = math.floor(pos.y + dy)
                local z = math.floor(pos.z + dz)
                
                local blockPos = Vec3.new(x, y, z)
                local dist = pos:distance(blockPos)
                
                if dist <= radius then
                    world:set_voxel(x, y, z, VoxelType.Air)
                end
            end
        end
    end
    
    log("Explosion!")
end
```

## Mod Structure

### Recommended Mod Layout

```
mods/
  my_mod/
    init.lua        -- Main mod entry point
    config.lua      -- Mod configuration
    scripts/
      gameplay.lua  -- Gameplay scripts
      world.lua     -- World generation
    README.md       -- Mod documentation
```

### init.lua Template

```lua
-- Mod metadata
MOD_NAME = "My Awesome Mod"
MOD_VERSION = "1.0.0"
MOD_AUTHOR = "Your Name"

-- Initialization
function on_init()
    log("Loading " .. MOD_NAME .. " v" .. MOD_VERSION)
    
    -- Register commands
    -- Load configuration
    -- Initialize systems
    
    log(MOD_NAME .. " loaded successfully!")
end

-- Update loop (called every frame)
function on_update(deltaTime)
    -- Update game logic
end

-- Cleanup
function on_shutdown()
    log("Unloading " .. MOD_NAME)
end

-- Call initialization
on_init()
```

## Best Practices

1. **Performance**: Avoid creating vectors in loops - reuse them
2. **Error Handling**: Use pcall() for operations that might fail
3. **Logging**: Use log() for debugging, not print()
4. **Naming**: Use descriptive function and variable names
5. **Documentation**: Comment your code for other modders

## Advanced Topics

### Custom Events

```lua
-- Define event listeners
listeners = {}

function register_event(eventName, callback)
    if not listeners[eventName] then
        listeners[eventName] = {}
    end
    table.insert(listeners[eventName], callback)
end

function fire_event(eventName, ...)
    if listeners[eventName] then
        for _, callback in ipairs(listeners[eventName]) do
            callback(...)
        end
    end
end

-- Usage
register_event("player_jump", function()
    log("Player jumped!")
end)

fire_event("player_jump")
```

### Persistent Data

```lua
-- Save mod data (future enhancement)
function save_mod_data(key, value)
    -- Will save to mods/mymod/data.json
end

function load_mod_data(key)
    -- Will load from mods/mymod/data.json
end
```

## Troubleshooting

### Common Issues

**Problem**: "attempt to index a nil value (global 'world')"
- The world object isn't available in this context
- Check that your script runs when the world is loaded

**Problem**: Blocks not appearing
- Ensure coordinates are within world bounds
- Check that voxel type is valid
- World might need to regenerate chunks

**Problem**: Script errors on load
- Check Lua syntax with a validator
- Look at console for error messages
- Use pcall() to catch errors gracefully

## References

- See `examples/scripts/api_demo.lua` for complete API demonstration
- See `examples/scripts/example.lua` for basic Lua examples
- Check `docs/guides/LUA_SCRIPTING_INTEGRATION.md` for setup guide

---

**Last Updated**: 2026-01-03
**Version**: 1.0.0
