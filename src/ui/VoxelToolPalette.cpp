#include "ui/VoxelToolPalette.h"
#include "core/Logger.h"
#include <imgui.h>

namespace fresh {

VoxelToolPalette::VoxelToolPalette()
    : m_visible(true)
    , m_terraformingSystem(nullptr)
    , m_currentTool(TerraformTool::SingleBlock)
    , m_currentVoxelType(VoxelType::Stone)
    , m_toolSize(3)
{
}

VoxelToolPalette::~VoxelToolPalette() {
}

bool VoxelToolPalette::initialize(TerraformingSystem* terraformingSystem) {
    if (!terraformingSystem) {
        LOG_ERROR_C("Invalid terraforming system", "VoxelToolPalette");
        return false;
    }

    m_terraformingSystem = terraformingSystem;
    
    LOG_INFO_C("Voxel Tool Palette initialized", "VoxelToolPalette");
    return true;
}

void VoxelToolPalette::render() {
    if (!m_visible) {
        return;
    }

    ImGui::Begin("Voxel Tools", &m_visible);

    // Tool selector
    if (ImGui::CollapsingHeader("Tools", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderToolSelector();
    }

    ImGui::Separator();

    // Material picker
    if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderMaterialPicker();
    }

    ImGui::Separator();

    // Tool settings
    if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        renderToolSettings();
    }

    ImGui::Separator();

    // Quick actions
    if (ImGui::CollapsingHeader("Quick Actions")) {
        renderQuickActions();
    }

    ImGui::End();
}

void VoxelToolPalette::renderToolSelector() {
    ImGui::Text("Editing Tools:");
    
    // Basic tools
    toolButton("Single Block", TerraformTool::SingleBlock);
    toolButton("Brush", TerraformTool::Brush);
    toolButton("Sphere", TerraformTool::Sphere);
    toolButton("Filled Sphere", TerraformTool::FilledSphere);
    
    ImGui::Separator();
    
    // Shape tools
    ImGui::Text("Shape Tools:");
    toolButton("Cube", TerraformTool::Cube);
    toolButton("Filled Cube", TerraformTool::FilledCube);
    toolButton("Line", TerraformTool::Line);
    
    ImGui::Separator();
    
    // Terrain tools
    ImGui::Text("Terrain Tools:");
    toolButton("Flatten", TerraformTool::Flatten);
    toolButton("Smooth", TerraformTool::Smooth);
    toolButton("Paint", TerraformTool::Paint);
}

void VoxelToolPalette::renderMaterialPicker() {
    ImGui::Text("Block Types:");
    
    // Basic materials
    voxelTypeButton("Air", VoxelType::Air);
    ImGui::SameLine();
    voxelTypeButton("Stone", VoxelType::Stone);
    
    voxelTypeButton("Dirt", VoxelType::Dirt);
    ImGui::SameLine();
    voxelTypeButton("Grass", VoxelType::Grass);
    
    voxelTypeButton("Sand", VoxelType::Sand);
    ImGui::SameLine();
    voxelTypeButton("Water", VoxelType::Water);
    
    ImGui::Separator();
    
    // Natural materials
    ImGui::Text("Natural Materials:");
    voxelTypeButton("Wood", VoxelType::Wood);
    ImGui::SameLine();
    voxelTypeButton("Leaves", VoxelType::Leaves);
    
    voxelTypeButton("Snow", VoxelType::Snow);
    ImGui::SameLine();
    voxelTypeButton("Ice", VoxelType::Ice);
    
    ImGui::Separator();
    
    // Crafted materials
    ImGui::Text("Crafted Materials:");
    voxelTypeButton("Cobblestone", VoxelType::Cobblestone);
    ImGui::SameLine();
    voxelTypeButton("Planks", VoxelType::Planks);
    
    voxelTypeButton("Glass", VoxelType::Glass);
}

