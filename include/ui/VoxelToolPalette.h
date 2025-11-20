#pragma once

// This header is deprecated - Windows native UI is used instead
// For Windows: Use ui/native/Win32TerraformingPanel.h
// This stub exists for backward compatibility during migration

#include <functional>

namespace fresh
{
    class TerraformingSystem;
    enum class VoxelType : uint8_t;
}

#ifdef _WIN32
    #include "ui/native/Win32TerraformingPanel.h"
    
    namespace fresh
    {
        /**
         * @brief Compatibility wrapper for VoxelToolPalette
         * 
         * This class wraps Win32TerraformingPanel to maintain API compatibility
         * with legacy code that expects VoxelToolPalette interface.
         */
        class VoxelToolPalette
        {
        public:
            VoxelToolPalette() = default;
            ~VoxelToolPalette() = default;

            // Legacy API compatibility methods
            bool initialize(TerraformingSystem* /*terraformingSystem*/) { return true; }
            void render() { }
            void setVoxelTypeCallback(std::function<void(VoxelType)> /*callback*/) { }
        };
    }
#else
    // Placeholder implementation for non-Windows platforms
    namespace fresh
    {
        class VoxelToolPalette
        {
        public:
            VoxelToolPalette() = default;
            ~VoxelToolPalette() = default;

            bool initialize(TerraformingSystem* /*terraformingSystem*/) { return true; }
            void render() { }
            void setVoxelTypeCallback(std::function<void(VoxelType)> /*callback*/) { }
        };
    }
#endif
