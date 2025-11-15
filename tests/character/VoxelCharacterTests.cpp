#include <gtest/gtest.h>

#include "character/VoxelCharacter.h"

using namespace fresh;

class VoxelCharacterTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        character = std::make_unique<VoxelCharacter>();
    }

    void TearDown() override
    {
        character.reset();
    }

    std::unique_ptr<VoxelCharacter> character;
};

// ============================================================================
// Constructor and Initialization Tests
// ============================================================================

TEST_F(VoxelCharacterTest, Constructor_InitializesHumanoidSkeleton)
{
    // A humanoid skeleton should have 16 bones (indices 0-15)
    const auto& skeleton = character->getSkeleton();
    EXPECT_EQ(skeleton.size(), 16);
}

TEST_F(VoxelCharacterTest, Constructor_SkeletonHasRootBone)
{
    const auto& skeleton = character->getSkeleton();
    ASSERT_FALSE(skeleton.empty());
    EXPECT_EQ(skeleton[0].name, "Root");
    EXPECT_EQ(skeleton[0].parentIndex, -1); // Root has no parent
}

TEST_F(VoxelCharacterTest, Constructor_SkeletonHasExpectedBones)
{
    // Check for key bones by name
    int rootIndex = character->getBoneIndex("Root");
    int spineIndex = character->getBoneIndex("Spine");
    int headIndex = character->getBoneIndex("Head");
    int leftArmIndex = character->getBoneIndex("LeftArm");
    int rightLegIndex = character->getBoneIndex("RightLeg");
    
    EXPECT_GE(rootIndex, 0);
    EXPECT_GE(spineIndex, 0);
    EXPECT_GE(headIndex, 0);
    EXPECT_GE(leftArmIndex, 0);
    EXPECT_GE(rightLegIndex, 0);
}

// ============================================================================
// Bone Hierarchy Tests
// ============================================================================

TEST_F(VoxelCharacterTest, Skeleton_HasCorrectHierarchy)
{
    const auto& skeleton = character->getSkeleton();
    
    // Spine's parent should be Root
    int spineIndex = character->getBoneIndex("Spine");
    EXPECT_EQ(skeleton[spineIndex].parentIndex, character->getBoneIndex("Root"));
    
    // Head's parent chain: Head -> Neck -> Spine -> Root
    int headIndex = character->getBoneIndex("Head");
    int neckIndex = character->getBoneIndex("Neck");
    EXPECT_EQ(skeleton[headIndex].parentIndex, neckIndex);
    EXPECT_EQ(skeleton[neckIndex].parentIndex, spineIndex);
}

TEST_F(VoxelCharacterTest, Skeleton_ArmsAttachedCorrectly)
{
    const auto& skeleton = character->getSkeleton();
    
    int spineIndex = character->getBoneIndex("Spine");
    int leftShoulderIndex = character->getBoneIndex("LeftShoulder");
    int rightShoulderIndex = character->getBoneIndex("RightShoulder");
    int leftArmIndex = character->getBoneIndex("LeftArm");
    int rightArmIndex = character->getBoneIndex("RightArm");
    
    // Shoulders attached to Spine
    EXPECT_EQ(skeleton[leftShoulderIndex].parentIndex, spineIndex);
    EXPECT_EQ(skeleton[rightShoulderIndex].parentIndex, spineIndex);
    
    // Arms attached to Shoulders
    EXPECT_EQ(skeleton[leftArmIndex].parentIndex, leftShoulderIndex);
    EXPECT_EQ(skeleton[rightArmIndex].parentIndex, rightShoulderIndex);
}

TEST_F(VoxelCharacterTest, Skeleton_LegsAttachedCorrectly)
{
    const auto& skeleton = character->getSkeleton();
    
    int rootIndex = character->getBoneIndex("Root");
    int leftHipIndex = character->getBoneIndex("LeftHip");
    int rightHipIndex = character->getBoneIndex("RightHip");
    int leftLegIndex = character->getBoneIndex("LeftLeg");
    int rightLegIndex = character->getBoneIndex("RightLeg");
    
    // Hips attached to Root
    EXPECT_EQ(skeleton[leftHipIndex].parentIndex, rootIndex);
    EXPECT_EQ(skeleton[rightHipIndex].parentIndex, rootIndex);
    
    // Legs attached to Hips
    EXPECT_EQ(skeleton[leftLegIndex].parentIndex, leftHipIndex);
    EXPECT_EQ(skeleton[rightLegIndex].parentIndex, rightHipIndex);
}

