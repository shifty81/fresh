# CMake Configuration Troubleshooting Guide

## ⚠️ CRITICAL: CMake Version Compatibility

**If you're experiencing CMake generation failures, check your CMake version first!**

### Quick Check
Run this command:
```powershell
cmake --version
```

### Supported Versions
- ✅ **Recommended**: CMake 3.20 through 3.31 (stable releases)
- ⚠️ **Not Recommended**: CMake 4.x or pre-release versions (rc, alpha, beta)
- ❌ **Not Supported**: CMake versions below 3.20

### Common Issue: CMake 4.x or Pre-Release Versions

**Symptoms:**
```
[ERROR] Failed to generate Visual Studio solution
CMake exit code: 1
```

**If you have CMake 4.2.0-rc2 or similar:**
1. This is a pre-release version with potential bugs and breaking changes
2. CMake 4.x is a major version bump not yet fully tested with this project
3. **Solution**: Install CMake 3.31.x (latest stable 3.x) from https://cmake.org/download/

**Quick Fix:**
1. Uninstall current CMake version
2. Download CMake 3.31.x: https://cmake.org/download/
3. Install and ensure it's added to PATH
4. Verify: `cmake --version` should show `cmake version 3.31.x`
5. Re-run: `.\setup-and-build.ps1`

---

## Recent Improvements

This document describes recent improvements made to the build system to help diagnose and fix CMake configuration issues on Windows.

## What Was Fixed

### Problem
The CMake generation step was failing on Windows with a generic error message:
```
[ERROR] Failed to generate Visual Studio solution
```

The error occurred after all vcpkg dependencies were successfully installed, but the actual cause of the failure was not visible in the logs.

### Solution
We've made several improvements to help identify and resolve the issue:

## 1. Enhanced PowerShell Script Error Handling

The `setup-and-build.ps1` script now:

### Better Output Capture
- Captures all CMake output before processing it
- Preserves the actual exit code from CMake
- Prevents the exit code from being lost during output streaming

### Improved Error Messages
When CMake fails, you'll now see:
```powershell
[ERROR] Failed to generate Visual Studio solution
Error: CMake generation failed with exit code 1

CMake exit code: 1

Last 20 lines of CMake output:
  [actual error message from CMake will be shown here]

Troubleshooting:
  1. Verify all prerequisites are installed
  2. Check that vcpkg is properly set up
  3. See BUILD.md for detailed instructions
  4. Check the full log file for more details: C:\path\to\logs\setup-and-build_timestamp.log
```

## 2. CMake Configuration Diagnostic Messages

Added status messages at key points in `CMakeLists.txt` to track progress:

### Dependency Finding Progress
```
-- OpenGL found
-- GLEW found for OpenGL extension loading
-- OpenAL found successfully
-- libvorbis found successfully
-- nlohmann-json found successfully
-- Native File Dialog Extended found successfully
-- All find_package calls completed
```

### Configuration Progress
```
-- Dependency checking completed
-- Preparing to define source files...
-- Windows Native UI enabled
-- Source and header files defined. Creating executable target...
-- Executable target 'FreshVoxelEngine' created successfully
```

### Completion Confirmation
```
========================================================================
-- CMake configuration completed successfully!
========================================================================
```

## How to Use These Improvements

### Running the Build Script

Simply run the improved script as before:
```powershell
.\setup-and-build.ps1
```

### If CMake Fails

1. **Check where it failed**: Look at which diagnostic messages appeared before the error
   - If you see "All find_package calls completed", the dependencies were found correctly
   - If you see "Preparing to define source files", the dependency checking passed
   - If you see "Executable target created", the source files were processed correctly

2. **Read the error message**: The script now shows the last 20 lines of CMake output, which will include the actual error message

3. **Check the log file**: The full log file location is shown in the error message. Open it to see complete details

4. **Share the information**: If you need help, share:
   - The CMake exit code
   - The last 20 lines of output shown
   - The stage where it failed (based on diagnostic messages)

## Common Issues and Solutions

### Issue 1: Package Not Found
If you see a message like:
```
CMake Error at CMakeLists.txt:119 (find_package):
  Could not find a package configuration file provided by "luajit"
```

