#pragma once

#include <memory>

struct GLFWwindow;

namespace fresh
{

class Window;
class IRenderContext;

/**
 * @brief Manages ImGui context and integration
 *
 * Handles ImGui initialization, frame management, and rendering
 * for all supported graphics APIs (OpenGL, DirectX 11/12).
 */
class ImGuiContext
{
public:
    ImGuiContext();
    ~ImGuiContext();

    // Prevent copying
    ImGuiContext(const ImGuiContext&) = delete;
    ImGuiContext& operator=(const ImGuiContext&) = delete;

    /**
     * @brief Initialize ImGui with the given window and render context
     * @param window Window to attach ImGui to
     * @param renderContext Render context for backend-specific setup
     * @return true if initialization was successful
     */
    bool initialize(Window* window, IRenderContext* renderContext);

    /**
     * @brief Begin a new ImGui frame
     * Call this at the start of each frame before any ImGui calls
     */
    void newFrame();

    /**
     * @brief Render ImGui and submit to GPU
     * Call this after all ImGui calls for the frame
     */
    void render();

    /**
     * @brief Shutdown ImGui and cleanup resources
     */
    void shutdown();

    /**
     * @brief Check if ImGui is initialized
     * @return true if initialized
     */
    bool isInitialized() const
    {
        return m_initialized;
    }

    /**
     * @brief Check if ImGui wants to capture mouse input
     * @return true if ImGui is using the mouse
     */
    bool wantCaptureMouse() const;

    /**
     * @brief Check if ImGui wants to capture keyboard input
     * @return true if ImGui is using the keyboard
     */
    bool wantCaptureKeyboard() const;

private:
    bool m_initialized;
    Window* m_window;
    IRenderContext* m_renderContext;

    // Cached pointers to avoid repeated dynamic_cast
    void* m_backendRenderContext; // Points to DirectX11/12RenderContext or nullptr for OpenGL
};

} // namespace fresh
