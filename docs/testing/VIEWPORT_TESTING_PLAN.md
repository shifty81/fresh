# Viewport Testing and Validation Plan

**Date**: 2026-01-09  
**Context**: Project-Based Architecture + Viewport Fixes  
**Issue**: "GUI viewport still isn't working something needs completely reworked"

## Overview

The viewport is a critical component that displays the 3D world within the editor. With the new project-based workflow, we need to ensure the viewport works correctly in all scenarios:

1. **Blank Projects** - Viewport should show empty scene (sky blue)
2. **Projects with Worlds** - Viewport should render world correctly
3. **Play Mode** - Viewport should handle game mode properly
4. **Resizing** - Viewport should adapt to window resize

## Test Scenarios

### Scenario 1: Launch with No Project

**Steps**:
1. Launch Fresh Engine
2. Editor opens with no project

**Expected Behavior**:
- Viewport visible and empty
- Shows sky blue clear color
- No errors in console
- Viewport responds to mouse/camera
- All UI panels visible and functional

**Validation**:
- [ ] Viewport shows sky blue
- [ ] No black screen
- [ ] No rendering behind UI
- [ ] Viewport is within editor bounds
- [ ] Console shows no errors

### Scenario 2: Create Blank Project

**Steps**:
1. File > New Project
2. Choose "Blank" template
3. Project created

**Expected Behavior**:
- Project created successfully
- Viewport remains empty (no world)
- Sky blue clear color maintained
- Editor fully functional

**Validation**:
- [ ] Project directory created
- [ ] .freshproj file exists
- [ ] Viewport still shows sky blue
- [ ] No world rendered (correct!)
- [ ] Can navigate viewport

### Scenario 3: Create Level in Blank Project

**Steps**:
1. Have blank project open
2. File > New Level
3. Choose "3D Terrain" generator
4. Configure seed (e.g., 12345)
5. Click Create

**Expected Behavior**:
- World generates successfully
- Viewport renders world **inside viewport panel**
- World does NOT render full-screen
- Editor UI remains visible
- Can navigate world in viewport

**Validation**:
- [ ] World visible in viewport
- [ ] Viewport bounds respected
- [ ] Editor UI not covered
- [ ] Terrain visible
- [ ] Camera works
- [ ] Console shows success messages

### Scenario 4: Create 3D Voxel Template Project

