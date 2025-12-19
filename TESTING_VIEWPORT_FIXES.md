# Testing Guide: Viewport and GUI Fixes

**Date:** 2025-12-19
**PR:** Fix viewport rendering issues, panel gaps, and add toolbar icons

---

## Quick Start Testing

### 1. Build the Application

```batch
# Generate Visual Studio project
generate_vs2022.bat

# Build in Release mode
cd build
cmake --build . --config Release

# Run the application
Release\FreshVoxelEngine.exe
```

### 2. Visual Inspection (No World Needed)

**Check Panel Gaps:**
1. Launch the application
2. Look at the edges where panels meet:
   - Inspector (right side)
   - Scene Hierarchy (right top)
   - Content Browser (bottom left)
   - Console (bottom right)
   - Terraforming Panel (left side)
   - Viewport (center)

**Expected:** Clean, solid borders with no thin gaps or "toothpick" seams showing through
**Before Fix:** You would see thin light/dark edges creating visible gaps
**After Fix:** Solid single-pixel borders, no gaps

**Check Toolbar Icons:**
1. Look at the toolbar below the menu bar
2. All buttons should show icons, not just text

**Expected Icons:**
- 📄 New (document icon)
- 📁 Open (folder icon)
- 💾 Save (floppy disk icon)
- ↶ Undo (back arrow)
- ↷ Redo (forward arrow)
- ▶ Play (play button)
- ⏸ Pause (pause icon)
- ⏹ Stop (stop icon)
- And 12 more tool icons

**Before Fix:** No icons visible, only text labels
**After Fix:** Each button shows a clear icon

### 3. Viewport Rendering Test

**Test 1: Before World Creation**
1. Launch application
2. Look at the center viewport panel (large area between panels)

**Expected:** Sky blue color (RGB: 135, 206, 235)
**Problem if seen:** Black viewport or no viewport visible

**Test 2: Create New Scene**
1. Click **File > New Scene** (or press Ctrl+N, or click New toolbar button)
2. In the dialog:
   - Select "3D Voxel" scene type
   - Enter a name (e.g., "TestWorld")
   - Enter a seed (e.g., 12345)
   - Click "Create"

**WATCH CAREFULLY during chunk generation:**

**Expected Behavior:**
- ✅ Voxel chunks should ONLY appear in the center viewport panel
- ✅ Sky blue should be visible above terrain
- ✅ Ground/terrain should be visible at bottom
- ✅ All rendering stays within viewport bounds
- ✅ Editor panels (left, right, bottom) remain fully visible
- ✅ No rendering appears in main window outside viewport

**Problem Behaviors to Report:**
- ❌ Chunks render across entire window (ignoring panels)
- ❌ Viewport stays black after world generation
- ❌ Blue sky/terrain visible THROUGH panel gaps
- ❌ World renders "behind" panels

**Test 3: Camera Movement**
1. Right-click and hold inside viewport
2. Move mouse to look around
3. Use WASD keys to move camera

**Expected:**
- ✅ Camera only moves when mouse is IN viewport
- ✅ 3D world updates smoothly
- ✅ No rendering artifacts outside viewport

### 4. Window Resize Test

1. Resize window to smaller (e.g., 1024x768)
2. Look for gaps between panels
3. Resize window to larger (e.g., 1920x1080)
4. Look for gaps again
5. Maximize window
6. Restore window

**Expected:** No gaps appear at any window size
**Problem:** Thin gaps or seams become visible during/after resize

---

## Detailed Testing Checklist

### Panel Border Testing

- [ ] **Inspector Panel** (right side, bottom)
  - Top edge: Clean border, no gap
  - Bottom edge: Clean border, no gap
  - Left edge: Clean border, no gap
  - Right edge: Clean border, no gap

- [ ] **Scene Hierarchy** (right side, top)
  - All four edges: Clean borders, no gaps

- [ ] **Content Browser** (bottom left)
  - All four edges: Clean borders, no gaps

- [ ] **Console** (bottom right)
  - All four edges: Clean borders, no gaps
  - Gap between Content Browser and Console: No visible seam

- [ ] **Terraforming Panel** (left side)
  - All four edges: Clean borders, no gaps

- [ ] **Viewport** (center)
  - All four edges: Clean borders, no gaps
  - No sky blue or terrain visible OUTSIDE viewport area

### Toolbar Icon Testing

For each button, verify icon is visible:

**File Operations:**
- [ ] New (document icon)
- [ ] Open (folder icon)
- [ ] Save (floppy icon)

**Edit Operations:**
- [ ] Undo (back arrow)
- [ ] Redo (forward arrow)

**Play Controls:**
- [ ] Play (play button)
- [ ] Pause (pause icon)
- [ ] Stop (stop square)

**View Controls:**
- [ ] Perspective (3D icon)
- [ ] Top (grid icon)
- [ ] Camera (camera icon)
- [ ] Fullscreen (maximize icon)

**Transform Tools:**
- [ ] Select (cursor icon)
- [ ] Move (arrows icon)
- [ ] Rotate (circular arrows)
- [ ] Scale (resize icon)

**Voxel Tools:**
- [ ] Brush (paintbrush icon)
- [ ] Paint (color icon)
- [ ] Sculpt (tool icon)
- [ ] Smooth (gradient icon)

### Viewport Rendering Testing

**Initial State (No World):**
- [ ] Viewport shows sky blue (not black)
- [ ] Main window shows black where not covered by panels
- [ ] No DirectX rendering outside viewport

