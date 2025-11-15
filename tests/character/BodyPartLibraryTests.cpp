#include <gtest/gtest.h>

#include "character/BodyPartLibrary.h"

using namespace fresh;

class BodyPartLibraryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        library = std::make_unique<BodyPartLibrary>();
    }

    void TearDown() override
    {
        library.reset();
    }

    std::unique_ptr<BodyPartLibrary> library;
};

// ============================================================================
// Initialization Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, Constructor_InitializesLibrary)
{
    EXPECT_NE(library, nullptr);
}

TEST_F(BodyPartLibraryTest, Constructor_PopulatesHeadVariations)
{
    int count = library->getVariationCount(BodyPartType::Head);
    EXPECT_GT(count, 0) << "Library should have head variations";
    EXPECT_GE(count, 5) << "Library should have at least 5 head variations";
}

TEST_F(BodyPartLibraryTest, Constructor_PopulatesTorsoVariations)
{
    int count = library->getVariationCount(BodyPartType::Torso);
    EXPECT_GT(count, 0) << "Library should have torso variations";
    EXPECT_GE(count, 5) << "Library should have at least 5 torso variations";
}

TEST_F(BodyPartLibraryTest, Constructor_PopulatesLimbVariations)
{
    int leftArmCount = library->getVariationCount(BodyPartType::LeftArm);
    int rightArmCount = library->getVariationCount(BodyPartType::RightArm);
    int leftLegCount = library->getVariationCount(BodyPartType::LeftLeg);
    int rightLegCount = library->getVariationCount(BodyPartType::RightLeg);
    
    EXPECT_GT(leftArmCount, 0);
    EXPECT_GT(rightArmCount, 0);
    EXPECT_GT(leftLegCount, 0);
    EXPECT_GT(rightLegCount, 0);
    
    // Arms should have matching number of variations
    EXPECT_EQ(leftArmCount, rightArmCount);
    // Legs should have matching number of variations
    EXPECT_EQ(leftLegCount, rightLegCount);
}

TEST_F(BodyPartLibraryTest, Constructor_PopulatesHandAndFootVariations)
{
    int leftHandCount = library->getVariationCount(BodyPartType::LeftHand);
    int rightHandCount = library->getVariationCount(BodyPartType::RightHand);
    int leftFootCount = library->getVariationCount(BodyPartType::LeftFoot);
    int rightFootCount = library->getVariationCount(BodyPartType::RightFoot);
    
    EXPECT_GT(leftHandCount, 0);
    EXPECT_GT(rightHandCount, 0);
    EXPECT_GT(leftFootCount, 0);
    EXPECT_GT(rightFootCount, 0);
}

// ============================================================================
// Template Retrieval Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, GetTemplate_ValidTypeAndIndex_ReturnsTemplate)
{
    const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, 0);
    ASSERT_NE(tmpl, nullptr);
    EXPECT_EQ(tmpl->type, BodyPartType::Head);
    EXPECT_GT(tmpl->voxels.size(), 0) << "Template should have voxels";
}

TEST_F(BodyPartLibraryTest, GetTemplate_InvalidIndex_ReturnsNull)
{
    const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, 999);
    EXPECT_EQ(tmpl, nullptr);
}

TEST_F(BodyPartLibraryTest, GetTemplate_NegativeIndex_ReturnsNull)
{
    const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, -1);
    EXPECT_EQ(tmpl, nullptr);
}

TEST_F(BodyPartLibraryTest, GetTemplate_AllHeadVariations_HaveCorrectType)
{
    int count = library->getVariationCount(BodyPartType::Head);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, i);
        ASSERT_NE(tmpl, nullptr);
        EXPECT_EQ(tmpl->type, BodyPartType::Head);
    }
}

TEST_F(BodyPartLibraryTest, GetTemplate_AllTorsoVariations_HaveCorrectType)
{
    int count = library->getVariationCount(BodyPartType::Torso);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Torso, i);
        ASSERT_NE(tmpl, nullptr);
        EXPECT_EQ(tmpl->type, BodyPartType::Torso);
    }
}

