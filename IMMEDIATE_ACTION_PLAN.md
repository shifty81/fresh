# Fresh Voxel Engine - Immediate Action Plan

> **Prioritized Implementation Guide** for the next 90 days  
> **Last Updated:** 2025-11-15  
> **Purpose:** Clear, actionable steps to move the project forward

---

## 🎯 30-60-90 Day Plan

### Days 1-30: Critical Foundation (Sprint 1)

#### Week 1: Voxel Selection System - Part 1
**Goal:** Basic selection working

**Tasks:**
1. [ ] Create `SelectionManager` class in `editor/`
   - Data structure for selected voxels
   - Selection bounds calculation
   - Add/remove individual voxels
   
2. [ ] Implement box selection with mouse
   - Start selection on mouse down
   - Update selection region as mouse drags
   - Finalize selection on mouse up
   
3. [ ] Add selection visualization
   - Wireframe outline rendering
   - Highlight selected voxels
   - Distinct color for selection

**Deliverable:** Can select regions of voxels with mouse drag

---

#### Week 2: Voxel Selection System - Part 2
**Goal:** Complete selection features

**Tasks:**
1. [ ] Add selection manipulation
   - Move selected voxels (arrow keys)
   - Delete selected voxels (Delete key)
   - Clear selection (Esc key)
   
2. [ ] Implement selection clipboard
   - Copy to clipboard (Ctrl+C)
   - Store voxel data and positions
   - Serialize/deserialize selection

3. [ ] Add selection modes
   - Box select (current)
   - Brush select (paint selection)
   - Wand select (similar voxels)

**Deliverable:** Full selection system with multiple modes

---

#### Week 3: File Dialog Integration
**Goal:** Native file dialogs working

**Tasks:**
1. [ ] Add NFD to vcpkg.json
   ```json
   {
     "name": "nfd",
     "features": ["extended"]
   }
   ```

2. [ ] Create `FileDialogManager` class
   - Open file dialog wrapper
   - Save file dialog wrapper
   - Multi-file select dialog

3. [ ] Integrate with editor menu
   - Open World → show open dialog
   - Save World As → show save dialog
   - Import Assets → show multi-file dialog

4. [ ] Test on Windows, Linux, macOS

**Deliverable:** File operations use native dialogs

---

#### Week 4: Cut/Copy/Paste Implementation
**Goal:** Clipboard operations working

**Tasks:**
1. [ ] Implement Cut (Ctrl+X)
   - Copy selection to clipboard
   - Delete selected voxels
   - Show "cut" feedback

2. [ ] Implement Copy (Ctrl+C)
   - Already done in Week 2
   - Test thoroughly

3. [ ] Implement Paste (Ctrl+V)
   - Restore voxels from clipboard
   - Position at cursor or player location
   - Preview before placing (ghost blocks)

4. [ ] Add undo/redo integration
   - All clipboard operations undoable
   - Test undo/redo with large selections

**Deliverable:** Full Cut/Copy/Paste workflow

---

### Days 31-60: Resource Loading (Sprint 2)

#### Week 5: Image Loading with stb_image
**Goal:** Load real textures from files

**Tasks:**
1. [ ] Add stb_image to project
   ```cpp
   #define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"
   ```

2. [ ] Update `Texture::loadFromFile()`
   - Use stb_image to load PNG, JPG, TGA
   - Create GPU texture from loaded data
   - Handle different formats (RGB, RGBA)

3. [ ] Implement for all backends
   - OpenGL texture upload
   - DirectX 11 texture creation
   - DirectX 12 texture creation

4. [ ] Test with various texture files
   - Different sizes
   - Different formats
   - Error handling

**Deliverable:** Can load custom textures

---

#### Week 6: Model Loading with tinyobjloader
**Goal:** Load 3D models from files

**Tasks:**
1. [ ] Add tinyobjloader to vcpkg.json

2. [ ] Update `ModelLoader::loadFromFile()`
   - Parse OBJ files
   - Extract vertices, normals, UVs
   - Build mesh structure

3. [ ] Create GPU mesh buffers
   - Vertex buffer creation
   - Index buffer creation
   - For all rendering backends

4. [ ] Add to Content Browser
   - Show model preview
   - Drag and drop to scene

**Deliverable:** Can load and render OBJ models

---

#### Week 7: Audio Format Support ✅ COMPLETE
**Goal:** Support more audio formats

