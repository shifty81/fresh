#pragma once

#include <functional>
#include <string>
#include <vector>

namespace fresh
{

#ifdef _WIN32
class Win32Window;
class Win32InputManager;
using WindowType = Win32Window;
using InputManagerType = Win32InputManager;
#else
class Window;
class InputManager;
using WindowType = Window;
using InputManagerType = InputManager;
#endif

/**
 * @brief Settings Panel for game configuration
 *
 * Provides UI for adjusting game settings like mouse sensitivity,
 * resolution, graphics options, and controls.
 */
class SettingsPanel
{
public:
    using SettingsCallback = std::function<void()>;

    SettingsPanel();
    ~SettingsPanel();

    /**
     * @brief Initialize the settings panel
     * @param window Window reference for resolution changes
     * @param inputManager Input manager reference for sensitivity changes
     * @return true if successful
     */
    bool initialize(WindowType* window, InputManagerType* inputManager);

    /**
     * @brief Render the settings panel UI
     */
    void render();

    /**
     * @brief Show/hide the settings panel
     */
    void setVisible(bool visible) { m_visible = visible; }

    /**
     * @brief Check if settings panel is visible
     */
    bool isVisible() const { return m_visible; }

    /**
     * @brief Toggle settings panel visibility
     */
    void toggle() { m_visible = !m_visible; }

    /**
     * @brief Load settings from config file
     */
    void loadSettings();

    /**
     * @brief Save settings to config file
     */
    void saveSettings();

    /**
     * @brief Get current mouse sensitivity
     */
    float getMouseSensitivity() const { return m_mouseSensitivity; }

    /**
     * @brief Set mouse sensitivity
     */
    void setMouseSensitivity(float sensitivity);

private:
    void renderVideoSettings();
    void renderAudioSettings();
    void renderControlsSettings();
    void renderGameplaySettings();

    void applyResolution();

private:
    bool m_visible;
    WindowType* m_window;
    InputManagerType* m_inputManager;

    // Video Settings
    struct Resolution
    {
        int width;
        int height;
        std::string label;
    };
    std::vector<Resolution> m_availableResolutions;
    int m_selectedResolutionIndex;
    int m_currentResolutionIndex;
    bool m_fullscreen;
    bool m_vsync;
    int m_fpsLimit;

    // Audio Settings
    float m_masterVolume;
    float m_musicVolume;
    float m_sfxVolume;

    // Controls Settings
    float m_mouseSensitivity;
    bool m_invertMouseY;
    float m_fov;

    // Gameplay Settings
    float m_renderDistance;

    // State flags
    bool m_settingsChanged;
};

} // namespace fresh
