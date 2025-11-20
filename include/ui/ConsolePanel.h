#pragma once

// This header is deprecated - Windows native UI is used instead
// For Windows: Use ui/native/Win32ConsolePanel.h
// This stub exists for backward compatibility during migration

#ifdef _WIN32
    #include "ui/native/Win32ConsolePanel.h"
    
    namespace fresh
    {
        // Type alias for backward compatibility
        using ConsolePanel = Win32ConsolePanel;
    }
#endif
