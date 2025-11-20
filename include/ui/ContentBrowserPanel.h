#pragma once

// This header is deprecated - Windows native UI is used instead
// For Windows: Use ui/native/Win32ContentBrowserPanel.h
// This stub exists for backward compatibility during migration

#ifdef _WIN32
    #include "ui/native/Win32ContentBrowserPanel.h"
    
    #ifndef FRESH_CONTENT_BROWSER_PANEL_ALIAS_DEFINED
    #define FRESH_CONTENT_BROWSER_PANEL_ALIAS_DEFINED
    namespace fresh {
        using ContentBrowserPanel = Win32ContentBrowserPanel;
    }
    #endif
#endif
