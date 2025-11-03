#pragma once

#include "RenderContext.h"

namespace fresh {

/**
 * @brief OpenGL implementation of render context
 */
class OpenGLRenderContext : public IRenderContext {
public:
    OpenGLRenderContext();
    ~OpenGLRenderContext() override;
    
    bool initialize(Window* window) override;
    void shutdown() override;
    bool beginFrame() override;
    void endFrame() override;
    void waitIdle() override;
    
    void setViewport(int x, int y, int width, int height) override;
    void setScissor(int x, int y, int width, int height) override;
    void clearColor(float r, float g, float b, float a) override;
    void clearDepth(float depth) override;
    
    GraphicsAPI getAPI() const override { return GraphicsAPI::OpenGL; }
    void* getNativeDevice() override { return nullptr; } // OpenGL doesn't have device concept
    
    int getSwapchainWidth() const override { return width; }
    int getSwapchainHeight() const override { return height; }
    
    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode, const std::string& fragmentCode) override;
    
private:
    bool loadGLFunctions();
    
    Window* window = nullptr;
    void* glContext = nullptr; // Platform-specific GL context
    int width = 0;
    int height = 0;
};

} // namespace fresh