void VoxelToolPalette::renderToolSettings() {
    // Tool size slider
    ImGui::Text("Tool Size:");
    int oldSize = m_toolSize;
    ImGui::SliderInt("##size", &m_toolSize, 1, 10);
    
    if (m_toolSize != oldSize && m_terraformingSystem) {
        m_terraformingSystem->setToolSize(m_toolSize);
        if (m_toolSizeCallback) {
            m_toolSizeCallback(m_toolSize);
        }
    }
    
    // Brush strength (for smooth/paint tools)
    static float strength = 1.0f;
    ImGui::Text("Strength:");
    ImGui::SliderFloat("##strength", &strength, 0.1f, 2.0f);
    
    // Noise settings for procedural tools
    if (ImGui::TreeNode("Procedural Settings")) {
        static float noiseScale = 0.1f;
        ImGui::Text("Noise Scale:");
        ImGui::SliderFloat("##noisescale", &noiseScale, 0.01f, 1.0f);
        
        static float noiseStrength = 1.0f;
        ImGui::Text("Noise Strength:");
        ImGui::SliderFloat("##noisestrength", &noiseStrength, 0.0f, 10.0f);
        
        static int seed = 12345;
        ImGui::Text("Seed:");
        ImGui::InputInt("##seed", &seed);
        
        ImGui::TreePop();
    }
}

void VoxelToolPalette::renderQuickActions() {
    if (ImGui::Button("Undo (Ctrl+Z)", ImVec2(-1, 0))) {
        if (m_terraformingSystem) {
            m_terraformingSystem->undo();
        }
    }
    
    if (ImGui::Button("Redo (Ctrl+Y)", ImVec2(-1, 0))) {
        if (m_terraformingSystem) {
            m_terraformingSystem->redo();
        }
    }
    
    ImGui::Separator();
    
    if (ImGui::Button("Generate Terrain", ImVec2(-1, 0))) {
        LOG_INFO_C("Terrain generation dialog would open here", "VoxelToolPalette");
    }
    
    if (ImGui::Button("Clear Selection", ImVec2(-1, 0))) {
        LOG_INFO_C("Clearing current selection", "VoxelToolPalette");
        // Selection clearing would be implemented when selection system is added
    }
}

bool VoxelToolPalette::toolButton(const char* label, TerraformTool tool) {
    bool isActive = (m_currentTool == tool);
    
    if (isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
    }
    
    bool clicked = ImGui::Button(label, ImVec2(-1, 0));
    
    if (isActive) {
        ImGui::PopStyleColor();
    }
    
    if (clicked) {
        m_currentTool = tool;
        if (m_terraformingSystem) {
            m_terraformingSystem->setTool(tool);
        }
        if (m_toolChangeCallback) {
            m_toolChangeCallback(tool);
        }
    }
    
    return clicked;
}

bool VoxelToolPalette::voxelTypeButton(const char* label, VoxelType type) {
    bool isActive = (m_currentVoxelType == type);
    
    if (isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.8f, 0.4f, 1.0f));
    }
    
    // Color coding for different voxel types
    ImVec4 buttonColor;
    switch (type) {
        case VoxelType::Stone: buttonColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); break;
        case VoxelType::Dirt: buttonColor = ImVec4(0.6f, 0.4f, 0.2f, 1.0f); break;
        case VoxelType::Grass: buttonColor = ImVec4(0.3f, 0.7f, 0.3f, 1.0f); break;
        case VoxelType::Sand: buttonColor = ImVec4(0.9f, 0.9f, 0.6f, 1.0f); break;
        case VoxelType::Water: buttonColor = ImVec4(0.2f, 0.5f, 0.8f, 1.0f); break;
        case VoxelType::Wood: buttonColor = ImVec4(0.6f, 0.4f, 0.2f, 1.0f); break;
        case VoxelType::Leaves: buttonColor = ImVec4(0.2f, 0.6f, 0.2f, 1.0f); break;
        default: buttonColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); break;
    }
    
    if (!isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    }
    
    bool clicked = ImGui::Button(label, ImVec2(120, 30));
    
    if (!isActive) {
        ImGui::PopStyleColor();
    }
    
    if (isActive) {
        ImGui::PopStyleColor();
    }
    
    if (clicked) {
        m_currentVoxelType = type;
        if (m_terraformingSystem) {
            m_terraformingSystem->setVoxelType(type);
        }
        if (m_voxelTypeCallback) {
            m_voxelTypeCallback(type);
        }
    }
    
    return clicked;
}

} // namespace fresh
