# Procedural Voxel Character System - Design Document

## Overview

This document describes the design and implementation of the procedural voxel character generation and animation system for the Fresh Voxel Engine.

## Architecture

### Three Core Systems

#### 1. Voxel Character Generation System
Responsible for creating unique character models using voxels.

**Components:**
- `VoxelCharacter` - Main character class holding voxel data and skeleton
- `BodyPartTemplate` - Reusable body part definitions
- `CharacterGenerator` - Algorithms for generating characters
- `CharacterDatabase` - Storage for templates and presets

**Generation Methods:**
- **Modular Assembly**: Combine pre-made body parts
- **Algorithmic Generation**: Use noise and rules to create unique shapes
- **Hybrid Approach**: Start with template, apply algorithmic variations

#### 2. Rigging and Animation System
Provides skeletal structure and animation data management.

**Components:**
- `Bone` - Individual bone with hierarchy and transforms
- `Skeleton` - Complete bone hierarchy (shared across characters)
- `AnimationClip` - Keyframe data for animations
- `AnimationController` - Manages animation playback and blending

**Features:**
- Shared skeleton for all humanoid characters
- Animation retargeting for different proportions
- Bone weights for smooth deformation
- Animation state machine

#### 3. Procedural Animation System
Generates and modifies animations dynamically at runtime.

**Components:**
- `IKSolver` - Inverse Kinematics for natural limb placement
- `ProceduralAnimator` - Real-time animation generation
- `PhysicsAnimator` - Physics-based secondary motion
- `EnvironmentAdapter` - Adapts animation to terrain

**Features:**
- **Inverse Kinematics**: Foot placement on uneven terrain
- **Dynamic Weapon Handling**: Adjust grip for different items
- **Physics Integration**: Cloth, hair, backpack physics
- **Environmental Interaction**: Climbing, ducking, leaning

## Implementation Plan

### Phase 1: Basic Character Generation (Week 1-2)
- [x] Create `VoxelCharacter` class structure
- [ ] Implement humanoid skeleton definition
- [ ] Create basic algorithmic generator (simple humanoid)
- [ ] Add color palette support
- [ ] Implement voxel-to-bone binding
- [ ] Create simple character mesh renderer
- [ ] Add character to player representation

### Phase 2: Modular Body Parts (Week 3-4)
- [ ] Design body part template system
- [ ] Create basic body part library (5-10 variations each)
  - Head shapes (round, square, triangular)
  - Torso types (slim, normal, bulky)
  - Arm variations (thin, normal, muscular)
  - Leg variations
- [ ] Implement body part assembly system
- [ ] Add attachment point calculation
- [ ] Create character customization UI

### Phase 3: Basic Animation (Week 5-6)
- [ ] Define animation clip format (keyframes)
- [ ] Create animation controller
- [ ] Implement basic animations:
  - Idle pose
  - Walk cycle
  - Run cycle
  - Jump
- [ ] Add animation blending (walk→run transition)
- [ ] Integrate with player movement

### Phase 4: Procedural Animation - IK (Week 7-8)
- [ ] Implement Two-Bone IK solver (arms, legs)
- [ ] Create foot IK system for terrain adaptation
- [ ] Add hand IK for item holding
- [ ] Implement look-at IK (head tracking)
- [ ] Test on uneven voxel terrain

### Phase 5: Dynamic Weapon/Tool Handling (Week 9-10)
- [ ] Design weapon attachment system
- [ ] Create grip point calculation
- [ ] Implement dynamic swing animations
- [ ] Add two-handed weapon support
- [ ] Create shield holding animations
- [ ] Test with various tool sizes

### Phase 6: Physics Integration (Week 11-12)
- [ ] Add cloth physics for capes/clothing
- [ ] Implement backpack/item physics
- [ ] Create hair physics system
- [ ] Add ragdoll physics for death
- [ ] Implement hit reactions

### Phase 7: Environmental Interaction (Week 13-14)
- [ ] Climbing system
- [ ] Ladder climbing
- [ ] Swimming animations
- [ ] Ducking under obstacles
- [ ] Ledge grabbing
- [ ] Slope adaptation

## Technical Details

### Skeleton Definition

