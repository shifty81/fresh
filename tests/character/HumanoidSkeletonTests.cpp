/**
 * @file HumanoidSkeletonTests.cpp
 * @brief Unit tests for HumanoidSkeleton class
 */

#include <gtest/gtest.h>
#include "character/HumanoidSkeleton.h"
#include <glm/glm.hpp>

namespace fresh {

class HumanoidSkeletonTest : public ::testing::Test {
protected:
    void SetUp() override {
        skeleton = std::make_unique<HumanoidSkeleton>();
        skeleton->initialize(16.0f);
    }

    void TearDown() override {
        skeleton.reset();
    }

    std::unique_ptr<HumanoidSkeleton> skeleton;
};

TEST_F(HumanoidSkeletonTest, Initialization) {
    ASSERT_NE(skeleton->getRootBone(), nullptr);
    EXPECT_EQ(skeleton->getRootBone()->getName(), "root");
}

TEST_F(HumanoidSkeletonTest, AllBonesExist) {
    // Test that all 18 bones exist
    EXPECT_NE(skeleton->getBone("root"), nullptr);
    EXPECT_NE(skeleton->getBone("spine"), nullptr);
    EXPECT_NE(skeleton->getBone("chest"), nullptr);
    EXPECT_NE(skeleton->getBone("head"), nullptr);
    
    EXPECT_NE(skeleton->getBone("left_shoulder"), nullptr);
    EXPECT_NE(skeleton->getBone("left_upper_arm"), nullptr);
    EXPECT_NE(skeleton->getBone("left_lower_arm"), nullptr);
    EXPECT_NE(skeleton->getBone("left_hand"), nullptr);
    
    EXPECT_NE(skeleton->getBone("right_shoulder"), nullptr);
    EXPECT_NE(skeleton->getBone("right_upper_arm"), nullptr);
    EXPECT_NE(skeleton->getBone("right_lower_arm"), nullptr);
    EXPECT_NE(skeleton->getBone("right_hand"), nullptr);
    
    EXPECT_NE(skeleton->getBone("left_upper_leg"), nullptr);
    EXPECT_NE(skeleton->getBone("left_lower_leg"), nullptr);
    EXPECT_NE(skeleton->getBone("left_foot"), nullptr);
    
    EXPECT_NE(skeleton->getBone("right_upper_leg"), nullptr);
    EXPECT_NE(skeleton->getBone("right_lower_leg"), nullptr);
    EXPECT_NE(skeleton->getBone("right_foot"), nullptr);
}

TEST_F(HumanoidSkeletonTest, BoneHierarchy) {
    Bone* spine = skeleton->getBone("spine");
    ASSERT_NE(spine, nullptr);
    EXPECT_EQ(spine->getParent(), skeleton->getRootBone());
    
    Bone* chest = skeleton->getBone("chest");
    ASSERT_NE(chest, nullptr);
    EXPECT_EQ(chest->getParent(), spine);
    
    Bone* head = skeleton->getBone("head");
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->getParent(), chest);
}

TEST_F(HumanoidSkeletonTest, ArmHierarchy) {
    Bone* leftShoulder = skeleton->getBone("left_shoulder");
    Bone* leftUpperArm = skeleton->getBone("left_upper_arm");
    Bone* leftLowerArm = skeleton->getBone("left_lower_arm");
    Bone* leftHand = skeleton->getBone("left_hand");
    
    EXPECT_EQ(leftUpperArm->getParent(), leftShoulder);
    EXPECT_EQ(leftLowerArm->getParent(), leftUpperArm);
    EXPECT_EQ(leftHand->getParent(), leftLowerArm);
}

TEST_F(HumanoidSkeletonTest, LegHierarchy) {
    Bone* leftUpperLeg = skeleton->getBone("left_upper_leg");
    Bone* leftLowerLeg = skeleton->getBone("left_lower_leg");
    Bone* leftFoot = skeleton->getBone("left_foot");
    
    EXPECT_EQ(leftUpperLeg->getParent(), skeleton->getRootBone());
    EXPECT_EQ(leftLowerLeg->getParent(), leftUpperLeg);
    EXPECT_EQ(leftFoot->getParent(), leftLowerLeg);
}

TEST_F(HumanoidSkeletonTest, GetBoneByEnum) {
    Bone* root = skeleton->getBone(HumanoidSkeleton::BoneName::Root);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getName(), "root");
    
    Bone* head = skeleton->getBone(HumanoidSkeleton::BoneName::Head);
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->getName(), "head");
}

TEST_F(HumanoidSkeletonTest, InvalidBoneName) {
    Bone* invalidBone = skeleton->getBone("nonexistent_bone");
    EXPECT_EQ(invalidBone, nullptr);
}

TEST_F(HumanoidSkeletonTest, ResetTPose) {
    // Modify a bone rotation
    Bone* leftArm = skeleton->getBone("left_upper_arm");
    ASSERT_NE(leftArm, nullptr);
    leftArm->setLocalRotation(glm::quat(0.7f, 0.7f, 0.0f, 0.0f));
    
    // Reset to T-pose
    skeleton->resetToPose();
    
    // Check that rotation is identity
    glm::quat rotation = leftArm->getLocalRotation();
    EXPECT_FLOAT_EQ(rotation.w, 1.0f);
    EXPECT_FLOAT_EQ(rotation.x, 0.0f);
    EXPECT_FLOAT_EQ(rotation.y, 0.0f);
    EXPECT_FLOAT_EQ(rotation.z, 0.0f);
}

TEST_F(HumanoidSkeletonTest, CustomHeight) {
    auto tallSkeleton = std::make_unique<HumanoidSkeleton>();
    tallSkeleton->initialize(32.0f); // Double height
    
    Bone* spine = tallSkeleton->getBone("spine");
    ASSERT_NE(spine, nullptr);
    
    // Spine position should be scaled proportionally
    glm::vec3 spinePos = spine->getLocalPosition();
    EXPECT_GT(spinePos.y, 0.0f);
}

TEST_F(HumanoidSkeletonTest, BoneSymmetry) {
    Bone* leftArm = skeleton->getBone("left_upper_arm");
    Bone* rightArm = skeleton->getBone("right_upper_arm");
    
    ASSERT_NE(leftArm, nullptr);
    ASSERT_NE(rightArm, nullptr);
    
    // Arms should be symmetric (opposite X, same Y)
    glm::vec3 leftPos = leftArm->getLocalPosition();
    glm::vec3 rightPos = rightArm->getLocalPosition();
    
    EXPECT_FLOAT_EQ(std::abs(leftPos.x), std::abs(rightPos.x));
    EXPECT_FLOAT_EQ(leftPos.y, rightPos.y);
}

TEST_F(HumanoidSkeletonTest, GetAllBones) {
    const auto& allBones = skeleton->getAllBones();
    
    // Should have 18 bones
    EXPECT_EQ(allBones.size(), 18);
}

} // namespace fresh
