# Lua Scripting Integration - Implementation Guide

## Overview

The Fresh Voxel Engine now includes full Lua scripting support using **Sol2**, a modern C++ binding library for Lua. This enables modding, scripting gameplay logic, and extending the engine without recompiling C++ code.

## Dependencies

- **LuaJIT**: Fast Just-In-Time compiler for Lua (already included)
- **Sol2**: Modern C++ to Lua binding library (header-only)

## Installation

The dependencies are managed through vcpkg and should be automatically installed during project setup:

```bash
vcpkg install luajit:x64-windows sol2:x64-windows
```

## Features

### ✅ Implemented

1. **Script Execution**
   - Execute Lua code directly from strings
   - Load and execute Lua files
   - Call Lua functions from C++

2. **Global Variable Access**
   - Set/get integers, floats, and strings
   - Bidirectional data exchange between C++ and Lua

3. **Function Registration**
   - Register C++ functions for Lua to call
   - Lambda support for inline callbacks

4. **Mod Loading System**
   - Load mods from directories
   - Automatic detection of init.lua or mod.lua
   - Track loaded mods

5. **Error Handling**
   - Comprehensive exception handling
   - Detailed error messages
   - Last error tracking

## Usage Examples

### Basic Script Execution

```cpp
#include "scripting/lua/ScriptingEngine.h"

using namespace fresh::scripting;

ScriptingEngine engine;
engine.initialize();

// Execute Lua code
engine.executeScript("x = 10 + 20");

// Get result
int x = engine.getGlobalInt("x");  // x = 30
```

### Loading Script Files

```cpp
// Load and execute a Lua file
if (engine.loadScript("scripts/example.lua")) {
    std::cout << "Script loaded successfully" << std::endl;
} else {
    std::cerr << "Error: " << engine.getLastError() << std::endl;
}
```

### Calling Lua Functions

```cpp
// Define function in Lua
engine.executeScript(R"(
    function greet(name)
        return "Hello, " .. name
    end
)");

// Call it from C++
engine.callFunction("greet");
```

### Registering C++ Functions

```cpp
// Register a C++ function for Lua to call
engine.registerFunction("log_message", []() {
    std::cout << "Called from Lua!" << std::endl;
});

// Call from Lua
engine.executeScript("log_message()");
```

### Setting and Getting Variables

```cpp
// Set variables from C++
engine.setGlobal("player_health", 100);
engine.setGlobal("player_name", std::string("Hero"));
engine.setGlobal("gravity", -9.81f);

// Get variables from Lua
int health = engine.getGlobalInt("player_health");
std::string name = engine.getGlobalString("player_name");
float gravity = engine.getGlobalFloat("gravity");
```

### Advanced: Registering C++ Classes

```cpp
#include "scripting/lua/ScriptingEngineImpl.h"

// Define a C++ class
class Player {
public:
    int health;
    std::string name;
    
    void takeDamage(int amount) {
        health -= amount;
    }
};

// Register the class
sol::state* lua = engine.getState();
lua->new_usertype<Player>("Player",
    "health", &Player::health,
    "name", &Player::name,
    "take_damage", &Player::takeDamage
);

// Use in Lua
engine.executeScript(R"(
    player = Player.new()
    player.health = 100
    player.name = "Hero"
    player:take_damage(10)
    print("Health: " .. player.health)  -- Prints: Health: 90
)");
```

## Mod System

### Mod Structure

A mod directory should contain an `init.lua` or `mod.lua` file:

```
mods/
  my_mod/
    init.lua          # Main mod script
    config.json       # Mod metadata (optional)
    scripts/          # Additional scripts
    assets/           # Mod assets
```

### Loading Mods

```cpp
// Load a mod
if (engine.loadMod("mods/my_mod")) {
    std::cout << "Mod loaded!" << std::endl;
}

// Get list of loaded mods
auto mods = engine.getLoadedMods();
for (const auto& mod : mods) {
    std::cout << "Loaded: " << mod << std::endl;
}
```

