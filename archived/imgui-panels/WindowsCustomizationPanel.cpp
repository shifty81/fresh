// DEPRECATED: This file contains ImGui code and is no longer used.
// Archived: 2025-11-22
// Replaced by: Win32TerraformingPanel and Win32SettingsDialog (native Win32 UI)
// Fresh Voxel Engine uses Windows Native Win32 UI exclusively.
// See WINDOWS_NATIVE_ONLY_POLICY.md and IMGUI_DEPRECATION_NOTICE.md

#if 0 // ImGui code disabled - not compiled
#ifdef _WIN32

#include "ui/WindowsCustomizationPanel.h"
#include "ui/WindowsThemeManager.h"
#include "ui/WindowsDialogManager.h"
#include "ui/WindowsTaskbarManager.h"
#include "core/Logger.h"

namespace fresh
{

WindowsCustomizationPanel::WindowsCustomizationPanel()
    : m_visible(false),
      m_themeManager(nullptr),
      m_dialogManager(nullptr),
      m_taskbarManager(nullptr),
      m_selectedThemeIndex(0),
      m_showCustomThemeEditor(false),
      m_progressValue(0),
      m_progressMax(100)
{
    // Initialize custom theme colors
    m_customBackground[0] = 0.12f;
    m_customBackground[1] = 0.12f;
    m_customBackground[2] = 0.12f;
    
    m_customAccent[0] = 0.0f;
    m_customAccent[1] = 0.47f;
    m_customAccent[2] = 0.83f;
    
    m_customText[0] = 0.88f;
    m_customText[1] = 0.88f;
    m_customText[2] = 0.88f;
}

WindowsCustomizationPanel::~WindowsCustomizationPanel()
{
}

bool WindowsCustomizationPanel::initialize(WindowsThemeManager* themeManager,
                                          WindowsDialogManager* dialogManager,
                                          WindowsTaskbarManager* taskbarManager)
{
    m_themeManager = themeManager;
    m_dialogManager = dialogManager;
    m_taskbarManager = taskbarManager;

    LOG_INFO_C("Windows Customization Panel initialized", "WindowsCustomizationPanel");
    return true;
}

void WindowsCustomizationPanel::render()
{
    if (!m_visible) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 700), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Windows Customization", &m_visible, ImGuiWindowFlags_NoCollapse)) {
        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), 
                          "Windows Native Integration");
        ImGui::Text("Customize your Windows experience with native features");
        ImGui::Separator();
        ImGui::Spacing();

        // Tabs for different sections
        if (ImGui::BeginTabBar("WindowsCustomizationTabs")) {
            if (ImGui::BeginTabItem("Theme")) {
                renderThemeSection();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dialogs")) {
                renderDialogSection();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Taskbar")) {
                renderTaskbarSection();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();

    // Render custom theme editor as separate window if open
    if (m_showCustomThemeEditor) {
        renderCustomThemeEditor();
    }
#endif
}

void WindowsCustomizationPanel::renderThemeSection()
{
    if (!m_themeManager) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 
                          "Theme manager not available");
        return;
    }

    ImGui::Spacing();
    ImGui::Text("Theme Selection");
    ImGui::Separator();
    ImGui::Spacing();

    // Get available themes
    auto themes = m_themeManager->getAvailableThemes();
    
    // Theme selector
    ImGui::Text("Active Theme:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(300);
    
    if (ImGui::BeginCombo("##ThemeSelector", themes[m_selectedThemeIndex].c_str())) {
        for (size_t i = 0; i < themes.size(); i++) {
            bool isSelected = (m_selectedThemeIndex == static_cast<int>(i));
            if (ImGui::Selectable(themes[i].c_str(), isSelected)) {
                m_selectedThemeIndex = static_cast<int>(i);
                m_themeManager->setTheme(static_cast<WindowsTheme>(i));
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Windows theme info
    ImGui::Text("Windows Theme Detection");
    ImGui::Spacing();
    
    bool isDarkMode = m_themeManager->isWindowsDarkMode();
    ImGui::Text("Windows Dark Mode:");
    ImGui::SameLine();
    if (isDarkMode) {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "Enabled");
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Disabled");
    }

    ImGui::Spacing();
    
    uint32_t accentColor = m_themeManager->getWindowsAccentColor();
    float r = ((accentColor >> 24) & 0xFF) / 255.0f;
    float g = ((accentColor >> 16) & 0xFF) / 255.0f;
    float b = ((accentColor >> 8) & 0xFF) / 255.0f;
    
    ImGui::Text("Windows Accent Color:");
    ImGui::SameLine();
    ImGui::ColorButton("##AccentColor", ImVec4(r, g, b, 1.0f), 
                      ImGuiColorEditFlags_NoTooltip, ImVec2(40, 20));

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Custom theme editor button
    if (ImGui::Button("Open Custom Theme Editor", ImVec2(250, 30))) {
        m_showCustomThemeEditor = true;
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Tip: Use 'Auto' theme to match Windows system theme");
    ImGui::TextDisabled("Tip: Custom theme allows full color customization");
#endif
}

void WindowsCustomizationPanel::renderDialogSection()
{
    if (!m_dialogManager) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 
                          "Dialog manager not available");
        return;
    }

    ImGui::Spacing();
    ImGui::Text("Native Windows Dialogs");
    ImGui::Separator();
    ImGui::Spacing();

    // File dialog demos
    ImGui::Text("File Dialogs");
    ImGui::Spacing();

    if (ImGui::Button("Open File Dialog", ImVec2(200, 30))) {
        std::vector<FileFilter> filters = {
            {"Text Files", "*.txt"},
            {"Image Files", "*.png;*.jpg;*.bmp"},
            {"All Files", "*.*"}
        };
        
        auto files = m_dialogManager->showOpenFileDialog("Select a file", filters, false);
        if (!files.empty()) {
            LOG_INFO_C("Selected file: " + files[0], "WindowsCustomizationPanel");
        }
    }

    ImGui::SameLine();
    
    if (ImGui::Button("Open Multiple Files", ImVec2(200, 30))) {
        std::vector<FileFilter> filters = {
            {"All Files", "*.*"}
        };
        
        auto files = m_dialogManager->showOpenFileDialog("Select files", filters, true);
        LOG_INFO_C("Selected " + std::to_string(files.size()) + " files", 
                   "WindowsCustomizationPanel");
    }

    ImGui::Spacing();

    if (ImGui::Button("Save File Dialog", ImVec2(200, 30))) {
        std::vector<FileFilter> filters = {
            {"Text Files", "*.txt"},
            {"All Files", "*.*"}
        };
        
        auto file = m_dialogManager->showSaveFileDialog("Save file", "untitled.txt", filters);
        if (!file.empty()) {
            LOG_INFO_C("Save location: " + file, "WindowsCustomizationPanel");
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Select Folder", ImVec2(200, 30))) {
        auto folder = m_dialogManager->showFolderBrowserDialog("Select a folder");
        if (!folder.empty()) {
            LOG_INFO_C("Selected folder: " + folder, "WindowsCustomizationPanel");
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Message box demos
    ImGui::Text("Message Boxes");
    ImGui::Spacing();

    if (ImGui::Button("Information", ImVec2(150, 30))) {
        m_dialogManager->showMessageBox(
            "Information",
            "This is an information message with native Windows styling.",
            MessageBoxButtons::OK,
            MessageBoxIcon::Information);
    }

    ImGui::SameLine();

    if (ImGui::Button("Warning", ImVec2(150, 30))) {
        m_dialogManager->showMessageBox(
            "Warning",
            "This is a warning message.",
            MessageBoxButtons::OK,
            MessageBoxIcon::Warning);
    }

    ImGui::SameLine();

    if (ImGui::Button("Error", ImVec2(150, 30))) {
        m_dialogManager->showMessageBox(
            "Error",
            "This is an error message.",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
    }

    ImGui::Spacing();

    if (ImGui::Button("Yes/No Question", ImVec2(200, 30))) {
        auto result = m_dialogManager->showMessageBox(
            "Question",
            "Do you want to continue?",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Question);
        
        if (result == MessageBoxResult::Yes) {
            LOG_INFO_C("User clicked Yes", "WindowsCustomizationPanel");
        } else {
            LOG_INFO_C("User clicked No", "WindowsCustomizationPanel");
        }
    }

    ImGui::Spacing();
    ImGui::TextDisabled("All dialogs use native Windows styling and behavior");
#endif
}

void WindowsCustomizationPanel::renderTaskbarSection()
{
    if (!m_taskbarManager) {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 
                          "Taskbar manager not available");
        return;
    }

    ImGui::Spacing();
    ImGui::Text("Windows Taskbar Integration");
    ImGui::Separator();
    ImGui::Spacing();

    // Progress indicator
    ImGui::Text("Taskbar Progress Indicator");
    ImGui::Spacing();

    ImGui::SliderInt("Progress Value", &m_progressValue, 0, m_progressMax);
    
    ImGui::Spacing();

    if (ImGui::Button("Show Normal Progress", ImVec2(200, 30))) {
        m_taskbarManager->setProgressState(TaskbarProgressState::Normal);
        m_taskbarManager->setProgressValue(m_progressValue, m_progressMax);
    }

    ImGui::SameLine();

    if (ImGui::Button("Show Error Progress", ImVec2(200, 30))) {
        m_taskbarManager->setProgressState(TaskbarProgressState::Error);
        m_taskbarManager->setProgressValue(m_progressValue, m_progressMax);
    }

    ImGui::Spacing();

    if (ImGui::Button("Show Paused Progress", ImVec2(200, 30))) {
        m_taskbarManager->setProgressState(TaskbarProgressState::Paused);
        m_taskbarManager->setProgressValue(m_progressValue, m_progressMax);
    }

    ImGui::SameLine();

    if (ImGui::Button("Show Indeterminate", ImVec2(200, 30))) {
        m_taskbarManager->setProgressState(TaskbarProgressState::Indeterminate);
    }

    ImGui::Spacing();

    if (ImGui::Button("Clear Progress", ImVec2(200, 30))) {
        m_taskbarManager->setProgressState(TaskbarProgressState::NoProgress);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Flash taskbar
    ImGui::Text("Taskbar Attention");
    ImGui::Spacing();

    if (ImGui::Button("Flash Taskbar (3 times)", ImVec2(250, 30))) {
        m_taskbarManager->flashTaskbar(false);
    }

    ImGui::SameLine();

    if (ImGui::Button("Flash Until Focus", ImVec2(250, 30))) {
        m_taskbarManager->flashTaskbar(true);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Overlay icon
    ImGui::Text("Overlay Icon");
    ImGui::Spacing();

    ImGui::TextDisabled("Overlay icons require icon files to be available");
    
    if (ImGui::Button("Clear Overlay Icon", ImVec2(200, 30))) {
        m_taskbarManager->clearOverlayIcon();
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Tip: Progress indicators show build/loading status");
    ImGui::TextDisabled("Tip: Flash taskbar to notify users of important events");
#endif
}

void WindowsCustomizationPanel::renderCustomThemeEditor()
{
    if (!m_themeManager) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(400, 450), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Custom Theme Editor", &m_showCustomThemeEditor)) {
        ImGui::Text("Design Your Custom Theme");
        ImGui::Separator();
        ImGui::Spacing();

        // Color pickers
        ImGui::Text("Background Color:");
        ImGui::ColorEdit3("##Background", m_customBackground);
        
        ImGui::Spacing();

        ImGui::Text("Accent Color:");
        ImGui::ColorEdit3("##Accent", m_customAccent);
        
        ImGui::Spacing();

        ImGui::Text("Text Color:");
        ImGui::ColorEdit3("##Text", m_customText);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Preview
        ImGui::Text("Preview:");
        ImGui::PushStyleColor(ImGuiCol_ChildBg, 
                             ImVec4(m_customBackground[0], m_customBackground[1], 
                                   m_customBackground[2], 1.0f));
        ImGui::BeginChild("PreviewArea", ImVec2(0, 100), true);
        
        ImGui::PushStyleColor(ImGuiCol_Text,
                             ImVec4(m_customText[0], m_customText[1], m_customText[2], 1.0f));
        ImGui::Text("Sample text with custom colors");
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Button,
                             ImVec4(m_customAccent[0], m_customAccent[1], m_customAccent[2], 1.0f));
        ImGui::Button("Sample Button");
        ImGui::PopStyleColor();
        
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Spacing();

        // Apply button
        if (ImGui::Button("Apply Custom Theme", ImVec2(200, 35))) {
            // Convert float RGB to uint32_t RGBA
            auto toRGBA = [](float r, float g, float b) -> uint32_t {
                uint8_t ri = static_cast<uint8_t>(r * 255);
                uint8_t gi = static_cast<uint8_t>(g * 255);
                uint8_t bi = static_cast<uint8_t>(b * 255);
                return (ri << 24) | (gi << 16) | (bi << 8) | 0xFF;
            };

            ThemeColors customColors;
            customColors.background = toRGBA(m_customBackground[0], m_customBackground[1], m_customBackground[2]);
            customColors.foreground = toRGBA(m_customBackground[0] * 1.1f, m_customBackground[1] * 1.1f, m_customBackground[2] * 1.1f);
            customColors.accent = toRGBA(m_customAccent[0], m_customAccent[1], m_customAccent[2]);
            customColors.text = toRGBA(m_customText[0], m_customText[1], m_customText[2]);
            customColors.textDisabled = toRGBA(m_customText[0] * 0.5f, m_customText[1] * 0.5f, m_customText[2] * 0.5f);
            customColors.border = toRGBA(m_customAccent[0] * 0.5f, m_customAccent[1] * 0.5f, m_customAccent[2] * 0.5f);
            customColors.highlight = toRGBA(m_customAccent[0] * 0.7f, m_customAccent[1] * 0.7f, m_customAccent[2] * 0.7f);
            customColors.shadow = 0x000000FF;

            m_themeManager->setCustomTheme(customColors);
            m_themeManager->setTheme(WindowsTheme::Custom);
            m_selectedThemeIndex = static_cast<int>(WindowsTheme::Custom);

            LOG_INFO_C("Custom theme applied", "WindowsCustomizationPanel");
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset Colors", ImVec2(150, 35))) {
            m_customBackground[0] = 0.12f;
            m_customBackground[1] = 0.12f;
            m_customBackground[2] = 0.12f;
            
            m_customAccent[0] = 0.0f;
            m_customAccent[1] = 0.47f;
            m_customAccent[2] = 0.83f;
            
            m_customText[0] = 0.88f;
            m_customText[1] = 0.88f;
            m_customText[2] = 0.88f;
        }
    }
    ImGui::End();
#endif
}

} // namespace fresh

#endif // _WIN32
#endif // ImGui code disabled
