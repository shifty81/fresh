# Automated Build Script Guide

This guide explains how to use the automated build script that sets up and builds Fresh Voxel Engine with a single command on Windows.

## Overview

Fresh Voxel Engine provides an automated PowerShell build script for Windows 10/11:

- **Windows:** `setup-and-build.ps1` (PowerShell)

**Note:** Cross-platform scripts for Linux/macOS have been archived. Fresh is now Windows-exclusive.

This script automates the entire build process:

1. ✅ Check prerequisites (Visual Studio, CMake, Git)
2. ✅ Install/verify dependencies via vcpkg
3. ✅ Generate Visual Studio solution
4. ✅ Build the project with DirectX support
5. ✅ (Optional) Run tests

---

## Windows: PowerShell Script

### Quick Start

#### Option 1: Full Automated Build (Recommended)

Open PowerShell in the project root directory and run:

```powershell
.\setup-and-build.ps1
```

This will:
- Check all prerequisites (Visual Studio 2022, CMake, Git)
- Set up vcpkg if needed
- Install DirectX SDK dependencies (via Windows SDK)
- Generate the Visual Studio 2022 solution
- Build the Release configuration
- Open Visual Studio 2022 with the solution

**Time:** 10-20 minutes on first run (includes downloading dependencies)

#### Option 2: Setup Only (No Build)

If you want to just set up and generate the solution without building:

```powershell
.\setup-and-build.ps1 -SkipBuild
```

You can then build manually in Visual Studio.

#### Option 3: Build Debug Configuration

To build the Debug configuration instead of Release:

```powershell
.\setup-and-build.ps1 -BuildConfig Debug
```

### Prerequisites

The script will check for these prerequisites and guide you if anything is missing:

#### Required
- **Windows 10/11** (x64)
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - With "Desktop development with C++" workload
  - Download: https://visualstudio.microsoft.com/downloads/
- **CMake 3.20+**
  - Download: https://cmake.org/download/
  - Must be in system PATH
- **Git for Windows**
  - Download: https://git-scm.com/download/win

#### Optional
- **.NET 9 SDK** (for C# bindings)
  - Download: https://dotnet.microsoft.com/download/dotnet/9.0

### Script Parameters

The script supports several optional parameters:

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `-SkipBuild` | Switch | false | Skip the build step, only generate solution |
| `-BuildConfig` | String | "Release" | Build configuration: "Debug" or "Release" |
| `-SkipVcpkgBootstrap` | Switch | false | Skip vcpkg bootstrap if already exists |
| `-OpenVS` | Boolean | true | Open Visual Studio after completion |

### Examples

**Build Release configuration (default):**
```powershell
.\setup-and-build.ps1
```

**Build Debug configuration:**
```powershell
.\setup-and-build.ps1 -BuildConfig Debug
```

**Setup and generate solution without building:**
```powershell
.\setup-and-build.ps1 -SkipBuild
```

**Don't open Visual Studio automatically:**
```powershell
.\setup-and-build.ps1 -OpenVS $false
```

**Build Debug without opening Visual Studio:**
```powershell
.\setup-and-build.ps1 -BuildConfig Debug -OpenVS $false
```

---


## Archived Cross-Platform Support

**Note:** The Linux/macOS bash script (`setup-and-build.sh`) has been archived. Fresh Voxel Engine is now Windows-exclusive.

For archived build scripts, see `archived/cross-platform/setup-and-build.sh`.

See [ARCHIVED.md](ARCHIVED.md) for more information about archived features.

---

### First Time Setup
```powershell
# Clone repository
git clone https://github.com/shifty81/fresh.git
cd fresh

# Run automated setup and build
.\setup-and-build.ps1

# Visual Studio opens - set FreshVoxelEngine as startup project
# Press F5 to run
```

### Daily Development
1. Open `build\FreshVoxelEngine.sln` in Visual Studio
2. Make code changes
3. Press F7 to build
4. Press F5 to run

### After Pulling New Changes
```powershell
# If CMakeLists.txt or dependencies changed
.\setup-and-build.ps1

# Otherwise, just rebuild in Visual Studio
```

## Additional Resources

- **Build Instructions:** [BUILD.md](BUILD.md) - Manual build process
- **Visual Studio Setup:** [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Detailed VS2022 guide
- **Developer Setup:** [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - Development environment
- **Getting Started:** [GETTING_STARTED.md](GETTING_STARTED.md) - First steps with the engine
- **Controls:** [CONTROLS.md](CONTROLS.md) - How to use the engine
- **Documentation Index:** [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) - All documentation

## Support

If you encounter issues:

1. Check the error messages - they usually indicate the problem
2. Review the Troubleshooting section above
3. See [BUILD.md](BUILD.md) for detailed build instructions
4. Open an issue: https://github.com/shifty81/fresh/issues

## Summary

The `setup-and-build.ps1` script provides a streamlined, automated way to set up and build Fresh Voxel Engine. It handles all the complex setup steps, checks prerequisites, and gets you to a working build with a single command.

**Quick Command:**
```powershell
.\setup-and-build.ps1
```

**Expected Time:**
- First run: 10-20 minutes (includes dependency download/build)
- Subsequent runs: 2-5 minutes (just build)

**Result:**
- Visual Studio 2022 opens with solution
- Project is built and ready to run
- You're ready to develop!
