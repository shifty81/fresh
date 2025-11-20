#include "ui/NativeMainMenu.h"

#include <filesystem>
#include <sstream>

#include "core/Logger.h"

#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace fs = std::filesystem;

namespace fresh
{

NativeMainMenu::NativeMainMenu()
    : m_parentWindow(nullptr),
      m_mainDialog(nullptr),
      m_createNewWorld(false),
      m_loadWorld(false),
      m_isWorld3D(true),
      m_worldSeed(0)
{
}

NativeMainMenu::~NativeMainMenu() {}

bool NativeMainMenu::initialize(HWND parentWindow)
{
    m_parentWindow = parentWindow;
    LOG_INFO_C("Initializing Native Main Menu", "NativeMainMenu");
    scanWorldSaves();
    return true;
}

bool NativeMainMenu::show()
{
    // Create and show modal dialog
    INT_PTR result =
        DialogBoxParam(GetModuleHandle(nullptr),
                       nullptr, // We'll create it dynamically
                       m_parentWindow, mainMenuDialogProc, reinterpret_cast<LPARAM>(this));

    return result == IDOK;
}

void NativeMainMenu::applyDarkTheme(HWND hwnd)
{
    // Enable Windows 11 dark mode
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

    // Set window background to dark
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(UnrealColors::WindowBg));
}

void NativeMainMenu::applyDarkThemeToControl(HWND control)
{
    // Set control colors for dark theme
    // This is a simplified version - full implementation would need custom drawing
    SetClassLongPtr(control, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(UnrealColors::InputBg));
}

