#include <gtest/gtest.h>

#include "character/AnimationClip.h"

using namespace fresh;

class AnimationClipTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        clip = std::make_unique<AnimationClip>("TestAnimation");
    }

    void TearDown() override
    {
        clip.reset();
    }

    std::unique_ptr<AnimationClip> clip;
};

// ============================================================================
// Constructor and Basic Tests
// ============================================================================

TEST_F(AnimationClipTest, Constructor_SetsName)
{
    EXPECT_EQ(clip->getName(), "TestAnimation");
}

TEST_F(AnimationClipTest, Constructor_InitialDurationIsZero)
{
    EXPECT_FLOAT_EQ(clip->getDuration(), 0.0f);
}

TEST_F(AnimationClipTest, Constructor_DefaultIsNotLooping)
{
    EXPECT_FALSE(clip->isLooping());
}

TEST_F(AnimationClipTest, SetLooping_UpdatesLoopingState)
{
    clip->setLooping(true);
    EXPECT_TRUE(clip->isLooping());
    
    clip->setLooping(false);
    EXPECT_FALSE(clip->isLooping());
}

// ============================================================================
// Keyframe Addition Tests
// ============================================================================

TEST_F(AnimationClipTest, AddKeyframe_SingleKeyframe_UpdatesDuration)
{
    AnimationKeyframe keyframe;
    keyframe.time = 1.0f;
    keyframe.boneRotations[0] = glm::vec3(0.5f, 0.0f, 0.0f);
    
    clip->addKeyframe(keyframe);
    
    EXPECT_FLOAT_EQ(clip->getDuration(), 1.0f);
}

TEST_F(AnimationClipTest, AddKeyframe_MultipleKeyframes_DurationIsMaxTime)
{
    AnimationKeyframe kf1;
    kf1.time = 0.5f;
    kf1.boneRotations[0] = glm::vec3(0.0f);
    
    AnimationKeyframe kf2;
    kf2.time = 1.5f;
    kf2.boneRotations[0] = glm::vec3(1.0f, 0.0f, 0.0f);
    
    AnimationKeyframe kf3;
    kf3.time = 2.0f;
    kf3.boneRotations[0] = glm::vec3(0.0f);
    
    clip->addKeyframe(kf1);
    clip->addKeyframe(kf2);
    clip->addKeyframe(kf3);
    
    EXPECT_FLOAT_EQ(clip->getDuration(), 2.0f);
}

// ============================================================================
// Sampling Tests
// ============================================================================

TEST_F(AnimationClipTest, SampleBone_NoKeyframes_ReturnsFalse)
{
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(0.5f, 0, rotation, position);
    
    EXPECT_FALSE(result);
}

TEST_F(AnimationClipTest, SampleBone_SingleKeyframe_ReturnsKeyframeValue)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[5] = glm::vec3(0.5f, 1.0f, 1.5f);
    kf.bonePositions[5] = glm::vec3(1.0f, 2.0f, 3.0f);
    
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(1.0f, 5, rotation, position);
    
    EXPECT_TRUE(result);
    EXPECT_FLOAT_EQ(rotation.x, 0.5f);
    EXPECT_FLOAT_EQ(rotation.y, 1.0f);
    EXPECT_FLOAT_EQ(rotation.z, 1.5f);
    EXPECT_FLOAT_EQ(position.x, 1.0f);
    EXPECT_FLOAT_EQ(position.y, 2.0f);
    EXPECT_FLOAT_EQ(position.z, 3.0f);
}

TEST_F(AnimationClipTest, SampleBone_BetweenKeyframes_InterpolatesValues)
{
    AnimationKeyframe kf1;
    kf1.time = 0.0f;
    kf1.boneRotations[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    kf1.bonePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    
    AnimationKeyframe kf2;
    kf2.time = 1.0f;
    kf2.boneRotations[0] = glm::vec3(2.0f, 4.0f, 6.0f);
    kf2.bonePositions[0] = glm::vec3(10.0f, 20.0f, 30.0f);
    
    clip->addKeyframe(kf1);
    clip->addKeyframe(kf2);
    
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(0.5f, 0, rotation, position);
    
    EXPECT_TRUE(result);
    
    // Should interpolate halfway between keyframes
    EXPECT_NEAR(rotation.x, 1.0f, 0.01f);
    EXPECT_NEAR(rotation.y, 2.0f, 0.01f);
    EXPECT_NEAR(rotation.z, 3.0f, 0.01f);
    EXPECT_NEAR(position.x, 5.0f, 0.01f);
    EXPECT_NEAR(position.y, 10.0f, 0.01f);
    EXPECT_NEAR(position.z, 15.0f, 0.01f);
}

TEST_F(AnimationClipTest, SampleBone_BoneNotInAnimation_ReturnsFalse)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[5] = glm::vec3(1.0f, 0.0f, 0.0f);
    
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(1.0f, 999, rotation, position); // Non-existent bone
    
    EXPECT_FALSE(result);
}

TEST_F(AnimationClipTest, SampleBone_TimeBeforeFirstKeyframe_ReturnsFirstKeyframe)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[0] = glm::vec3(1.0f, 2.0f, 3.0f);
    
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(0.0f, 0, rotation, position);
    
    EXPECT_TRUE(result);
    EXPECT_FLOAT_EQ(rotation.x, 1.0f);
    EXPECT_FLOAT_EQ(rotation.y, 2.0f);
    EXPECT_FLOAT_EQ(rotation.z, 3.0f);
}

