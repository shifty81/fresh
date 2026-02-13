/**
 * @file EngineSystemTests.cpp
 * @brief Tests for the pluggable IEngineSystem interface and EngineSystemRegistry
 *
 * These tests verify that game systems can be registered, discovered, and
 * managed through the EngineSystemRegistry, enabling the editor to treat
 * gameplay features as pluggable modules rather than hard-wired subsystems.
 */

#include <gtest/gtest.h>
#include "core/IEngineSystem.h"

namespace fresh {

// ---------------------------------------------------------------------------
// Concrete test system used by several tests
// ---------------------------------------------------------------------------
class TestSystem : public IEngineSystem {
public:
    explicit TestSystem(const std::string& id, bool editorSys = false, bool playSys = true)
        : m_id(id), m_editorSys(editorSys), m_playSys(playSys) {}

    EngineSystemInfo getInfo() const override {
        return {m_id, "Test " + m_id, "test system", "1.0.0", m_editorSys, m_playSys};
    }
    bool initialize() override { m_initialized = true; return true; }
    void shutdown() override   { m_shutDown = true; }
    void update(float dt) override { m_totalTime += dt; ++m_updateCount; }
    void setEnabled(bool e) override { m_enabled = e; }
    bool isEnabled() const override { return m_enabled; }

