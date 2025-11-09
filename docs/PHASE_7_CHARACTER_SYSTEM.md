# Phase 7: Procedural Voxel Character System - Implementation Guide

> **Completion Date**: 2025-11-09  
> **Status**: ✅ Core Implementation Complete  
> **Next Phase**: Integration with Player Movement & Weapon Systems

---

## 📋 Overview

Phase 7 introduces a comprehensive procedural voxel character system to the Fresh Voxel Engine, enabling dynamic character generation, skeletal animation, inverse kinematics, and efficient rendering of voxel-based characters.

### Key Features
- ✅ **Procedural Character Generation** - Create unique characters algorithmically
- ✅ **Modular Body Part System** - Assemble characters from template libraries
- ✅ **Skeletal Animation** - 15-bone humanoid skeleton with keyframe animation
- ✅ **Animation State Machine** - Smooth transitions and blending between animations
- ✅ **Inverse Kinematics** - Adaptive foot placement and hand positioning
- ✅ **Character Rendering** - Efficient voxel-to-mesh conversion and rendering

---

## 🏗️ System Architecture

### Component Hierarchy

```
VoxelCharacter (Core)
├── Skeleton (15 bones)
│   ├── Root (Hips)
│   ├── Spine → Neck → Head
│   ├── Arms (Shoulder → Arm → Hand) × 2
│   └── Legs (Hip → Leg → Foot) × 2
├── Voxels (with bone binding)
└── Transform (position, rotation)

BodyPartLibrary (Generation)
├── Head Variations (5+)
├── Torso Variations (5+)
└── Limb Variations (3+ per type)

AnimationController (Animation)
├── Animation States
│   ├── Idle
│   ├── Walk
│   ├── Run
│   ├── Jump
│   └── Crouch
├── Transitions (parameter-based)
└── Blending System

IK Solvers (Procedural Animation)
├── Two-Bone IK (law of cosines)
├── Foot IK (terrain adaptation)
├── Hand IK (item holding)
└── Look-At IK (head tracking)

CharacterRenderer (Rendering)
├── Mesh Generation
├── GPU Buffer Management
└── Batch Rendering
```

---

## 🚀 Quick Start

### 1. Create a Procedural Character

```cpp
#include "character/VoxelCharacter.h"

fresh::VoxelCharacter character;

fresh::CharacterGenerationParams params;
params.baseHeight = 16;          // Height in voxels
params.headScale = 1.0f;         // Head size multiplier
params.torsoScale = 1.1f;        // Torso size multiplier
params.limbScale = 1.0f;         // Limb size multiplier
params.skinColor = glm::vec3(0.8f, 0.6f, 0.5f);
params.primaryColor = glm::vec3(0.2f, 0.4f, 0.8f);   // Clothing
params.secondaryColor = glm::vec3(0.3f, 0.3f, 0.3f); // Pants
params.seed = 12345;             // Random seed

character.generateFromParams(params);
character.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
```

### 2. Assemble from Body Parts

```cpp
#include "character/BodyPartLibrary.h"

fresh::BodyPartLibrary library;
library.initialize();

// Create random character
auto randomParts = library.createRandomCharacter(54321);
fresh::VoxelCharacter character;
character.assembleFromParts(randomParts);

// Or manually select parts
std::vector<fresh::BodyPartTemplate> parts;
parts.push_back(*library.getTemplate(fresh::BodyPartType::Head, 0));
parts.push_back(*library.getTemplate(fresh::BodyPartType::Torso, 1));
// ... add more parts
character.assembleFromParts(parts);
```

### 3. Set Up Animation

```cpp
#include "character/AnimationController.h"

fresh::AnimationController animController;

// Add animation states
fresh::AnimationState idleState;
idleState.name = "Idle";
idleState.clip = fresh::AnimationClipFactory::createIdleAnimation();
idleState.speed = 1.0f;
animController.addState(idleState);

fresh::AnimationState walkState;
walkState.name = "Walk";
walkState.clip = fresh::AnimationClipFactory::createWalkAnimation();
animController.addState(walkState);

// Add transition (Idle → Walk when speed > 0.1)
fresh::AnimationTransition idleToWalk;
idleToWalk.fromState = "Idle";
idleToWalk.toState = "Walk";
idleToWalk.conditionParameter = "speed";
idleToWalk.conditionValue = 0.1f;
idleToWalk.conditionGreaterThan = true;
idleToWalk.blendDuration = 0.3f;
animController.addTransition(idleToWalk);

// Update in game loop
float deltaTime = 0.016f;  // 60 FPS
animController.setParameter("speed", playerSpeed);
animController.update(character, deltaTime);
```

