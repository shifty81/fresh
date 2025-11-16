#pragma once

#ifdef _WIN32

#include <windows.h>
#include <string>
#include <memory>

namespace fresh
{

/**
 * @brief Base class for Win32 UI panels
 * 
 * Provides common functionality for all Win32-based UI panels
 * including window management, message handling, and lifecycle.
 */
class Win32Panel
{
public:
    Win32Panel();
    virtual ~Win32Panel();

    // Prevent copying
    Win32Panel(const Win32Panel&) = delete;
    Win32Panel& operator=(const Win32Panel&) = delete;

    /**
     * @brief Create the panel window
     * @param parent Parent window handle
     * @param x X position
     * @param y Y position
     * @param width Panel width
     * @param height Panel height
     * @return true if successful
     */
    bool create(HWND parent, int x, int y, int width, int height);

    /**
     * @brief Destroy the panel window
     */
    void destroy();

    /**
     * @brief Show or hide the panel
     * @param show true to show, false to hide
     */
    void setVisible(bool show);

    /**
     * @brief Check if panel is visible
     */
    bool isVisible() const;

    /**
     * @brief Get the window handle
     */
    HWND getHandle() const { return m_hwnd; }

    /**
     * @brief Set panel position
     */
    void setPosition(int x, int y);

    /**
     * @brief Set panel size
     */
    void setSize(int width, int height);

    /**
     * @brief Get panel width
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Get panel height
     */
    int getHeight() const { return m_height; }

protected:
    /**
     * @brief Called when panel is created
     * Override to perform custom initialization
     */
    virtual void onCreate() {}

    /**
     * @brief Called when panel needs to be painted
     * Override to perform custom drawing
     * @param hdc Device context for drawing
     */
    virtual void onPaint(HDC hdc) { (void)hdc; }

    /**
     * @brief Called when panel is resized
     * Override to handle resize logic
     * @param width New width
     * @param height New height
     */
    virtual void onResize(int width, int height) { (void)width; (void)height; }

    /**
     * @brief Called when panel is destroyed
     * Override to perform custom cleanup
     */
    virtual void onDestroy() {}

    /**
     * @brief Handle window messages
     * Override to handle custom messages
     * @return true if message was handled
     */
    virtual bool handleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result);

    /**
     * @brief Register panel window class
     */
    static bool registerWindowClass();

    HWND m_hwnd;
    HWND m_parent;
    int m_width;
    int m_height;

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static const wchar_t* WINDOW_CLASS_NAME;
    static bool s_classRegistered;
};

} // namespace fresh

#endif // _WIN32
