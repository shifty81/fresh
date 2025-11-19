# Windows-Only Transition Summary

## Overview

Fresh Voxel Engine has been transitioned from a cross-platform engine to a **Windows-exclusive gaming development platform** focused on providing the best possible game creation experience with DirectX rendering and an Unreal Engine-like editor.

**Date:** November 2025  
**Version:** Post-transition to Windows-only

---

## What Changed

### 1. Platform Support

**Before:**
- Windows 10/11 (DirectX 11/12 + OpenGL)
- Linux (OpenGL 4.5+)
- macOS (OpenGL)

**After:**
- **Windows 10/11 ONLY** (DirectX 12/11 primary, OpenGL optional)
- Linux and macOS support archived

### 2. Build System

**Before:**
- Cross-platform CMake with platform detection
- Linux/macOS: system packages via apt/brew
- Windows: vcpkg for dependencies

**After:**
- **Windows-only CMake** - Rejects non-Windows platforms
- **Visual Studio 2022 exclusive**
- vcpkg for all dependencies
- DirectX SDK via Windows 10 SDK

### 3. Graphics API Priority

**Before:**
- OpenGL 4.5+ (default, cross-platform)
- DirectX 11/12 (Windows alternative)

**After:**
- **DirectX 12** (primary for Windows 10/11)
- **DirectX 11** (compatibility fallback)
- OpenGL 4.5+ (optional for compatibility testing)

### 4. Documentation

**Updated Files:**
- `README.md` - Windows-exclusive, Unreal-like editor focus
- `BUILD.md` - Windows-only build instructions
- `AUTOMATED_BUILD.md` - PowerShell script only
- `CMakeLists.txt` - Windows platform requirement
- `.github/workflows/README.md` - Windows CI only

**New Files:**
- `ARCHIVED.md` - Comprehensive guide to archived features
- `WINDOWS_ONLY_TRANSITION.md` - This file

**Archived Files:**
- `setup-and-build.sh` → `archived/cross-platform/`
- `tools/validate_vcpkg_baseline.sh` → `archived/cross-platform/`
- `asset_packs/create_pack.sh` → `archived/cross-platform/`

---

## New Features Emphasized

### Unreal Engine-Like Editor

The editor now emphasizes professional game development capabilities:

- **Professional Game Editor** - Comprehensive ImGui-based editor
- **Windows Native Integration** - Native themes, dialogs, taskbar features
- **Terraforming Tools** - World building with brush, sphere, cube, flatten, smooth, paint
- **Scene Hierarchy** - Visual world structure and entity browser
- **Content Browser** - Complete asset management
- **Entity Inspector** - Edit properties of selected NPCs and objects
- **Play-in-Editor** - Test gameplay instantly without leaving editor

### NPC Creation System (In Development)

Easy-to-use NPC system integrated into the editor:

**Features:**
- **Visual NPC Placement** - Click to add NPCs to world
- **Behavior Dropdown** - Select from predefined behaviors:
  - Friendly (villagers, peaceful NPCs)
  - Trader (wandering merchants, shop keepers)
  - Hostile (enemies, monsters)
  - Patrol (guards with routes)
  - Guard (stationary defense)
  - Wander (ambient creatures)
- **No Coding Required** - Game designers can add NPCs without programming
- **Integration** - Uses existing character, AI, and behavior tree systems

**Implementation Status:**
- ✅ Character system ready
- ✅ AI and behavior tree framework ready  
- ✅ Documentation updated
- 🚧 Editor UI integration (in development)
- 🚧 Behavior dropdown implementation (in development)
- 🚧 Visual NPC placement (in development)

---

## Rationale

### Why Windows-Only?

1. **Focused Development** - Single platform allows:
   - Better DirectX optimization
   - Native Windows features
   - More polished editor experience

2. **Learning Environment** - Primary developer:
   - Learning on Windows platform
   - Cannot test on Linux/macOS currently
   - Focused expertise development

3. **Gaming Market** - Windows dominates PC gaming:
   - DirectX 12/11 cutting-edge graphics
   - Visual Studio 2022 superior tools
   - Windows 10/11 SDK optimizations

4. **Professional Tools** - Game development needs:
   - Native Windows UI
   - DirectX real-time preview
   - Visual Studio debugging

### Why Unreal-Like Editor?

1. **Industry Standard** - Unreal Engine is the gold standard
2. **Designer Friendly** - No coding required for basic game creation
3. **Visual Workflow** - Click, drag, configure instead of scripting
4. **Rapid Iteration** - Test gameplay instantly in editor

---

## Migration Guide

### For Developers

**If you were using Linux/macOS:**
1. Install Windows 10/11 (dual boot or VM)
2. Install Visual Studio 2022
3. Follow Windows build instructions
4. See `ARCHIVED.md` for old cross-platform files

**If you were using Windows:**
- No changes needed!
- DirectX is now primary renderer
- Continue using Visual Studio 2022

### For Contributors

**Contributing to Windows-only project:**
1. Focus on Windows platform only
2. Test with DirectX 12/11
3. Use Visual Studio 2022
4. Submit PRs targeting Windows features

**Maintaining cross-platform fork:**
- Feel free to fork and maintain Linux/macOS support
- Archived scripts available in `archived/cross-platform/`
- Original cross-platform code preserved in git history

---

## Build Instructions Quick Reference

### Prerequisites
- Windows 10/11 (x64)
- Visual Studio 2022 with "Desktop development with C++"
- CMake 3.20+
- Git for Windows

### Quick Build
```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Automated build (recommended)
.\setup-and-build.ps1

# Manual build
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..
generate_vs2022.bat
cd build
cmake --build . --config Release
Release\FreshVoxelEngine.exe
```

---

## Future Plans

### Short Term
- Complete NPC editor UI implementation
- Behavior dropdown system
- Visual NPC placement tool
- Integrate with existing systems

### Medium Term
- Enhanced DirectX 12 optimizations
- More Windows-native features
- Expanded NPC behavior types
- AI behavior visual scripting

### Long Term
- Complete game creation without coding
- Asset marketplace integration
- One-click game publishing
- Potential cross-platform reconsideration (community driven)

---

## Resources

### Documentation
- [README.md](README.md) - Project overview
- [BUILD.md](BUILD.md) - Build instructions
- [AUTOMATED_BUILD.md](AUTOMATED_BUILD.md) - Automated build script
- [ARCHIVED.md](ARCHIVED.md) - Archived features
- [WINDOWS_INTEGRATION.md](WINDOWS_INTEGRATION.md) - Windows features

### Support
- GitHub Issues: https://github.com/shifty81/fresh/issues
- GitHub Discussions: https://github.com/shifty81/fresh/discussions

### Archived
- Cross-platform scripts: `archived/cross-platform/`
- Git history: Previous cross-platform commits

---

## Questions?

**Q: Will cross-platform support return?**  
A: Possibly, if community contributors maintain it or the project matures sufficiently on Windows.

**Q: Can I still use the old cross-platform code?**  
A: Yes! Check `archived/cross-platform/` and git history. You can fork and maintain your own cross-platform version.

**Q: Why not use OpenGL as primary on Windows?**  
A: DirectX 12/11 provides better Windows integration, performance, and access to Windows-specific features.

**Q: Is this change permanent?**  
A: The focus is Windows-only for now, but we're open to community contributions for other platforms.

**Q: When will the NPC editor be complete?**  
A: It's in active development. The foundation (character, AI, behavior trees) is ready. UI integration is next.

---

**Last Updated:** November 2025  
**Status:** Windows-only transition complete  
**Next:** NPC editor UI implementation