### 4. Apply IK

```cpp
#include "character/IKSolver.h"

// Foot IK for terrain adaptation
fresh::FootIK footIK;
footIK.setLegLengths(2.0f, 2.0f);  // Upper leg, lower leg
footIK.setMaxStretch(1.2f);

glm::vec3 hipPosition = character.getPosition() + glm::vec3(0, 10, 0);
glm::vec3 groundPosition = getTerrainHeightAt(x, z);
glm::vec3 surfaceNormal = getTerrainNormalAt(x, z);

fresh::IKSolution footSolution = footIK.solveFoot(
    hipPosition, groundPosition, surfaceNormal
);

if (footSolution.success) {
    int legBoneIndex = character.getBoneIndex("LeftLeg");
    character.setBoneRotation(legBoneIndex, footSolution.joint1Rotation);
    // Apply joint2Rotation to knee
}

// Look-at IK
glm::vec3 headPosition = character.getPosition() + glm::vec3(0, 15, 0);
glm::vec3 lookTarget = enemy.getPosition();
glm::vec3 headRotation = fresh::LookAtIK::calculateLookRotation(
    headPosition, lookTarget
);

int headBoneIndex = character.getBoneIndex("Head");
character.setBoneRotation(headBoneIndex, headRotation);
```

### 5. Render Character

```cpp
#include "character/CharacterRenderer.h"

fresh::CharacterRenderer renderer;
renderer.initialize(renderContext);

// Generate mesh from character
fresh::CharacterMesh mesh = renderer.generateMesh(character);

// Render in game loop
glm::mat4 worldTransform(1.0f);
worldTransform = glm::translate(worldTransform, character.getPosition());
renderer.render(mesh, worldTransform);

// Cleanup when done
renderer.releaseMesh(mesh);
```

---

## 📚 API Reference

### VoxelCharacter

**Purpose**: Core character class with skeleton and voxel data.

**Key Methods**:
- `generateFromParams(params)` - Generate character procedurally
- `assembleFromParts(parts)` - Assemble from body part templates
- `update(deltaTime)` - Update animations and transforms
- `setBoneRotation(boneIndex, rotation)` - Set bone rotation for animation
- `getTransformedVoxels()` - Get voxels with bone transforms applied

**Skeleton Bones** (indices):
```
0:  Root (Hips)
1:  Spine
2:  Neck
3:  Head
4:  LeftShoulder
5:  LeftArm
6:  LeftHand
7:  RightShoulder
8:  RightArm
9:  RightHand
10: LeftHip
11: LeftLeg
12: LeftFoot
13: RightHip
14: RightLeg
15: RightFoot
```

### BodyPartLibrary

**Purpose**: Library of pre-generated body part templates.

**Variations Available**:
- **Heads**: Cubic, Rounded, Tall, Wide, Small (5 total)
- **Torsos**: Standard, Bulky, Slim, Tall, Wide (5 total)
- **Arms**: Standard, Thick, Thin (3 variations × 2 sides)
- **Legs**: Standard, Thick, Long (3 variations × 2 sides)

**Key Methods**:
- `getTemplate(type, variationIndex)` - Get specific body part
- `getVariationCount(type)` - Get number of variations for a type
- `createRandomCharacter(seed)` - Generate random character parts

### AnimationClip

**Purpose**: Keyframe-based animation data.

**Key Methods**:
- `addKeyframe(keyframe)` - Add animation keyframe
- `sampleBone(time, boneIndex, outRotation, outPosition)` - Sample animation at time
- `getDuration()` - Get animation duration
- `setLooping(loop)` - Enable/disable looping

**Standard Animations** (via `AnimationClipFactory`):
- **Idle**: 4-second gentle swaying animation
- **Walk**: 1-second alternating leg movement
- **Run**: 0.6-second fast movement with body lean
- **Jump**: 1-second crouch-jump-land sequence
- **Crouch**: 0.3-second crouch down animation

### AnimationController

**Purpose**: State machine for managing animation playback and transitions.

