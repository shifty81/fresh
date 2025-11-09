#pragma once

#include <functional>

namespace fresh
{

/**
 * @brief Main toolbar with quick-access tool buttons
 *
 * Provides quick access to essential tools like Move, Rotate, Scale,
 * and Play/Pause/Stop buttons for testing.
 */
class EditorToolbar
{
public:
    enum class Tool { Select, Move, Rotate, Scale, Place, Remove, Paint };

    enum class PlayMode { Stopped, Playing, Paused };

    using ToolCallback = std::function<void(Tool)>;
    using PlayModeCallback = std::function<void(PlayMode)>;

    EditorToolbar();
    ~EditorToolbar();

    /**
     * @brief Initialize the toolbar
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Render the toolbar
     */
    void render();

    /**
     * @brief Set current active tool
     */
    void setActiveTool(Tool tool)
    {
        m_activeTool = tool;
    }

    /**
     * @brief Get current active tool
     */
    Tool getActiveTool() const
    {
        return m_activeTool;
    }

    /**
     * @brief Set play mode
     */
    void setPlayMode(PlayMode mode)
    {
        m_playMode = mode;
    }

    /**
     * @brief Get current play mode
     */
    PlayMode getPlayMode() const
    {
        return m_playMode;
    }

    /**
     * @brief Set callback for tool selection
     */
    void setToolCallback(ToolCallback callback)
    {
        m_toolCallback = callback;
    }

    /**
     * @brief Set callback for play mode changes
     */
    void setPlayModeCallback(PlayModeCallback callback)
    {
        m_playModeCallback = callback;
    }

private:
    bool toolButton(const char* label, Tool tool);
    bool playModeButton(const char* label, PlayMode mode);

private:
    Tool m_activeTool;
    PlayMode m_playMode;
    ToolCallback m_toolCallback;
    PlayModeCallback m_playModeCallback;
};

} // namespace fresh
