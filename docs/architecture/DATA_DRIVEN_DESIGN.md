# Data-Driven Design Guide

## Overview

Fresh Voxel Engine uses **Data-Driven Design** where game logic, parameters, and configurations are stored in external data files (JSON, XML, etc.) rather than being hardcoded. This approach enables:

- **Rapid Iteration**: Change game balance without recompiling
- **Designer-Friendly**: Non-programmers can tweak gameplay
- **A/B Testing**: Easy to test different configurations
- **Modding Support**: Players can modify game data
- **Configuration Management**: Different builds with different settings

## Configuration Files

### Character Stats

**Location**: `config/character_stats.json`

Define character types with health, speed, and other attributes:

```json
{
  "character_types": [
    {
      "id": "player",
      "name": "Player Character",
      "health": 100.0,
      "maxSpeed": 5.0,
      "acceleration": 10.0,
      "jumpHeight": 2.0,
      "armor": 0.0
    },
    {
      "id": "warrior",
      "name": "Warrior",
      "health": 150.0,
      "maxSpeed": 4.5,
      "acceleration": 8.0,
      "jumpHeight": 1.8,
      "armor": 5.0
    }
  ]
}
```

**Benefits**:
- Balance character stats without code changes
- Create new character types by copying/modifying JSON
- Test different stat combinations quickly
- Share balance changes via configuration file

### Weapon Stats

**Location**: `config/weapon_stats.json`

Define weapons with damage, speed, and special effects:

```json
{
  "weapons": [
    {
      "id": "sword_iron",
      "name": "Iron Sword",
      "damage": 10.0,
      "attackSpeed": 1.2,
      "range": 2.0,
      "durability": 100,
      "special_effects": []
    },
    {
      "id": "staff_fire",
      "name": "Fire Staff",
      "damage": 18.0,
      "attackSpeed": 0.6,
      "range": 10.0,
      "special_effects": ["fire_damage", "area_of_effect"]
    }
  ],
  "special_effects": {
    "fire_damage": {
      "description": "Deals additional fire damage over time",
      "dotDamage": 2.0,
      "dotDuration": 5.0
    }
  }
}
```

**Benefits**:
- Adjust weapon balance without rebuilding
- Add new weapons by editing JSON
- Experiment with different special effect combinations
- Community can create weapon packs

### Voxel Materials

**Location**: `asset_packs/example_pack/voxel_materials.json`

Define block types with textures and properties:

```json
{
  "materials": [
    {
      "voxelType": "Stone",
      "name": "Stone Block",
      "textures": {
        "all": "textures/stone.png"
      },
      "materialProperties": {
        "metallic": 0.0,
        "roughness": 0.9,
        "ao": 1.0,
        "alpha": 1.0
      }
    }
  ]
}
```

**Benefits**:
- Add new block types without code
- Adjust visual properties (roughness, metallic)
- Create texture packs
- Support user-created content

### Difficulty Modifiers

Built into `character_stats.json`:

```json
{
  "difficulty_modifiers": {
    "easy": {
      "playerHealthMultiplier": 1.5,
      "enemyHealthMultiplier": 0.75,
      "enemyDamageMultiplier": 0.5
    },
    "hard": {
      "playerHealthMultiplier": 0.75,
      "enemyHealthMultiplier": 1.5,
      "enemyDamageMultiplier": 1.5
    }
  }
}
```

**Benefits**:
- Quickly test different difficulty levels
- Fine-tune game balance for different player types
- Create custom difficulty modes

## Using Data-Driven Configs in Code

### Loading Character Stats

```cpp
#include <fstream>
#include <nlohmann/json.hpp>

class CharacterStatsLoader
{
public:
    struct CharacterStats {
        std::string id;
        std::string name;
        float health;
        float maxSpeed;
        float acceleration;
        float jumpHeight;
        float armor;
    };

    static std::map<std::string, CharacterStats> loadStats(const std::string& filepath)
    {
        std::map<std::string, CharacterStats> stats;
        
        std::ifstream file(filepath);
        nlohmann::json config = nlohmann::json::parse(file);
        
        for (const auto& charData : config["character_types"]) {
            CharacterStats character;
            character.id = charData["id"];
            character.name = charData["name"];
            character.health = charData["health"];
            character.maxSpeed = charData["maxSpeed"];
            character.acceleration = charData["acceleration"];
            character.jumpHeight = charData["jumpHeight"];
            character.armor = charData["armor"];
            
            stats[character.id] = character;
        }
        
        return stats;
    }
};
```

