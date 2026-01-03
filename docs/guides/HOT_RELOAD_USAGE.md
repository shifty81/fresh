# Hot-Reload Support for Lua Scripts

## Overview

The Fresh Voxel Engine now includes automatic hot-reload support for Lua scripts. When enabled, the engine monitors loaded Lua files for changes and automatically reloads them when modifications are detected.

## Features

- **Automatic File Watching**: Scripts are tracked for modifications
- **Zero-Restart Development**: Make changes to scripts without restarting the engine
- **Safe Reloading**: Scripts are reloaded in a controlled manner
- **Development Mode**: Hot-reload can be enabled/disabled as needed

## How It Works

### Engine Integration

The hot-reload system is integrated directly into the Engine's update loop:

```cpp
// In Engine::update()
if (m_scriptingEngine && m_scriptingEngine->isHotReloadEnabled()) {
    m_scriptingEngine->checkForScriptChanges();
}
```

### File Tracking

When a script is loaded with hot-reload enabled:

1. The script filepath is recorded
2. The file's last modification timestamp is stored
3. On each update cycle, timestamps are checked
4. If a file has changed, it's automatically reloaded

## Usage

### Enabling Hot-Reload

Hot-reload is automatically enabled in development builds when the engine initializes:

```cpp
m_scriptingEngine->enableHotReload(true);
```

### Loading Scripts

Simply load scripts as normal - they will be automatically tracked if hot-reload is enabled:

```cpp
// From C++
m_scriptingEngine->loadScript("scripts/gameplay.lua");
```

### Making Changes

1. Open your Lua script in an editor
2. Make your changes and save the file
3. The engine will detect the change on the next frame
4. The script is automatically reloaded
5. Continue testing immediately

## Example Workflow

### Initial Script (scripts/player_controller.lua)

```lua
function on_player_move(x, y, z)
    log("Player moved to: " .. x .. ", " .. y .. ", " .. z)
end
```

### Updated Script (without restarting the engine)

```lua
function on_player_move(x, y, z)
    log("Player is now at coordinates: " .. x .. ", " .. y .. ", " .. z)
    
    -- New feature added without restarting!
    if y < 0 then
        warn("Player went below ground!")
    end
end
```

The engine automatically picks up the changes and reloads the script.

## Configuration

### Enable/Disable at Runtime

```cpp
// Enable hot-reload
engine->getScriptingEngine()->enableHotReload(true);

// Disable hot-reload (e.g., for production builds)
engine->getScriptingEngine()->enableHotReload(false);

// Check if enabled
bool enabled = engine->getScriptingEngine()->isHotReloadEnabled();
```

### Manual Reload

You can also manually trigger a reload:

```cpp
engine->getScriptingEngine()->reloadScript("scripts/my_script.lua");
```

## Best Practices

### 1. Use for Development Only

Hot-reload is designed for development. In production builds, disable it for better performance:

```cpp
#ifdef NDEBUG
    m_scriptingEngine->enableHotReload(false);  // Production
#else
    m_scriptingEngine->enableHotReload(true);   // Development
#endif
```

### 2. Handle State Preservation

When a script is reloaded, all of its state is reset. To preserve state:

**Option A: Use Global Variables**

```lua
-- Store important state in engine globals
if player_score == nil then
    player_score = 0
end

function add_score(points)
    player_score = player_score + points
end
```

**Option B: Implement State Save/Load**

```lua
-- Called before reload
function on_before_reload()
    save_state("player_score", player_score)
end

-- Called after reload
function on_after_reload()
    player_score = load_state("player_score") or 0
end
```

### 3. Check for Errors

Hot-reload will show errors in the console if a script fails to reload:

```
[LuaScriptingEngine] Detected change in: scripts/gameplay.lua
[LuaScriptingEngine] Reloading script: scripts/gameplay.lua
[LuaScriptingEngine Error] Failed to load script 'scripts/gameplay.lua': [string "scripts/gameplay.lua"]:5: unexpected symbol near ')'
```

Fix the error and save again - the engine will try to reload automatically.

### 4. Organize Scripts

Keep scripts organized for easier hot-reload workflow:

```
scripts/
├── core/
│   ├── init.lua          # Core initialization
│   └── utils.lua         # Utility functions
├── gameplay/
│   ├── player.lua        # Player logic
│   ├── enemies.lua       # Enemy AI
│   └── items.lua         # Item interactions
└── world/
    ├── generation.lua    # World generation
    └── weather.lua       # Weather system
```

## Technical Details

### File Modification Checking

The system uses platform-specific file stat APIs to check modification times:

```cpp
std::time_t getFileModificationTime(const std::string& filepath) {
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) == 0) {
        return fileInfo.st_mtime;
    }
    return 0;
}
```

### Performance

- **Minimal Overhead**: File stat checks are very fast (microseconds)
- **Per-Frame Check**: Scripts are checked every frame for changes
- **Lazy Reload**: Only changed files are reloaded

### Thread Safety

The hot-reload system is designed to be called from the main thread only. Do not call `checkForScriptChanges()` from multiple threads.

## Troubleshooting

### Script Changes Not Detected

**Issue**: Changes to script are saved but not reflected in the engine.

**Solutions**:
1. Check if hot-reload is enabled: `isHotReloadEnabled()`
2. Verify the script was loaded with the full correct path
3. Check console for reload errors
4. Ensure the file is actually being modified (not just opened)

### Frequent Reloads

**Issue**: Script is reloading multiple times per second.

**Solutions**:
1. Check if your editor is creating multiple save operations
2. Disable auto-save in your editor
3. Use manual save (Ctrl+S) instead

### State Lost on Reload

**Issue**: All script variables are reset when reloading.

**Solution**: This is expected behavior. Implement state preservation as described in Best Practices.

## Future Enhancements

Planned improvements for the hot-reload system:

- [ ] Hot-reload for scene files (`.scene` JSON files)
- [ ] Hot-reload notification UI in the editor
- [ ] Selective reload (skip unchanged functions)
- [ ] State preservation helpers
- [ ] Reload hooks (on_before_reload, on_after_reload)
- [ ] Watch multiple script directories
- [ ] Configurable check interval (reduce checks in production)

## Example Integration

See the full example in `examples/scripts/hot_reload_demo.lua`:

```lua
-- Hot-reload demo script
-- Try editing this file while the engine is running!

local demo_state = {
    counter = 0,
    last_message = ""
}

function on_update(deltaTime)
    demo_state.counter = demo_state.counter + 1
    
    if demo_state.counter % 60 == 0 then
        log("Hot-reload demo running! Counter: " .. demo_state.counter)
        -- Try changing this message and saving the file!
    end
end

function on_key_press(key)
    if key == "R" then
        log("Reload key pressed! Try editing this message!")
    end
end

log("Hot-reload demo script loaded!")
```

## See Also

- [Lua Scripting Integration Guide](LUA_SCRIPTING_INTEGRATION.md)
- [Lua API Reference](LUA_API_REFERENCE.md)
- [Scene Serialization](SCENE_SERIALIZATION.md)

---

**Version:** 1.0  
**Last Updated:** 2026-01-03  
**Status:** Production Ready
