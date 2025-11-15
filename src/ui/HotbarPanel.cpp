#include "ui/HotbarPanel.h"

#include "core/Logger.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace fresh
{

HotbarPanel::HotbarPanel() : m_visible(false), m_selectedSlot(0)
{
    // Initialize all slots as empty
    clearAllSlots();
}

HotbarPanel::~HotbarPanel() {}

bool HotbarPanel::initialize()
{
    LOG_INFO_C("Hotbar Panel initialized", "HotbarPanel");
    return true;
}

void HotbarPanel::render()
{
    if (!m_visible) {
        return;
    }

    // Position hotbar at bottom middle of screen
    ImGuiIO& io = ImGui::GetIO();
    const float slotSize = 50.0f;
    const float spacing = 4.0f;
    const float totalWidth = (slotSize + spacing) * HOTBAR_SIZE - spacing;
    const float bottomPadding = 20.0f;
    
    ImVec2 windowPos((io.DisplaySize.x - totalWidth) * 0.5f, 
                     io.DisplaySize.y - slotSize - bottomPadding);
    
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(totalWidth, slotSize + 20.0f));
    
    // Window flags for a non-interactive overlay (can still see slot contents)
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoScrollbar | 
                            ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoBackground |
                            ImGuiWindowFlags_NoFocusOnAppearing |
                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Hotbar", nullptr, flags);

    // Render each slot
    for (int i = 0; i < HOTBAR_SIZE; ++i) {
        if (i > 0) {
            ImGui::SameLine();
        }
        
        renderSlot(i, m_slots[i], i == m_selectedSlot);
    }

    ImGui::End();
}

void HotbarPanel::renderSlot(int slotIndex, const HotbarSlot& slot, bool isSelected)
{
    const float slotSize = 50.0f;
    
    // Push style for slot background
    if (isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 1.0f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.7f, 1.0f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.5f, 0.9f, 1.0f));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    }
    
    ImGui::PushID(slotIndex);
    ImGui::Button("", ImVec2(slotSize, slotSize));
    ImGui::PopID();
    
    ImGui::PopStyleColor(3);
    
    // Draw slot number in top-left corner
    ImVec2 pos = ImGui::GetItemRectMin();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Slot number (1-9, 0 for last slot)
    char slotNumText[3];
    snprintf(slotNumText, sizeof(slotNumText), "%d", (slotIndex + 1) % 10);
    drawList->AddText(ImVec2(pos.x + 3, pos.y + 2), IM_COL32(255, 255, 255, 255), slotNumText);
    
    // Draw item if slot is not empty
    if (!slot.isEmpty) {
        // Draw item name or voxel type
        const char* itemName = slot.itemName.empty() ? getVoxelTypeName(slot.voxelType) : slot.itemName.c_str();
        
        // Center text in slot
        ImVec2 textSize = ImGui::CalcTextSize(itemName);
        ImVec2 textPos(pos.x + (slotSize - textSize.x) * 0.5f, 
                      pos.y + (slotSize - textSize.y) * 0.5f);
        drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), itemName);
        
        // Draw count if > 1
        if (slot.count > 1) {
            char countText[8];
            snprintf(countText, sizeof(countText), "x%d", slot.count);
            ImVec2 countSize = ImGui::CalcTextSize(countText);
            drawList->AddText(ImVec2(pos.x + slotSize - countSize.x - 3, 
                                    pos.y + slotSize - countSize.y - 2), 
                            IM_COL32(255, 255, 255, 255), countText);
        }
    }
}

void HotbarPanel::setSelectedSlot(int slotIndex)
{
    if (slotIndex >= 0 && slotIndex < HOTBAR_SIZE) {
        m_selectedSlot = slotIndex;
        
        // Trigger callback if set
        if (m_slotActivatedCallback) {
            m_slotActivatedCallback(slotIndex);
        }
    }
}

void HotbarPanel::setSlot(int slotIndex, const HotbarSlot& slot)
{
    if (slotIndex >= 0 && slotIndex < HOTBAR_SIZE) {
        m_slots[slotIndex] = slot;
    }
}

const HotbarPanel::HotbarSlot& HotbarPanel::getSlot(int slotIndex) const
{
    static HotbarSlot emptySlot;
    if (slotIndex >= 0 && slotIndex < HOTBAR_SIZE) {
        return m_slots[slotIndex];
    }
    return emptySlot;
}

void HotbarPanel::clearSlot(int slotIndex)
{
    if (slotIndex >= 0 && slotIndex < HOTBAR_SIZE) {
        m_slots[slotIndex] = HotbarSlot();
    }
}

void HotbarPanel::clearAllSlots()
{
    for (auto& slot : m_slots) {
        slot = HotbarSlot();
    }
}

void HotbarPanel::handleKeyPress(int key)
{
    // Map keys 1-9 to slots 0-8, and key 0 to slot 9
    int slotIndex = -1;
    
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
        slotIndex = key - GLFW_KEY_1;
    } else if (key == GLFW_KEY_0) {
        slotIndex = 9;
    }
    
    if (slotIndex >= 0) {
        setSelectedSlot(slotIndex);
    }
}

const char* HotbarPanel::getVoxelTypeName(VoxelType type) const
{
    switch (type) {
        case VoxelType::Stone: return "Stone";
        case VoxelType::Dirt: return "Dirt";
        case VoxelType::Grass: return "Grass";
        case VoxelType::Sand: return "Sand";
        case VoxelType::Water: return "Water";
        case VoxelType::Wood: return "Wood";
        case VoxelType::Leaves: return "Leaves";
        case VoxelType::Glass: return "Glass";
        case VoxelType::Brick: return "Brick";
        case VoxelType::Snow: return "Snow";
        case VoxelType::Ice: return "Ice";
        case VoxelType::Gravel: return "Gravel";
        case VoxelType::Coal: return "Coal";
        case VoxelType::Iron: return "Iron";
        default: return "Unknown";
    }
}

} // namespace fresh
