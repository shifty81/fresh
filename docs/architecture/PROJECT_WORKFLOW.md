# Project-Based Workflow (Unreal Engine 5 Style)

## Overview

Fresh Engine has been redesigned to use a **project-based workflow** similar to Unreal Engine 5. This document explains the new workflow and how it differs from the previous scene-centric approach.

## What Changed?

### Old Workflow (Scene-Centric)
```
Launch Editor → File > New Scene → Create World → Edit → Save
```
- **Problem**: You always had to create a world/scene immediately
- **Problem**: No top-level container for managing multiple assets
- **Problem**: World-first approach didn't match industry standards

### New Workflow (Project-Centric)
```
Launch Editor → File > New Project → Choose Template → Start Creating
```
- **Better**: Create a project first, then add content as needed
- **Better**: Project contains everything: worlds, assets, scripts, config
- **Better**: Matches Unreal Engine 5 workflow that developers expect

## Concepts

### Project
A **Project** is the top-level container for your entire game:
- **Contains**: Assets, worlds/levels, scripts, configuration
- **Stored as**: A directory with a `.freshproj` file
- **Similar to**: Unreal Engine's `.uproject` file

### Project Structure
```
MyGame/                          # Project directory
├── MyGame.freshproj             # Project file (metadata)
├── Content/                     # All game content
│   ├── Maps/                    # Worlds/levels
│   ├── Assets/                  # Imported assets
│   └── Scripts/                 # Lua scripts
├── Config/                      # Configuration files
└── Saved/                       # Save games, logs
```

### Templates
When creating a new project, you choose a **template**:
- **Blank**: Empty project, start from scratch
- **3D Voxel Game**: Minecraft-style template (world generation ready)
- **2D Platformer**: Terraria-style template (side-scrolling)
- **2D Top-Down**: Zelda-style template (overhead view)

**Note**: Templates configure the project but don't create worlds immediately. You create worlds within the project as needed.

## Getting Started

### Creating a New Project

1. **Launch Fresh Engine**
   ```bash
   Release\FreshVoxelEngine.exe
   ```

2. **Create New Project**
   - Click `File > New Project...` (or press `Ctrl+N`)
   - Enter project name (e.g., "MyGame")
   - Choose template:
     - **Blank** - Empty canvas
     - **3D Voxel Game** - Pre-configured for 3D voxel gameplay
     - **2D Platformer** - Pre-configured for side-scrolling
     - **2D Top-Down** - Pre-configured for overhead view
   - Click "Create"

3. **Project Created**
   - Editor opens with blank project
   - Viewport shows empty scene (sky blue)
   - All editor tools available
   - No world exists yet - that's intentional!

### Adding a World to Your Project

A **world** (or level) is created within a project as needed:

1. **Create New Level**
   - Click `File > New Level...` (or press `Ctrl+Shift+N`)
   - Choose world generator:
     - **3D Terrain** - Procedural 3D terrain with biomes
     - **2D Platformer** - Side-scrolling terrain
     - **2D Top-Down** - Overhead world
     - **Empty** - Blank world for manual building
   - Configure seed and settings
   - Click "Create"

2. **World Generated**
   - World renders in viewport
   - Can create multiple worlds within same project
   - Each world can be saved independently

### Opening an Existing Project

1. **Open Project**
   - Click `File > Open Project...` (or press `Ctrl+O`)
   - Browse to project directory
   - Select `.freshproj` file
   - Click "Open"

2. **Project Loaded**
   - Project settings restored
   - Can load any worlds within project
   - Assets and scripts available

## Menu Structure

### File Menu (Updated)

The File menu now reflects the project-first workflow:

```
File
├── New Project...         (Ctrl+N)          # Create new project
├── Open Project...        (Ctrl+O)          # Open existing project
├── ───────────────
├── New Level...           (Ctrl+Shift+N)   # Create world within project
├── Open Level...          (Ctrl+Shift+O)   # Load world within project
├── ───────────────
├── Save                   (Ctrl+S)          # Save current world
├── Save As...             (Ctrl+Shift+S)   # Save world with new name
├── Save All               (Ctrl+Shift+A)   # Save project + all worlds
├── ───────────────
├── Import...              (Ctrl+I)          # Import assets
├── Export...              (Ctrl+E)          # Export content
├── ───────────────
├── Project Settings...                      # Configure project
├── ───────────────
└── Exit                   (Alt+F4)          # Close editor
```

## Workflow Examples

### Example 1: Blank Project → Manual Building

**Goal**: Create a project and build everything manually

1. `File > New Project...`
   - Name: "ManualCity"
   - Template: **Blank**
   - Click "Create"

2. `File > New Level...`
   - Generator: **Empty**
   - Click "Create"

3. Start building:
   - Use Construction Hammer to place blocks
   - Use Transform Gizmo to arrange objects
   - No procedural generation, full control

### Example 2: 3D Voxel Game Template

**Goal**: Create a Minecraft-style game quickly

