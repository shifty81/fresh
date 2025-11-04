#include "ui/EditorToolbar.h"
#include "core/Logger.h"
#include <imgui.h>

namespace fresh {

EditorToolbar::EditorToolbar()
    : m_activeTool(Tool::Select)
    , m_playMode(PlayMode::Stopped)
{
}

EditorToolbar::~EditorToolbar() {
}

bool EditorToolbar::initialize() {
    LOG_INFO_C("Editor Toolbar initialized", "EditorToolbar");
    return true;
}

void EditorToolbar::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                             ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoSavedSettings;
    
    // Position toolbar below menu bar
    const float menuBarHeight = ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 50));
    
    ImGui::Begin("Toolbar", nullptr, flags);
    
    // Transform tools
    ImGui::Text("Transform:");
    ImGui::SameLine();
    toolButton("Select", Tool::Select);
    ImGui::SameLine();
    toolButton("Move (W)", Tool::Move);
    ImGui::SameLine();
    toolButton("Rotate (E)", Tool::Rotate);
    ImGui::SameLine();
    toolButton("Scale (R)", Tool::Scale);
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Editing tools
    ImGui::Text("Edit:");
    ImGui::SameLine();
    toolButton("Place (B)", Tool::Place);
    ImGui::SameLine();
    toolButton("Remove (X)", Tool::Remove);
    ImGui::SameLine();
    toolButton("Paint (P)", Tool::Paint);
    
    // Spacer to push play buttons to the right
    ImGui::SameLine();
    float width = ImGui::GetWindowWidth();
    float buttonWidth = 200.0f;
    ImGui::SetCursorPosX(width - buttonWidth - 10);
    
    // Play mode buttons
    playModeButton("Play", PlayMode::Playing);
    ImGui::SameLine();
    playModeButton("Pause", PlayMode::Paused);
    ImGui::SameLine();
    playModeButton("Stop", PlayMode::Stopped);
    
    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

bool EditorToolbar::toolButton(const char* label, Tool tool) {
    bool isActive = (m_activeTool == tool);
    
    if (isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
    }
    
    bool clicked = ImGui::Button(label);
    
    if (isActive) {
        ImGui::PopStyleColor();
    }
    
    if (clicked) {
        m_activeTool = tool;
        if (m_toolCallback) {
            m_toolCallback(tool);
        }
    }
    
    return clicked;
}

bool EditorToolbar::playModeButton(const char* label, PlayMode mode) {
    bool isActive = (m_playMode == mode);
    
    if (isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
    }
    
    bool clicked = ImGui::Button(label);
    
    if (isActive) {
        ImGui::PopStyleColor();
    }
    
    if (clicked) {
        m_playMode = mode;
        if (m_playModeCallback) {
            m_playModeCallback(mode);
        }
    }
    
    return clicked;
}

} // namespace fresh
