#pragma once

#ifdef _WIN32

// Prevent Windows.h from defining min/max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

namespace fresh
{

/**
 * @brief Unreal Engine-inspired color theme for native Win32 UI
 * 
 * Colors based on Unreal Engine 5 Editor's dark theme with professional
 * blue accents and excellent contrast ratios for readability.
 */
struct UnrealStyleTheme
{
    // Main background colors (very dark charcoal with slight blue tint)
    static constexpr COLORREF WindowBackground = RGB(32, 32, 32);           // #202020
    static constexpr COLORREF PanelBackground = RGB(37, 37, 38);            // #252526
    static constexpr COLORREF DarkBackground = RGB(28, 28, 28);             // #1C1C1C
    static constexpr COLORREF MenuBarBackground = RGB(45, 45, 48);          // #2D2D30
    static constexpr COLORREF ToolbarBackground = RGB(45, 45, 48);          // #2D2D30
    
    // Border and separator colors
    static constexpr COLORREF BorderLight = RGB(60, 60, 60);                // #3C3C3C
    static constexpr COLORREF BorderDark = RGB(45, 45, 45);                 // #2D2D2D
    static constexpr COLORREF Separator = RGB(51, 51, 51);                  // #333333
    
    // Text colors (high contrast white/gray)
    static constexpr COLORREF TextPrimary = RGB(241, 241, 241);             // #F1F1F1
    static constexpr COLORREF TextSecondary = RGB(180, 180, 180);           // #B4B4B4
    static constexpr COLORREF TextDisabled = RGB(108, 108, 108);            // #6C6C6C
    static constexpr COLORREF TextHighlight = RGB(255, 255, 255);           // #FFFFFF
    
    // Unreal Blue accent colors (signature blue from UE5)
    static constexpr COLORREF AccentBlue = RGB(0, 122, 204);                // #007ACC - Unreal signature blue
    static constexpr COLORREF AccentBlueHover = RGB(28, 151, 234);          // #1C97EA
    static constexpr COLORREF AccentBlueActive = RGB(0, 98, 163);           // #0062A3
    static constexpr COLORREF AccentBlueDim = RGB(0, 84, 141);              // #00548D
    
    // Button colors
    static constexpr COLORREF ButtonNormal = RGB(60, 60, 60);               // #3C3C3C
    static constexpr COLORREF ButtonHover = RGB(80, 80, 80);                // #505050
    static constexpr COLORREF ButtonActive = RGB(48, 48, 48);               // #303030
    static constexpr COLORREF ButtonDisabled = RGB(45, 45, 45);             // #2D2D2D
    
    // Input field colors
    static constexpr COLORREF InputBackground = RGB(51, 51, 51);            // #333333
    static constexpr COLORREF InputBackgroundHover = RGB(60, 60, 60);       // #3C3C3C
    static constexpr COLORREF InputBackgroundActive = RGB(45, 45, 45);      // #2D2D2D
    static constexpr COLORREF InputBorder = RGB(65, 65, 65);                // #414141
    
    // Selection colors
    static constexpr COLORREF SelectionBackground = RGB(0, 122, 204);       // #007ACC
    static constexpr COLORREF SelectionBackgroundInactive = RGB(60, 60, 60);// #3C3C3C
    static constexpr COLORREF SelectionText = RGB(255, 255, 255);           // #FFFFFF
    
    // Tab colors
    static constexpr COLORREF TabActive = RGB(45, 45, 48);                  // #2D2D30
    static constexpr COLORREF TabInactive = RGB(37, 37, 38);                // #252526
    static constexpr COLORREF TabHover = RGB(52, 52, 54);                   // #343436
    static constexpr COLORREF TabBorder = RGB(0, 122, 204);                 // #007ACC - Active tab indicator
    
    // Status and feedback colors
    static constexpr COLORREF StatusSuccess = RGB(73, 190, 70);             // #49BE46 - Green
    static constexpr COLORREF StatusWarning = RGB(255, 194, 10);            // #FFC20A - Yellow/Gold
    static constexpr COLORREF StatusError = RGB(232, 17, 35);               // #E81123 - Red
    static constexpr COLORREF StatusInfo = RGB(0, 122, 204);                // #007ACC - Blue
    
