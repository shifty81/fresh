/**
 * @file BoneTests.cpp
 * @brief Unit tests for Bone class
 */

#include <gtest/gtest.h>
#include "character/Bone.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace fresh {

class BoneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple bone hierarchy for testing
        rootBone = std::make_unique<Bone>("root");
        childBone = std::make_unique<Bone>("child", rootBone.get());
        grandchildBone = std::make_unique<Bone>("grandchild", childBone.get());
    }

    void TearDown() override {
        grandchildBone.reset();
        childBone.reset();
        rootBone.reset();
    }

    std::unique_ptr<Bone> rootBone;
    std::unique_ptr<Bone> childBone;
    std::unique_ptr<Bone> grandchildBone;
};

TEST_F(BoneTest, ConstructorInitialization) {
    EXPECT_EQ(rootBone->getName(), "root");
    EXPECT_EQ(rootBone->getParent(), nullptr);
    EXPECT_EQ(childBone->getParent(), rootBone.get());
}

TEST_F(BoneTest, LocalTransform) {
    rootBone->setLocalPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    glm::vec3 pos = rootBone->getLocalPosition();
    
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 3.0f);
}

TEST_F(BoneTest, WorldPosition_SingleBone) {
    rootBone->setLocalPosition(glm::vec3(5.0f, 10.0f, 15.0f));
    glm::vec3 worldPos = rootBone->getWorldPosition();
    
    // Root bone's world position should equal local position
    EXPECT_FLOAT_EQ(worldPos.x, 5.0f);
    EXPECT_FLOAT_EQ(worldPos.y, 10.0f);
    EXPECT_FLOAT_EQ(worldPos.z, 15.0f);
}

TEST_F(BoneTest, WorldPosition_BoneHierarchy) {
    // Set up bone hierarchy positions
    rootBone->setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    childBone->setLocalPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    grandchildBone->setLocalPosition(glm::vec3(0.0f, 3.0f, 0.0f));
    
    // Grandchild's world position should be sum of all parent positions
    glm::vec3 worldPos = grandchildBone->getWorldPosition();
    EXPECT_FLOAT_EQ(worldPos.y, 8.0f); // 0 + 5 + 3
}

TEST_F(BoneTest, BoneLength) {
    childBone->setLocalPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    float length = rootBone->getLength();
    
    // Length should be distance to first child
    EXPECT_FLOAT_EQ(length, 5.0f);
}

TEST_F(BoneTest, BoneLength_NoChi ldren) {
    auto singleBone = std::make_unique<Bone>("single");
    float length = singleBone->getLength();
    
    // Default length for bones with no children
    EXPECT_FLOAT_EQ(length, 1.0f);
}

TEST_F(BoneTest, AddRemoveChildren) {
    auto newChild = std::make_unique<Bone>("newchild");
    size_t initialChildCount = rootBone->getChildren().size();
    
    rootBone->addChild(newChild.get());
    EXPECT_EQ(rootBone->getChildren().size(), initialChildCount + 1);
    
    rootBone->removeChild(newChild.get());
    EXPECT_EQ(rootBone->getChildren().size(), initialChildCount);
}

TEST_F(BoneTest, TransformMatrix) {
    rootBone->setLocalPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    rootBone->setLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));
    
    glm::mat4 transform = rootBone->getLocalTransform();
    
    // Verify translation component
    EXPECT_FLOAT_EQ(transform[3][0], 1.0f);
    EXPECT_FLOAT_EQ(transform[3][1], 2.0f);
    EXPECT_FLOAT_EQ(transform[3][2], 3.0f);
}

TEST_F(BoneTest, RotationQuaternion) {
    // Set a 90-degree rotation around Y axis
    glm::quat rotation = glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    rootBone->setLocalRotation(rotation);
    
    glm::quat retrieved = rootBone->getLocalRotation();
    
    EXPECT_NEAR(retrieved.w, rotation.w, 0.001f);
    EXPECT_NEAR(retrieved.x, rotation.x, 0.001f);
    EXPECT_NEAR(retrieved.y, rotation.y, 0.001f);
    EXPECT_NEAR(retrieved.z, rotation.z, 0.001f);
}

} // namespace fresh
