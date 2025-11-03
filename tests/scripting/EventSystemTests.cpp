/**
 * @file EventSystemTests.cpp
 * @brief Unit tests for the EventSystem
 */

#include <gtest/gtest.h>
#include "scripting/EventSystem.h"
#include <thread>
#include <chrono>

using namespace fresh;

/**
 * Test fixture for EventSystem tests
 */
class EventSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        EventSystem::getInstance().clear();
    }

    void TearDown() override {
        EventSystem::getInstance().clear();
    }
};

/**
 * Test EventData storage and retrieval
 */
TEST_F(EventSystemTest, EventData_SetAndGet_Int_ReturnsCorrectValue) {
    // Arrange
    EventData data;
    int expected = 42;
    
    // Act
    data.set("value", expected);
    int actual = data.get<int>("value");
    
    // Assert
    EXPECT_EQ(actual, expected);
}

TEST_F(EventSystemTest, EventData_SetAndGet_String_ReturnsCorrectValue) {
    // Arrange
    EventData data;
    std::string expected = "test string";
    
    // Act
    data.set("message", expected);
    std::string actual = data.get<std::string>("message");
    
    // Assert
    EXPECT_EQ(actual, expected);
}

TEST_F(EventSystemTest, EventData_SetAndGet_Float_ReturnsCorrectValue) {
    // Arrange
    EventData data;
    float expected = 3.14159f;
    
    // Act
    data.set("pi", expected);
    float actual = data.get<float>("pi");
    
    // Assert
    EXPECT_FLOAT_EQ(actual, expected);
}

TEST_F(EventSystemTest, EventData_SetAndGet_Bool_ReturnsCorrectValue) {
    // Arrange
    EventData data;
    
    // Act
    data.set("flag", true);
    bool actual = data.get<bool>("flag");
    
    // Assert
    EXPECT_TRUE(actual);
}

TEST_F(EventSystemTest, EventData_GetNonExistent_ReturnsDefault) {
    // Arrange
    EventData data;
    int defaultValue = 999;
    
    // Act
    int actual = data.get<int>("nonexistent", defaultValue);
    
    // Assert
    EXPECT_EQ(actual, defaultValue);
}

TEST_F(EventSystemTest, EventData_Has_ExistingKey_ReturnsTrue) {
    // Arrange
    EventData data;
    data.set("key", 123);
    
    // Act & Assert
    EXPECT_TRUE(data.has("key"));
}

TEST_F(EventSystemTest, EventData_Has_NonExistentKey_ReturnsFalse) {
    // Arrange
    EventData data;
    
    // Act & Assert
    EXPECT_FALSE(data.has("nonexistent"));
}

TEST_F(EventSystemTest, EventData_MultipleValues_AllStored) {
    // Arrange
    EventData data;
    
    // Act
    data.set("int", 42);
    data.set("string", std::string("test"));
    data.set("float", 3.14f);
    data.set("bool", true);
    
    // Assert
    EXPECT_EQ(data.get<int>("int"), 42);
    EXPECT_EQ(data.get<std::string>("string"), "test");
    EXPECT_FLOAT_EQ(data.get<float>("float"), 3.14f);
    EXPECT_TRUE(data.get<bool>("bool"));
}

/**
 * Test event subscription
 */
TEST_F(EventSystemTest, Subscribe_ValidCallback_ReturnsSubscriptionID) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    // Act
    int subID = eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    // Assert
    EXPECT_GT(subID, 0);
}

TEST_F(EventSystemTest, Subscribe_MultipleCallbacks_ReturnsDifferentIDs) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    
    // Act
    int sub1 = eventSys.subscribe("event1", [](const EventData&) {});
    int sub2 = eventSys.subscribe("event2", [](const EventData&) {});
    
    // Assert
    EXPECT_NE(sub1, sub2);
}

/**
 * Test event emission
 */
TEST_F(EventSystemTest, Emit_WithSubscriber_CallsCallback) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    // Act
    eventSys.emit("test_event");
    
    // Assert
    EXPECT_TRUE(callbackCalled);
}

TEST_F(EventSystemTest, Emit_WithoutSubscriber_DoesNotCrash) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(eventSys.emit("nonexistent_event"));
}

TEST_F(EventSystemTest, Emit_WithData_CallbackReceivesData) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int receivedValue = 0;
    
    eventSys.subscribe("test_event", [&](const EventData& data) {
        receivedValue = data.get<int>("value", -1);
    });
    
    EventData data;
    data.set("value", 42);
    
    // Act
    eventSys.emit("test_event", data);
    
    // Assert
    EXPECT_EQ(receivedValue, 42);
}

TEST_F(EventSystemTest, Emit_MultipleSubscribers_AllCalled) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int callCount = 0;
    
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    
    // Act
    eventSys.emit("test_event");
    
    // Assert
    EXPECT_EQ(callCount, 3);
}

/**
 * Test event unsubscription
 */
TEST_F(EventSystemTest, Unsubscribe_ValidID_RemovesCallback) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    int subID = eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    // Act
    eventSys.unsubscribe(subID);
    eventSys.emit("test_event");
    
    // Assert
    EXPECT_FALSE(callbackCalled);
}

TEST_F(EventSystemTest, Unsubscribe_InvalidID_DoesNotCrash) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    
    // Act & Assert - should not crash
    EXPECT_NO_THROW(eventSys.unsubscribe(99999));
}

TEST_F(EventSystemTest, UnsubscribeAll_RemovesAllCallbacks) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int callCount = 0;
    
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    
    // Act
    eventSys.unsubscribeAll("test_event");
    eventSys.emit("test_event");
    
    // Assert
    EXPECT_EQ(callCount, 0);
}

/**
 * Test queued events
 */
