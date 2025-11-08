# Fix Summary: Lua Linking Issue on Windows

## Issue
Linking errors on Windows with vcpkg:
- 11 unresolved external symbols: `__imp_lua_close`, `__imp_lua_settop`, etc.
- Occurred in AssetManager functions using Lua

## Root Cause
vcpkg's `lua` package on Windows compiles Lua as C++, causing symbol name mangling despite `extern "C"` blocks in the code.

## Solution
Changed dependency from `lua` to `luajit`:
- LuaJIT provides proper C linkage on Windows
- API-compatible with Lua 5.1
- Better performance (JIT compilation)

## Files Changed
1. **vcpkg.json** - Changed `"lua"` to `"luajit"`
2. **CMakeLists.txt** - Enhanced Lua/LuaJIT detection and linking
3. **LUA_LINKING_FIX.md** - Comprehensive documentation
4. **.gitignore** - Added build_test directory

## Testing Performed
- ✅ CMakeLists.txt syntax validation
- ✅ vcpkg.json JSON validation  
- ✅ CMake configuration test
- ✅ Security scan (CodeQL)
- ✅ Code compatibility verification

## Impact
- **Windows users**: Must reinstall dependencies (`vcpkg install`)
- **Linux/macOS users**: No impact (falls back to system Lua)
- **Scripts**: No changes needed (Lua 5.1 compatible)
- **Code**: No changes needed

## Next Steps for Windows Users
```bash
# Option 1: Clean rebuild
vcpkg install
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build

# Option 2: Just install LuaJIT
vcpkg install luajit:x64-windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg]/scripts/buildsystems/vcpkg.cmake  
cmake --build build
```

See `LUA_LINKING_FIX.md` for detailed instructions.
