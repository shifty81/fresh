#include "editor/EditorSettingsDialog.h"
#include "core/Logger.h"
#include <imgui.h>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fresh
{

EditorSettingsDialog::EditorSettingsDialog()
    : m_visible(false),
      m_initialized(false),
      m_settingsModified(false)
{
}

EditorSettingsDialog::~EditorSettingsDialog()
{
}

bool EditorSettingsDialog::initialize(const std::string& configPath)
{
    m_configPath = configPath;
    
    // Create configs directory if it doesn't exist
    std::filesystem::path path(configPath);
    std::filesystem::path dir = path.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        try {
            std::filesystem::create_directories(dir);
            LOG_INFO_C("Created configs directory: " + dir.string(), "EditorSettingsDialog");
        } catch (const std::exception& e) {
            LOG_ERROR_C("Failed to create configs directory: " + std::string(e.what()), "EditorSettingsDialog");
            return false;
        }
    }
    
    // Load settings from file or use defaults
    if (!loadSettings()) {
        LOG_WARN_C("Could not load editor settings, using defaults", "EditorSettingsDialog");
        m_settings = EditorSettings(); // Use default settings
    }
    
    m_tempSettings = m_settings;
    m_initialized = true;
    
    LOG_INFO_C("Editor settings dialog initialized", "EditorSettingsDialog");
    return true;
}

void EditorSettingsDialog::render()
{
    if (!m_visible || !m_initialized) {
        return;
    }
    
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Editor Settings", &m_visible, ImGuiWindowFlags_NoCollapse)) {
        // Create tabs for different settings categories
        if (ImGui::BeginTabBar("SettingsTabs")) {
            if (ImGui::BeginTabItem("General")) {
                renderAutoSaveSettings();
                ImGui::Separator();
                renderUISettings();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Grid & Snap")) {
                renderGridSettings();
                ImGui::Separator();
                renderSnapSettings();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Tools")) {
                renderToolSettings();
                ImGui::Separator();
                renderCameraSettings();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Performance")) {
                renderPerformanceSettings();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
        
        ImGui::Separator();
        
        // Bottom buttons
        ImGui::Spacing();
        
        // Check if settings were modified
        m_settingsModified = (m_tempSettings.autoSaveEnabled != m_settings.autoSaveEnabled ||
                              m_tempSettings.autoSaveIntervalMinutes != m_settings.autoSaveIntervalMinutes ||
                              m_tempSettings.showGrid != m_settings.showGrid ||
                              m_tempSettings.gridSize != m_settings.gridSize ||
                              m_tempSettings.gridSubdivisions != m_settings.gridSubdivisions ||
                              m_tempSettings.gridOpacity != m_settings.gridOpacity ||
                              m_tempSettings.snapToGrid != m_settings.snapToGrid ||
                              m_tempSettings.snapValue != m_settings.snapValue ||
                              m_tempSettings.snapRotation != m_settings.snapRotation ||
                              m_tempSettings.snapRotationDegrees != m_settings.snapRotationDegrees ||
                              m_tempSettings.uiScale != m_settings.uiScale ||
                              m_tempSettings.fontSize != m_settings.fontSize ||
                              m_tempSettings.showTooltips != m_settings.showTooltips ||
                              m_tempSettings.defaultBrushSize != m_settings.defaultBrushSize ||
                              m_tempSettings.defaultBrushShape != m_settings.defaultBrushShape ||
                              m_tempSettings.defaultCameraSpeed != m_settings.defaultCameraSpeed ||
                              m_tempSettings.defaultCameraFastSpeed != m_settings.defaultCameraFastSpeed ||
                              m_tempSettings.vsync != m_settings.vsync ||
                              m_tempSettings.targetFPS != m_settings.targetFPS);
        
        float buttonWidth = 100.0f;
        float spacing = 10.0f;
        float totalWidth = buttonWidth * 3 + spacing * 2;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
        
        if (ImGui::Button("Apply", ImVec2(buttonWidth, 0))) {
            applySettings();
        }
        
        ImGui::SameLine(0, spacing);
        if (ImGui::Button("OK", ImVec2(buttonWidth, 0))) {
            applySettings();
            m_visible = false;
        }
        
        ImGui::SameLine(0, spacing);
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
            m_tempSettings = m_settings; // Revert changes
            m_visible = false;
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Reset to Defaults")) {
            resetToDefaults();
        }
        
        if (m_settingsModified) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "* Settings have been modified");
        }
    }
    ImGui::End();
}

void EditorSettingsDialog::renderAutoSaveSettings()
{
    ImGui::SeparatorText("Auto-Save");
    
    ImGui::Checkbox("Enable Auto-Save", &m_tempSettings.autoSaveEnabled);
    
    if (m_tempSettings.autoSaveEnabled) {
        ImGui::SliderInt("Auto-Save Interval (minutes)", &m_tempSettings.autoSaveIntervalMinutes, 1, 30);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Automatically save the world every N minutes");
        }
    }
}

