#pragma once

#ifdef _WIN32

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace fresh
{

/**
 * @brief Win32 native menu bar implementation
 * 
 * Provides a native Windows menu bar with menus and menu items.
 * Supports nested submenus and command callbacks.
 */
class Win32MenuBar
{
public:
    using MenuCallback = std::function<void()>;

    Win32MenuBar();
    ~Win32MenuBar();

    // Prevent copying
    Win32MenuBar(const Win32MenuBar&) = delete;
    Win32MenuBar& operator=(const Win32MenuBar&) = delete;

    /**
     * @brief Create menu bar and attach to window
     * @param hwnd Window handle
     * @return true if successful
     */
    bool create(HWND hwnd);

    /**
     * @brief Add a top-level menu
     * @param name Menu name (e.g., "File", "Edit")
     * @return Menu ID
     */
    int addMenu(const std::string& name);

    /**
     * @brief Add menu item to a menu
     * @param menuId Parent menu ID
     * @param name Item name
     * @param callback Function to call when clicked
     * @return Item ID
     */
    int addMenuItem(int menuId, const std::string& name, MenuCallback callback);

    /**
     * @brief Add menu separator
     * @param menuId Parent menu ID
     */
    void addSeparator(int menuId);

    /**
     * @brief Add submenu
     * @param menuId Parent menu ID
     * @param name Submenu name
     * @return Submenu ID
     */
    int addSubmenu(int menuId, const std::string& name);

    /**
     * @brief Enable or disable menu item
     * @param itemId Item ID
     * @param enabled true to enable, false to disable
     */
    void setItemEnabled(int itemId, bool enabled);

    /**
     * @brief Set menu item checked state
     * @param itemId Item ID
     * @param checked true to check, false to uncheck
     */
    void setItemChecked(int itemId, bool checked);

    /**
     * @brief Handle menu command
     * @param commandId Command ID from WM_COMMAND
     * @return true if command was handled
     */
    bool handleCommand(int commandId);

    /**
     * @brief Get menu bar handle
     */
    HMENU getHandle() const { return m_menuBar; }

private:
    struct MenuItem {
        int id;
        std::string name;
        MenuCallback callback;
        HMENU submenu;
    };

    HWND m_hwnd;
    HMENU m_menuBar;
    std::map<int, HMENU> m_menus;
    std::map<int, MenuItem> m_items;
    int m_nextId;

    HMENU getMenu(int menuId);
    std::wstring toWideString(const std::string& str);
};

} // namespace fresh

#endif // _WIN32
