# CMake Configuration Troubleshooting Guide

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

### Issue 3: CMake Version
If you see warnings about CMake version:
```
CMake Warning: CMake version X.Y.Z may not work correctly
```

**Solution**: Ensure you have CMake 3.20 or higher installed. Note that release candidates (like 4.2.0-rc2) may have issues.

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
