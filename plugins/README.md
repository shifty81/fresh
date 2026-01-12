# Fresh Engine Plugins

This directory contains plugin modules for the Fresh Voxel Engine. Plugins extend the engine's functionality without modifying core engine code.

## Plugin Types

### World Generation Plugins (`worldgen/`)
World generation plugins implement the `IWorldGenerator` interface to create different types of procedural worlds:
- 3D terrain generation
- 2D platformer worlds
- 2D top-down worlds
- Custom generation algorithms

Each plugin is self-contained and can be loaded/unloaded at runtime.

### Engine Mode Plugins (`enginemode/`) [Planned]
Engine mode plugins implement the `IEngineMode` interface to switch between rendering and physics modes:
- 3D mode (voxel-based)
- 2D mode (sprite-based)
- Custom rendering modes

## Plugin Structure

Each plugin should have:
```
plugin_name/
├── plugin.json          # Plugin metadata
├── include/             # Public headers
├── src/                 # Implementation
└── README.md            # Plugin documentation
```

## Plugin Metadata (plugin.json)

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "type": "worldgen",
  "description": "Plugin description",
  "author": "Author Name",
  "dependencies": []
}
```

## Creating a Plugin

See [docs/guides/PLUGIN_DEVELOPMENT.md](../docs/guides/PLUGIN_DEVELOPMENT.md) for detailed instructions on creating plugins.

## Loading Plugins

Plugins are automatically discovered and loaded by the engine at startup. The plugin system scans the `plugins/` directory and loads all valid plugins.

## Hot Reload

Plugins support hot reload during development. Changes to plugin code can be recompiled and reloaded without restarting the engine (when in debug mode).
