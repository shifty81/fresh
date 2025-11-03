# Fresh Voxel Engine - Implementation Plan

## Phase 6: Critical Systems Implementation

This document provides detailed implementation guidance for the highest-priority features needed to make the engine playable.

---

## 1. First-Person Player Controller

### Overview
Implement a complete FPS character controller with smooth movement, jumping, and collision detection.

### Components to Implement

#### 1.1 Player Class (`include/gameplay/Player.h`)
```cpp
class Player {
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec2 rotation; // pitch and yaw
    float height = 1.8f;
    float radius = 0.3f;
    bool isGrounded = false;
    bool isSprinting = false;
    bool isCrouching = false;
    
public:
    void update(float deltaTime);
    void handleMovement(const InputState& input, float deltaTime);
    void handleJump();
    void applyGravity(float deltaTime);
    bool checkCollision(const VoxelWorld* world);
    glm::mat4 getViewMatrix() const;
};
```

#### 1.2 Camera System (`include/gameplay/Camera.h`)
```cpp
class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float pitch = 0.0f;
    float yaw = -90.0f;
    float fov = 75.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    
public:
    void updateVectors();
    void processMouseMovement(float xOffset, float yOffset, float sensitivity);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    void setPosition(const glm::vec3& pos);
    void setRotation(float pitch, float yaw);
};
```

#### 1.3 Implementation Steps
1. Create Player class with basic properties
2. Implement movement in XZ plane (WASD)
3. Add gravity and jumping
4. Implement collision detection with voxel grid
5. Add crouching and sprinting
6. Integrate with Camera class
7. Add smooth interpolation for camera movement

### Physics Integration
```cpp
// Collision detection pseudo-code
bool Player::checkCollision(const VoxelWorld* world) {
    // Check AABB (Axis-Aligned Bounding Box) against voxel grid
    glm::vec3 min = position - glm::vec3(radius, 0, radius);
    glm::vec3 max = position + glm::vec3(radius, height, radius);
    
    // Sample voxels in bounding box
    for (int x = floor(min.x); x <= ceil(max.x); x++) {
        for (int y = floor(min.y); y <= ceil(max.y); y++) {
            for (int z = floor(min.z); z <= ceil(max.z); z++) {
                VoxelType voxel = world->getVoxel(x, y, z);
                if (voxel != VoxelType::Air) {
                    return true; // Collision detected
                }
            }
        }
    }
    return false;
}
```

---

## 2. Enhanced Input System

### Overview
Create a flexible input management system that supports key rebinding and multiple input contexts.

### Components to Implement

#### 2.1 Input Manager (`include/input/InputManager.h`)
```cpp
enum class InputAction {
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
    Crouch,
    Sprint,
    Use,
    Attack,
    OpenInventory,
    OpenMenu,
    ToggleEditor,
    // ... more actions
};

enum class InputContext {
    Gameplay,
    Menu,
    Editor,
    Chat
};

class InputManager {
private:
    std::map<InputAction, int> keyBindings; // Action -> GLFW key code
    std::map<int, bool> keyStates;
    std::map<int, bool> keyPressedThisFrame;
    InputContext currentContext = InputContext::Gameplay;
    glm::vec2 mouseDelta;
    float mouseX = 0, mouseY = 0;
    float sensitivity = 0.002f;
    
public:
    void initialize();
    void update();
    void processKeyEvent(int key, int action);
    void processMouseMovement(double xpos, double ypos);
    
    bool isActionPressed(InputAction action) const;
    bool isActionJustPressed(InputAction action) const;
    bool isActionReleased(InputAction action) const;
    
    glm::vec2 getMouseDelta() const { return mouseDelta; }
    void setContext(InputContext context);
    void setKeyBinding(InputAction action, int key);
    void loadKeyBindings(const std::string& filepath);
    void saveKeyBindings(const std::string& filepath);
};
```

#### 2.2 Default Key Bindings
```cpp
// Default bindings
W -> MoveForward
A -> MoveLeft
S -> MoveBackward
D -> MoveRight
Space -> Jump
Shift -> Sprint
Ctrl -> Crouch
E -> Use
Mouse Left -> Attack
Mouse Right -> PlaceBlock
Tab -> OpenInventory
Escape -> OpenMenu
T -> ToggleEditor
Enter -> OpenChat
```