TEST_F(AnimationClipTest, SampleBone_TimeAfterLastKeyframe_ReturnsLastKeyframe)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[0] = glm::vec3(5.0f, 6.0f, 7.0f);
    
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    bool result = clip->sampleBone(10.0f, 0, rotation, position);
    
    EXPECT_TRUE(result);
    EXPECT_FLOAT_EQ(rotation.x, 5.0f);
    EXPECT_FLOAT_EQ(rotation.y, 6.0f);
    EXPECT_FLOAT_EQ(rotation.z, 7.0f);
}

// ============================================================================
// AnimationClipFactory Tests
// ============================================================================

TEST_F(AnimationClipTest, Factory_CreateIdleAnimation_HasNonZeroDuration)
{
    auto idleClip = AnimationClipFactory::createIdleAnimation();
    EXPECT_GT(idleClip.getDuration(), 0.0f);
    EXPECT_TRUE(idleClip.isLooping());
}

TEST_F(AnimationClipTest, Factory_CreateWalkAnimation_HasNonZeroDuration)
{
    auto walkClip = AnimationClipFactory::createWalkAnimation();
    EXPECT_GT(walkClip.getDuration(), 0.0f);
    EXPECT_TRUE(walkClip.isLooping());
}

TEST_F(AnimationClipTest, Factory_CreateRunAnimation_HasNonZeroDuration)
{
    auto runClip = AnimationClipFactory::createRunAnimation();
    EXPECT_GT(runClip.getDuration(), 0.0f);
    EXPECT_TRUE(runClip.isLooping());
}

TEST_F(AnimationClipTest, Factory_CreateJumpAnimation_HasNonZeroDuration)
{
    auto jumpClip = AnimationClipFactory::createJumpAnimation();
    EXPECT_GT(jumpClip.getDuration(), 0.0f);
    EXPECT_FALSE(jumpClip.isLooping()); // Jump should not loop
}

TEST_F(AnimationClipTest, Factory_CreateCrouchAnimation_HasNonZeroDuration)
{
    auto crouchClip = AnimationClipFactory::createCrouchAnimation();
    EXPECT_GT(crouchClip.getDuration(), 0.0f);
}

TEST_F(AnimationClipTest, Factory_CreateAnimation_Idle_WorksCorrectly)
{
    auto idleClip = AnimationClipFactory::createAnimation(AnimationType::Idle);
    EXPECT_EQ(idleClip.getName(), "Idle");
    EXPECT_GT(idleClip.getDuration(), 0.0f);
}

TEST_F(AnimationClipTest, Factory_CreateAnimation_Walk_WorksCorrectly)
{
    auto walkClip = AnimationClipFactory::createAnimation(AnimationType::Walk);
    EXPECT_EQ(walkClip.getName(), "Walk");
    EXPECT_GT(walkClip.getDuration(), 0.0f);
}

TEST_F(AnimationClipTest, Factory_CreateAnimation_Run_WorksCorrectly)
{
    auto runClip = AnimationClipFactory::createAnimation(AnimationType::Run);
    EXPECT_EQ(runClip.getName(), "Run");
    EXPECT_GT(runClip.getDuration(), 0.0f);
}

TEST_F(AnimationClipTest, Factory_WalkAnimation_CanBeSampled)
{
    auto walkClip = AnimationClipFactory::createWalkAnimation();
    
    glm::vec3 rotation, position;
    // Sample at midpoint of animation
    float sampleTime = walkClip.getDuration() * 0.5f;
    
    // Try sampling a leg bone (legs should animate during walk)
    bool result = walkClip.sampleBone(sampleTime, 11, rotation, position); // LeftLeg bone
    
    // Animation should contain leg bone data
    EXPECT_TRUE(result) << "Walk animation should animate leg bones";
}

// ============================================================================
// Edge Cases and Robustness Tests
// ============================================================================

TEST_F(AnimationClipTest, AddKeyframe_NegativeTime_StillAdds)
{
    AnimationKeyframe kf;
    kf.time = -1.0f;
    kf.boneRotations[0] = glm::vec3(1.0f);
    
    EXPECT_NO_THROW(clip->addKeyframe(kf));
}

TEST_F(AnimationClipTest, SampleBone_NegativeTime_DoesNotCrash)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[0] = glm::vec3(1.0f);
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    EXPECT_NO_THROW(clip->sampleBone(-1.0f, 0, rotation, position));
}

TEST_F(AnimationClipTest, MultipleKeyframes_SameTime_DoesNotCrash)
{
    AnimationKeyframe kf1;
    kf1.time = 1.0f;
    kf1.boneRotations[0] = glm::vec3(1.0f);
    
    AnimationKeyframe kf2;
    kf2.time = 1.0f;
    kf2.boneRotations[0] = glm::vec3(2.0f);
    
    EXPECT_NO_THROW(clip->addKeyframe(kf1));
    EXPECT_NO_THROW(clip->addKeyframe(kf2));
}

TEST_F(AnimationClipTest, KeyframeWithMultipleBones_SamplesCorrectly)
{
    AnimationKeyframe kf;
    kf.time = 1.0f;
    kf.boneRotations[0] = glm::vec3(1.0f, 0.0f, 0.0f);
    kf.boneRotations[1] = glm::vec3(0.0f, 2.0f, 0.0f);
    kf.boneRotations[2] = glm::vec3(0.0f, 0.0f, 3.0f);
    
    clip->addKeyframe(kf);
    
    glm::vec3 rotation, position;
    
    EXPECT_TRUE(clip->sampleBone(1.0f, 0, rotation, position));
    EXPECT_FLOAT_EQ(rotation.x, 1.0f);
    
    EXPECT_TRUE(clip->sampleBone(1.0f, 1, rotation, position));
    EXPECT_FLOAT_EQ(rotation.y, 2.0f);
    
    EXPECT_TRUE(clip->sampleBone(1.0f, 2, rotation, position));
    EXPECT_FLOAT_EQ(rotation.z, 3.0f);
}
