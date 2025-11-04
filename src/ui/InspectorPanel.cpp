#include "ui/InspectorPanel.h"
#include "ui/SceneHierarchyPanel.h"
#include "core/Logger.h"
#include <imgui.h>

namespace fresh {

InspectorPanel::InspectorPanel()
    : m_visible(true)
    , m_inspectedNode(nullptr)
{
}

InspectorPanel::~InspectorPanel() {
}

bool InspectorPanel::initialize() {
    LOG_INFO_C("Inspector Panel initialized", "InspectorPanel");
    return true;
}

void InspectorPanel::render() {
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Inspector", &m_visible);

    if (!m_inspectedNode) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No object selected");
        ImGui::Text("Select an object in the Scene Hierarchy");
        ImGui::End();
        return;
    }

    // Object name
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
    ImGui::Text("Object: %s", m_inspectedNode->name.c_str());
    ImGui::PopStyleColor();
    ImGui::Separator();

    // Visibility checkbox
    ImGui::Checkbox("Visible", &m_inspectedNode->visible);
    ImGui::Separator();

    // Transform properties
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderTransformProperties();
    }

    // Component properties
    if (ImGui::CollapsingHeader("Components")) {
        renderComponentProperties();
    }

    // Material properties
    if (ImGui::CollapsingHeader("Material")) {
        renderMaterialProperties();
    }

    ImGui::End();
}

void InspectorPanel::renderTransformProperties() {
    // Position
    static float position[3] = {0.0f, 0.0f, 0.0f};
    ImGui::Text("Position");
    ImGui::DragFloat3("##pos", position, 0.1f);

    // Rotation
    static float rotation[3] = {0.0f, 0.0f, 0.0f};
    ImGui::Text("Rotation");
    ImGui::DragFloat3("##rot", rotation, 1.0f, -180.0f, 180.0f);

    // Scale
    static float scale[3] = {1.0f, 1.0f, 1.0f};
    ImGui::Text("Scale");
    ImGui::DragFloat3("##scale", scale, 0.1f, 0.01f, 100.0f);
}

void InspectorPanel::renderComponentProperties() {
    ImGui::Indent();
    
    // Physics component example
    if (ImGui::TreeNode("Physics Component")) {
        static bool useGravity = true;
        ImGui::Checkbox("Use Gravity", &useGravity);
        
        static float mass = 1.0f;
        ImGui::Text("Mass");
        ImGui::SliderFloat("##mass", &mass, 0.1f, 100.0f);
        
        static float friction = 0.5f;
        ImGui::Text("Friction");
        ImGui::SliderFloat("##friction", &friction, 0.0f, 1.0f);
        
        ImGui::TreePop();
    }
    
    // Renderer component example
    if (ImGui::TreeNode("Renderer Component")) {
        static bool castShadows = true;
        ImGui::Checkbox("Cast Shadows", &castShadows);
        
        static bool receiveShadows = true;
        ImGui::Checkbox("Receive Shadows", &receiveShadows);
        
        static int renderLayer = 0;
        ImGui::Text("Render Layer");
        ImGui::SliderInt("##layer", &renderLayer, 0, 31);
        
        ImGui::TreePop();
    }
    
    ImGui::Unindent();
    
    // Add component button
    ImGui::Spacing();
    if (ImGui::Button("+ Add Component")) {
        ImGui::OpenPopup("AddComponentPopup");
    }
    
    if (ImGui::BeginPopup("AddComponentPopup")) {
        ImGui::Text("Select Component Type:");
        ImGui::Separator();
        if (ImGui::Selectable("Physics Component")) {
            LOG_INFO_C("Adding Physics Component", "InspectorPanel");
        }
        if (ImGui::Selectable("Renderer Component")) {
            LOG_INFO_C("Adding Renderer Component", "InspectorPanel");
        }
        if (ImGui::Selectable("Audio Source")) {
            LOG_INFO_C("Adding Audio Source Component", "InspectorPanel");
        }
        if (ImGui::Selectable("Script Component")) {
            LOG_INFO_C("Adding Script Component", "InspectorPanel");
        }
        ImGui::EndPopup();
    }
}

void InspectorPanel::renderMaterialProperties() {
    ImGui::Indent();
    
    // Shader selection
    const char* shaders[] = { "Standard", "Unlit", "Water", "Transparent", "Custom" };
    static int currentShader = 0;
    ImGui::Text("Shader");
    ImGui::Combo("##shader", &currentShader, shaders, IM_ARRAYSIZE(shaders));
    
    // Color
    static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    ImGui::Text("Color");
    ImGui::ColorEdit4("##color", color);
    
    // Texture
    ImGui::Text("Texture");
    ImGui::Button("None (Texture)", ImVec2(200, 0));
    if (ImGui::BeginDragDropTarget()) {
        // Texture drag-drop handling would be implemented here
        // when asset drag-drop system is complete
        ImGui::EndDragDropTarget();
    }
    
    // Material properties
    static float metallic = 0.0f;
    ImGui::Text("Metallic");
    ImGui::SliderFloat("##metallic", &metallic, 0.0f, 1.0f);
    
    static float smoothness = 0.5f;
    ImGui::Text("Smoothness");
    ImGui::SliderFloat("##smoothness", &smoothness, 0.0f, 1.0f);
    
    static float emission = 0.0f;
    ImGui::Text("Emission");
    ImGui::SliderFloat("##emission", &emission, 0.0f, 5.0f);
    
    ImGui::Unindent();
}

} // namespace fresh
