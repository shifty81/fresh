# Fresh Voxel Engine - GUI Testing Guide

This guide provides comprehensive testing procedures for the GUI system.

---

## Test Environment

### Requirements

- Windows 10/11 (x64)
- Visual Studio 2022
- DirectX 11 or DirectX 12 capable GPU
- Build completed successfully

### Build Instructions

```powershell
# Automated build (recommended)
.\setup-and-build.ps1

# Or manual build
.\generate_vs2022.bat
cd build
cmake --build . --config Release
```

---

## Phase 1: Basic GUI Tests

### 1.1 Application Launch

**Test**: Basic window creation and initialization

**Steps**:
1. Launch `FreshVoxelEngine.exe`
2. Verify window opens with proper dimensions (1920x1080 default)
3. Check window title: "Fresh Voxel Engine"
4. Verify no crashes or errors in console

**Expected Results**:
- ✅ Window opens successfully
- ✅ ImGui renders properly
- ✅ No error messages in console
- ✅ Frame rate is stable (>30 FPS)

**Pass Criteria**: All expected results met

---

### 1.2 Menu Bar

**Test**: Native Windows menu bar functionality

**Steps**:
1. Launch application
2. Click on each menu: File, Edit, View, Tools, Help
3. Verify dropdown menus appear
4. Test keyboard shortcuts: Alt+F (File), Alt+E (Edit), etc.
5. Navigate menus with arrow keys

**Expected Results**:
- ✅ All menus open correctly
- ✅ Menu items are readable
- ✅ Keyboard navigation works
- ✅ Menus close properly

**Pass Criteria**: All menus functional

---

### 1.3 Toolbar

**Test**: Native toolbar buttons

**Steps**:
1. Verify toolbar is visible below menu bar
2. Hover over each toolbar button
3. Verify tooltips appear
4. Click each button and observe response
5. Check button states (enabled/disabled/toggled)

**Expected Results**:
- ✅ Toolbar displays all buttons
- ✅ Tooltips show for each button
- ✅ Buttons respond to clicks
- ✅ Toggle states work (Move/Rotate/Scale)
- ✅ Disabled buttons are grayed out

**Pass Criteria**: All toolbar buttons functional

---

## Phase 2: Editor Panel Tests

### 2.1 Scene Hierarchy Panel

**Test**: Scene hierarchy tree view

**Steps**:
1. Open View > Scene Hierarchy (if not visible)
2. Verify panel appears in editor layout
3. Observe entities/chunks in tree view
4. Click on an entity to select it
5. Right-click for context menu
6. Test "Select All" and "Deselect All"
7. Test rename, duplicate, delete operations

**Expected Results**:
- ✅ Panel renders correctly
- ✅ Tree structure is visible
- ✅ Selection works
- ✅ Context menu appears
- ✅ Operations work (rename, duplicate, delete)
- ✅ Panel can be resized/docked

**Pass Criteria**: All operations successful

---

### 2.2 Inspector Panel

**Test**: Property editing

**Steps**:
1. Open View > Inspector (if not visible)
2. Select an entity in Scene Hierarchy
3. Verify properties appear in Inspector
4. Edit transform values (Position, Rotation, Scale)
5. Modify other properties
6. Verify changes apply to entity

**Expected Results**:
- ✅ Inspector shows selected entity properties
- ✅ Properties are editable
- ✅ Changes apply in real-time
- ✅ Property types render correctly (float, int, bool, etc.)
- ✅ Panel updates when selection changes

**Pass Criteria**: Property editing works

---

### 2.3 Content Browser Panel

**Test**: Asset browsing and management

**Steps**:
1. Open View > Content Browser (if not visible)
2. Navigate through asset folders
3. View different asset types (textures, models, etc.)
4. Search for assets using search box
5. Test import asset functionality
6. Try drag-and-drop to scene (if implemented)

**Expected Results**:
- ✅ Panel displays asset folders
- ✅ Navigation works
- ✅ Assets display with icons
- ✅ Search filters assets correctly
- ✅ Import dialog opens
- ✅ Asset operations work

**Pass Criteria**: Asset browsing functional

---

### 2.4 Console Panel

