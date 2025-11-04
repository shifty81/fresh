#include "renderer/backends/DirectX12RenderContext.h"

#ifdef _WIN32

#include "core/Window.h"
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace fresh {

// Helper classes for DirectX 12 resources
class D3D12Buffer : public RenderBuffer {
public:
    D3D12Buffer(ComPtr<ID3D12Resource> res, size_t sz) : resource(res), size(sz) {}
    
    void bind() override {
        // Binding in D3D12 is done via descriptor tables and root signatures
    }
    
    void unbind() override {
        // Not applicable in D3D12
    }
    
    void updateData(const void* data, size_t dataSize, size_t offset) override {
        (void)data; (void)dataSize; (void)offset; // Unused - stub implementation
        // Would use upload heap and copy commands
    }
    
    size_t getSize() const override { return size; }
    void* getNativeHandle() override { return resource.Get(); }
    
private:
    ComPtr<ID3D12Resource> resource;
    size_t size;
};

class D3D12Texture : public RenderTexture {
public:
    D3D12Texture(ComPtr<ID3D12Resource> res, int w, int h)
        : resource(res), width(w), height(h) {}
    
    void bind(int unit) override {
        (void)unit; // Unused - binding done via descriptor tables
        // Binding in D3D12 is done via descriptor tables
    }
    
    void unbind() override {
        // Not applicable in D3D12
    }
    
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }
    void* getNativeHandle() override { return resource.Get(); }
    
private:
    ComPtr<ID3D12Resource> resource;
    int width;
    int height;
};

class D3D12Shader : public RenderShader {
public:
    D3D12Shader() {}
    
    void bind() override {
        // Shaders in D3D12 are bound via pipeline state objects
    }
    
    void unbind() override {
        // Not applicable in D3D12
    }
    
