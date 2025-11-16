#ifdef _WIN32

#include "core/Win32Window.h"
#include "core/Logger.h"
#include "ui/native/Win32MenuBar.h"

#include <windowsx.h>

// Forward declare ImGui Win32 handler if ImGui is available
#ifdef FRESH_IMGUI_AVAILABLE
    #ifndef IMGUI_IMPL_API
        #define IMGUI_IMPL_API
    #endif
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace fresh
{

Win32Window::Win32Window(uint32_t width, uint32_t height, const std::string& title)
    : m_hwnd(nullptr),
      m_hinstance(GetModuleHandle(nullptr)),
      m_hdc(nullptr),
      m_hglrc(nullptr),
      m_width(width),
      m_height(height),
      m_title(title),
      m_shouldClose(false),
      m_framebufferResized(false),
      m_useOpenGL(false)
{
}

Win32Window::~Win32Window()
{
    if (m_hglrc) {
        wglDeleteContext(m_hglrc);
        m_hglrc = nullptr;
    }

    if (m_hdc) {
        ReleaseDC(m_hwnd, m_hdc);
        m_hdc = nullptr;
    }

    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    UnregisterClassW(WINDOW_CLASS_NAME, m_hinstance);
}

bool Win32Window::initialize(bool useOpenGL)
{
    m_useOpenGL = useOpenGL;

    if (!registerWindowClass()) {
        LOG_ERROR_C("Failed to register window class", "Win32Window");
        return false;
    }

    if (!createWindow()) {
        LOG_ERROR_C("Failed to create window", "Win32Window");
        return false;
    }

    if (useOpenGL && !initializeOpenGL()) {
        LOG_ERROR_C("Failed to initialize OpenGL context", "Win32Window");
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    LOG_INFO_C("Win32 window created successfully", "Win32Window");
    return true;
}

bool Win32Window::registerWindowClass()
{
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hinstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassExW(&wc)) {
        LOG_ERROR_C("Failed to register window class: " + std::to_string(GetLastError()),
                    "Win32Window");
        return false;
    }

    return true;
}

bool Win32Window::createWindow()
{
    // Calculate window size to get desired client area
    RECT rect = {0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height)};
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = WS_EX_APPWINDOW;

    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    // Convert title to wide string
    int titleLen = MultiByteToWideChar(CP_UTF8, 0, m_title.c_str(), -1, nullptr, 0);
    std::wstring wTitle(titleLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, m_title.c_str(), -1, &wTitle[0], titleLen);

    m_hwnd = CreateWindowExW(
        exStyle,
        WINDOW_CLASS_NAME,
        wTitle.c_str(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        m_hinstance,
        this // Pass 'this' pointer to be retrieved in WM_CREATE
    );

    if (!m_hwnd) {
        LOG_ERROR_C("Failed to create window: " + std::to_string(GetLastError()), "Win32Window");
        return false;
    }

    // Get device context
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc) {
        LOG_ERROR_C("Failed to get device context", "Win32Window");
        return false;
    }

    return true;
}

bool Win32Window::initializeOpenGL()
{
    // Set pixel format
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (pixelFormat == 0) {
        LOG_ERROR_C("Failed to choose pixel format", "Win32Window");
        return false;
    }

    if (!SetPixelFormat(m_hdc, pixelFormat, &pfd)) {
        LOG_ERROR_C("Failed to set pixel format", "Win32Window");
        return false;
    }

    // Create OpenGL context
    m_hglrc = wglCreateContext(m_hdc);
    if (!m_hglrc) {
        LOG_ERROR_C("Failed to create OpenGL context", "Win32Window");
        return false;
    }

    if (!wglMakeCurrent(m_hdc, m_hglrc)) {
        LOG_ERROR_C("Failed to make OpenGL context current", "Win32Window");
        return false;
    }

    LOG_INFO_C("OpenGL context created successfully", "Win32Window");
    return true;
}

void Win32Window::pollEvents()
{
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            m_shouldClose = true;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void Win32Window::setSize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    
    if (m_hwnd) {
        RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
        DWORD style = GetWindowLong(m_hwnd, GWL_STYLE);
        DWORD exStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
        AdjustWindowRectEx(&rect, style, FALSE, exStyle);
        
        SetWindowPos(m_hwnd, nullptr, 0, 0,
                     rect.right - rect.left,
                     rect.bottom - rect.top,
                     SWP_NOMOVE | SWP_NOZORDER);
    }
}

void Win32Window::swapBuffers()
{
    if (m_useOpenGL && m_hdc) {
        SwapBuffers(m_hdc);
    }
}

Win32MenuBar* Win32Window::getMenuBar()
{
    if (!m_menuBar && m_hwnd) {
        m_menuBar = std::make_unique<Win32MenuBar>();
        if (!m_menuBar->create(m_hwnd)) {
            LOG_ERROR_C("Failed to create menu bar", "Win32Window");
            m_menuBar.reset();
            return nullptr;
        }
    }
    return m_menuBar.get();
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Let ImGui handle the message first if it's available
#ifdef FRESH_IMGUI_AVAILABLE
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
        return true;
    }
#endif

    Win32Window* window = nullptr;

    if (uMsg == WM_CREATE) {
        // Retrieve the 'this' pointer passed during CreateWindowEx
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<Win32Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window) {
        switch (uMsg) {
        case WM_COMMAND: {
            // Handle menu commands
            int commandId = LOWORD(wParam);
            if (window->m_menuBar && window->m_menuBar->handleCommand(commandId)) {
                return 0;
            }
            break;
        }

        case WM_CLOSE:
            window->m_shouldClose = true;
            return 0;

        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            if (width != window->m_width || height != window->m_height) {
                window->m_width = width;
                window->m_height = height;
                window->m_framebufferResized = true;
            }
            return 0;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (window->m_keyCallback) {
                window->m_keyCallback(static_cast<int>(wParam), true);
            }
            return 0;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (window->m_keyCallback) {
                window->m_keyCallback(static_cast<int>(wParam), false);
            }
            return 0;

        case WM_LBUTTONDOWN:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_LBUTTON, true);
            }
            return 0;

        case WM_LBUTTONUP:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_LBUTTON, false);
            }
            return 0;

        case WM_RBUTTONDOWN:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_RBUTTON, true);
            }
            return 0;

        case WM_RBUTTONUP:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_RBUTTON, false);
            }
            return 0;

        case WM_MBUTTONDOWN:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_MBUTTON, true);
            }
            return 0;

        case WM_MBUTTONUP:
            if (window->m_mouseButtonCallback) {
                window->m_mouseButtonCallback(VK_MBUTTON, false);
            }
            return 0;

        case WM_MOUSEMOVE: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            if (window->m_mouseMoveCallback) {
                window->m_mouseMoveCallback(x, y);
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

} // namespace fresh

#endif // _WIN32
