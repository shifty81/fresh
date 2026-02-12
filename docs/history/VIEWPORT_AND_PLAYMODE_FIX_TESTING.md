# Viewport and Play Mode Fix - Testing Guide

## Overview
This document provides comprehensive testing instructions for verifying the viewport and play mode fixes.

## Problem Statement
The following issues were reported:
1. World generation appears behind GUI panels (should only render in viewport)
2. Small gaps in GUI show world behind it (rendering to wrong target) 
3. Play mode turns fullscreen and hides GUI (should keep GUI visible with viewport)
4. Everything should be contained within the viewport panel only

## Fixes Applied
1. Viewport panel explicitly kept visible during play mode transitions
2. Z-order enforcement after visibility changes to keep viewport behind UI panels
3. Proper separation of rendering targets (viewport only, not main window)

## Testing Checklist

### Test 1: Initial Launch
**Steps:**
1. Launch FreshVoxelEngine.exe
2. Observe the initial editor layout

**Expected Results:**
- [ ] Editor window opens with proper layout
- [ ] Viewport panel visible in center (showing sky blue clear color if no world)
- [ ] UI panels visible around viewport (hierarchy, inspector, console, content browser, terraforming)
- [ ] Menu bar visible at top
- [ ] Toolbar visible below menu
- [ ] Status bar visible at bottom
- [ ] No world rendering visible in gaps between panels
- [ ] Gaps show black color (main window background)

### Test 2: New Scene Creation
**Steps:**
1. Click File > New Scene (or Ctrl+N)
2. Configure scene parameters in dialog
3. Click Create
4. Observe world generation

**Expected Results During Generation:**
- [ ] World creation dialog appears
- [ ] After clicking Create, dialog closes
- [ ] Viewport shows sky blue clear color during generation
- [ ] Console shows generation progress
- [ ] UI remains responsive (can click panels)
- [ ] NO world rendering appears behind GUI panels
- [ ] NO world rendering visible through gaps

**Expected Results After Generation:**
- [ ] World appears IN viewport only
- [ ] World is NOT visible behind UI panels
- [ ] World is NOT visible through gaps between panels
- [ ] Gaps continue to show black
- [ ] Can navigate camera in viewport
- [ ] UI panels remain on top of viewport (not covered by viewport)

### Test 3: Viewport Boundaries
**Steps:**
1. With a world loaded, observe the viewport area
2. Move camera around to see different parts of world
3. Look at panel edges and gaps

**Expected Results:**
- [ ] World rendering stays within viewport bounds
- [ ] World does NOT extend beyond viewport edges
- [ ] Panel edges cleanly border the viewport
- [ ] 5px gaps between panels show black, not world
- [ ] UI panels are fully opaque (not transparent)
- [ ] Viewport is clearly behind UI panels (proper Z-order)

### Test 4: Panel Resizing
**Steps:**
1. Resize the main window (drag corner or maximize)
2. Observe how panels and viewport adjust

**Expected Results:**
- [ ] All panels resize proportionally
- [ ] Viewport remains in center between side panels
- [ ] Z-order maintained (viewport behind, UI on top)
- [ ] No rendering artifacts or gaps
- [ ] World rendering stays confined to viewport
- [ ] Gaps continue to show black

### Test 5: Play Mode - Enter
**Steps:**
1. With a world loaded in editor
2. Click Play button on toolbar (or press Alt+P or F5)
3. Observe the transition