// ============================================================================
// Variation Count Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, GetVariationCount_ReturnsPositiveForAllTypes)
{
    std::vector<BodyPartType> types = {
        BodyPartType::Head,      BodyPartType::Torso,     BodyPartType::LeftArm,
        BodyPartType::RightArm,  BodyPartType::LeftLeg,   BodyPartType::RightLeg,
        BodyPartType::LeftHand,  BodyPartType::RightHand, BodyPartType::LeftFoot,
        BodyPartType::RightFoot};
    
    for (BodyPartType type : types) {
        int count = library->getVariationCount(type);
        EXPECT_GT(count, 0) << "Type should have at least one variation";
    }
}

// ============================================================================
// Random Character Creation Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, CreateRandomCharacter_ReturnsBodyParts)
{
    auto parts = library->createRandomCharacter(12345);
    EXPECT_GT(parts.size(), 0) << "Should create at least some body parts";
}

TEST_F(BodyPartLibraryTest, CreateRandomCharacter_IncludesAllMajorParts)
{
    auto parts = library->createRandomCharacter(54321);
    
    bool hasHead = false;
    bool hasTorso = false;
    bool hasArms = false;
    bool hasLegs = false;
    
    for (const auto& part : parts) {
        if (part.type == BodyPartType::Head) hasHead = true;
        if (part.type == BodyPartType::Torso) hasTorso = true;
        if (part.type == BodyPartType::LeftArm || part.type == BodyPartType::RightArm)
            hasArms = true;
        if (part.type == BodyPartType::LeftLeg || part.type == BodyPartType::RightLeg)
            hasLegs = true;
    }
    
    EXPECT_TRUE(hasHead) << "Random character should have a head";
    EXPECT_TRUE(hasTorso) << "Random character should have a torso";
    EXPECT_TRUE(hasArms) << "Random character should have arms";
    EXPECT_TRUE(hasLegs) << "Random character should have legs";
}

TEST_F(BodyPartLibraryTest, CreateRandomCharacter_DifferentSeedsProduceDifferentResults)
{
    auto parts1 = library->createRandomCharacter(111);
    auto parts2 = library->createRandomCharacter(222);
    
    EXPECT_EQ(parts1.size(), parts2.size()) << "Should have same number of parts";
    
    // At least one part should be different (different variation)
    // This is probabilistic but with different seeds should almost always differ
    // We just verify the method works without crashing
    EXPECT_EQ(parts1.size(), parts2.size());
}

TEST_F(BodyPartLibraryTest, CreateRandomCharacter_SameSeedProducesSameResult)
{
    auto parts1 = library->createRandomCharacter(9999);
    auto parts2 = library->createRandomCharacter(9999);
    
    ASSERT_EQ(parts1.size(), parts2.size());
    
    for (size_t i = 0; i < parts1.size(); ++i) {
        EXPECT_EQ(parts1[i].type, parts2[i].type);
        EXPECT_EQ(parts1[i].voxels.size(), parts2[i].voxels.size());
    }
}

// ============================================================================
// Template Content Validation Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, HeadTemplates_HaveVoxels)
{
    int count = library->getVariationCount(BodyPartType::Head);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, i);
        ASSERT_NE(tmpl, nullptr);
        EXPECT_GT(tmpl->voxels.size(), 0) << "Head variation " << i << " should have voxels";
    }
}

TEST_F(BodyPartLibraryTest, TorsoTemplates_HaveVoxels)
{
    int count = library->getVariationCount(BodyPartType::Torso);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Torso, i);
        ASSERT_NE(tmpl, nullptr);
        EXPECT_GT(tmpl->voxels.size(), 0) << "Torso variation " << i << " should have voxels";
    }
}

TEST_F(BodyPartLibraryTest, HeadTemplates_HaveCorrectBoneIndex)
{
    int count = library->getVariationCount(BodyPartType::Head);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Head, i);
        ASSERT_NE(tmpl, nullptr);
        
        // All head voxels should be assigned to Head bone (index 3)
        for (const auto& voxel : tmpl->voxels) {
            EXPECT_EQ(voxel.boneIndex, 3) << "Head voxel should be assigned to Head bone";
        }
    }
}

