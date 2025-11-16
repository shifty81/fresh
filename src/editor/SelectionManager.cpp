#include "editor/SelectionManager.h"
#include "voxel/VoxelWorld.h"
#include "core/Logger.h"
#include <algorithm>
#include <cmath>

namespace fresh
{

SelectionManager::SelectionManager()
    : m_isSelecting(false)
    , m_selectionStart(0.0f)
    , m_selectionEnd(0.0f)
{
}

SelectionManager::~SelectionManager() = default;

void SelectionManager::startSelection(const glm::vec3& worldPos)
{
    m_selectionStart = worldPos;
    m_selectionEnd = worldPos;
    m_isSelecting = true;
    m_selection.clear();
    
    Logger::getInstance().info("Started voxel selection at (" + 
                                std::to_string(worldPos.x) + ", " + 
                                std::to_string(worldPos.y) + ", " + 
                                std::to_string(worldPos.z) + ")", "SelectionManager");
}

void SelectionManager::updateSelection(const glm::vec3& worldPos)
{
    if (!m_isSelecting) {
        return;
    }
    
    m_selectionEnd = worldPos;
}

void SelectionManager::finalizeSelection(VoxelWorld* world)
{
    if (!m_isSelecting) {
        return;
    }
    
    m_isSelecting = false;
    
    if (world != nullptr) {
        buildBoxSelection(world);
        calculateBounds();
        
        Logger::getInstance().info("Finalized selection: " + 
                                    std::to_string(m_selection.size()) + " voxels selected", "SelectionManager");
    }
}

void SelectionManager::clearSelection()
{
    m_isSelecting = false;
    m_selection.clear();
    m_selectionStart = glm::vec3(0.0f);
    m_selectionEnd = glm::vec3(0.0f);
    
    Logger::getInstance().info("Cleared voxel selection", "SelectionManager");
}

bool SelectionManager::hasSelection() const
{
    return !m_selection.isEmpty() || m_isSelecting;
}

size_t SelectionManager::getSelectionSize() const
{
    return m_selection.size();
}

const std::vector<VoxelPosition>& SelectionManager::getSelectedVoxels() const
{
    return m_selection.positions;
}

bool SelectionManager::getSelectionBounds(glm::ivec3& min, glm::ivec3& max) const
{
    if (m_isSelecting) {
        // Return bounds of the current dragging box
        min = glm::ivec3(
            std::floor(std::min(m_selectionStart.x, m_selectionEnd.x)),
            std::floor(std::min(m_selectionStart.y, m_selectionEnd.y)),
            std::floor(std::min(m_selectionStart.z, m_selectionEnd.z))
        );
        max = glm::ivec3(
            std::floor(std::max(m_selectionStart.x, m_selectionEnd.x)),
            std::floor(std::max(m_selectionStart.y, m_selectionEnd.y)),
            std::floor(std::max(m_selectionStart.z, m_selectionEnd.z))
        );
        return true;
    }
    else if (!m_selection.isEmpty()) {
        // Return bounds of finalized selection
        min = m_selection.boundsMin;
        max = m_selection.boundsMax;
        return true;
    }
    
    return false;
}

void SelectionManager::deleteSelected(VoxelWorld* world)
{
    if (world == nullptr || m_selection.isEmpty()) {
        return;
    }
    
    // Delete all selected voxels by setting them to Air
    for (const auto& pos : m_selection.positions) {
        world->setVoxel(pos.toWorldPos(), Voxel(VoxelType::Air));
    }
    
    Logger::getInstance().info("Deleted " + std::to_string(m_selection.size()) + " selected voxels", "SelectionManager");
    
    // Clear selection after deletion
    clearSelection();
}

void SelectionManager::moveSelection(const glm::ivec3& delta, VoxelWorld* world)
{
    if (world == nullptr || m_selection.isEmpty()) {
        return;
    }
    
    // Store the voxel types before moving
    std::vector<VoxelType> voxelTypes = m_selection.types;
    
    // First, clear the old positions
    for (const auto& pos : m_selection.positions) {
        world->setVoxel(pos.toWorldPos(), Voxel(VoxelType::Air));
    }
    
    // Then, place voxels at new positions
    for (size_t i = 0; i < m_selection.positions.size(); ++i) {
        VoxelPosition newPos(
            m_selection.positions[i].x + delta.x,
            m_selection.positions[i].y + delta.y,
            m_selection.positions[i].z + delta.z
        );
        world->setVoxel(newPos.toWorldPos(), Voxel(voxelTypes[i]));
        
        // Update selection position
        m_selection.positions[i] = newPos;
    }
    
    // Update bounds
    m_selection.boundsMin += delta;
    m_selection.boundsMax += delta;
    
    Logger::getInstance().info("Moved selection by (" + 
                                std::to_string(delta.x) + ", " + 
                                std::to_string(delta.y) + ", " + 
                                std::to_string(delta.z) + ")", "SelectionManager");
}

void SelectionManager::copyToClipboard(VoxelWorld* world)
{
    if (world == nullptr || m_selection.isEmpty()) {
        return;
    }
    
    // Copy selection to clipboard
    m_clipboard = m_selection;
    
    Logger::getInstance().info("Copied " + std::to_string(m_clipboard.size()) + " voxels to clipboard", "SelectionManager");
}

void SelectionManager::cutToClipboard(VoxelWorld* world)
{
    if (world == nullptr || m_selection.isEmpty()) {
        return;
    }
    
    // Copy to clipboard
    copyToClipboard(world);
    
    // Delete selected voxels
    for (const auto& pos : m_selection.positions) {
        world->setVoxel(pos.toWorldPos(), Voxel(VoxelType::Air));
    }
    
    Logger::getInstance().info("Cut " + std::to_string(m_clipboard.size()) + " voxels to clipboard", "SelectionManager");
    
    // Clear selection after cut
    clearSelection();
}

bool SelectionManager::hasClipboardData() const
{
    return !m_clipboard.isEmpty();
}

void SelectionManager::pasteFromClipboard(const glm::ivec3& pastePos, VoxelWorld* world)
{
    if (world == nullptr || m_clipboard.isEmpty()) {
        return;
    }
    
    // Calculate offset from original bounds to paste position
    glm::ivec3 offset = pastePos - m_clipboard.boundsMin;
    
    // Place all voxels from clipboard at new position
    for (size_t i = 0; i < m_clipboard.positions.size(); ++i) {
        VoxelPosition newPos(
            m_clipboard.positions[i].x + offset.x,
            m_clipboard.positions[i].y + offset.y,
            m_clipboard.positions[i].z + offset.z
        );
        world->setVoxel(newPos.toWorldPos(), Voxel(m_clipboard.types[i]));
    }
    
    Logger::getInstance().info("Pasted " + std::to_string(m_clipboard.size()) + 
                                " voxels at (" + 
                                std::to_string(pastePos.x) + ", " + 
                                std::to_string(pastePos.y) + ", " + 
                                std::to_string(pastePos.z) + ")", "SelectionManager");
}

void SelectionManager::buildBoxSelection(VoxelWorld* world)
{
    if (world == nullptr) {
        return;
    }
    
    m_selection.clear();
    
    // Calculate box bounds
    int minX = static_cast<int>(std::floor(std::min(m_selectionStart.x, m_selectionEnd.x)));
    int minY = static_cast<int>(std::floor(std::min(m_selectionStart.y, m_selectionEnd.y)));
    int minZ = static_cast<int>(std::floor(std::min(m_selectionStart.z, m_selectionEnd.z)));
    int maxX = static_cast<int>(std::floor(std::max(m_selectionStart.x, m_selectionEnd.x)));
    int maxY = static_cast<int>(std::floor(std::max(m_selectionStart.y, m_selectionEnd.y)));
    int maxZ = static_cast<int>(std::floor(std::max(m_selectionStart.z, m_selectionEnd.z)));
    
    // Iterate through all voxels in the box
    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            for (int z = minZ; z <= maxZ; ++z) {
                WorldPos pos(x, y, z);
                Voxel* voxel = world->getVoxel(pos);
                
                // Only select solid voxels (not air)
                if (voxel != nullptr && voxel->isSolid()) {
                    m_selection.positions.emplace_back(x, y, z);
                    m_selection.types.push_back(voxel->type);
                }
            }
        }
    }
}

void SelectionManager::calculateBounds()
{
    if (m_selection.isEmpty()) {
        m_selection.boundsMin = glm::ivec3(0);
        m_selection.boundsMax = glm::ivec3(0);
        return;
    }
    
    // Initialize with first position
    m_selection.boundsMin = glm::ivec3(
        m_selection.positions[0].x,
        m_selection.positions[0].y,
        m_selection.positions[0].z
    );
    m_selection.boundsMax = m_selection.boundsMin;
    
    // Find min/max across all positions
    for (const auto& pos : m_selection.positions) {
        m_selection.boundsMin.x = std::min(m_selection.boundsMin.x, pos.x);
        m_selection.boundsMin.y = std::min(m_selection.boundsMin.y, pos.y);
        m_selection.boundsMin.z = std::min(m_selection.boundsMin.z, pos.z);
        
        m_selection.boundsMax.x = std::max(m_selection.boundsMax.x, pos.x);
        m_selection.boundsMax.y = std::max(m_selection.boundsMax.y, pos.y);
        m_selection.boundsMax.z = std::max(m_selection.boundsMax.z, pos.z);
    }
}

} // namespace fresh
