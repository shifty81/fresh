# Viewport Fix Testing Checklist

## Quick Test (5 minutes)

### Basic Functionality
- [ ] Launch `Release\FreshVoxelEngine.exe`
- [ ] Create new scene (File > New Scene or Ctrl+N)
- [ ] Select "3D Voxel" world type
- [ ] Enter any world name and seed
- [ ] Click "Create"

### Expected Results
- [ ] ✅ World renders ONLY in center viewport panel (not full screen)
- [ ] ✅ Left panel (Terraforming Tools) is visible
- [ ] ✅ Right panels (Scene Hierarchy, Inspector) are visible
- [ ] ✅ Bottom panels (Content Browser, Console) are visible
- [ ] ✅ Console shows: "✓ Viewport swap chain configured: [width]x[height]"
- [ ] ✅ No error messages in console

## Comprehensive Test (15 minutes)

### Window Resize Test
- [ ] Resize window to smaller size (e.g., 1024x768)
  - [ ] ✅ Viewport scales correctly
  - [ ] ✅ World still renders only in viewport panel
  - [ ] ✅ All GUI panels remain visible
  
- [ ] Resize window to larger size (e.g., 1920x1080)
  - [ ] ✅ Viewport scales correctly
  - [ ] ✅ World still renders only in viewport panel
  - [ ] ✅ All GUI panels remain visible

- [ ] Maximize window
  - [ ] ✅ Viewport scales to fill available space
  - [ ] ✅ World still renders only in viewport panel
  - [ ] ✅ All GUI panels scale appropriately

- [ ] Restore window to normal size
  - [ ] ✅ Viewport returns to correct size
  - [ ] ✅ No rendering artifacts

### Multiple Scene Creation Test
- [ ] Create first scene (3D Voxel)
  - [ ] ✅ Renders correctly in viewport
  
- [ ] Create second scene (2D Platformer)
  - [ ] ✅ Renders correctly in viewport
  - [ ] ✅ First scene unloaded properly
  
- [ ] Create third scene (2D Top-Down)
  - [ ] ✅ Renders correctly in viewport
  - [ ] ✅ Previous scene unloaded properly

### Scene Loading Test
- [ ] Create and save a scene
- [ ] Close application
- [ ] Relaunch application
- [ ] Load the saved scene (File > Open Scene)
  - [ ] ✅ Scene loads correctly
  - [ ] ✅ World renders in viewport panel only
  - [ ] ✅ No full-screen rendering

### Different Scene Types Test
- [ ] Create 3D Voxel scene
  - [ ] ✅ Perspective camera works
  - [ ] ✅ Viewport rendering correct
  
- [ ] Create 2D Platformer scene
  - [ ] ✅ Side-scrolling view works
  - [ ] ✅ Viewport rendering correct
  
- [ ] Create 2D Top-Down scene
  - [ ] ✅ Top-down view works
  - [ ] ✅ Viewport rendering correct

## Log Verification

### Expected Console Output
Look for these messages in the console during world creation:

```
Pre-world-creation viewport check: [width]x[height]
Creating new 3D world: [name] (seed: [number])
...
Setting up viewport rendering: [width]x[height]
Setting viewport window handle in renderer...
Viewport window handle set successfully. Recreating swap chain...
DirectX swap chain successfully recreated for viewport after world creation
✓ Viewport swap chain configured: [width]x[height]
Ensured proper panel Z-order after viewport swap chain creation
```

### Warning Signs
If you see these messages, there may be an issue:

❌ **"CRITICAL: Viewport has invalid dimensions before world creation"**
- This means viewport dimensions are zero/negative
- Should be followed by "Attempting to fix..."
- If fix succeeds, world creation should continue normally

❌ **"ERROR: Failed to create viewport swap chain"**
- This indicates a DirectX swap chain creation failure
- World will render full screen instead of in viewport
- Check DirectX driver and GPU

❌ **"ERROR: No viewport panel handle"**
- This means viewport panel wasn't created properly
- GUI will not display correctly
- Indicates a serious initialization problem

## Bug Report Template

If you encounter issues, please report using this template:

```
### Issue Description
[Describe what went wrong]

### Steps to Reproduce
1. [Step 1]
2. [Step 2]
3. [etc.]

### Expected Behavior
[What should have happened]

### Actual Behavior
[What actually happened]

### Screenshots
[Attach screenshots showing the issue]

### Console Output
[Copy relevant console output, especially error messages]

### System Information
- OS: Windows [version]
- GPU: [GPU model]
- DirectX Version: [11 or 12]
- Window Resolution: [width]x[height]
- Application Version: [git commit hash]
```

## Known Issues & Limitations

### Expected Limitations
- **Windows Only**: Fix is specific to Win32 windowing
- **DirectX Only**: Fix applies to DirectX 11/12 render contexts
- **Render Loop Continues**: During chunk generation, render loop continues to run

### Not Bugs
- ❓ World briefly flickers during creation
  - This is expected as chunks are being generated
  - Not related to viewport rendering
  
- ❓ Console shows multiple "Viewport resized" messages
  - Normal during window resize operations
  - Each resize triggers swap chain recreation

## Success Criteria

✅ **Test Passes If:**
1. World always renders within viewport panel boundaries
2. GUI panels remain visible during and after world creation
3. Window resize correctly scales viewport and maintains panel visibility
4. Console shows "✓ Viewport swap chain configured" message
5. No critical errors in console during normal operation

❌ **Test Fails If:**
1. World renders full screen behind GUI
2. GUI panels are invisible or obscured
3. Console shows "CRITICAL" or "ERROR" messages
4. Window resize breaks viewport rendering
5. Application crashes during world creation

## Performance Check

### Frame Rate Test
- [ ] Create a world and let it run for 1 minute
- [ ] Observe FPS (should be displayed in status bar or console)
  - [ ] ✅ FPS is consistent (55-60 FPS)
  - [ ] ✅ No significant frame drops during rendering
  - [ ] ✅ Viewport rendering is smooth

### Memory Check
- [ ] Create multiple scenes in succession
- [ ] Check Task Manager for memory usage
  - [ ] ✅ Memory usage is stable
  - [ ] ✅ No memory leaks (memory doesn't continuously increase)
  - [ ] ✅ Application remains responsive

## Regression Test

To ensure the fix doesn't break existing functionality:

- [ ] Editor panels still work (Scene Hierarchy, Inspector, Console)
- [ ] Terraforming tools still work
- [ ] Play mode (F5) still works
- [ ] Camera controls still work (WASD, mouse)
- [ ] Save/Load still works
- [ ] Undo/Redo (Ctrl+Z/Ctrl+Y) still works
- [ ] Transform gizmo (W/E/R) still works

## Test Results

Date: _______________
Tester: _______________
Build: _______________

### Quick Test Result: ☐ Pass ☐ Fail
Notes: _______________________________________________

### Comprehensive Test Result: ☐ Pass ☐ Fail
Notes: _______________________________________________

### Issues Found:
1. _______________________________________________
2. _______________________________________________
3. _______________________________________________

### Overall Assessment: ☐ Ready for Merge ☐ Needs Fixes
