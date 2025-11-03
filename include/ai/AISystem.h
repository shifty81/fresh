#pragma once

namespace fresh {

/**
 * @brief AI system for NPC behavior
 */
class AISystem {
public:
    AISystem();
    ~AISystem();

    /**
     * @brief Initialize AI system
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Update AI
     * @param deltaTime Time since last update
     */
    void update(float deltaTime);
};

} // namespace fresh
