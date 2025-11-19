# Windows Native UI Migration Plan

## Executive Summary

This document outlines the plan to migrate the Fresh Voxel Engine from ImGui-based UI to Windows native Win32 UI components. This addresses the requirement to "move to windows native only" and remove ImGui dependency.

## Current State Analysis

### ImGui Usage

**Total ImGui References:** 1073 occurrences across the codebase

**Major ImGui Components:**
- Editor panels (Content Browser, Inspector, Scene Hierarchy, Console, etc.)
- Menu bars and toolbars
- Settings dialogs
- Main menu system
- Hotbar and tool palettes
- Debug overlays

### Existing Win32 Infrastructure

Already implemented:
- ✅ `Win32Window` - Native window creation and management
- ✅ `Win32InputManager` - Native input handling
- ✅ Windows-specific integration (DPI, themes, taskbar, etc.)

Still using ImGui:
- ❌ All UI panels and controls
- ❌ Rendering pipeline for UI elements
- ❌ Layout and docking system

## Migration Strategy

### Approach: Incremental Hybrid Migration

Instead of a big-bang rewrite, we'll use a **phased migration** approach:

1. **Maintain ImGui temporarily** for complex panels
2. **Implement Win32 replacements** one component at a time
3. **Test thoroughly** after each component migration
4. **Remove ImGui completely** once all components are migrated

### Why Not Immediate Removal?

❌ **Big Bang Approach Risks:**
- Loss of functionality for extended period
- Difficult to debug issues
- High risk of introducing bugs
- User experience disruption

✅ **Incremental Approach Benefits:**
- Always have working UI
- Test each component independently
- Roll back individual changes if needed
- Maintain user productivity

## Phase-by-Phase Plan

### Phase 2A: Core Infrastructure (2-3 weeks)

**Goal:** Establish foundation for Win32 UI components

1. **Create Win32 UI Base Classes**
   ```cpp
   class Win32Panel {
       HWND m_hwnd;
       virtual void onCreate() = 0;
       virtual void onPaint() = 0;
       virtual void onResize() = 0;
   };
   
   class Win32MenuBar {
       HMENU m_menu;
       void addMenu(const std::string& name);
       void addMenuItem(const std::string& menu, const std::string& item, int id);
   };
   
   class Win32Toolbar {
       HWND m_hwnd;
       void addButton(int id, const std::string& text, HICON icon);
   };
   ```

2. **Implement Docking System**
   - Use Win32 splitter windows
   - Implement drag-and-drop docking
   - Save/restore layouts

3. **Create Common Controls Wrappers**
   - TreeView (for scene hierarchy)
   - ListView (for content browser)
   - PropertyGrid (for inspector)
   - RichEdit (for console)

**Deliverables:**
- Win32 UI framework classes
- Docking system implementation
- Common control wrappers
- Documentation and examples

### Phase 2B: Simple Panels (2-3 weeks)

**Goal:** Migrate simple, non-critical panels first

**Order of Migration:**

1. **Console Panel** (Simplest)
   - Text output only
   - Win32 RichEdit control
   - Command input textbox

2. **Settings Panel**
   - Property sheets
   - Tab controls
   - Standard Win32 controls

3. **Hotbar Panel**
   - Custom draw owner-draw toolbar
   - Icon buttons
   - Keyboard shortcuts

**Testing Checklist per Panel:**
- [ ] Visual appearance matches original
- [ ] All functionality works
- [ ] No crashes or memory leaks
- [ ] Performance is acceptable
- [ ] Keyboard shortcuts work

### Phase 2C: Complex Panels (4-5 weeks)

**Goal:** Migrate feature-rich panels

1. **Content Browser**
   - ListView with icons
   - File operations (drag, copy, paste)
   - Thumbnail generation
   - Search and filtering

2. **Scene Hierarchy**
   - TreeView control
   - Drag-and-drop reordering
   - Multi-selection
   - Context menus

3. **Inspector Panel**
   - Property grid
   - Different control types per property
   - Real-time updates
   - Undo/redo integration

4. **Tool Palettes**
   - Voxel tool palette
   - Material picker
   - Custom drawn UI

### Phase 2D: Menu and Toolbar (2 weeks)

**Goal:** Replace ImGui menu bar and toolbars

1. **Main Menu Bar**
   - File, Edit, View, Tools, Help menus
   - Standard Win32 menu
   - Keyboard accelerators