    // Console-specific color aliases
    static constexpr COLORREF INFO_COLOR = StatusInfo;
    static constexpr COLORREF WARNING_COLOR = StatusWarning;
    static constexpr COLORREF ERROR_COLOR = StatusError;
    static constexpr COLORREF PRIMARY_TEXT = TextPrimary;
    static constexpr COLORREF DARK_BACKGROUND = DarkBackground;
    
    // Special UI elements
    static constexpr COLORREF ScrollbarBackground = RGB(37, 37, 38);        // #252526
    static constexpr COLORREF ScrollbarThumb = RGB(104, 104, 104);          // #686868
    static constexpr COLORREF ScrollbarThumbHover = RGB(158, 158, 158);     // #9E9E9E
    
    // Grid and viewport colors
    static constexpr COLORREF GridMajor = RGB(51, 51, 51);                  // #333333
    static constexpr COLORREF GridMinor = RGB(40, 40, 40);                  // #282828
    static constexpr COLORREF ViewportBackground = RGB(28, 28, 28);         // #1C1C1C
    
    // HUD overlay colors (for in-game UI)
    static constexpr COLORREF HUDBackground = RGB(20, 20, 20);              // #141414 - Semi-transparent in practice
    static constexpr COLORREF HUDOverlay = RGB(40, 40, 45);                 // #28282D
    static constexpr COLORREF HUDAccent = RGB(0, 122, 204);                 // #007ACC
    static constexpr COLORREF HUDHealthBar = RGB(220, 50, 50);              // #DC3232 - Red
    static constexpr COLORREF HUDStaminaBar = RGB(120, 200, 80);            // #78C850 - Green
    static constexpr COLORREF HUDManaBar = RGB(80, 120, 200);               // #5078C8 - Blue
    
    // Crosshair and targeting
    static constexpr COLORREF Crosshair = RGB(255, 255, 255);               // #FFFFFF
    static constexpr COLORREF CrosshairTarget = RGB(255, 100, 100);         // #FF6464 - Red when targeting
    
    // Helper function to create semi-transparent colors (for layering)
    static COLORREF WithAlpha(COLORREF color, int alpha) {
        // Note: Use AlphaBlend or layered windows for true transparency
        // This is a helper for documentation purposes
        (void)alpha;  // Mark as unused - functionality not implemented yet
        return color;
    }
    
    // Font sizes (in points)
    static constexpr int FontSizeNormal = 9;
    static constexpr int FontSizeSmall = 8;
    static constexpr int FontSizeLarge = 10;
    static constexpr int FontSizeTitle = 12;
    
    // Spacing and sizing constants (in pixels)
    static constexpr int PaddingSmall = 4;
    static constexpr int PaddingMedium = 8;
    static constexpr int PaddingLarge = 12;
    static constexpr int BorderRadius = 0;  // Unreal uses sharp corners
    static constexpr int BorderWidth = 1;
    static constexpr int ButtonHeight = 24;
    static constexpr int InputHeight = 22;
    static constexpr int TabHeight = 24;
    
    // Icon sizes
    static constexpr int IconSmall = 16;
    static constexpr int IconMedium = 24;
    static constexpr int IconLarge = 32;
    
    // Helper functions for applying theme
    /**
     * @brief Apply theme colors to a window
     * @param hwnd Window handle to apply theme to
     */
    static void ApplyToWindow(HWND hwnd) {
        // Set background color using window subclassing or custom painting
        // This is a placeholder implementation
        (void)hwnd;
    }
    
    /**
     * @brief Get default font for UI elements
     * @return Handle to default font
     */
    static HFONT GetFont() {
        static HFONT hFont = nullptr;
        if (!hFont) {
            hFont = CreateFontW(
                FontSizeNormal,
                0, 0, 0,
                FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                L"Segoe UI"
            );
        }
        return hFont;
    }
};

} // namespace fresh

#endif // _WIN32
