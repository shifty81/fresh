# Quick Fix for Build Errors

If you're getting build errors like:
- `Cannot open include file: 'glm/glm.hpp'`
- `Cannot open include file: 'GLFW/glfw3.h'`
- `vcpkg not found`

## Solution

Run the automated installer to set up vcpkg and dependencies:

```batch
install.bat
```

This will:
1. Install vcpkg in the project directory (simplest setup)
2. Automatically install GLFW, GLM, and ImGui
3. Generate the Visual Studio solution
4. Build the project

## Manual Fix

If you prefer manual setup:

### Option 1: Install vcpkg in project directory (Recommended)
```batch
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
cd ..
generate_vs2022.bat
```

### Option 2: Install vcpkg in parent directory (Alternative)
```batch
cd ..
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
cd fresh
generate_vs2022.bat
```

## Verify Your Setup

Check if vcpkg is properly installed:
```batch
verify_vcpkg.bat
```

## Why Project Directory?

Installing vcpkg in the project directory (recommended):
- Simplest setup with no path confusion
- Self-contained within the project
- Easier to understand for beginners
- Works consistently across different environments

Alternative: Installing in parent directory saves disk space if you work with multiple projects.

## Need More Help?

See the detailed documentation:
- **DEVELOPER_SETUP.md** - Complete setup guide
- **VCPKG_PARENT_SETUP.md** - Implementation details
- **BATCH_INSTALLER_GUIDE.md** - Installer troubleshooting