    void setUniformInt(const std::string& name, int value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void setUniformFloat(const std::string& name, float value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void setUniformVec2(const std::string& name, const glm::vec2& value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void setUniformVec3(const std::string& name, const glm::vec3& value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void setUniformVec4(const std::string& name, const glm::vec4& value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void setUniformMat4(const std::string& name, const glm::mat4& value) override {
        (void)name; (void)value; // Parameters reserved for D3D12 root constants/constant buffer implementation
        // Constants in D3D12 are set via root constants or constant buffers
    }
    
    void* getNativeHandle() override { return nullptr; }
    
private:
    // Would store shader bytecode or pipeline state
};

DirectX12RenderContext::DirectX12RenderContext()
    : window(nullptr), width(0), height(0), currentFrame(0) {
    for (int i = 0; i < FRAME_COUNT; ++i) {
        fenceValues[i] = 0;
    }
}

DirectX12RenderContext::~DirectX12RenderContext() {
    shutdown();
}

bool DirectX12RenderContext::initialize(Window* win) {
    std::cout << "[DirectX 12] Initializing DirectX 12 render context..." << std::endl;
    
    if (!win) {
        std::cerr << "[DirectX 12] Invalid window pointer" << std::endl;
        return false;
    }
    
    window = win;
    width = 1280;  // TODO: Get from window
    height = 720;
    
#ifdef _DEBUG
    if (!enableDebugLayer()) {
        std::cout << "[DirectX 12] Warning: Failed to enable debug layer" << std::endl;
    }
#endif
    
    if (!createDevice()) {
        std::cerr << "[DirectX 12] Failed to create device" << std::endl;
        return false;
    }
    
    if (!createCommandQueue()) {
        std::cerr << "[DirectX 12] Failed to create command queue" << std::endl;
        return false;
    }
    
    if (!createSwapchain()) {
        std::cerr << "[DirectX 12] Failed to create swapchain" << std::endl;
        return false;
    }
    
    if (!createDescriptorHeaps()) {
        std::cerr << "[DirectX 12] Failed to create descriptor heaps" << std::endl;
        return false;
    }
    
    if (!createRenderTargets()) {
        std::cerr << "[DirectX 12] Failed to create render targets" << std::endl;
        return false;
    }
    
    if (!createDepthStencil()) {
        std::cerr << "[DirectX 12] Failed to create depth stencil" << std::endl;
        return false;
    }
    
    if (!createCommandAllocators()) {
        std::cerr << "[DirectX 12] Failed to create command allocators" << std::endl;
        return false;
    }
    
    if (!createCommandList()) {
        std::cerr << "[DirectX 12] Failed to create command list" << std::endl;
        return false;
    }
    
    if (!createFences()) {
        std::cerr << "[DirectX 12] Failed to create fences" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 12] DirectX 12 context initialized successfully" << std::endl;
    return true;
}

void DirectX12RenderContext::shutdown() {
    std::cout << "[DirectX 12] Shutting down DirectX 12 context..." << std::endl;
    
    // Wait for GPU to finish all work
    waitForGPU();
    
    // Close fence event
    if (fenceEvent) {
        CloseHandle(fenceEvent);
        fenceEvent = nullptr;
    }
    
    // Release resources
    fence.Reset();
    commandList.Reset();
    
    for (int i = 0; i < FRAME_COUNT; ++i) {
        commandAllocators[i].Reset();
        renderTargets[i].Reset();
    }
    
    depthStencil.Reset();
    dsvHeap.Reset();
    srvHeap.Reset();
    rtvHeap.Reset();
    swapchain.Reset();
    commandQueue.Reset();
    device.Reset();
}

bool DirectX12RenderContext::beginFrame() {
    // Reset command allocator for current frame
    if (commandAllocators[currentFrame]) {
        HRESULT hr = commandAllocators[currentFrame]->Reset();
        if (FAILED(hr)) {
            std::cerr << "[DirectX 12] Failed to reset command allocator" << std::endl;
            return false;
        }
    }
    
    // Reset command list
    if (commandList && commandAllocators[currentFrame]) {
        HRESULT hr = commandList->Reset(commandAllocators[currentFrame].Get(), nullptr);
        if (FAILED(hr)) {
            std::cerr << "[DirectX 12] Failed to reset command list" << std::endl;
            return false;
        }
    }
    
    // Transition render target to render target state
    // Set render targets
    // Clear render target and depth stencil
    
    return true;
}

void DirectX12RenderContext::endFrame() {
    // Transition render target to present state
    // Close command list
    // Execute command list
    // Present
    
    if (commandList) {
        commandList->Close();
    }
    
    if (commandQueue && commandList) {
        ID3D12CommandList* commandLists[] = { commandList.Get() };
        commandQueue->ExecuteCommandLists(1, commandLists);
    }
    
    if (swapchain) {
        swapchain->Present(1, 0); // VSync enabled
    }
    
    // Move to next frame
    moveToNextFrame();
}

void DirectX12RenderContext::waitIdle() {
    waitForGPU();
}

void DirectX12RenderContext::setViewport(int x, int y, int w, int h) {
    if (!commandList) return;
    
    D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = static_cast<float>(x);
    viewport.TopLeftY = static_cast<float>(y);
    viewport.Width = static_cast<float>(w);
    viewport.Height = static_cast<float>(h);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    
    commandList->RSSetViewports(1, &viewport);
    width = w;
    height = h;
}

void DirectX12RenderContext::setScissor(int x, int y, int w, int h) {
    if (!commandList) return;
    
    D3D12_RECT scissorRect = {};
    scissorRect.left = x;
    scissorRect.top = y;
    scissorRect.right = x + w;
    scissorRect.bottom = y + h;
    
    commandList->RSSetScissorRects(1, &scissorRect);
}

void DirectX12RenderContext::clearColor(float r, float g, float b, float a) {
    (void)r; (void)g; (void)b; (void)a; // Unused - stub implementation
    // Would use ClearRenderTargetView on command list
}

void DirectX12RenderContext::clearDepth(float depth) {
    (void)depth; // Unused - stub implementation
    // Would use ClearDepthStencilView on command list
}

std::shared_ptr<RenderBuffer> DirectX12RenderContext::createVertexBuffer(const void* data, size_t size) {
    if (!device) return nullptr;
    
    // Create committed resource for vertex buffer
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    ComPtr<ID3D12Resource> buffer;
    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create vertex buffer" << std::endl;
        return nullptr;
    }
    
    // Copy data if provided
    if (data) {
        void* mappedData = nullptr;
        hr = buffer->Map(0, nullptr, &mappedData);
        if (SUCCEEDED(hr)) {
            memcpy(mappedData, data, size);
            buffer->Unmap(0, nullptr);
        }
    }
    
    return std::make_shared<D3D12Buffer>(buffer, size);
}

std::shared_ptr<RenderBuffer> DirectX12RenderContext::createIndexBuffer(const void* data, size_t size) {
    // Similar to vertex buffer
    return createVertexBuffer(data, size);
}

std::shared_ptr<RenderBuffer> DirectX12RenderContext::createUniformBuffer(size_t size) {
    // Constant buffers must be 256-byte aligned
    size_t alignedSize = (size + 255) & ~255;
    return createVertexBuffer(nullptr, alignedSize);
}

std::shared_ptr<RenderTexture> DirectX12RenderContext::createTexture(int w, int h, const void* data) {
    (void)data; // Unused - stub implementation
    if (!device) return nullptr;
    
    // Create texture resource (stub implementation)
    std::cout << "[DirectX 12] Creating texture (stub)" << std::endl;
    
    ComPtr<ID3D12Resource> texture;
    return std::make_shared<D3D12Texture>(texture, w, h);
}

std::shared_ptr<RenderShader> DirectX12RenderContext::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    (void)vertexCode; (void)fragmentCode; // Unused - stub implementation
    // Would compile HLSL shaders to bytecode and create pipeline state object
    std::cout << "[DirectX 12] Creating shader (stub)" << std::endl;
    return std::make_shared<D3D12Shader>();
}

bool DirectX12RenderContext::enableDebugLayer() {
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        std::cout << "[DirectX 12] Debug layer enabled" << std::endl;
        return true;
    }
#endif
    return false;
}

bool DirectX12RenderContext::createDevice() {
    HRESULT hr = D3D12CreateDevice(
        nullptr,                // Default adapter
        D3D_FEATURE_LEVEL_11_0, // Minimum feature level
        IID_PPV_ARGS(&device)
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create D3D12 device" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 12] D3D12 device created successfully" << std::endl;
    return true;
}

bool DirectX12RenderContext::createCommandQueue() {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    
    HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create command queue" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 12] Command queue created" << std::endl;
    return true;
}

bool DirectX12RenderContext::createSwapchain() {
    // Stub - would create swapchain with DXGI factory and window handle
    std::cout << "[DirectX 12] Swapchain created (stub)" << std::endl;
    return true;
}

bool DirectX12RenderContext::createRenderTargets() {
    // Stub - would get back buffers from swapchain
    std::cout << "[DirectX 12] Render targets created (stub)" << std::endl;
    return true;
}

bool DirectX12RenderContext::createDepthStencil() {
    // Stub - would create depth stencil buffer
    std::cout << "[DirectX 12] Depth stencil created (stub)" << std::endl;
    return true;
}

bool DirectX12RenderContext::createDescriptorHeaps() {
    // Create RTV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    HRESULT hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create RTV heap" << std::endl;
        return false;
    }
    
    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    // Create DSV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    hr = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create DSV heap" << std::endl;
        return false;
    }
    
    // Create SRV descriptor heap for ImGui and other shader resources
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = SRV_HEAP_SIZE;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    
    hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create SRV heap" << std::endl;
        return false;
    }
    
