# Viewport Integration TODO

## Overview
The Win32ViewportPanel has been created to provide a dedicated child window for 3D rendering, separating it from editor UI panels. However, the DirectX rendering pipeline still renders to the main window. This document outlines the steps needed to complete the viewport integration.

## Current State

### What Works
- ✅ Win32ViewportPanel class created
- ✅ Viewport child window created during EditorManager initialization
- ✅ Viewport positioned in center-right area (680, 80) with dynamic sizing
- ✅ Menu system and callbacks working correctly
- ✅ Editor UI panels positioned correctly

### What Doesn't Work
- ❌ DirectX renders to main window (full screen) instead of viewport child window
- ❌ Mouse input coordinates not viewport-relative
- ❌ Camera controls not restricted to viewport bounds
- ❌ Viewport resize doesn't trigger swap chain recreation

## Required Changes

### 1. Modify DirectX Swap Chain Creation

**Files to modify:**
- `src/renderer/backends/DirectX11RenderContext.cpp`
- `src/renderer/backends/DirectX12RenderContext.cpp`
- `include/renderer/IRenderContext.h`

**Changes needed:**

#### A. Add method to set/update viewport window handle
```cpp
// In IRenderContext.h
virtual bool setViewportWindow(void* viewportHwnd) = 0;

// In DirectX11RenderContext.h and DirectX12RenderContext.h
bool setViewportWindow(void* viewportHwnd) override;
```

#### B. Store viewport HWND separately from main window
```cpp
// In DirectX11RenderContext.h (private members)
HWND m_viewportHwnd = nullptr;  // Viewport child window handle
HWND m_mainHwnd = nullptr;      // Main window handle
```

#### C. Modify createSwapchain() to use viewport HWND
```cpp
bool DirectX11RenderContext::createSwapchain()
{
    // Use viewport HWND if available, otherwise use main window HWND
    HWND targetHwnd = m_viewportHwnd ? m_viewportHwnd : m_mainHwnd;
    
    // ... rest of swap chain creation using targetHwnd ...
    swapchainDesc.OutputWindow = targetHwnd;
    
    // ... continue ...
}
```

#### D. Add swap chain recreation method
```cpp
bool DirectX11RenderContext::recreateSwapChain(int newWidth, int newHeight)
{
    // Release existing swap chain resources
    releaseSwapChainResources();
    
    // Update dimensions
    width = newWidth;
    height = newHeight;
    
    // Recreate swap chain
    return createSwapchain() && createRenderTargetView() && createDepthStencilView();
}
```

### 2. Update Engine Initialization Flow

**File to modify:**
- `src/core/Engine.cpp`

**Changes needed:**

#### A. After viewport is created, update renderer with viewport HWND
In `Engine::initializeGameSystems()` after world creation:

```cpp
#ifdef _WIN32
    // Update DirectX renderer to use viewport child window for rendering
    if (m_editorManager && m_editorManager->getViewportPanel()) {
        auto* viewportPanel = m_editorManager->getViewportPanel();
        HWND viewportHwnd = viewportPanel->getHandle();
        
        if (viewportHwnd && m_renderer) {
            LOG_INFO_C("Updating renderer to use viewport child window", "Engine");
            
            // Set viewport window handle in renderer
            m_renderer->setViewportWindow(viewportHwnd);
            
            // Recreate swap chain for viewport
            int vpWidth = viewportPanel->getWidth();
            int vpHeight = viewportPanel->getHeight();
            
            auto* dx11Context = dynamic_cast<DirectX11RenderContext*>(m_renderer.get());
            if (dx11Context) {
                if (!dx11Context->recreateSwapChain(vpWidth, vpHeight)) {
                    LOG_ERROR_C("Failed to recreate DirectX 11 swap chain for viewport", "Engine");
                }
            }
            
            auto* dx12Context = dynamic_cast<DirectX12RenderContext*>(m_renderer.get());
            if (dx12Context) {
                if (!dx12Context->recreateSwapChain(vpWidth, vpHeight)) {
                    LOG_ERROR_C("Failed to recreate DirectX 12 swap chain for viewport", "Engine");
                }
            }
        }
    }
#endif
```

### 3. Handle Viewport Resize

**File to modify:**
- `src/core/Engine.cpp` (in the update() or processInput() method)

**Changes needed:**

Add resize check in main loop:

```cpp
void Engine::update(float deltaTime)
{
    // ... existing code ...
    
#ifdef _WIN32
    // Check if viewport was resized
    if (m_editorManager && m_editorManager->getViewportPanel()) {
        auto* viewportPanel = m_editorManager->getViewportPanel();
        
        if (viewportPanel->wasResized()) {
            LOG_INFO_C("Viewport resized, recreating swap chain", "Engine");
            
            int vpWidth = viewportPanel->getWidth();
            int vpHeight = viewportPanel->getHeight();
            
            // Recreate DirectX swap chain
            auto* dx11Context = dynamic_cast<DirectX11RenderContext*>(m_renderer.get());
            if (dx11Context) {
                dx11Context->recreateSwapChain(vpWidth, vpHeight);
            }
            
            auto* dx12Context = dynamic_cast<DirectX12RenderContext*>(m_renderer.get());
            if (dx12Context) {
                dx12Context->recreateSwapChain(vpWidth, vpHeight);
            }
            
            // Update camera aspect ratio
            if (m_player) {
                float aspectRatio = static_cast<float>(vpWidth) / static_cast<float>(vpHeight);
                m_player->getCamera().setAspectRatio(aspectRatio);
            }
            
            viewportPanel->clearResizedFlag();
        }
    }
#endif
    
    // ... rest of update ...
}
```

