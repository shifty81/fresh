# vcpkg Setup - Visual Guide

## 🎯 Quick Fix for "Cannot open include file" Errors

If you're seeing errors like:
```
Cannot open include file: 'glm/glm.hpp': No such file or directory
Cannot open include file: 'GLFW/glfw3.h': No such file or directory
```

Follow this simple 5-step setup:

```
📁 Your Computer
└── 📁 Projects
    └── 📁 fresh                      ← You are here
        ├── 📄 CMakeLists.txt
        ├── 📄 generate_vs2022.bat
        ├── 📁 src
        ├── �� include
        └── 📁 vcpkg                  ← Install vcpkg HERE
            ├── 📄 vcpkg.exe
            └── 📁 scripts
```

### Step-by-Step Commands

```batch
# Step 1: Make sure you're in the fresh project directory
cd fresh

# Step 2: Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git

# Step 3: Enter the vcpkg directory
cd vcpkg

# Step 4: Bootstrap vcpkg
bootstrap-vcpkg.bat

# Step 5: Return to project root and generate solution
cd ..
generate_vs2022.bat
```

## ✅ What Changed?

### Before (Old Way - Caused Confusion)
```
📁 Projects
├── 📁 vcpkg                 ← vcpkg was here
└── 📁 fresh
    └── 📄 generate_vs2022.bat
```
**Problem**: Users got confused about paths and got "file not found" errors

### After (New Way - Simpler!)
```
�� Projects
└── 📁 fresh
    ├── 📁 vcpkg             ← vcpkg is now here
    └── 📄 generate_vs2022.bat
```
**Benefit**: Everything in one place, no path confusion!

## 🔍 How Detection Works

The `generate_vs2022.bat` script automatically looks for vcpkg:

```
1. Check: ./vcpkg/scripts/buildsystems/vcpkg.cmake     ✓ Checks here FIRST
   └─ If found → Use project directory vcpkg
   
2. Check: ../vcpkg/scripts/buildsystems/vcpkg.cmake    ✓ Then checks here
   └─ If found → Use parent directory vcpkg (for backward compatibility)
   
3. Not found → Show helpful error message with setup instructions
```

## 📋 Verification Checklist

After setup, verify these files exist:

```batch
# From the fresh directory, check:
dir vcpkg\vcpkg.exe
dir vcpkg\scripts\buildsystems\vcpkg.cmake
```

If both exist, you're good to go! ✅

## 🆘 Still Having Issues?

See our comprehensive guides:
- [VCPKG_SETUP_GUIDE.md](../VCPKG_SETUP_GUIDE.md) - Detailed setup guide
- [QUICK_FIX_BUILD_ERRORS.md](../QUICK_FIX_BUILD_ERRORS.md) - Common errors
- [BUILD.md](../BUILD.md) - Complete build instructions

## 💡 Alternative: Parent Directory Setup

If you prefer to share vcpkg across multiple projects:

```
📁 Projects
├── 📁 vcpkg                 ← Shared vcpkg here
├── 📁 fresh                 ← Project 1
├── 📁 another-project       ← Project 2
└── 📁 yet-another-project   ← Project 3
```

The script will automatically detect this setup too! It checks the project directory first, then the parent directory.

## 🎓 What This Fixes

Installing vcpkg and running `generate_vs2022.bat` will:
1. ✅ Install GLFW (window and input handling)
2. ✅ Install GLM (mathematics library)
3. ✅ Install ImGui (user interface)
4. ✅ Install GLEW (OpenGL extensions)
5. ✅ Configure CMake with the correct toolchain file
6. ✅ Generate Visual Studio 2022 solution
7. ✅ Resolve all "Cannot open include file" errors

The first time may take 5-15 minutes as vcpkg downloads and builds dependencies.

---

**Questions?** Check the [FAQ](../FAQ.md) or open an issue on GitHub.