**Test**: Log message display

**Steps**:
1. Open View > Console (if not visible)
2. Verify log messages appear
3. Test filter buttons (Info, Warning, Error)
4. Test search functionality
5. Test clear button
6. Verify timestamps appear
7. Test copy functionality

**Expected Results**:
- ✅ Console displays messages
- ✅ Messages color-coded by level
- ✅ Filters work correctly
- ✅ Search finds messages
- ✅ Clear removes all messages
- ✅ Timestamps are correct
- ✅ Copy works

**Pass Criteria**: All console features work

---

### 2.5 Voxel Tool Palette

**Test**: Terraforming tool selection

**Steps**:
1. Open Voxel Tool Palette
2. Click each tool button (Brush, Sphere, Cube, Flatten, Smooth, Paint)
3. Adjust tool size slider
4. Adjust tool strength slider
5. Select different materials
6. Apply tool to voxel world

**Expected Results**:
- ✅ All tool buttons visible and clickable
- ✅ Active tool is highlighted
- ✅ Size slider changes tool size
- ✅ Strength slider changes effect
- ✅ Material selection works
- ✅ Tools modify voxels correctly

**Pass Criteria**: Terraforming tools functional

---

## Phase 3: Transform Gizmo Tests

### 3.1 Gizmo Rendering

**Test**: Visual gizmo display

**Steps**:
1. Select an entity in the viewport or hierarchy
2. Verify transform gizmo appears at entity position
3. Check gizmo axes are visible (Red X, Green Y, Blue Z)
4. Rotate camera and verify gizmo always faces camera
5. Verify gizmo scales with camera distance

**Expected Results**:
- ✅ Gizmo renders at entity position
- ✅ Axes are color-coded correctly
- ✅ Gizmo is visible from all angles
- ✅ Gizmo maintains readable size
- ✅ No z-fighting or visual artifacts

**Pass Criteria**: Gizmo renders correctly

---

### 3.2 Translate Mode (Move)

**Test**: Moving objects with gizmo

**Steps**:
1. Select an entity
2. Press W key or click Move button in toolbar
3. Verify gizmo shows translation arrows
4. Hover over X axis (red) - should highlight
5. Click and drag X axis
6. Verify entity moves along X axis only
7. Test Y and Z axes similarly
8. Hold Ctrl while dragging for grid snapping

**Expected Results**:
- ✅ W key activates translate mode
- ✅ Translation arrows visible
- ✅ Hover highlights axis
- ✅ Dragging moves entity along axis
- ✅ Movement is smooth
- ✅ Grid snapping works with Ctrl
- ✅ Inspector updates position values

**Pass Criteria**: Translation works on all axes

---

### 3.3 Rotate Mode

**Test**: Rotating objects with gizmo

**Steps**:
1. Select an entity
2. Press E key or click Rotate button in toolbar
3. Verify gizmo shows rotation circles
4. Hover over X circle (red) - should highlight
5. Click and drag X circle
6. Verify entity rotates around X axis only
7. Test Y and Z axes similarly
8. Hold Ctrl for angle snapping (15° increments)

**Expected Results**:
- ✅ E key activates rotate mode
- ✅ Rotation circles visible
- ✅ Hover highlights circle
- ✅ Dragging rotates entity around axis
- ✅ Rotation is smooth
- ✅ Angle snapping works with Ctrl
- ✅ Inspector updates rotation values

**Pass Criteria**: Rotation works on all axes

---

### 3.4 Scale Mode

**Test**: Scaling objects with gizmo

**Steps**:
1. Select an entity
2. Press R key or click Scale button in toolbar
3. Verify gizmo shows scale handles
4. Hover over X handle (red) - should highlight
5. Click and drag X handle
6. Verify entity scales along X axis only
7. Test Y and Z axes similarly
8. Test uniform scaling (center handle)

**Expected Results**:
- ✅ R key activates scale mode
- ✅ Scale handles visible
- ✅ Hover highlights handle
- ✅ Dragging scales entity along axis
- ✅ Scaling is smooth
- ✅ Uniform scaling works
- ✅ Inspector updates scale values

**Pass Criteria**: Scaling works on all axes

