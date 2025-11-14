#include "ui/ImGuiContext.h"

#include "core/Logger.h"
#include "core/Window.h"
#include "renderer/GraphicsAPI.h"
#include "renderer/RenderContext.h"

#ifdef FRESH_IMGUI_AVAILABLE
    // ImGui headers
    #include <imgui.h>
    #include <imgui_impl_glfw.h>

    // Backend-specific headers
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
        #include <imgui_impl_opengl3.h>
    #endif

    #ifdef FRESH_DIRECTX_SUPPORT
        #include "renderer/backends/DirectX11RenderContext.h"
        #include "renderer/backends/DirectX12RenderContext.h"

        #include <imgui_impl_dx11.h>
        #include <imgui_impl_dx12.h>
    #endif
#endif // FRESH_IMGUI_AVAILABLE

#include <GLFW/glfw3.h>

namespace fresh
{

ImGuiContext::ImGuiContext()
    : m_initialized(false),
      m_window(nullptr),
      m_renderContext(nullptr),
      m_backendRenderContext(nullptr)
{
}

ImGuiContext::~ImGuiContext()
{
    shutdown();
}

bool ImGuiContext::initialize(Window* window, IRenderContext* renderContext)
{
#ifndef FRESH_IMGUI_AVAILABLE
    (void)window;
    (void)renderContext;
    LOG_WARNING_C("ImGui not available - Editor UI disabled", "ImGuiContext");
    return false;
#else
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

    // Enable docking (optional, for advanced layouts)
    // Only available when ImGui is built with docking branch
    #ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    #endif

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
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
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
            // Get DirectX 11 device and context from render context
            DirectX11RenderContext* dx11Context =
                dynamic_cast<DirectX11RenderContext*>(renderContext);
            if (dx11Context) {
                m_backendRenderContext = dx11Context; // Cache for later use
                ID3D11Device* device = dx11Context->getD3D11Device();
                ID3D11DeviceContext* deviceContext = dx11Context->getD3D11DeviceContext();

                if (device && deviceContext) {
                    backendInitialized = ImGui_ImplDX11_Init(device, deviceContext);
                    if (backendInitialized) {
                        LOG_INFO_C("ImGui DirectX 11 backend initialized", "ImGuiContext");
                    } else {
                        LOG_ERROR_C("Failed to initialize ImGui DirectX 11 backend",
                                    "ImGuiContext");
                    }
                } else {
                    LOG_ERROR_C("Invalid DirectX 11 device or context", "ImGuiContext");
                    backendInitialized = false;
                }
            } else {
                LOG_ERROR_C("Failed to cast render context to DirectX11RenderContext",
                            "ImGuiContext");
                backendInitialized = false;
            }
        }
        break;

    case GraphicsAPI::DirectX12:
        // Initialize GLFW for DirectX (uses "Other" mode)
        glfwInitialized = ImGui_ImplGlfw_InitForOther(window->getHandle(), true);
        if (glfwInitialized) {
            // Get DirectX 12 device and context from render context
            DirectX12RenderContext* dx12Context =
                dynamic_cast<DirectX12RenderContext*>(renderContext);
            if (dx12Context) {
                m_backendRenderContext = dx12Context; // Cache for later use
                ID3D12Device* device = dx12Context->getD3D12Device();
                ID3D12DescriptorHeap* srvHeap = dx12Context->getSRVDescriptorHeap();

                if (device && srvHeap) {
                    // ImGui DirectX 12 requires:
                    // - Device
                    // - Number of frames in flight
                    // - RTV format
                    // - SRV descriptor heap
                    // - SRV CPU and GPU descriptor handles
                    backendInitialized = ImGui_ImplDX12_Init(
                        device, DirectX12RenderContext::FRAME_COUNT, dx12Context->getRTVFormat(),
                        srvHeap, srvHeap->GetCPUDescriptorHandleForHeapStart(),
                        srvHeap->GetGPUDescriptorHandleForHeapStart());

                    if (backendInitialized) {
                        LOG_INFO_C("ImGui DirectX 12 backend initialized", "ImGuiContext");
                    } else {
                        LOG_ERROR_C("Failed to initialize ImGui DirectX 12 backend",
                                    "ImGuiContext");
                    }
                } else {
                    LOG_ERROR_C("Invalid DirectX 12 device or descriptor heap", "ImGuiContext");
                    backendInitialized = false;
                }
            } else {
                LOG_ERROR_C("Failed to cast render context to DirectX12RenderContext",
                            "ImGuiContext");
                backendInitialized = false;
            }
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
#endif // FRESH_IMGUI_AVAILABLE
}

void ImGuiContext::newFrame()
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_initialized || !m_renderContext) {
        return;
    }

    // Start the Dear ImGui frame
    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    case GraphicsAPI::OpenGL:
        ImGui_ImplOpenGL3_NewFrame();
        break;
    #endif

    #ifdef FRESH_DIRECTX_SUPPORT
    case GraphicsAPI::DirectX11:
        ImGui_ImplDX11_NewFrame();
        break;

    case GraphicsAPI::DirectX12:
        ImGui_ImplDX12_NewFrame();
        break;
    #endif

    default:
        break;
    }

    ImGui_ImplGlfw_NewFrame();
    ::ImGui::NewFrame();
