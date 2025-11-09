#pragma once

#include "renderer/RenderContext.h"

#ifdef _WIN32
    #define NOMINMAX
    #include <wrl/client.h>

    #include <d3d12.h>
    #include <dxgi1_6.h>
using Microsoft::WRL::ComPtr;

namespace fresh
{

/**
 * @brief DirectX 12 implementation of render context
 */
class DirectX12RenderContext : public IRenderContext
{
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

    GraphicsAPI getAPI() const override
    {
        return GraphicsAPI::DirectX12;
    }
    void* getNativeDevice() override
    {
        return device.Get();
    }

    int getSwapchainWidth() const override
    {
        return width;
    }
    int getSwapchainHeight() const override
    {
        return height;
    }

    // DirectX 12 specific getters for ImGui integration
    ID3D12Device* getD3D12Device() const
    {
        return device.Get();
    }
    ID3D12DescriptorHeap* getSRVDescriptorHeap() const
    {
        return srvHeap.Get();
    }
    ID3D12GraphicsCommandList* getCommandList() const
    {
        return commandList.Get();
    }
    UINT getCurrentFrameIndex() const
    {
        return currentFrame;
    }
    DXGI_FORMAT getRTVFormat() const
    {
        return rtvFormat;
    }

    // Frame count constant
    static constexpr int FRAME_COUNT = 2;

    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode,
                                               const std::string& fragmentCode) override;

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

    // SRV descriptor heap size for ImGui and other shader resources
    // 256 descriptors is sufficient for:
    // - ImGui font textures (1-2)
    // - ImGui dynamic textures (variable, typically < 50)
    // - Game textures and resources (remaining capacity)
    // This can be increased if needed for texture-heavy applications
    static constexpr UINT SRV_HEAP_SIZE = 256;

    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<IDXGISwapChain3> swapchain;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    ComPtr<ID3D12DescriptorHeap> srvHeap; // For ImGui and other shader resources
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
    DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM; // Store actual format

    Window* window = nullptr;
    int width = 0;
    int height = 0;
    float clearColorValue[4] = {0.53f, 0.81f, 0.92f, 1.0f}; // Sky blue default
};

} // namespace fresh

#endif // _WIN32