---

### 3.5 Keyboard Shortcuts

**Test**: Gizmo mode switching

**Steps**:
1. Select an entity
2. Press W key rapidly multiple times
3. Verify translate mode activates each time
4. Press E key
5. Verify rotate mode activates
6. Verify toolbar button highlights E (Rotate)
7. Press R key
8. Verify scale mode activates
9. Verify toolbar button highlights R (Scale)
10. Switch between modes rapidly (W-E-R-W-E-R)

**Expected Results**:
- ✅ W key always activates translate
- ✅ E key always activates rotate
- ✅ R key always activates scale
- ✅ Toolbar syncs with keyboard
- ✅ Gizmo visual updates immediately
- ✅ No lag or glitches during switching

**Pass Criteria**: Keyboard shortcuts reliable

---

### 3.6 Toolbar Integration

**Test**: Toolbar button control

**Steps**:
1. Select an entity
2. Click Move button in toolbar
3. Verify translate gizmo appears
4. Click Rotate button in toolbar
5. Verify rotate gizmo appears
6. Click Scale button in toolbar
7. Verify scale gizmo appears
8. Verify only one button is highlighted at a time
9. Test rapid clicking between buttons

**Expected Results**:
- ✅ Move button activates translate
- ✅ Rotate button activates rotate
- ✅ Scale button activates scale
- ✅ Button states are exclusive
- ✅ Visual feedback is immediate
- ✅ No conflicts with keyboard shortcuts

**Pass Criteria**: Toolbar controls work correctly

---

## Phase 4: Selection System Tests

### 4.1 Single Selection

**Test**: Selecting individual entities

**Steps**:
1. Click on an entity in viewport
2. Verify entity is highlighted/selected
3. Verify Inspector shows entity properties
4. Verify Scene Hierarchy highlights entity
5. Click another entity
6. Verify selection switches

**Expected Results**:
- ✅ Click selects entity
- ✅ Visual selection indicator appears
- ✅ Inspector updates
- ✅ Hierarchy syncs
- ✅ Only one entity selected at a time

**Pass Criteria**: Single selection works

---

### 4.2 Box Selection

**Test**: Multi-selection with box

**Steps**:
1. Click and drag in viewport to draw selection box
2. Verify box outline appears while dragging
3. Release mouse button
4. Verify all entities within box are selected
5. Verify Inspector shows multi-selection
6. Test undo/redo with selection

**Expected Results**:
- ✅ Selection box visualized during drag
- ✅ Multiple entities selected
- ✅ All selected entities highlighted
- ✅ Inspector handles multi-selection
- ✅ Undo/redo works

**Pass Criteria**: Box selection functional

---

### 4.3 Selection Operations

**Test**: Cut, Copy, Paste operations

**Steps**:
1. Select one or more entities
2. Press Ctrl+C to copy
3. Press Ctrl+V to paste
4. Verify copied entities appear
5. Select entities and press Ctrl+X to cut
6. Press Ctrl+V to paste
7. Verify entities moved
8. Test undo/redo for all operations

**Expected Results**:
- ✅ Copy creates duplicates on paste
- ✅ Cut removes originals
- ✅ Paste works correctly
- ✅ Pasted entities are selected
- ✅ Undo/redo works for all operations

**Pass Criteria**: All operations work

---

## Phase 5: File Operations Tests

### 5.1 New World

**Test**: Creating a new world

**Steps**:
1. Click File > New World (or Ctrl+N)
2. Verify dialog appears for world settings
3. Enter world name and settings
4. Click Create
5. Verify new world loads
6. Verify console shows no errors

**Expected Results**:
- ✅ Dialog appears
- ✅ Settings are configurable
- ✅ World creates successfully
- ✅ Viewport shows new world
- ✅ No errors in console

**Pass Criteria**: New world creation works

---

### 5.2 Open World

**Test**: Loading existing worlds

**Steps**:
1. Click File > Open World (or Ctrl+O)
2. Verify native file dialog appears
3. Navigate to worlds directory
4. Select a .world file
5. Click Open
6. Verify world loads
7. Verify entities and chunks appear

