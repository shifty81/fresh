#ifdef _WIN32

#include "ui/native/Win32ViewportPanel.h"
#include "core/Logger.h"

namespace fresh
{

const wchar_t* Win32ViewportPanel::WINDOW_CLASS_NAME = L"FreshVoxelViewport";
bool Win32ViewportPanel::s_classRegistered = false;

Win32ViewportPanel::Win32ViewportPanel()
    : m_hwnd(nullptr)
    , m_parent(nullptr)
    , m_width(800)
    , m_height(600)
    , m_wasResized(false)
{
}

Win32ViewportPanel::~Win32ViewportPanel()
{
    destroy();
}

bool Win32ViewportPanel::registerWindowClass()
{
    if (s_classRegistered) {
        return true;
    }

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_OWNDC important for DirectX
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassExW(&wc)) {
        LOG_ERROR_C("Failed to register viewport window class", "Win32ViewportPanel");
        return false;
    }

    s_classRegistered = true;
    return true;
}

bool Win32ViewportPanel::create(HWND parent, int x, int y, int width, int height)
{
    if (m_hwnd) {
        LOG_WARNING_C("Viewport panel already created", "Win32ViewportPanel");
        return false;
    }

    if (!parent) {
        LOG_ERROR_C("Invalid parent window", "Win32ViewportPanel");
        return false;
    }

    // Register window class
    if (!registerWindowClass()) {
        return false;
    }

    m_parent = parent;
    m_width = width;
    m_height = height;

    // Create child window for viewport
    // WS_CHILD makes it a child window
    // WS_VISIBLE makes it initially visible
    // WS_CLIPSIBLINGS and WS_CLIPCHILDREN are important for DirectX rendering
    m_hwnd = CreateWindowExW(
        0,                          // Extended style
        WINDOW_CLASS_NAME,          // Class name
        L"Viewport",                // Window title (not visible for child windows)
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        x, y,                       // Position
        width, height,              // Size
        parent,                     // Parent window
        nullptr,                    // Menu (not used for child windows)
        GetModuleHandle(nullptr),   // Instance
        this                        // lpParam (pointer to this object)
    );

    if (!m_hwnd) {
        DWORD error = GetLastError();
        LOG_ERROR_C("Failed to create viewport window. Error code: " + std::to_string(error), "Win32ViewportPanel");
        return false;
    }

    LOG_INFO_C("Viewport panel created successfully", "Win32ViewportPanel");
    return true;
}

void Win32ViewportPanel::destroy()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

void Win32ViewportPanel::setVisible(bool show)
{
    if (m_hwnd) {
        ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE);
    }
}

bool Win32ViewportPanel::isVisible() const
{
    if (m_hwnd) {
        return IsWindowVisible(m_hwnd) != FALSE;
    }
    return false;
}

void Win32ViewportPanel::setPosition(int x, int y)
{
    if (m_hwnd) {
        SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, 
                     SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void Win32ViewportPanel::setSize(int width, int height)
{
    if (m_hwnd && (m_width != width || m_height != height)) {
        m_width = width;
        m_height = height;
        m_wasResized = true;
        
        SetWindowPos(m_hwnd, nullptr, 0, 0, width, height,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        
        LOG_INFO_C("Viewport resized to " + std::to_string(width) + "x" + std::to_string(height), "Win32ViewportPanel");
    }
}

bool Win32ViewportPanel::isMouseInViewport(int screenX, int screenY) const
{
    if (!m_hwnd) {
        return false;
    }

    // Get viewport window rectangle in screen coordinates
    RECT rect;
    if (!GetWindowRect(m_hwnd, &rect)) {
        return false;
    }

    // Check if point is within rectangle
    POINT pt = {screenX, screenY};
    return PtInRect(&rect, pt) != FALSE;
}

void Win32ViewportPanel::screenToViewport(int screenX, int screenY, int& outViewportX, int& outViewportY) const
{
    if (!m_hwnd) {
        outViewportX = screenX;
        outViewportY = screenY;
        return;
    }

    // Convert screen coordinates to viewport window coordinates
    POINT pt = {screenX, screenY};
    ScreenToClient(m_hwnd, &pt);
    
    outViewportX = pt.x;
    outViewportY = pt.y;
}

LRESULT CALLBACK Win32ViewportPanel::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Win32ViewportPanel* panel = nullptr;

    if (msg == WM_NCCREATE) {
        // Store pointer to Win32ViewportPanel in window user data
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        panel = reinterpret_cast<Win32ViewportPanel*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(panel));
    } else {
        panel = reinterpret_cast<Win32ViewportPanel*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (panel) {
        switch (msg) {
        case WM_SIZE: {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            if (width != static_cast<UINT>(panel->m_width) || height != static_cast<UINT>(panel->m_height)) {
                panel->m_width = static_cast<int>(width);
                panel->m_height = static_cast<int>(height);
                panel->m_wasResized = true;
                LOG_INFO_C("Viewport panel resized to " + std::to_string(width) + "x" + std::to_string(height), "Win32ViewportPanel");
            }
            return 0;
        }

        case WM_PAINT: {
            // Validate the window to prevent continuous WM_PAINT messages
            // DirectX will handle actual rendering
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // Draw black background (will be overdrawn by DirectX)
            FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_ERASEBKGND:
            // Don't erase background - DirectX will draw everything
            return 1;

        case WM_DESTROY:
            panel->m_hwnd = nullptr;
            return 0;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

} // namespace fresh

#endif // _WIN32