void NativeMainMenu::scanWorldSaves()
{
    m_worldSaves.clear();

    const std::string savesDir = "saves";

    if (!fs::exists(savesDir)) {
        LOG_INFO_C("Saves directory does not exist, creating it", "NativeMainMenu");
        try {
            fs::create_directory(savesDir);
        } catch (const std::exception& e) {
            LOG_ERROR_C("Failed to create saves directory: " + std::string(e.what()),
                        "NativeMainMenu");
            return;
        }
    }

    try {
        for (const auto& entry : fs::directory_iterator(savesDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".world") {
                std::wstring filename = entry.path().stem().wstring();
                m_worldSaves.push_back(filename);
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR_C("Failed to scan saves directory: " + std::string(e.what()), "NativeMainMenu");
    }

    LOG_INFO_C("Found " + std::to_string(m_worldSaves.size()) + " saved worlds", "NativeMainMenu");
}

INT_PTR CALLBACK NativeMainMenu::mainMenuDialogProc(HWND hwnd, UINT msg, WPARAM wParam,
                                                    LPARAM lParam)
{
    NativeMainMenu* menu = nullptr;

    if (msg == WM_INITDIALOG) {
        // Store pointer to our class instance
        SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
        menu = reinterpret_cast<NativeMainMenu*>(lParam);
        menu->m_mainDialog = hwnd;
    } else {
        menu = reinterpret_cast<NativeMainMenu*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!menu)
        return FALSE;

    switch (msg) {
    case WM_INITDIALOG: {
        // Center dialog on screen
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd, nullptr, (screenWidth - width) / 2, (screenHeight - height) / 2, width,
                     height, SWP_NOZORDER);

        // Apply dark theme
        menu->applyDarkTheme(hwnd);

        // Set window title
        SetWindowText(hwnd, L"Fresh Voxel Engine");

        // Create controls
        menu->createMainMenuDialog(hwnd);

        return TRUE;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_CREATE_WORLD: {
            // Show create world dialog
            INT_PTR result = DialogBoxParam(GetModuleHandle(nullptr), nullptr, hwnd,
                                            createWorldDialogProc, reinterpret_cast<LPARAM>(menu));

            if (result == IDOK) {
                menu->m_createNewWorld = true;
                EndDialog(hwnd, IDOK);
            }
            return TRUE;
        }

        case ID_BTN_LOAD_WORLD: {
            // Show load world dialog
            INT_PTR result = DialogBoxParam(GetModuleHandle(nullptr), nullptr, hwnd,
                                            loadWorldDialogProc, reinterpret_cast<LPARAM>(menu));

            if (result == IDOK) {
                menu->m_loadWorld = true;
                EndDialog(hwnd, IDOK);
            }
            return TRUE;
        }

        case ID_BTN_EXIT:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
        // Make static text and dialog background dark
        SetBkColor((HDC)wParam, UnrealColors::WindowBg);
        SetTextColor((HDC)wParam, UnrealColors::TextNormal);
        return (INT_PTR)CreateSolidBrush(UnrealColors::WindowBg);
    }

    return FALSE;
}

void NativeMainMenu::createMainMenuDialog(HWND hwnd)
{
    // Set dialog size
    SetWindowPos(hwnd, nullptr, 0, 0, 600, 400, SWP_NOMOVE | SWP_NOZORDER);

    // Title
    HWND title = CreateWindowEx(0, L"STATIC", L"FRESH VOXEL ENGINE",
                                WS_CHILD | WS_VISIBLE | SS_CENTER, 150, 30, 300, 40, hwnd,
                                (HMENU)ID_STATIC_TITLE, GetModuleHandle(nullptr), nullptr);

    HFONT titleFont = CreateFontW(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                 DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
    SendMessage(title, WM_SETFONT, (WPARAM)titleFont, TRUE);

    // Buttons
    int buttonY = 120;
    int buttonWidth = 300;
    int buttonHeight = 50;
    int spacing = 20;
    int centerX = (600 - buttonWidth) / 2;

    // Create New World button
    CreateWindowEx(0, L"BUTTON", L"Create New World", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   centerX, buttonY, buttonWidth, buttonHeight, hwnd, (HMENU)ID_BTN_CREATE_WORLD,
                   GetModuleHandle(nullptr), nullptr);

    buttonY += buttonHeight + spacing;

    // Load World button
    CreateWindowEx(0, L"BUTTON", L"Load Existing World", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   centerX, buttonY, buttonWidth, buttonHeight, hwnd, (HMENU)ID_BTN_LOAD_WORLD,
                   GetModuleHandle(nullptr), nullptr);

    buttonY += buttonHeight + spacing;

    // Exit button
    CreateWindowEx(0, L"BUTTON", L"Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, centerX, buttonY,
                   buttonWidth, buttonHeight, hwnd, (HMENU)ID_BTN_EXIT, GetModuleHandle(nullptr),
                   nullptr);

    // Version info at bottom
    CreateWindowEx(0, L"STATIC", L"Version 0.1.0 - C++20 Voxel Engine",
                   WS_CHILD | WS_VISIBLE | SS_CENTER, 100, 350, 400, 20, hwnd, nullptr,
                   GetModuleHandle(nullptr), nullptr);
}

INT_PTR CALLBACK NativeMainMenu::createWorldDialogProc(HWND hwnd, UINT msg, WPARAM wParam,
                                                       LPARAM lParam)
{
    NativeMainMenu* menu = nullptr;

    if (msg == WM_INITDIALOG) {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
        menu = reinterpret_cast<NativeMainMenu*>(lParam);
    } else {
        menu = reinterpret_cast<NativeMainMenu*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!menu)
        return FALSE;

    switch (msg) {
    case WM_INITDIALOG: {
        // Center on parent
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd, nullptr, (screenWidth - width) / 2, (screenHeight - height) / 2, width,
                     height, SWP_NOZORDER);

        menu->applyDarkTheme(hwnd);
        SetWindowText(hwnd, L"Create New World");
        menu->createWorldCreationDialog(hwnd);
        return TRUE;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_CREATE: {
            // Get world name
            wchar_t worldName[256];
            GetDlgItemText(hwnd, ID_EDIT_WORLD_NAME, worldName, 256);
            menu->m_newWorldName = worldName;

            // Get seed
            wchar_t seedText[32];
            GetDlgItemText(hwnd, ID_EDIT_SEED, seedText, 32);
            if (wcslen(seedText) > 0) {
                menu->m_worldSeed = _wtoi(seedText);
            } else {
                menu->m_worldSeed = static_cast<int>(time(nullptr));
            }

            // Get world type
            menu->m_isWorld3D = (IsDlgButtonChecked(hwnd, ID_RADIO_3D) == BST_CHECKED);

            LOG_INFO_C("Creating new world: " +
                           std::string(menu->m_newWorldName.begin(), menu->m_newWorldName.end()),
                       "NativeMainMenu");

            EndDialog(hwnd, IDOK);
            return TRUE;
        }

        case ID_BTN_CANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
        SetBkColor((HDC)wParam, UnrealColors::WindowBg);
        SetTextColor((HDC)wParam, UnrealColors::TextNormal);
        return (INT_PTR)CreateSolidBrush(UnrealColors::WindowBg);
    }

    return FALSE;
}

void NativeMainMenu::createWorldCreationDialog(HWND hwnd)
{
    SetWindowPos(hwnd, nullptr, 0, 0, 550, 400, SWP_NOMOVE | SWP_NOZORDER);

    int y = 20;
    int labelX = 20;
    int controlX = 150;
    int controlWidth = 360;

    // World Name label
    CreateWindowEx(0, L"STATIC", L"World Name:", WS_CHILD | WS_VISIBLE, labelX, y, 120, 20, hwnd,
                   nullptr, GetModuleHandle(nullptr), nullptr);

    // World Name input
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"New World", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                   controlX, y, controlWidth, 24, hwnd, (HMENU)ID_EDIT_WORLD_NAME,
                   GetModuleHandle(nullptr), nullptr);

    y += 40;

    // World Seed label
    CreateWindowEx(0, L"STATIC", L"World Seed:", WS_CHILD | WS_VISIBLE, labelX, y, 120, 20, hwnd,
                   nullptr, GetModuleHandle(nullptr), nullptr);

    // World Seed input
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
                   WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER, controlX, y, controlWidth,
                   24, hwnd, (HMENU)ID_EDIT_SEED, GetModuleHandle(nullptr), nullptr);

    y += 30;

    CreateWindowEx(0, L"STATIC", L"(Leave empty for random)", WS_CHILD | WS_VISIBLE, controlX, y,
                   300, 20, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);

    y += 40;

    // World Type label
    CreateWindowEx(0, L"STATIC", L"World Type:", WS_CHILD | WS_VISIBLE, labelX, y, 120, 20, hwnd,
                   nullptr, GetModuleHandle(nullptr), nullptr);

    y += 25;

    // 3D World radio button
    HWND radio3D =
        CreateWindowEx(0, L"BUTTON", L"3D World (Full Voxel)",
                       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, controlX, y, 300, 20,
                       hwnd, (HMENU)ID_RADIO_3D, GetModuleHandle(nullptr), nullptr);
    CheckDlgButton(hwnd, ID_RADIO_3D, BST_CHECKED);

    y += 30;

    // 2D World radio button
    CreateWindowEx(0, L"BUTTON", L"2D World", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, controlX,
                   y, 300, 20, hwnd, (HMENU)ID_RADIO_2D, GetModuleHandle(nullptr), nullptr);

    y = 320;

    // Buttons
    int buttonWidth = 150;
    int buttonHeight = 40;
    int centerX = (550 - (buttonWidth * 2 + 20)) / 2;

    CreateWindowEx(0, L"BUTTON", L"Create",
                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, centerX, y,
                   buttonWidth, buttonHeight, hwnd, (HMENU)ID_BTN_CREATE, GetModuleHandle(nullptr),
                   nullptr);

    CreateWindowEx(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   centerX + buttonWidth + 20, y, buttonWidth, buttonHeight, hwnd,
                   (HMENU)ID_BTN_CANCEL, GetModuleHandle(nullptr), nullptr);
}

INT_PTR CALLBACK NativeMainMenu::loadWorldDialogProc(HWND hwnd, UINT msg, WPARAM wParam,
                                                     LPARAM lParam)
{
    NativeMainMenu* menu = nullptr;

    if (msg == WM_INITDIALOG) {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
        menu = reinterpret_cast<NativeMainMenu*>(lParam);
    } else {
        menu = reinterpret_cast<NativeMainMenu*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!menu)
        return FALSE;

    switch (msg) {
    case WM_INITDIALOG: {
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd, nullptr, (screenWidth - width) / 2, (screenHeight - height) / 2, width,
                     height, SWP_NOZORDER);

        menu->applyDarkTheme(hwnd);
        SetWindowText(hwnd, L"Load World");
        menu->createWorldLoadingDialog(hwnd);
        return TRUE;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_LOAD: {
            // Get selected world
            HWND listBox = GetDlgItem(hwnd, ID_LIST_WORLDS);
            int sel = (int)SendMessage(listBox, LB_GETCURSEL, 0, 0);

            if (sel != LB_ERR) {
                wchar_t worldName[256];
                SendMessage(listBox, LB_GETTEXT, sel, (LPARAM)worldName);
                menu->m_loadWorldName = worldName;

                LOG_INFO_C("Loading world: " + std::string(menu->m_loadWorldName.begin(),
                                                           menu->m_loadWorldName.end()),
                           "NativeMainMenu");

                EndDialog(hwnd, IDOK);
            }
            return TRUE;
        }

        case ID_BTN_CANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        return TRUE;

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        SetBkColor((HDC)wParam, UnrealColors::WindowBg);
        SetTextColor((HDC)wParam, UnrealColors::TextNormal);
        return (INT_PTR)CreateSolidBrush(UnrealColors::WindowBg);
    }

    return FALSE;
}

void NativeMainMenu::createWorldLoadingDialog(HWND hwnd)
{
    SetWindowPos(hwnd, nullptr, 0, 0, 500, 400, SWP_NOMOVE | SWP_NOZORDER);

    // Title
    CreateWindowEx(0, L"STATIC", L"Select a world to load:", WS_CHILD | WS_VISIBLE, 20, 20, 460, 20,
                   hwnd, nullptr, GetModuleHandle(nullptr), nullptr);

    // List box for worlds
    HWND listBox =
        CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", nullptr,
                       WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER, 20, 50, 460,
                       250, hwnd, (HMENU)ID_LIST_WORLDS, GetModuleHandle(nullptr), nullptr);

    // Populate list
    for (const auto& world : m_worldSaves) {
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)world.c_str());
    }

    if (m_worldSaves.empty()) {
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)L"No saved worlds found");
        EnableWindow(GetDlgItem(hwnd, ID_BTN_LOAD), FALSE);
    } else {
        SendMessage(listBox, LB_SETCURSEL, 0, 0);
    }

    // Buttons
    int buttonY = 320;
    int buttonWidth = 150;
    int buttonHeight = 40;
    int centerX = (500 - (buttonWidth * 2 + 20)) / 2;

    CreateWindowEx(0, L"BUTTON", L"Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,
                   centerX, buttonY, buttonWidth, buttonHeight, hwnd, (HMENU)ID_BTN_LOAD,
                   GetModuleHandle(nullptr), nullptr);

    CreateWindowEx(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   centerX + buttonWidth + 20, buttonY, buttonWidth, buttonHeight, hwnd,
                   (HMENU)ID_BTN_CANCEL, GetModuleHandle(nullptr), nullptr);
}

} // namespace fresh
