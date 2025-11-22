// DEPRECATED: This file contains ImGui code and is no longer used.
// Fresh Voxel Engine uses Windows Native Win32 UI exclusively.
// See WINDOWS_NATIVE_ONLY_POLICY.md

#if 0 // ImGui code disabled - not compiled
#include "ui/MainMenuPanel.h"

#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>

#include "core/Logger.h"

namespace fs = std::filesystem;

namespace fresh
{

MainMenuPanel::MainMenuPanel()
    : m_menuActive(true),
      m_createNewWorld(false),
      m_loadWorld(false),
      m_exitRequested(false),
      m_showNewWorldDialog(false),
      m_showLoadWorldDialog(false),
      m_isWorld3D(true), // Default to 3D world
      m_2dGameStyle(0),  // Default to Platformer/Terraria style
      m_worldSeed(0),
      m_selectedWorldIndex(0)
{
    std::memset(m_worldNameBuffer, 0, sizeof(m_worldNameBuffer));
    std::memset(m_seedBuffer, 0, sizeof(m_seedBuffer));

    // Default world name
#ifdef _WIN32
    strncpy_s(m_worldNameBuffer, sizeof(m_worldNameBuffer), "New World", _TRUNCATE);
#else
    std::strncpy(m_worldNameBuffer, "New World", sizeof(m_worldNameBuffer) - 1);
#endif
    m_worldNameBuffer[sizeof(m_worldNameBuffer) - 1] = '\0';

    // Random seed by default
    m_worldSeed = static_cast<int>(std::time(nullptr));
    std::snprintf(m_seedBuffer, sizeof(m_seedBuffer), "%d", m_worldSeed);
}

MainMenuPanel::~MainMenuPanel() {}

bool MainMenuPanel::initialize()
{
    LOG_INFO_C("Initializing Main Menu Panel", "MainMenuPanel");
    scanWorldSaves();
    return true;
}

void MainMenuPanel::render()
{
    if (!m_menuActive) {
        return;
    }

    // Center the window on screen and update position on resize
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Appearing);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Welcome to Fresh Voxel Engine", nullptr, window_flags)) {
        renderWorldSelection();
    }
    ImGui::End();

    // Open modal dialogs when requested
    if (m_showNewWorldDialog) {
        ImGui::OpenPopup("Create New World");
    }
    
    if (m_showLoadWorldDialog) {
        ImGui::OpenPopup("Load World");
    }

    // Render modal dialogs
    renderNewWorldDialog();
    renderLoadWorldDialog();
#endif
}

void MainMenuPanel::renderWorldSelection()
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Title
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Use default font
    const char* title = "FRESH VOXEL ENGINE";
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", title);
    ImGui::PopFont();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Center the buttons
    float buttonWidth = 300.0f;
    float buttonHeight = 50.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    // Create New World button
    if (ImGui::Button("Create New World", ImVec2(buttonWidth, buttonHeight))) {
        m_showNewWorldDialog = true;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    // Load World button
    if (ImGui::Button("Load Existing World", ImVec2(buttonWidth, buttonHeight))) {
        scanWorldSaves(); // Refresh list
        m_showLoadWorldDialog = true;
    }

    ImGui::Spacing();
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    // Exit button
    if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
        m_exitRequested = true;
        LOG_INFO_C("Exit requested from main menu", "MainMenuPanel");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Version info at bottom
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 30);
    const char* version = "Version 0.1.0 - C++20 Voxel Engine";
    textWidth = ImGui::CalcTextSize(version).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextDisabled("%s", version);
#endif
}

