# Batch-Only Installation Guide

## Overview

The Fresh Voxel Engine now uses **batch files exclusively** for installation. PowerShell is no longer required.

---

## Why Batch-Only?

✅ **Universal Compatibility**: Works on all Windows versions (7, 8, 10, 11)  
✅ **No Execution Policies**: Avoids PowerShell execution policy issues  
✅ **Corporate Friendly**: Works in restrictive corporate environments  
✅ **Simpler**: One file format, easier to maintain  
✅ **Easier Debugging**: Standard batch syntax everyone knows  
✅ **No Admin Required**: Unlike PowerShell, doesn't trigger UAC prompts  

---

## Installation

Simply double-click `install.bat` in the root directory:

```
fresh/
├── install.bat          <-- Double-click this file
├── CMakeLists.txt
├── vcpkg.json
└── tools/
    └── build_tools/
        ├── install.bat   <-- Main installer script
        ├── build.bat
        ├── clean.bat
        └── rebuild.bat
```

---

## What the Installer Does

### Step 1: Check Prerequisites
- Verifies CMake is installed
- Checks for Visual Studio 2022
- Logs all findings

### Step 2: vcpkg Setup
- Checks if vcpkg is already installed
- Offers to install vcpkg if not found
- Clones vcpkg from GitHub
- Runs bootstrap script
- Integrates with Visual Studio

### Step 3: Configure Dependencies
- Verifies vcpkg manifest mode setup
- Lists required dependencies (GLFW, GLM, ImGui)
- Explains auto-installation during CMake

### Step 4: Generate Visual Studio Project
- Creates `build/` directory
- Runs CMake with vcpkg toolchain
- Automatically downloads and builds dependencies
- Generates `.sln` file

### Step 5: Build the Engine
- Compiles in Release mode
- Shows build progress
- Reports success/failure

### Step 6: Create Shortcuts
- Creates desktop shortcut to .sln file
- Creates shortcut to documentation

---

## Enhancements Over PowerShell Version

### Better Error Handling
```batch
REM Explicit error checking
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Command failed
    echo Error details logged to: %LOG_FILE%
    pause
    exit /b 1
)
```

### Comprehensive Logging
Every step logs to `logs/install_log_TIMESTAMP.txt`:
- All commands executed
- All output captured
- Error details with codes
- User responses recorded

### Clearer Output
```
[Step 3/6] Configuring dependencies...

Checking for vcpkg executable...
✓ vcpkg found at: C:\path\to\vcpkg\vcpkg.exe

This project uses vcpkg manifest mode (vcpkg.json).
Dependencies will be installed automatically during CMake configuration.

Dependencies defined in vcpkg.json:
  - GLFW3 (window management)
  - GLM (math library)
  - ImGui (editor UI)

✓ vcpkg is configured for manifest mode
  Dependencies will be installed in the next step during CMake configuration.

Press any key to continue to CMake configuration...
```

### Step Transitions
Each step explicitly logs:
- Start of step
- Actions taken
- User prompts
- Continuation to next step

---

## Troubleshooting

### Issue: "vcpkg not available"
**Solution**: The installer will offer to install vcpkg. Choose 'Y' to install automatically.

### Issue: "CMake not found"
**Solution**: 
1. Install CMake from https://cmake.org/download/
2. Make sure to check "Add CMake to system PATH" during installation
3. Restart the installer

### Issue: "Visual Studio 2022 not found"
**Solution**:
1. Download VS 2022 Community from https://visualstudio.microsoft.com/downloads/
2. During installation, select "Desktop development with C++"
3. Restart the installer

### Issue: CMake fails during dependency installation
**Solution**:
1. Check `logs/install_log_*.txt` for detailed error
2. Verify internet connection (vcpkg needs to download packages)
3. Try deleting `vcpkg/` folder and run installer again
4. Check if antivirus is blocking downloads

### Issue: Build fails
**Solution**:
1. Check `logs/install_log_*.txt` for compiler errors
2. Verify Visual Studio 2022 is fully installed
3. Try clean build: Run `tools/build_tools/clean.bat` then `install.bat`

---

## Manual Build (If Installer Fails)

If the automated installer doesn't work:

```batch
REM 1. Install vcpkg manually
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
cd ..

REM 2. Generate project
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake ..

REM 3. Build
cmake --build . --config Release

REM 4. Run
Release\FreshVoxelEngine.exe
```

---

## Log Files

All installation activities are logged to:
```
fresh/logs/install_log_YYYY-MM-DD_HH-MM-SS.txt
```

The log contains:
- Installation start time
- Each step executed
- All command output
- Error messages with codes
- User responses
- Installation result

**Always check the log file when reporting issues!**

---

## Comparison: Batch vs PowerShell

| Feature | Batch | PowerShell (Old) |
|---------|-------|------------------|
| Windows Compatibility | All versions | Windows 7+ |
| Execution Policy | Not required | May need bypass |
| Admin Rights | Not required | Sometimes needed |
| Corporate Environments | ✅ Works | ❌ Often blocked |
| Debugging | ✅ Easy | ⚠️ Moderate |
| Error Messages | ✅ Clear | ✅ Clear |
| Logging | ✅ Yes | ✅ Yes |
| User Prompts | ✅ Simple | ✅ Advanced |

---

## Advanced Usage

### Run Specific Steps Only

```batch
REM Just build (skip CMake generation)
tools\build_tools\build.bat

REM Clean build artifacts
tools\build_tools\clean.bat

REM Clean and rebuild everything
tools\build_tools\rebuild.bat
```

### Custom vcpkg Location

Edit `tools/build_tools/install.bat` line 173:
```batch
set "VCPKG_ROOT=C:\your\custom\path\to\vcpkg"
```

### Silent Installation (No Prompts)

Not currently supported, but you can edit the batch file to remove `pause` commands.

---

## For Developers

### Modifying the Installer

The installer is in `tools/build_tools/install.bat` and uses:
- `%BLUE%`, `%GREEN%`, `%RED%`, `%YELLOW%` for colors
- `%LOG_FILE%` for logging
- `%REPO_ROOT%` for repository path
- `%VCPKG_ROOT%` for vcpkg location

### Adding New Dependencies

1. Edit `vcpkg.json` to add dependency
2. Installer will auto-install during CMake step
3. No changes to install.bat needed!

### Testing Installer Changes

1. Delete `build/` folder
2. Delete `vcpkg/` folder (if testing vcpkg installation)
3. Run `install.bat`
4. Check `logs/install_log_*.txt` for issues

---

## Summary

The batch-only installer provides:
- ✅ Universal Windows compatibility
- ✅ Simple, straightforward operation
- ✅ Comprehensive error handling
- ✅ Detailed logging
- ✅ No PowerShell dependencies
- ✅ Corporate environment friendly

**Just double-click `install.bat` and follow the prompts!**

---

**Last Updated**: 2025-11-04  
**Version**: 2.0 (Batch-Only)
