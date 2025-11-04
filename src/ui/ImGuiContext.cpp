#include "ui/ImGuiContext.h"
#include "core/Window.h"
#include "core/Logger.h"
#include "renderer/RenderContext.h"
#include "renderer/GraphicsAPI.h"

// ImGui headers
#include <imgui.h>
#include <imgui_impl_glfw.h>

// Backend-specific headers
#ifdef FRESH_OPENGL_SUPPORT
#include <imgui_impl_opengl3.h>
#endif

#ifdef FRESH_DIRECTX_SUPPORT
#include <imgui_impl_dx11.h>
#include <imgui_impl_dx12.h>
#endif

#include <GLFW/glfw3.h>

namespace fresh {

ImGuiContext::ImGuiContext()
    : m_initialized(false)
    , m_window(nullptr)
    , m_renderContext(nullptr)
{
}

ImGuiContext::~ImGuiContext() {
    shutdown();
}

bool ImGuiContext::initialize(Window* window, IRenderContext* renderContext) {
    if (m_initialized) {
        LOG_WARNING_C("ImGuiContext already initialized", "ImGuiContext");
        return true;
    }

    if (!window || !renderContext) {
        LOG_ERROR_C("Invalid window or render context", "ImGuiContext");
        return false;
    }

    m_window = window;
    m_renderContext = renderContext;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();
    
    // Enable keyboard and gamepad navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
    // Enable docking and multi-viewport (optional, for advanced layouts)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup Dear ImGui style
    ::ImGui::StyleColorsDark();
    
    // Customize style for game engine look
    ImGuiStyle& style = ::ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    
    // Initialize appropriate graphics API backend first to determine GLFW init mode
    GraphicsAPI api = renderContext->getAPI();
    bool backendInitialized = false;

    // Setup Platform/Renderer backends - use appropriate GLFW init for the graphics API
    bool glfwInitialized = false;
    
    switch (api) {
#ifdef FRESH_OPENGL_SUPPORT
        case GraphicsAPI::OpenGL:
            // Initialize GLFW for OpenGL
            glfwInitialized = ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
            if (glfwInitialized) {
                // ImGui expects GLSL version string
                backendInitialized = ImGui_ImplOpenGL3_Init("#version 450");
                if (backendInitialized) {
                    LOG_INFO_C("ImGui OpenGL backend initialized", "ImGuiContext");
                }
            }
            break;
#endif

#ifdef FRESH_DIRECTX_SUPPORT
        case GraphicsAPI::DirectX11:
            // Initialize GLFW for DirectX (uses "Other" mode)
            glfwInitialized = ImGui_ImplGlfw_InitForOther(window->getHandle(), true);
            if (glfwInitialized) {
                // DirectX 11 backend requires device and context
                // TODO: Get these from the render context when available
                LOG_WARNING_C("DirectX 11 ImGui backend not yet implemented", "ImGuiContext");
                backendInitialized = false;
            }
            break;

        case GraphicsAPI::DirectX12:
            // Initialize GLFW for DirectX (uses "Other" mode)
            glfwInitialized = ImGui_ImplGlfw_InitForOther(window->getHandle(), true);
            if (glfwInitialized) {
                // DirectX 12 backend requires multiple parameters
                // TODO: Get these from the render context when available
                LOG_WARNING_C("DirectX 12 ImGui backend not yet implemented", "ImGuiContext");
                backendInitialized = false;
            }
            break;
#endif

        default:
            LOG_ERROR_C("Unsupported graphics API for ImGui", "ImGuiContext");
            backendInitialized = false;
            break;
    }
    
    if (!glfwInitialized) {
        LOG_ERROR_C("Failed to initialize ImGui GLFW backend", "ImGuiContext");
        ::ImGui::DestroyContext();
        return false;
    }

    if (!backendInitialized) {
        LOG_ERROR_C("Failed to initialize ImGui graphics backend", "ImGuiContext");
        ImGui_ImplGlfw_Shutdown();
        ::ImGui::DestroyContext();
        return false;
    }

    m_initialized = true;
    LOG_INFO_C("ImGui context initialized successfully", "ImGuiContext");
    return true;
}

void ImGuiContext::newFrame() {
    if (!m_initialized) {
        return;
    }

    // Start the Dear ImGui frame
    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
#ifdef FRESH_OPENGL_SUPPORT
        case GraphicsAPI::OpenGL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
#endif

#ifdef FRESH_DIRECTX_SUPPORT
        case GraphicsAPI::DirectX11:
            // ImGui_ImplDX11_NewFrame();
            break;

        case GraphicsAPI::DirectX12:
            // ImGui_ImplDX12_NewFrame();
            break;
#endif

        default:
            break;
    }

    ImGui_ImplGlfw_NewFrame();
    ::ImGui::NewFrame();
}

void ImGuiContext::render() {
    if (!m_initialized) {
        return;
    }

    // Finalize ImGui rendering
    ::ImGui::Render();

    // Render to the appropriate graphics backend
    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
#ifdef FRESH_OPENGL_SUPPORT
        case GraphicsAPI::OpenGL:
            ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
            break;
#endif

#ifdef FRESH_DIRECTX_SUPPORT
        case GraphicsAPI::DirectX11:
            // ImGui_ImplDX11_RenderDrawData(::ImGui::GetDrawData());
            break;

        case GraphicsAPI::DirectX12:
            // ImGui_ImplDX12_RenderDrawData(::ImGui::GetDrawData(), command_list);
            break;
#endif

        default:
            break;
    }

    // Handle multi-viewport if enabled
    ImGuiIO& io = ::ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ::ImGui::UpdatePlatformWindows();
        ::ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiContext::shutdown() {
    if (!m_initialized) {
        return;
    }

    // Shutdown graphics backend
    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
#ifdef FRESH_OPENGL_SUPPORT
        case GraphicsAPI::OpenGL:
            ImGui_ImplOpenGL3_Shutdown();
            break;
#endif

#ifdef FRESH_DIRECTX_SUPPORT
        case GraphicsAPI::DirectX11:
            // ImGui_ImplDX11_Shutdown();
            break;

        case GraphicsAPI::DirectX12:
            // ImGui_ImplDX12_Shutdown();
            break;
#endif

        default:
            break;
    }

    // Shutdown GLFW backend
    ImGui_ImplGlfw_Shutdown();
    
    // Destroy ImGui context
    ::ImGui::DestroyContext();

    m_initialized = false;
    LOG_INFO_C("ImGui context shutdown", "ImGuiContext");
}

bool ImGuiContext::wantCaptureMouse() const {
    if (!m_initialized) {
        return false;
    }
    
    ImGuiIO& io = ::ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool ImGuiContext::wantCaptureKeyboard() const {
    if (!m_initialized) {
        return false;
    }
    
    ImGuiIO& io = ::ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

} // namespace fresh
