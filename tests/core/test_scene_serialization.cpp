/**
 * @file test_scene_serialization.cpp
 * @brief Unit tests for scene serialization and deserialization
 */

#include <gtest/gtest.h>
#include "core/SceneManager.h"
#include <filesystem>
#include <fstream>

using namespace fresh;

class SceneSerializationTest : public ::testing::Test {
protected:
    SceneManager& manager = SceneManager::getInstance();
    std::string testScenePath;

    void SetUp() override {
        testScenePath = "test_scene.json";
    }

    void TearDown() override {
        // Clean up test files
        if (std::filesystem::exists(testScenePath)) {
            std::filesystem::remove(testScenePath);
        }
    }
};

TEST_F(SceneSerializationTest, CreateScene) {
    Scene* scene = manager.createScene("TestScene");
    ASSERT_NE(scene, nullptr);
    EXPECT_EQ(scene->getName(), "TestScene");
}

TEST_F(SceneSerializationTest, SaveEmptyScene) {
    Scene* scene = manager.createScene("EmptyScene");
    ASSERT_NE(scene, nullptr);
    
    bool result = manager.saveScene(testScenePath, scene);
    
#ifdef FRESH_JSON_AVAILABLE
    EXPECT_TRUE(result);
    EXPECT_TRUE(std::filesystem::exists(testScenePath));
#else
    EXPECT_FALSE(result);  // Should fail without JSON support
#endif
}

TEST_F(SceneSerializationTest, SaveAndLoadEmptyScene) {
#ifdef FRESH_JSON_AVAILABLE
    // Create and save scene
    Scene* originalScene = manager.createScene("SaveLoadTest");
    ASSERT_NE(originalScene, nullptr);
    
    bool saveResult = manager.saveScene(testScenePath, originalScene);
    ASSERT_TRUE(saveResult);

    // Unload original
    manager.unloadScene("SaveLoadTest");

    // Load scene back
    Scene* loadedScene = manager.loadScene(testScenePath);
    ASSERT_NE(loadedScene, nullptr);
    EXPECT_EQ(loadedScene->getName(), "SaveLoadTest");
#else
    GTEST_SKIP() << "Test requires FRESH_JSON_AVAILABLE";
#endif
}

TEST_F(SceneSerializationTest, SaveSceneWithNodes) {
#ifdef FRESH_JSON_AVAILABLE
    Scene* scene = manager.createScene("SceneWithNodes");
    ASSERT_NE(scene, nullptr);

    // Create a hierarchy
    auto child1 = std::make_shared<SceneNode>("Child1");
    child1->setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    child1->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    scene->addNode(child1);

    auto child2 = std::make_shared<SceneNode>("Child2");
    child2->setPosition(glm::vec3(10.0f, 20.0f, 30.0f));
    child2->setActive(false);
    scene->addNode(child2);

    // Save
    bool saveResult = manager.saveScene(testScenePath, scene);
    ASSERT_TRUE(saveResult);

    // Verify file was created
    EXPECT_TRUE(std::filesystem::exists(testScenePath));

    // Verify file is valid JSON
    std::ifstream file(testScenePath);
    ASSERT_TRUE(file.is_open());
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("SceneWithNodes"), std::string::npos);
    EXPECT_NE(content.find("Child1"), std::string::npos);
    EXPECT_NE(content.find("Child2"), std::string::npos);
#else
    GTEST_SKIP() << "Test requires FRESH_JSON_AVAILABLE";
#endif
}

