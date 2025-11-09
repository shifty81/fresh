#pragma once

#include <functional>

#include "editor/TerraformingSystem.h"
#include "voxel/VoxelTypes.h"

namespace fresh
{

/**
 * @brief Voxel Tool Palette Panel
 *
 * Provides UI for selecting voxel editing tools (Add, Remove, Smooth, Paint, etc.)
 * and configuring tool parameters.
 */
class VoxelToolPalette
{
public:
    using ToolChangeCallback = std::function<void(TerraformTool)>;
    using VoxelTypeCallback = std::function<void(VoxelType)>;
    using ToolSizeCallback = std::function<void(int)>;

    VoxelToolPalette();
    ~VoxelToolPalette();

    /**
     * @brief Initialize the palette
     * @param terraformingSystem Terraforming system to control
     * @return true if successful
     */
    bool initialize(TerraformingSystem* terraformingSystem);

    /**
     * @brief Render the palette UI
     */
    void render();

    /**
     * @brief Set panel visibility
     * @param visible true to show, false to hide
     */
    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    /**
     * @brief Check if panel is visible
     * @return true if visible
     */
    bool isVisible() const
    {
        return m_visible;
    }

    /**
     * @brief Set callback for tool changes
     */
    void setToolChangeCallback(ToolChangeCallback callback)
    {
        m_toolChangeCallback = callback;
    }

    /**
     * @brief Set callback for voxel type changes
     */
    void setVoxelTypeCallback(VoxelTypeCallback callback)
    {
        m_voxelTypeCallback = callback;
    }

    /**
     * @brief Set callback for tool size changes
     */
    void setToolSizeCallback(ToolSizeCallback callback)
    {
        m_toolSizeCallback = callback;
    }

private:
    void renderToolSelector();
    void renderMaterialPicker();
    void renderToolSettings();
    void renderQuickActions();

    bool toolButton(const char* label, TerraformTool tool);
    bool voxelTypeButton(const char* label, VoxelType type);

private:
    bool m_visible;
    TerraformingSystem* m_terraformingSystem;
    TerraformTool m_currentTool;
    VoxelType m_currentVoxelType;
    int m_toolSize;

    ToolChangeCallback m_toolChangeCallback;
    VoxelTypeCallback m_voxelTypeCallback;
    ToolSizeCallback m_toolSizeCallback;
};

} // namespace fresh
