# Editor Settings Dialog - User Guide

**Complete guide to configuring the Fresh Voxel Engine editor**

## Table of Contents

1. [Overview](#overview)
2. [Opening the Settings Dialog](#opening-the-settings-dialog)
3. [Settings Categories](#settings-categories)
4. [General Settings](#general-settings)
5. [Grid & Snap Settings](#grid--snap-settings)
6. [Tool Settings](#tool-settings)
7. [Performance Settings](#performance-settings)
8. [Saving and Applying Settings](#saving-and-applying-settings)
9. [Configuration File](#configuration-file)
10. [Best Practices](#best-practices)

---

## Overview

The Editor Settings Dialog provides a centralized interface for configuring all aspects of the Fresh Voxel Engine editor. Settings are organized into intuitive tabs and persist across editor sessions.

### Key Features

- ✅ Tabbed interface for easy navigation
- ✅ Real-time preview of changes
- ✅ Automatic persistence to configuration file
- ✅ Reset to defaults option
- ✅ Apply/OK/Cancel workflow
- ✅ Visual indication of modified settings

---

## Opening the Settings Dialog

### Via Menu

1. Click **Settings** menu in the menu bar
2. Select **Editor Settings...**

### Via Keyboard

Currently no keyboard shortcut assigned. Recommended shortcut: `Ctrl+,` (can be added in future update)

---

## Settings Categories

The dialog is organized into four main tabs:

1. **General** - Auto-save and UI appearance settings
2. **Grid & Snap** - Grid display and snapping behavior
3. **Tools** - Default tool configurations
4. **Performance** - Frame rate and rendering settings

---

## General Settings

### Auto-Save

**Enable Auto-Save**
- **Type:** Checkbox
- **Default:** Enabled
- **Description:** Automatically save the world at regular intervals
- **Tip:** Keep enabled to prevent data loss

**Auto-Save Interval (minutes)**
- **Type:** Slider (1-30)
- **Default:** 5 minutes
- **Description:** How often to auto-save the world
- **Recommendation:** 
  - Large worlds: 10-15 minutes
  - Small worlds: 3-5 minutes

### UI Appearance

**UI Scale**
- **Type:** Slider (0.5-2.0)
- **Default:** 1.0
- **Description:** Scale of the entire UI
- **Note:** Requires restart to take effect
- **Use Cases:**
  - High DPI displays: 1.5-2.0
  - Standard displays: 1.0
  - Smaller UI: 0.75-0.9

**Font Size**
- **Type:** Slider (10-24)
- **Default:** 16
- **Description:** Size of UI text
- **Note:** Requires restart to take effect
- **Recommendation:**
  - 4K displays: 18-20
  - 1080p displays: 14-16
  - Accessibility: 18-24

**Show Tooltips**
- **Type:** Checkbox
- **Default:** Enabled
- **Description:** Display helpful tooltips when hovering over UI elements
- **Tip:** Keep enabled for learning the editor

---

## Grid & Snap Settings

### Grid Display

**Show Grid**
- **Type:** Checkbox
- **Default:** Enabled
- **Description:** Display the editor grid in the viewport
- **Tip:** Disable for cleaner viewport when building

**Grid Size**
- **Type:** Slider (0.1-10.0)
- **Default:** 1.0
- **Description:** Size of each grid cell
- **Use Cases:**
  - Fine detail work: 0.5-1.0
  - Large structures: 5.0-10.0

**Grid Subdivisions**
- **Type:** Slider (1-20)
- **Default:** 10
- **Description:** Number of subdivisions per grid cell
- **Tip:** More subdivisions = more precise placement

**Grid Opacity**
- **Type:** Slider (0.0-1.0)
- **Default:** 0.3
- **Description:** Transparency of the grid lines
- **Use Cases:**
  - Subtle grid: 0.1-0.3
  - Prominent grid: 0.5-0.8

### Snap Settings

**Snap to Grid**
- **Type:** Checkbox
- **Default:** Disabled
- **Description:** Snap object positions to grid when moving
- **Keyboard Override:** Hold Ctrl to temporarily enable/disable

**Snap Value**
- **Type:** Slider (0.1-10.0)
- **Default:** 1.0
- **Description:** Snap distance for position snapping
- **Tip:** Match this to your grid size for aligned placement

**Snap Rotation**
- **Type:** Checkbox
- **Default:** Disabled
- **Description:** Snap object rotations to fixed angles
- **Use Cases:** 
  - Enable for architectural precision
  - Disable for organic placement

**Rotation Snap (degrees)**
- **Type:** Slider (1-90)
- **Default:** 15 degrees
- **Description:** Snap angle for rotation snapping
- **Common Values:**
  - 90°: Right angles only
  - 45°: Diagonal angles
  - 15°: Fine rotation control
  - 5°: Very precise rotation

---

## Tool Settings

### Default Tool Settings

**Default Brush Size**
- **Type:** Slider (1-20)
- **Default:** 3
- **Description:** Default size for terraforming brushes
- **Use Cases:**
  - Detail work: 1-3
  - General building: 5-8
  - Large terrain: 10-20

**Default Brush Shape**
- **Type:** Dropdown
- **Options:** Sphere, Cube, Cylinder
- **Default:** Sphere
- **Description:** Default shape for terraforming brushes
- **Use Cases:**
  - Sphere: Natural terrain, hills
  - Cube: Buildings, structures
  - Cylinder: Pillars, towers

### Camera Settings

**Camera Speed**
- **Type:** Slider (1.0-50.0)
- **Default:** 10.0
- **Description:** Normal camera movement speed
- **Tip:** Adjust based on your mouse sensitivity

**Fast Camera Speed**
- **Type:** Slider (10.0-100.0)
- **Default:** 30.0
- **Description:** Camera movement speed when holding Shift
- **Use Cases:**
  - Quick navigation: 50-100
  - Controlled movement: 20-40

---

## Performance Settings

**V-Sync**
- **Type:** Checkbox
- **Default:** Enabled
- **Description:** Synchronize frame rate with monitor refresh rate
- **Note:** Requires restart to take effect
- **Recommendation:**
  - Enable: Smoother rendering, less screen tearing
  - Disable: Higher FPS, more responsive (if GPU can handle it)

**Target FPS**
- **Type:** Slider (30-144)
- **Default:** 60
- **Description:** Target frame rate (0 = unlimited)
- **Use Cases:**
  - 60 FPS: Standard, good balance
  - 120/144 FPS: High refresh rate monitors
  - 30 FPS: Lower-end hardware

---

## Saving and Applying Settings

### Dialog Buttons

**Apply**
- Applies changes without closing the dialog
- Settings are saved to configuration file
- Callbacks notify other systems of changes

**OK**
- Applies changes and closes the dialog
- Equivalent to Apply + Close

**Cancel**
- Discards all changes since dialog was opened
- Reverts to previously saved settings

**Reset to Defaults**
- Resets all settings to their default values
- Does not automatically apply (click Apply or OK to confirm)

### Settings Modified Indicator

When settings are changed, a yellow text appears at the bottom:
```
* Settings have been modified
```

This reminds you to click Apply or OK to save your changes.

---

## Configuration File

### File Location

Settings are stored in:
```
configs/editor_settings.ini
```

### File Format

The configuration file uses INI format for human readability:

```ini
[AutoSave]
enabled=1
intervalMinutes=5

[Grid]
show=1
size=1.0
subdivisions=10
opacity=0.3

[Snap]
snapToGrid=0
snapValue=1.0
snapRotation=0
snapRotationDegrees=15.0

[UI]
scale=1.0
fontSize=16
showTooltips=1

[Tools]
defaultBrushSize=3
defaultBrushShape=0

[Camera]
defaultSpeed=10.0
defaultFastSpeed=30.0

[Performance]
vsync=1
targetFPS=60
```

### Manual Editing

You can manually edit the configuration file if needed:

1. Close the editor
2. Open `configs/editor_settings.ini` in a text editor
3. Modify values
4. Save the file
5. Restart the editor

**Warning:** Invalid values may cause the editor to use defaults.

---

## Best Practices

### For New Users

1. **Keep defaults initially**: The default settings work well for most users
2. **Enable tooltips**: Learn what each setting does
3. **Enable auto-save**: Prevent data loss
4. **Start with small grid**: Use grid size 1.0 for learning

### For Advanced Users

**Performance Optimization:**
- Disable V-Sync if you have a powerful GPU
- Reduce grid subdivisions if viewport is slow
- Lower grid opacity to reduce visual clutter

**Precision Work:**
- Enable snap to grid
- Use grid size 0.5 for fine detail
- Enable snap rotation with 5-degree increments
- Increase grid subdivisions to 20

**Large Projects:**
- Increase auto-save interval to 10-15 minutes
- Use larger grid size (5.0-10.0)
- Increase default brush size to 10+
- Increase camera speeds for faster navigation

### Workflow-Specific Settings

**Architectural Building:**
```
- Snap to Grid: Enabled
- Snap Value: 1.0
- Snap Rotation: Enabled
- Rotation Snap: 90 degrees
- Default Brush Shape: Cube
```

**Organic Terrain:**
```
- Snap to Grid: Disabled
- Default Brush Shape: Sphere
- Default Brush Size: 8-12
```

**Detail Work:**
```
- Grid Size: 0.5
- Grid Subdivisions: 15-20
- Default Brush Size: 1-2
- Camera Speed: 5.0 (slower)
```

---

## Troubleshooting

### Settings Not Saving

1. Check that `configs/` directory exists
2. Ensure the editor has write permissions
3. Check console for error messages
4. Try clicking "Apply" before closing dialog

### UI Scale Not Applying

- UI Scale requires **editor restart** to take effect
- Close and reopen the editor after changing

### Grid Not Visible

1. Check "Show Grid" is enabled
2. Increase grid opacity if too faint
3. Ensure grid isn't behind geometry
4. Check camera is in editor mode, not play mode

### Snap Not Working

1. Verify "Snap to Grid" is enabled
2. Check snap value matches your movement scale
3. Try holding Ctrl (temporary toggle)
4. Ensure transform gizmo is in use

---

## Future Enhancements

Planned features for future versions:

- **Keyboard Shortcuts:** Assign hotkeys to toggle settings
- **Themes:** Multiple color theme presets
- **Profiles:** Save/load complete settings profiles
- **Per-Project Settings:** Different settings for different projects
- **Advanced Rendering:** Shadow quality, anti-aliasing settings
- **Plugin Settings:** Configuration for editor plugins

---

## Related Documentation

- [Layout Management Guide](LAYOUT_MANAGEMENT.md) - Workspace layouts
- [GUI System Guide](GUI_GUIDE.md) - Complete GUI documentation
- [Editor Integration](EDITOR_INTEGRATION.md) - Editor architecture
- [Controls Guide](../guides/CONTROLS.md) - Input controls

---

**Last Updated:** 2025-11-19  
**Version:** 1.0  
**Status:** Production Ready