2. **Editor Toolbar**
   - Tool buttons with icons
   - Standard Win32 toolbar
   - Tooltips

3. **Context Menus**
   - Right-click menus
   - Context-sensitive options

### Phase 2E: ImGui Removal (1 week)

**Goal:** Remove ImGui dependency completely

1. **Remove ImGui includes**
2. **Remove ImGui from CMakeLists.txt**
3. **Remove ImGui from vcpkg.json**
4. **Remove ImGuiContext class**
5. **Clean up conditional compilation**
6. **Update documentation**

## Alternative Approach: ImGui + Win32 Backend

### Option B: Keep ImGui, Use Win32 Backend Only

Instead of removing ImGui entirely, we could:

**Pros:**
- ✅ Much faster implementation (1-2 weeks vs 3 months)
- ✅ Keep all existing UI code
- ✅ ImGui already has Win32 backend (imgui_impl_win32.cpp)
- ✅ Still "Windows native" rendering (uses Win32 APIs)
- ✅ No loss of functionality

**Cons:**
- ❌ Still dependent on ImGui library
- ❌ Not "pure" Win32 controls
- ❌ Custom rendering instead of native controls

**Implementation:**
```cpp
// Already using Win32 backend!
ImGui_ImplWin32_Init(hwnd);
ImGui_ImplWin32_NewFrame();
// ... render ImGui ...
ImGui_ImplWin32_Shutdown();
```

### Current Reality Check

**We're already using Win32 backend!** The codebase has:
- `Win32Window` - Native window
- `Win32InputManager` - Native input
- `ImGui_ImplWin32` - Win32 ImGui backend

This means:
- ✅ Window creation is Win32 native
- ✅ Input handling is Win32 native  
- ✅ ImGui renders using Win32 APIs
- ❌ UI controls are ImGui (not native Win32 controls)

## Recommendation

### Hybrid Approach: ImGui for Editor, Win32 for Game

**Rationale:**

1. **Editor UI Benefits from ImGui:**
   - Rapid development
   - Rich features out-of-box
   - Good enough for tool UI
   - Many game engines use it (Unreal uses Slate, Unity uses UIElements, but many tools use ImGui)

2. **Game UI Uses Native:**
   - Main menu → Win32
   - HUD → Custom rendering
   - Dialogs → Win32

3. **Best of Both Worlds:**
   - Fast iteration for editor
   - Native experience for players
   - Easier maintenance

### Minimum Viable Migration

If the goal is "remove ImGui entirely," the minimum path is:

**Priority 1 (Required for players):**
1. Main menu → Win32 dialogs ⚠️ **ALREADY DONE** (MainMenuPanel uses ImGui but could be Win32)
2. HUD elements → Custom rendering or Win32

**Priority 2 (Editor can keep ImGui):**
3. Editor panels → Can stay ImGui

**Timeline:** 2-3 weeks for Priority 1

## Detailed Implementation Plan (Full Migration)

### Month 1: Foundation

**Week 1-2: Base Classes**
- [ ] Create Win32Panel base class
- [ ] Implement docking manager
- [ ] Create common control wrappers
- [ ] Set up unit tests

**Week 3-4: First Panel Migration**
- [ ] Migrate Console Panel
- [ ] Test thoroughly
- [ ] Document approach
- [ ] Get user feedback

### Month 2: Core Panels

**Week 5-6: Content Browser**
- [ ] Implement Win32 ListView-based browser
- [ ] File operations
- [ ] Thumbnail generation
- [ ] Search functionality

**Week 7-8: Scene Hierarchy**
- [ ] Implement Win32 TreeView-based hierarchy
- [ ] Drag-and-drop
- [ ] Context menus
- [ ] Multi-selection

### Month 3: Remaining Panels

**Week 9-10: Inspector Panel**
- [ ] Property grid implementation
- [ ] Custom property editors
- [ ] Undo/redo integration

**Week 11: Tool Palettes**
- [ ] Voxel tool palette
- [ ] Material picker
- [ ] Custom drawing

**Week 12: Cleanup**
- [ ] Remove ImGui
- [ ] Update documentation
- [ ] Performance testing
- [ ] Bug fixes

## Technical Challenges

### Challenge 1: Layout Management