1. `File > New Project...`
   - Name: "MyVoxelGame"
   - Template: **3D Voxel Game**
   - Click "Create"

2. `File > New Level...`
   - Generator: **3D Terrain**
   - Seed: 12345
   - Click "Create"

3. World generated:
   - Procedural terrain with biomes
   - Hills, caves, water
   - Ready for gameplay

### Example 3: Multi-Level Game

**Goal**: Create a game with multiple levels

1. Create project:
   - `File > New Project...`
   - Name: "Adventure"
   - Template: **Blank**

2. Create Level 1:
   - `File > New Level...`
   - Name: "Level1"
   - Generator: **3D Terrain**
   - `File > Save` → Save as "Level1"

3. Create Level 2:
   - `File > New Level...`
   - Name: "Level2"
   - Generator: **3D Terrain** (different seed)
   - `File > Save` → Save as "Level2"

4. Switch between levels:
   - `File > Open Level...`
   - Select "Level1" or "Level2"

## Benefits of Project-Based Workflow

### 1. Industry Standard
- Matches Unreal Engine 5, Unity, Godot
- Developers familiar with these engines will feel at home
- Reduces learning curve

### 2. Better Organization
- All game content in one place
- Clear project structure
- Easy to share and version control

### 3. Flexibility
- Start with blank canvas
- Add content incrementally
- No forced world generation

### 4. Scalability
- Support for multiple levels/worlds
- Manage large games with many assets
- Build/packaging system ready for future

### 5. Template System
- Quick start with pre-configured projects
- Community can create custom templates
- Marketplace support in future

## Comparison to Unreal Engine 5

| Feature | Fresh Engine | Unreal Engine 5 |
|---------|-------------|-----------------|
| Project file | `.freshproj` | `.uproject` |
| Content directory | `Content/` | `Content/` |
| Levels/Maps | `Content/Maps/` | `Content/Maps/` |
| Assets | `Content/Assets/` | `Content/Assets/` |
| Scripts | `Content/Scripts/` | `Content/Scripts/` |
| Config | `Config/` | `Config/` |
| Templates | Blank, 3D Voxel, 2D Platformer, 2D Top-Down | Blank, First Person, Third Person, etc. |
| New Project | `Ctrl+N` | File > New Project |
| Open Project | `Ctrl+O` | File > Open Project |
| New Level | `Ctrl+Shift+N` | File > New Level |

## Migration from Old System

### For Existing Worlds

If you have existing worlds from the old scene-based system:

1. Create a new project:
   - `File > New Project...`
   - Choose appropriate template

2. Import old world:
   - `File > Import...`
   - Select old world save file
   - Imports into `Content/Maps/`

3. Open imported world:
   - `File > Open Level...`
   - Select imported world

### Converting Old Projects

Old "worlds" become "levels" within new projects:

**Before**:
```
saves/
└── MyWorld.world
```

**After**:
```
Projects/
└── MyGame/
    ├── MyGame.freshproj
    └── Content/
        └── Maps/
            └── MyWorld.world
```

## FAQ

### Q: Do I always need to create a project?
**A**: Yes. Projects are now the primary way to work in Fresh Engine. This provides better organization and matches industry standards.

### Q: Can I create a world without a project?
**A**: No. Worlds (levels) are created within projects. However, you can use the **Blank** template to create an empty project quickly.

### Q: What happened to "New Scene"?
**A**: "New Scene" has been replaced with "New Level". The functionality is the same - it creates a world within your project.

### Q: Can I have multiple worlds in one project?
**A**: Yes! Projects can contain multiple levels/worlds. Use `File > New Level...` to create additional worlds, and `File > Open Level...` to switch between them.

### Q: What's the difference between a template and a generator?
**A**: 
- **Template**: Initial project setup (Blank, 3D Voxel Game, etc.)
- **Generator**: Creates terrain within a level (3D Terrain, 2D Platformer, etc.)

### Q: Why can't I build the game immediately after creating a blank project?
**A**: Blank projects start empty intentionally. This gives you full control. Create a level with `File > New Level...` to add content.

### Q: How do I share my project?
**A**: Share the entire project directory. Others can open it with `File > Open Project...` and select your `.freshproj` file.

## Next Steps

- **Learn About Templates**: See [TEMPLATES.md](TEMPLATES.md) for template system details
- **World Generation**: See [WORLD_GENERATION_GUIDE.md](../guides/WORLD_GENERATION_GUIDE.md) for generator options
- **Project Settings**: See [PROJECT_SETTINGS.md](PROJECT_SETTINGS.md) for configuration options
- **Build System**: See [BUILD_AND_PACKAGING.md](BUILD_AND_PACKAGING.md) (coming soon)

## See Also

- [SCENE_MANAGEMENT.md](SCENE_MANAGEMENT.md) - Scene (world) management within projects
- [Unreal Engine 5 Documentation](https://docs.unrealengine.com/5.0/) - Inspiration for this workflow
- [ROADMAP.md](../status/ROADMAP.md) - What's coming next