**Key Methods**:
- `addState(state)` - Add animation state
- `addTransition(transition)` - Add state transition rule
- `setState(stateName)` - Manually set current state
- `setParameter(name, value)` - Set transition parameter
- `update(character, deltaTime)` - Update animation and apply to character

**Features**:
- Automatic state transitions based on parameters
- Smooth blending between animations
- Configurable blend durations
- Loop/one-shot animation support

### IK Solvers

**TwoBoneIK**:
- `solve(origin, joint1Length, joint2Length, target, poleVector)` - Solve IK chain
- `solveWithConstraints(...)` - Solve with angle limits

**FootIK**:
- `setLegLengths(upper, lower)` - Set leg bone lengths
- `solveFoot(hipPos, targetPos, surfaceNormal)` - Position foot on terrain

**HandIK**:
- `setArmLengths(upper, lower)` - Set arm bone lengths
- `solveHand(shoulderPos, targetPos, gripRotation)` - Position hand

**LookAtIK**:
- `calculateLookRotation(headPos, targetPos)` - Calculate head rotation
- `calculateLookRotationConstrained(...)` - With angle limits

---

## 🎨 Body Part Variations

### Head Types
1. **Cubic** - Classic blocky Minecraft-style head (4×4×4)
2. **Rounded** - Smooth spherical head (5×5×5)
3. **Tall** - Elongated head (4×5×4)
4. **Wide** - Broader head (5×4×4)
5. **Small** - Compact head (3×3×3)

### Torso Types
1. **Standard** - Balanced rectangular torso (4×6×2)
2. **Bulky** - Muscular/armored torso (5×6×3)
3. **Slim** - Lean build (3×6×2)
4. **Tall** - Elongated torso (4×7×2)
5. **Wide** - Broad shoulders (6×6×2)

### Limb Types
1. **Standard** - Balanced proportions
2. **Thick** - Muscular/bulky limbs
3. **Thin/Long** - Slim or elongated limbs

---

## 🎬 Animation System

### State Machine Flow

```
        [Idle]
         ↑  ↓
    speed>0.1 / speed<0.1
         ↓  ↑
        [Walk]
         ↑  ↓
    speed>5.0 / speed<5.0
         ↓  ↑
         [Run]
```

### Custom Animations

Create custom animations by defining keyframes:

```cpp
fresh::AnimationClip customClip("MyAnimation");
customClip.setLooping(true);

// Keyframe at 0.0s
fresh::AnimationKeyframe frame0;
frame0.time = 0.0f;
frame0.boneRotations[5] = glm::vec3(0.0f, 0.0f, 0.0f);  // LeftArm
customClip.addKeyframe(frame0);

// Keyframe at 1.0s
fresh::AnimationKeyframe frame1;
frame1.time = 1.0f;
frame1.boneRotations[5] = glm::vec3(1.57f, 0.0f, 0.0f);  // Rotate 90°
customClip.addKeyframe(frame1);

// Animation will interpolate between keyframes
```

---

## 🔧 Integration Guide

### With Player Controller

```cpp
class Player {
private:
    fresh::VoxelCharacter character;
    fresh::AnimationController animController;
    
public:
    void update(float deltaTime) {
        // Update movement
        glm::vec3 movement = getInputMovement();
        float speed = glm::length(movement);
        
        // Update animation based on movement
        animController.setParameter("speed", speed);
        animController.update(character, deltaTime);
        
        // Update character position
        character.setPosition(position);
        
        // Apply IK for terrain
        applyFootIK();
    }
    
    void applyFootIK() {
        // Get terrain height at foot positions
        // Apply foot IK to adapt to terrain
    }
};
```

### With Rendering Pipeline

```cpp
class Game {
private:
    fresh::CharacterRenderer charRenderer;
    fresh::CharacterRenderManager renderManager;
    std::vector<Player*> players;
    
public:
    void initialize() {
        charRenderer.initialize(renderContext);
        renderManager.initialize(&charRenderer);
        
        // Register player characters
        for (auto* player : players) {
            renderManager.registerCharacter(&player->character);
        }
    }
    
    void render() {
        renderManager.renderAll(viewMatrix, projectionMatrix);
    }
};
```

---

## 🧪 Testing

### Example Test Cases

