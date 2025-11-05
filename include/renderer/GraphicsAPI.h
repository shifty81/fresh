#pragma once

namespace fresh {

/**
 * @brief Supported graphics APIs (cross-platform)
 */
enum class GraphicsAPI {
    Auto,        // Automatically select best available API
    OpenGL,      // OpenGL 4.5+ (cross-platform)
    DirectX11,   // DirectX 11 (Windows)
    DirectX12    // DirectX 12 (Windows)
};

/**
 * @brief Check if a graphics API is available on the current platform
 */
inline bool isGraphicsAPIAvailable(GraphicsAPI api) {
    switch (api) {
        case GraphicsAPI::Auto:
            return true;
        case GraphicsAPI::OpenGL:
#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
            return true;
#else
            return false;
#endif
#ifdef _WIN32
        case GraphicsAPI::DirectX11:
        case GraphicsAPI::DirectX12:
            return true;
#endif
        default:
            return false;
    }
}

/**
 * @brief Get the name of a graphics API
 */
inline const char* getGraphicsAPIName(GraphicsAPI api) {
    switch (api) {
        case GraphicsAPI::Auto:        return "Auto";
        case GraphicsAPI::OpenGL:      return "OpenGL";
        case GraphicsAPI::DirectX11:   return "DirectX 11";
        case GraphicsAPI::DirectX12:   return "DirectX 12";
        default:                       return "Unknown";
    }
}

/**
 * @brief Select the best graphics API for the platform
 * Windows: DirectX 12 -> DirectX 11 -> OpenGL (if GLEW available)
 * Linux/Mac: OpenGL (if GLEW available)
 */
inline GraphicsAPI selectBestGraphicsAPI() {
#ifdef _WIN32
    // On Windows, prefer DirectX 12, fallback to DirectX 11
    // OpenGL is optional if GLEW is available
    return GraphicsAPI::DirectX12;
#else
    // Linux, Mac, and other platforms use OpenGL
#if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    return GraphicsAPI::OpenGL;
#else
    // If OpenGL is not available, we have a problem
    // This should not happen in practice on Linux/Mac
    return GraphicsAPI::OpenGL;
#endif
#endif
}

} // namespace fresh