**Expected Results**:
- ✅ File dialog opens
- ✅ Can navigate folders
- ✅ World files are visible
- ✅ Selected world loads
- ✅ All world data restored
- ✅ No loading errors

**Pass Criteria**: World loading works

---

### 5.3 Save World

**Test**: Saving current world

**Steps**:
1. Make changes to world (place blocks, move entities)
2. Click File > Save World (or Ctrl+S)
3. Verify save completes
4. Check console for save confirmation
5. Close and reopen world
6. Verify changes persisted

**Expected Results**:
- ✅ Save completes quickly
- ✅ Confirmation message appears
- ✅ No errors during save
- ✅ Changes persist after reload

**Pass Criteria**: Save functionality works

---

### 5.4 Save World As

**Test**: Saving with new name

**Steps**:
1. Open an existing world
2. Click File > Save World As (or Ctrl+Shift+S)
3. Verify native save dialog appears
4. Enter new world name
5. Click Save
6. Verify world saved with new name
7. Verify original world unchanged

**Expected Results**:
- ✅ Save dialog opens
- ✅ Can choose location and name
- ✅ World saves successfully
- ✅ Both worlds exist independently

**Pass Criteria**: Save As works correctly

---

### 5.5 Import Assets

**Test**: Importing asset files

**Steps**:
1. Click File > Import Assets
2. Verify native file dialog appears
3. Select one or more asset files (textures, models, etc.)
4. Click Open
5. Verify assets appear in Content Browser
6. Check console for import messages

**Expected Results**:
- ✅ File dialog supports multi-select
- ✅ Multiple file types supported
- ✅ Import completes successfully
- ✅ Assets appear in browser
- ✅ No import errors

**Pass Criteria**: Asset import works

---

## Phase 6: Dark Theme Tests

### 6.1 Theme Colors

**Test**: Dark theme appearance

**Steps**:
1. Launch application
2. Verify dark theme is active
3. Check background colors (should be dark gray/black)
4. Check text colors (should be light gray/white)
5. Check accent colors (should be blue)
6. Verify contrast is readable

**Expected Results**:
- ✅ Background is dark (rgb(37,37,38))
- ✅ Text is light (rgb(241,241,241))
- ✅ Accent is blue (rgb(51,153,255))
- ✅ All text is readable
- ✅ No color conflicts

**Pass Criteria**: Theme is consistent and readable

---

### 6.2 Panel Styling

**Test**: Consistent panel appearance

**Steps**:
1. Open all editor panels
2. Verify all panels use dark theme
3. Check panel borders and separators
4. Check panel titles
5. Verify docking indicators match theme

**Expected Results**:
- ✅ All panels styled consistently
- ✅ Borders are visible but subtle
- ✅ Titles are readable
- ✅ Docking indicators visible

**Pass Criteria**: All panels match theme

---

## Phase 7: Performance Tests

### 7.1 Frame Rate

**Test**: Consistent rendering performance

**Steps**:
1. Launch application
2. Note FPS in title bar or console
3. Open all editor panels
4. Interact with UI (drag panels, click buttons, etc.)
5. Monitor FPS during interaction
6. Check for frame drops or stuttering

**Expected Results**:
- ✅ FPS > 60 when idle
- ✅ FPS > 30 during heavy interaction
- ✅ No stuttering or freezing
- ✅ Smooth animations

**Pass Criteria**: Performance is acceptable

---

### 7.2 Memory Usage

**Test**: Memory efficiency

**Steps**:
1. Launch application
2. Open Task Manager / Resource Monitor
3. Note initial memory usage
4. Use application for 10 minutes
5. Note final memory usage
6. Check for memory leaks (steady increase)

**Expected Results**:
- ✅ Initial memory < 500MB
- ✅ Memory stable during use
- ✅ No continuous increase
- ✅ No memory leaks

**Pass Criteria**: Memory usage is stable

---

## Phase 8: Stress Tests

### 8.1 Rapid Panel Operations

**Test**: UI stability under load

**Steps**:
1. Rapidly open and close panels
2. Rapidly drag panels around
3. Rapidly resize panels
4. Rapidly dock and undock panels
5. Check for crashes or errors

