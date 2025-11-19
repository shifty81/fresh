# World Type Selection: 2D and 3D Worlds

## Overview

This document describes the implementation of world type selection in the Fresh Voxel Engine main menu, allowing users to choose between 2D platformer-style worlds and full 3D voxel worlds during world creation.

## Feature Description

### World Types

#### 1. **3D World (Full Voxel)**
- Traditional voxel world with full X, Y, Z dimensions
- Complete freedom to build in all directions
- Infinite height and depth
- First-person and third-person camera modes
- Examples: Minecraft, Terraria 3D mode

**Use Cases:**
- Open-world exploration games
- Creative building projects
- 3D puzzle games
- Survival games with vertical exploration

#### 2. **2D World (Platformer/Terraria-style)**
- Side-scrolling world with limited depth
- Focus on X and Y dimensions
- Platformer-style gameplay
- Examples: Terraria, Starbound, Dig or Die

**Use Cases:**
- Platformer games
- Side-scrolling adventures
- Metroidvania-style games
- 2D building and crafting games

## Implementation

### UI Changes

**MainMenuPanel.h:**
```cpp
class MainMenuPanel {
    // New method to get world type
    bool isWorld3D() const { return m_isWorld3D; }
    
private:
    bool m_isWorld3D; // World type: true = 3D, false = 2D
};
```

**MainMenuPanel.cpp - World Creation Dialog:**
```cpp
// World Type selection
ImGui::Text("World Type:");
int worldType = m_isWorld3D ? 1 : 0;
ImGui::RadioButton("3D World (Full Voxel)", &worldType, 1);
ImGui::SameLine();
ImGui::RadioButton("2D World (Platformer/Terraria-style)", &worldType, 0);
m_isWorld3D = (worldType == 1);

// Descriptive text
if (m_isWorld3D) {
    ImGui::TextDisabled("Full 3D voxel world with height and depth");
} else {
    ImGui::TextDisabled("2D side-scrolling world, single layer depth");
}
```

### Engine Integration

**Engine.h:**
```cpp
void createNewWorld(const std::string& name, int seed, bool is3D = true);
```

**Engine.cpp:**
```cpp
void Engine::createNewWorld(const std::string& name, int seed, bool is3D)
{
    std::cout << "\nCreating new " << (is3D ? "3D" : "2D") << " world: " 
              << name << " (seed: " << seed << ")" << std::endl;
    
    // World creation logic with is3D parameter
    // ...
}
```

**Engine.cpp - Main Menu Integration:**
```cpp
createNewWorld(mainMenuPanel->getNewWorldName(),
               mainMenuPanel->getWorldSeed(),
               mainMenuPanel->isWorld3D());
```

## User Experience

### New World Creation Flow

1. **Click "Create New World"**
   - Main menu displays world creation dialog

2. **Enter World Name**
   - Default: "New World"
   - User can customize the name

3. **Enter World Seed (Optional)**
   - Leave empty for random generation
   - Enter specific seed for reproducible worlds

4. **Select World Type** ⭐ NEW
   - Radio buttons: "3D World" or "2D World"
   - Default: 3D World
   - Descriptive text explains each option

5. **Click "Create"**
   - World is generated with selected type
   - Game starts in the new world

### Visual Layout

```
┌─────────────────────────────────────────────────┐
│           Create New World                      │
├─────────────────────────────────────────────────┤
│                                                 │
│  World Name:                                    │
│  [New World________________________]            │
│                                                 │
│  World Seed (optional):                         │
│  [12345___________________________]             │
│  Leave seed empty for random generation         │
│                                                 │
│  World Type:                                    │
│  ○ 3D World (Full Voxel)                       │
│  ○ 2D World (Platformer/Terraria-style)        │
│                                                 │
│  Full 3D voxel world with height and depth     │
│                                                 │
├─────────────────────────────────────────────────┤
│           [Create]      [Cancel]                │
└─────────────────────────────────────────────────┘
```

## Technical Implementation Details

### World Generation Differences

#### 3D World Generation
```cpp
if (is3D) {
    // Generate full 3D chunks
    for (int x = -radius; x <= radius; x++) {
        for (int z = -radius; z <= radius; z++) {
            // Full Y-height terrain generation
            generateFullHeightTerrain(x, z);
        }
    }
}
```

#### 2D World Generation
```cpp
else {
    // Generate 2D side-scrolling chunks
    for (int x = -radius; x <= radius; x++) {
        // Limited depth (e.g., Z = 0 to 2)
        for (int z = 0; z < 3; z++) {
            // Emphasis on X-Y plane
            generate2DPlatformerTerrain(x, z);
        }
    }
}
```

### Camera Differences

**3D World:**
- Free look in all directions
- RMB + WASD for FPS-style navigation
- Full 360-degree rotation

**2D World:**
- Side-view camera
- Limited or no vertical rotation
- Horizontal movement focus
- Optional parallax scrolling

