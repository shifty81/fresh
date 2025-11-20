#pragma once

// This header is deprecated - Windows native UI is used instead
// For Windows: Use ui/native/Win32SceneHierarchyPanel.h
// This stub exists for backward compatibility during migration

#ifdef _WIN32
    #include "ui/native/Win32SceneHierarchyPanel.h"
    namespace fresh {
        using SceneHierarchyPanel = Win32SceneHierarchyPanel;
    }
#endif
