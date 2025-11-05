# Batch-Only Installation Guide

## Overview

The Fresh Voxel Engine uses **batch files exclusively** for installation. PowerShell is no longer required.

**New in Latest Version:**
- ✨ **Manual step-by-step control** with pauses between each step
- ✨ **Enhanced debugging output** to trace execution and identify issues
- ✨ **Clear confirmation prompts** before each major operation
- ✨ **Optional build step** - skip automated build and use Visual Studio instead
- ✨ **Detailed error logging** with exit codes for all operations
- ✨ **Clear next steps** displayed after installation completes
- ✨ **Option to open Visual Studio** automatically after installation
- ✨ **Better error messages** with troubleshooting guidance
- ✨ **Improved user flow** between launcher and main installer
- 📚 **Documentation index** to help you find the right docs

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

### Quick Start

Simply double-click `install.bat` in the root directory:

```
fresh/
├── install.bat          <-- Double-click this file (Launcher)
├── CMakeLists.txt
├── vcpkg.json
└── tools/
    └── build_tools/
        ├── install.bat   <-- Main installer (called automatically)
        ├── build.bat
        ├── clean.bat
        └── rebuild.bat
```

**Estimated time**: 10-20 minutes (depending on internet speed)

### What Happens

1. **Root `install.bat`** shows overview and launches the main installer
2. **Main installer** (`tools\build_tools\install.bat`) performs installation
3. **Root `install.bat`** displays completion summary and next steps

### Manual Control

The installer now provides **manual control** at each step:
- You'll be prompted to continue after each major step
- Each step shows debug information to help identify issues
- Press Ctrl+C at any prompt to stop the installation
- Review the log file at any time in the `logs/` directory

---

## What the Installer Does

### Step 1: Check Prerequisites
- Verifies CMake is installed
- Checks for Visual Studio 2022
- Logs all findings
- **Pauses for confirmation before proceeding**

### Step 2: vcpkg Setup
- Checks if vcpkg is already installed
- Offers to install vcpkg if not found
- Clones vcpkg from GitHub (with confirmation prompt)
- Runs bootstrap script (with confirmation prompt)
- Integrates with Visual Studio
- Logs all exit codes and directory changes
- **Pauses for confirmation before proceeding**

### Step 3: Configure Dependencies
- Verifies vcpkg manifest mode setup
- Lists required dependencies (GLFW, GLM, ImGui)
- Explains auto-installation during CMake
- Shows debug output if vcpkg is not found
- **Pauses for confirmation before proceeding**

### Step 4: Generate Visual Studio Project
- Creates `build/` directory
- Asks for confirmation before running CMake
- Runs CMake with vcpkg toolchain
- Automatically downloads and builds dependencies
- Generates `.sln` file
- Logs CMake exit codes
- **Pauses for confirmation before proceeding**

### Step 5: Build the Engine
- **Asks if you want to build now or skip this step**
- If yes: Compiles in Release mode
- If no: You can build manually in Visual Studio
- Shows build progress
- Reports success/failure with exit codes
- **Pauses for confirmation before proceeding**

### Step 6: Create Shortcuts
- Asks if you want shortcuts
- Creates batch file shortcuts to .sln file
- Creates shortcut to run the engine

---

## Recent Improvements (v2.2 - Manual/Debug Mode)

### Manual Step-by-Step Control (November 2025)
The installer now provides enhanced manual control and debugging capabilities to help diagnose installation issues:

1. **Explicit Pauses Between Steps**: After each major step completes, the installer pauses and shows a message like "Step X completed. Press any key to continue to Step Y..." This prevents the window from closing unexpectedly and gives you time to review the output.

2. **No Silent Pauses**: Changed `pause >nul` (silent) to `pause` (visible) so you can see when the installer is waiting for input instead of appearing frozen.

3. **Debug Output**: Added "DEBUG:" prefixed messages throughout showing:
   - Current directory before and after directory changes
   - Exit codes from all major operations (git clone, cmake, build)
   - File existence checks before attempting operations
   - Directory listings when files are missing

4. **Optional Build Step**: The build step (Step 5) now asks if you want to build automatically or skip it. This lets you:
   - Generate the Visual Studio solution without waiting for a full build
   - Open the solution in Visual Studio to build manually with better error messages
   - Debug build issues more easily in the IDE

5. **Manual Confirmation Prompts**: Several operations now ask for confirmation before proceeding:
   - vcpkg git clone (gives you a chance to verify git is working)
   - vcpkg bootstrap (can take several minutes, confirms before starting)
   - CMake generation (can take 10-15 minutes on first run with dependency downloads)
   - Build (optional, can be skipped entirely)