**Expected Results:**
- [ ] Viewport REMAINS visible (continues showing world)
- [ ] World rendering STAYS in viewport (doesn't go fullscreen)
- [ ] UI panels hide (hierarchy, inspector, console, content browser, terraforming)
- [ ] Menu bar REMAINS visible (File, Edit, View, etc.)
- [ ] Toolbar REMAINS visible (with Stop button now active)
- [ ] Status bar may be hidden (this is acceptable)
- [ ] Game HUD appears (health, hotbar, etc.)
- [ ] Cursor is captured for FPS controls
- [ ] Camera switches to FPS mode with physics
- [ ] Can control player with WASD, mouse look, Space to jump

### Test 6: Play Mode - Gameplay
**Steps:**
1. While in play mode, test gameplay
2. Move around, jump, interact with blocks

**Expected Results:**
- [ ] Game renders IN viewport only
- [ ] Viewport boundaries respected (game doesn't go fullscreen)
- [ ] Menu and toolbar remain visible around viewport
- [ ] Game HUD visible over viewport
- [ ] Physics working (gravity, collision)
- [ ] FPS controls working (WASD, mouse, jump)
- [ ] World rendering doesn't spill outside viewport

### Test 7: Play Mode - Exit
**Steps:**
1. While in play mode, press ESC
2. Observe the transition back to editor

**Expected Results:**
- [ ] Returns to editor mode
- [ ] Viewport REMAINS visible (still showing world)
- [ ] UI panels reappear (hierarchy, inspector, console, content browser, terraforming)
- [ ] Menu bar still visible
- [ ] Toolbar still visible (Play button active again)
- [ ] Status bar reappears
- [ ] Game HUD hides
- [ ] Cursor released for UI interaction
- [ ] Camera switches back to free-fly mode
- [ ] Z-order correct (viewport behind UI panels)

### Test 8: Multiple Play Mode Cycles
**Steps:**
1. Enter play mode (Alt+P)
2. Exit play mode (ESC)
3. Repeat 3-5 times

**Expected Results:**
- [ ] Each transition works correctly
- [ ] No degradation in behavior
- [ ] Z-order remains correct
- [ ] Viewport always visible
- [ ] No rendering artifacts
- [ ] No gaps showing world rendering

### Test 9: Panel Visibility Toggles
**Steps:**
1. In editor mode, toggle panel visibility via View menu
2. Toggle Scene Hierarchy, Inspector, Console, Content Browser
3. Observe viewport and panel behavior

**Expected Results:**
- [ ] Viewport ALWAYS remains visible (never hidden)
- [ ] Individual panels hide/show correctly
- [ ] Viewport Z-order maintained (always behind visible panels)
- [ ] World rendering stays in viewport
- [ ] Gaps continue to show black

### Test 10: Edge Cases
**Steps:**
1. Hide all UI panels (toggle each off)
2. Enter play mode with all panels hidden
3. Exit play mode
4. Show all panels again

**Expected Results:**
- [ ] Viewport remains visible throughout
- [ ] Play mode works with panels hidden
- [ ] Returning to editor respects panel visibility settings
- [ ] Z-order correct in all states
- [ ] World rendering always in viewport only

## Known Acceptable Behaviors

1. **Gaps Between Panels**: Small 5px gaps are intentional for visual separation. These show the main window background (black), NOT the world rendering.

2. **Status Bar in Play Mode**: Status bar may hide during play mode. This is acceptable as it's part of the editor UI.

3. **Menu/Toolbar in Play Mode**: Menu bar and toolbar remain visible during play mode because they are native Win32 controls attached to the main window frame, not editor panels.

4. **Viewport Clear Color**: When no world exists or during generation, viewport shows sky blue clear color (RGB: 0.53, 0.81, 0.92). This is intentional.

## Reporting Issues

If any test fails, report the following:
1. Test number and step that failed
2. Expected vs actual behavior
3. Screenshot if visual issue
4. Steps to reproduce
5. System info (Windows version, GPU, DirectX version)

## Success Criteria

All tests should pass with expected results. Specifically:
- ✓ Viewport always visible in both editor and play modes
- ✓ World rendering confined to viewport only
- ✓ UI panels properly layered on top of viewport
- ✓ Play mode doesn't go fullscreen
- ✓ Smooth transitions between editor and play modes
- ✓ No rendering artifacts or gaps showing world