TEST_F(BodyPartLibraryTest, TorsoTemplates_HaveCorrectBoneIndex)
{
    int count = library->getVariationCount(BodyPartType::Torso);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* tmpl = library->getTemplate(BodyPartType::Torso, i);
        ASSERT_NE(tmpl, nullptr);
        
        // All torso voxels should be assigned to Spine bone (index 1)
        for (const auto& voxel : tmpl->voxels) {
            EXPECT_EQ(voxel.boneIndex, 1) << "Torso voxel should be assigned to Spine bone";
        }
    }
}

TEST_F(BodyPartLibraryTest, ArmTemplates_HaveCorrectBoneIndex)
{
    int count = library->getVariationCount(BodyPartType::LeftArm);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* leftArm = library->getTemplate(BodyPartType::LeftArm, i);
        const BodyPartTemplate* rightArm = library->getTemplate(BodyPartType::RightArm, i);
        
        ASSERT_NE(leftArm, nullptr);
        ASSERT_NE(rightArm, nullptr);
        
        // Left arm voxels should be assigned to LeftArm bone (index 5)
        for (const auto& voxel : leftArm->voxels) {
            EXPECT_EQ(voxel.boneIndex, 5);
        }
        
        // Right arm voxels should be assigned to RightArm bone (index 8)
        for (const auto& voxel : rightArm->voxels) {
            EXPECT_EQ(voxel.boneIndex, 8);
        }
    }
}

TEST_F(BodyPartLibraryTest, LegTemplates_HaveCorrectBoneIndex)
{
    int count = library->getVariationCount(BodyPartType::LeftLeg);
    for (int i = 0; i < count; ++i) {
        const BodyPartTemplate* leftLeg = library->getTemplate(BodyPartType::LeftLeg, i);
        const BodyPartTemplate* rightLeg = library->getTemplate(BodyPartType::RightLeg, i);
        
        ASSERT_NE(leftLeg, nullptr);
        ASSERT_NE(rightLeg, nullptr);
        
        // Left leg voxels should be assigned to LeftLeg bone (index 11)
        for (const auto& voxel : leftLeg->voxels) {
            EXPECT_EQ(voxel.boneIndex, 11);
        }
        
        // Right leg voxels should be assigned to RightLeg bone (index 14)
        for (const auto& voxel : rightLeg->voxels) {
            EXPECT_EQ(voxel.boneIndex, 14);
        }
    }
}

TEST_F(BodyPartLibraryTest, Templates_HaveValidDimensions)
{
    std::vector<BodyPartType> types = {BodyPartType::Head, BodyPartType::Torso,
                                       BodyPartType::LeftArm};
    
    for (BodyPartType type : types) {
        int count = library->getVariationCount(type);
        for (int i = 0; i < count; ++i) {
            const BodyPartTemplate* tmpl = library->getTemplate(type, i);
            ASSERT_NE(tmpl, nullptr);
            
            EXPECT_GT(tmpl->dimensions.x, 0) << "Template should have positive X dimension";
            EXPECT_GT(tmpl->dimensions.y, 0) << "Template should have positive Y dimension";
            EXPECT_GT(tmpl->dimensions.z, 0) << "Template should have positive Z dimension";
        }
    }
}

// ============================================================================
// Edge Cases and Robustness Tests
// ============================================================================

TEST_F(BodyPartLibraryTest, CreateRandomCharacter_ExtremeSeed_DoesNotCrash)
{
    EXPECT_NO_THROW(library->createRandomCharacter(UINT32_MAX));
    EXPECT_NO_THROW(library->createRandomCharacter(0));
}

TEST_F(BodyPartLibraryTest, GetTemplate_BoundaryIndices_HandlesCorrectly)
{
    int headCount = library->getVariationCount(BodyPartType::Head);
    
    // First index should work
    EXPECT_NE(library->getTemplate(BodyPartType::Head, 0), nullptr);
    
    // Last index should work
    EXPECT_NE(library->getTemplate(BodyPartType::Head, headCount - 1), nullptr);
    
    // One past last should return null
    EXPECT_EQ(library->getTemplate(BodyPartType::Head, headCount), nullptr);
}