**During World Creation:**
- [ ] Progress shown (chunk generation messages)
- [ ] Voxel chunks appear ONLY in viewport
- [ ] No rendering in main window
- [ ] No rendering "behind" panels
- [ ] Editor panels remain fully opaque

**After World Created:**
- [ ] Terrain visible in viewport
- [ ] Sky blue visible above terrain
- [ ] Camera controls work (right-click + drag)
- [ ] WASD movement works
- [ ] World updates smoothly
- [ ] No rendering artifacts outside viewport

**Multiple Worlds:**
- [ ] Create second world: File > New Scene
- [ ] Previous world clears properly
- [ ] New world renders in viewport only
- [ ] No mixing of old and new world rendering

**Play Mode:**
- [ ] Press F5 or click Play button
- [ ] Editor panels hide
- [ ] Viewport expands (or game takes focus)
- [ ] Press ESC to return to editor
- [ ] Editor panels reappear correctly

---

## Expected Visual Results

### Panel Borders - Before vs After

**BEFORE (With WS_EX_CLIENTEDGE):**
```
Panel 1     |❙|     Panel 2
           ^^
    Light/dark edges
    creating visible gap
```

**AFTER (Without WS_EX_CLIENTEDGE):**
```
Panel 1     |     Panel 2
           ^
    Single solid border
    no visible gap
```

### Toolbar - Before vs After

**BEFORE:**
```
[New] [Open] [Save] [Undo] [Redo] [Play] ...
(text only, no icons)
```

**AFTER:**
```
[📄] [📁] [💾] [↶] [↷] [▶] ...
(icons visible on each button)
```

### Viewport Rendering - Correct Behavior

```
┌────────────────────────────────────────────┐
│ Main Window (Black background)            │
│ ┌────────┬─────────────────────┬────────┐ │
│ │ Terra  │                     │ Scene  │ │
│ │ -form  │                     │ Hier-  │ │
│ │ Panel  │     VIEWPORT        │ archy  │ │
│ │        │   (Sky Blue + 3D)   │────────│ │
│ │        │                     │ Insp-  │ │
│ │        │   ← Rendering ONLY  │ ector  │ │
│ │        │     in this area    │        │ │
│ ├────────┴─────────────────────┴────────┤ │
│ │ Content Browser │ Console             │ │
│ └─────────────────┴─────────────────────┘ │
└────────────────────────────────────────────┘
```

**No rendering should appear outside the viewport area!**

---

## Troubleshooting

### Issue: Black Viewport

**Symptom:** Viewport shows solid black instead of sky blue or 3D world

**Possible Causes:**
1. Swap chain not created (check logs for "viewport swap chain" messages)
2. Wrong clear color set
3. Rendering skipped due to missing swap chain

**Check:**
- Look in logs for "Viewport swap chain configured" messages
- Verify DirectX device initialized successfully
- Try creating a world to trigger swap chain setup

### Issue: Rendering Outside Viewport

**Symptom:** World renders across entire window, ignoring panels

**Possible Causes:**
1. Swap chain created for main window instead of viewport
2. Viewport window handle not set correctly

**Check:**
- Logs should say "will only use viewport swap chain"
- Should NOT see "main window swap chain" messages
- Verify viewport panel created successfully

### Issue: Panel Gaps Still Visible

**Symptom:** Thin gaps or seams between panels

**Possible Causes:**
1. Code changes not compiled
2. Different panel type still using WS_EX_CLIENTEDGE
3. Window theme causing border rendering issues

**Check:**
- Rebuild project completely: `cmake --build . --config Release --clean-first`
- Check all panels (some might be different classes)
- Try different Windows theme (light vs dark)

### Issue: No Toolbar Icons

**Symptom:** Toolbar buttons show text but no icons

**Possible Causes:**
1. shell32.dll not found or inaccessible
2. Icon indices incorrect for your Windows version
3. ExtractIconW failed

**Check:**
- Look in logs for "Failed to get system directory" message
- Verify shell32.dll exists: `C:\Windows\System32\shell32.dll`
- Check Windows version (should work on Win10/11)

**Workaround:** Buttons still work with text labels even without icons

---

## Reporting Results

### If Everything Works ✅

Report:
- "All fixes verified working"
- Include window size tested (e.g., 1920x1080)
- Mention Windows version (e.g., Windows 11)
- Optional: Include screenshot showing clean panel borders and toolbar icons

### If Issues Found ❌

Report with details:
1. **Which issue:**
   - Panel gaps?
   - Missing icons?
   - Viewport rendering?

2. **Symptoms:**
   - Describe what you see
   - When does it occur?

3. **Context:**
   - Window size
   - Windows version
   - Graphics API (DirectX 11 or 12)

4. **Logs:**
   - Copy relevant log messages
   - Especially any ERROR or WARNING messages

5. **Screenshots:**
   - Very helpful for visual issues
   - Show the problem clearly

---

## Success Criteria

All of these should be true:

✅ No visible gaps or seams between any panels at any window size
✅ All 20 toolbar buttons display icons clearly
✅ Viewport shows sky blue before world creation (not black)
✅ Creating a world renders ONLY within viewport panel
✅ No 3D rendering appears outside viewport bounds
✅ No sky or terrain visible through panel gaps
✅ Camera controls work within viewport
✅ Window resizing doesn't create gaps or artifacts

If all criteria met, the fixes are successful! 🎉

---

**Ready to test? Build and run FreshVoxelEngine.exe!**
