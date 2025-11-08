#include "renderer/backends/DirectX11RenderContext.h"

#ifdef _WIN32

#include "core/Window.h"
#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace fresh {

// Helper classes for DirectX 11 resources
class D3D11Buffer : public RenderBuffer {
public:
    D3D11Buffer(ComPtr<ID3D11Buffer> buf, ComPtr<ID3D11DeviceContext> ctx, size_t sz) 
        : buffer(buf), context(ctx), size(sz) {}
    
    void bind() override {
        // Binding is done through device context based on buffer type
    }
    
    void unbind() override {
        // Unbinding not typically needed in D3D11
    }
    
    void updateData(const void* data, size_t dataSize, size_t offset) override {
        if (!context || !buffer || !data) return;
        
        // For simple case with no offset, use UpdateSubresource
        if (offset == 0 && dataSize <= size) {
            context->UpdateSubresource(buffer.Get(), 0, nullptr, data, 0, 0);
        } else {
            // For partial updates, need to map/unmap the buffer
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            HRESULT hr = context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            if (SUCCEEDED(hr)) {
                if (offset + dataSize <= size) {
                    memcpy(static_cast<char*>(mappedResource.pData) + offset, data, dataSize);
                }
                context->Unmap(buffer.Get(), 0);
            }
        }
    }
    
    size_t getSize() const override { return size; }
    void* getNativeHandle() override { return buffer.Get(); }
    
private:
    ComPtr<ID3D11Buffer> buffer;
    ComPtr<ID3D11DeviceContext> context;
    size_t size;
};

class D3D11Texture : public RenderTexture {
public:
    D3D11Texture(ComPtr<ID3D11Texture2D> tex, ComPtr<ID3D11ShaderResourceView> srv, int w, int h)
        : texture(tex), shaderResourceView(srv), width(w), height(h) {}
    
    void bind(int unit) override {
        (void)unit; // Unused - binding done through device context
        // Binding is done through device context
    }
    
    void unbind() override {
        // Unbinding not typically needed in D3D11
    }
    
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }
    void* getNativeHandle() override { return texture.Get(); }
    
private:
    ComPtr<ID3D11Texture2D> texture;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    int width;
    int height;
};

class D3D11Shader : public RenderShader {
public:
    D3D11Shader(ComPtr<ID3D11VertexShader> vs, ComPtr<ID3D11PixelShader> ps)
        : vertexShader(vs), pixelShader(ps) {}
    
    void bind() override {
        // Binding is done through device context
    }
    
    void unbind() override {
        // Unbinding not typically needed in D3D11
    }
    
