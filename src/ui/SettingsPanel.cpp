#include "ui/SettingsPanel.h"

#include <fstream>
#include <sstream>

#include "core/Logger.h"
#ifdef _WIN32
    #include "core/Win32Window.h"
    #include "input/Win32InputManager.h"
#else
    #include "core/Window.h"
    #include "input/InputManager.h"
#endif

#ifdef FRESH_IMGUI_AVAILABLE
    #include <imgui.h>
#endif

namespace fresh
{

SettingsPanel::SettingsPanel()
    : m_visible(false),
      m_window(nullptr),
      m_inputManager(nullptr),
      m_selectedResolutionIndex(0),
      m_currentResolutionIndex(0),
      m_fullscreen(false),
      m_vsync(true),
      m_fpsLimit(60),
      m_masterVolume(1.0f),
      m_musicVolume(0.7f),
      m_sfxVolume(0.8f),
      m_mouseSensitivity(0.5f),
      m_invertMouseY(false),
      m_fov(75.0f),
      m_renderDistance(150.0f),
      m_settingsChanged(false)
{
    // Initialize common resolutions
    m_availableResolutions = {
        {1280, 720, "1280 x 720 (HD)"},
        {1366, 768, "1366 x 768"},
        {1600, 900, "1600 x 900 (HD+)"},
        {1920, 1080, "1920 x 1080 (Full HD)"},
        {2560, 1440, "2560 x 1440 (2K)"},
        {3840, 2160, "3840 x 2160 (4K)"}};
}

SettingsPanel::~SettingsPanel()
{
    if (m_settingsChanged) {
        saveSettings();
    }
}

bool SettingsPanel::initialize(WindowType* window, InputManagerType* inputManager)
{
    if (!window || !inputManager) {
        LOG_ERROR_C("Invalid parameters for SettingsPanel", "SettingsPanel");
        return false;
    }

    m_window = window;
    m_inputManager = inputManager;

    // Load settings from config file
    loadSettings();

    LOG_INFO_C("Settings panel initialized", "SettingsPanel");
    return true;
}

void SettingsPanel::render()
{
#ifdef FRESH_IMGUI_AVAILABLE
    if (!m_visible) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Settings", &m_visible, ImGuiWindowFlags_NoCollapse)) {
        // Tabs for different settings categories
        if (ImGui::BeginTabBar("SettingsTabs")) {
            if (ImGui::BeginTabItem("Video")) {
                renderVideoSettings();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Audio")) {
                renderAudioSettings();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Controls")) {
                renderControlsSettings();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Gameplay")) {
                renderGameplaySettings();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();
        ImGui::Spacing();

        // Bottom buttons
        if (ImGui::Button("Apply", ImVec2(100, 30))) {
            saveSettings();
            m_settingsChanged = false;
            LOG_INFO_C("Settings applied and saved", "SettingsPanel");
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset to Defaults", ImVec2(150, 30))) {
            // Reset to default values
            m_mouseSensitivity = 0.5f;
            m_invertMouseY = false;
            m_fov = 75.0f;
            m_masterVolume = 1.0f;
            m_musicVolume = 0.7f;
            m_sfxVolume = 0.8f;
            m_renderDistance = 150.0f;
            m_vsync = true;
            m_fpsLimit = 60;

            setMouseSensitivity(m_mouseSensitivity);
            m_settingsChanged = true;
            LOG_INFO_C("Settings reset to defaults", "SettingsPanel");
        }

        ImGui::SameLine();
        if (ImGui::Button("Close", ImVec2(100, 30))) {
            m_visible = false;
        }
    }
    ImGui::End();
#endif
}

void SettingsPanel::renderVideoSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::Text("Display Settings");
    ImGui::Separator();
    ImGui::Spacing();

    // Resolution selector
    ImGui::Text("Resolution:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(300);

    const char* currentResolution = m_availableResolutions[m_selectedResolutionIndex].label.c_str();
    if (ImGui::BeginCombo("##Resolution", currentResolution)) {
        for (size_t i = 0; i < m_availableResolutions.size(); i++) {
            bool isSelected = (m_selectedResolutionIndex == static_cast<int>(i));
            if (ImGui::Selectable(m_availableResolutions[i].label.c_str(), isSelected)) {
                m_selectedResolutionIndex = static_cast<int>(i);
                if (m_selectedResolutionIndex != m_currentResolutionIndex) {
                    m_settingsChanged = true;
                }
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (m_selectedResolutionIndex != m_currentResolutionIndex) {
        ImGui::SameLine();
        if (ImGui::Button("Apply Resolution")) {
            applyResolution();
        }
    }

    ImGui::Spacing();

    // Fullscreen toggle
    if (ImGui::Checkbox("Fullscreen", &m_fullscreen)) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();

    // VSync toggle
    if (ImGui::Checkbox("VSync", &m_vsync)) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();

    // FPS Limit
    ImGui::Text("FPS Limit:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    if (ImGui::SliderInt("##FPSLimit", &m_fpsLimit, 30, 240, "%d FPS")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Current window: %dx%d",
                        m_window ? m_window->getWidth() : 0,
                        m_window ? m_window->getHeight() : 0);
#endif
}

void SettingsPanel::renderAudioSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::Text("Volume Settings");
    ImGui::Separator();
    ImGui::Spacing();

    // Master volume
    ImGui::Text("Master Volume:");
    if (ImGui::SliderFloat("##MasterVolume", &m_masterVolume, 0.0f, 1.0f, "%.2f")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();

    // Music volume
    ImGui::Text("Music Volume:");
    if (ImGui::SliderFloat("##MusicVolume", &m_musicVolume, 0.0f, 1.0f, "%.2f")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();

    // SFX volume
    ImGui::Text("Sound Effects Volume:");
    if (ImGui::SliderFloat("##SFXVolume", &m_sfxVolume, 0.0f, 1.0f, "%.2f")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Note: Audio system integration in progress");
#endif
}

void SettingsPanel::renderControlsSettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::Text("Mouse Settings");
    ImGui::Separator();
    ImGui::Spacing();

    // Mouse sensitivity
    ImGui::Text("Mouse Sensitivity:");
    if (ImGui::SliderFloat("##MouseSensitivity", &m_mouseSensitivity, 0.1f, 5.0f, "%.2f")) {
        setMouseSensitivity(m_mouseSensitivity);
        m_settingsChanged = true;
    }

    ImGui::Spacing();

    // Invert mouse Y
    if (ImGui::Checkbox("Invert Mouse Y-Axis", &m_invertMouseY)) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Camera settings
    ImGui::Text("Camera Settings");
    ImGui::Spacing();

    // Field of view
    ImGui::Text("Field of View:");
    if (ImGui::SliderFloat("##FOV", &m_fov, 60.0f, 120.0f, "%.1f degrees")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Tip: Lower sensitivity = more precise aiming");
    ImGui::TextDisabled("Tip: Higher FOV = wider view but may cause distortion");
#endif
}

void SettingsPanel::renderGameplaySettings()
{
#ifdef FRESH_IMGUI_AVAILABLE
    ImGui::Spacing();
    ImGui::Text("Performance Settings");
    ImGui::Separator();
    ImGui::Spacing();

    // Render distance
    ImGui::Text("Render Distance:");
    if (ImGui::SliderFloat("##RenderDistance", &m_renderDistance, 50.0f, 500.0f, "%.1f blocks")) {
        m_settingsChanged = true;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Higher render distance may impact performance");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Gameplay Options");
    ImGui::Spacing();
    ImGui::TextDisabled("Additional gameplay settings will be added here");
#endif
}

void SettingsPanel::setMouseSensitivity(float sensitivity)
{
    m_mouseSensitivity = sensitivity;
    if (m_inputManager) {
        m_inputManager->setMouseSensitivity(sensitivity);
        LOG_INFO_C("Mouse sensitivity set to: " + std::to_string(sensitivity), "SettingsPanel");
    }
}

void SettingsPanel::applyResolution()
{
    if (m_window && m_selectedResolutionIndex >= 0 &&
        m_selectedResolutionIndex < static_cast<int>(m_availableResolutions.size())) {

        const auto& res = m_availableResolutions[m_selectedResolutionIndex];
        m_window->setSize(res.width, res.height);
        m_currentResolutionIndex = m_selectedResolutionIndex;
        m_settingsChanged = true;

        LOG_INFO_C("Resolution changed to: " + res.label, "SettingsPanel");
    }
}

void SettingsPanel::loadSettings()
{
    std::ifstream file("settings.cfg");
    if (!file.is_open()) {
        LOG_INFO_C("No settings file found, using defaults", "SettingsPanel");
        // Apply default mouse sensitivity
        setMouseSensitivity(m_mouseSensitivity);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            std::getline(iss, value);

            // Parse settings
            if (key == "mouse_sensitivity") {
                m_mouseSensitivity = std::stof(value);
                setMouseSensitivity(m_mouseSensitivity);
            } else if (key == "invert_mouse_y") {
                m_invertMouseY = (value == "1" || value == "true");
            } else if (key == "fov") {
                m_fov = std::stof(value);
            } else if (key == "master_volume") {
                m_masterVolume = std::stof(value);
            } else if (key == "music_volume") {
                m_musicVolume = std::stof(value);
            } else if (key == "sfx_volume") {
                m_sfxVolume = std::stof(value);
            } else if (key == "render_distance") {
                m_renderDistance = std::stof(value);
            } else if (key == "vsync") {
                m_vsync = (value == "1" || value == "true");
            } else if (key == "fps_limit") {
                m_fpsLimit = std::stoi(value);
            } else if (key == "fullscreen") {
                m_fullscreen = (value == "1" || value == "true");
            } else if (key == "resolution_index") {
                int idx = std::stoi(value);
                if (idx >= 0 && idx < static_cast<int>(m_availableResolutions.size())) {
                    m_selectedResolutionIndex = idx;
                    m_currentResolutionIndex = idx;
                }
            }
        }
    }

    file.close();
    LOG_INFO_C("Settings loaded from file", "SettingsPanel");
}

void SettingsPanel::saveSettings()
{
    std::ofstream file("settings.cfg");
    if (!file.is_open()) {
        LOG_ERROR_C("Failed to save settings file", "SettingsPanel");
        return;
    }

    file << "mouse_sensitivity=" << m_mouseSensitivity << "\n";
    file << "invert_mouse_y=" << (m_invertMouseY ? "1" : "0") << "\n";
    file << "fov=" << m_fov << "\n";
    file << "master_volume=" << m_masterVolume << "\n";
    file << "music_volume=" << m_musicVolume << "\n";
    file << "sfx_volume=" << m_sfxVolume << "\n";
    file << "render_distance=" << m_renderDistance << "\n";
    file << "vsync=" << (m_vsync ? "1" : "0") << "\n";
    file << "fps_limit=" << m_fpsLimit << "\n";
    file << "fullscreen=" << (m_fullscreen ? "1" : "0") << "\n";
    file << "resolution_index=" << m_currentResolutionIndex << "\n";

    file.close();
    LOG_INFO_C("Settings saved to file", "SettingsPanel");
}

} // namespace fresh