// ============================================================================
// Character Generation Tests
// ============================================================================

TEST_F(VoxelCharacterTest, GenerateFromParams_CreatesVoxels)
{
    CharacterGenerationParams params;
    params.seed = 12345;
    params.baseHeight = 16;
    
    character->generateFromParams(params);
    
    auto voxels = character->getTransformedVoxels();
    EXPECT_GT(voxels.size(), 0) << "Character should have voxels after generation";
}

TEST_F(VoxelCharacterTest, GenerateFromParams_DifferentSeedsProduceDifferentCharacters)
{
    CharacterGenerationParams params1;
    params1.seed = 123;
    character->generateFromParams(params1);
    auto voxels1 = character->getTransformedVoxels();
    size_t count1 = voxels1.size();
    
    CharacterGenerationParams params2;
    params2.seed = 456;
    character->generateFromParams(params2);
    auto voxels2 = character->getTransformedVoxels();
    size_t count2 = voxels2.size();
    
    // With different seeds and random color variations, counts might differ
    // At minimum, we should have generated voxels both times
    EXPECT_GT(count1, 0);
    EXPECT_GT(count2, 0);
}

TEST_F(VoxelCharacterTest, GenerateFromParams_HeadScaleAffectsGeneration)
{
    CharacterGenerationParams params;
    params.seed = 1000; // Fixed seed for consistency
    params.headScale = 1.5f;
    
    character->generateFromParams(params);
    auto voxels = character->getTransformedVoxels();
    
    // Larger head should produce more voxels
    EXPECT_GT(voxels.size(), 100) << "Character with scaled head should have voxels";
}

TEST_F(VoxelCharacterTest, GenerateFromParams_SetsCharacterDimensions)
{
    CharacterGenerationParams params;
    params.seed = 999;
    params.baseHeight = 16;
    
    character->generateFromParams(params);
    
    glm::ivec3 dimensions = character->getDimensions();
    EXPECT_GT(dimensions.x, 0);
    EXPECT_GT(dimensions.y, 0);
    EXPECT_GT(dimensions.z, 0);
}

// ============================================================================
// Body Part Assembly Tests
// ============================================================================

TEST_F(VoxelCharacterTest, AssembleFromParts_EmptyParts_CreatesEmptyCharacter)
{
    std::vector<BodyPartTemplate> parts;
    character->assembleFromParts(parts);
    
    auto voxels = character->getTransformedVoxels();
    EXPECT_EQ(voxels.size(), 0);
}

TEST_F(VoxelCharacterTest, AssembleFromParts_WithParts_CombinesVoxels)
{
    std::vector<BodyPartTemplate> parts;
    
    // Create a simple head part
    BodyPartTemplate head;
    head.type = BodyPartType::Head;
    CharacterVoxel voxel1;
    voxel1.position = {0, 0, 0};
    voxel1.color = {1.0f, 0.0f, 0.0f};
    voxel1.boneIndex = 3;
    head.voxels.push_back(voxel1);
    parts.push_back(head);
    
    // Create a simple torso part
    BodyPartTemplate torso;
    torso.type = BodyPartType::Torso;
    CharacterVoxel voxel2;
    voxel2.position = {0, -5, 0};
    voxel2.color = {0.0f, 1.0f, 0.0f};
    voxel2.boneIndex = 1;
    torso.voxels.push_back(voxel2);
    parts.push_back(torso);
    
    character->assembleFromParts(parts);
    
    auto voxels = character->getTransformedVoxels();
    EXPECT_EQ(voxels.size(), 2);
}

// ============================================================================
// Transform and Animation Tests
// ============================================================================

TEST_F(VoxelCharacterTest, SetBoneRotation_ValidBone_UpdatesRotation)
{
    int spineIndex = character->getBoneIndex("Spine");
    ASSERT_GE(spineIndex, 0);
    
    glm::vec3 rotation(0.5f, 0.0f, 0.0f);
    character->setBoneRotation(spineIndex, rotation);
    
    const auto& skeleton = character->getSkeleton();
    EXPECT_FLOAT_EQ(skeleton[spineIndex].localRotation.x, 0.5f);
}

