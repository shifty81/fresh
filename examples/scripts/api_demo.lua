-- Fresh Voxel Engine - Advanced Lua API Example
-- This script demonstrates interaction with engine systems

print("=== Fresh Engine Lua API Demo ===")

-- ============================================================================
-- Math API Demo
-- ============================================================================

print("\n--- Math API ---")

-- Create vectors
local pos1 = Vec3.new(10, 20, 30)
local pos2 = Vec3.new(5, 10, 15)

print("Position 1: (" .. pos1.x .. ", " .. pos1.y .. ", " .. pos1.z .. ")")
print("Position 2: (" .. pos2.x .. ", " .. pos2.y .. ", " .. pos2.z .. ")")

-- Vector math
local sum = pos1 + pos2
print("Sum: (" .. sum.x .. ", " .. sum.y .. ", " .. sum.z .. ")")

local diff = pos1 - pos2
print("Difference: (" .. diff.x .. ", " .. diff.y .. ", " .. diff.z .. ")")

local scaled = pos1 * 0.5
print("Scaled: (" .. scaled.x .. ", " .. scaled.y .. ", " .. scaled.z .. ")")

local dist = pos1:distance(pos2)
print("Distance between positions: " .. dist)

-- ============================================================================
-- Voxel Type Demo
-- ============================================================================

print("\n--- Voxel Types ---")

print("Available voxel types:")
print("  Air = " .. VoxelType.Air)
print("  Stone = " .. VoxelType.Stone)
print("  Grass = " .. VoxelType.Grass)
print("  Water = " .. VoxelType.Water)
print("  Diamond = " .. VoxelType.Diamond)

-- ============================================================================
-- World Interaction (if world is available)
-- ============================================================================

if world then
    print("\n--- World Interaction ---")
    
    -- Get voxel at a position
    local voxel = world:get_voxel(0, 64, 0)
    print("Voxel at (0, 64, 0): " .. voxel)
    
    -- Place a diamond block
    world:set_voxel(10, 65, 10, VoxelType.Diamond)
    print("Placed diamond at (10, 65, 10)")
    
    -- Create a small structure
    print("Creating a stone pillar...")
    for y = 64, 70 do
        world:set_voxel(5, y, 5, VoxelType.Stone)
    end
    print("Stone pillar created at x=5, z=5, y=64-70")
    
else
    print("\n--- World not available in this context ---")
end

-- ============================================================================
-- Player Interaction (if player is available)
-- ============================================================================

if player then
    print("\n--- Player Interaction ---")
    
    -- Get player position
    local playerPos = player:get_position()
    print("Player position: (" .. playerPos.x .. ", " .. playerPos.y .. ", " .. playerPos.z .. ")")
    
    -- Get player velocity
    local vel = player:get_velocity()
    print("Player velocity: (" .. vel.x .. ", " .. vel.y .. ", " .. vel.z .. ")")
    
    -- Check player state
    if player:is_grounded() then
        print("Player is on the ground")
    else
        print("Player is in the air")
    end
    
    if player:is_flying() then
        print("Player is in fly mode")
    else
        print("Player is in walk mode")
    end
    
    -- Access camera through player
    local camera = player:get_camera()
    local cameraPos = camera:get_position()
    print("Camera position: (" .. cameraPos.x .. ", " .. cameraPos.y .. ", " .. cameraPos.z .. ")")
    
    local cameraFront = camera:get_front()
    print("Camera looking direction: (" .. cameraFront.x .. ", " .. cameraFront.y .. ", " .. cameraFront.z .. ")")
    
else
    print("\n--- Player not available in this context ---")
end

-- ============================================================================
-- Input System (if input is available)
-- ============================================================================

if input then
    print("\n--- Input System ---")
    
    -- Note: These would be called in an update loop in a real mod
    -- Here we just demonstrate the API
    
    print("Input API available:")
    print("  - input:is_key_pressed(key)")
    print("  - input:is_key_down(key)")
    print("  - input:is_mouse_button_pressed(button)")
    print("  - input:get_mouse_position()")
    print("  - input:get_mouse_delta()")
    
else
    print("\n--- Input not available in this context ---")
end

-- ============================================================================
-- Utility Functions
-- ============================================================================

print("\n--- Utility Functions ---")

-- Logging
log("This is an info message from Lua")
log_warning("This is a warning message")
log_error("This is an error message (for testing)")

-- Time
local currentTime = get_time()
print("Current engine time: " .. currentTime .. " seconds")

-- ============================================================================
-- Example: Custom Functions
-- ============================================================================

print("\n--- Custom Functions ---")

-- Function to create a cube of blocks
function create_cube(world, x, y, z, size, voxelType)
    log("Creating " .. size .. "x" .. size .. "x" .. size .. " cube at (" .. x .. ", " .. y .. ", " .. z .. ")")
    
    for dx = 0, size - 1 do
        for dy = 0, size - 1 do
            for dz = 0, size - 1 do
                world:set_voxel(x + dx, y + dy, z + dz, voxelType)
            end
        end
    end
    
    log("Cube created successfully")
end

-- Function to calculate distance to player
function distance_to_player(player, x, y, z)
    local playerPos = player:get_position()
    local target = Vec3.new(x, y, z)
    return playerPos:distance(target)
end

-- Function to check if player is near a location
function is_player_near(player, x, y, z, radius)
    local dist = distance_to_player(player, x, y, z)
    return dist < radius
end

-- Example usage (if world and player are available)
if world and player then
    print("\nTesting custom functions:")
    
    -- Create a small gold cube
    create_cube(world, 20, 64, 20, 3, VoxelType.Gold)
    
    -- Check distance to a location
    local dist = distance_to_player(player, 20, 64, 20)
    print("Distance to gold cube: " .. dist .. " units")
    
    if is_player_near(player, 20, 64, 20, 50) then
        log("Player is near the gold cube!")
    else
        log("Player is far from the gold cube")
    end
end

-- ============================================================================
-- Event Handler Example
-- ============================================================================

print("\n--- Event Handlers ---")

-- Define an update function (would be called each frame by the engine)
function on_update(deltaTime)
    -- This would run every frame
    -- Example: Check input, update game logic, etc.
end

-- Define an initialization function
function on_init()
    log("Mod initialized!")
end

-- Define a player move event
function on_player_move(player)
    local pos = player:get_position()
    -- React to player movement
end

print("\nEvent handlers defined (on_update, on_init, on_player_move)")

-- ============================================================================

print("\n=== Lua API Demo Complete ===")
print("All engine systems are accessible from Lua!")
print("Use these APIs to create mods, customize gameplay, and extend the engine.")
