#pragma once

#include "renderer/RenderContext.h"

#ifdef _WIN32
#define NOMINMAX
#include <d3d11.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace fresh {

/**
 * @brief DirectX 11 implementation of render context
 */
class DirectX11RenderContext : public IRenderContext {
public:
    DirectX11RenderContext();
    ~DirectX11RenderContext() override;
    
    bool initialize(Window* window) override;
    void shutdown() override;
    bool beginFrame() override;
    void endFrame() override;
    void waitIdle() override;
    
    void setViewport(int x, int y, int width, int height) override;
    void setScissor(int x, int y, int width, int height) override;
    void clearColor(float r, float g, float b, float a) override;
    void clearDepth(float depth) override;
    
    GraphicsAPI getAPI() const override { return GraphicsAPI::DirectX11; }
    void* getNativeDevice() override { return device.Get(); }
    
    int getSwapchainWidth() const override { return width; }
    int getSwapchainHeight() const override { return height; }
    
    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode, const std::string& fragmentCode) override;
    
private:
    bool createDevice();
    bool createSwapchain();
    bool createRenderTargetView();
    bool createDepthStencilView();
    
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapchain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    
    Window* window = nullptr;
    int width = 0;
    int height = 0;
};

} // namespace fresh

#endif // _WIN32
