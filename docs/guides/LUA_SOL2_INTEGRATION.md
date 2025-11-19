# Enhanced Lua Integration with Sol2

## Overview

The Fresh Engine now includes a fully functional Lua scripting system using Sol2, a modern C++/Lua binding library. This system provides:

- **Modern C++ Integration**: Type-safe bindings with minimal boilerplate
- **Reflection Integration**: Automatic registration of reflected C++ classes
- **Hot-Reload Support**: Scripts can be reloaded at runtime without restart
- **Error Handling**: Comprehensive error reporting and exception safety
- **Performance**: Minimal overhead with efficient C++/Lua communication

## Architecture

```
┌─────────────────────────────────────┐
│   Lua Scripts (gameplay logic)     │
│   - AI behavior                     │
│   - Item systems                    │
│   - Quest logic                     │
└─────────────────────────────────────┘
              ↓ Sol2 Bindings
┌─────────────────────────────────────┐
│   LuaScriptingEngine (C++)          │
│   - Script loading & execution      │
│   - Type registration               │
│   - Hot-reload management           │
└─────────────────────────────────────┘
              ↓ Integration
┌─────────────────────────────────────┐
│   Reflection System                 │
│   - Automatic type exposure         │
│   - Property access                 │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│   Fresh Engine Core                 │
│   - Game systems                    │
│   - Entity management               │
└─────────────────────────────────────┘
```

## Quick Start

### 1. Initialize the Scripting Engine

```cpp
#include "scripting/lua/LuaScriptingEngine.h"

using namespace fresh::scripting;

// Create and initialize the engine
LuaScriptingEngine luaEngine;
if (!luaEngine.initialize()) {
    std::cerr << "Failed to initialize Lua: " << luaEngine.getLastError() << std::endl;
    return;
}
```

### 2. Load and Execute Scripts

```cpp
// Load a script file
if (luaEngine.loadScript("scripts/example_gameplay.lua")) {
    std::cout << "Script loaded successfully!" << std::endl;
}

// Execute inline Lua code
luaEngine.executeScript(R"(
    print("Hello from Lua!")
    local x = 10
    local y = 20
    print("Sum: " .. (x + y))
)");
```

### 3. Call Lua Functions from C++

```cpp
// Call a simple function
luaEngine.callFunction("onGameStart");

// Call with parameters
luaEngine.callFunction("onPlayerMove", 10.5f, 20.3f, 5.0f);

// Call and get return value
int result = luaEngine.callFunctionWithReturn<int>("calculateDamage", 50, 1.5f);
std::cout << "Calculated damage: " << result << std::endl;
```

### 4. Register C++ Functions for Lua

```cpp
// Register a simple function
luaEngine.registerFunction("spawnEnemy", []() {
    std::cout << "Spawning enemy from Lua!" << std::endl;
    // Spawn enemy logic...
});

// Register function with parameters
luaEngine.registerFunction("damagePlayer", [](int damage) {
    std::cout << "Player takes " << damage << " damage!" << std::endl;
    // Player damage logic...
});
```

### 5. Set and Get Global Variables

```cpp
// Set globals
luaEngine.setGlobal("playerHealth", 100);
luaEngine.setGlobal("gameSpeed", 1.5f);
luaEngine.setGlobal("playerName", "Hero");
luaEngine.setGlobal("debugMode", true);

// Get globals
int health = luaEngine.getGlobal<int>("playerHealth");
std::string name = luaEngine.getGlobal<std::string>("playerName");
```

### 6. Register C++ Objects

```cpp
class Player {
public:
    std::string name;
    int health;
    
    void takeDamage(int amount) {
        health -= amount;
        std::cout << name << " took " << amount << " damage!" << std::endl;
    }
};

Player player;
player.name = "Hero";
player.health = 100;

// Register the player object
luaEngine.registerObject("player", &player);

// Now Lua can access it
luaEngine.executeScript(R"(
    print("Player name: " .. player.name)
    print("Player health: " .. player.health)
    player:takeDamage(25)
)");
```

## Reflection Integration

The Lua engine can automatically register all reflected C++ types:

```cpp
// Register a single reflected type
luaEngine.registerReflectedType("Transform");

// Register all reflected types at once
luaEngine.registerAllReflectedTypes();
```

Once registered, Lua scripts can access properties defined in the reflection system:

```lua
-- Access reflected properties
local pos = transform.position
print("Position: " .. pos.x .. ", " .. pos.y .. ", " .. pos.z)

-- Modify properties
transform.position = {x = 10, y = 20, z = 30}
```

## Hot-Reload Support

Enable hot-reload to automatically reload scripts when they change:

```cpp
// Enable hot-reload
luaEngine.enableHotReload(true);

// In your game loop
void update(float deltaTime) {
    // Check for script changes (call once per frame or less frequently)
    luaEngine.checkForScriptChanges();
    
    // Rest of game logic...
}
```

When a script file is modified, it will be automatically reloaded and re-executed.

## Example Lua Scripts

### Gameplay Logic

