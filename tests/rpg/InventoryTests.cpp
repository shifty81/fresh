#include <gtest/gtest.h>
#include "rpg/Inventory.h"

using namespace fresh::rpg;

class InventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = std::make_unique<Inventory>(1000.0f);
    }
    
    std::unique_ptr<Inventory> inventory;
};

// Constructor Tests
TEST_F(InventoryTest, Constructor_WithCapacity_SetsCorrectCapacity) {
    Inventory inv(500.0f);
    EXPECT_EQ(500.0f, inv.getCapacity());
    EXPECT_EQ(0.0f, inv.getTotalUsed());
}

TEST_F(InventoryTest, Constructor_DefaultCapacity_CorrectlyInitialized) {
    EXPECT_EQ(1000.0f, inventory->getCapacity());
    EXPECT_EQ(0.0f, inventory->getTotalUsed());
}

// Add Resource Tests
TEST_F(InventoryTest, AddResource_ValidAmount_AddsSuccessfully) {
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 100.0f));
    EXPECT_EQ(100.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, AddResource_MultipleTypes_AllStored) {
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 100.0f));
    EXPECT_TRUE(inventory->addResource(ResourceType::Titanium, 50.0f));
    EXPECT_TRUE(inventory->addResource(ResourceType::Energy, 200.0f));
    
    EXPECT_EQ(100.0f, inventory->getResourceAmount(ResourceType::Iron));
    EXPECT_EQ(50.0f, inventory->getResourceAmount(ResourceType::Titanium));
    EXPECT_EQ(200.0f, inventory->getResourceAmount(ResourceType::Energy));
}

TEST_F(InventoryTest, AddResource_ExceedsCapacity_Fails) {
    EXPECT_FALSE(inventory->addResource(ResourceType::Iron, 1500.0f));
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, AddResource_NegativeAmount_Fails) {
    EXPECT_FALSE(inventory->addResource(ResourceType::Iron, -100.0f));
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, AddResource_ZeroAmount_Fails) {
    EXPECT_FALSE(inventory->addResource(ResourceType::Iron, 0.0f));
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, AddResource_ToExisting_IncreasesAmount) {
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 100.0f));
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 50.0f));
    EXPECT_EQ(150.0f, inventory->getResourceAmount(ResourceType::Iron));
}

// Remove Resource Tests
TEST_F(InventoryTest, RemoveResource_ExistingResource_RemovesSuccessfully) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    EXPECT_TRUE(inventory->removeResource(ResourceType::Iron, 50.0f));
    EXPECT_EQ(50.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, RemoveResource_AllAmount_RemovesCompletely) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    EXPECT_TRUE(inventory->removeResource(ResourceType::Iron, 100.0f));
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, RemoveResource_InsufficientAmount_Fails) {
    inventory->addResource(ResourceType::Iron, 50.0f);
    EXPECT_FALSE(inventory->removeResource(ResourceType::Iron, 100.0f));
    EXPECT_EQ(50.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, RemoveResource_NonExistent_Fails) {
    EXPECT_FALSE(inventory->removeResource(ResourceType::Iron, 50.0f));
}

TEST_F(InventoryTest, RemoveResource_NegativeAmount_Fails) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    EXPECT_FALSE(inventory->removeResource(ResourceType::Iron, -50.0f));
    EXPECT_EQ(100.0f, inventory->getResourceAmount(ResourceType::Iron));
}

TEST_F(InventoryTest, RemoveResource_ZeroAmount_Fails) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    EXPECT_FALSE(inventory->removeResource(ResourceType::Iron, 0.0f));
    EXPECT_EQ(100.0f, inventory->getResourceAmount(ResourceType::Iron));
}

// Capacity Tests
TEST_F(InventoryTest, GetTotalUsed_EmptyInventory_ReturnsZero) {
    EXPECT_EQ(0.0f, inventory->getTotalUsed());
}

TEST_F(InventoryTest, GetTotalUsed_WithResources_ReturnsCorrectTotal) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    inventory->addResource(ResourceType::Titanium, 200.0f);
    inventory->addResource(ResourceType::Energy, 300.0f);
    EXPECT_EQ(600.0f, inventory->getTotalUsed());
}

TEST_F(InventoryTest, GetAvailableSpace_EmptyInventory_ReturnsFullCapacity) {
    EXPECT_EQ(1000.0f, inventory->getAvailableSpace());
}

TEST_F(InventoryTest, GetAvailableSpace_PartiallyFilled_ReturnsCorrectSpace) {
    inventory->addResource(ResourceType::Iron, 300.0f);
    EXPECT_EQ(700.0f, inventory->getAvailableSpace());
}

TEST_F(InventoryTest, IsFull_EmptyInventory_ReturnsFalse) {
    EXPECT_FALSE(inventory->isFull());
}

TEST_F(InventoryTest, IsFull_AtCapacity_ReturnsTrue) {
    inventory->addResource(ResourceType::Iron, 1000.0f);
    EXPECT_TRUE(inventory->isFull());
}

