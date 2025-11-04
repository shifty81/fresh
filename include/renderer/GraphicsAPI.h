#pragma once

namespace fresh {

/**
 * @brief Supported graphics APIs (DirectX only - Windows exclusive)
 */
enum class GraphicsAPI {
    Auto,        // Automatically select best available DirectX version
    DirectX11,   // DirectX 11
    DirectX12    // DirectX 12 (default)
};

/**
 * @brief Check if a graphics API is available on the current platform
 */
inline bool isGraphicsAPIAvailable(GraphicsAPI api) {
    switch (api) {
        case GraphicsAPI::Auto:
        case GraphicsAPI::DirectX11:
        case GraphicsAPI::DirectX12:
            return true;
        default:
            return false;
    }
}

/**
 * @brief Get the name of a graphics API
 */
inline const char* getGraphicsAPIName(GraphicsAPI api) {
    switch (api) {
        case GraphicsAPI::Auto:        return "Auto (DirectX)";
        case GraphicsAPI::DirectX11:   return "DirectX 11";
        case GraphicsAPI::DirectX12:   return "DirectX 12";
        default:                       return "Unknown";
    }
}

/**
 * @brief Select the best graphics API for the platform
 * Prefers DirectX 12 on Windows 10+, falls back to DirectX 11
 */
inline GraphicsAPI selectBestGraphicsAPI() {
    // TODO: Check Windows version and DirectX 12 support at runtime
    // For now, default to DirectX 12
    return GraphicsAPI::DirectX12;
}

} // namespace fresh