#### 2.3 Implementation Steps
1. Create InputManager with key/mouse state tracking
2. Implement action mapping system
3. Add context switching
4. Create default key bindings
5. Implement key rebinding support
6. Add save/load for custom bindings
7. Create input configuration UI (Phase 17)

---

## 3. Infinite World - Chunk Streaming System

### Overview
Dynamically load and unload chunks based on player position to create an infinite world.

### Components to Implement

#### 3.1 Chunk Manager Enhancement (`include/voxel/ChunkManager.h`)
```cpp
struct ChunkLoadRequest {
    glm::ivec2 chunkPos;
    int priority; // Based on distance from player
    bool urgent = false;
};

class ChunkManager {
private:
    VoxelWorld* world;
    glm::vec3 playerPosition;
    std::map<glm::ivec2, Chunk*> loadedChunks;
    std::priority_queue<ChunkLoadRequest> loadQueue;
    std::vector<glm::ivec2> unloadQueue;
    
    int viewDistance = 8; // In chunks (8 * 16 = 128 blocks)
    int maxLoadedChunks = 1000;
    
    // Threading
    std::thread generationThread;
    std::mutex chunkMutex;
    std::atomic<bool> shouldRun{true};
    
public:
    void update(const glm::vec3& playerPos);
    void determineChunksToLoad();
    void determineChunksToUnload();
    void processLoadQueue();
    void generateChunkAsync(const glm::ivec2& chunkPos);
    void setViewDistance(int distance);
    
    // Thread-safe accessors
    Chunk* getChunk(const glm::ivec2& pos);
    bool isChunkLoaded(const glm::ivec2& pos);
};
```

#### 3.2 Circular Loading Pattern
```cpp
void ChunkManager::determineChunksToLoad() {
    glm::ivec2 playerChunk = worldToChunk(playerPosition);
    
    // Load chunks in a circular pattern around player
    for (int r = 0; r <= viewDistance; r++) {
        for (int x = -r; x <= r; x++) {
            for (int z = -r; z <= r; z++) {
                // Only load chunks at current ring radius
                if (abs(x) == r || abs(z) == r) {
                    glm::ivec2 chunkPos = playerChunk + glm::ivec2(x, z);
                    
                    if (!isChunkLoaded(chunkPos)) {
                        int priority = r; // Closer chunks have higher priority (lower number)
                        loadQueue.push({chunkPos, priority, r <= 2});
                    }
                }
            }
        }
    }
}
```

#### 3.3 Implementation Steps
1. Extract chunk management from VoxelWorld to ChunkManager
2. Add player position tracking
3. Implement circular chunk loading pattern
4. Create priority queue for chunk loading
5. Add chunk unloading for distant chunks
6. Implement background generation thread
7. Add thread-safe chunk access
8. Optimize mesh generation to run async
9. Add chunk caching (save/load frequently accessed chunks)
10. Implement view distance configuration

---

## 4. Level of Detail (LOD) System

### Overview
Reduce polygon count for distant chunks while maintaining visual quality.

### Components to Implement

#### 4.1 LOD Manager (`include/renderer/LODManager.h`)
```cpp
enum class LODLevel {
    High = 0,    // Full detail, < 5 chunks
    Medium = 1,  // Reduced detail, 5-10 chunks
    Low = 2,     // Low detail, 10-15 chunks
    VeryLow = 3  // Minimal detail, > 15 chunks
};

class LODManager {
private:
    std::map<glm::ivec2, LODLevel> chunkLODs;
    float lodDistances[4] = {80.0f, 160.0f, 240.0f, 320.0f};
    
public:
    LODLevel calculateLOD(const glm::vec3& playerPos, const glm::ivec2& chunkPos);
    void updateChunkLODs(const glm::vec3& playerPos);
    void setLODDistances(float high, float medium, float low);
};
```