### Creating Entities from Config

```cpp
Entity createCharacterFromConfig(const std::string& characterID, EntityManager* entityManager)
{
    // Load stats once (cache this in production)
    static auto characterStats = CharacterStatsLoader::loadStats("config/character_stats.json");
    
    auto it = characterStats.find(characterID);
    if (it == characterStats.end()) {
        throw std::runtime_error("Unknown character ID: " + characterID);
    }
    
    const auto& stats = it->second;
    
    // Create entity with components based on config
    Entity entity = entityManager->createEntity();
    entityManager->addComponent(entity, new HealthComponent(stats.health));
    entityManager->addComponent(entity, new MovementComponent(stats.maxSpeed, stats.acceleration));
    entityManager->addComponent(entity, new ArmorComponent(stats.armor));
    entityManager->addComponent(entity, new JumpComponent(stats.jumpHeight));
    
    return entity;
}

// Usage
Entity player = createCharacterFromConfig("player", entityManager);
Entity warrior = createCharacterFromConfig("warrior", entityManager);
Entity zombie = createCharacterFromConfig("enemy_zombie", entityManager);
```

### Applying Difficulty Modifiers

```cpp
class DifficultySystem
{
public:
    enum class Difficulty {
        Easy,
        Normal,
        Hard,
        Extreme
    };
    
    struct DifficultyModifiers {
        float playerHealthMultiplier;
        float enemyHealthMultiplier;
        float enemyDamageMultiplier;
    };
    
    static DifficultyModifiers loadModifiers(Difficulty difficulty)
    {
        std::ifstream file("config/character_stats.json");
        nlohmann::json config = nlohmann::json::parse(file);
        
        std::string difficultyKey;
        switch (difficulty) {
            case Difficulty::Easy: difficultyKey = "easy"; break;
            case Difficulty::Normal: difficultyKey = "normal"; break;
            case Difficulty::Hard: difficultyKey = "hard"; break;
            case Difficulty::Extreme: difficultyKey = "extreme"; break;
        }
        
        const auto& mods = config["difficulty_modifiers"][difficultyKey];
        
        return {
            mods["playerHealthMultiplier"],
            mods["enemyHealthMultiplier"],
            mods["enemyDamageMultiplier"]
        };
    }
    
    static void applyDifficulty(Entity entity, const DifficultyModifiers& modifiers, bool isPlayer)
    {
        auto* health = entityManager->getComponent<HealthComponent>(entity);
        if (health) {
            float multiplier = isPlayer ? modifiers.playerHealthMultiplier 
                                       : modifiers.enemyHealthMultiplier;
            health->setMaxHealth(health->getMaxHealth() * multiplier);
        }
    }
};

// Usage
auto modifiers = DifficultySystem::loadModifiers(DifficultySystem::Difficulty::Hard);
DifficultySystem::applyDifficulty(player, modifiers, true);   // Player gets 0.75x health
DifficultySystem::applyDifficulty(enemy, modifiers, false);   // Enemy gets 1.5x health
```

## Data-Driven Workflow

### 1. Initial Setup (Programmer)

Create config file structure:

```json
{
  "enemies": [
    {
      "id": "zombie",
      "health": 50.0,
      "speed": 2.0,
      "damage": 10.0
    }
  ]
}
```

Create loader code:

```cpp
class EnemyConfig {
public:
    static std::map<std::string, EnemyData> loadConfigs();
};
```

### 2. Game Designer Iteration (No Code)

Game designer opens `config/enemies.json` and modifies:

```json
{
  "enemies": [
    {
      "id": "zombie",
      "health": 40.0,      // Reduced from 50
      "speed": 2.5,        // Increased from 2.0
      "damage": 8.0        // Reduced from 10.0
    }
  ]
}
```

Save file, restart game - new values take effect immediately!

### 3. Playtesting & Feedback

- Tester: "Zombies are too tanky"
- Designer adjusts `health` in JSON
- Test again
- Repeat until fun

No waiting for code compilation or programmer availability!

### 4. A/B Testing

Create multiple config versions:

```
config/
  enemies_v1.json  // Current balance
  enemies_v2.json  // More aggressive enemies
  enemies_v3.json  // Faster but weaker enemies
```

