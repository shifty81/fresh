# Fixing "ALL_BUILD Access Denied" Error in Visual Studio

## The Problem

When you try to run the Fresh Voxel Engine in Visual Studio 2022 by pressing F5 or clicking the "Start" button, you get this error:

```
Unable to start program 'C:\GIT Projects\fresh\build\x64\release\ALL_BUILD'
Access is denied.
```

## Why This Happens

**ALL_BUILD** is a special CMake target that builds all projects in the solution, but it's **not an executable program**. Visual Studio is trying to run it like a program, which fails.

Think of it like trying to run a folder instead of an .exe file - it doesn't make sense!

## The Solution (2 Methods)

### Method 1: Set Startup Project in Solution Explorer (Recommended)

This is the permanent fix:

1. **Open Solution Explorer** (usually on the right side of Visual Studio)
   - If you can't see it, go to `View > Solution Explorer` or press `Ctrl+Alt+L`

2. **Find the FreshVoxelEngine project**
   ```
   Solution 'FreshVoxelEngine' (4 of 4 projects)
   ├── ALL_BUILD          ← Currently the startup project (in bold)
   ├── FreshVoxelEngine   ← This is what we want to run!
   ├── INSTALL
   └── ZERO_CHECK
   ```

3. **Right-click on `FreshVoxelEngine`**

4. **Click "Set as Startup Project"**

5. **Verify**: The `FreshVoxelEngine` project name should now appear in **bold**, and ALL_BUILD should no longer be bold.

6. **Try again**: Press F5 or Ctrl+F5 to run

### Method 2: Use Toolbar Dropdown (Quick Fix)

This works but needs to be done every time:

1. **Look at the toolbar** at the top of Visual Studio

2. **Find the green "Start" button** - next to it you'll see a dropdown that shows the current startup item

3. **Click the dropdown arrow** next to the green button

4. **Select `FreshVoxelEngine.exe`** from the list

5. **Press the green "Start" button** or press F5

## How to Tell If It's Fixed

### Before (Wrong):
- Green button says: "Local Windows Debugger" with dropdown showing "ALL_BUILD"
- Solution Explorer: **ALL_BUILD** is in bold
- Pressing F5 gives "Access is denied" error

### After (Correct):
- Green button says: "Local Windows Debugger" with dropdown showing "FreshVoxelEngine.exe"
- Solution Explorer: **FreshVoxelEngine** is in bold
- Pressing F5 launches the engine successfully!

## Why CMake Creates ALL_BUILD

CMake creates several special targets automatically:

- **ALL_BUILD** - Builds all projects (like "Build Solution")
- **INSTALL** - Installs built files to target directory
- **ZERO_CHECK** - Checks if CMake needs to regenerate files

These are **build targets**, not programs to run!

Only `FreshVoxelEngine` is the actual executable.

## Still Getting Errors?

If you set the correct startup project and still get errors:

### Error: "The system cannot find the file specified"

**Cause**: The executable hasn't been built yet.

**Fix**:
1. Build the solution first: Press `F7` or `Ctrl+Shift+B`
2. Wait for build to complete
3. Then press `F5` to run

### Error: "Access is denied" on FreshVoxelEngine.exe

**Possible causes**:
1. **Antivirus blocking**: Add an exception for your project folder
2. **File in use**: Close any running instances of the engine
3. **Permissions**: Run Visual Studio as Administrator (right-click VS icon, "Run as Administrator")

### Error: Missing DLLs (e.g., "glfw3.dll not found")

**Cause**: vcpkg dependencies aren't copied to output directory.

**Fix**:
1. Make sure you built with the vcpkg toolchain
2. Rebuild: `Build > Rebuild Solution`
3. Check `build/Release/` or `build/Debug/` for .dll files

## Prevention for Future Projects

To avoid this with other CMake projects:

1. **Always check** which project is in bold in Solution Explorer before running
2. **Look for the actual executable project** - usually the one with your project name
3. **Avoid running** projects ending in:
   - ALL_BUILD
   - INSTALL  
   - ZERO_CHECK
   - Lib/Library names

## Visual Checklist

Use this checklist every time you open the project:

- [ ] Visual Studio 2022 is open with FreshVoxelEngine.sln
- [ ] Solution Explorer is visible
- [ ] **FreshVoxelEngine** project is in **bold** (startup project)
- [ ] Configuration is set to `Release` or `Debug`
- [ ] Platform is set to `x64`
- [ ] Solution has been built successfully (F7)
- [ ] Ready to run (F5 or Ctrl+F5)

## Quick Reference Card

```
┌─────────────────────────────────────────────────────┐
│  BEFORE RUNNING THE ENGINE - CHECK THESE:           │
├─────────────────────────────────────────────────────┤
│                                                      │
│  ✓ Startup Project: FreshVoxelEngine (in bold)     │
│  ✓ Configuration: Release or Debug                  │
│  ✓ Platform: x64                                     │
│  ✓ Built: Press F7 first if not built              │
│                                                      │
│  THEN: Press F5 (with debugger) or Ctrl+F5         │
│                                                      │
└─────────────────────────────────────────────────────┘
```

## Related Documentation

- **[QUICKSTART_VISUAL.md](QUICKSTART_VISUAL.md)** - Complete setup guide
- **[VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)** - Detailed Visual Studio instructions
- **[DEVELOPER_SETUP.md](DEVELOPER_SETUP.md)** - General development setup

## Got It Working?

Great! Now you can:
- Explore the engine
- Modify the code
- Build your voxel game

For next steps, see [QUICKSTART_VISUAL.md](QUICKSTART_VISUAL.md) and [GETTING_STARTED.md](GETTING_STARTED.md).

---

**Remember**: Set `FreshVoxelEngine` as startup project, and you're good to go! 🚀
