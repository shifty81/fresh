#include "galaxy/GalaxyGenerator.h"

#include <cmath>
#include <sstream>

namespace fresh
{
namespace galaxy
{

GalaxyGenerator::GalaxyGenerator(uint32_t seed)
    : baseSeed(seed), asteroidDensity(10.0f), stationChance(0.1f), shipDensity(2.0f)
{
}

void GalaxyGenerator::setSeed(uint32_t seed)
{
    baseSeed = seed;
}

std::unique_ptr<GalaxySector> GalaxyGenerator::generateSector(int x, int y)
{
    auto sector = std::make_unique<GalaxySector>(x, y);

    // Create sector-specific RNG
    uint32_t sectorSeed = getSectorSeed(x, y);
    std::mt19937 rng(sectorSeed);

    // Generate sector content
    generateAsteroids(sector.get(), rng);
    generateStations(sector.get(), rng);
    generateShips(sector.get(), rng);

    return sector;
}

void GalaxyGenerator::generateAsteroids(GalaxySector* sector, std::mt19937& rng)
{
    std::uniform_real_distribution<float> posDist(-500.0f, 500.0f);
    std::uniform_real_distribution<float> sizeDist(5.0f, 50.0f);
    std::uniform_real_distribution<float> amountDist(100.0f, 1000.0f);
    std::uniform_int_distribution<int> countDist(5, 20);

    int asteroidCount = static_cast<int>(asteroidDensity * countDist(rng) / 10.0f);

    for (int i = 0; i < asteroidCount; ++i) {
        AsteroidData asteroid;
        asteroid.position = glm::vec3(posDist(rng), posDist(rng), posDist(rng));
        asteroid.size = sizeDist(rng);
        asteroid.resourceType = getRandomResourceType(rng);
        asteroid.resourceAmount = amountDist(rng);

        sector->addAsteroid(asteroid);
    }
}

void GalaxyGenerator::generateStations(GalaxySector* sector, std::mt19937& rng)
{
    std::uniform_real_distribution<float> chanceDist(0.0f, 1.0f);

    if (chanceDist(rng) < stationChance) {
        std::uniform_real_distribution<float> posDist(-300.0f, 300.0f);
        std::uniform_int_distribution<int> levelDist(1, 5);

        StationData station;
        station.position = glm::vec3(posDist(rng), posDist(rng), posDist(rng));
        station.type = getRandomStationType(rng);
        station.name = generateStationName(sector->getX(), sector->getY(), rng);
        station.level = levelDist(rng);

        sector->addStation(station);
    }
}

void GalaxyGenerator::generateShips(GalaxySector* sector, std::mt19937& rng)
{
    std::uniform_real_distribution<float> posDist(-400.0f, 400.0f);
    std::uniform_real_distribution<float> velDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> hostileDist(0.0f, 1.0f);
    std::uniform_int_distribution<int> countDist(0, 5);

    int shipCount = static_cast<int>(shipDensity * countDist(rng) / 2.0f);

    for (int i = 0; i < shipCount; ++i) {
        ShipData ship;
        ship.position = glm::vec3(posDist(rng), posDist(rng), posDist(rng));
        ship.velocity = glm::vec3(velDist(rng), velDist(rng), velDist(rng));
        ship.type = getRandomShipType(rng);
        ship.faction = getRandomFaction(rng);
        ship.isHostile = hostileDist(rng) < 0.3f; // 30% chance of hostile

        sector->addShip(ship);
    }
}

uint32_t GalaxyGenerator::getSectorSeed(int x, int y) const
{
    // Combine sector coordinates with base seed using hash function
    uint32_t hash = baseSeed;
    hash ^= static_cast<uint32_t>(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= static_cast<uint32_t>(y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}

ResourceType GalaxyGenerator::getRandomResourceType(std::mt19937& rng)
{
    std::uniform_int_distribution<int> dist(0, 6);
    return static_cast<ResourceType>(dist(rng));
}

StationType GalaxyGenerator::getRandomStationType(std::mt19937& rng)
{
    std::uniform_int_distribution<int> dist(0, 5);
    return static_cast<StationType>(dist(rng));
}

ShipType GalaxyGenerator::getRandomShipType(std::mt19937& rng)
{
    std::uniform_int_distribution<int> dist(0, 6);
    return static_cast<ShipType>(dist(rng));
}

std::string GalaxyGenerator::generateStationName(int x, int y, std::mt19937& rng)
{
    std::vector<std::string> prefixes = {"Alpha",   "Beta", "Gamma", "Delta",
                                         "Epsilon", "Zeta", "Nova",  "Stellar"};
    std::vector<std::string> suffixes = {"Station", "Outpost",  "Haven", "Port",
                                         "Hub",     "Terminal", "Base"};

    std::uniform_int_distribution<size_t> prefixDist(0, prefixes.size() - 1);
    std::uniform_int_distribution<size_t> suffixDist(0, suffixes.size() - 1);

    std::ostringstream oss;
    oss << prefixes[prefixDist(rng)] << " " << suffixes[suffixDist(rng)] << " " << x << "-" << y;

    return oss.str();
}

std::string GalaxyGenerator::getRandomFaction(std::mt19937& rng)
{
    std::vector<std::string> factions = {"Independent", "Federation", "Empire",   "Republic",
                                         "Alliance",    "Pirates",    "Merchants"};

    std::uniform_int_distribution<size_t> dist(0, factions.size() - 1);
    return factions[dist(rng)];
}

} // namespace galaxy
} // namespace fresh