**Steps**:
1. File > New Project
2. Choose "3D Voxel Game" template
3. Create project
4. (Template doesn't create world automatically)
5. File > New Level
6. Choose "3D Terrain"
7. Create

**Expected Behavior**:
- Same as Scenario 3
- World renders in viewport
- No full-screen issues

**Validation**:
- [ ] Same checks as Scenario 3

### Scenario 5: Play Mode from Editor

**Steps**:
1. Have project with world open
2. Click Play button (F5)
3. Enter play mode
4. Play around
5. Press ESC
6. Return to editor

**Expected Behavior**:
- Editor UI hides
- Viewport expands to full window
- Game plays normally
- On ESC, editor UI returns
- Viewport returns to panel

**Validation**:
- [ ] Play mode works
- [ ] Viewport handles mode switch
- [ ] Editor returns correctly
- [ ] No rendering artifacts

### Scenario 6: Window Resize

**Steps**:
1. Have project open (with or without world)
2. Resize main window
3. Maximize window
4. Restore window

**Expected Behavior**:
- Viewport resizes proportionally
- All panels adjust positions
- No gaps or overlaps
- Rendering continues smoothly

**Validation**:
- [ ] Viewport resizes correctly
- [ ] No gaps between panels
- [ ] Content scales properly
- [ ] No visual glitches

## Known Issues (Previous Fixes)

According to existing documentation, these issues were previously addressed:

1. ✅ **Viewport Stays Black**: Fixed with dimension validation
2. ✅ **World Renders Behind UI**: Fixed by DirectX swap chain configuration
3. ✅ **Gaps Between Panels**: Fixed by removing WS_BORDER
4. ✅ **Swap Chain Configuration**: Changed to FLIP_SEQUENTIAL

However, the problem statement indicates issues persist. Possible causes:

1. **Regression**: Recent changes broke previous fixes
2. **New Issues**: Project-based changes introduced new problems
3. **Edge Cases**: Fixes work in some scenarios but not all

## Debugging Steps

If viewport issues are found:

### Step 1: Check Console Output

Look for these log messages:
```
"Viewport panel created successfully"
"Viewport swap chain created"
"Viewport dimensions: WxH"
"Render target bound to output merger"
"beginFrame() success"
```

### Step 2: Verify Viewport Window

Check that:
- Viewport HWND is valid
- Viewport has correct dimensions
- Viewport is visible (not hidden)
- Viewport is positioned correctly

### Step 3: Check DirectX State

Verify:
- Swap chain exists for viewport
- Render target view is created
- Depth stencil view is created
- Viewport is set in DirectX

### Step 4: Test Rendering Pipeline

Validate:
- clearRenderTarget() works
- beginFrame() returns true
- Render commands execute
- endFrame() presents correctly

## Potential Fixes

### Fix 1: Ensure Viewport Init Before World Creation

**Problem**: Viewport might not be fully initialized when world is created

**Solution**:
```cpp
// In Engine::createNewWorld()
if (!validateViewportState()) {
    LOG_ERROR("Viewport not ready for world creation");
    return;
}
```

### Fix 2: Handle Blank Projects Correctly

**Problem**: Rendering code might assume world always exists

**Solution**:
```cpp
// In Engine::render()
if (!m_world) {
    // Just clear to sky blue, don't try to render world
    m_renderer->clearRenderTarget(0.53f, 0.81f, 0.92f, 1.0f);
    return;
}
```

### Fix 3: Viewport State Tracking

**Problem**: Viewport state might get desynchronized

**Solution**:
```cpp
// Track viewport readiness
bool m_viewportReady = false;

// In Engine::initializeRendering()
m_viewportReady = m_editorManager->isViewportReady();

// Before rendering
if (!m_viewportReady) {
    LOG_WARNING("Viewport not ready, skipping render");
    return;
}
```

### Fix 4: Force Viewport Refresh After Project Creation

**Problem**: Viewport might not update after project state changes

**Solution**:
```cpp
// After project creation
if (m_editorManager) {
    m_editorManager->refreshViewport();
}
```

## Testing Checklist

Before marking viewport as fixed, validate ALL of the following:

### Basic Functionality
- [ ] Viewport appears on editor launch
- [ ] Viewport shows sky blue when empty
- [ ] No black screen issues
- [ ] No full-screen rendering over UI
- [ ] Viewport handles mouse input

### Project Integration
- [ ] Works with blank projects
- [ ] Works when creating levels in projects
- [ ] Works with template projects
- [ ] Viewport state persists across project operations

### Rendering Quality
- [ ] World renders correctly in viewport
- [ ] No visual artifacts
- [ ] Proper depth buffer
- [ ] Correct lighting
- [ ] Smooth frame rate

### Editor Integration
- [ ] Viewport positioned correctly in layout
- [ ] No gaps between panels
- [ ] Resizes with window
- [ ] Play mode works
- [ ] Camera controls work

### Performance
- [ ] No memory leaks
- [ ] Swap chain recreates properly on resize
- [ ] No dropped frames
- [ ] Efficient rendering

## Acceptance Criteria

The viewport is considered **fixed** when:

1. ✅ All test scenarios pass
2. ✅ All validation checks pass
3. ✅ No console errors related to viewport
4. ✅ User can work normally without viewport issues
5. ✅ Project-based workflow works seamlessly with viewport

## Next Steps

1. **Run All Test Scenarios**: Execute each test manually
2. **Document Findings**: Record any failures with screenshots
3. **Implement Fixes**: Address identified issues
4. **Re-test**: Verify fixes work
5. **Regression Test**: Ensure old issues don't return

## See Also

- [VIEWPORT_FIX_IMPLEMENTATION.md](../../VIEWPORT_FIX_IMPLEMENTATION.md) - Previous fixes
- [VIEWPORT_RENDER_TARGET_FIX.md](../../VIEWPORT_RENDER_TARGET_FIX.md) - Render target issues
- [PROJECT_WORKFLOW.md](../architecture/PROJECT_WORKFLOW.md) - New project workflow