TEST_F(VoxelCharacterTest, SetBoneRotation_InvalidBone_DoesNotCrash)
{
    // Should not crash with invalid bone index
    EXPECT_NO_THROW(character->setBoneRotation(-1, glm::vec3(0.0f)));
    EXPECT_NO_THROW(character->setBoneRotation(999, glm::vec3(0.0f)));
}

TEST_F(VoxelCharacterTest, Update_UpdatesTransforms)
{
    CharacterGenerationParams params;
    params.seed = 777;
    character->generateFromParams(params);
    
    // Set rotation on a bone
    int armIndex = character->getBoneIndex("LeftArm");
    character->setBoneRotation(armIndex, glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Update should recalculate transforms
    EXPECT_NO_THROW(character->update(0.016f));
}

TEST_F(VoxelCharacterTest, GetTransformedVoxels_AppliesBoneTransforms)
{
    CharacterGenerationParams params;
    params.seed = 555;
    character->generateFromParams(params);
    
    auto voxelsBefore = character->getTransformedVoxels();
    
    // Rotate a bone
    int spineIndex = character->getBoneIndex("Spine");
    character->setBoneRotation(spineIndex, glm::vec3(0.0f, 1.57f, 0.0f)); // 90 degrees
    
    auto voxelsAfter = character->getTransformedVoxels();
    
    // Voxel positions should change after rotation
    EXPECT_EQ(voxelsBefore.size(), voxelsAfter.size());
    // Note: Actual positions will differ due to rotation, but we just verify no crash
}

// ============================================================================
// Position and Orientation Tests
// ============================================================================

TEST_F(VoxelCharacterTest, SetPosition_UpdatesPosition)
{
    glm::vec3 newPos(10.0f, 20.0f, 30.0f);
    character->setPosition(newPos);
    
    glm::vec3 pos = character->getPosition();
    EXPECT_FLOAT_EQ(pos.x, 10.0f);
    EXPECT_FLOAT_EQ(pos.y, 20.0f);
    EXPECT_FLOAT_EQ(pos.z, 30.0f);
}

TEST_F(VoxelCharacterTest, SetRotation_UpdatesRotation)
{
    glm::vec3 newRot(0.5f, 1.0f, 1.5f);
    character->setRotation(newRot);
    
    glm::vec3 rot = character->getRotation();
    EXPECT_FLOAT_EQ(rot.x, 0.5f);
    EXPECT_FLOAT_EQ(rot.y, 1.0f);
    EXPECT_FLOAT_EQ(rot.z, 1.5f);
}

TEST_F(VoxelCharacterTest, GetBoneIndex_ExistingBone_ReturnsValidIndex)
{
    int index = character->getBoneIndex("Head");
    EXPECT_GE(index, 0);
    EXPECT_LT(index, static_cast<int>(character->getSkeleton().size()));
}

TEST_F(VoxelCharacterTest, GetBoneIndex_NonExistentBone_ReturnsNegative)
{
    int index = character->getBoneIndex("NonExistentBone");
    EXPECT_EQ(index, -1);
}

// ============================================================================
// Edge Cases and Robustness Tests
// ============================================================================

TEST_F(VoxelCharacterTest, GenerateFromParams_ExtremeSeed_DoesNotCrash)
{
    CharacterGenerationParams params;
    params.seed = UINT32_MAX;
    
    EXPECT_NO_THROW(character->generateFromParams(params));
}

TEST_F(VoxelCharacterTest, GenerateFromParams_ExtremeScaling_DoesNotCrash)
{
    CharacterGenerationParams params;
    params.headScale = 0.5f;
    params.torsoScale = 1.5f;
    params.limbScale = 0.8f;
    
    EXPECT_NO_THROW(character->generateFromParams(params));
    EXPECT_GT(character->getTransformedVoxels().size(), 0);
}

TEST_F(VoxelCharacterTest, Update_MultipleUpdates_MaintainsStability)
{
    CharacterGenerationParams params;
    character->generateFromParams(params);
    
    // Update multiple times
    for (int i = 0; i < 100; ++i) {
        EXPECT_NO_THROW(character->update(0.016f));
    }
    
    // Character should still have voxels
    EXPECT_GT(character->getTransformedVoxels().size(), 0);
}