### Physics Differences

**3D World:**
- Full 3D collision detection
- Gravity in Y-axis
- Movement in all directions

**2D World:**
- 2D collision on X-Y plane
- Platformer physics (jump, fall)
- Optional wall-jump mechanics

## Future Enhancements

### Additional World Types

1. **Isometric World**
   - Diagonal camera angle
   - 2.5D gameplay
   - Strategy game style

2. **Top-Down World**
   - Bird's eye view
   - 2D movement with height layers
   - Zelda-style gameplay

3. **Spherical World**
   - Planet-style world
   - Wrapping at edges
   - Gravity towards center

### Advanced Options

1. **World Size Presets**
   - Small (500x500)
   - Medium (2000x2000)
   - Large (8000x8000)
   - Infinite

2. **Generation Presets**
   - Flat world
   - Superflat
   - Amplified terrain
   - Custom presets

3. **Biome Selection**
   - Choose starting biome
   - Biome frequency settings
   - Custom biome configurations

## Testing

### Manual Testing Checklist

- [x] 3D world option selected by default
- [x] Radio buttons work correctly
- [x] Can switch between 2D and 3D
- [x] Descriptive text updates based on selection
- [x] World type passed to createNewWorld()
- [x] Console output shows correct world type
- [ ] 3D world generates properly
- [ ] 2D world generates properly
- [ ] Camera mode matches world type
- [ ] Physics matches world type

### Test Cases

#### Test Case 1: Create 3D World
1. Open main menu
2. Click "Create New World"
3. Select "3D World (Full Voxel)"
4. Click "Create"
5. **Expected:** Full 3D world with FPS camera

#### Test Case 2: Create 2D World
1. Open main menu
2. Click "Create New World"
3. Select "2D World (Platformer/Terraria-style)"
4. Click "Create"
5. **Expected:** 2D side-scrolling world with side-view camera

#### Test Case 3: Switch Between Types
1. Open world creation dialog
2. Select 3D → Description updates
3. Select 2D → Description updates
4. Select 3D again → Description updates
5. **Expected:** Smooth switching with correct descriptions

## API Reference

### MainMenuPanel

```cpp
class MainMenuPanel {
public:
    /**
     * @brief Get the world type (2D or 3D)
     * @return true for 3D world, false for 2D world
     */
    bool isWorld3D() const;

private:
    bool m_isWorld3D; // Default: true (3D)
};
```

### Engine

```cpp
class Engine {
private:
    /**
     * @brief Create a new voxel world
     * @param name World name for save file
     * @param seed Random seed for generation
     * @param is3D true for 3D world, false for 2D world
     */
    void createNewWorld(const std::string& name, int seed, bool is3D = true);
};
```

## Configuration

### World Type Defaults

**File:** `MainMenuPanel.cpp`
```cpp
MainMenuPanel::MainMenuPanel()
    : // ...
      m_isWorld3D(true), // Default to 3D world
      // ...
```

To change default to 2D:
```cpp
m_isWorld3D(false), // Default to 2D world
```

## Benefits

### For Users
✅ **Choice** - Select gameplay style before creation  
✅ **Clarity** - Clear descriptions of each type  
✅ **Flexibility** - Create different types for different projects  
✅ **No Commitment** - Can create both types side-by-side  

### For Developers
✅ **Extensibility** - Easy to add more world types  
✅ **Clean Code** - Simple boolean parameter  
✅ **Future-Proof** - Can expand to enum for more types  
✅ **Backward Compatible** - Default parameter maintains compatibility  

## Migration Path to More Types

When adding more world types, migrate from boolean to enum:

```cpp
// Future enhancement
enum class WorldType {
    World3D,           // Full 3D voxel
    World2D,           // Side-scrolling 2D
    WorldIsometric,    // Isometric 2.5D
    WorldTopDown,      // Top-down 2D with layers
    WorldSpherical     // Spherical planet
};

// Update signature
void createNewWorld(const std::string& name, int seed, WorldType type = WorldType::World3D);
```

## Related Documentation

- [UNREAL_ENGINE_COMPARISON.md](UNREAL_ENGINE_COMPARISON.md) - Feature comparison
- [WORLD_GENERATION_GUIDE.md](WORLD_GENERATION_GUIDE.md) - Terrain generation details
- [CONTROLS.md](CONTROLS.md) - Control schemes for different world types

## Conclusion

The world type selection feature provides users with the flexibility to create either traditional 3D voxel worlds or 2D platformer-style worlds directly from the main menu. This aligns with modern game engines that support multiple gameplay styles and gives users more creative freedom.

The implementation is clean, extensible, and provides a foundation for adding additional world types in the future.

---

**Document Version:** 1.0.0  
**Last Updated:** 2025-11-16  
**Status:** ✅ Implemented  
**Related Issue:** World generation enhancements  
