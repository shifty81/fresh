# Lua Linking Fix for Windows

## Problem

When building the Fresh Voxel Engine on Windows with vcpkg, you may encounter linking errors like:

```
unresolved external symbol __imp_lua_close referenced in function ...
unresolved external symbol __imp_lua_settop referenced in function ...
unresolved external symbol __imp_lua_type referenced in function ...
... and more Lua symbols
```

## Root Cause

The vcpkg `lua` package on Windows compiles Lua using the C++ compiler, which causes C++ name mangling on all symbols. Even though the code properly uses `extern "C"` blocks when including Lua headers, the actual library symbols have C++ mangling, causing linking to fail.

## Solution

We've changed from using `lua` to `luajit` in vcpkg.json. LuaJIT provides:

1. **Proper C linkage** on all platforms, including Windows
2. **Better performance** - JIT compilation makes Lua code run faster
3. **API compatibility** - LuaJIT is compatible with Lua 5.1 API (most Lua code works)
4. **Active maintenance** - Well-supported project

## What Changed

### vcpkg.json
```json
{
  "name": "luajit",  // Changed from "lua"
  "platform": "windows"
}
```

### CMakeLists.txt
- Added detection for LuaJIT via `find_package(luajit CONFIG QUIET)`
- Added support for `luajit` and `luajit::luajit` CMake targets
- Falls back to standard `lua` if LuaJIT is not available
- Updated warning messages to suggest LuaJIT first

## How to Use

### For New Builds

If you haven't installed dependencies yet, the new configuration will automatically use LuaJIT:

```bash
# Install vcpkg dependencies
vcpkg install

# Or manually
vcpkg install luajit:x64-windows

# Configure and build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### For Existing Builds

If you already have vcpkg's `lua` installed, you can either:

**Option 1: Install LuaJIT alongside Lua**
```bash
vcpkg install luajit:x64-windows
# CMake will automatically prefer LuaJIT
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

**Option 2: Remove Lua and install LuaJIT**
```bash
vcpkg remove lua:x64-windows
vcpkg install luajit:x64-windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

**Option 3: Clean rebuild**
```bash
# Remove build directory
rm -rf build

# Reinstall dependencies (vcpkg will use new vcpkg.json)
vcpkg install

# Rebuild
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Compatibility Notes

### LuaJIT vs Lua Differences

LuaJIT is API-compatible with Lua 5.1, which means:

- ✅ **Most Lua 5.1 code works** without changes
- ✅ **Most Lua 5.2-5.4 code works** (minor differences)
- ✅ **Better performance** for most use cases
- ⚠️ Some Lua 5.2+ features are not available (rare edge cases)

### For Modders and Script Writers

If you're writing Lua scripts for the engine:

- Use **Lua 5.1 API** for maximum compatibility
- Most common Lua features work identically
- Avoid Lua 5.2+ specific features like `goto` statements

### Code Compatibility

The C++ code doesn't need changes:
- `extern "C" { #include <lua.h> }` works the same
- All Lua C API functions are identical
- No code changes needed in AssetManager.cpp or other files

## Linux/macOS

On Linux and macOS, the system Lua packages work correctly (they're compiled as C). This fix specifically targets the Windows vcpkg issue. The CMakeLists.txt still supports standard Lua on these platforms.

## Verification

After building, you should see in the CMake output:

```
-- LuaJIT found via vcpkg CONFIG mode
-- LuaJIT enabled via CONFIG mode - linked target 'luajit'
--   LUA_INCLUDE_DIR: [path to LuaJIT headers]
```

If you see this, the fix is working correctly!

## Troubleshooting

### "luajit not found"

If CMake can't find LuaJIT:

1. Make sure you ran `vcpkg install` after updating vcpkg.json
2. Or manually install: `vcpkg install luajit:x64-windows`
3. Check that vcpkg toolchain file is specified in CMake command

### Still getting linking errors

If you still get Lua linking errors:

1. Clean your build directory: `rm -rf build`
2. Reinstall vcpkg packages: `vcpkg install --recurse`
3. Rebuild from scratch

### Want to use standard Lua instead

If you prefer standard Lua (not recommended on Windows):

1. Change `vcpkg.json` back to `"name": "lua"`
2. On Windows, you may need to add compile definitions or use a different Lua port

## References

- [LuaJIT Website](https://luajit.org/)
- [LuaJIT Compatibility](https://luajit.org/extensions.html)
- [vcpkg LuaJIT Port](https://github.com/microsoft/vcpkg/tree/master/ports/luajit)

## Related Files

- `vcpkg.json` - Dependency specification
- `CMakeLists.txt` - Build configuration
- `src/assets/AssetManager.cpp` - Uses Lua for asset loading
- `include/assets/AssetManager.h` - Asset manager interface