**Tasks:**
1. [x] Add libvorbis to vcpkg.json
2. [x] Implement OGG/Vorbis loader
   - Decode OGG files
   - Feed to OpenAL
   - Support streaming for music
3. [x] Update CMakeLists.txt for libvorbis
4. [x] Create universal audio file loader
5. [x] Update play2D(), play3D(), playMusic() to use new loader
6. [x] Add documentation (AUDIO_FORMAT_SUPPORT.md)

**Deliverable:** Multi-format audio support ✅

---

#### Week 8: Resource System Polish
**Goal:** Robust resource loading

**Tasks:**
1. [ ] Add async loading
   - Load resources in background thread
   - Show loading indicator
   - Don't block main thread

2. [ ] Implement resource caching
   - Don't reload same resource twice
   - Reference counting
   - Memory management

3. [ ] Better error handling
   - Graceful failure for missing files
   - Fallback to placeholder assets
   - Clear error messages

4. [ ] Update documentation
   - How to add custom resources
   - Supported formats
   - Asset creation guide

**Deliverable:** Production-ready resource system

---

### Days 61-90: Gameplay Foundation (Sprint 3)

#### Week 9-10: Inventory System
**Goal:** Working inventory

**Tasks:**
1. [ ] Create `InventorySystem` class
   - Item data structure (ID, count, metadata)
   - Inventory container (array or map)
   - Add/remove/stack items

2. [ ] Build inventory UI
   - Grid layout with ImGui
   - Drag and drop items
   - Item tooltips
   - Split stack (shift+click)

3. [ ] Add item types
   - Define 20+ items
   - Icons for each item
   - Item properties (stackable, usable, etc.)

4. [ ] Integrate with player
   - Player has inventory
   - Picking up blocks adds to inventory
   - Placing blocks consumes from inventory

5. [ ] Hotbar implementation
   - Quick access to 9 items
   - Number keys to select
   - Visual indicator

**Deliverable:** Complete inventory system

---

#### Week 11-12: Crafting System
**Goal:** Working crafting

**Tasks:**
1. [ ] Define recipe system
   ```cpp
   struct Recipe {
       std::vector<ItemStack> inputs;
       ItemStack output;
       CraftingStation requiredStation;
   };
   ```

2. [ ] Create `CraftingSystem` class
   - Recipe registry
   - Check if player can craft
   - Consume inputs, give output

3. [ ] Build crafting UI
   - Show available recipes
   - Crafting grid (3×3)
   - Recipe filter/search
   - Craft button

4. [ ] Define initial recipes
   - Wood → Planks
   - Planks → Sticks
   - Cobblestone → Stone
   - Stone + Sticks → Tools
   - At least 20 recipes

5. [ ] Crafting stations
   - Workbench block
   - Furnace block
   - Smithing table

**Deliverable:** Working crafting loop

---

#### Week 13: Day/Night Cycle
**Goal:** Dynamic time of day

**Tasks:**
1. [ ] Create `TimeManager` class
   - Current time (0-24000 ticks)
   - Time progression rate
   - Pause/resume time

2. [ ] Implement sun/moon movement
   - Calculate sun position from time
   - Calculate moon position
   - Smooth transitions

3. [ ] Update lighting
   - Ambient light intensity
   - Sun light direction
   - Sky color gradient

4. [ ] Add sky rendering
   - Day sky (blue gradient)
   - Sunset/sunrise colors
   - Night sky (dark blue/black)
   - Stars at night (optional)

5. [ ] Time controls
   - Set time command
   - Skip to dawn/dusk/noon/midnight
   - Time display on HUD

**Deliverable:** Full day/night cycle

---

## 📋 Quick Implementation Checklist

### Must Complete (Critical Path)

- [ ] **Voxel Selection System**
  - [ ] Selection data structure
  - [ ] Box selection with mouse
  - [ ] Selection visualization
  - [ ] Selection manipulation
  - [ ] Clipboard integration

- [ ] **File Dialogs**
  - [ ] Add NFD library
  - [ ] FileDialogManager class
  - [ ] Integration with menus
  - [ ] Cross-platform testing

- [ ] **Cut/Copy/Paste**
  - [ ] Cut implementation
  - [ ] Paste implementation
  - [ ] Preview before paste
  - [ ] Undo/redo integration

### Should Complete (High Value)

- [ ] **Resource Loading**
  - [ ] stb_image integration
  - [ ] tinyobjloader integration
  - [ ] Audio format support
  - [ ] Async loading

