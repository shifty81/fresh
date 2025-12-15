# Scene Management (Unreal-Style)

## Overview

Fresh Voxel Engine adopts an **Unreal Engine-style scene management** approach, where the editor is the primary application and "scenes" are the fundamental unit of game development. This document explains the scene system and how it relates to the underlying world data.

## Terminology

### Scene vs World

In Fresh, we distinguish between these concepts:

| Term | Description | Technical Implementation |
|------|-------------|-------------------------|
| **Scene** | Editor/user-facing concept - a game level or environment | File on disk (`.scene` or world save) |
| **World** | Engine/code-level concept - the voxel data container | `VoxelWorld` class instance |

**Analogy**: Think of a scene as a "document" in the editor, and the world as the "data model" that the document contains.

### Example:
- User perspective: "I'm working on the 'Castle' scene"
- Code perspective: "The engine loaded a `VoxelWorld` instance from the Castle save file"

## Scene Workflow

### Editor-First Design

Fresh follows an **editor-first workflow** similar to Unreal Engine:

```
Launch Editor → Create/Open Scene → Edit Scene → Save Scene → Test in Play Mode
```

**Not** the old pattern:
```
Launch Game → Main Menu → Create World → Play
```

### Scene Lifecycle

1. **Editor Launch**
   - Editor UI appears immediately
   - No world loaded yet (empty viewport)
   - All editor tools available

2. **Create New Scene** (`File > New Scene`)
   - Opens scene creation dialog
   - Choose scene type (3D Voxel, 2D Platformer, 2D Top-Down, Empty)
   - Select world generator (plugin)
   - Configure seed and options
   - Scene created in editor

3. **Edit Scene**
   - Use editor tools (brush, gizmos, inspector)
   - Place entities, NPCs, structures
   - Modify terrain with terraforming tools
   - Configure scene properties

4. **Save Scene** (`File > Save Scene`)
   - Serializes world data to disk
   - Saves entity data, settings, metadata
   - Scene appears in Open Scene dialog

5. **Test Scene** (`Play > Enter Play Mode`)
   - Switches to game mode
   - Editor UI hidden
   - Test gameplay
   - Press ESC to return to editor

6. **Close Scene** (`File > Close Scene`)
   - Prompt to save unsaved changes
   - Unload world
   - Return to empty editor

## Scene Types

Fresh supports multiple scene types, each suited for different game genres:

### 3D Voxel Scene

**Best for**: Minecraft-style games, 3D building games, voxel adventures

**Features**:
- Full 3D voxel world with chunks
- Procedural terrain generation
- Biomes, caves, structures
- Height-based gameplay
- First-person or third-person camera

**Generator Options**: 3D Terrain, Custom 3D generators

### 2D Platformer Scene

**Best for**: Terraria-style games, side-scrolling platformers

**Features**:
- Side-view 2D voxel world
- Layers and underground caves
- Gravity-based gameplay
- Jump and run mechanics
- Side-scrolling camera

**Generator Options**: 2D Platformer, Custom 2D generators

### 2D Top-Down Scene

**Best for**: Zelda-style games, top-down RPGs, strategy games

**Features**:
- Top-down 2D voxel world
- Ground-level gameplay
- Grid-based movement
- Orthographic camera
- Tactical combat

**Generator Options**: 2D Top-Down, Custom 2D generators

### Empty Scene

**Best for**: Complete manual creation, non-terrain games, puzzles

**Features**:
- Blank voxel world
- No procedural generation
- Full manual control
- Build everything from scratch

**Generator Options**: None (Empty)

## Scene File Format

Scenes are saved as directories containing multiple files:

```
saves/
  MyScene/
    world.dat          # Voxel world data (chunks)
    entities.json      # Entity/NPC data
    scene.json         # Scene metadata
    thumbnail.png      # Scene preview image
```

### scene.json Example

```json
{
  "name": "MyScene",
  "type": "3D Voxel",
  "created": "2025-01-15T10:30:00Z",
  "modified": "2025-01-15T14:45:00Z",
  "generator": "3D Terrain",
  "seed": 12345,
  "is3D": true,
  "gameStyle2D": 0,
  "description": "My awesome game level",
  "tags": ["adventure", "survival"],
  "version": "0.1.0"
}
```

## Scene Operations

### Creating a Scene

From the editor:

1. Click `File > New Scene` (or `Ctrl+N`)
2. Scene Creation Dialog appears:
   - **Name**: Enter scene name
   - **Type**: Select 3D/2D/Empty
   - **Generator**: Choose generation plugin (if not empty)
   - **Seed**: Random or custom seed
   - **Options**: Generator-specific settings

3. Click **Create**
4. Scene generates and opens in editor

### Opening a Scene

From the editor:

1. Click `File > Open Scene` (or `Ctrl+O`)
2. Scene Selection Dialog shows saved scenes:
   - Thumbnail previews
   - Scene name, type, date
   - Tags and description

3. Double-click scene to open
4. Scene loads in editor

### Saving a Scene

Manual save:
- `File > Save Scene` (`Ctrl+S`) - Save current scene
- `File > Save Scene As...` - Save as new scene

Auto-save:
- Editor auto-saves every 5 minutes (configurable)
- Backup saves kept in `saves/.backup/`

### Closing a Scene

Options:
- `File > Close Scene` - Close current scene
- `File > New Scene` - Prompts to save, then creates new
- `File > Open Scene` - Prompts to save, then opens another

Unsaved changes prompt:
```
Scene "MyScene" has unsaved changes.
Save before closing?
[Save] [Don't Save] [Cancel]
```

## Multiple Scene Support (Future)

Future versions will support multiple open scenes:

- **Scene Tabs**: Switch between open scenes with tabs
- **Scene Comparison**: View two scenes side-by-side
- **Scene Merging**: Combine elements from multiple scenes
- **Scene Templates**: Create scene templates for reuse

## Scene Properties

Access via `Scene > Scene Settings`:

### General Properties
- **Name**: Scene display name
- **Description**: Scene description text
- **Tags**: Search tags (e.g., "tutorial", "boss_level")
- **Type**: 3D Voxel, 2D Platformer, 2D Top-Down
- **Read-Only**: Lock scene from editing

### World Properties
- **Generator**: World generator used
- **Seed**: Generation seed value
- **Dimensions**: World size/boundaries
- **Spawn Point**: Player spawn location
- **Time of Day**: Sky lighting (dawn, noon, dusk, night)

### Gameplay Properties
- **Game Mode**: Survival, Creative, Adventure, Custom
- **Difficulty**: Easy, Normal, Hard
- **Rules**: Enable/disable gameplay rules

## Scene Hierarchy

Scenes contain a hierarchy of objects visible in the Scene Hierarchy panel:

```
MyScene
├── World (VoxelWorld)
│   ├── Chunk_0_0
│   ├── Chunk_1_0
│   └── Chunk_0_1
├── Entities
│   ├── Player
│   ├── NPCs
│   │   ├── Villager_001
│   │   └── Trader_001
│   └── Enemies
│       ├── Zombie_001
│       └── Zombie_002
├── Structures
│   ├── House_001
│   └── Bridge_001
└── Lighting
    ├── DirectionalLight (Sun)
    └── PointLight_001
```

## Scene Templates (Future)

Pre-built scene templates for quick start:

| Template | Description |
|----------|-------------|
| **Blank 3D** | Empty 3D voxel world |
| **Survival Island** | Island world with resources |
| **Underground Cave** | Cave system for exploration |
| **Platformer Level** | 2D platformer with platforms |
| **Top-Down Dungeon** | 2D dungeon with rooms |
| **Tutorial Level** | Pre-built tutorial environment |

## Best Practices

### 1. Organize Your Scenes

Use clear naming conventions:
- Good: `Level_01_Castle`, `Tutorial_01_Movement`, `Boss_DragonLair`
- Bad: `Scene1`, `test`, `asdf`

### 2. Use Tags and Descriptions

Make scenes searchable:
```json
{
  "name": "Level_03_Forest",
  "description": "Forest level with tree puzzles and enemy spawns",
  "tags": ["level", "forest", "puzzles", "combat"]
}
```

### 3. Save Frequently

- Use `Ctrl+S` regularly
- Enable auto-save
- Keep backups of important scenes

### 4. Test in Play Mode

- Test gameplay frequently with `F5` (Enter Play Mode)
- Verify spawns, triggers, difficulty
- Exit with `ESC` to return to editing

### 5. Version Control

Scenes are save directories - use version control:
```bash
git add saves/Level_01_Castle/
git commit -m "Added castle interior and boss room"
```

## Technical Details

### Scene Loading Process

1. **Parse scene.json**: Read metadata
2. **Load VoxelWorld**: Deserialize world data
3. **Load Entities**: Create entities from entities.json
4. **Initialize Systems**: Physics, AI, rendering
5. **Camera Setup**: Position camera at spawn point
6. **Editor Ready**: Scene loaded in editor

### Scene Unloading Process

1. **Save Check**: Prompt if unsaved changes
2. **Cleanup Systems**: Stop physics, AI
3. **Unload Entities**: Destroy all entities
4. **Unload World**: Free voxel data
5. **Reset Editor**: Clear hierarchy, inspector
6. **Empty State**: Editor ready for new scene

## Code Example: Creating a Scene

```cpp
// In Engine.cpp
void Engine::createNewScene(const std::string& name, int seed, bool is3D, int gameStyle2D)
{
    LOG_INFO("Creating new scene: " + name);
    
    // Select appropriate world generator
    std::string generatorName;
    if (!is3D && gameStyle2D == 0) {
        generatorName = "2D Platformer";
    } else if (!is3D && gameStyle2D == 1) {
        generatorName = "2D Top-Down";
    } else {
        generatorName = "3D Terrain";
    }
    
    // Create generator instance
    auto generator = WorldGeneratorFactory::createGenerator(generatorName);
    if (!generator) {
        LOG_ERROR("Failed to create generator: " + generatorName);
        return;
    }
    generator->setSeed(seed);
    
    // Create voxel world
    m_world = std::make_unique<VoxelWorld>();
    m_world->initialize();
    m_world->setGenerator(std::move(generator));
    
    // Setup scene metadata
    m_sceneName = name;
    m_sceneSeed = seed;
    m_sceneType = is3D ? "3D Voxel" : (gameStyle2D == 0 ? "2D Platformer" : "2D Top-Down");
    
    // Initialize game systems
    initializeGameSystems();
    
    // Update editor
    m_editorManager->updateWorld(m_world.get());
    m_editorManager->setVisible(true);
    
    LOG_INFO("Scene '" + name + "' created successfully");
}
```

## Summary

Fresh Voxel Engine's scene management system provides:

- ✅ **Editor-first workflow** - Start with editor, not game menu
- ✅ **Unreal-style scenes** - Scenes as primary units of development
- ✅ **Clear terminology** - "Scene" for user, "World" for code
- ✅ **Multiple scene types** - 3D, 2D platformer, 2D top-down, empty
- ✅ **Flexible save system** - Directory-based with metadata
- ✅ **Quick iteration** - Create, edit, test cycle
- ✅ **Professional workflow** - Same approach as industry-standard engines

This makes Fresh a true **game editor platform**, not just a voxel engine.