#endif // FRESH_IMGUI_AVAILABLE
}

void ImGuiContext::render()
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_initialized || !m_renderContext) {
        return;
    }

    // Finalize ImGui rendering
    ::ImGui::Render();

    // Render to the appropriate graphics backend
    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    case GraphicsAPI::OpenGL:
        ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
        break;
    #endif

    #ifdef FRESH_DIRECTX_SUPPORT
    case GraphicsAPI::DirectX11:
        ImGui_ImplDX11_RenderDrawData(::ImGui::GetDrawData());
        break;

    case GraphicsAPI::DirectX12: {
        // Use cached DirectX12RenderContext pointer
        if (m_backendRenderContext) {
            DirectX12RenderContext* dx12Context =
                static_cast<DirectX12RenderContext*>(m_backendRenderContext);
            ID3D12GraphicsCommandList* commandList = dx12Context->getCommandList();
            if (commandList) {
                ImGui_ImplDX12_RenderDrawData(::ImGui::GetDrawData(), commandList);
            }
        }
        break;
    }
    #endif

    default:
        break;
    }

    // Handle multi-viewport if enabled
    // Only available when ImGui is built with docking branch and multi-viewport support
    ImGuiIO& io = ::ImGui::GetIO();
    #ifdef IMGUI_HAS_VIEWPORT
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ::ImGui::UpdatePlatformWindows();
        ::ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    #endif
#endif // FRESH_IMGUI_AVAILABLE
}

void ImGuiContext::shutdown()
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_initialized) {
        return;
    }

    // Shutdown graphics backend only if render context is still valid
    if (!m_renderContext) {
        LOG_WARNING_C("Render context is null during shutdown, skipping backend cleanup",
                      "ImGuiContext");
        // Still need to cleanup ImGui and GLFW
        ImGui_ImplGlfw_Shutdown();
        ::ImGui::DestroyContext();
        m_initialized = false;
        m_backendRenderContext = nullptr;
        m_renderContext = nullptr; // Ensure it's null
        m_window = nullptr;        // Clear the window pointer as well
        return;
    }

    GraphicsAPI api = m_renderContext->getAPI();

    switch (api) {
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    case GraphicsAPI::OpenGL:
        ImGui_ImplOpenGL3_Shutdown();
        break;
    #endif

    #ifdef FRESH_DIRECTX_SUPPORT
    case GraphicsAPI::DirectX11:
        ImGui_ImplDX11_Shutdown();
        break;

    case GraphicsAPI::DirectX12:
        ImGui_ImplDX12_Shutdown();
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
    m_backendRenderContext = nullptr;
    m_renderContext = nullptr; // Clear the render context pointer to avoid dangling pointer
    m_window = nullptr;        // Clear the window pointer as well
    LOG_INFO_C("ImGui context shutdown", "ImGuiContext");
#endif // FRESH_IMGUI_AVAILABLE
}

bool ImGuiContext::wantCaptureMouse() const
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_initialized) {
        return false;
    }

    ImGuiIO& io = ::ImGui::GetIO();
    // Only capture mouse if ImGui wants it AND we're actually hovering over a window
    // This allows mouse interaction with the 3D viewport even when editor is open
    return io.WantCaptureMouse || ::ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
#else
    return false;
#endif
}

bool ImGuiContext::wantCaptureKeyboard() const
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_initialized) {
        return false;
    }

    ImGuiIO& io = ::ImGui::GetIO();
    return io.WantCaptureKeyboard;
#else
    return false;
#endif
}

} // namespace fresh