6. **Enhanced Error Messages**: All error messages now include:
   - The exact command that failed
   - The exit code returned
   - Troubleshooting suggestions
   - Path to the log file for detailed analysis

These improvements help you:
- See exactly where the installer is in the process
- Identify the exact point of failure if something goes wrong
- Review log files between steps
- Skip problematic steps and continue manually

---

## Recent Improvements (v2.1)

### Directory Change Robustness (November 2025)
The installer now includes comprehensive error checking for directory operations:

1. **Robust Directory Changes**: All `cd` commands now use `cd /d` flag to ensure both directory and drive letter are changed, preventing issues when vcpkg or build directories are on different drives.

2. **Directory Change Verification**: After critical directory changes (especially after vcpkg installation), the script verifies the change succeeded and displays clear error messages if it fails.

3. **VCPKG_ROOT Validation**: Before checking for vcpkg.exe, the script explicitly validates that the VCPKG_ROOT environment variable is set, preventing silent failures.

4. **Enhanced Diagnostic Logging**: The script now logs the exact paths being checked and confirms when critical operations complete, making debugging much easier.

These improvements address issues where:
- The installer could exit silently if directory changes failed
- Scripts would continue in unexpected directories leading to confusing errors
- Path issues on multi-drive systems were hard to diagnose

### vcpkg Installation Robustness (November 2025)
The installer now includes enhanced error checking for vcpkg installation:

1. **Post-Bootstrap Verification**: After bootstrapping vcpkg, the script explicitly verifies that `vcpkg.exe` was created. If not, it displays a clear error message explaining possible causes (antivirus, permissions, disk space).

2. **Integration Error Handling**: The `vcpkg integrate install` command now uses the full path to vcpkg.exe and checks for errors. If integration fails, it displays a warning but continues (since integration is not critical for building).

3. **Enhanced Logging**: User responses to prompts are now logged to the installation log file, making it easier to debug issues by reviewing exactly what choices were made.

4. **Full Path Usage**: All vcpkg commands now use full paths (e.g., `"%VCPKG_ROOT%\vcpkg.exe"`) instead of relying on PATH, preventing issues where vcpkg might not be found.

5. **generate_vs2022.bat vcpkg Support**: The standalone Visual Studio generation script now detects and uses vcpkg toolchain when available. Previously, running `generate_vs2022.bat` would always generate without vcpkg, causing CMake warnings about missing dependencies even when vcpkg was properly installed.

These improvements address issues where:
- The installer could appear to succeed but actually fail silently when vcpkg.exe wasn't properly created
- Manual project generation via `generate_vs2022.bat` wouldn't use vcpkg dependencies

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

### Issue: "vcpkg.exe not found after bootstrapping"
**Cause**: This error indicates that vcpkg bootstrap completed without errors, but the vcpkg.exe file was not created.

**Solution**:
1. Check `logs/install_log_*.txt` for detailed error messages
2. **Antivirus software** may be blocking or quarantining vcpkg.exe:
   - Temporarily disable antivirus or add an exception for the `vcpkg/` folder
   - Re-run the installer
3. **File permissions** may be preventing file creation:
   - Ensure you have write permissions to the repository folder
   - Try running from a folder outside of Program Files
4. **Disk space** may be insufficient:
   - Check you have at least 5GB of free space
   - vcpkg needs space to download and build packages
5. If issue persists, try manual vcpkg installation (see Manual Build section)

### Issue: "vcpkg integrate install returned error code"
**Cause**: The vcpkg integration with Visual Studio failed, but this is not critical for building.

**Note**: This is a **warning, not an error**. The build can still proceed.

**Solution**:
- The installer will continue - this is not critical
- You can manually integrate later by running: `vcpkg\vcpkg.exe integrate install`
- Or you can build without integration by using the CMake toolchain file (automatic)

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
REM Just generate VS solution (skip full installation)
generate_vs2022.bat

REM Just build (skip CMake generation)
tools\build_tools\build.bat

REM Clean build artifacts
tools\build_tools\clean.bat

REM Clean and rebuild everything
tools\build_tools\rebuild.bat
```

**Note**: `generate_vs2022.bat` will automatically use vcpkg if it's installed in the `vcpkg/` directory. If you see CMake warnings about missing GLFW, GLM, or ImGui, it means vcpkg wasn't found - run `install.bat` first to set up vcpkg and dependencies.

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
