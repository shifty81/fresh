#pragma once

#include "renderer/RenderContext.h"

#ifdef FRESH_OPENGL_SUPPORT

#include <GL/glew.h>
#include <memory>
#include <vector>

namespace fresh {

class Window;

/**
 * @brief OpenGL implementation of render context
 * 
 * Provides cross-platform OpenGL 4.5+ rendering support for
 * Linux, macOS, and Windows.
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
    void* getNativeDevice() override { return nullptr; } // OpenGL doesn't have a device object
    
    int getSwapchainWidth() const override { return width; }
    int getSwapchainHeight() const override { return height; }
    
    std::shared_ptr<RenderBuffer> createVertexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createIndexBuffer(const void* data, size_t size) override;
    std::shared_ptr<RenderBuffer> createUniformBuffer(size_t size) override;
    std::shared_ptr<RenderTexture> createTexture(int width, int height, const void* data) override;
    std::shared_ptr<RenderShader> createShader(const std::string& vertexCode, const std::string& fragmentCode) override;
    
private:
    bool initializeGLEW();
    void checkGLErrors(const char* context);
    
    Window* window = nullptr;
    int width = 0;
    int height = 0;
    
    // OpenGL state
    GLuint defaultVAO = 0;  // Default Vertex Array Object
    glm::vec4 clearColorValue = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    float clearDepthValue = 1.0f;
};

} // namespace fresh

#endif // FRESH_OPENGL_SUPPORT
