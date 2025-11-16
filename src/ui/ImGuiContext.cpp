#include "ui/ImGuiContext.h"

#include "core/Logger.h"
#include "core/Window.h"
#include "renderer/GraphicsAPI.h"
#include "renderer/RenderContext.h"

#ifdef _WIN32
    #include "core/Win32Window.h"
#endif

#ifdef FRESH_IMGUI_AVAILABLE
    // ImGui headers
    #include <imgui.h>
    
    // Platform backends
    #include <imgui_impl_glfw.h>
    #ifdef _WIN32
        #include <imgui_impl_win32.h>
    #endif

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
#ifdef _WIN32
      ,m_win32Window(nullptr),
      m_usingWin32(false)
#endif
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

    // Setup Dear ImGui style - Unreal Engine Dark Theme
    ::ImGui::StyleColorsDark();

    // Customize style for Unreal Engine-like appearance
    ImGuiStyle& style = ::ImGui::GetStyle();
    
    // Unreal uses sharp corners (minimal rounding)
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.PopupRounding = 0.0f;
    
    // Borders - thin and subtle
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;
    
    // Spacing - compact like Unreal
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    
    // Unreal Engine 5 Dark Theme Color Scheme
    ImVec4* colors = style.Colors;
    
    // Background colors - Unreal charcoal gray (#202020, #252526)
    colors[ImGuiCol_WindowBg] = ImVec4(0.125f, 0.125f, 0.125f, 1.00f);           // #202020
    colors[ImGuiCol_ChildBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.00f);            // #252526
    colors[ImGuiCol_PopupBg] = ImVec4(0.125f, 0.125f, 0.125f, 0.98f);            // #202020
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.176f, 0.176f, 0.188f, 1.00f);          // #2D2D30
    
    // Border colors - Subtle gray
    colors[ImGuiCol_Border] = ImVec4(0.235f, 0.235f, 0.235f, 0.50f);             // #3C3C3C
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    // Frame/input field colors - Darker with subtle contrast
    colors[ImGuiCol_FrameBg] = ImVec4(0.200f, 0.200f, 0.200f, 1.00f);            // #333333
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.235f, 0.235f, 0.235f, 1.00f);     // #3C3C3C
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.176f, 0.176f, 0.176f, 1.00f);      // #2D2D2D
    
    // Title bar - Dark
    colors[ImGuiCol_TitleBg] = ImVec4(0.110f, 0.110f, 0.110f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.110f, 0.110f, 0.110f, 0.75f);
    
    // Tabs - Unreal style
    colors[ImGuiCol_Tab] = ImVec4(0.145f, 0.145f, 0.149f, 1.00f);                // #252526
    colors[ImGuiCol_TabHovered] = ImVec4(0.204f, 0.204f, 0.212f, 1.00f);         // #343436
    colors[ImGuiCol_TabActive] = ImVec4(0.176f, 0.176f, 0.188f, 1.00f);          // #2D2D30
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.125f, 0.125f, 0.125f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
    
    // Buttons - Unreal accent blue (#007ACC)
    colors[ImGuiCol_Button] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);             // #007ACC - Unreal signature blue
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.110f, 0.592f, 0.918f, 1.00f);      // #1C97EA
    colors[ImGuiCol_ButtonActive] = ImVec4(0.000f, 0.384f, 0.639f, 1.00f);       // #0062A3
    
    // Headers/collapsing headers
    colors[ImGuiCol_Header] = ImVec4(0.235f, 0.235f, 0.235f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.000f, 0.478f, 0.800f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    
    // Separators
    colors[ImGuiCol_Separator] = ImVec4(0.200f, 0.200f, 0.200f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.000f, 0.478f, 0.800f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    
    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.408f, 0.408f, 0.408f, 1.00f);      // #686868
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.620f, 0.620f, 0.620f, 1.00f); // #9E9E9E
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.620f, 0.620f, 0.620f, 1.00f);
    
    // Sliders - Unreal blue
    colors[ImGuiCol_SliderGrab] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.110f, 0.592f, 0.918f, 1.00f);
    
    // Checkboxes - Unreal blue
    colors[ImGuiCol_CheckMark] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    
    // Resize grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.000f, 0.478f, 0.800f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.000f, 0.478f, 0.800f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.000f, 0.478f, 0.800f, 0.95f);
    
    // Text colors - High contrast white for Unreal style
    colors[ImGuiCol_Text] = ImVec4(0.945f, 0.945f, 0.945f, 1.00f);               // #F1F1F1 - Bright white
    colors[ImGuiCol_TextDisabled] = ImVec4(0.424f, 0.424f, 0.424f, 1.00f);       // #6C6C6C - Disabled gray
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.000f, 0.478f, 0.800f, 0.35f);
    
    // Docking preview - Unreal blue
    colors[ImGuiCol_DockingPreview] = ImVec4(0.000f, 0.478f, 0.800f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.110f, 0.110f, 0.110f, 1.00f);
    
    // Plot colors
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.110f, 0.592f, 0.918f, 1.00f);
    
    // Tables
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.200f, 0.200f, 0.200f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.235f, 0.235f, 0.235f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.176f, 0.176f, 0.176f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    
    // Drag and drop - Unreal blue
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.000f, 0.478f, 0.800f, 0.90f);
    
    // Navigation - Unreal blue
    colors[ImGuiCol_NavHighlight] = ImVec4(0.000f, 0.478f, 0.800f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    
    // Modal window dimming
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.75f);

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

