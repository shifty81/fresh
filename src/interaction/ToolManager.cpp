#include "interaction/ToolManager.h"
#include "voxel/VoxelWorld.h"
#include <iostream>

namespace fresh
{

ToolManager::ToolManager()
    : m_world(nullptr),
      m_activeTool(nullptr),
      m_activeToolType(VoxelToolType::ConstructionHammer)
{
}

ToolManager::~ToolManager() = default;

bool ToolManager::initialize(VoxelWorld* world)
{
    m_world = world;
    
    if (!m_world) {
        std::cerr << "ToolManager: Cannot initialize without valid world" << std::endl;
        return false;
    }

    // Create all tools
    createTools();

    // Select default tool (Construction Hammer)
    selectTool(VoxelToolType::ConstructionHammer);

    std::cout << "ToolManager initialized with " << m_tools.size() << " tools" << std::endl;
    return true;
}

void ToolManager::createTools()
{
    // Create Construction Hammer
    auto hammer = std::make_unique<ConstructionHammer>();
    hammer->initialize(m_world);
    m_tools[VoxelToolType::ConstructionHammer] = std::move(hammer);

    // Create Pickaxe
    auto pickaxe = std::make_unique<Pickaxe>();
    pickaxe->initialize(m_world);
    m_tools[VoxelToolType::Pickaxe] = std::move(pickaxe);

    // Create Rake
    auto rake = std::make_unique<Rake>();
    rake->initialize(m_world);
    m_tools[VoxelToolType::Rake] = std::move(rake);
}

bool ToolManager::selectTool(VoxelToolType type)
{
    auto it = m_tools.find(type);
    if (it == m_tools.end()) {
        std::cerr << "ToolManager: Tool type not found" << std::endl;
        return false;
    }

    m_activeTool = it->second.get();
    m_activeToolType = type;
    
    std::cout << "Selected tool: " << m_activeTool->getName() << std::endl;
    return true;
}

VoxelTool* ToolManager::getTool(VoxelToolType type) const
{
    auto it = m_tools.find(type);
    if (it != m_tools.end()) {
        return it->second.get();
    }
    return nullptr;
}

ConstructionHammer* ToolManager::getConstructionHammer() const
{
    return dynamic_cast<ConstructionHammer*>(getTool(VoxelToolType::ConstructionHammer));
}

Pickaxe* ToolManager::getPickaxe() const
{
    return dynamic_cast<Pickaxe*>(getTool(VoxelToolType::Pickaxe));
}

Rake* ToolManager::getRake() const
{
    return dynamic_cast<Rake*>(getTool(VoxelToolType::Rake));
}

bool ToolManager::useActiveTool(const WorldPos& pos, VoxelType voxelType)
{
    if (!m_activeTool) {
        std::cerr << "ToolManager: No active tool selected" << std::endl;
        return false;
    }

    return m_activeTool->useTool(pos, voxelType);
}

void ToolManager::cycleNextTool()
{
    // Cycle through tools: Hammer -> Pickaxe -> Rake -> Hammer
    switch (m_activeToolType) {
    case VoxelToolType::ConstructionHammer:
        selectTool(VoxelToolType::Pickaxe);
        break;
    case VoxelToolType::Pickaxe:
        selectTool(VoxelToolType::Rake);
        break;
    case VoxelToolType::Rake:
        selectTool(VoxelToolType::ConstructionHammer);
        break;
    }
}

void ToolManager::cyclePreviousTool()
{
    // Cycle backwards: Hammer -> Rake -> Pickaxe -> Hammer
    switch (m_activeToolType) {
    case VoxelToolType::ConstructionHammer:
        selectTool(VoxelToolType::Rake);
        break;
    case VoxelToolType::Rake:
        selectTool(VoxelToolType::Pickaxe);
        break;
    case VoxelToolType::Pickaxe:
        selectTool(VoxelToolType::ConstructionHammer);
        break;
    }
}

VoxelToolType ToolManager::getActiveToolType() const
{
    return m_activeToolType;
}

} // namespace fresh