    std::cout << "[DirectX 12] Descriptor heaps created" << std::endl;
    return true;
}

bool DirectX12RenderContext::createCommandAllocators() {
    for (int i = 0; i < FRAME_COUNT; ++i) {
        HRESULT hr = device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&commandAllocators[i])
        );
        
        if (FAILED(hr)) {
            std::cerr << "[DirectX 12] Failed to create command allocator " << i << std::endl;
            return false;
        }
    }
    
    std::cout << "[DirectX 12] Command allocators created" << std::endl;
    return true;
}

bool DirectX12RenderContext::createCommandList() {
    HRESULT hr = device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        commandAllocators[0].Get(),
        nullptr,
        IID_PPV_ARGS(&commandList)
    );
    
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create command list" << std::endl;
        return false;
    }
    
    // Command lists are created in recording state, close it for now
    commandList->Close();
    
    std::cout << "[DirectX 12] Command list created" << std::endl;
    return true;
}

bool DirectX12RenderContext::createFences() {
    HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (FAILED(hr)) {
        std::cerr << "[DirectX 12] Failed to create fence" << std::endl;
        return false;
    }
    
    // Create event for fence signaling
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!fenceEvent) {
        std::cerr << "[DirectX 12] Failed to create fence event" << std::endl;
        return false;
    }
    
    // Initialize fence values
    for (int i = 0; i < FRAME_COUNT; ++i) {
        fenceValues[i] = 0;
    }
    
    std::cout << "[DirectX 12] Fences created" << std::endl;
    return true;
}

void DirectX12RenderContext::waitForGPU() {
    if (!commandQueue || !fence) return;
    
    // Signal and wait for fence
    const UINT64 fenceValue = fenceValues[currentFrame];
    HRESULT hr = commandQueue->Signal(fence.Get(), fenceValue);
    if (FAILED(hr)) return;
    
    hr = fence->SetEventOnCompletion(fenceValue, fenceEvent);
    if (FAILED(hr)) return;
    
    WaitForSingleObject(fenceEvent, INFINITE);
    
    fenceValues[currentFrame]++;
}

void DirectX12RenderContext::moveToNextFrame() {
    // Schedule a Signal command in the queue
    const UINT64 currentFenceValue = fenceValues[currentFrame];
    if (commandQueue) {
        commandQueue->Signal(fence.Get(), currentFenceValue);
    }
    
    // Update frame index
    currentFrame = (currentFrame + 1) % FRAME_COUNT;
    
    // Wait if next frame is not ready yet
    if (fence && fence->GetCompletedValue() < fenceValues[currentFrame]) {
        fence->SetEventOnCompletion(fenceValues[currentFrame], fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
    }
    
    // Set fence value for next frame
    fenceValues[currentFrame] = currentFenceValue + 1;
}

} // namespace fresh

#endif // _WIN32