void EditorSettingsDialog::renderGridSettings()
{
    ImGui::SeparatorText("Grid Display");
    
    ImGui::Checkbox("Show Grid", &m_tempSettings.showGrid);
    
    if (m_tempSettings.showGrid) {
        ImGui::SliderFloat("Grid Size", &m_tempSettings.gridSize, 0.1f, 10.0f, "%.1f");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Size of each grid cell");
        }
        
        ImGui::SliderInt("Grid Subdivisions", &m_tempSettings.gridSubdivisions, 1, 20);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Number of subdivisions per grid cell");
        }
        
        ImGui::SliderFloat("Grid Opacity", &m_tempSettings.gridOpacity, 0.0f, 1.0f, "%.2f");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Transparency of the grid lines");
        }
    }
}

void EditorSettingsDialog::renderSnapSettings()
{
    ImGui::SeparatorText("Snap Settings");
    
    ImGui::Checkbox("Snap to Grid", &m_tempSettings.snapToGrid);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Snap object positions to grid when moving");
    }
    
    if (m_tempSettings.snapToGrid) {
        ImGui::SliderFloat("Snap Value", &m_tempSettings.snapValue, 0.1f, 10.0f, "%.1f");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Snap distance for position snapping");
        }
    }
    
    ImGui::Spacing();
    ImGui::Checkbox("Snap Rotation", &m_tempSettings.snapRotation);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Snap object rotations to fixed angles");
    }
    
    if (m_tempSettings.snapRotation) {
        ImGui::SliderFloat("Rotation Snap (degrees)", &m_tempSettings.snapRotationDegrees, 1.0f, 90.0f, "%.0f");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Snap angle for rotation snapping");
        }
    }
}

void EditorSettingsDialog::renderUISettings()
{
    ImGui::SeparatorText("UI Appearance");
    
    ImGui::SliderFloat("UI Scale", &m_tempSettings.uiScale, 0.5f, 2.0f, "%.1f");
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Scale of the entire UI (requires restart)");
    }
    
    ImGui::SliderInt("Font Size", &m_tempSettings.fontSize, 10, 24);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Size of UI text (requires restart)");
    }
    
    ImGui::Checkbox("Show Tooltips", &m_tempSettings.showTooltips);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Display helpful tooltips when hovering over UI elements");
    }
}

void EditorSettingsDialog::renderToolSettings()
{
    ImGui::SeparatorText("Default Tool Settings");
    
    ImGui::SliderInt("Default Brush Size", &m_tempSettings.defaultBrushSize, 1, 20);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Default size for terraforming brushes");
    }
    
    const char* brushShapes[] = {"Sphere", "Cube", "Cylinder"};
    ImGui::Combo("Default Brush Shape", &m_tempSettings.defaultBrushShape, brushShapes, 3);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Default shape for terraforming brushes");
    }
}

void EditorSettingsDialog::renderCameraSettings()
{
    ImGui::SeparatorText("Camera Settings");
    
    ImGui::SliderFloat("Camera Speed", &m_tempSettings.defaultCameraSpeed, 1.0f, 50.0f, "%.1f");
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Normal camera movement speed");
    }
    
    ImGui::SliderFloat("Fast Camera Speed", &m_tempSettings.defaultCameraFastSpeed, 10.0f, 100.0f, "%.1f");
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Camera movement speed when holding Shift");
    }
}

void EditorSettingsDialog::renderPerformanceSettings()
{
    ImGui::SeparatorText("Performance");
    
    ImGui::Checkbox("V-Sync", &m_tempSettings.vsync);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Synchronize frame rate with monitor refresh rate (requires restart)");
    }
    
    ImGui::SliderInt("Target FPS", &m_tempSettings.targetFPS, 30, 144);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Target frame rate (0 = unlimited)");
    }
}

void EditorSettingsDialog::setSettings(const EditorSettings& settings)
{
    m_settings = settings;
    m_tempSettings = settings;
    notifySettingsChanged();
}

void EditorSettingsDialog::applySettings()
{
    m_settings = m_tempSettings;
    saveSettings();
    notifySettingsChanged();
    LOG_INFO_C("Editor settings applied", "EditorSettingsDialog");
}

void EditorSettingsDialog::notifySettingsChanged()
{
    if (m_settingsChangedCallback) {
        m_settingsChangedCallback(m_settings);
    }
}