#### 4.2 LOD Mesh Generation
```cpp
// Different mesh generation strategies per LOD level
class MeshGenerator {
public:
    static Mesh generateMesh(Chunk* chunk, LODLevel lod) {
        switch(lod) {
            case LODLevel::High:
                return generateFullMesh(chunk);
            case LODLevel::Medium:
                return generateReducedMesh(chunk, 2); // Skip every 2nd detail
            case LODLevel::Low:
                return generateReducedMesh(chunk, 4); // Skip every 4th detail
            case LODLevel::VeryLow:
                return generateSimplifiedMesh(chunk); // Single color per chunk face
        }
    }
    
private:
    static Mesh generateSimplifiedMesh(Chunk* chunk);
    static Mesh generateReducedMesh(Chunk* chunk, int skipFactor);
};
```

#### 4.3 Frustum Culling
```cpp
class FrustumCuller {
private:
    glm::vec4 frustumPlanes[6]; // Left, Right, Bottom, Top, Near, Far
    
public:
    void extractFrustum(const glm::mat4& viewProjection);
    bool isChunkVisible(const glm::ivec2& chunkPos, float chunkSize);
    
private:
    float distanceToPlane(const glm::vec3& point, const glm::vec4& plane);
};
```

#### 4.4 Implementation Steps
1. Create LODManager class
2. Implement distance-based LOD calculation
3. Add LOD-aware mesh generation
4. Implement mesh simplification algorithms
5. Add frustum culling
6. Optimize rendering to skip invisible chunks
7. Add smooth LOD transitions (optional)
8. Profile and optimize performance

---

## 5. Basic Inventory System

### Overview
Implement item storage, stacking, and basic inventory UI.

### Components to Implement

#### 5.1 Item System (`include/gameplay/Item.h`)
```cpp
enum class ItemType {
    None,
    // Resources
    Wood, Stone, Dirt, Sand, Iron, Gold, Diamond, Coal,
    // Tools
    WoodenPickaxe, StonePickaxe, IronPickaxe,
    WoodenAxe, StoneAxe, IronAxe,
    // Food
    Apple, Bread, CookedMeat,
    // Building
    Torch, Chest, Door,
    // Special
    Blueprint, Key
};

struct ItemData {
    ItemType type;
    std::string name;
    std::string description;
    int maxStackSize;
    bool isStackable;
    // ... more properties
};

class Item {
private:
    ItemType type;
    int quantity;
    int durability; // For tools
    
public:
    Item(ItemType type, int quantity = 1);
    bool canStackWith(const Item& other) const;
    bool addQuantity(int amount); // Returns true if successful
    int removeQuantity(int amount); // Returns amount actually removed
    ItemData* getData() const;
};
```

#### 5.2 Inventory System (`include/gameplay/Inventory.h`)
```cpp
class Inventory {
private:
    static const int INVENTORY_SIZE = 36; // Main inventory
    static const int HOTBAR_SIZE = 9;     // Quick access
    
    std::vector<Item*> slots;
    int selectedHotbarSlot = 0;
    int maxWeight = 1000; // Optional weight system
    int currentWeight = 0;
    
public:
    Inventory();
    
    bool addItem(Item* item);
    bool removeItem(int slot, int quantity = 1);
    bool moveItem(int fromSlot, int toSlot);
    bool swapItems(int slot1, int slot2);
    Item* getItem(int slot) const;
    Item* getSelectedItem() const;
    
    void setSelectedSlot(int slot);
    int getSelectedSlot() const { return selectedHotbarSlot; }
    
    bool hasSpace() const;
    int findItem(ItemType type) const; // Returns slot or -1
    int countItem(ItemType type) const;
    
    void clear();
    void sort(); // Sort by type, then quantity
};
```

#### 5.3 Inventory UI (`include/ui/InventoryUI.h`)
```cpp
class InventoryUI {
private:
    Inventory* inventory;
    bool isOpen = false;
    int hoveredSlot = -1;
    Item* draggedItem = nullptr;
    
public:
    void render(); // Using ImGui
    void handleInput(const InputManager& input);
    void open();
    void close();
    bool isShowing() const { return isOpen; }
    
private:
    void renderInventoryGrid();
    void renderHotbar();
    void renderItemTooltip(const Item* item);
    void handleSlotClick(int slot);
    void handleSlotRightClick(int slot);
};
```