    void setUniformInt(const std::string& name, int value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void setUniformFloat(const std::string& name, float value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void setUniformVec2(const std::string& name, const glm::vec2& value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void setUniformVec3(const std::string& name, const glm::vec3& value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void setUniformVec4(const std::string& name, const glm::vec4& value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void setUniformMat4(const std::string& name, const glm::mat4& value) override {
        (void)name; (void)value; // Parameters reserved for D3D11 constant buffer implementation
        // Set via constant buffers in D3D11
    }
    
    void* getNativeHandle() override { return vertexShader.Get(); }
    
private:
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
};

DirectX11RenderContext::DirectX11RenderContext()
    : window(nullptr), width(0), height(0) {
}

DirectX11RenderContext::~DirectX11RenderContext() {
    shutdown();
}

bool DirectX11RenderContext::initialize(Window* win) {
    std::cout << "[DirectX 11] Initializing DirectX 11 render context..." << std::endl;
    
    if (!win) {
        std::cerr << "[DirectX 11] Invalid window pointer" << std::endl;
        return false;
    }
    
    window = win;
    width = static_cast<int>(window->getWidth());
    height = static_cast<int>(window->getHeight());
    
    if (!createDevice()) {
        std::cerr << "[DirectX 11] Failed to create device" << std::endl;
        return false;
    }
    
    if (!createSwapchain()) {
        std::cerr << "[DirectX 11] Failed to create swapchain" << std::endl;
        return false;
    }
    
    if (!createRenderTargetView()) {
        std::cerr << "[DirectX 11] Failed to create render target view" << std::endl;
        return false;
    }
    
    if (!createDepthStencilView()) {
        std::cerr << "[DirectX 11] Failed to create depth stencil view" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 11] DirectX 11 context initialized successfully" << std::endl;
    return true;
}

void DirectX11RenderContext::shutdown() {
    std::cout << "[DirectX 11] Shutting down DirectX 11 context..." << std::endl;
    
    // Release resources in reverse order
    depthStencilView.Reset();
    depthStencilBuffer.Reset();
    renderTargetView.Reset();
    swapchain.Reset();
    deviceContext.Reset();
    device.Reset();
}

bool DirectX11RenderContext::beginFrame() {
    // Clear render target and depth stencil
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    if (renderTargetView && deviceContext) {
        deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
    }
    
    if (depthStencilView && deviceContext) {
        deviceContext->ClearDepthStencilView(depthStencilView.Get(), 
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
    
    return true;
}

void DirectX11RenderContext::endFrame() {
    // Present the frame
    if (swapchain) {
        swapchain->Present(1, 0); // VSync enabled
    }
}

void DirectX11RenderContext::waitIdle() {
    // Flush the device context
    if (deviceContext) {
        deviceContext->Flush();
    }
}

void DirectX11RenderContext::setViewport(int x, int y, int w, int h) {
    if (!deviceContext) return;
    
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = static_cast<float>(x);
    viewport.TopLeftY = static_cast<float>(y);
    viewport.Width = static_cast<float>(w);
    viewport.Height = static_cast<float>(h);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    
    deviceContext->RSSetViewports(1, &viewport);
    width = w;
    height = h;
}

void DirectX11RenderContext::setScissor(int x, int y, int w, int h) {
    if (!deviceContext) return;
    
    D3D11_RECT scissorRect = {};
    scissorRect.left = x;
    scissorRect.top = y;
    scissorRect.right = x + w;
    scissorRect.bottom = y + h;
    
    deviceContext->RSSetScissorRects(1, &scissorRect);
}

void DirectX11RenderContext::clearColor(float r, float g, float b, float a) {
    if (!renderTargetView || !deviceContext) return;
    
    float clearColor[4] = { r, g, b, a };
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void DirectX11RenderContext::clearDepth(float depth) {
    if (!depthStencilView || !deviceContext) return;
    
    deviceContext->ClearDepthStencilView(depthStencilView.Get(), 
        D3D11_CLEAR_DEPTH, depth, 0);
}

std::shared_ptr<RenderBuffer> DirectX11RenderContext::createVertexBuffer(const void* data, size_t size) {
    if (!device) return nullptr;
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(size);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;
    
    ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, data ? &initData : nullptr, &buffer);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create vertex buffer" << std::endl;
        return nullptr;
    }
    
    return std::make_shared<D3D11Buffer>(buffer, deviceContext, size);
}

std::shared_ptr<RenderBuffer> DirectX11RenderContext::createIndexBuffer(const void* data, size_t size) {
    if (!device) return nullptr;
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(size);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;
    
    ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, data ? &initData : nullptr, &buffer);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create index buffer" << std::endl;
        return nullptr;
    }
    
    return std::make_shared<D3D11Buffer>(buffer, deviceContext, size);
}

std::shared_ptr<RenderBuffer> DirectX11RenderContext::createUniformBuffer(size_t size) {
    if (!device) return nullptr;
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(size);
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &buffer);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create uniform buffer" << std::endl;
        return nullptr;
    }
    
    return std::make_shared<D3D11Buffer>(buffer, deviceContext, size);
}

std::shared_ptr<RenderTexture> DirectX11RenderContext::createTexture(int w, int h, const void* data) {
    if (!device) return nullptr;
    
    // Create texture
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = w;
    texDesc.Height = h;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;
    initData.SysMemPitch = w * 4; // 4 bytes per pixel (RGBA)
    
    ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = device->CreateTexture2D(&texDesc, data ? &initData : nullptr, &texture);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create texture" << std::endl;
        return nullptr;
    }
    
    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    
    ComPtr<ID3D11ShaderResourceView> srv;
    hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, &srv);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create shader resource view" << std::endl;
        return nullptr;
    }
    
    return std::make_shared<D3D11Texture>(texture, srv, w, h);
}

