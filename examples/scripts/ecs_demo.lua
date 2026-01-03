-- ECS Demo Script for Fresh Voxel Engine
-- Demonstrates entity creation, manipulation, and queries

log("=== ECS Demo Script Loaded ===")

-- Configuration
local DEMO_CONFIG = {
    gridSize = 5,
    spacing = 10.0,
    startHeight = 70.0,
    rotationSpeed = 45.0,  -- degrees per second
    bounceHeight = 5.0,
    bounceSpeed = 2.0
}

-- Demo state
local demoState = {
    entities = {},
    startTime = 0,
    initialized = false
}

--[[
    Demo 1: Create a grid of entities
]]
function createEntityGrid()
    log("Creating " .. (DEMO_CONFIG.gridSize * DEMO_CONFIG.gridSize) .. " entities in a grid...")
    
    local entities = {}
    local halfSize = DEMO_CONFIG.gridSize / 2
    
    for x = 0, DEMO_CONFIG.gridSize - 1 do
        for z = 0, DEMO_CONFIG.gridSize - 1 do
            -- Create entity
            local entity = ECS.createEntity()
            
            -- Calculate position (centered around origin)
            local posX = (x - halfSize) * DEMO_CONFIG.spacing
            local posZ = (z - halfSize) * DEMO_CONFIG.spacing
            
            -- Add transform component
            ECS.addTransform(entity, posX, DEMO_CONFIG.startHeight, posZ)
            
            -- Store entity
            table.insert(entities, entity)
        end
    end
    
    log("Created " .. #entities .. " entities!")
    return entities
end

--[[
    Demo 2: Animate entities with rotation and bouncing
]]
function animateEntities(entities, deltaTime)
    local time = os.clock() - demoState.startTime
    
    for i, entity in ipairs(entities) do
        if ECS.isEntityValid(entity) and ECS.hasTransform(entity) then
            -- Get current position
            local pos = ECS.getPosition(entity)
            
            -- Calculate bounce offset (each entity has phase offset)
            local phase = (i / #entities) * math.pi * 2
            local bounce = math.sin(time * DEMO_CONFIG.bounceSpeed + phase) * DEMO_CONFIG.bounceHeight
            
            -- Update position with bounce
            ECS.setPosition(entity, pos.x, DEMO_CONFIG.startHeight + bounce, pos.z)
            
            -- Update rotation
            local rotation = (time * DEMO_CONFIG.rotationSpeed + (i * 10)) % 360
            ECS.setRotation(entity, 0, rotation, 0)
        end
    end
end

--[[
    Demo 3: Query and display entity statistics
]]
function displayEntityStats()
    local allEntities = ECS.getAllEntities()
    local transformEntities = ECS.getEntitiesWithTransform()
    
    log("=== Entity Statistics ===")
    log("Total entities: " .. #allEntities)
    log("Entities with Transform: " .. #transformEntities)
    
    if #transformEntities > 0 then
        log("\nFirst 3 entity positions:")
        for i = 1, math.min(3, #transformEntities) do
            local entity = transformEntities[i]
            local pos = ECS.getPosition(entity)
            log(string.format("  Entity %d: (%.2f, %.2f, %.2f)", 
                entity:id(), pos.x, pos.y, pos.z))
        end
    end
end

--[[
    Demo 4: Demonstrate entity lifecycle
]]
function demonstrateLifecycle()
    log("\n=== Demonstrating Entity Lifecycle ===")
    
    -- Create temporary entity
    local temp = ECS.createEntity()
    log("Created temporary entity: " .. temp:id())
    log("Is valid? " .. tostring(ECS.isEntityValid(temp)))
    
    -- Add component
    ECS.addTransform(temp, 0, 100, 0)
    log("Added transform component")
    log("Has transform? " .. tostring(ECS.hasTransform(temp)))
    
    -- Query position
    local pos = ECS.getPosition(temp)
    log(string.format("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z))
    
    -- Modify properties
    ECS.setPosition(temp, 10, 110, 10)
    ECS.setRotation(temp, 0, 45, 0)
    ECS.setScale(temp, 2, 2, 2)
    
    pos = ECS.getPosition(temp)
    local rot = ECS.getRotation(temp)
    local scale = ECS.getScale(temp)
    
    log(string.format("Updated - Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z))
    log(string.format("Updated - Rot: (%.2f, %.2f, %.2f)", rot.x, rot.y, rot.z))
    log(string.format("Updated - Scale: (%.2f, %.2f, %.2f)", scale.x, scale.y, scale.z))
    
    -- Remove component
    ECS.removeTransform(temp)
    log("Removed transform component")
    log("Has transform? " .. tostring(ECS.hasTransform(temp)))
    
    -- Destroy entity
    ECS.destroyEntity(temp)
    log("Destroyed entity")
    log("Is valid? " .. tostring(ECS.isEntityValid(temp)))
end

--[[
    Demo 5: Find nearest entity to a position
]]
function findNearestEntity(position)
    local entities = ECS.getEntitiesWithTransform()
    local nearestEntity = nil
    local nearestDistance = math.huge
    
    for _, entity in ipairs(entities) do
        local pos = ECS.getPosition(entity)
        
        -- Calculate distance
        local dx = pos.x - position.x
        local dy = pos.y - position.y
        local dz = pos.z - position.z
        local distance = math.sqrt(dx*dx + dy*dy + dz*dz)
        
        if distance < nearestDistance then
            nearestDistance = distance
            nearestEntity = entity
        end
    end
    
    if nearestEntity then
        local pos = ECS.getPosition(nearestEntity)
        log(string.format("Nearest entity: %d at distance %.2f", 
            nearestEntity:id(), nearestDistance))
        log(string.format("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z))
    else
        log("No entities found")
    end
    
    return nearestEntity, nearestDistance
end

--[[
    Initialize demo
]]
function initializeDemo()
    if demoState.initialized then
        log("Demo already initialized!")
        return
    end
    
    log("\n========================================")
    log("  Fresh Voxel Engine - ECS Demo")
    log("========================================\n")
    
    -- Store start time
    demoState.startTime = os.clock()
    
    -- Run lifecycle demonstration
    demonstrateLifecycle()
    
    -- Create entities for animation
    log("\n=== Creating Entity Grid for Animation ===")
    demoState.entities = createEntityGrid()
    
    -- Display initial statistics
    displayEntityStats()
    
    -- Test nearest entity search
    log("\n=== Testing Nearest Entity Search ===")
    findNearestEntity({ x = 0, y = 70, z = 0 })
    
    demoState.initialized = true
    
    log("\n=== Demo Initialized! ===")
    log("Entities will animate automatically.")
    log("Edit this file to see hot-reload in action!")
end

--[[
    Update function (called every frame)
]]
function on_update(deltaTime)
    if not demoState.initialized then
        initializeDemo()
    end
    
    -- Animate entities
    if #demoState.entities > 0 then
        animateEntities(demoState.entities, deltaTime)
    end
    
    -- Print statistics every 5 seconds
    local time = os.clock() - demoState.startTime
    if math.floor(time) % 5 == 0 and math.floor(time * 10) % 10 == 0 then
        log(string.format("\n[%.1f seconds] Animating %d entities", 
            time, #demoState.entities))
    end
end

--[[
    Cleanup function
]]
function cleanup()
    log("\n=== Cleaning Up Demo ===")
    
    -- Destroy all demo entities
    for _, entity in ipairs(demoState.entities) do
        if ECS.isEntityValid(entity) then
            ECS.destroyEntity(entity)
        end
    end
    
    demoState.entities = {}
    demoState.initialized = false
    
    log("Demo cleanup complete!")
end

-- Initialize demo on load
initializeDemo()

log("=== ECS Demo Script Ready ===")
log("Modify this file to test hot-reload!")
