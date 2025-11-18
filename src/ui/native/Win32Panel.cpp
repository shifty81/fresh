#ifdef _WIN32

#include "ui/native/Win32Panel.h"
#include "core/Logger.h"

namespace fresh
{

const wchar_t* Win32Panel::WINDOW_CLASS_NAME = L"FreshWin32Panel";
bool Win32Panel::s_classRegistered = false;

Win32Panel::Win32Panel()
    : m_hwnd(nullptr)
    , m_parent(nullptr)
    , m_width(0)
    , m_height(0)
{
}

Win32Panel::~Win32Panel()
{
    destroy();
}

bool Win32Panel::registerWindowClass()
{
    if (s_classRegistered) {
        return true;
    }

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = nullptr;

    if (!RegisterClassExW(&wc)) {
        LOG_ERROR_C("Failed to register Win32Panel window class", "Win32Panel");
        return false;
    }

    s_classRegistered = true;
    return true;
}

bool Win32Panel::create(HWND parent, int x, int y, int width, int height, const wchar_t* title)
{
    if (m_hwnd) {
        LOG_WARNING_C("Panel already created", "Win32Panel");
        return false;
    }

    if (!registerWindowClass()) {
        return false;
    }

    m_parent = parent;
    m_width = width;
    m_height = height;

    m_hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        title,
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        x, y, width, height,
        parent,
        nullptr,
        GetModuleHandle(nullptr),
        this  // Pass 'this' pointer for retrieval in WM_CREATE
    );

    if (!m_hwnd) {
        LOG_ERROR_C("Failed to create Win32Panel window", "Win32Panel");
        return false;
    }

    return true;
}

void Win32Panel::destroy()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

void Win32Panel::setVisible(bool show)
{
    if (m_hwnd) {
        ShowWindow(m_hwnd, show ? SW_SHOW : SW_HIDE);
    }
}

bool Win32Panel::isVisible() const
{
    return m_hwnd && IsWindowVisible(m_hwnd);
}

void Win32Panel::setPosition(int x, int y)
{
    if (m_hwnd) {
        SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

void Win32Panel::setSize(int width, int height)
{
    if (m_hwnd) {
        m_width = width;
        m_height = height;
        SetWindowPos(m_hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

bool Win32Panel::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
    // Default implementation - derived classes can override
    (void)msg;
    (void)wParam;
    (void)lParam;
    (void)result;
    return false;
}

LRESULT CALLBACK Win32Panel::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Win32Panel* panel = nullptr;

    if (msg == WM_CREATE) {
        // Retrieve 'this' pointer from CreateWindowEx
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        panel = reinterpret_cast<Win32Panel*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(panel));
        
        if (panel) {
            panel->onCreate();
        }
        return 0;
    }

    // Retrieve stored pointer
    panel = reinterpret_cast<Win32Panel*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (panel) {
        LRESULT result = 0;
        if (panel->handleMessage(msg, wParam, lParam, result)) {
            return result;
        }

        switch (msg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                panel->onPaint(hdc);
                EndPaint(hwnd, &ps);
                return 0;
            }

            case WM_SIZE: {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                panel->m_width = width;
                panel->m_height = height;
                panel->onResize(width, height);
                return 0;
            }

            case WM_DESTROY:
                panel->onDestroy();
                return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

} // namespace fresh

#endif // _WIN32
