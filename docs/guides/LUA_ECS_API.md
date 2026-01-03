# Lua ECS (Entity-Component-System) API Reference

## Overview

The Fresh Voxel Engine exposes its Entity-Component-System (ECS) architecture to Lua, allowing scripts to create and manipulate game entities dynamically. This enables powerful gameplay scripting without needing to modify or recompile C++ code.

## Table of Contents

1. [Core Concepts](#core-concepts)
2. [Entity Management](#entity-management)
3. [Transform Component](#transform-component)
4. [Entity Queries](#entity-queries)
5. [Examples](#examples)
6. [Best Practices](#best-practices)

---

## Core Concepts

### What is ECS?

The Entity-Component-System architecture consists of:

- **Entities**: Unique IDs that represent game objects
- **Components**: Pure data containers attached to entities
- **Systems**: Logic that processes entities with specific components

### Available in Lua

The Lua ECS API provides access to:
- Entity creation and destruction
- Component addition and removal
- Component property manipulation
- Entity querying by component type

---

## Entity Management

### ECS.createEntity()

Creates a new entity in the world.

**Returns:** `Entity` - A new entity object

**Example:**
```lua
local player = ECS.createEntity()
local enemy = ECS.createEntity()
```

### ECS.destroyEntity(entity)

Destroys an entity and removes all its components.

**Parameters:**
- `entity` (Entity): The entity to destroy

**Example:**
```lua
local temp = ECS.createEntity()
-- ... use entity ...
ECS.destroyEntity(temp)  -- Clean up
```

### ECS.isEntityValid(entity)

Checks if an entity still exists and is valid.

**Parameters:**
- `entity` (Entity): The entity to check

**Returns:** `boolean` - True if valid, false otherwise

**Example:**
```lua
local entity = ECS.createEntity()
if ECS.isEntityValid(entity) then
    log("Entity is valid!")
end

ECS.destroyEntity(entity)
if not ECS.isEntityValid(entity) then
    log("Entity was destroyed")
end
```

### ECS.getAllEntities()

Gets all entities currently in the world.

**Returns:** `table` - Array of Entity objects

**Example:**
```lua
local allEntities = ECS.getAllEntities()
log("Total entities in world: " .. #allEntities)

for i, entity in ipairs(allEntities) do
    log("Entity " .. i .. ": ID=" .. entity:id())
end
```

---

## Transform Component

The Transform component stores an entity's position, rotation, and scale in 3D space.

### Adding Transform

#### ECS.addTransform(entity, x, y, z)

Adds a Transform component with the specified position.

**Parameters:**
- `entity` (Entity): The entity to add the component to
- `x` (number): X position
- `y` (number): Y position  
- `z` (number): Z position

**Example:**
```lua
local entity = ECS.createEntity()
ECS.addTransform(entity, 10.0, 65.0, 10.0)
```

#### ECS.addTransformWithRotation(entity, x, y, z, rx, ry, rz)

Adds a Transform component with position and rotation.

**Parameters:**
- `entity` (Entity): The entity to add the component to
- `x, y, z` (number): Position coordinates
- `rx, ry, rz` (number): Rotation euler angles in degrees

**Example:**
```lua
local entity = ECS.createEntity()
-- Position at (5, 64, 5), rotated 45 degrees around Y axis
ECS.addTransformWithRotation(entity, 5.0, 64.0, 5.0, 0.0, 45.0, 0.0)
```

### Checking and Removing

#### ECS.hasTransform(entity)

Checks if an entity has a Transform component.

**Parameters:**
- `entity` (Entity): The entity to check

**Returns:** `boolean` - True if entity has Transform

**Example:**
```lua
if ECS.hasTransform(entity) then
    log("Entity has transform!")
end
```

#### ECS.removeTransform(entity)

Removes the Transform component from an entity.

**Parameters:**
- `entity` (Entity): The entity to remove component from

**Example:**
```lua
ECS.removeTransform(entity)
```

### Position

#### ECS.setPosition(entity, x, y, z)

Sets the entity's position.

**Parameters:**
- `entity` (Entity): The entity to modify
- `x, y, z` (number): New position coordinates

**Example:**
```lua
ECS.setPosition(entity, 100.0, 70.0, 100.0)
```

#### ECS.getPosition(entity)

Gets the entity's current position.

**Parameters:**
- `entity` (Entity): The entity to query

**Returns:** `Vec3Result` with fields `x`, `y`, `z`

**Example:**
```lua
local pos = ECS.getPosition(entity)
log("Entity at: " .. pos.x .. ", " .. pos.y .. ", " .. pos.z)
```

### Rotation

#### ECS.setRotation(entity, rx, ry, rz)

Sets the entity's rotation in euler angles (degrees).

**Parameters:**
- `entity` (Entity): The entity to modify
- `rx, ry, rz` (number): Rotation angles in degrees

**Example:**
```lua
-- Rotate 90 degrees around Y axis
ECS.setRotation(entity, 0.0, 90.0, 0.0)
```

#### ECS.getRotation(entity)

Gets the entity's current rotation in euler angles (degrees).

**Parameters:**
- `entity` (Entity): The entity to query

**Returns:** `Vec3Result` with fields `x`, `y`, `z` (degrees)

**Example:**
```lua
local rot = ECS.getRotation(entity)
log("Entity rotated: " .. rot.x .. ", " .. rot.y .. ", " .. rot.z)
```

### Scale

#### ECS.setScale(entity, sx, sy, sz)

Sets the entity's scale.

**Parameters:**
- `entity` (Entity): The entity to modify
- `sx, sy, sz` (number): Scale factors (1.0 = normal size)

**Example:**
```lua
-- Make entity twice as large
ECS.setScale(entity, 2.0, 2.0, 2.0)
```

#### ECS.getScale(entity)

Gets the entity's current scale.

**Parameters:**
- `entity` (Entity): The entity to query

**Returns:** `Vec3Result` with fields `x`, `y`, `z`

**Example:**
```lua
local scale = ECS.getScale(entity)
log("Entity scale: " .. scale.x .. ", " .. scale.y .. ", " .. scale.z)
```

---

## Entity Queries

### ECS.getEntitiesWithTransform()

Gets all entities that have a Transform component.

**Returns:** `table` - Array of Entity objects

**Example:**
```lua
local transformEntities = ECS.getEntitiesWithTransform()
log("Entities with transforms: " .. #transformEntities)

for _, entity in ipairs(transformEntities) do
    local pos = ECS.getPosition(entity)
    log("Entity at " .. pos.x .. ", " .. pos.y .. ", " .. pos.z)
end
```

---

## Examples

### Example 1: Spawning Objects

```lua
-- Spawn a grid of objects
function spawnGrid()
    local entities = {}
    
    for x = 0, 9 do
        for z = 0, 9 do
            local entity = ECS.createEntity()
            ECS.addTransform(entity, x * 5.0, 65.0, z * 5.0)
            table.insert(entities, entity)
        end
    end
    
    log("Spawned " .. #entities .. " entities in a grid")
    return entities
end
```

### Example 2: Moving Entities

```lua
-- Move all entities in a circle pattern
function updateCircularMotion(deltaTime)
    local entities = ECS.getEntitiesWithTransform()
    local time = os.clock()
    
    for i, entity in ipairs(entities) do
        local angle = time + (i * 0.1)
        local radius = 10.0
        
        local x = math.cos(angle) * radius
        local z = math.sin(angle) * radius
        
        ECS.setPosition(entity, x, 65.0, z)
    end
end
```

### Example 3: Entity Management System

```lua
-- Entity pool system
local EntityPool = {
    active = {},
    inactive = {}
}

function EntityPool:get()
    local entity
    
    if #self.inactive > 0 then
        -- Reuse inactive entity
        entity = table.remove(self.inactive)
    else
        -- Create new entity
        entity = ECS.createEntity()
        ECS.addTransform(entity, 0.0, 0.0, 0.0)
    end
    
    table.insert(self.active, entity)
    return entity
end

function EntityPool:release(entity)
    -- Find and remove from active
    for i, e in ipairs(self.active) do
        if e:id() == entity:id() then
            table.remove(self.active, i)
            table.insert(self.inactive, entity)
            
            -- Reset position
            ECS.setPosition(entity, 0.0, 0.0, 0.0)
            break
        end
    end
end

function EntityPool:clear()
    -- Destroy all entities
    for _, entity in ipairs(self.active) do
        ECS.destroyEntity(entity)
    end
    for _, entity in ipairs(self.inactive) do
        ECS.destroyEntity(entity)
    end
    
    self.active = {}
    self.inactive = {}
end

-- Usage
local entity = EntityPool:get()
ECS.setPosition(entity, 10.0, 65.0, 10.0)
-- ... use entity ...
EntityPool:release(entity)  -- Return to pool
```

### Example 4: Distance-based Culling

```lua
-- Hide/show entities based on distance from player
function updateEntityCulling(playerPos, maxDistance)
    local entities = ECS.getEntitiesWithTransform()
    
    for _, entity in ipairs(entities) do
        local pos = ECS.getPosition(entity)
        
        -- Calculate distance
        local dx = pos.x - playerPos.x
        local dy = pos.y - playerPos.y
        local dz = pos.z - playerPos.z
        local distance = math.sqrt(dx*dx + dy*dy + dz*dz)
        
        if distance > maxDistance then
            -- Hide entity (in real implementation, would disable renderer)
            log("Entity " .. entity:id() .. " is too far (distance: " .. distance .. ")")
        end
    end
end
```

### Example 5: Entity Spawner System

```lua
-- Spawn entities over time
local Spawner = {
    spawnInterval = 2.0,  -- Spawn every 2 seconds
    nextSpawnTime = 0,
    maxEntities = 50,
    spawnRadius = 20.0
}

function Spawner:update(deltaTime, currentTime)
    if currentTime >= self.nextSpawnTime then
        local entities = ECS.getEntitiesWithTransform()
        
        if #entities < self.maxEntities then
            -- Spawn new entity at random position
            local angle = math.random() * math.pi * 2
            local radius = math.random() * self.spawnRadius
            
            local x = math.cos(angle) * radius
            local z = math.sin(angle) * radius
            
            local entity = ECS.createEntity()
            ECS.addTransform(entity, x, 65.0, z)
            
            log("Spawned entity " .. entity:id() .. " at " .. x .. ", " .. z)
        end
        
        self.nextSpawnTime = currentTime + self.spawnInterval
    end
end

-- Usage in game loop
local currentTime = 0
function on_update(deltaTime)
    currentTime = currentTime + deltaTime
    Spawner:update(deltaTime, currentTime)
end
```

---

## Best Practices

### 1. Check Entity Validity

Always check if an entity is valid before using it, especially if you store entity references:

```lua
if ECS.isEntityValid(storedEntity) then
    ECS.setPosition(storedEntity, x, y, z)
end
```

### 2. Use Entity Queries Efficiently

Cache entity query results when possible instead of calling every frame:

```lua
-- Bad: Queries every frame
function update()
    local entities = ECS.getEntitiesWithTransform()  -- Called every frame!
    -- ... process entities ...
end

-- Good: Query once, update cache when needed
local cachedEntities = {}
local needsRefresh = true

function update()
    if needsRefresh then
        cachedEntities = ECS.getEntitiesWithTransform()
        needsRefresh = false
    end
    -- ... process cachedEntities ...
end

function onEntitySpawned()
    needsRefresh = true
end
```

### 3. Clean Up Entities

Always destroy entities when they're no longer needed:

```lua
function despawnTemporaryObject(entity)
    -- Clean up
    ECS.destroyEntity(entity)
end
```

### 4. Use Object Pooling

For frequently spawned/despawned entities, use an object pool (see Example 3).

### 5. Organize by Component

Structure your code around components rather than specific entity types:

```lua
-- Good: Process all entities with transforms
function updateAllTransforms()
    local entities = ECS.getEntitiesWithTransform()
    for _, entity in ipairs(entities) do
        -- Process entity
    end
end

-- Instead of tracking specific entity types
```

---

## Future Component Types

The following component types are planned for future Lua bindings:

- **Renderer Component**: Visual appearance (mesh, material)
- **Physics Component**: Rigid body physics
- **Health Component**: Hit points and damage
- **AI Component**: Behavior and decision making
- **Audio Component**: Sound emission
- **Collider Component**: Collision shapes

Stay tuned for updates!

---

## See Also

- [Lua Scripting Integration Guide](LUA_SCRIPTING_INTEGRATION.md)
- [Lua API Reference](LUA_API_REFERENCE.md)
- [Hot-Reload Usage Guide](HOT_RELOAD_USAGE.md)
- [Scene Serialization](SCENE_SERIALIZATION.md)

---

**Version:** 1.0  
**Last Updated:** 2026-01-03  
**Status:** Production Ready
