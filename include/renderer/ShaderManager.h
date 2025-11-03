#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace fresh {

class VulkanDevice;

/**
 * @brief Manages shader loading, compilation, and hot-reloading
 * 
 * Provides shader management functionality including loading from files,
 * compilation to SPIR-V, and optional hot-reloading during development.
 */
class ShaderManager {
public:
    explicit ShaderManager(VulkanDevice* device);
    ~ShaderManager();

    // Prevent copying
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    /**
     * @brief Load a shader from file
     * @param path Path to shader file (GLSL or SPIR-V)
     * @return Shader code as string, empty on failure
     */
    std::string loadShader(const std::string& path);

    /**
     * @brief Compile GLSL to SPIR-V
     * @param glslCode GLSL shader code
     * @param shaderType Shader type (vertex, fragment, etc.)
     * @return SPIR-V binary code
     */
    std::string compileGLSL(const std::string& glslCode, const std::string& shaderType);

    /**
     * @brief Enable hot-reloading for development
     * @param enabled true to enable hot-reloading, false to disable
     */
    void setHotReloadEnabled(bool enabled) { m_hotReloadEnabled = enabled; }

    /**
     * @brief Check if any shaders have been modified
     * @return true if shaders need reloading, false otherwise
     */
    bool checkForChanges();

    /**
     * @brief Reload all modified shaders
     */
    void reloadModified();

private:
    VulkanDevice* m_device;
    bool m_hotReloadEnabled;
    std::unordered_map<std::string, time_t> m_shaderTimestamps;
};

} // namespace fresh