**Standard Humanoid Skeleton (15 bones):**
```
Root (Hips)
├── Spine
│   ├── LeftShoulder
│   │   ├── LeftArm
│   │   │   └── LeftHand
│   ├── RightShoulder
│   │   ├── RightArm
│   │   │   └── RightHand
│   └── Neck
│       └── Head
├── LeftHip
│   ├── LeftLeg
│   │   └── LeftFoot
└── RightHip
    ├── RightLeg
        └── RightFoot
```

### Animation Data Format

**Keyframe Structure:**
```cpp
struct Keyframe {
    float time;                    // Time in seconds
    std::vector<BoneTransform> transforms;  // One per bone
};

struct BoneTransform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};
```

### IK Algorithm (Two-Bone IK)

```
Given: Target position, bone lengths L1 and L2
1. Calculate distance D from origin to target
2. Use law of cosines to find joint angles
3. Apply angle constraints
4. Update bone rotations
```

## Performance Considerations

### Character Rendering
- **Instancing**: Render multiple characters efficiently
- **LOD**: Reduce voxel count for distant characters
- **Culling**: Don't render off-screen characters
- **Batching**: Group similar characters

### Animation Updates
- **Update Rate**: 30-60 FPS for animations
- **Distance-based**: Reduce update rate for distant characters
- **Thread Pool**: Update animations on worker threads
- **Spatial Partitioning**: Only update nearby characters

## File Structure

```
include/character/
├── VoxelCharacter.h              - Main character class
├── CharacterGenerator.h          - Generation algorithms
├── BodyPartTemplate.h            - Body part definitions
├── Skeleton.h                    - Bone hierarchy
├── AnimationClip.h               - Animation data
├── AnimationController.h         - Animation playback
├── IKSolver.h                    - Inverse kinematics
├── ProceduralAnimator.h          - Runtime animation
└── CharacterRenderer.h           - Character rendering

src/character/
├── VoxelCharacter.cpp
├── CharacterGenerator.cpp
├── BodyPartTemplate.cpp
├── Skeleton.cpp
├── AnimationClip.cpp
├── AnimationController.cpp
├── IKSolver.cpp
├── ProceduralAnimator.cpp
└── CharacterRenderer.cpp
```

## Integration with Existing Systems

### Player Class
- Add `VoxelCharacter* visualRepresentation`
- Update position/rotation to character
- Sync animation state with movement

### Rendering System
- Add character mesh to render queue
- Support instanced rendering
- Add character shader (voxel + lighting)

### Physics System
- Query character bounds for collision
- Apply forces to character position
- Handle ragdoll physics

## Testing Strategy

### Unit Tests
- Character generation variations
- Skeleton hierarchy traversal
- Animation blending calculations
- IK solver accuracy

### Integration Tests
- Character in game world
- Animation with player movement
- IK on varied terrain
- Multiple characters rendering

### Visual Tests
- Character appearance variety
- Animation smoothness
- IK natural movement
- Physics interaction

## Examples from Popular Voxel Games

### Minecraft
- Simple blocky characters (8x8x8 heads, 8x12x4 body)
- Basic skeletal animation (6 bones)
- Simple walk cycle animation

### Teardown
- High-resolution voxels for characters
- Destructible character models
- Ragdoll physics on death

### Hytale
- Detailed voxel characters with many parts
- Complex animations with IK
- Modular customization system

### Vintage Story
- Detailed character models (100+ voxels)
- Smooth animations with many keyframes
- Tool-specific animations

## Future Enhancements

### Advanced Features (Post-v1.0)
- Morph targets for facial expressions
- Cloth simulation for capes/robes
- Procedural idle animations (breathing, fidgeting)
- Emotion system (affects posture)
- Dynamic LOD based on importance
- Animation compression
- GPU skinning
- Character editor in-game

### Multiplayer Considerations
- Network animation synchronization
- Client-side prediction
- Server-authoritative positioning
- Animation state replication

## References

- Two-Bone IK: https://theorangeduck.com/page/simple-two-joint
- Animation Blending: Game Programming Gems series
- Voxel Rendering: Efficient Sparse Voxel Octrees
- Minecraft Character Model: https://minecraft.wiki/Character