- [ ] **Inventory System**
  - [ ] Item data structures
  - [ ] Inventory UI
  - [ ] Drag and drop
  - [ ] Hotbar

- [ ] **Crafting System**
  - [ ] Recipe system
  - [ ] Crafting UI
  - [ ] Initial recipes
  - [ ] Crafting stations

- [ ] **Day/Night Cycle**
  - [ ] Time manager
  - [ ] Sun/moon movement
  - [ ] Dynamic lighting
  - [ ] Sky rendering

### Nice to Have (Polish)

- [ ] Selection modes (brush, wand)
- [ ] Model preview in browser
- [ ] Audio streaming
- [ ] Resource caching
- [ ] Item tooltips
- [ ] Crafting search
- [ ] Time commands
- [ ] Stars at night

---

## 🚀 Getting Started Today

### Set Up Your Development Environment

1. **Clone the repo** (if you haven't)
   ```bash
   git clone https://github.com/shifty81/fresh.git
   cd fresh
   ```

2. **Create a feature branch**
   ```bash
   git checkout -b feature/voxel-selection
   ```

3. **Build the project**
   ```bash
   ./setup-and-build.sh  # Linux/Mac
   .\setup-and-build.ps1  # Windows
   ```

4. **Run tests**
   ```bash
   cd build
   ctest --output-on-failure
   ```

---

### Your First Task: Voxel Selection - Day 1

**Goal for Today:** Create the SelectionManager class

1. **Create header file** `include/editor/SelectionManager.h`:
   ```cpp
   #pragma once
   #include <vector>
   #include <glm/glm.hpp>
   #include "voxel/VoxelTypes.h"
   
   struct VoxelPosition {
       int x, y, z;
       // Operators for map/set
   };
   
   class SelectionManager {
   public:
       SelectionManager();
       
       // Selection operations
       void startSelection(const glm::vec3& worldPos);
       void updateSelection(const glm::vec3& worldPos);
       void finalizeSelection();
       void clearSelection();
       
       // Query
       bool hasSelection() const;
       size_t getSelectionSize() const;
       const std::vector<VoxelPosition>& getSelectedVoxels() const;
       
       // Manipulation
       void deleteSelected();
       void moveSelection(const glm::ivec3& delta);
       
   private:
       std::vector<VoxelPosition> m_selectedVoxels;
       glm::vec3 m_selectionStart;
       glm::vec3 m_selectionEnd;
       bool m_isSelecting;
   };
   ```

2. **Create implementation** `src/editor/SelectionManager.cpp`:
   ```cpp
   #include "editor/SelectionManager.h"
   #include <algorithm>
   
   SelectionManager::SelectionManager()
       : m_isSelecting(false)
   {
   }
   
   void SelectionManager::startSelection(const glm::vec3& worldPos)
   {
       m_selectionStart = worldPos;
       m_selectionEnd = worldPos;
       m_isSelecting = true;
       m_selectedVoxels.clear();
   }
   
   void SelectionManager::updateSelection(const glm::vec3& worldPos)
   {
       if (!m_isSelecting) return;
       
       m_selectionEnd = worldPos;
       
       // TODO: Update selected voxels based on box from start to end
   }
   
   void SelectionManager::finalizeSelection()
   {
       m_isSelecting = false;
       // TODO: Build final list of selected voxels
   }
   
   // ... implement other methods
   ```

3. **Add to CMakeLists.txt**:
   ```cmake
   # In the editor section
   include/editor/SelectionManager.h
   src/editor/SelectionManager.cpp
   ```

4. **Test compilation**:
   ```bash
   cd build
   cmake --build . -j$(nproc)
   ```

**Congratulations!** You've started the voxel selection system. Tomorrow you'll add the box selection algorithm.

---

## 📊 Progress Tracking

### Daily Standup Questions

1. **What did I complete yesterday?**
   - List specific tasks done
   - Show working code

2. **What will I do today?**
   - Choose 1-2 tasks from plan
   - Be realistic about time

3. **What blockers do I have?**
   - Technical issues?
   - Need help/review?
   - Missing information?

### Weekly Review

Every Friday, update your progress:

```markdown
## Week N Progress

### Completed
- [ ] Task 1
- [ ] Task 2

### In Progress
- [ ] Task 3 (50%)

### Blocked
- [ ] Task 4 (waiting for X)

### Next Week
- [ ] Task 5
- [ ] Task 6
```

---

## 🤝 Getting Help

### Resources

1. **Documentation**
   - [COMPREHENSIVE_ROADMAP.md](COMPREHENSIVE_ROADMAP.md) - Detailed planning
   - [MISSING_FEATURES_SUMMARY.md](MISSING_FEATURES_SUMMARY.md) - Quick reference
   - [IMPLEMENTATION_STATUS_MATRIX.md](IMPLEMENTATION_STATUS_MATRIX.md) - Status tracking
   - [ARCHITECTURE.md](ARCHITECTURE.md) - System design

2. **Code Examples**
   - Look at existing systems (e.g., TerraformingSystem for undo/redo)
   - Check tests for usage patterns
   - Review ImGui panels for UI examples

3. **Community**
   - GitHub Issues - Ask questions
   - GitHub Discussions - Design discussions
   - Pull Requests - Code reviews

### Getting Unstuck

**If you're stuck for > 30 minutes:**

1. Check documentation
2. Look for similar code
3. Search GitHub issues
4. Ask in Discussions
5. Create a draft PR with questions

---

## ✅ Definition of Done

### For Each Feature

A feature is complete when:

1. **Code is written and tested**
   - Compiles without warnings
   - Unit tests pass (if applicable)
   - Manual testing done

2. **Documentation is updated**
   - Code comments added
   - README updated if needed
   - Examples added if needed

3. **Code is reviewed**
   - PR created and reviewed
   - Feedback addressed
   - Approved by maintainer

4. **CI passes**
   - All tests pass
   - No security issues
   - Code style checks pass

### For Each Sprint

A sprint is complete when:

1. All planned features are done
2. No critical bugs remain
3. Documentation is updated
4. Demo/showcase works
5. Sprint retrospective held

---

## 🎉 Celebration Milestones

### Week 4: First Sprint Complete! 🎊
You've implemented critical editor features. The engine is now truly usable for level design!

**Achievement Unlocked:** Selection Master

### Week 8: Second Sprint Complete! 🎊
Resources can be loaded from real files. The engine can now use custom content!

**Achievement Unlocked:** Resource Wrangler

### Week 13: Third Sprint Complete! 🎊
Basic gameplay loop exists. Inventory, crafting, and day/night cycle work!

**Achievement Unlocked:** Gameplay Guru

---

## 📅 Calendar View

```
Week 1:  [====Selection Part 1====]
Week 2:  [====Selection Part 2====]
Week 3:  [====File Dialogs====]
Week 4:  [====Cut/Copy/Paste====]
         🎊 SPRINT 1 COMPLETE

Week 5:  [====Image Loading====]
Week 6:  [====Model Loading====]
Week 7:  [====Audio Formats====]
Week 8:  [====Resource Polish====]
         🎊 SPRINT 2 COMPLETE

Week 9:  [====Inventory Part 1====]
Week 10: [====Inventory Part 2====]
Week 11: [====Crafting Part 1====]
Week 12: [====Crafting Part 2====]
Week 13: [====Day/Night Cycle====]
         🎊 SPRINT 3 COMPLETE

         ✅ v0.3.0 READY FOR RELEASE
```

---

## 🚦 Success Criteria

### After 90 Days, You Should Have:

1. ✅ Voxel selection system working
2. ✅ File dialogs integrated
3. ✅ Cut/copy/paste functional
4. ✅ Real textures loading
5. ✅ Real models loading
6. ✅ Multi-format audio
7. ✅ Complete inventory system
8. ✅ Working crafting system
9. ✅ Day/night cycle

### Metrics

- **Code Coverage:** 70% → 80%
- **Tests:** 186 → 220+
- **Build Time:** < 5 minutes
- **Zero Critical Bugs**
- **Documentation:** All features documented

---

## 💡 Pro Tips

1. **Start Small** - One feature at a time
2. **Test Often** - Run tests after each change
3. **Commit Regularly** - Save progress frequently
4. **Document as You Go** - Don't wait until the end
5. **Ask for Help** - Don't struggle alone
6. **Take Breaks** - Avoid burnout
7. **Celebrate Wins** - Acknowledge progress

---

**Ready to start? Pick Week 1, Day 1 and let's build! 🚀**

---

**Document Status:** ✅ Ready to Use  
**Last Updated:** 2025-11-15  
**Next Review:** After Sprint 1 completion  
**Maintained By:** Development Team