#### 5.4 Implementation Steps
1. Create Item and ItemData structures
2. Implement Inventory class with slot management
3. Add item stacking logic
4. Create inventory serialization (save/load)
5. Implement basic inventory UI with ImGui
6. Add drag-and-drop functionality
7. Create hotbar quick access
8. Add item tooltips
9. Implement split stack functionality (Shift+Click)

---

## 6. Resource Gathering System

### Overview
Place resource nodes in the world and implement gathering mechanics.

### Components to Implement

#### 6.1 Resource Node (`include/gameplay/ResourceNode.h`)
```cpp
enum class ResourceType {
    Tree,
    Rock,
    IronOre,
    GoldOre,
    CoalOre,
    Plant,
    Bush
};

class ResourceNode {
private:
    ResourceType type;
    glm::vec3 position;
    int health;
    int maxHealth;
    float respawnTime = 300.0f; // 5 minutes
    float timeSinceDestroyed = 0.0f;
    bool isDestroyed = false;
    
    // Loot table
    std::vector<std::pair<ItemType, int>> lootTable;
    
public:
    void takeDamage(int damage);
    void update(float deltaTime);
    bool canHarvest() const;
    std::vector<Item*> harvest(); // Returns loot
    void respawn();
};
```

#### 6.2 Resource Manager (`include/gameplay/ResourceManager.h`)
```cpp
class ResourceManager {
private:
    std::vector<ResourceNode*> activeNodes;
    VoxelWorld* world;
    
    // Generation parameters
    float treeFrequency = 0.05f;
    float rockFrequency = 0.02f;
    
public:
    void generateResourcesForChunk(Chunk* chunk);
    void update(float deltaTime);
    ResourceNode* findNearestNode(const glm::vec3& pos, float maxDistance);
    void removeNode(ResourceNode* node);
    
private:
    void placeTree(const glm::vec3& pos);
    void placeRock(const glm::vec3& pos);
    void placeOreVein(const glm::vec3& pos, ResourceType oreType);
};
```

#### 6.3 Gathering Mechanics
```cpp
class GatheringSystem {
private:
    Player* player;
    ResourceManager* resourceManager;
    ResourceNode* targetNode = nullptr;
    float gatherProgress = 0.0f;
    float gatherTime = 2.0f; // Time to complete gathering
    
public:
    void update(float deltaTime, const InputManager& input);
    void startGathering(ResourceNode* node);
    void stopGathering();
    bool isGathering() const { return targetNode != nullptr; }
    float getProgress() const { return gatherProgress / gatherTime; }
    
private:
    void completeGathering();
    bool canGather(ResourceNode* node) const; // Check tool, distance, etc.
};
```

#### 6.4 Implementation Steps
1. Create ResourceNode class
2. Implement loot tables
3. Create ResourceManager for world generation
4. Add tree/rock/ore placement in TerrainGenerator
5. Implement gathering mechanics
6. Add progress bar UI
7. Integrate with Inventory system
8. Add respawn timer system
9. Create tool requirement checking
10. Add gathering animations (optional)

---

## 7. ImGui Integration

### Overview
Integrate ImGui for all UI elements (inventory, menus, editor, etc.)

### Components to Implement

#### 7.1 ImGui Setup (`include/ui/ImGuiManager.h`)
```cpp
class ImGuiManager {
private:
    GLFWwindow* window;
    VkInstance instance;
    VkDevice device;
    // ... Vulkan resources for ImGui
    
public:
    void initialize(GLFWwindow* window, VkInstance instance, VkDevice device);
    void shutdown();
    void beginFrame();
    void endFrame();
    void render(VkCommandBuffer commandBuffer);
};
```

#### 7.2 Implementation Steps
1. Add ImGui as dependency to CMakeLists.txt
2. Set up ImGui with GLFW backend
3. Set up ImGui with Vulkan backend
4. Create ImGuiManager class
5. Integrate with main render loop
6. Create UI theme/styling
7. Implement example UI (debug window)
8. Test with inventory UI

---