#ifdef _WIN32
bool ImGuiContext::initializeWin32(Win32Window* window, IRenderContext* renderContext)
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

    m_win32Window = window;
    m_renderContext = renderContext;
    m_usingWin32 = true;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();

    // Enable keyboard and gamepad navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Enable docking (optional, for advanced layouts)
    #ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    #endif

    // Setup Dear ImGui style - Modern Dark Theme with readable text
    ::ImGui::StyleColorsDark();

    // Customize style for modern game engine look with better readability
    ImGuiStyle& style = ::ImGui::GetStyle();
    
    // Rounding
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.PopupRounding = 4.0f;
    
    // Borders
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;
    
    // Spacing
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    
    // Apply the same color scheme as GLFW version
    ImVec4* colors = style.Colors;
    
    // Background colors - Very dark with slight blue tint
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.11f, 0.13f, 0.98f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.09f, 0.11f, 1.00f);
    
    // Border colors
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.28f, 0.35f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    // Frame/input colors
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.20f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    
    // Title bar
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.09f, 0.11f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.09f, 0.11f, 0.75f);
    
    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.13f, 0.16f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.48f, 0.70f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.38f, 0.58f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.32f, 0.50f, 1.00f);
    
    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.48f, 0.70f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    
    // Separators
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.28f, 0.35f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.48f, 0.70f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    
    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.38f, 0.45f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.48f, 0.55f, 1.00f);
    
    // Sliders
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.35f, 0.58f, 0.85f, 1.00f);
    
    // Checkboxes
    colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.65f, 0.95f, 1.00f);
    
    // Resize grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.35f, 0.55f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.48f, 0.70f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.28f, 0.48f, 0.70f, 0.95f);
    
    // Text colors
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.52f, 0.55f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.48f, 0.70f, 0.35f);
    
    // Docking
    colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.48f, 0.70f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    
    // Plot colors
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.58f, 0.85f, 1.00f);
    
    // Tables
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.16f, 0.19f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    
    // Drag and drop
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.28f, 0.48f, 0.70f, 0.90f);
    
    // Navigation
    colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.48f, 0.70f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    
    // Modal window dimming
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.75f);

    // Initialize Win32 platform backend
    bool platformInitialized = ImGui_ImplWin32_Init(window->getHandle());
    if (!platformInitialized) {
        LOG_ERROR_C("Failed to initialize ImGui Win32 backend", "ImGuiContext");
        ::ImGui::DestroyContext();
        return false;
    }

    // Initialize graphics backend
    GraphicsAPI api = renderContext->getAPI();
    bool backendInitialized = false;

    switch (api) {
    #if defined(FRESH_OPENGL_SUPPORT) && defined(FRESH_GLEW_AVAILABLE)
    case GraphicsAPI::OpenGL:
        // ImGui expects GLSL version string
        backendInitialized = ImGui_ImplOpenGL3_Init("#version 450");
        if (backendInitialized) {
            LOG_INFO_C("ImGui OpenGL backend initialized", "ImGuiContext");
        } else {
            LOG_ERROR_C("Failed to initialize ImGui OpenGL backend", "ImGuiContext");
        }
        break;
    #endif

    #ifdef FRESH_DIRECTX_SUPPORT
    case GraphicsAPI::DirectX11: {
        DirectX11RenderContext* dx11Context =
            dynamic_cast<DirectX11RenderContext*>(renderContext);
        if (dx11Context) {
            m_backendRenderContext = dx11Context;
            ID3D11Device* device = dx11Context->getD3D11Device();
            ID3D11DeviceContext* deviceContext = dx11Context->getD3D11DeviceContext();

            if (device && deviceContext) {
                backendInitialized = ImGui_ImplDX11_Init(device, deviceContext);
                if (backendInitialized) {
                    LOG_INFO_C("ImGui DirectX 11 backend initialized", "ImGuiContext");
                }
            }
        }
        break;
    }

    case GraphicsAPI::DirectX12: {
        DirectX12RenderContext* dx12Context =
            dynamic_cast<DirectX12RenderContext*>(renderContext);
        if (dx12Context) {
            m_backendRenderContext = dx12Context;
            ID3D12Device* device = dx12Context->getD3D12Device();
            ID3D12DescriptorHeap* srvHeap = dx12Context->getSRVDescriptorHeap();

            if (device && srvHeap) {
                backendInitialized = ImGui_ImplDX12_Init(
                    device, DirectX12RenderContext::FRAME_COUNT, dx12Context->getRTVFormat(),
                    srvHeap, srvHeap->GetCPUDescriptorHandleForHeapStart(),
                    srvHeap->GetGPUDescriptorHandleForHeapStart());

                if (backendInitialized) {
                    LOG_INFO_C("ImGui DirectX 12 backend initialized", "ImGuiContext");
                }
            }
        }
        break;
    }
    #endif

    default:
        LOG_ERROR_C("Unsupported graphics API for ImGui with Win32", "ImGuiContext");
        backendInitialized = false;
        break;
    }

    if (!backendInitialized) {
        LOG_ERROR_C("Failed to initialize ImGui graphics backend", "ImGuiContext");
        ImGui_ImplWin32_Shutdown();
        ::ImGui::DestroyContext();
        return false;
    }

    m_initialized = true;
    LOG_INFO_C("ImGui Win32 context initialized successfully", "ImGuiContext");
    return true;