```lua
-- example_gameplay.lua

PlayerHealth = {
    current = 100,
    max = 100,
    
    damage = function(self, amount)
        self.current = math.max(0, self.current - amount)
        print("Player took " .. amount .. " damage. Health: " .. self.current)
        
        if self.current == 0 then
            print("Player died!")
        end
    end,
    
    heal = function(self, amount)
        self.current = math.min(self.max, self.current + amount)
        print("Player healed " .. amount .. ". Health: " .. self.current)
    end
}

-- Use from C++:
-- luaEngine.callFunction("PlayerHealth.damage", 25);
```

### Enemy AI

```lua
-- enemy_ai.lua

Enemy = {}
Enemy.__index = Enemy

function Enemy:new(name, health, damage)
    local enemy = {
        name = name or "Enemy",
        health = health or 50,
        damage = damage or 10,
        state = "idle"
    }
    setmetatable(enemy, self)
    return enemy
end

function Enemy:attack(target)
    print(self.name .. " attacks " .. target .. " for " .. self.damage .. " damage!")
    return self.damage
end

function Enemy:takeDamage(amount)
    self.health = self.health - amount
    print(self.name .. " took " .. amount .. " damage. Health: " .. self.health)
    
    if self.health <= 0 then
        self:die()
    end
end

function Enemy:die()
    print(self.name .. " has been defeated!")
    self.state = "dead"
end

-- Create enemies from Lua
local goblin = Enemy:new("Goblin", 50, 15)
local dragon = Enemy:new("Dragon", 200, 35)
```

## Mod Loading

Load mods from directories containing Lua scripts:

```cpp
// Load a mod
if (luaEngine.loadMod("mods/my_mod")) {
    std::cout << "Mod loaded successfully!" << std::endl;
}

// Get list of loaded mods
auto mods = luaEngine.getLoadedMods();
for (const auto& mod : mods) {
    std::cout << "Loaded mod: " << mod << std::endl;
}
```

Mod directory structure:
```
mods/
└── my_mod/
    ├── mod.lua        # Main mod script (loaded first)
    ├── init.lua       # Alternative entry point
    ├── entities.lua   # Custom entities
    └── items.lua      # Custom items
```

## Error Handling

The Lua engine provides comprehensive error handling:

```cpp
if (!luaEngine.executeScript("invalid lua code {{")) {
    std::cerr << "Error: " << luaEngine.getLastError() << std::endl;
    luaEngine.clearError();
}

// Check if engine has errors
if (luaEngine.hasError()) {
    std::cerr << "Lua error occurred: " << luaEngine.getLastError() << std::endl;
}
```

Errors are also logged to stderr with the `[LuaScriptingEngine Error]` prefix.

## Performance Considerations

### Best Practices

1. **Minimize C++/Lua Crossings**: Batch operations when possible
2. **Cache Function References**: Store frequently-called functions
3. **Use Hot-Reload Wisely**: Check for changes at most once per second
4. **Profile Scripts**: Use Lua profiler for performance-critical code

### Example: Efficient Batch Operations

```cpp
// Bad: Multiple crossings
for (int i = 0; i < 1000; i++) {
    luaEngine.callFunction("updateEntity", i);
}

// Good: Single crossing with table
luaEngine.executeScript(R"(
    for i = 1, 1000 do
        updateEntity(i)
    end
)");
```

## Integration with Other Systems

### With Event System

```cpp
// Register event callbacks in Lua
luaEngine.executeScript(R"(
    function onPlayerDied()
        print("Player died event triggered")
        -- Handle player death
    end
)");

// Trigger from C++ event system
eventSystem.subscribe("player_died", [&](const EventData& data) {
    luaEngine.callFunction("onPlayerDied");
});
```

### With ECS

```cpp
// Lua script component
class LuaScriptComponent : public IComponent {
public:
    std::string scriptPath;
    std::string updateFunction;
    
    void update(float deltaTime) {
        luaEngine.callFunction(updateFunction, entityId, deltaTime);
    }
};
```

## Advanced Features

### Custom Type Registration

For types not in the reflection system, use manual registration:

```cpp
// Register a custom class
auto playerType = luaEngine.getLuaState()->new_usertype<Player>("Player",
    sol::constructors<Player()>(),
    "name", &Player::name,
    "health", &Player::health,
    "takeDamage", &Player::takeDamage
);
```

### Table Access

```cpp
// Create a table in Lua
luaEngine.executeScript(R"(
    config = {
        width = 1920,
        height = 1080,
        fullscreen = true
    }
)");

// Access table from C++
auto lua = luaEngine.getLuaState();
sol::table config = (*lua)["config"];
int width = config["width"];
int height = config["height"];
bool fullscreen = config["fullscreen"];
```

## Troubleshooting

### Lua Not Available

If you see "Lua support not compiled in", ensure:
1. Lua library is installed (vcpkg or system package)
2. CMake found Lua (check CMake output)
3. `FRESH_LUA_AVAILABLE` is defined (check build output)

### Script Won't Load

- Check file path is correct and file exists
- Verify Lua syntax (use `luac -p script.lua`)
- Check console for detailed error messages

### Function Call Fails

- Verify function is defined in loaded script
- Check parameter types match
- Ensure function is in global scope or accessed via table

## See Also

- [Reflection System Documentation](REFLECTION_SYSTEM.md)
- [Event System Documentation](../scripting/EventSystem.h)
- [Sol2 Documentation](https://sol2.readthedocs.io/)
- [Lua 5.4 Reference Manual](https://www.lua.org/manual/5.4/)
