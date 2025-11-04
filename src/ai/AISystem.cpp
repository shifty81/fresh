#include "ai/AISystem.h"
#include "ai/BehaviorTree.h"
#include "ecs/Entity.h"
#include "core/Logger.h"
#include <vector>
#include <memory>

namespace fresh {

/**
 * @brief Internal NPC data structure
 */
struct NPC {
    std::unique_ptr<Entity> entity;
    std::unique_ptr<BehaviorTree> behaviorTree;
    bool active = true;
};

/**
 * @brief Private implementation details
 */
class AISystemImpl {
public:
    std::vector<std::unique_ptr<NPC>> npcs;
};

AISystem::AISystem() : pImpl(new AISystemImpl()) {
}

AISystem::~AISystem() {
    delete pImpl;
}

bool AISystem::initialize() {
    LOG_INFO_C("AI System initialized", "AISystem");
    return true;
}

void AISystem::update(float deltaTime) {
    // Update all NPC behavior trees
    for (auto& npc : pImpl->npcs) {
        if (npc->active && npc->behaviorTree) {
            npc->behaviorTree->tick(npc->entity.get(), deltaTime);
        }
    }
}

void AISystem::addNPC(std::unique_ptr<Entity> entity, std::unique_ptr<BehaviorTree> behavior) {
    auto npc = std::make_unique<NPC>();
    npc->entity = std::move(entity);
    npc->behaviorTree = std::move(behavior);
    pImpl->npcs.push_back(std::move(npc));
}

void AISystem::removeNPC(Entity* entity) {
    pImpl->npcs.erase(
        std::remove_if(pImpl->npcs.begin(), pImpl->npcs.end(),
            [entity](const std::unique_ptr<NPC>& npc) {
                return npc->entity.get() == entity;
            }),
        pImpl->npcs.end()
    );
}

size_t AISystem::getNPCCount() const {
    return pImpl->npcs.size();
}

} // namespace fresh