void MainMenuPanel::renderNewWorldDialog()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(550, 450), ImGuiCond_Appearing);

    // Use modal popup for proper background dimming and click blocking
    if (ImGui::BeginPopupModal("Create New World", &m_showNewWorldDialog, 
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Spacing();

        // World name input
        ImGui::Text("World Name:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##WorldName", m_worldNameBuffer, sizeof(m_worldNameBuffer));

        ImGui::Spacing();

        // World seed input
        ImGui::Text("World Seed (optional):");
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputText("##WorldSeed", m_seedBuffer, sizeof(m_seedBuffer),
                             ImGuiInputTextFlags_CharsDecimal)) {
            // Parse seed when user types
            if (std::strlen(m_seedBuffer) > 0) {
                m_worldSeed = std::atoi(m_seedBuffer);
            }
        }

        ImGui::Spacing();
        ImGui::TextDisabled("Leave seed empty for random generation");

        ImGui::Spacing();
        
        // World Type selection (2D or 3D)
        ImGui::Text("World Type:");
        int worldType = m_isWorld3D ? 1 : 0;
        ImGui::RadioButton("3D World (Full Voxel)", &worldType, 1);
        ImGui::RadioButton("2D World", &worldType, 0);
        m_isWorld3D = (worldType == 1);
        
        ImGui::Spacing();
        
        // Show 2D game style selection only when 2D is selected
        if (!m_isWorld3D) {
            ImGui::Indent(20.0f);
            ImGui::Text("2D Game Style:");
            ImGui::RadioButton("Platformer (Terraria-style side-scrolling)", &m_2dGameStyle, 0);
            ImGui::RadioButton("Top-down (Zelda-style exploration)", &m_2dGameStyle, 1);
            ImGui::Unindent(20.0f);
            
            ImGui::Spacing();
            if (m_2dGameStyle == 0) {
                ImGui::TextDisabled("Side-scrolling platformer world with caves and terrain");
            } else {
                ImGui::TextDisabled("Top-down exploration world with dungeons and obstacles");
            }
        } else {
            ImGui::TextDisabled("Full 3D voxel world with height and depth");
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Buttons
        float buttonWidth = 150.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = buttonWidth * 2 + spacing;
        float startX = (ImGui::GetWindowSize().x - totalWidth) * 0.5f;

        ImGui::SetCursorPosX(startX);
        if (ImGui::Button("Create", ImVec2(buttonWidth, 40))) {
            m_newWorldName = m_worldNameBuffer;
            
            // Use seed from buffer if provided, otherwise use timestamp
            if (std::strlen(m_seedBuffer) > 0) {
                m_worldSeed = std::atoi(m_seedBuffer);
            } else {
                m_worldSeed = static_cast<int>(std::time(nullptr));
            }

            m_createNewWorld = true;
            m_showNewWorldDialog = false;
            m_menuActive = false; // Hide menu after world creation
            
            std::string worldTypeStr = m_isWorld3D ? "3D" : 
                                      (m_2dGameStyle == 0 ? "2D Platformer" : "2D Top-down");
            LOG_INFO_C("Creating new " + worldTypeStr + " world: " + m_newWorldName + 
                       " with seed: " + std::to_string(m_worldSeed), "MainMenuPanel");
            
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 40))) {
            m_showNewWorldDialog = false;
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
#endif
}

void MainMenuPanel::renderLoadWorldDialog()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Appearing);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Load World", &m_showLoadWorldDialog, window_flags)) {
        ImGui::Spacing();

        if (m_worldSaves.empty()) {
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "No saved worlds found.");
            ImGui::Spacing();
            ImGui::TextDisabled("Create a new world to get started!");
        } else {
            ImGui::Text("Select a world to load:");
            ImGui::Spacing();

            // List of saved worlds
            ImGui::BeginChild("WorldList", ImVec2(0, 250), true);
            for (size_t i = 0; i < m_worldSaves.size(); i++) {
                bool isSelected = (m_selectedWorldIndex == static_cast<int>(i));
                if (ImGui::Selectable(m_worldSaves[i].c_str(), isSelected, 0, ImVec2(0, 30))) {
                    m_selectedWorldIndex = static_cast<int>(i);
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndChild();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Buttons
        float buttonWidth = 150.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = buttonWidth * 2 + spacing;
        float startX = (ImGui::GetWindowSize().x - totalWidth) * 0.5f;

        ImGui::SetCursorPosX(startX);
        
        // Disable load button if no worlds available
        if (m_worldSaves.empty()) {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Load", ImVec2(buttonWidth, 40))) {
            if (!m_worldSaves.empty() && m_selectedWorldIndex >= 0 &&
                m_selectedWorldIndex < static_cast<int>(m_worldSaves.size())) {
                m_loadWorldName = m_worldSaves[m_selectedWorldIndex];
                m_loadWorld = true;
                m_showLoadWorldDialog = false;
                m_menuActive = false; // Hide menu after loading world
                LOG_INFO_C("Loading world: " + m_loadWorldName, "MainMenuPanel");
            }
        }

        if (m_worldSaves.empty()) {
            ImGui::EndDisabled();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 40))) {
            m_showLoadWorldDialog = false;
        }
    }
    ImGui::End();
#endif
}

void MainMenuPanel::clearFlags()
{
    m_createNewWorld = false;
    m_loadWorld = false;
    m_exitRequested = false;
}

void MainMenuPanel::scanWorldSaves()
{
    m_worldSaves.clear();

    // Check if saves directory exists
    const std::string savesDir = "saves";

    if (!fs::exists(savesDir)) {
        LOG_INFO_C("Saves directory does not exist, creating it", "MainMenuPanel");
        try {
            fs::create_directory(savesDir);
        } catch (const std::exception& e) {
            LOG_ERROR_C("Failed to create saves directory: " + std::string(e.what()),
                        "MainMenuPanel");
            return;
        }
    }

    // Scan for .world files
    try {
        for (const auto& entry : fs::directory_iterator(savesDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".world") {
                m_worldSaves.push_back(entry.path().stem().string());
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR_C("Failed to scan saves directory: " + std::string(e.what()), "MainMenuPanel");
    }

    LOG_INFO_C("Found " + std::to_string(m_worldSaves.size()) + " saved worlds", "MainMenuPanel");
}

} // namespace fresh
#endif // ImGui code disabled