### Example Mod (init.lua)

```lua
-- mods/example_mod/init.lua

print("Example Mod Loading...")

-- Mod configuration
MOD_NAME = "Example Mod"
MOD_VERSION = "1.0.0"

-- Mod initialization
function init()
    print("Initializing " .. MOD_NAME .. " v" .. MOD_VERSION)
    -- Register custom voxel types
    -- Add custom recipes
    -- Hook into game events
end

-- Mod update (called each frame)
function update(deltaTime)
    -- Update mod logic
end

print("Example Mod Loaded Successfully!")
```

## Engine Integration

### Binding Engine Systems

To allow Lua scripts to interact with engine systems, you need to register them:

```cpp
// Example: Binding VoxelWorld
sol::state* lua = engine.getState();

lua->new_usertype<VoxelWorld>("VoxelWorld",
    "get_voxel", &VoxelWorld::getVoxel,
    "set_voxel", &VoxelWorld::setVoxel,
    "get_chunk", &VoxelWorld::getChunk
);

// Register global instance
(*lua)["world"] = worldInstance;

// Now Lua can access it:
// voxel = world:get_voxel(10, 64, 10)
```

### Event System Integration

```cpp
// Register event callbacks
engine.registerFunction("on_player_move", [](float x, float y, float z) {
    // Handle player movement
});

// Lua script can call:
// on_player_move(10.0, 64.0, 10.0)
```

## Testing

Unit tests are provided in `tests/scripting/test_scripting_engine.cpp`. Run them with:

```bash
cd build
ctest -R test_scripting_engine
```

## Error Handling

The scripting engine provides comprehensive error handling:

```cpp
if (!engine.executeScript("invalid lua code")) {
    std::string error = engine.getLastError();
    std::cerr << "Lua error: " << error << std::endl;
}
```

Errors are also logged to the console with the `[ScriptingEngine Error]` prefix.

## Performance Considerations

1. **Use LuaJIT**: The project uses LuaJIT for better performance
2. **Minimize C++/Lua Boundaries**: Batch operations when possible
3. **Cache Lua Functions**: Store function references instead of looking them up repeatedly
4. **Profile Scripts**: Use Lua profiling tools to identify bottlenecks

## Best Practices

1. **Error Handling**: Always check return values and handle errors
2. **Resource Management**: Clean up Lua resources properly
3. **Security**: Validate and sanitize user scripts if allowing user-generated content
4. **Documentation**: Document your Lua API for mod creators
5. **Testing**: Write tests for both C++ and Lua code

## Future Enhancements

### Planned Features

- [ ] Hot-reload support (automatically reload scripts on file change)
- [ ] Lua debugger integration
- [ ] More engine systems exposed to Lua
- [ ] Complete API documentation
- [ ] Example mods and tutorials
- [ ] Mod dependency management
- [ ] Lua standard library restrictions for security

## Resources

- [Sol2 Documentation](https://sol2.readthedocs.io/)
- [Lua 5.1 Reference Manual](https://www.lua.org/manual/5.1/)
- [LuaJIT](https://luajit.org/)
- [Fresh Engine Docs](../../README.md)

## Troubleshooting

### Build Errors

**Problem**: Sol2 not found during build
```
Solution: Ensure Sol2 is installed via vcpkg:
vcpkg install sol2:x64-windows
```

**Problem**: Linking errors with LuaJIT
```
Solution: Clean and rebuild the project:
cmake --build build --clean-first
```

### Runtime Errors

**Problem**: "Lua state not initialized"
```
Solution: Call engine.initialize() before using any scripting features
```

**Problem**: "Function not found"
```
Solution: Ensure the Lua script defining the function was executed before calling it
```

## License

The Lua scripting integration follows the same MIT license as the Fresh Voxel Engine.

---

**Last Updated**: 2026-01-03
**Status**: ✅ Fully Implemented
**Version**: 1.0.0
