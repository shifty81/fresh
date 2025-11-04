#pragma once

namespace fresh {

/**
 * @brief Graphics API enumeration
 * 
 * Defines which graphics API backend to use for rendering on Windows.
 */
enum class GraphicsAPI {
    OpenGL,       // OpenGL 4.5+ (broad hardware support)
    DirectX11,    // DirectX 11 (excellent compatibility)
    DirectX12,    // DirectX 12 (modern, best performance)
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
 * @brief Check if graphics API is available on Windows
 */
inline bool isGraphicsAPIAvailable(GraphicsAPI api) {
    // Windows supports DirectX 11/12 and OpenGL
    return api == GraphicsAPI::DirectX11 || api == GraphicsAPI::DirectX12 || api == GraphicsAPI::OpenGL;
}

/**
 * @brief Select best available graphics API for Windows
 */
inline GraphicsAPI selectBestGraphicsAPI() {
    // Prefer DirectX 12 on Windows 10+, fallback to DirectX 11
    return GraphicsAPI::DirectX12;
}

} // namespace fresh
