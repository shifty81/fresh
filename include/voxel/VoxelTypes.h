#pragma once

#include <cstdint>
#include <functional>

namespace fresh {

/**
 * @brief Voxel type enumeration
 */
enum class VoxelType : uint8_t {
    Air = 0,
    Stone,
    Dirt,
    Grass,
    Sand,
    Water,
    Wood,
    Leaves,
    Bedrock,
    Snow,
    Ice,
    Cobblestone,
    Planks,
    Glass,
    // Add more types as needed
    Count
};

/**
 * @brief Voxel data structure
 */
struct Voxel {
    VoxelType type;
    uint8_t light; // Light level 0-15
    
    Voxel() : type(VoxelType::Air), light(0) {}
    explicit Voxel(VoxelType t) : type(t), light(0) {}
    
    bool isOpaque() const {
        return type != VoxelType::Air && type != VoxelType::Water && type != VoxelType::Glass;
    }
    
    bool isTransparent() const {
        return type == VoxelType::Water || type == VoxelType::Glass || type == VoxelType::Ice;
    }
    
    bool isSolid() const {
        return type != VoxelType::Air;
    }
};

/**
 * @brief Chunk dimensions
 */
constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

/**
 * @brief World coordinates
 */
struct WorldPos {
    int x, y, z;
    
    WorldPos() : x(0), y(0), z(0) {}
    WorldPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    
    bool operator==(const WorldPos& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

/**
 * @brief Chunk coordinates
 */
struct ChunkPos {
    int x, z;
    
    ChunkPos() : x(0), z(0) {}
    ChunkPos(int _x, int _z) : x(_x), z(_z) {}
    
    bool operator==(const ChunkPos& other) const {
        return x == other.x && z == other.z;
    }
    
    static ChunkPos fromWorldPos(const WorldPos& pos) {
        return ChunkPos(
            pos.x >= 0 ? pos.x / CHUNK_SIZE : (pos.x + 1) / CHUNK_SIZE - 1,
            pos.z >= 0 ? pos.z / CHUNK_SIZE : (pos.z + 1) / CHUNK_SIZE - 1
        );
    }
};

} // namespace fresh

// Hash function for ChunkPos to use in unordered_map
namespace std {
    template<>
    struct hash<fresh::ChunkPos> {
        std::size_t operator()(const fresh::ChunkPos& pos) const {
            // Use FNV-1a hash
            std::size_t h1 = std::hash<int>{}(pos.x);
            std::size_t h2 = std::hash<int>{}(pos.z);
            return h1 ^ (h2 << 1);
        }
    };
}
