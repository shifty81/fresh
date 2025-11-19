# Camera Controller - Enhanced Editor Camera Features

**Complete guide to the Fresh Editor Camera Controller system**

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [View Modes](#view-modes)
4. [Keyboard Shortcuts](#keyboard-shortcuts)
5. [Camera Operations](#camera-operations)
6. [Camera Bookmarks](#camera-bookmarks)
7. [Camera Speed Controls](#camera-speed-controls)
8. [API Reference](#api-reference)
9. [Usage Examples](#usage-examples)
10. [Configuration](#configuration)

---

## Overview

The Camera Controller provides advanced camera manipulation features for the Fresh Voxel Engine editor. It enables:

- **Orthographic Views**: Top, Front, Side, Bottom, Back, Left views for precise editing
- **Focus on Selection**: Center camera on selected objects (F key)
- **Frame Selection**: Automatically adjust camera to fit selection in view
- **Camera Bookmarks**: Save and restore camera positions
- **Speed Controls**: Adjust camera movement speed for different tasks
- **Smooth Transitions**: Optional smooth interpolation between camera positions

### Architecture

```
┌──────────────────────────────────────┐
│         EditorManager                 │
│  ┌────────────────────────────────┐  │
│  │    CameraController             │  │
│  │  - View Modes                   │  │
│  │  - Focus/Frame                  │  │
│  │  - Bookmarks                    │  │
│  │  - Speed Control                │  │
│  └────────────┬───────────────────┘  │
└───────────────┼──────────────────────┘
                │
        ┌───────▼──────┐
        │    Player     │
        │  ┌─────────┐ │
        │  │ Camera  │ │
        │  └─────────┘ │
        └──────────────┘
```

---

## Features

### ✅ Implemented

- **7 View Modes**: Perspective + 6 orthographic directions
- **Focus on Selection** (F key): Centers camera on selection
- **Frame Selection**: Fits selection in view with proper padding
- **Camera Bookmarks**: Save/load camera positions by name
- **Bookmark Persistence**: Save/load bookmarks to/from file
- **Speed Controls**: Configurable normal, fast, and slow speeds
- **Smooth Transitions**: Eased interpolation with configurable duration
- **Bounding Box Framing**: Intelligent framing based on selection bounds
- **Sphere Framing**: Frame objects based on bounding sphere

### 🔮 Planned

- **UI Panel**: Visual controls for camera settings
- **Menu Integration**: View menu with camera mode options
- **Preset Bookmarks**: Quick-save slots (F1-F12)
- **Camera Path Recording**: Record and playback camera movements
- **Ortho Zoom**: Zoom controls for orthographic views

---

## View Modes

The Camera Controller supports 7 view modes:

### Perspective
**Default 3D view** - Standard first-person camera
- Full freedom of movement
- Mouse look controls
- WASD movement with physics

### Top
**Orthographic top view** - Looking down (Y-)
- Camera positioned above target
- Pitch: -90°, Yaw: 0°
- Best for: Layout design, overhead planning

### Front
**Orthographic front view** - Looking along Z-
- Camera positioned in front of target
- Pitch: 0°, Yaw: -90°
- Best for: Front-facing details, Z-axis alignment

### Side (Right)
**Orthographic side view** - Looking along X-
- Camera positioned to the right of target
- Pitch: 0°, Yaw: 0°
- Best for: Side profiles, X-axis alignment

### Bottom
**Orthographic bottom view** - Looking up (Y+)
- Camera positioned below target
- Pitch: 90°, Yaw: 0°
- Best for: Underside inspection

### Back
**Orthographic back view** - Looking along Z+
- Camera positioned behind target
- Pitch: 0°, Yaw: 90°
- Best for: Rear details

### Left
**Orthographic left view** - Looking along X+
- Camera positioned to the left of target
- Pitch: 0°, Yaw: 180°
- Best for: Left-side profiles

---

## Keyboard Shortcuts

| Key | Action | Description |
|-----|--------|-------------|
| `F` | Focus on Selection | Centers camera on selected objects |
| _(TBD)_ | Frame Selection | Fits selection in view |
| _(TBD)_ | Toggle View Mode | Cycle through view modes |
| _(TBD)_ | Perspective View | Return to perspective mode |
| _(TBD)_ | Top View | Switch to top orthographic view |
| _(TBD)_ | Front View | Switch to front orthographic view |
| _(TBD)_ | Side View | Switch to side orthographic view |

**Note**: Additional shortcuts will be added via menu integration.

---

## Camera Operations

### Focus on Selection

**Purpose**: Center the camera on selected objects while maintaining viewing angle.

**How it works**:
1. Calculates center point of all selected blocks
2. Maintains current camera direction
3. Moves camera to view the selection from current angle
4. Preserves distance (or uses specified distance)

**Usage**:
```cpp
// Via keyboard shortcut
// Press F key when selection exists

// Via API
editorManager->focusOnSelection();

// Via CameraController directly
cameraController->focusOn(targetPos, distance);
```

**Behavior**:
- Works in all view modes (perspective and orthographic)
- Smooth transition if enabled
- Logged to console for debugging
- No-op if no selection exists

### Frame Selection

**Purpose**: Adjust camera position and distance to fit selection perfectly in view.

**How it works**:
1. Calculates bounding box of selection (min/max corners)
2. Computes required distance based on FOV and box size
3. Moves camera to optimal viewing distance
4. Adds 20% padding for comfortable viewing

**Usage**:
```cpp
// Via API
editorManager->frameSelection();

// Via CameraController directly
cameraController->frameBox(min, max);
// or
cameraController->frameSphere(center, radius);
```

**Mathematical Details**:
```cpp
// Distance calculation for perspective view
float fov = camera->getFOV();
float maxDimension = max(size.x, size.y, size.z);
float distance = maxDimension / (2.0f * tan(radians(fov * 0.5f)));
distance *= 1.2f; // Add 20% padding
```

---

## Camera Bookmarks

### Overview

Camera bookmarks save and restore camera positions, rotations, and view modes.

### Bookmark Structure

```cpp
struct Bookmark {
    std::string name;      // User-defined name
    glm::vec3 position;    // Camera position in world space
    float pitch;           // Camera pitch angle
    float yaw;             // Camera yaw angle
    ViewMode viewMode;     // View mode when saved
};
```

### Operations

#### Save Bookmark
```cpp
// Save current camera state
cameraController->saveBookmark("WorkingPosition");

// Result: Creates or updates bookmark with given name
```

#### Load Bookmark
```cpp
// Restore saved camera state
bool success = cameraController->loadBookmark("WorkingPosition");

// Result: 
// - Moves camera to saved position
// - Restores saved rotation
// - Switches to saved view mode
// - Returns false if bookmark doesn't exist
```

#### Delete Bookmark
```cpp
// Remove a bookmark
bool success = cameraController->deleteBookmark("WorkingPosition");
```

#### List Bookmarks
```cpp
// Get all bookmarks
const auto& bookmarks = cameraController->getBookmarks();
for (const auto& bookmark : bookmarks) {
    LOG_INFO("Bookmark: " + bookmark.name);
}
```

### File Persistence

Bookmarks can be saved to and loaded from files in CSV format.

#### File Format
```csv
# Fresh Voxel Engine - Camera Bookmarks
# Format: name,posX,posY,posZ,pitch,yaw,viewMode
MainView,0.0,100.0,0.0,-45.0,0.0,0
SideDetail,50.0,50.0,0.0,0.0,0.0,3
TopDown,0.0,200.0,0.0,-90.0,0.0,1
```

#### Save to File
```cpp
// Save all bookmarks to file
cameraController->saveBookmarksToFile("configs/camera_bookmarks.csv");
```

#### Load from File
```cpp
// Load bookmarks from file
cameraController->loadBookmarksFromFile("configs/camera_bookmarks.csv");
```

### Use Cases

1. **Quick Navigation**: Save bookmarks at important locations
2. **Review Workflow**: Save positions for iterative review
3. **Tutorial Creation**: Record camera positions for documentation
4. **Team Collaboration**: Share bookmark files with team members

---

## Camera Speed Controls

### Overview

The Camera Controller provides three speed presets for different editing scenarios.

### Speed Types

1. **Normal Speed** (Default: 10.0 units/sec)
   - Default camera movement speed
   - Balanced for general editing

2. **Fast Speed** (Default: 30.0 units/sec)
   - Rapid movement for covering large distances
   - Activated with Shift key

3. **Slow Speed** (Default: 3.0 units/sec)
   - Precise movement for detailed work
   - Activated with Ctrl key

### Configuration

```cpp
// Set normal speed
cameraController->setNormalSpeed(15.0f);

// Set fast speed
cameraController->setFastSpeed(50.0f);

// Set slow speed
cameraController->setSlow Speed(5.0f);

// Apply speeds to player
cameraController->applySpeedToPlayer();
```

### Integration

Speeds are applied to the Player class:
- `normalSpeed` → `Player::setWalkSpeed()`
- `fastSpeed` → `Player::setSprintSpeed()`

---

## API Reference

### CameraController Class

#### Initialization
```cpp
void initialize(Player* player);
```
Initializes controller with player reference. Required before use.

#### View Mode Control
```cpp
void setViewMode(ViewMode mode);
ViewMode getViewMode() const;
static std::string getViewModeName(ViewMode mode);
```

#### Selection Operations
```cpp
void focusOn(const glm::vec3& target, float distance = -1.0f);
void frameBox(const glm::vec3& min, const glm::vec3& max);
void frameSphere(const glm::vec3& center, float radius);
```

#### Speed Control
```cpp
void setNormalSpeed(float speed);
float getNormalSpeed() const;
void setFastSpeed(float speed);
float getFastSpeed() const;
void setSlowSpeed(float speed);
float getSlowSpeed() const;
void applySpeedToPlayer();
```

#### Bookmark Management
```cpp
bool saveBookmark(const std::string& name);
bool loadBookmark(const std::string& name);
bool deleteBookmark(const std::string& name);
const std::vector<Bookmark>& getBookmarks() const;
bool saveBookmarksToFile(const std::string& filename) const;
bool loadBookmarksFromFile(const std::string& filename);
```

#### Transition Control
```cpp
void setSmoothTransitions(bool enabled);
bool isSmoothTransitionsEnabled() const;
void setTransitionDuration(float duration);
float getTransitionDuration() const;
```

#### Update
```cpp
void update(float deltaTime);
```
Call each frame to update smooth transitions.

---

## Usage Examples

### Example 1: Focus on Selected Blocks

```cpp
// User presses F key
if (inputManager->isKeyJustPressed(KEY_F)) {
    editorManager->focusOnSelection();
}

// Results in:
// 1. Selection center calculated
// 2. Camera smoothly moves to view selection
// 3. Camera direction preserved
// 4. Comfortable viewing distance maintained
```

### Example 2: Switch to Top View

```cpp
// Via EditorManager
editorManager->setCameraViewMode("Top");

// Via CameraController
cameraController->setViewMode(CameraController::ViewMode::Top);

// Results in:
// 1. Camera positioned above target
// 2. Looking straight down (pitch: -90°)
// 3. Smooth transition if enabled
// 4. View mode updated
```

### Example 3: Create and Use Bookmarks

```cpp
// Save current position
cameraController->saveBookmark("DetailView");
LOG_INFO("Saved DetailView bookmark");

// Do other work...

// Restore saved position
if (cameraController->loadBookmark("DetailView")) {
    LOG_INFO("Restored DetailView bookmark");
} else {
    LOG_WARNING("DetailView bookmark not found");
}
```

### Example 4: Frame Selected Region

```cpp
// Frame all selected blocks in view
editorManager->frameSelection();

// Results in:
// 1. Selection bounds calculated
// 2. Required viewing distance computed
// 3. Camera moves to optimal position
// 4. Selection fits comfortably in view
```

### Example 5: Adjust Camera Speeds

```cpp
// Get camera controller
auto* controller = editorManager->getCameraController();

// Configure speeds for precise editing
controller->setNormalSpeed(5.0f);  // Slower normal speed
controller->setFastSpeed(20.0f);   // Moderate fast speed
controller->setSlowSpeed(1.0f);    // Very precise slow speed

// Apply to player
controller->applySpeedToPlayer();
```

---

## Configuration

### Default Settings

```cpp
// View mode
ViewMode: Perspective

// Camera speeds
Normal Speed: 10.0 units/sec
Fast Speed:   30.0 units/sec
Slow Speed:   3.0 units/sec

// Smooth transitions
Enabled: true
Duration: 0.3 seconds

// Orthographic views
Distance: 50.0 units from target
```

### Configuration File

Camera settings can be persisted in `configs/camera_settings.ini`:

```ini
[Camera]
DefaultViewMode=Perspective
SmoothTransitions=true
TransitionDuration=0.3

[Speeds]
NormalSpeed=10.0
FastSpeed=30.0
SlowSpeed=3.0

[Orthographic]
Distance=50.0
```

### Bookmark File

Bookmarks are stored in `configs/camera_bookmarks.csv`.

---

## Best Practices

### When to Use Each View Mode

1. **Perspective**: 
   - General navigation
   - Getting spatial sense
   - First-person preview

2. **Top View**:
   - Layout planning
   - Overhead design
   - Placement verification

3. **Front/Side/Back Views**:
   - Precise alignment
   - Symmetry checking
   - Profile work

4. **Bottom View**:
   - Underside details
   - Foundation checking

### Workflow Tips

1. **Use F Key Frequently**: Quick focus saves time
2. **Create Bookmarks**: Save important viewpoints
3. **Frame Before Details**: Get overview before zooming in
4. **Use Slow Speed**: Enable Ctrl for precise adjustments
5. **Smooth Transitions**: Keep enabled for comfortable navigation

### Performance Considerations

- Smooth transitions add minimal overhead
- Bookmark files are lightweight (text format)
- No performance impact when camera is stationary
- Update() only processes during transitions

---

## Troubleshooting

### Camera Not Moving

**Issue**: Focus or frame operations don't move camera.

**Solutions**:
1. Check player is set: `editorManager->setPlayer(player)`
2. Verify camera controller is initialized
3. Check for selection: `selectionManager->hasSelection()`

### Transitions Too Fast/Slow

**Issue**: Camera transitions feel wrong.

**Solutions**:
```cpp
// Adjust duration
cameraController->setTransitionDuration(0.5f); // Slower
cameraController->setTransitionDuration(0.1f); // Faster

// Or disable smooth transitions
cameraController->setSmoothTransitions(false);
```

### Bookmark Not Found

**Issue**: `loadBookmark()` returns false.

**Solutions**:
1. Check bookmark name spelling
2. Verify bookmark was saved
3. Check if file was loaded: `loadBookmarksFromFile()`

### Orthographic View Wrong Distance

**Issue**: Camera too close or too far in ortho views.

**Solutions**:
```cpp
// After switching to ortho view
cameraController->focusOn(target, desiredDistance);
```

---

## Future Enhancements

### Planned Features

1. **UI Panel**: Visual controls for all camera settings
2. **Menu Integration**: View menu with mode switching
3. **Preset Slots**: F1-F12 quick-save bookmarks
4. **Ortho Zoom**: Zoom in/out in orthographic views
5. **Camera Gizmo**: Visual representation of camera in scene
6. **Path Recording**: Record and playback camera movements
7. **Camera Shake**: Add camera shake effects
8. **Depth of Field**: Adjustable DOF for focus effects

### API Extensions

```cpp
// Planned methods
void setCameraShake(float intensity, float duration);
void startRecording();
void stopRecording();
void playbackRecording(const std::string& name);
void setOrthoZoom(float zoom);
```

---

## Related Documentation

- [GUI Guide](GUI_GUIDE.md) - Complete GUI system documentation
- [Camera Class](../gameplay/Camera.h) - Base camera implementation
- [Player Class](../gameplay/Player.h) - Player and camera integration
- [Editor Manager](EditorManager.h) - Editor system overview
- [Input System](../guides/INPUT_SYSTEM.md) - Keyboard shortcuts

---

## Version History

- **v1.0** (2025-11-19): Initial implementation
  - 7 view modes
  - Focus and frame operations
  - Bookmark system
  - Speed controls
  - Smooth transitions
  - File persistence

---

**Last Updated**: 2025-11-19  
**Status**: Production Ready  
**Author**: Fresh Voxel Engine Team
