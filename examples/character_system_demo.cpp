#include "character/VoxelCharacter.h"
#include "character/BodyPartLibrary.h"
#include "character/AnimationClip.h"
#include "character/AnimationController.h"
#include "character/IKSolver.h"
#include "character/CharacterRenderer.h"
#include <iostream>

/**
 * @brief Example demonstrating the Phase 7 character system
 * 
 * This example shows how to:
 * 1. Create a procedurally generated character
 * 2. Assemble a character from body part templates
 * 3. Set up animation controller with state machine
 * 4. Apply IK for foot placement
 * 5. Render the character
 */

int main() {
    std::cout << "=== Fresh Voxel Engine - Character System Demo ===" << std::endl;
    
    // ========================================================================
    // 1. Create a procedurally generated character
    // ========================================================================
    std::cout << "\n1. Creating procedurally generated character..." << std::endl;
    
    fresh::VoxelCharacter proceduralCharacter;
    
    fresh::CharacterGenerationParams params;
    params.baseHeight = 16;
    params.headScale = 1.0f;
    params.torsoScale = 1.1f;
    params.limbScale = 1.0f;
    params.skinColor = glm::vec3(0.8f, 0.6f, 0.5f);
    params.primaryColor = glm::vec3(0.2f, 0.4f, 0.8f);   // Blue clothing
    params.secondaryColor = glm::vec3(0.3f, 0.3f, 0.3f); // Gray pants
    params.seed = 12345;
    
    proceduralCharacter.generateFromParams(params);
    proceduralCharacter.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    std::cout << "  ✓ Generated character with " << proceduralCharacter.getTransformedVoxels().size() 
              << " voxels" << std::endl;
    std::cout << "  ✓ Character dimensions: " 
              << proceduralCharacter.getDimensions().x << "x"
              << proceduralCharacter.getDimensions().y << "x"
              << proceduralCharacter.getDimensions().z << std::endl;
    
    // ========================================================================
    // 2. Create a character from body part templates
    // ========================================================================
    std::cout << "\n2. Creating character from body part library..." << std::endl;
    
    fresh::BodyPartLibrary library;
    library.initialize();
    
    std::cout << "  ✓ Body part library initialized" << std::endl;
    std::cout << "    - Head variations: " << library.getVariationCount(fresh::BodyPartType::Head) << std::endl;
    std::cout << "    - Torso variations: " << library.getVariationCount(fresh::BodyPartType::Torso) << std::endl;
    std::cout << "    - Arm variations: " << library.getVariationCount(fresh::BodyPartType::LeftArm) << std::endl;
    std::cout << "    - Leg variations: " << library.getVariationCount(fresh::BodyPartType::LeftLeg) << std::endl;
    
    // Create a random character
    fresh::VoxelCharacter modularCharacter;
    auto randomParts = library.createRandomCharacter(54321);
    modularCharacter.assembleFromParts(randomParts);
    modularCharacter.setPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    
    std::cout << "  ✓ Created random character from " << randomParts.size() << " body parts" << std::endl;
    
    // ========================================================================
    // 3. Set up animation controller
    // ========================================================================
    std::cout << "\n3. Setting up animation system..." << std::endl;
    
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
    walkState.speed = 1.0f;
    animController.addState(walkState);
    
    fresh::AnimationState runState;
    runState.name = "Run";
    runState.clip = fresh::AnimationClipFactory::createRunAnimation();
    runState.speed = 1.5f;
    animController.addState(runState);
    
    fresh::AnimationState jumpState;
    jumpState.name = "Jump";
    jumpState.clip = fresh::AnimationClipFactory::createJumpAnimation();
    jumpState.speed = 1.0f;
    animController.addState(jumpState);
    
    std::cout << "  ✓ Added 4 animation states (Idle, Walk, Run, Jump)" << std::endl;
    
    // Add transitions
    fresh::AnimationTransition idleToWalk;
    idleToWalk.fromState = "Idle";
    idleToWalk.toState = "Walk";
    idleToWalk.conditionParameter = "speed";
    idleToWalk.conditionValue = 0.1f;
    idleToWalk.conditionGreaterThan = true;
    idleToWalk.blendDuration = 0.3f;
    animController.addTransition(idleToWalk);
    
    fresh::AnimationTransition walkToRun;
    walkToRun.fromState = "Walk";
    walkToRun.toState = "Run";
    walkToRun.conditionParameter = "speed";
    walkToRun.conditionValue = 5.0f;
    walkToRun.conditionGreaterThan = true;
    walkToRun.blendDuration = 0.2f;
    animController.addTransition(walkToRun);
    
    fresh::AnimationTransition runToWalk;
    runToWalk.fromState = "Run";
    runToWalk.toState = "Walk";
    runToWalk.conditionParameter = "speed";
    runToWalk.conditionValue = 5.0f;
    runToWalk.conditionGreaterThan = false;
    runToWalk.blendDuration = 0.2f;
    animController.addTransition(runToWalk);
    
    fresh::AnimationTransition walkToIdle;
    walkToIdle.fromState = "Walk";
    walkToIdle.toState = "Idle";
    walkToIdle.conditionParameter = "speed";
    walkToIdle.conditionValue = 0.1f;
    walkToIdle.conditionGreaterThan = false;
    walkToIdle.blendDuration = 0.3f;
    animController.addTransition(walkToIdle);
    
    std::cout << "  ✓ Added 4 animation transitions" << std::endl;
    
    // ========================================================================
    // 4. Apply IK for foot placement
    // ========================================================================
    std::cout << "\n4. Setting up IK system..." << std::endl;
    
    fresh::FootIK footIK;
    footIK.setLegLengths(2.0f, 2.0f);  // Upper leg, lower leg
    footIK.setMaxStretch(1.2f);
    footIK.setEnabled(true);
    
    // Example: Place left foot on ground
    glm::vec3 hipPosition(0.0f, 10.0f, 0.0f);
    glm::vec3 groundPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 surfaceNormal(0.0f, 1.0f, 0.0f);
    
    fresh::IKSolution footSolution = footIK.solveFoot(hipPosition, groundPosition, surfaceNormal);
    
    if (footSolution.success) {
        std::cout << "  ✓ Foot IK solved successfully" << std::endl;
        std::cout << "    - Hip rotation: (" 
                  << footSolution.joint1Rotation.x << ", "
                  << footSolution.joint1Rotation.y << ", "
                  << footSolution.joint1Rotation.z << ")" << std::endl;
        std::cout << "    - Knee rotation: (" 
                  << footSolution.joint2Rotation.x << ", "
                  << footSolution.joint2Rotation.y << ", "
                  << footSolution.joint2Rotation.z << ")" << std::endl;
    }
    
    // Look-at IK
    glm::vec3 headPosition(0.0f, 15.0f, 0.0f);
    glm::vec3 lookTarget(5.0f, 15.0f, 5.0f);
    glm::vec3 lookRotation = fresh::LookAtIK::calculateLookRotation(headPosition, lookTarget);
    
    std::cout << "  ✓ Look-at IK calculated" << std::endl;
    std::cout << "    - Head rotation: (" 
              << lookRotation.x << ", "
              << lookRotation.y << ", "
              << lookRotation.z << ")" << std::endl;
    
    // ========================================================================
    // 5. Simulate animation updates
    // ========================================================================
    std::cout << "\n5. Simulating animation updates..." << std::endl;
    
    // Start with idle
    animController.setState("Idle");
    std::cout << "  ✓ Initial state: " << animController.getCurrentState() << std::endl;
    
    // Simulate walking
    animController.setParameter("speed", 2.0f);
    animController.update(proceduralCharacter, 0.016f);  // 60 FPS
    std::cout << "  ✓ After walking transition: " << animController.getCurrentState() << std::endl;
    
    // Simulate running
    animController.setParameter("speed", 7.0f);
    for (int i = 0; i < 10; ++i) {
        animController.update(proceduralCharacter, 0.016f);
    }
    std::cout << "  ✓ After running transition: " << animController.getCurrentState() << std::endl;
    
    // Stop
    animController.setParameter("speed", 0.0f);
    for (int i = 0; i < 20; ++i) {
        animController.update(proceduralCharacter, 0.016f);
    }
    std::cout << "  ✓ After stopping: " << animController.getCurrentState() << std::endl;
    
    // ========================================================================
    // 6. Character renderer setup (stub)
    // ========================================================================
    std::cout << "\n6. Character renderer (stub example)..." << std::endl;
    
    fresh::CharacterRenderer renderer;
    std::cout << "  ✓ Character renderer created" << std::endl;
    std::cout << "    Note: Actual rendering requires RenderContext initialization" << std::endl;
    
    fresh::CharacterRenderManager renderManager;
    renderManager.initialize(&renderer);
    int charId1 = renderManager.registerCharacter(&proceduralCharacter);
    int charId2 = renderManager.registerCharacter(&modularCharacter);
    
    std::cout << "  ✓ Registered 2 characters with render manager" << std::endl;
    std::cout << "    - Character 1 ID: " << charId1 << std::endl;
    std::cout << "    - Character 2 ID: " << charId2 << std::endl;
    
    // ========================================================================
    // Summary
    // ========================================================================
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "✓ Phase 7 Character System Features Demonstrated:" << std::endl;
    std::cout << "  1. Procedural character generation with customizable parameters" << std::endl;
    std::cout << "  2. Modular character assembly from body part library" << std::endl;
    std::cout << "  3. Animation system with state machine and blending" << std::endl;
    std::cout << "  4. IK system for foot placement and look-at" << std::endl;
    std::cout << "  5. Character rendering infrastructure" << std::endl;
    std::cout << "\nCharacter system is ready for integration with game engine!" << std::endl;
    
    return 0;
}