#endif // FRESH_IMGUI_AVAILABLE
}
#endif // _WIN32

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

#ifdef _WIN32
    if (m_usingWin32) {
        ImGui_ImplWin32_NewFrame();
    } else {
        ImGui_ImplGlfw_NewFrame();
    }
#else
    ImGui_ImplGlfw_NewFrame();
#endif
    
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
        // Still need to cleanup ImGui and platform backend
#ifdef _WIN32
        if (m_usingWin32) {
            ImGui_ImplWin32_Shutdown();
        } else {
            ImGui_ImplGlfw_Shutdown();
        }
#else
        ImGui_ImplGlfw_Shutdown();
#endif
        ::ImGui::DestroyContext();
        m_initialized = false;
        m_backendRenderContext = nullptr;
        m_renderContext = nullptr;
        m_window = nullptr;
#ifdef _WIN32
        m_win32Window = nullptr;
        m_usingWin32 = false;
#endif
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

    // Shutdown platform backend
#ifdef _WIN32
    if (m_usingWin32) {
        ImGui_ImplWin32_Shutdown();
    } else {
        ImGui_ImplGlfw_Shutdown();
    }
#else
    ImGui_ImplGlfw_Shutdown();
#endif

    // Destroy ImGui context
    ::ImGui::DestroyContext();

    m_initialized = false;
    m_backendRenderContext = nullptr;
    m_renderContext = nullptr;
    m_window = nullptr;
#ifdef _WIN32
    m_win32Window = nullptr;
    m_usingWin32 = false;
#endif
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
    // Only capture mouse when actively interacting with UI elements
    // Check if mouse is over a window AND if that window wants input (not just hovering)
    // This allows mouse look to work when not actively using UI controls
    bool hoveringWindow = ::ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | 
                                                    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    bool activeItem = ::ImGui::IsAnyItemActive(); // User is actively using a control (dragging slider, typing, etc.)
    
    // Capture mouse only if ImGui wants it (hovering over interactive elements) or an item is active
    return io.WantCaptureMouse && (hoveringWindow || activeItem);
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
