# Viewport System Diagnosis and Fix Plan

## Current Implementation Analysis

### What's Already Implemented:
1. ✅ Win32ViewportPanel class with proper window creation
2. ✅ DirectX swap chain recreation for viewport
3. ✅ Viewport resize handling
4. ✅ Mouse bounds checking
5. ✅ Z-order management
6. ✅ Pre-world-creation swap chain configuration

### Potential Issues Based on Unreal Engine Comparison:

#### 1. **Swap Chain Configuration**
- Current: Using `DXGI_SWAP_EFFECT_FLIP_DISCARD`
- Unreal Engine uses: `DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL` for smoother presentation
- **Impact**: May cause tearing or frame presentation issues

#### 2. **Viewport Initialization Timing**
- Current: Viewport swap chain created in `Engine::initialize()`
- Issue: If viewport panel dimensions are invalid at initialization, swap chain fails
- **Solution**: Add retry mechanism and better error handling

#### 3. **Render Target Binding**
- Current: Binding happens in `beginFrame()`
- Unreal approach: Explicit render target management with validation
- **Solution**: Add render target state validation

#### 4. **Window Message Handling**
- Current: Viewport has basic WM_PAINT and WM_SIZE handling
- Issue: May not properly handle WM_ERASEBKGND or WM_NCPAINT
- **Solution**: Improve message handling for smoother rendering

#### 5. **DPI Scaling**
- Current: May not handle DPI scaling properly
- Unreal: Explicit DPI awareness
- **Solution**: Add DPI-aware viewport sizing

## Proposed Fixes (Aligned with Unreal Engine)

### Fix 1: Improve Swap Chain Configuration
```cpp
// Change swap effect to FLIP_SEQUENTIAL for smoother presentation
swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
// Add additional flags for better performance
swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | 
                      DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
```

### Fix 2: Add Viewport State Validation
```cpp
bool validateViewportState() {
    if (!viewportPanel || !viewportPanel->getHandle()) return false;
    if (viewportPanel->getWidth() <= 0 || viewportPanel->getHeight() <= 0) return false;
    if (!IsWindowVisible(viewportPanel->getHandle())) return false;
    return true;
}
```

### Fix 3: Improve Window Message Handling
```cpp
case WM_NCPAINT:
    // Don't allow non-client area painting
    return 0;
    
case WM_ERASEBKGND:
    // Already handled, but ensure we return 1
    return 1;
    
case WM_PAINT:
    // Validate without painting anything
    ValidateRect(hwnd, nullptr);
    return 0;
```

### Fix 4: Add DPI Awareness
```cpp
// Get DPI for viewport window
UINT dpi = GetDpiForWindow(viewportHwnd);
// Adjust dimensions based on DPI
int scaledWidth = MulDiv(width, dpi, 96);
int scaledHeight = MulDiv(height, dpi, 96);
```

### Fix 5: Add Diagnostic Logging
Add comprehensive logging to track viewport state throughout the lifecycle.

## Implementation Priority

1. **HIGH**: Fix swap chain configuration (FLIP_SEQUENTIAL)
2. **HIGH**: Add viewport state validation
3. **MEDIUM**: Improve window message handling
4. **MEDIUM**: Add DPI awareness
5. **LOW**: Enhanced diagnostic logging

## Testing Checklist

After implementing fixes:
- [ ] Viewport renders correctly on startup
- [ ] World creation renders to viewport only
- [ ] Viewport handles resize smoothly
- [ ] No tearing or artifacts
- [ ] Works on high DPI displays
- [ ] Multiple world creations work consistently
- [ ] Play mode transitions work properly
- [ ] Editor UI remains visible and responsive
