#include "renderer/backends/OpenGLRenderContext.h"
#include "core/Window.h"
#include <iostream>

// Platform-specific OpenGL headers
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace fresh {

// Helper classes for OpenGL resources
class OpenGLBuffer : public RenderBuffer {
public:
    OpenGLBuffer(unsigned int type) : bufferType(type), bufferId(0), size(0) {}
    
    ~OpenGLBuffer() override {
        if (bufferId != 0) {
            // glDeleteBuffers(1, &bufferId);
        }
    }
    
    void bind() override {
        // glBindBuffer(bufferType, bufferId);
    }
    
    void unbind() override {
        // glBindBuffer(bufferType, 0);
    }
    
    void updateData(const void* data, size_t dataSize, size_t offset = 0) override {
        // glBindBuffer(bufferType, bufferId);
        // glBufferSubData(bufferType, offset, dataSize, data);
    }
    
    size_t getSize() const override { return size; }
    void* getNativeHandle() override { return &bufferId; }
    
private:
    unsigned int bufferType;
    unsigned int bufferId;
    size_t size;
};

class OpenGLTexture : public RenderTexture {
public:
    OpenGLTexture(int w, int h) : textureId(0), texWidth(w), texHeight(h) {}
    
    ~OpenGLTexture() override {
        if (textureId != 0) {
            // glDeleteTextures(1, &textureId);
        }
    }
    
    void bind(int unit) override {
        // glActiveTexture(GL_TEXTURE0 + unit);
        // glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    void unbind() override {
        // glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    int getWidth() const override { return texWidth; }
    int getHeight() const override { return texHeight; }
    void* getNativeHandle() override { return &textureId; }
    
private:
    unsigned int textureId;
    int texWidth;
    int texHeight;
};

class OpenGLShader : public RenderShader {
public:
    OpenGLShader() : programId(0) {}
    
    ~OpenGLShader() override {
        if (programId != 0) {
            // glDeleteProgram(programId);
        }
    }
    
    void bind() override {
        // glUseProgram(programId);
    }
    
    void unbind() override {
        // glUseProgram(0);
    }
    
    void setUniformInt(const std::string& name, int value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniform1i(location, value);
    }
    
    void setUniformFloat(const std::string& name, float value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniform1f(location, value);
    }
    
    void setUniformVec2(const std::string& name, const glm::vec2& value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniform2fv(location, 1, &value[0]);
    }
    
    void setUniformVec3(const std::string& name, const glm::vec3& value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniform3fv(location, 1, &value[0]);
    }
    
    void setUniformVec4(const std::string& name, const glm::vec4& value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniform4fv(location, 1, &value[0]);
    }
    
    void setUniformMat4(const std::string& name, const glm::mat4& value) override {
        // int location = glGetUniformLocation(programId, name.c_str());
        // glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
    
    void* getNativeHandle() override { return &programId; }
    
private:
    unsigned int programId;
};

OpenGLRenderContext::OpenGLRenderContext() 
    : window(nullptr), glContext(nullptr), width(0), height(0) {
}

OpenGLRenderContext::~OpenGLRenderContext() {
    shutdown();
}

bool OpenGLRenderContext::initialize(Window* win) {
    std::cout << "[OpenGL] Initializing OpenGL render context..." << std::endl;
    
    if (!win) {
        std::cerr << "[OpenGL] Invalid window pointer" << std::endl;
        return false;
    }
    
    window = win;
    width = 1280;  // TODO: Get from window
    height = 720;
    
    if (!loadGLFunctions()) {
        std::cerr << "[OpenGL] Failed to load OpenGL functions" << std::endl;
        return false;
    }
    
    // Set up default OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    std::cout << "[OpenGL] OpenGL context initialized successfully" << std::endl;
    return true;
}

void OpenGLRenderContext::shutdown() {
    std::cout << "[OpenGL] Shutting down OpenGL context..." << std::endl;
    
    if (glContext) {
        // Platform-specific context cleanup
        glContext = nullptr;
    }
}

bool OpenGLRenderContext::beginFrame() {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return true;
}

void OpenGLRenderContext::endFrame() {
    // Swap buffers is handled by GLFW or platform layer
}

void OpenGLRenderContext::waitIdle() {
    glFinish();
}

void OpenGLRenderContext::setViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
    width = w;
    height = h;
}

void OpenGLRenderContext::setScissor(int x, int y, int w, int h) {
    glScissor(x, y, w, h);
}

void OpenGLRenderContext::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLRenderContext::clearDepth(float depth) {
    glClearDepth(depth);
}

std::shared_ptr<RenderBuffer> OpenGLRenderContext::createVertexBuffer(const void* data, size_t size) {
    auto buffer = std::make_shared<OpenGLBuffer>(0x8892); // GL_ARRAY_BUFFER
    // Implementation would create and upload buffer data
    return buffer;
}

std::shared_ptr<RenderBuffer> OpenGLRenderContext::createIndexBuffer(const void* data, size_t size) {
    auto buffer = std::make_shared<OpenGLBuffer>(0x8893); // GL_ELEMENT_ARRAY_BUFFER
    // Implementation would create and upload buffer data
    return buffer;
}

std::shared_ptr<RenderBuffer> OpenGLRenderContext::createUniformBuffer(size_t size) {
    auto buffer = std::make_shared<OpenGLBuffer>(0x8A11); // GL_UNIFORM_BUFFER
    // Implementation would create buffer
    return buffer;
}

std::shared_ptr<RenderTexture> OpenGLRenderContext::createTexture(int w, int h, const void* data) {
    auto texture = std::make_shared<OpenGLTexture>(w, h);
    // Implementation would create and upload texture data
    return texture;
}

std::shared_ptr<RenderShader> OpenGLRenderContext::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    auto shader = std::make_shared<OpenGLShader>();
    // Implementation would compile and link shaders
    return shader;
}

bool OpenGLRenderContext::loadGLFunctions() {
    // Platform-specific GL function loading
    // On Windows: wglGetProcAddress
    // On Linux: glXGetProcAddress or use GLAD/GLEW
    // For now, return true as stub
    std::cout << "[OpenGL] Loading OpenGL functions..." << std::endl;
    return true;
}

} // namespace fresh