    bool m_initialized = false;
    bool m_shutDown = false;
    bool m_enabled = true;
    float m_totalTime = 0.0f;
    int m_updateCount = 0;

private:
    std::string m_id;
    bool m_editorSys;
    bool m_playSys;
};

// A system whose initialize() deliberately fails
class FailingSystem : public IEngineSystem {
public:
    EngineSystemInfo getInfo() const override {
        return {"fail", "Failing System", "", "1.0.0", false, true};
    }
    bool initialize() override { return false; }
    void shutdown() override {}
    void update(float) override {}
    void setEnabled(bool e) override { m_enabled = e; }
    bool isEnabled() const override { return m_enabled; }
    bool m_enabled = true;
};

// ---------------------------------------------------------------------------
// Test fixture
// ---------------------------------------------------------------------------
class EngineSystemRegistryTest : public ::testing::Test {
protected:
    EngineSystemRegistry registry;
};

// ---- Registration tests ---------------------------------------------------

TEST_F(EngineSystemRegistryTest, RegisterSystem_Succeeds) {
    auto sys = std::make_unique<TestSystem>("physics");
    EXPECT_TRUE(registry.registerSystem(std::move(sys)));
}

TEST_F(EngineSystemRegistryTest, RegisterNull_Fails) {
    EXPECT_FALSE(registry.registerSystem(nullptr));
}

TEST_F(EngineSystemRegistryTest, RegisterDuplicateId_Fails) {
    registry.registerSystem(std::make_unique<TestSystem>("physics"));
    EXPECT_FALSE(registry.registerSystem(std::make_unique<TestSystem>("physics")));
}

TEST_F(EngineSystemRegistryTest, GetSystem_ReturnsRegistered) {
    auto* raw = new TestSystem("ai");
    registry.registerSystem(std::unique_ptr<TestSystem>(raw));
    EXPECT_EQ(registry.getSystem("ai"), raw);
}

TEST_F(EngineSystemRegistryTest, GetSystem_UnknownId_ReturnsNull) {
    EXPECT_EQ(registry.getSystem("nonexistent"), nullptr);
}

TEST_F(EngineSystemRegistryTest, GetSystems_ReturnsList) {
    registry.registerSystem(std::make_unique<TestSystem>("a"));
    registry.registerSystem(std::make_unique<TestSystem>("b"));
    EXPECT_EQ(registry.getSystems().size(), 2u);
}

// ---- Lifecycle tests ------------------------------------------------------

TEST_F(EngineSystemRegistryTest, InitializeAll_CallsEachSystem) {
    registry.registerSystem(std::make_unique<TestSystem>("a"));
    registry.registerSystem(std::make_unique<TestSystem>("b"));
    EXPECT_TRUE(registry.initializeAll());

    for (auto& s : registry.getSystems()) {
        auto* ts = dynamic_cast<TestSystem*>(s.get());
        ASSERT_NE(ts, nullptr);
        EXPECT_TRUE(ts->m_initialized);
    }
}

TEST_F(EngineSystemRegistryTest, InitializeAll_StopsOnFailure) {
    registry.registerSystem(std::make_unique<FailingSystem>());
    EXPECT_FALSE(registry.initializeAll());
}

TEST_F(EngineSystemRegistryTest, ShutdownAll_CallsInReverseOrder) {
    registry.registerSystem(std::make_unique<TestSystem>("first"));
    registry.registerSystem(std::make_unique<TestSystem>("second"));
    registry.shutdownAll();

    for (auto& s : registry.getSystems()) {
        auto* ts = dynamic_cast<TestSystem*>(s.get());
        ASSERT_NE(ts, nullptr);
        EXPECT_TRUE(ts->m_shutDown);
    }
}

// ---- Update tests ---------------------------------------------------------

TEST_F(EngineSystemRegistryTest, UpdateAll_PlayMode_OnlyUpdatesPlaySystems) {
    // play-mode-only system
    auto play = std::make_unique<TestSystem>("play", /*editor=*/false, /*play=*/true);
    auto* playPtr = play.get();
    registry.registerSystem(std::move(play));

    // editor-only system
    auto editor = std::make_unique<TestSystem>("editor", /*editor=*/true, /*play=*/false);
    auto* editorPtr = editor.get();
    registry.registerSystem(std::move(editor));

    registry.updateAll(0.016f, /*isPlayMode=*/true);

    EXPECT_EQ(playPtr->m_updateCount, 1);
    EXPECT_EQ(editorPtr->m_updateCount, 0);
}

TEST_F(EngineSystemRegistryTest, UpdateAll_EditorMode_OnlyUpdatesEditorSystems) {
    auto play = std::make_unique<TestSystem>("play", false, true);
    auto* playPtr = play.get();
    registry.registerSystem(std::move(play));

    auto editor = std::make_unique<TestSystem>("editor", true, false);
    auto* editorPtr = editor.get();
    registry.registerSystem(std::move(editor));

    registry.updateAll(0.016f, /*isPlayMode=*/false);

    EXPECT_EQ(playPtr->m_updateCount, 0);
    EXPECT_EQ(editorPtr->m_updateCount, 1);
}

TEST_F(EngineSystemRegistryTest, UpdateAll_DisabledSystemSkipped) {
    auto sys = std::make_unique<TestSystem>("disabled", false, true);
    auto* sysPtr = sys.get();
    sysPtr->setEnabled(false);
    registry.registerSystem(std::move(sys));

    registry.updateAll(0.016f, true);
    EXPECT_EQ(sysPtr->m_updateCount, 0);
}

TEST_F(EngineSystemRegistryTest, UpdateAll_DualModeSystem_UpdatesInBothModes) {
    auto sys = std::make_unique<TestSystem>("dual", /*editor=*/true, /*play=*/true);
    auto* sysPtr = sys.get();
    registry.registerSystem(std::move(sys));

    registry.updateAll(0.016f, true);   // play mode
    registry.updateAll(0.016f, false);  // editor mode

    EXPECT_EQ(sysPtr->m_updateCount, 2);
}

// ---- IEngineSystem interface tests ----------------------------------------

TEST(EngineSystemInfoTest, MetadataFieldsPopulated) {
    TestSystem sys("my_system", true, false);
    auto info = sys.getInfo();

    EXPECT_EQ(info.id, "my_system");
    EXPECT_EQ(info.name, "Test my_system");
    EXPECT_TRUE(info.isEditorSystem);
    EXPECT_FALSE(info.isPlayModeSystem);
}

TEST(EngineSystemInterfaceTest, EnableDisableToggle) {
    TestSystem sys("toggle");
    EXPECT_TRUE(sys.isEnabled());

    sys.setEnabled(false);
    EXPECT_FALSE(sys.isEnabled());

    sys.setEnabled(true);
    EXPECT_TRUE(sys.isEnabled());
}

} // namespace fresh