**ImGui Approach:**
```cpp
ImGui::Begin("Window");
ImGui::Text("Hello");
ImGui::Button("Click me");
ImGui::End();
```

**Win32 Approach:**
```cpp
// Much more code!
HWND hwnd = CreateWindowEx(...);
CreateWindow("STATIC", "Hello", ...);
CreateWindow("BUTTON", "Click me", ...);
// Handle WM_SIZE for layout
// Handle WM_COMMAND for button clicks
```

**Solution:** Create layout manager helper classes

### Challenge 2: Immediate vs Retained Mode

**ImGui:** Immediate mode (redraw every frame)
**Win32:** Retained mode (update on change)

**Solution:** Adapt mental model, implement smart updating

### Challenge 3: Custom Rendering

**ImGui:** Easy custom drawing
**Win32:** More complex (GDI+, Direct2D)

**Solution:** Use Direct2D for custom rendering

## Testing Strategy

### Automated Testing
```cpp
// Unit tests for Win32 controls
TEST(Win32Panel, CreateAndDestroy) {
    Win32Panel panel;
    ASSERT_TRUE(panel.create());
    panel.destroy();
}

// Integration tests
TEST(EditorUI, AllPanelsVisible) {
    Editor editor;
    editor.initialize();
    ASSERT_TRUE(editor.isVisible("Console"));
    ASSERT_TRUE(editor.isVisible("Inspector"));
}
```

### Manual Testing
- Visual comparison screenshots (before/after)
- Functionality checklist per panel
- Performance benchmarks
- User acceptance testing

## Risk Assessment

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Loss of functionality | HIGH | MEDIUM | Incremental migration, thorough testing |
| Performance degradation | MEDIUM | LOW | Benchmark each panel |
| Extended timeline | MEDIUM | HIGH | Prioritize critical panels first |
| User disruption | HIGH | MEDIUM | Maintain backward compatibility during migration |
| Developer time | HIGH | HIGH | Consider hybrid approach |

## Success Criteria

### Functional Success
- [ ] All UI panels have Win32 equivalents
- [ ] No loss of functionality
- [ ] All keyboard shortcuts work
- [ ] Drag-and-drop operations work
- [ ] Docking and layout saving works

### Non-Functional Success
- [ ] Performance equal or better than ImGui
- [ ] Memory usage equal or lower
- [ ] Startup time not significantly slower
- [ ] Build size reduced (no ImGui library)
- [ ] Native Windows look and feel

### User Success
- [ ] Users can accomplish same tasks
- [ ] No increase in clicks/actions needed
- [ ] Familiar Windows UI patterns
- [ ] Positive user feedback

## Resources Needed

### Development Time
- **Full migration:** 3 months (1 developer full-time)
- **Hybrid approach:** 2-3 weeks (Priority 1 only)
- **Recommended:** 1-2 months (incremental, tested)

### Skills Required
- Win32 API expertise
- GDI+/Direct2D for custom rendering
- C++ UI development
- Testing and QA

### Tools and Libraries
- Windows SDK
- Direct2D (for custom rendering)
- Windows Common Controls
- Visual Studio (for UI design)

## Conclusion

### Recommended Path Forward

**Option 1: Full Win32 Migration** (If removing ImGui is mandatory)
- Timeline: 3 months
- Risk: Medium-High
- Benefit: Pure Windows native, no ImGui dependency
- **When:** If "no ImGui" is hard requirement

**Option 2: Hybrid Approach** (Recommended)
- Timeline: 2-3 weeks
- Risk: Low
- Benefit: Native player experience, productive editor
- **When:** Most practical for this project

**Option 3: Current State + Improvements** (Pragmatic)
- Timeline: Ongoing
- Risk: Very Low
- Benefit: Focus on features, not UI migration
- **When:** If "Windows native" means Win32Window + Win32Input (already done)

### Next Steps

1. **Clarify requirements** - Does "Windows native only" mean:
   - No cross-platform libraries at all? (Remove GLFW, ImGui, etc.)
   - Native window and input? (Already done!)
   - Native UI controls? (Requires full migration)

2. **Get stakeholder approval** - Which approach to take?

3. **Begin implementation** - Start with approved approach

4. **Regular check-ins** - Review progress weekly

---

**Document Version:** 1.0.0  
**Last Updated:** 2025-11-16  
**Status:** 📋 Planning  
**Decision Needed:** Choose migration approach  