bool EditorSettingsDialog::saveSettings()
{
    std::ofstream file(m_configPath);
    if (!file.is_open()) {
        LOG_ERROR_C("Failed to open settings file for writing: " + m_configPath, "EditorSettingsDialog");
        return false;
    }
    
    file << "[AutoSave]" << std::endl;
    file << "enabled=" << (m_settings.autoSaveEnabled ? "1" : "0") << std::endl;
    file << "intervalMinutes=" << m_settings.autoSaveIntervalMinutes << std::endl;
    file << std::endl;
    
    file << "[Grid]" << std::endl;
    file << "show=" << (m_settings.showGrid ? "1" : "0") << std::endl;
    file << "size=" << m_settings.gridSize << std::endl;
    file << "subdivisions=" << m_settings.gridSubdivisions << std::endl;
    file << "opacity=" << m_settings.gridOpacity << std::endl;
    file << std::endl;
    
    file << "[Snap]" << std::endl;
    file << "snapToGrid=" << (m_settings.snapToGrid ? "1" : "0") << std::endl;
    file << "snapValue=" << m_settings.snapValue << std::endl;
    file << "snapRotation=" << (m_settings.snapRotation ? "1" : "0") << std::endl;
    file << "snapRotationDegrees=" << m_settings.snapRotationDegrees << std::endl;
    file << std::endl;
    
    file << "[UI]" << std::endl;
    file << "scale=" << m_settings.uiScale << std::endl;
    file << "fontSize=" << m_settings.fontSize << std::endl;
    file << "showTooltips=" << (m_settings.showTooltips ? "1" : "0") << std::endl;
    file << std::endl;
    
    file << "[Tools]" << std::endl;
    file << "defaultBrushSize=" << m_settings.defaultBrushSize << std::endl;
    file << "defaultBrushShape=" << m_settings.defaultBrushShape << std::endl;
    file << std::endl;
    
    file << "[Camera]" << std::endl;
    file << "defaultSpeed=" << m_settings.defaultCameraSpeed << std::endl;
    file << "defaultFastSpeed=" << m_tempSettings.defaultCameraFastSpeed << std::endl;
    file << std::endl;
    
    file << "[Performance]" << std::endl;
    file << "vsync=" << (m_settings.vsync ? "1" : "0") << std::endl;
    file << "targetFPS=" << m_settings.targetFPS << std::endl;
    
    file.close();
    LOG_INFO_C("Editor settings saved to: " + m_configPath, "EditorSettingsDialog");
    return true;
}

bool EditorSettingsDialog::loadSettings()
{
    std::ifstream file(m_configPath);
    if (!file.is_open()) {
        LOG_WARN_C("Settings file not found: " + m_configPath, "EditorSettingsDialog");
        return false;
    }
    
    std::string line;
    std::string currentSection;
    
    while (std::getline(file, line)) {
        // Remove whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Parse section headers
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // Parse key=value pairs
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            
            // Trim whitespace from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Apply settings based on section
            if (currentSection == "AutoSave") {
                if (key == "enabled") m_settings.autoSaveEnabled = (value == "1");
                else if (key == "intervalMinutes") m_settings.autoSaveIntervalMinutes = std::stoi(value);
            }
            else if (currentSection == "Grid") {
                if (key == "show") m_settings.showGrid = (value == "1");
                else if (key == "size") m_settings.gridSize = std::stof(value);
                else if (key == "subdivisions") m_settings.gridSubdivisions = std::stoi(value);
                else if (key == "opacity") m_settings.gridOpacity = std::stof(value);
            }
            else if (currentSection == "Snap") {
                if (key == "snapToGrid") m_settings.snapToGrid = (value == "1");
                else if (key == "snapValue") m_settings.snapValue = std::stof(value);
                else if (key == "snapRotation") m_settings.snapRotation = (value == "1");
                else if (key == "snapRotationDegrees") m_settings.snapRotationDegrees = std::stof(value);
            }
            else if (currentSection == "UI") {
                if (key == "scale") m_settings.uiScale = std::stof(value);
                else if (key == "fontSize") m_settings.fontSize = std::stoi(value);
                else if (key == "showTooltips") m_settings.showTooltips = (value == "1");
            }
            else if (currentSection == "Tools") {
                if (key == "defaultBrushSize") m_settings.defaultBrushSize = std::stoi(value);
                else if (key == "defaultBrushShape") m_settings.defaultBrushShape = std::stoi(value);
            }
            else if (currentSection == "Camera") {
                if (key == "defaultSpeed") m_settings.defaultCameraSpeed = std::stof(value);
                else if (key == "defaultFastSpeed") m_settings.defaultCameraFastSpeed = std::stof(value);
            }
            else if (currentSection == "Performance") {
                if (key == "vsync") m_settings.vsync = (value == "1");
                else if (key == "targetFPS") m_settings.targetFPS = std::stoi(value);
            }
        }
    }
    
    file.close();
    LOG_INFO_C("Editor settings loaded from: " + m_configPath, "EditorSettingsDialog");
    return true;
}

void EditorSettingsDialog::resetToDefaults()
{
    m_tempSettings = EditorSettings(); // Reset to default constructor values
    LOG_INFO_C("Editor settings reset to defaults", "EditorSettingsDialog");
}

} // namespace fresh