Switch between versions to test which is more fun.

## Benefits for Development

### Rapid Balancing

**Without Data-Driven Design:**
```cpp
// Hardcoded in C++
class Zombie {
    const float health = 50.0f;  // Need to change code
    const float speed = 2.0f;    // Recompile
    const float damage = 10.0f;  // Rebuild
};

// Change cycle: Edit code → Compile (30s-5min) → Test → Repeat
```

**With Data-Driven Design:**
```json
// In JSON file
{
  "health": 50.0,
  "speed": 2.0,
  "damage": 10.0
}

// Change cycle: Edit JSON → Test → Repeat (no compile!)
```

### Designer Empowerment

Non-programmers can:
- Adjust game balance
- Create new content
- Test variations
- Fix balance issues

Without touching code or waiting for programmers.

### Version Control

Easy to track balance changes:

```diff
{
  "id": "zombie",
- "health": 50.0,
+ "health": 40.0,
  "speed": 2.0,
- "damage": 10.0
+ "damage": 8.0
}
```

Clear what changed, who changed it, and why (commit message).

### Modding Support

Players can create mods by editing config files:

```
mods/
  hardcore_mode/
    config/
      character_stats.json  // Half health for everyone
      weapon_stats.json     // Double damage for all weapons
```

Load mod configs to override defaults.

## Advanced Patterns

### Hot Reloading

Watch config files for changes and reload:

```cpp
class ConfigWatcher
{
public:
    void update()
    {
        if (fileChanged("config/character_stats.json")) {
            reloadCharacterStats();
            
            // Update all existing entities with new stats
            updateExistingEntities();
            
            std::cout << "Character stats reloaded!" << std::endl;
        }
    }
};
```

Change JSON file while game is running - sees update immediately!

### Config Validation

Validate configs on load to catch errors early:

```cpp
class ConfigValidator
{
public:
    static bool validateCharacterStats(const nlohmann::json& config)
    {
        for (const auto& character : config["character_types"]) {
            // Check required fields exist
            if (!character.contains("id") || !character.contains("health")) {
                std::cerr << "Invalid character config: missing required field" << std::endl;
                return false;
            }
            
            // Check value ranges
            if (character["health"] <= 0) {
                std::cerr << "Invalid character config: health must be positive" << std::endl;
                return false;
            }
            
            // More validation...
        }
        
        return true;
    }
};
```

### Config Inheritance

Support inheritance to reduce duplication:

```json
{
  "base_enemy": {
    "health": 50.0,
    "speed": 2.0,
    "ai": "aggressive"
  },
  "enemies": [
    {
      "id": "zombie",
      "inherits": "base_enemy",
      "damage": 10.0
    },
    {
      "id": "skeleton",
      "inherits": "base_enemy",
      "health": 40.0,        // Override
      "damage": 8.0,
      "ranged": true          // Add new property
    }
  ]
}
```

### Environment-Specific Configs

Different configs for development vs. production:

```
config/
  development/
    character_stats.json  // Higher health for easier testing
  production/
    character_stats.json  // Balanced for players
  testing/
    character_stats.json  // Extreme values for edge case testing
```

Load based on build configuration.

## Integration with Other Systems

### Data-Driven + Component-Based

```cpp
// Load component values from config
Entity createFromConfig(const std::string& entityType)
{
    auto config = loadEntityConfig(entityType);
    
    Entity entity = entityManager->createEntity();
    
    // Components configured from data
    entityManager->addComponent(entity, 
        new HealthComponent(config["health"]));
    entityManager->addComponent(entity,
        new MovementComponent(config["speed"], config["acceleration"]));
    entityManager->addComponent(entity,
        new CombatComponent(config["damage"], config["attackSpeed"]));
    
    return entity;
}
```

### Data-Driven + Event-Driven

```json
{
  "events": {
    "player_death": {
      "respawnDelay": 3.0,
      "healthOnRespawn": 100.0,
      "dropItems": true,
      "playSound": "death.wav"
    }
  }
}
```

```cpp
auto eventConfig = loadEventConfig("player_death");

EventSystem::getInstance().subscribe("player_died", [eventConfig](const EventData& data) {
    // Behavior driven by config
    scheduleRespawn(eventConfig["respawnDelay"]);
    setRespawnHealth(eventConfig["healthOnRespawn"]);
    if (eventConfig["dropItems"]) {
        dropPlayerItems(player);
    }
    audioEngine->play(eventConfig["playSound"]);
});
```