### 4. Update Mouse Input Handling

**File to modify:**
- `src/core/Engine.cpp` (in processInput() and update() methods)

**Changes needed:**

#### A. Check if mouse is within viewport before processing input

```cpp
void Engine::processInput()
{
    // ... existing input handling ...
    
#ifdef _WIN32
    // Only process world input if mouse is within viewport
    bool mouseInViewport = false;
    if (m_editorManager && m_editorManager->getViewportPanel()) {
        auto* viewport = m_editorManager->getViewportPanel();
        
        // Get current mouse position in screen coordinates
        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            mouseInViewport = viewport->isMouseInViewport(cursorPos.x, cursorPos.y);
        }
    }
    
    // Only allow camera control and block interaction within viewport
    if (mouseInViewport) {
        // ... existing camera and block interaction code ...
    }
#endif
}
```

#### B. Convert mouse coordinates to viewport-relative

In the mouse movement handling for camera:

```cpp
if (m_rightMouseHeldForCamera && mouseInViewport) {
    glm::vec2 mouseDelta = m_inputManager->getMouseDelta();
    
    // Mouse delta is already relative, so it doesn't need conversion
    // Just make sure we only use it when mouse is in viewport
    if (glm::length(mouseDelta) > 0.0f) {
        m_player->handleMouseMovement(mouseDelta.x, mouseDelta.y);
    }
}
```

For raycasting (block interaction), you may need viewport-relative coordinates:

```cpp
// Get mouse position relative to viewport for raycasting
int viewportX, viewportY;
POINT cursorPos;
GetCursorPos(&cursorPos);

if (m_editorManager && m_editorManager->getViewportPanel()) {
    m_editorManager->getViewportPanel()->screenToViewport(
        cursorPos.x, cursorPos.y, viewportX, viewportY);
    
    // Use viewportX, viewportY for raycasting calculations
}
```

### 5. Update Render Viewport

**File to modify:**
- `src/core/Engine.cpp` (render() method)

**Changes needed:**

Make sure the viewport is set to match the viewport panel size:

```cpp
void Engine::render()
{
    // ... existing render setup ...
    
#ifdef _WIN32
    // Set viewport to match viewport panel size
    if (m_editorManager && m_editorManager->getViewportPanel()) {
        auto* viewport = m_editorManager->getViewportPanel();
        int vpWidth = viewport->getWidth();
        int vpHeight = viewport->getHeight();
        
        m_renderer->setViewport(0, 0, vpWidth, vpHeight);
    } else
#endif
    {
        // Fallback: use full swapchain size
        m_renderer->setViewport(0, 0, m_renderer->getSwapchainWidth(),
                                m_renderer->getSwapchainHeight());
    }
    
    // ... continue with rendering ...
}
```

## Testing Checklist

After implementing these changes, test:

- [ ] Viewport panel is visible and correctly positioned
- [ ] 3D world renders ONLY within viewport bounds
- [ ] Editor UI panels visible and not obscured by 3D rendering
- [ ] Right-click camera control works within viewport
- [ ] Right-click outside viewport doesn't affect camera
- [ ] Left-click block interaction works within viewport
- [ ] Mouse cursor shows correctly (hidden when right-click in viewport, visible otherwise)
- [ ] Viewport resize works correctly (drag window edge)
- [ ] Swap chain recreates on viewport resize
- [ ] Camera aspect ratio updates on resize
- [ ] Menu items work (already tested - should still work)
- [ ] Keyboard shortcuts work (already tested - should still work)

## Additional Considerations

### Performance
- Swap chain recreation is expensive - only do it when viewport size actually changes
- Use the `wasResized()` flag to avoid unnecessary recreations

### Error Handling
- Gracefully handle cases where viewport panel creation fails
- Fall back to full-window rendering if viewport not available

### Camera
- Update camera aspect ratio whenever viewport resizes
- Consider adding viewport border visualization in editor mode

### Multiple Viewports (Future)
- The architecture should support multiple viewports in the future
- Consider making IRenderContext support multiple swap chains
- Each viewport could have its own camera and view

## References

- Win32ViewportPanel: `include/ui/native/Win32ViewportPanel.h`
- DirectX11RenderContext: `src/renderer/backends/DirectX11RenderContext.cpp`
- DirectX12RenderContext: `src/renderer/backends/DirectX12RenderContext.cpp`
- Engine render loop: `src/core/Engine.cpp`

## Estimated Time
- Swap chain modification: 2-4 hours
- Mouse input handling: 2-3 hours
- Resize handling: 1-2 hours
- Testing and debugging: 3-5 hours
- **Total: 8-14 hours**
