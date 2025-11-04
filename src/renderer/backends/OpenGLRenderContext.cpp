#include "renderer/backends/OpenGLRenderContext.h"
#include "core/Window.h"
#include <iostream>

// Windows-only OpenGL headers
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>

namespace fresh {

// Helper classes for OpenGL resources
class OpenGLBuffer : public RenderBuffer {
public:
    OpenGLBuffer(unsigned int type, const void* data, size_t dataSize) 
        : bufferType(type), bufferId(0), size(dataSize) {
        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferType, bufferId);
        glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);
        glBindBuffer(bufferType, 0);
    }
    
    ~OpenGLBuffer() override {
        if (bufferId != 0) {
            glDeleteBuffers(1, &bufferId);
        }
    }
    
    void bind() override {
        glBindBuffer(bufferType, bufferId);
    }
    
    void unbind() override {
        glBindBuffer(bufferType, 0);
    }
    
    void updateData(const void* data, size_t dataSize, size_t offset = 0) override {
        glBindBuffer(bufferType, bufferId);
        glBufferSubData(bufferType, offset, dataSize, data);
        glBindBuffer(bufferType, 0);
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
    OpenGLTexture(int w, int h, const void* data) : textureId(0), texWidth(w), texHeight(h) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    ~OpenGLTexture() override {
        if (textureId != 0) {
            glDeleteTextures(1, &textureId);
        }
    }
    
    void bind(int unit) override {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
    
    void unbind() override {
        glBindTexture(GL_TEXTURE_2D, 0);
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
    OpenGLShader(const std::string& vertexCode, const std::string& fragmentCode) : programId(0) {
        // Compile vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertSrc = vertexCode.c_str();
        glShaderSource(vertexShader, 1, &vertSrc, nullptr);
        glCompileShader(vertexShader);
        
        // Check vertex shader compilation
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "[OpenGL] Vertex shader compilation failed: " << infoLog << std::endl;
        }
        
        // Compile fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragSrc = fragmentCode.c_str();
        glShaderSource(fragmentShader, 1, &fragSrc, nullptr);
        glCompileShader(fragmentShader);
        
        // Check fragment shader compilation
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "[OpenGL] Fragment shader compilation failed: " << infoLog << std::endl;
        }
        
        // Link shaders into program
        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);
        
        // Check linking
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            std::cerr << "[OpenGL] Shader linking failed: " << infoLog << std::endl;
        }
        
        // Clean up shaders (they're linked into the program now)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    ~OpenGLShader() override {
        if (programId != 0) {
            glDeleteProgram(programId);
        }
    }
    
    void bind() override {
        glUseProgram(programId);
    }
    
    void unbind() override {
        glUseProgram(0);
    }
    
    void setUniformInt(const std::string& name, int value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniform1i(location, value);
    }
    
    void setUniformFloat(const std::string& name, float value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniform1f(location, value);
    }
    
    void setUniformVec2(const std::string& name, const glm::vec2& value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniform2fv(location, 1, &value[0]);
    }
    
    void setUniformVec3(const std::string& name, const glm::vec3& value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniform3fv(location, 1, &value[0]);
    }
    
    void setUniformVec4(const std::string& name, const glm::vec4& value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniform4fv(location, 1, &value[0]);
    }
    
    void setUniformMat4(const std::string& name, const glm::mat4& value) override {
        int location = glGetUniformLocation(programId, name.c_str());
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
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
    return std::make_shared<OpenGLBuffer>(GL_ARRAY_BUFFER, data, size);
}

std::shared_ptr<RenderBuffer> OpenGLRenderContext::createIndexBuffer(const void* data, size_t size) {
    return std::make_shared<OpenGLBuffer>(GL_ELEMENT_ARRAY_BUFFER, data, size);
}

std::shared_ptr<RenderBuffer> OpenGLRenderContext::createUniformBuffer(size_t size) {
    return std::make_shared<OpenGLBuffer>(GL_UNIFORM_BUFFER, nullptr, size);
}

std::shared_ptr<RenderTexture> OpenGLRenderContext::createTexture(int w, int h, const void* data) {
    return std::make_shared<OpenGLTexture>(w, h, data);
}

std::shared_ptr<RenderShader> OpenGLRenderContext::createShader(const std::string& vertexCode, const std::string& fragmentCode) {
    return std::make_shared<OpenGLShader>(vertexCode, fragmentCode);
}

bool OpenGLRenderContext::loadGLFunctions() {
    // Windows-specific GL function loading using wglGetProcAddress
    // For now, return true as stub
    std::cout << "[OpenGL] Loading OpenGL functions..." << std::endl;
    return true;
}

} // namespace fresh
