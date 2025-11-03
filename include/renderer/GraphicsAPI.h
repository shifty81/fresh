#pragma once

namespace fresh {

/**
 * @brief Graphics API enumeration
 * 
 * Defines which graphics API backend to use for rendering.
 */
enum class GraphicsAPI {
    Vulkan,       // Vulkan API (cross-platform, modern)
    OpenGL,       // OpenGL 4.5+ (cross-platform, legacy support)
    DirectX11,    // DirectX 11 (Windows only)
    DirectX12,    // DirectX 12 (Windows only, modern)
    Auto          // Automatically select best available API
};

/**
 * @brief Get string name for graphics API
 */
inline const char* getGraphicsAPIName(GraphicsAPI api) {
    switch (api) {
        case GraphicsAPI::Vulkan: return "Vulkan";
        case GraphicsAPI::OpenGL: return "OpenGL";
        case GraphicsAPI::DirectX11: return "DirectX 11";
        case GraphicsAPI::DirectX12: return "DirectX 12";
        case GraphicsAPI::Auto: return "Auto";
        default: return "Unknown";
    }
}

/**
 * @brief Check if graphics API is available on current platform
 */
inline bool isGraphicsAPIAvailable(GraphicsAPI api) {
#ifdef _WIN32
    // Windows supports all APIs
    return true;
#elif defined(__APPLE__)
    // macOS supports Vulkan (via MoltenVK) and OpenGL
    return api == GraphicsAPI::Vulkan || api == GraphicsAPI::OpenGL;
#else
    // Linux supports Vulkan and OpenGL
    return api == GraphicsAPI::Vulkan || api == GraphicsAPI::OpenGL;
#endif
}

/**
 * @brief Select best available graphics API for platform
 */
inline GraphicsAPI selectBestGraphicsAPI() {
#ifdef _WIN32
    // Prefer DirectX 12 on Windows 10+, fallback to DirectX 11
    return GraphicsAPI::DirectX12;
#elif defined(__APPLE__)
    // Prefer Vulkan (MoltenVK) on macOS
    return GraphicsAPI::Vulkan;
#else
    // Prefer Vulkan on Linux
    return GraphicsAPI::Vulkan;
#endif
}

} // namespace fresh
