#include "voxel/ChunkStreamer.h"
#include "voxel/VoxelWorld.h"
#include "voxel/Chunk.h"
#include <cmath>
#include <iostream>

namespace fresh {

ChunkStreamer::ChunkStreamer(VoxelWorld* world)
    : world(world) {
    // Start background generation thread
    generationThread = std::thread(&ChunkStreamer::generationThreadFunc, this);
}

ChunkStreamer::~ChunkStreamer() {
    shutdown();
}

void ChunkStreamer::update(const glm::vec3& playerPosition) {
    if (!world) return;
    
    glm::ivec2 playerChunk = worldToChunk(playerPosition);
    
    // Only update if player moved to a different chunk
    if (playerChunk != lastPlayerChunk) {
        lastPlayerChunk = playerChunk;
        determineChunksToLoad(playerPosition);
        determineChunksToUnload(playerPosition);
    }
    
    // Process some chunks from load queue
    processLoadQueue();
}

void ChunkStreamer::setViewDistance(int chunks) {
    viewDistance = std::max(1, std::min(chunks, 32)); // Clamp to reasonable range
}

void ChunkStreamer::shutdown() {
    shouldRun = false;
    if (generationThread.joinable()) {
        generationThread.join();
    }
}

int ChunkStreamer::getLoadedChunkCount() const {
    // This would need to query VoxelWorld for actual count
    // For now, return estimate
    return 0; // TODO: implement in VoxelWorld
}

bool ChunkStreamer::isChunkLoaded(const glm::ivec2& chunkPos) const {
    // TODO: Query VoxelWorld
    return false;
}

void ChunkStreamer::determineChunksToLoad(const glm::vec3& playerPos) {
    glm::ivec2 playerChunk = worldToChunk(playerPos);
    
    std::lock_guard<std::mutex> lock(queueMutex);
    
    // Load chunks in circular pattern around player
    for (int radius = 0; radius <= viewDistance; ++radius) {
        // Only process chunks at current radius (forms expanding circles)
        for (int x = -radius; x <= radius; ++x) {
            for (int z = -radius; z <= radius; ++z) {
                // Check if this is on the current radius ring
                int dist = std::max(std::abs(x), std::abs(z));
                if (dist != radius) continue;
                
                glm::ivec2 chunkPos = playerChunk + glm::ivec2(x, z);
                
                // Skip if already loaded or pending
                if (isChunkLoaded(chunkPos)) continue;
                if (pendingLoads.find(chunkPos) != pendingLoads.end()) continue;
                
                // Add to load queue with priority based on distance
                ChunkLoadRequest request;
                request.chunkPos = chunkPos;
                request.priority = radius;
                
                loadQueue.push(request);
                pendingLoads.insert(chunkPos);
            }
        }
    }
}

void ChunkStreamer::determineChunksToUnload(const glm::vec3& playerPos) {
    glm::ivec2 playerChunk = worldToChunk(playerPos);
    
    unloadQueue.clear();
    
    // TODO: Query VoxelWorld for all loaded chunks
    // For each loaded chunk, if distance > viewDistance + buffer, mark for unload
    float unloadDistance = static_cast<float>(viewDistance) + 2.0f;
    
    // Example (would need actual implementation):
    // for (auto& chunk : world->getLoadedChunks()) {
    //     float dist = chunkDistance(chunk.position, playerChunk);
    //     if (dist > unloadDistance) {
    //         unloadQueue.push_back(chunk.position);
    //     }
    // }
}

void ChunkStreamer::processLoadQueue() {
    std::lock_guard<std::mutex> lock(queueMutex);
    
    int processed = 0;
    while (!loadQueue.empty() && processed < chunksPerFrame) {
        ChunkLoadRequest request = loadQueue.top();
        loadQueue.pop();
        
        // Remove from pending
        pendingLoads.erase(request.chunkPos);
        
        // Generate chunk (this should be fast if generation is in background)
        // TODO: Actually generate and add to world
        // world->generateChunk(request.chunkPos.x, request.chunkPos.y);
        
        ++processed;
    }
    
    // Process unload queue
    for (const auto& chunkPos : unloadQueue) {
        // TODO: Unload chunk from world
        // world->unloadChunk(chunkPos.x, chunkPos.y);
    }
    unloadQueue.clear();
}

void ChunkStreamer::generationThreadFunc() {
    while (shouldRun) {
        // Background chunk generation would happen here
        // For now, just sleep to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // TODO: Pull from generation queue and pre-generate chunk data
        // std::lock_guard<std::mutex> lock(queueMutex);
        // if (!loadQueue.empty()) {
        //     auto request = loadQueue.top();
        //     // Pre-generate chunk data in background
        //     // Store in cache for main thread to pick up
        // }
    }
}

glm::ivec2 ChunkStreamer::worldToChunk(const glm::vec3& worldPos) const {
    // Assuming chunk size of 16 blocks
    const int CHUNK_SIZE = 16;
    return glm::ivec2(
        static_cast<int>(std::floor(worldPos.x / CHUNK_SIZE)),
        static_cast<int>(std::floor(worldPos.z / CHUNK_SIZE))
    );
}

float ChunkStreamer::chunkDistance(const glm::ivec2& a, const glm::ivec2& b) const {
    int dx = a.x - b.x;
    int dz = a.y - b.y;
    return std::sqrt(static_cast<float>(dx * dx + dz * dz));
}

} // namespace fresh