TEST_F(InventoryTest, IsFull_NearCapacity_ReturnsFalse) {
    inventory->addResource(ResourceType::Iron, 999.0f);
    EXPECT_FALSE(inventory->isFull());
}

TEST_F(InventoryTest, SetCapacity_LargerValue_UpdatesCorrectly) {
    inventory->setCapacity(2000.0f);
    EXPECT_EQ(2000.0f, inventory->getCapacity());
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 1500.0f));
}

TEST_F(InventoryTest, SetCapacity_SmallerValue_UpdatesCorrectly) {
    inventory->setCapacity(500.0f);
    EXPECT_EQ(500.0f, inventory->getCapacity());
    EXPECT_FALSE(inventory->addResource(ResourceType::Iron, 600.0f));
}

// Query Tests
TEST_F(InventoryTest, HasResource_Sufficient_ReturnsTrue) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    EXPECT_TRUE(inventory->hasResource(ResourceType::Iron, 50.0f));
    EXPECT_TRUE(inventory->hasResource(ResourceType::Iron, 100.0f));
}

TEST_F(InventoryTest, HasResource_Insufficient_ReturnsFalse) {
    inventory->addResource(ResourceType::Iron, 50.0f);
    EXPECT_FALSE(inventory->hasResource(ResourceType::Iron, 100.0f));
}

TEST_F(InventoryTest, HasResource_NonExistent_ReturnsFalse) {
    EXPECT_FALSE(inventory->hasResource(ResourceType::Iron, 10.0f));
}

TEST_F(InventoryTest, GetResourceAmount_NonExistent_ReturnsZero) {
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
}

// Clear Tests
TEST_F(InventoryTest, Clear_WithResources_RemovesAll) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    inventory->addResource(ResourceType::Titanium, 200.0f);
    inventory->clear();
    
    EXPECT_EQ(0.0f, inventory->getTotalUsed());
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Iron));
    EXPECT_EQ(0.0f, inventory->getResourceAmount(ResourceType::Titanium));
}

TEST_F(InventoryTest, Clear_EmptyInventory_DoesNotCrash) {
    inventory->clear();
    EXPECT_EQ(0.0f, inventory->getTotalUsed());
}

// GetAllResources Tests
TEST_F(InventoryTest, GetAllResources_EmptyInventory_ReturnsEmptyMap) {
    EXPECT_TRUE(inventory->getAllResources().empty());
}

TEST_F(InventoryTest, GetAllResources_WithResources_ReturnsCorrectMap) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    inventory->addResource(ResourceType::Titanium, 200.0f);
    
    const auto& resources = inventory->getAllResources();
    EXPECT_EQ(2u, resources.size());
    EXPECT_EQ(100.0f, resources.at(ResourceType::Iron));
    EXPECT_EQ(200.0f, resources.at(ResourceType::Titanium));
}

// Resource Type Name Tests
TEST(ResourceTypeTest, GetResourceName_AllTypes_ReturnsCorrectNames) {
    EXPECT_EQ("Iron", getResourceName(ResourceType::Iron));
    EXPECT_EQ("Titanium", getResourceName(ResourceType::Titanium));
    EXPECT_EQ("Naonite", getResourceName(ResourceType::Naonite));
    EXPECT_EQ("Trinium", getResourceName(ResourceType::Trinium));
    EXPECT_EQ("Xanion", getResourceName(ResourceType::Xanion));
    EXPECT_EQ("Ogonite", getResourceName(ResourceType::Ogonite));
    EXPECT_EQ("Avorion", getResourceName(ResourceType::Avorion));
    EXPECT_EQ("Energy", getResourceName(ResourceType::Energy));
    EXPECT_EQ("Credits", getResourceName(ResourceType::Credits));
}

// Edge Case Tests
TEST_F(InventoryTest, AddResource_FillToCapacity_Succeeds) {
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 1000.0f));
    EXPECT_TRUE(inventory->isFull());
    EXPECT_EQ(0.0f, inventory->getAvailableSpace());
}

TEST_F(InventoryTest, AddResource_MultipleTypesFillingCapacity_Succeeds) {
    EXPECT_TRUE(inventory->addResource(ResourceType::Iron, 400.0f));
    EXPECT_TRUE(inventory->addResource(ResourceType::Titanium, 300.0f));
    EXPECT_TRUE(inventory->addResource(ResourceType::Energy, 300.0f));
    EXPECT_TRUE(inventory->isFull());
}

TEST_F(InventoryTest, RemoveResource_RemovesEntryWhenZero_EntryRemoved) {
    inventory->addResource(ResourceType::Iron, 100.0f);
    inventory->removeResource(ResourceType::Iron, 100.0f);
    
    // Verify the resource is not in the map anymore
    const auto& resources = inventory->getAllResources();
    EXPECT_EQ(0u, resources.count(ResourceType::Iron));
}
