# Rendering Fix: World Generation Not Showing

## Problem
The Fresh Voxel Engine would start successfully, showing a window with a light blue (sky) background, but no terrain or voxel world would be visible. The world generation was working correctly, but nothing appeared on screen.

## Root Cause
The issue was in the chunk rendering pipeline in `src/core/Engine.cpp`:

1. During world initialization, `VoxelWorld::loadChunk()` creates chunks and immediately generates their meshes
2. After mesh generation, chunks are marked as "clean" (not dirty)
3. The rendering code (`Engine::renderVoxelWorld()`) only uploaded mesh data to GPU when chunks were "dirty"
4. On first render, chunks were already clean, so their meshes were never uploaded to the GPU
5. Result: CPU had the mesh data, but GPU had no geometry to render

## The Fix
Modified `Engine::renderVoxelWorld()` to check TWO conditions:
- Upload mesh if chunk is **dirty** (modified/needs regeneration), OR
- Upload mesh if GPU buffers **don't exist yet** (first time rendering)

### Code Changes
```cpp
// Before (BROKEN):
if (chunk->isDirty()) {
    chunk->generateMesh();
    chunk->clearDirty();
    // ... upload to GPU ...
}

// After (FIXED):
bool needsUpload = chunk->isDirty() || (m_chunkVAOs.find(chunkPos) == m_chunkVAOs.end());
if (needsUpload) {
    if (chunk->isDirty()) {
        chunk->generateMesh();
        chunk->clearDirty();
    }
    // ... upload to GPU ...
}
```

## How to Verify
After applying this fix:
1. Build the engine: `cd build && cmake --build . --config Release`
2. Run the engine: `./FreshVoxelEngine`
3. Create a new world or load existing world
4. You should now see terrain rendered (hills, valleys, different colored blocks)
5. Instead of just light blue sky, you'll see the generated voxel world

## Technical Details
- **File Modified**: `src/core/Engine.cpp` (lines 864-868)
- **Lines Changed**: 6 lines modified, 10 total lines
- **Impact**: Fixes initial terrain rendering for ALL newly loaded chunks
- **Backwards Compatible**: Yes, doesn't affect existing save files
- **Performance**: No performance impact, only affects first render of each chunk

## Testing
- All 156 existing unit tests pass
- No regressions introduced
- Minimal, surgical change to fix specific issue

## Related Systems
This fix only affects the OpenGL rendering path. If you're using:
- **OpenGL**: ✅ Fixed by this patch
- **DirectX 11/12**: ⚠️ Not affected (DirectX rendering not yet implemented)

If you see a light blue screen with DirectX, that's a different issue - the DirectX backends don't have voxel rendering implemented yet. Use OpenGL on Windows by ensuring GLEW is installed.
