#include "ui/HotbarPanel.h"

#include "core/Logger.h"

#include <GLFW/glfw3.h>

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

    // TODO: Implement with Windows native UI (ImGui has been removed from this project)
    // Use Win32HUD or similar native UI component instead
}

void HotbarPanel::renderSlot(int slotIndex, const HotbarSlot& slot, bool isSelected)
{
    // Stub - ImGui UI not available
    (void)slotIndex;
    (void)slot;
    (void)isSelected;
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
