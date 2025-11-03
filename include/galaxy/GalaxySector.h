#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace fresh {
namespace galaxy {

/**
 * @brief Resource types found in asteroids
 */
enum class ResourceType {
    Iron,
    Titanium,
    Naonite,
    Trinium,
    Xanion,
    Ogonite,
    Avorion
};

/**
 * @brief Data structure for an asteroid
 */
struct AsteroidData {
    glm::vec3 position;
    float size;
    ResourceType resourceType;
    float resourceAmount;
    
    AsteroidData() 
        : position(0.0f), size(1.0f), 
          resourceType(ResourceType::Iron), resourceAmount(0.0f) {}
};

/**
 * @brief Station types
 */
enum class StationType {
    TradingPost,
    RepairDock,
    Shipyard,
    Factory,
    MiningStation,
    ResearchStation
};

/**
 * @brief Data structure for a station
 */
struct StationData {
    glm::vec3 position;
    StationType type;
    std::string name;
    int level;
    
    StationData()
        : position(0.0f), type(StationType::TradingPost), 
          name("Station"), level(1) {}
};

/**
 * @brief Ship types
 */
enum class ShipType {
    Fighter,
    Corvette,
    Frigate,
    Destroyer,
    Cruiser,
    Battleship,
    Carrier
};

/**
 * @brief Data structure for a ship
 */
struct ShipData {
    glm::vec3 position;
    glm::vec3 velocity;
    ShipType type;
    std::string faction;
    bool isHostile;
    
    ShipData()
        : position(0.0f), velocity(0.0f), type(ShipType::Fighter),
          faction("Independent"), isHostile(false) {}
};

/**
 * @brief Represents a sector in the galaxy
 */
class GalaxySector {
public:
    GalaxySector();
    GalaxySector(int x, int y);
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    const std::vector<AsteroidData>& getAsteroids() const { return asteroids; }
    const std::vector<StationData>& getStations() const { return stations; }
    const std::vector<ShipData>& getShips() const { return ships; }
    
    // Setters
    void addAsteroid(const AsteroidData& asteroid);
    void addStation(const StationData& station);
    void addShip(const ShipData& ship);
    
    // Clear sector data
    void clear();
    
private:
    int x;
    int y;
    std::vector<AsteroidData> asteroids;
    std::vector<StationData> stations;
    std::vector<ShipData> ships;
};

} // namespace galaxy
} // namespace fresh
