#pragma once

#include "RenderContext.h"

#ifdef _WIN32
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace fresh {

/**
 * @brief DirectX 12 implementation of render context
 */
class DirectX12RenderContext : public IRenderContext {
public:
    DirectX12RenderContext();
    ~DirectX12RenderContext() override;
    
    bool initialize(Window* window) override;
    void shutdown() override;
    bool beginFrame() override;
    void endFrame() override;
    void waitIdle() override;
    
    void setViewport(int x, int y, int width, int height) override;
    void setScissor(int x, int y, int width, int height) override;
    void clearColor(float r, float g, float b, float a) override;
    void clearDepth(float depth) override;
    
    GraphicsAPI getAPI() const override { return GraphicsAPI::DirectX12; }
    void* getNativeDevice() override { return device.Get(); }
    
    int getSwapchainWidth() const override { return width; }
    int getSwapchainHeight() const override { return height; }
    
    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode, const std::string& fragmentCode) override;
    
private:
    bool enableDebugLayer();
    bool createDevice();
    bool createCommandQueue();
    bool createSwapchain();
    bool createRenderTargets();
    bool createDepthStencil();
    bool createDescriptorHeaps();
    bool createCommandAllocators();
    bool createCommandList();
    bool createFences();
    
    void waitForGPU();
    void moveToNextFrame();
    
    static constexpr int FRAME_COUNT = 2;
    
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<IDXGISwapChain3> swapchain;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    ComPtr<ID3D12Resource> renderTargets[FRAME_COUNT];
    ComPtr<ID3D12Resource> depthStencil;
    ComPtr<ID3D12CommandAllocator> commandAllocators[FRAME_COUNT];
    ComPtr<ID3D12GraphicsCommandList> commandList;
    
    // Synchronization
    ComPtr<ID3D12Fence> fence;
    UINT64 fenceValues[FRAME_COUNT];
    HANDLE fenceEvent;
    
    UINT rtvDescriptorSize = 0;
    UINT currentFrame = 0;
    
    Window* window = nullptr;
    int width = 0;
    int height = 0;
};

} // namespace fresh

#endif // _WIN32