```cpp
// Test 1: Character generation
TEST(CharacterSystem, ProceduralGeneration) {
    fresh::VoxelCharacter character;
    fresh::CharacterGenerationParams params;
    params.baseHeight = 16;
    character.generateFromParams(params);
    
    EXPECT_GT(character.getTransformedVoxels().size(), 0);
    EXPECT_EQ(character.getSkeleton().size(), 16);
}

// Test 2: Animation playback
TEST(CharacterSystem, AnimationPlayback) {
    fresh::AnimationController controller;
    fresh::AnimationState idle;
    idle.name = "Idle";
    idle.clip = fresh::AnimationClipFactory::createIdleAnimation();
    controller.addState(idle);
    
    fresh::VoxelCharacter character;
    controller.update(character, 0.016f);
    
    EXPECT_EQ(controller.getCurrentState(), "Idle");
}

// Test 3: IK solving
TEST(CharacterSystem, IKSolver) {
    glm::vec3 origin(0, 10, 0);
    glm::vec3 target(0, 0, 0);
    glm::vec3 pole(0, 0, 1);
    
    auto solution = fresh::TwoBoneIK::solve(origin, 2.0f, 2.0f, target, pole);
    
    EXPECT_TRUE(solution.success);
}
```

---

## 📊 Performance Considerations

### Character Complexity
- **Voxel Count**: ~200-500 voxels per character (depending on body type)
- **Bone Count**: 16 bones (fixed humanoid skeleton)
- **Mesh Vertices**: ~4,800-12,000 vertices (24 vertices × 6 faces per voxel)

### Optimization Tips
1. **Instancing**: Use `CharacterRenderManager` for batch rendering
2. **LOD**: Reduce voxel detail for distant characters
3. **Culling**: Skip animation updates for off-screen characters
4. **Caching**: Cache mesh generation results when character doesn't change

---

## 🐛 Known Limitations

1. **IK Solver**: Uses simplified 2D IK projected to 3D (sufficient for most cases)
2. **Animation**: Uses Euler angles (may have gimbal lock; consider quaternions for production)
3. **Rendering**: Platform-specific buffer creation not implemented (needs RenderContext)
4. **Physics**: Character physics and collision not yet integrated

---

## 🚀 Future Enhancements

### Phase 7 Remaining Work
- [ ] Weapon attachment system with grip points
- [ ] Dynamic weapon swing animations
- [ ] Character physics and collision
- [ ] Ragdoll system
- [ ] Integration with player movement

### Phase 8+ Ideas
- [ ] Facial expressions and eye blinking
- [ ] Cloth simulation for capes/clothing
- [ ] Procedural walk/run cycle generation
- [ ] Character customization UI
- [ ] Animation retargeting between different character rigs
- [ ] Morph targets for facial animation

---

## 📝 Code Examples

See `examples/character_system_demo.cpp` for a complete working example demonstrating:
- Procedural character generation
- Body part assembly
- Animation setup and playback
- IK foot placement
- Look-at IK
- Character rendering

---

## 📖 Additional Resources

- **Architecture**: See `ARCHITECTURE.md` for engine design
- **Roadmap**: See `ROADMAP.md` for Phase 7 details
- **Building**: See `BUILD.md` for compilation instructions
- **Contributing**: See `CONTRIBUTING.md` for contribution guidelines

---

## ✅ Phase 7 Completion Status

### Weeks 1-4: Character Generation ✅ COMPLETE
- [x] VoxelCharacter class
- [x] Humanoid skeleton (15 bones)
- [x] Procedural generation
- [x] Modular body parts (5+ variations each)
- [x] Body part library
- [x] Character customization
- [x] Voxel-to-bone binding
- [x] Character mesh renderer

### Weeks 5-7: Animation System ✅ COMPLETE
- [x] Animation clip format
- [x] Animation controller
- [x] Basic animations (Idle, Walk, Run, Jump, Crouch)
- [x] Animation blending
- [x] Animation state machine
- [ ] Player movement integration (next step)

### Weeks 8-10: IK System ✅ COMPLETE
- [x] Two-Bone IK solver
- [x] Foot IK for terrain
- [x] Hand IK for items
- [x] Look-at IK

### Weeks 11-12: Weapon/Tool Handling 🚧 IN PROGRESS
- [ ] Weapon attachment system
- [ ] Grip point calculation
- [ ] Dynamic swing animations

### Weeks 13-14: Physics Integration 📅 PLANNED
- [ ] Character physics
- [ ] Ragdoll system

---

**Last Updated**: 2025-11-09  
**Authors**: Fresh Voxel Engine Team  
**License**: MIT
