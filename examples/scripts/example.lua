-- Example Lua Script for Fresh Voxel Engine
-- This demonstrates the scripting capabilities

print("=== Fresh Voxel Engine Lua Script Example ===")

-- Basic math operations
x = 10
y = 20
sum = x + y
print("Sum: " .. x .. " + " .. y .. " = " .. sum)

-- String manipulation
greeting = "Hello from Lua!"
print(greeting)

-- Table/dictionary
config = {
    world_size = 1000,
    chunk_size = 16,
    render_distance = 10,
    enable_fog = true
}

print("World Config:")
for k, v in pairs(config) do
    print("  " .. k .. " = " .. tostring(v))
end

-- Function definition
function calculate_distance(x1, y1, x2, y2)
    local dx = x2 - x1
    local dy = y2 - y1
    return math.sqrt(dx*dx + dy*dy)
end

-- Test the function
distance = calculate_distance(0, 0, 3, 4)
print("Distance from (0,0) to (3,4): " .. distance)

-- Iteration
print("\nCounting to 5:")
for i = 1, 5 do
    print("  " .. i)
end

-- Conditional logic
player_health = 75
if player_health > 80 then
    print("Player health: Excellent")
elseif player_health > 50 then
    print("Player health: Good")
else
    print("Player health: Low - Find healing!")
end

print("\n=== Script execution complete ===")