TEST_F(SceneSerializationTest, SaveAndLoadSceneWithTransforms) {
#ifdef FRESH_JSON_AVAILABLE
    // Create scene with transformed nodes
    Scene* originalScene = manager.createScene("TransformTest");
    ASSERT_NE(originalScene, nullptr);

    auto node = std::make_shared<SceneNode>("TransformedNode");
    glm::vec3 testPosition(5.0f, 10.0f, 15.0f);
    glm::vec3 testScale(0.5f, 1.5f, 2.0f);
    
    node->setPosition(testPosition);
    node->setScale(testScale);
    originalScene->addNode(node);

    // Save
    bool saveResult = manager.saveScene(testScenePath, originalScene);
    ASSERT_TRUE(saveResult);

    // Unload and reload
    manager.unloadScene("TransformTest");
    Scene* loadedScene = manager.loadScene(testScenePath);
    ASSERT_NE(loadedScene, nullptr);

    // Find the node
    SceneNode* loadedNode = loadedScene->findNode("TransformedNode");
    ASSERT_NE(loadedNode, nullptr);

    // Verify transforms were preserved
    glm::vec3 loadedPosition = loadedNode->getPosition();
    EXPECT_NEAR(loadedPosition.x, testPosition.x, 0.001f);
    EXPECT_NEAR(loadedPosition.y, testPosition.y, 0.001f);
    EXPECT_NEAR(loadedPosition.z, testPosition.z, 0.001f);

    glm::vec3 loadedScale = loadedNode->getScale();
    EXPECT_NEAR(loadedScale.x, testScale.x, 0.001f);
    EXPECT_NEAR(loadedScale.y, testScale.y, 0.001f);
    EXPECT_NEAR(loadedScale.z, testScale.z, 0.001f);
#else
    GTEST_SKIP() << "Test requires FRESH_JSON_AVAILABLE";
#endif
}

TEST_F(SceneSerializationTest, SaveAndLoadNestedHierarchy) {
#ifdef FRESH_JSON_AVAILABLE
    Scene* scene = manager.createScene("HierarchyTest");
    ASSERT_NE(scene, nullptr);

    // Create nested hierarchy
    auto parent = std::make_shared<SceneNode>("Parent");
    auto child = std::make_shared<SceneNode>("Child");
    auto grandchild = std::make_shared<SceneNode>("Grandchild");

    parent->addChild(child);
    child->addChild(grandchild);
    scene->addNode(parent);

    // Save
    bool saveResult = manager.saveScene(testScenePath, scene);
    ASSERT_TRUE(saveResult);

    // Reload
    manager.unloadScene("HierarchyTest");
    Scene* loadedScene = manager.loadScene(testScenePath);
    ASSERT_NE(loadedScene, nullptr);

    // Verify hierarchy
    SceneNode* loadedParent = loadedScene->findNode("Parent");
    ASSERT_NE(loadedParent, nullptr);

    SceneNode* loadedChild = loadedScene->findNode("Child");
    ASSERT_NE(loadedChild, nullptr);
    EXPECT_EQ(loadedChild->getParent(), loadedParent);

    SceneNode* loadedGrandchild = loadedScene->findNode("Grandchild");
    ASSERT_NE(loadedGrandchild, nullptr);
    EXPECT_EQ(loadedGrandchild->getParent(), loadedChild);
#else
    GTEST_SKIP() << "Test requires FRESH_JSON_AVAILABLE";
#endif
}

TEST_F(SceneSerializationTest, SaveWithNullScene) {
    bool result = manager.saveScene(testScenePath, nullptr);
    EXPECT_FALSE(result);
}

TEST_F(SceneSerializationTest, LoadNonexistentFile) {
    Scene* scene = manager.loadScene("nonexistent_file.json");
    EXPECT_EQ(scene, nullptr);
}

TEST_F(SceneSerializationTest, ActiveStatePreservation) {
#ifdef FRESH_JSON_AVAILABLE
    Scene* scene = manager.createScene("ActiveStateTest");
    ASSERT_NE(scene, nullptr);

    // Create node with inactive state
    auto inactiveNode = std::make_shared<SceneNode>("InactiveNode");
    inactiveNode->setActive(false);
    scene->addNode(inactiveNode);

    // Create node with active state
    auto activeNode = std::make_shared<SceneNode>("ActiveNode");
    activeNode->setActive(true);
    scene->addNode(activeNode);

    // Save and reload
    manager.saveScene(testScenePath, scene);
    manager.unloadScene("ActiveStateTest");
    Scene* loadedScene = manager.loadScene(testScenePath);
    ASSERT_NE(loadedScene, nullptr);

    // Verify active states
    SceneNode* loadedInactive = loadedScene->findNode("InactiveNode");
    ASSERT_NE(loadedInactive, nullptr);
    EXPECT_FALSE(loadedInactive->isActive());

    SceneNode* loadedActive = loadedScene->findNode("ActiveNode");
    ASSERT_NE(loadedActive, nullptr);
    EXPECT_TRUE(loadedActive->isActive());
#else
    GTEST_SKIP() << "Test requires FRESH_JSON_AVAILABLE";
#endif
}
