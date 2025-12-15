# Testing Guide: Viewport Utilization Fix

## Overview

This guide provides step-by-step instructions for testing the viewport utilization fix when creating new worlds in Fresh Voxel Engine.

## Prerequisites

- Windows 10/11 system
- Fresh Voxel Engine built in Release configuration
- Understanding of the expected behavior (world should render in viewport only)

## Test Scenarios

### Test 1: Basic World Creation

**Objective**: Verify that a new world renders only within the viewport panel, not full screen.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Click `File > New Scene` (or press `Ctrl+N`)
3. Select "3D Voxel" as the scene type
4. Enter a world name (e.g., "Test World 1")
5. Enter a seed (e.g., 12345)
6. Click "Create"

**Expected Results**:
- ✅ World generation progress displays in console
- ✅ World renders ONLY within the center viewport panel
- ✅ All GUI panels remain visible (left tools, right outliner/inspector, bottom console/content browser)
- ✅ No full-screen rendering occurs
- ✅ Console shows log messages:
  ```
  Pre-world-creation viewport check: [width]x[height]
  Configuring viewport swap chain BEFORE world creation - viewport size: [width]x[height]
  ✓ Viewport swap chain configured before world creation: [width]x[height]
  ✓ Viewport ready for rendering: [width]x[height]
  ```

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 2: World Creation After Window Resize

**Objective**: Verify viewport works correctly after resizing the main window.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Resize the main window to a different size (drag corner to make larger/smaller)
3. Click `File > New Scene`
4. Create a new world with any settings
5. Observe rendering

**Expected Results**:
- ✅ Viewport dimensions are recalculated for new window size
- ✅ World renders in viewport panel at correct size
- ✅ GUI panels adjust to new window size
- ✅ No rendering outside viewport bounds

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 3: World Creation on Maximized Window

**Objective**: Verify viewport works when window is maximized.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Maximize the window
3. Click `File > New Scene`
4. Create a new world with any settings
5. Observe rendering

**Expected Results**:
- ✅ Viewport scales to use available space in maximized window
- ✅ World renders in viewport panel
- ✅ GUI panels scale appropriately
- ✅ No full-screen rendering

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 4: Multiple World Creations

**Objective**: Verify viewport configuration persists across multiple world creations.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Create first world (File > New Scene)
3. After world loads, create second world (File > New Scene)
4. After second world loads, create third world (File > New Scene)
5. Observe each world creation

**Expected Results**:
- ✅ All three worlds render in viewport panel
- ✅ No full-screen rendering on any creation
- ✅ GUI remains visible for all creations
- ✅ Swap chain logs show configuration before each world creation

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 5: Different World Types

**Objective**: Verify viewport works for all world/scene types.

**Steps**:
1. Create a 3D Voxel world → Observe rendering
2. Create a 2D Platformer world → Observe rendering
3. Create a 2D Top-Down world → Observe rendering
4. Create an Empty scene → Observe rendering

**Expected Results**:
- ✅ All scene types render in viewport panel
- ✅ No full-screen rendering for any type
- ✅ GUI remains visible for all types

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 6: Viewport During Chunk Generation

**Objective**: Verify viewport is used even during active chunk generation.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Create a new 3D world with large seed
3. **While chunks are being generated** (watch console output), observe the viewport
4. Wait for all chunks to generate

**Expected Results**:
- ✅ During generation, partial world renders in viewport only
- ✅ No full-screen rendering appears during generation
- ✅ After generation completes, world remains in viewport
- ✅ GUI panels remain visible throughout

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

### Test 7: Window Resize After World Creation

**Objective**: Verify viewport continues to work correctly after world exists.

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Create a new world
3. After world is loaded, resize the window
4. Maximize and restore the window
5. Observe rendering after each resize

**Expected Results**:
- ✅ Viewport scales correctly with each resize
- ✅ World continues to render in viewport panel
- ✅ GUI panels adjust appropriately
- ✅ No rendering artifacts or full-screen bleeding

**Pass/Fail**: ___________

**Notes**: ___________________________________________

---

## Log Analysis

### Expected Log Sequence

When creating a new world, you should see this sequence in the console/log file:

```
Pre-world-creation viewport check: [width]x[height]
Configuring viewport swap chain BEFORE world creation - viewport size: [width]x[height]
✓ Viewport swap chain configured before world creation: [width]x[height]
✓ Viewport ready for rendering: [width]x[height]
Voxel world initialized
Generating initial terrain...
  Generated X/Y chunks...
  Generated Y/Y chunks...
Voxel interaction initialized
Player initialized
Camera set to [mode]
Re-validating viewport swap chain after world creation - viewport size: [width]x[height]
Re-confirming viewport window handle in renderer...
Viewport window handle confirmed. Ensuring swap chain is current...
✓ Viewport swap chain re-verified: [width]x[height]
Editor manager set visible after viewport swap chain configuration
```

### Warning/Error Conditions

If you see any of these, document them as test failures:

❌ `CRITICAL: Invalid viewport dimensions before world creation`
- Indicates viewport dimensions are zero or negative
- Should trigger automatic fix, but document the occurrence

❌ `Failed to recreate swap chain before world creation`
- Indicates DirectX swap chain creation failed
- Major issue that will cause full-screen rendering

❌ `Skipping swap chain configuration before world creation`
- Indicates viewport state is invalid
- Should include details about what's invalid (handle, dimensions)

❌ `ERROR: Invalid viewport dimensions - GUI will not display correctly!`
- Indicates dimensions are still invalid after fix attempts
- Critical failure

## Performance Checks

During testing, also note:

1. **Frame Rate**: Does world creation cause significant FPS drops?
2. **Memory Usage**: Monitor task manager during world generation
3. **Responsiveness**: Can you still interact with GUI during world creation?
4. **Startup Time**: How long from "Create" click to fully loaded world?

## Regression Testing

Ensure existing functionality still works:

- [ ] Play mode (F5) enters/exits correctly
- [ ] Camera controls work in viewport
- [ ] Voxel editing tools work in viewport
- [ ] GUI panels can be interacted with
- [ ] Scene save/load works
- [ ] Window minimize/maximize/restore works

## Reporting Issues

If you encounter any failures, please report:

1. **Test Scenario**: Which test failed?
2. **Steps to Reproduce**: Exact steps taken
3. **Expected vs Actual**: What should happen vs what happened
4. **Screenshots**: Capture the failure state
5. **Logs**: Provide console output or log file
6. **System Info**: Windows version, GPU, display resolution

## Success Criteria

All tests must pass with ✅ for the fix to be considered successful. The critical requirement is:

**World MUST render in viewport panel only, never full screen, during and after world creation.**

## Additional Notes

- Test on multiple monitor setups if possible (single monitor, dual monitor, different DPI)
- Test with different window sizes and aspect ratios
- Test with high DPI displays (4K, 5K)
- Test after system resume from sleep/hibernate if possible

---

**Tester Name**: ___________
**Date**: ___________
**Build Version**: ___________
**Overall Result**: PASS / FAIL
