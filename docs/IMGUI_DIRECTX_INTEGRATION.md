# ImGui DirectX Integration

## Overview

Fresh Voxel Engine now includes full DirectX 11 and DirectX 12 rendering backend support for ImGui, enabling rich UI development for the editor and in-game interfaces.

## Features

### DirectX 11 Backend
- Seamless integration with DirectX 11 render context
- Automatic device and device context management
- Support for all ImGui rendering features

### DirectX 12 Backend
- Full DirectX 12 command list integration
- Dedicated SRV descriptor heap for ImGui resources
- Support for multiple frames in flight
- Optimized for DirectX 12's explicit rendering model

## Architecture

### ImGuiContext Class
The `ImGuiContext` class in `src/ui/ImGuiContext.cpp` handles:
- Graphics API detection
- Backend-specific initialization
- Frame management
- Rendering and cleanup

### Render Context Integration
- **DirectX11RenderContext**: Exposes `ID3D11Device` and `ID3D11DeviceContext`
- **DirectX12RenderContext**: Exposes `ID3D12Device`, `ID3D12GraphicsCommandList`, and SRV descriptor heap

## Usage

### Initialization

```cpp
#include "ui/ImGuiContext.h"
#include "core/Window.h"
#include "renderer/RenderContext.h"

// Create render context (DirectX 11 or 12)
auto renderContext = RenderContextFactory::createBest();
renderContext->initialize(window);

// Initialize ImGui
ImGuiContext imguiContext;
imguiContext.initialize(window, renderContext.get());
```

### Frame Rendering

```cpp
// At the start of each frame
imguiContext.newFrame();

// Your ImGui UI code
ImGui::Begin("My Window");
ImGui::Text("Hello, DirectX!");
ImGui::End();

// At the end of the frame
imguiContext.render();
```

### Cleanup

```cpp
// Automatically handled by destructor, or explicitly:
imguiContext.shutdown();
```

## Implementation Details

### DirectX 11
- Uses `ImGui_ImplDX11_Init()` with device and device context
- Calls `ImGui_ImplDX11_NewFrame()` and `ImGui_ImplDX11_RenderDrawData()`
- GLFW initialized in "Other" mode (not OpenGL mode)

### DirectX 12
- Uses `ImGui_ImplDX12_Init()` with:
  - D3D12 device
  - Frame count (2 for double buffering)
  - RTV format (DXGI_FORMAT_R8G8B8A8_UNORM)
  - SRV descriptor heap (256 descriptors)
  - CPU and GPU descriptor handles
- Requires command list for rendering
- SRV heap created during DirectX12RenderContext initialization

## Technical Notes

### Descriptor Heap for DirectX 12
The DirectX12RenderContext creates a dedicated SRV (Shader Resource View) descriptor heap with:
- 256 descriptors (sufficient for ImGui and other resources)
- Type: `D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV`
- Flags: `D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE`

### Dynamic Casting
The implementation uses `dynamic_cast` to safely downcast `IRenderContext*` to specific backend types:
```cpp
DirectX11RenderContext* dx11Context = dynamic_cast<DirectX11RenderContext*>(renderContext);
```

This ensures type safety and allows access to backend-specific methods.

## Dependencies

- ImGui core library
- ImGui GLFW backend (`imgui_impl_glfw.h`)
- ImGui DirectX 11 backend (`imgui_impl_dx11.h`)
- ImGui DirectX 12 backend (`imgui_impl_dx12.h`)

Install via vcpkg:
```bash
vcpkg install imgui[glfw-binding,dx11-binding,dx12-binding]:x64-windows
```

## Error Handling

The implementation includes comprehensive error checking:
- Validates render context and window pointers
- Checks for successful GLFW initialization
- Verifies backend initialization
- Logs errors and warnings appropriately

## Future Enhancements

- Multi-viewport support (currently disabled for DirectX 12)
- Docking layouts for complex editor UIs
- Additional descriptor heap management for large texture sets
- Performance profiling and optimization

## Related Files

- `include/ui/ImGuiContext.h` - ImGuiContext interface
- `src/ui/ImGuiContext.cpp` - Implementation with DirectX backends
- `include/renderer/backends/DirectX11RenderContext.h` - DirectX 11 render context
- `include/renderer/backends/DirectX12RenderContext.h` - DirectX 12 render context
- `src/renderer/backends/DirectX12RenderContext.cpp` - DirectX 12 descriptor heap creation

## See Also

- [ImGui Documentation](https://github.com/ocornut/imgui)
- [DirectX 11 Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews)
- [DirectX 12 Documentation](https://docs.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide)