TEST_F(EventSystemTest, QueueEvent_NotProcessed_DoesNotCallCallback) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    // Act
    eventSys.queueEvent("test_event");
    
    // Assert
    EXPECT_FALSE(callbackCalled);
}

TEST_F(EventSystemTest, ProcessEvents_QueuedEvent_CallsCallback) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    eventSys.queueEvent("test_event");
    
    // Act
    eventSys.processEvents();
    
    // Assert
    EXPECT_TRUE(callbackCalled);
}

TEST_F(EventSystemTest, ProcessEvents_MultipleQueuedEvents_AllProcessed) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int callCount = 0;
    
    eventSys.subscribe("event1", [&](const EventData&) { callCount++; });
    eventSys.subscribe("event2", [&](const EventData&) { callCount++; });
    eventSys.subscribe("event3", [&](const EventData&) { callCount++; });
    
    eventSys.queueEvent("event1");
    eventSys.queueEvent("event2");
    eventSys.queueEvent("event3");
    
    // Act
    eventSys.processEvents();
    
    // Assert
    EXPECT_EQ(callCount, 3);
}

TEST_F(EventSystemTest, ProcessEvents_QueuedEventWithData_DataReceived) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int receivedValue = 0;
    
    eventSys.subscribe("test_event", [&](const EventData& data) {
        receivedValue = data.get<int>("value", -1);
    });
    
    EventData data;
    data.set("value", 123);
    eventSys.queueEvent("test_event", data);
    
    // Act
    eventSys.processEvents();
    
    // Assert
    EXPECT_EQ(receivedValue, 123);
}

TEST_F(EventSystemTest, ProcessEvents_CalledTwice_DoesNotReprocessEvents) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    int callCount = 0;
    
    eventSys.subscribe("test_event", [&](const EventData&) { callCount++; });
    eventSys.queueEvent("test_event");
    
    // Act
    eventSys.processEvents();
    eventSys.processEvents(); // Should not process again
    
    // Assert
    EXPECT_EQ(callCount, 1);
}

/**
 * Test clear functionality
 */
TEST_F(EventSystemTest, Clear_RemovesAllSubscriptions) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    // Act
    eventSys.clear();
    eventSys.emit("test_event");
    
    // Assert
    EXPECT_FALSE(callbackCalled);
}

TEST_F(EventSystemTest, Clear_RemovesQueuedEvents) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool callbackCalled = false;
    
    eventSys.subscribe("test_event", [&](const EventData&) {
        callbackCalled = true;
    });
    
    eventSys.queueEvent("test_event");
    
    // Act
    eventSys.clear();
    eventSys.processEvents();
    
    // Assert
    EXPECT_FALSE(callbackCalled);
}

/**
 * Test event ordering
 */
TEST_F(EventSystemTest, MultipleEvents_ProcessedInOrder) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    std::vector<int> order;
    
    eventSys.subscribe("event1", [&](const EventData&) { order.push_back(1); });
    eventSys.subscribe("event2", [&](const EventData&) { order.push_back(2); });
    eventSys.subscribe("event3", [&](const EventData&) { order.push_back(3); });
    
    // Act
    eventSys.queueEvent("event1");
    eventSys.queueEvent("event2");
    eventSys.queueEvent("event3");
    eventSys.processEvents();
    
    // Assert
    ASSERT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
}

/**
 * Test complex event data
 */
TEST_F(EventSystemTest, EventData_ComplexStructure_StoresCorrectly) {
    // Arrange
    EventData data;
    
    struct Position {
        float x, y, z;
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
    
    Position pos{1.0f, 2.0f, 3.0f};
    
    // Act
    data.set("position", pos);
    Position retrieved = data.get<Position>("position");
    
    // Assert
    EXPECT_EQ(retrieved, pos);
}

/**
 * Test singleton pattern
 */
TEST_F(EventSystemTest, GetInstance_CalledTwice_ReturnsSameInstance) {
    // Act
    auto& instance1 = EventSystem::getInstance();
    auto& instance2 = EventSystem::getInstance();
    
    // Assert - both should be the same instance
    EXPECT_EQ(&instance1, &instance2);
}

/**
 * Test predefined event constants
 */
TEST_F(EventSystemTest, PredefinedEvents_DefinedCorrectly) {
    // Assert
    EXPECT_STREQ(Events::PLAYER_SPAWNED, "player_spawned");
    EXPECT_STREQ(Events::PLAYER_DIED, "player_died");
    EXPECT_STREQ(Events::CHUNK_LOADED, "chunk_loaded");
    EXPECT_STREQ(Events::CHUNK_UNLOADED, "chunk_unloaded");
    EXPECT_STREQ(Events::BLOCK_PLACED, "block_placed");
    EXPECT_STREQ(Events::BLOCK_BROKEN, "block_broken");
    EXPECT_STREQ(Events::ITEM_PICKED_UP, "item_picked_up");
    EXPECT_STREQ(Events::ITEM_USED, "item_used");
    EXPECT_STREQ(Events::GAME_PAUSED, "game_paused");
    EXPECT_STREQ(Events::GAME_RESUMED, "game_resumed");
    EXPECT_STREQ(Events::SCENE_LOADED, "scene_loaded");
    EXPECT_STREQ(Events::RESOURCE_LOADED, "resource_loaded");
}

/**
 * Test using predefined event types
 */
TEST_F(EventSystemTest, PredefinedEvent_PLAYER_SPAWNED_Works) {
    // Arrange
    auto& eventSys = EventSystem::getInstance();
    bool called = false;
    
    eventSys.subscribe(Events::PLAYER_SPAWNED, [&](const EventData&) {
        called = true;
    });
    
    // Act
    eventSys.emit(Events::PLAYER_SPAWNED);
    
    // Assert
    EXPECT_TRUE(called);
}
