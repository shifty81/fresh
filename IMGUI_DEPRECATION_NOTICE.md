# ImGui Deprecation Notice

**Date:** 2025-11-22  
**Status:** ImGui has been completely removed from Fresh Voxel Engine  

---

## ⚠️ IMPORTANT: ImGui is No Longer Supported

**Fresh Voxel Engine now uses Windows Native Win32 UI exclusively.**

### What This Means

- ❌ **ImGui is NOT installed** - It has been removed from vcpkg.json
- ❌ **ImGui code is NOT compiled** - All ImGui-based panels have been archived
- ❌ **ImGui will NOT be added back** - This is a permanent architectural decision
- ✅ **All UI is Win32 native** - Professional Windows 10/11 appearance and integration

### What Changed

1. **Dependency Removed**: `imgui` removed from vcpkg.json
2. **Source Files Archived**: All ImGui-based panel files moved to `archived/imgui-panels/`
   - MainMenuPanel.h/cpp
   - EngineConfigPanel.h/cpp
   - SettingsPanel.h/cpp
   - WindowsCustomizationPanel.cpp
3. **Replaced By**: Windows Native Win32 UI components
   - Win32MenuBar
   - Win32Toolbar
   - Win32ConsolePanel
   - Win32InspectorPanel
   - Win32SceneHierarchyPanel
   - Win32ContentBrowserPanel
   - Win32SettingsDialog
   - Win32TerraformingPanel
   - Win32HUD

### For Developers

If you see references to ImGui in documentation or code:

1. **In Documentation**: These are historical references. Ignore ImGui instructions and use Win32 equivalents.
2. **In Code Comments**: These explain what was replaced. They're kept for historical context.
3. **In Example Code**: Update examples to use Win32 native UI instead.

### Why This Decision?

Fresh Voxel Engine is a **Windows-exclusive gaming platform**. Using native Win32 UI provides:

- ✅ **Better Performance**: No UI framework overhead
- ✅ **Professional Appearance**: Native Windows 10/11 look and feel
- ✅ **Smaller Binary**: ~500KB-1MB reduction
- ✅ **Faster Builds**: One less dependency
- ✅ **Better Integration**: Taskbar, themes, DPI, jump lists
- ✅ **Simpler Codebase**: Single UI system

### Related Documentation

- **Policy**: See [WINDOWS_NATIVE_ONLY_POLICY.md](WINDOWS_NATIVE_ONLY_POLICY.md)
- **Architecture**: See [docs/architecture/UI_ARCHITECTURE.md](docs/architecture/UI_ARCHITECTURE.md)
- **GUI Guide**: See [docs/editor/GUI_GUIDE.md](docs/editor/GUI_GUIDE.md)
- **Integration**: See [docs/architecture/WINDOWS_INTEGRATION.md](docs/architecture/WINDOWS_INTEGRATION.md)

### Migration Guide

If you have code that references ImGui:

**Before (ImGui):**
```cpp
#include "imgui.h"

void render() {
    ImGui::Begin("My Panel");
    ImGui::Text("Hello World");
    ImGui::End();
}
```

**After (Win32):**
```cpp
#include "ui/native/Win32Panel.h"

class MyPanel : public Win32Panel {
public:
    void render() {
        // Use native Win32 controls
        // See Win32Panel, Win32ConsolePanel, etc. for examples
    }
};
```

---

## Questions?

- See [WINDOWS_NATIVE_ONLY_POLICY.md](WINDOWS_NATIVE_ONLY_POLICY.md) for full policy details
- See [docs/editor/GUI_GUIDE.md](docs/editor/GUI_GUIDE.md) for Win32 UI usage
- Open a [GitHub Discussion](https://github.com/shifty81/fresh/discussions) for help

---

**This is a permanent architectural decision. ImGui will not be re-added to the project.**