**Expected Results**:
- ✅ No crashes
- ✅ No visual glitches
- ✅ UI remains responsive
- ✅ No error messages

**Pass Criteria**: UI is stable

---

### 8.2 Rapid Gizmo Switching

**Test**: Gizmo mode switching stability

**Steps**:
1. Select an entity
2. Rapidly press W-E-R-W-E-R-W-E-R (10 times)
3. Rapidly click toolbar buttons
4. Switch modes while dragging gizmo
5. Check for visual artifacts or crashes

**Expected Results**:
- ✅ Mode switches reliably
- ✅ No visual glitches
- ✅ No crashes
- ✅ Gizmo renders correctly

**Pass Criteria**: Gizmo is stable

---

## Test Results Template

```markdown
## Test Session: [Date]

**Tester**: [Name]
**Build**: [Version/Commit]
**Platform**: Windows [10/11]
**GPU**: [GPU Model]

### Phase 1: Basic GUI Tests
- [ ] 1.1 Application Launch: PASS / FAIL
- [ ] 1.2 Menu Bar: PASS / FAIL
- [ ] 1.3 Toolbar: PASS / FAIL

### Phase 2: Editor Panel Tests
- [ ] 2.1 Scene Hierarchy: PASS / FAIL
- [ ] 2.2 Inspector: PASS / FAIL
- [ ] 2.3 Content Browser: PASS / FAIL
- [ ] 2.4 Console: PASS / FAIL
- [ ] 2.5 Voxel Tool Palette: PASS / FAIL

### Phase 3: Transform Gizmo Tests
- [ ] 3.1 Gizmo Rendering: PASS / FAIL
- [ ] 3.2 Translate Mode: PASS / FAIL
- [ ] 3.3 Rotate Mode: PASS / FAIL
- [ ] 3.4 Scale Mode: PASS / FAIL
- [ ] 3.5 Keyboard Shortcuts: PASS / FAIL
- [ ] 3.6 Toolbar Integration: PASS / FAIL

### Phase 4: Selection System Tests
- [ ] 4.1 Single Selection: PASS / FAIL
- [ ] 4.2 Box Selection: PASS / FAIL
- [ ] 4.3 Selection Operations: PASS / FAIL

### Phase 5: File Operations Tests
- [ ] 5.1 New World: PASS / FAIL
- [ ] 5.2 Open World: PASS / FAIL
- [ ] 5.3 Save World: PASS / FAIL
- [ ] 5.4 Save World As: PASS / FAIL
- [ ] 5.5 Import Assets: PASS / FAIL

### Phase 6: Dark Theme Tests
- [ ] 6.1 Theme Colors: PASS / FAIL
- [ ] 6.2 Panel Styling: PASS / FAIL

### Phase 7: Performance Tests
- [ ] 7.1 Frame Rate: PASS / FAIL
- [ ] 7.2 Memory Usage: PASS / FAIL

### Phase 8: Stress Tests
- [ ] 8.1 Rapid Panel Operations: PASS / FAIL
- [ ] 8.2 Rapid Gizmo Switching: PASS / FAIL

### Issues Found:
1. [Issue description]
2. [Issue description]

### Overall Result: PASS / FAIL
```

---

## Automated Testing

For continuous integration, consider these automated tests:

### Unit Tests
- Test gizmo mode switching logic
- Test selection system state management
- Test file dialog results
- Test keyboard shortcut mapping

### Integration Tests
- Test panel communication
- Test toolbar-gizmo synchronization
- Test file save/load cycles

### Visual Regression Tests
- Screenshot comparisons for UI layouts
- Theme consistency checks
- Gizmo rendering validation

---

## Reporting Bugs

When reporting GUI bugs, include:

1. **Description**: Clear description of the issue
2. **Steps to Reproduce**: Exact steps that cause the bug
3. **Expected Behavior**: What should happen
4. **Actual Behavior**: What actually happens
5. **Screenshots/Video**: Visual evidence when possible
6. **System Info**: Windows version, GPU, build version
7. **Console Logs**: Any error messages

---

**Last Updated**: 2025-11-19
**Version**: 1.0
**Next Review**: After GUI implementation complete