## Implementation Order & Time Estimates

### Phase 6.1: Player & Input (1-2 weeks)
1. **Day 1-2**: Camera system implementation
2. **Day 3-4**: Player movement (WASD)
3. **Day 5-6**: Collision detection
4. **Day 7-8**: Jump and gravity
5. **Day 9-10**: Input manager with key bindings
6. **Day 11-12**: Sprint and crouch
7. **Day 13-14**: Testing and polish

### Phase 6.2: Chunk Streaming (1-2 weeks)
1. **Day 1-2**: Extract ChunkManager from VoxelWorld
2. **Day 3-4**: Implement circular loading pattern
3. **Day 5-6**: Priority queue and background generation
4. **Day 7-8**: Chunk unloading system
5. **Day 9-10**: Thread-safe implementation
6. **Day 11-12**: Optimization and caching
7. **Day 13-14**: Testing with various view distances

### Phase 6.3: LOD System (1 week)
1. **Day 1-2**: LODManager implementation
2. **Day 3-4**: Simplified mesh generation
3. **Day 5-6**: Frustum culling
4. **Day 7**: Testing and optimization

### Phase 6.4: Inventory & Resources (1-2 weeks)
1. **Day 1-2**: Item system
2. **Day 3-4**: Inventory implementation
3. **Day 5-6**: ResourceNode and loot tables
4. **Day 7-8**: Resource generation in world
5. **Day 9-10**: Gathering mechanics
6. **Day 11-12**: ImGui setup
7. **Day 13-14**: Inventory UI

### Phase 6.5: Integration & Testing (1 week)
1. **Day 1-3**: Integration of all systems
2. **Day 4-5**: Bug fixing
3. **Day 6-7**: Performance optimization

**Total Estimated Time for Phase 6: 5-8 weeks**

---

## Testing Checklist

### Player Controller
- [ ] Player can move forward/back/left/right
- [ ] Player can look around with mouse
- [ ] Player can jump
- [ ] Player collides with terrain
- [ ] Player can't walk through walls
- [ ] Sprint increases movement speed
- [ ] Crouch reduces player height

### Input System
- [ ] All keys can be rebound
- [ ] Key bindings save/load correctly
- [ ] Input contexts switch properly
- [ ] Mouse sensitivity is adjustable

### Chunk Streaming
- [ ] Chunks load as player moves
- [ ] Distant chunks unload
- [ ] No stuttering during chunk loading
- [ ] Memory usage stays within limits
- [ ] View distance can be changed

### LOD System
- [ ] Distant chunks use lower detail
- [ ] LOD transitions are smooth
- [ ] FPS improves with LOD enabled
- [ ] Chunks outside frustum aren't rendered

### Inventory & Resources
- [ ] Items can be added to inventory
- [ ] Items stack correctly
- [ ] Inventory UI displays correctly
- [ ] Resources spawn in world
- [ ] Resources can be gathered
- [ ] Resources respawn after time
- [ ] Loot is added to inventory

---

## Performance Targets

### FPS Targets
- **High Settings**: 60+ FPS (view distance 12 chunks)
- **Medium Settings**: 60+ FPS (view distance 8 chunks)
- **Low Settings**: 60+ FPS (view distance 6 chunks)

### Memory Targets
- **Chunk Data**: < 100 KB per chunk
- **Total Loaded Chunks**: < 1 GB
- **Mesh Data**: < 500 MB
- **Total Game**: < 2 GB

### Network Targets (for Phase 12)
- **Player Position Updates**: 20/second
- **Chunk Data Transfer**: < 1 MB/second
- **Latency**: < 100ms acceptable

---

## Next Steps

After Phase 6 completion, proceed to:
1. **Phase 7**: Day/Night cycle and lighting
2. **Phase 8**: Profession system
3. **Phase 9**: Advanced inventory and crafting
4. **Phase 10**: Weather and biome improvements
5. **Phase 11**: Dungeon system
6. **Phase 12**: Multiplayer networking

Refer to `ROADMAP.md` for complete feature list and long-term planning.

---

**Document Version**: 1.0
**Last Updated**: 2025-11-03
**Status**: Active Development