std::shared_ptr<RenderShader> DirectX11RenderContext::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    if (!device) return nullptr;
    
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;
    
    // Compile vertex shader
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> errorBlob;
    
    HRESULT hr = D3DCompile(
        vertexCode.c_str(),
        vertexCode.size(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &vsBlob,
        &errorBlob
    );
    
    if (FAILED(hr)) {
        if (errorBlob) {
            std::cerr << "[DirectX 11] Vertex shader compilation error: " 
                      << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
        } else {
            std::cerr << "[DirectX 11] Vertex shader compilation failed" << std::endl;
        }
        return nullptr;
    }
    
    hr = device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create vertex shader" << std::endl;
        return nullptr;
    }
    
    // Compile pixel shader
    ComPtr<ID3DBlob> psBlob;
    
    hr = D3DCompile(
        fragmentCode.c_str(),
        fragmentCode.size(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &psBlob,
        &errorBlob
    );
    
    if (FAILED(hr)) {
        if (errorBlob) {
            std::cerr << "[DirectX 11] Pixel shader compilation error: " 
                      << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
        } else {
            std::cerr << "[DirectX 11] Pixel shader compilation failed" << std::endl;
        }
        return nullptr;
    }
    
    hr = device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create pixel shader" << std::endl;
        return nullptr;
    }
    
    std::cout << "[DirectX 11] Shaders compiled and created successfully" << std::endl;
    return std::make_shared<D3D11Shader>(vertexShader, pixelShader);
}

bool DirectX11RenderContext::createDevice() {
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    
    D3D_FEATURE_LEVEL featureLevel;
    
    HRESULT hr = D3D11CreateDevice(
        nullptr,                    // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,   // Hardware device
        nullptr,                    // No software device
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &device,
        &featureLevel,
        &deviceContext
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create D3D11 device" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 11] D3D11 device created successfully" << std::endl;
    return true;
}

bool DirectX11RenderContext::createSwapchain() {
    // Get native window handle
    HWND hwnd = static_cast<HWND>(window->getNativeWindowHandle());
    if (!hwnd) {
        std::cerr << "[DirectX 11] Failed to get native window handle" << std::endl;
        return false;
    }
    
    // Get DXGI factory from device
    ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = device.As(&dxgiDevice);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to get DXGI device" << std::endl;
        return false;
    }
    
    ComPtr<IDXGIAdapter> adapter;
    hr = dxgiDevice->GetAdapter(&adapter);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to get DXGI adapter" << std::endl;
        return false;
    }
    
    ComPtr<IDXGIFactory> factory;
    hr = adapter->GetParent(__uuidof(IDXGIFactory), &factory);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to get DXGI factory" << std::endl;
        return false;
    }
    
    // Create swapchain description
    DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
    swapchainDesc.BufferCount = 2;
    swapchainDesc.BufferDesc.Width = width;
    swapchainDesc.BufferDesc.Height = height;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = hwnd;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.Windowed = TRUE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    
    // Create the swapchain
    hr = factory->CreateSwapChain(device.Get(), &swapchainDesc, &swapchain);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create swapchain" << std::endl;
        return false;
    }
    
    // Disable Alt+Enter fullscreen toggle (can be re-enabled if needed)
    factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
    
    std::cout << "[DirectX 11] Swapchain created successfully" << std::endl;
    return true;
}

bool DirectX11RenderContext::createRenderTargetView() {
    if (!swapchain) {
        std::cerr << "[DirectX 11] Swapchain not initialized" << std::endl;
        return false;
    }
    
    // Get back buffer from swapchain
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
        reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to get back buffer from swapchain" << std::endl;
        return false;
    }
    
    // Create render target view
    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create render target view" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 11] Render target view created successfully" << std::endl;
    return true;
}

bool DirectX11RenderContext::createDepthStencilView() {
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    
    HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create depth stencil buffer" << std::endl;
        return false;
    }
    
    // Create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthStencilDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    
    hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, &depthStencilView);
    if (FAILED(hr)) {
        std::cerr << "[DirectX 11] Failed to create depth stencil view" << std::endl;
        return false;
    }
    
    // Bind render targets
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
    
    std::cout << "[DirectX 11] Depth stencil view created successfully" << std::endl;
    return true;
}

} // namespace fresh

#endif // _WIN32