**Solution**: The vcpkg package may not have been installed correctly. Try:
1. Delete the `vcpkg` directory
2. Run the script again to re-clone and bootstrap vcpkg

### Issue 2: Compiler Not Found
If you see:
```
CMake Error: Could not find a toolchain file for Visual Studio 17 2022
```

**Solution**: Ensure Visual Studio 2022 with "Desktop development with C++" workload is installed

### Issue 3: CMake Version Issues

#### Issue 3a: CMake Version Too Old
If you see an error about CMake version:
```
CMake version 3.20 or higher is required
Current version: 3.15.0
```

**Solution**: Update to CMake 3.20 or higher:
1. Download from: https://cmake.org/download/
2. Install and ensure it's in your system PATH
3. Verify: `cmake --version`

#### Issue 3b: CMake Pre-Release or Unstable Version
If you see a warning about unstable CMake:
```
WARNING: You are using an unstable CMake version!
  Current version: 4.2.0-rc2
  This is a pre-release version and may have bugs or breaking changes.
```

**Solution**: Use a stable CMake 3.x release:
1. Uninstall the pre-release CMake version
2. Download a stable version (3.30.x or 3.31.x) from: https://cmake.org/download/
3. Install and verify: `cmake --version`

**Why this matters:**
- Pre-release versions (rc, alpha, beta) may have bugs
- CMake 4.x introduces major breaking changes
- This project is tested with CMake 3.20 through 3.31
- Using stable versions ensures compatibility

#### Issue 3c: CMake 4.x Detected
If you see a warning about CMake 4.x:
```
WARNING: CMake 4.x or higher detected!
  Current version: 4.2.0
  CMake 4.x introduces major changes and may not be compatible.
```

**Solution**: Downgrade to CMake 3.30.x or 3.31.x:
1. Uninstall CMake 4.x
2. Download CMake 3.31.x (latest stable 3.x): https://cmake.org/download/
3. Install and verify: `cmake --version`
4. Re-run the build script: `.\setup-and-build.ps1`

**Technical Details:**
CMake 4.x is a major version bump with potential breaking changes:
- Generator behavior may be different
- Policy changes may affect how CMakeLists.txt is interpreted
- vcpkg integration may not be fully compatible
- Visual Studio generator may have changes

The Fresh Voxel Engine has been developed and tested with CMake 3.20-3.31. Using CMake 4.x is not recommended until it's officially released and tested.

## Understanding the Logs

### Log File Location
The full log is saved to:
```
<project-directory>\logs\setup-and-build_YYYYMMDD_HHMMSS.log
```

### Log Format
Each log entry includes a timestamp and category:
```
[2025-11-22 22:02:12] [CMAKE] -- OpenGL found
[2025-11-22 22:02:13] [INFO] Project directory: C:\path\to\fresh
```

### Categories
- `[HEADER]`: Section headers
- `[INFO]`: Informational messages
- `[SUCCESS]`: Successful operations
- `[STEP]`: Major step indicators
- `[CMAKE]`: CMake output
- `[BUILD]`: Build output
- `[ERROR]`: Error messages

## What to Do Next

1. **Run the script**: Try running `setup-and-build.ps1` again
2. **Review the output**: Look for the diagnostic messages and any errors
3. **Check the log**: If issues persist, review the full log file
4. **Report findings**: Share the exit code, error messages, and which stage failed

## Additional Resources

- **BUILD.md**: Detailed build instructions
- **README.md**: Project overview and requirements
- **DOCUMENTATION.md**: Complete documentation index

## Technical Details

### PowerShell Script Changes
- File: `setup-and-build.ps1`
- Key improvements:
  - Output capture before processing (line ~350)
  - Exit code preservation (line ~351)
  - Enhanced error reporting (line ~379)

### CMakeLists.txt Changes
- File: `CMakeLists.txt`
- Key improvements:
  - Diagnostic messages after find_package calls
  - Progress tracking messages
  - Completion confirmation message

These changes are non-breaking and improve debuggability without affecting normal operation.
