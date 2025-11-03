#pragma once

namespace fresh {

/**
 * @brief Graphics API enumeration
 * 
 * Defines which graphics API backend to use for rendering.
 */
enum class GraphicsAPI {
    OpenGL,       // OpenGL 4.5+ (cross-platform)
    DirectX11,    // DirectX 11 (Windows only)
    DirectX12,    // DirectX 12 (Windows only, modern)
    Auto          // Automatically select best available API
};

/**
 * @brief Get string name for graphics API
 */
inline const char* getGraphicsAPIName(GraphicsAPI api) {
    switch (api) {
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
    // Windows supports DirectX and OpenGL
    return api == GraphicsAPI::DirectX11 || api == GraphicsAPI::DirectX12 || api == GraphicsAPI::OpenGL;
#else
    // Linux and macOS support OpenGL
    return api == GraphicsAPI::OpenGL;
#endif
}

/**
 * @brief Select best available graphics API for platform
 */
inline GraphicsAPI selectBestGraphicsAPI() {
#ifdef _WIN32
    // Prefer DirectX 12 on Windows 10+, fallback to DirectX 11
    return GraphicsAPI::DirectX12;
#else
    // Prefer OpenGL on Linux and macOS
    return GraphicsAPI::OpenGL;
#endif
}

} // namespace fresh