### Data-Driven Testing

Create test-specific configs:

```json
// config/test/fast_combat.json
{
  "character_types": [
    {
      "id": "test_dummy",
      "health": 10.0,      // Dies quickly for faster tests
      "maxSpeed": 100.0,   // Moves fast for faster tests
      "acceleration": 1000.0
    }
  ]
}
```

```cpp
TEST(CombatSystemTest, FastCombatTest)
{
    // Use test config for faster test execution
    loadConfig("config/test/fast_combat.json");
    
    Entity dummy = createCharacterFromConfig("test_dummy", entityManager);
    // Test runs 10x faster with test config
}
```

## Best Practices

### ✅ DO

- **Use JSON for human-readable configs**: Easy to edit
- **Validate configs on load**: Catch errors early
- **Provide sensible defaults**: Graceful fallback if config missing
- **Version your configs**: Track in source control
- **Document config format**: Comment the structure
- **Use meaningful keys**: `maxSpeed` not `ms`
- **Group related settings**: Organize by feature
- **Support hot-reloading**: For rapid iteration

### ❌ DON'T

- **Don't put code in configs**: Keep it as data
- **Don't hardcode values**: If it might change, config it
- **Don't skip validation**: Bad config = hard-to-debug crashes
- **Don't use binary formats**: Hard to edit, bad for version control
- **Don't duplicate data**: Use inheritance or references
- **Don't commit sensitive data**: API keys, passwords, etc.

## Performance Considerations

### Config Caching

```cpp
class ConfigCache
{
public:
    static const CharacterStats& getCharacterStats(const std::string& id)
    {
        static std::map<std::string, CharacterStats> cache;
        
        if (cache.empty()) {
            cache = CharacterStatsLoader::loadStats("config/character_stats.json");
        }
        
        return cache.at(id);
    }
};

// Fast lookups after first load
auto stats = ConfigCache::getCharacterStats("warrior");  // Instant
```

### Lazy Loading

```cpp
// Don't load all configs at startup
// Load on-demand as needed

class ConfigManager
{
public:
    const WeaponConfig& getWeaponConfig(const std::string& weaponID)
    {
        if (!weaponConfigs.contains(weaponID)) {
            // Load only when first requested
            weaponConfigs[weaponID] = loadWeaponConfig(weaponID);
        }
        return weaponConfigs[weaponID];
    }
    
private:
    std::map<std::string, WeaponConfig> weaponConfigs;
};
```

## Example: Complete Data-Driven Feature

Adding a new enemy type without code changes:

**1. Designer adds to config:**

```json
{
  "character_types": [
    {
      "id": "enemy_golem",
      "name": "Stone Golem",
      "health": 200.0,
      "maxSpeed": 1.5,
      "acceleration": 2.0,
      "armor": 15.0,
      "damage": 30.0,
      "attackRange": 3.0,
      "ai": "slow_tank"
    }
  ]
}
```

**2. System loads and creates:**

```cpp
// Existing code automatically handles new enemy type!
Entity golem = createCharacterFromConfig("enemy_golem", entityManager);

// All components configured from JSON
auto* health = entityManager->getComponent<HealthComponent>(golem);
// health->getMaxHealth() == 200.0

auto* movement = entityManager->getComponent<MovementComponent>(golem);
// movement->getMaxSpeed() == 1.5
```

**3. Test and iterate:**

Designer tweaks values in JSON, tests, repeats until golem is fun!

## Tools & Libraries

Fresh Engine uses:
- **nlohmann/json**: Modern C++ JSON library
- **JSON Schema**: For validation
- **Hot-reload**: File watching for live updates

## Further Reading

- [COMPONENT_BASED_ARCHITECTURE.md](COMPONENT_BASED_ARCHITECTURE.md) - Components use data-driven configs
- [EVENT_DRIVEN_ARCHITECTURE.md](EVENT_DRIVEN_ARCHITECTURE.md) - Events can be configured via data
- [ITERATIVE_DEVELOPMENT.md](ITERATIVE_DEVELOPMENT.md) - Data-driven design enables rapid iteration
- Example configs: `config/character_stats.json`, `config/weapon_stats.json`
