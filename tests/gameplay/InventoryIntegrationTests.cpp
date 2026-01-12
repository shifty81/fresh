#include <gtest/gtest.h>
#include "gameplay/InventoryManager.h"
#include "ui/CraftingPanel.h"
#include "rpg/CraftingSystem.h"

namespace fresh
{
namespace
{

class InventoryIntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create inventory manager
        inventoryManager = std::make_unique<InventoryManager>(1000.0f);
        ASSERT_TRUE(inventoryManager->initialize());

        // Create crafting system and panel
        craftingSystem = std::make_unique<rpg::CraftingSystem>();
        craftingPanel = std::make_unique<CraftingPanel>();
        ASSERT_TRUE(craftingPanel->initialize(inventoryManager.get()));
        craftingPanel->setCraftingSystem(craftingSystem.get());
    }

    std::unique_ptr<InventoryManager> inventoryManager;
    std::unique_ptr<rpg::CraftingSystem> craftingSystem;
    std::unique_ptr<CraftingPanel> craftingPanel;
};

TEST_F(InventoryIntegrationTest, AddAndRetrieveItems)
{
    // Add items to inventory
    EXPECT_TRUE(inventoryManager->addItem(rpg::ResourceType::Iron, 100.0f));
    EXPECT_TRUE(inventoryManager->addItem(rpg::ResourceType::Titanium, 50.0f));

    // Check amounts
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 100.0f);
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Titanium), 50.0f);

    // Check has item
    EXPECT_TRUE(inventoryManager->hasItem(rpg::ResourceType::Iron, 50.0f));
    EXPECT_FALSE(inventoryManager->hasItem(rpg::ResourceType::Iron, 150.0f));
}

TEST_F(InventoryIntegrationTest, CapacityManagement)
{
    // Fill inventory to capacity
    EXPECT_TRUE(inventoryManager->addItem(rpg::ResourceType::Iron, 800.0f));
    EXPECT_FALSE(inventoryManager->isFull());

    // Try to overfill
    EXPECT_FALSE(inventoryManager->addItem(rpg::ResourceType::Titanium, 300.0f));
    
    // Add within capacity
    EXPECT_TRUE(inventoryManager->addItem(rpg::ResourceType::Titanium, 100.0f));
    EXPECT_EQ(inventoryManager->getAvailableSpace(), 100.0f);
}

TEST_F(InventoryIntegrationTest, HotbarBinding)
{
    // Add items
    inventoryManager->addItem(rpg::ResourceType::Iron, 100.0f);
    inventoryManager->addItem(rpg::ResourceType::Titanium, 50.0f);

    // Bind to hotbar slots
    EXPECT_TRUE(inventoryManager->bindToHotbar(0, rpg::ResourceType::Iron));
    EXPECT_TRUE(inventoryManager->bindToHotbar(1, rpg::ResourceType::Titanium));

    // Verify bindings
    EXPECT_EQ(inventoryManager->getHotbarBinding(0), rpg::ResourceType::Iron);
    EXPECT_EQ(inventoryManager->getHotbarBinding(1), rpg::ResourceType::Titanium);

    // Use item from hotbar
    inventoryManager->selectHotbarSlot(0);
    EXPECT_TRUE(inventoryManager->useSelectedHotbarItem(10.0f));
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 90.0f);
}

TEST_F(InventoryIntegrationTest, RemoveItems)
{
    // Add then remove
    inventoryManager->addItem(rpg::ResourceType::Iron, 100.0f);
    EXPECT_TRUE(inventoryManager->removeItem(rpg::ResourceType::Iron, 30.0f));
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 70.0f);

    // Try to remove more than available
    EXPECT_FALSE(inventoryManager->removeItem(rpg::ResourceType::Iron, 100.0f));
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 70.0f);
}

TEST_F(InventoryIntegrationTest, CraftingWithSufficientResources)
{
    // Add resources for "Basic Shield"
    inventoryManager->addItem(rpg::ResourceType::Iron, 100.0f);
    inventoryManager->addItem(rpg::ResourceType::Titanium, 50.0f);

    // Get recipe names
    auto recipeNames = craftingSystem->getAllRecipeNames();
    ASSERT_FALSE(recipeNames.empty());

    // Find "Basic Shield" recipe
    auto it = std::find(recipeNames.begin(), recipeNames.end(), "Basic Shield");
    ASSERT_NE(it, recipeNames.end());
    int recipeIndex = std::distance(recipeNames.begin(), it);

    // Check can craft
    EXPECT_TRUE(craftingSystem->canCraft("Basic Shield", inventoryManager->getInventory()));

    // Select and craft
    craftingPanel->setSelectedRecipe(recipeIndex);
    EXPECT_TRUE(craftingPanel->craftSelectedRecipe());

    // Verify resources consumed
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 50.0f);
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Titanium), 30.0f);
}

TEST_F(InventoryIntegrationTest, CraftingWithInsufficientResources)
{
    // Add insufficient resources
    inventoryManager->addItem(rpg::ResourceType::Iron, 10.0f);

    // Try to craft "Basic Shield" (needs 50 Iron, 20 Titanium)
    auto recipeNames = craftingSystem->getAllRecipeNames();
    auto it = std::find(recipeNames.begin(), recipeNames.end(), "Basic Shield");
    ASSERT_NE(it, recipeNames.end());
    int recipeIndex = std::distance(recipeNames.begin(), it);

    // Check cannot craft
    EXPECT_FALSE(craftingSystem->canCraft("Basic Shield", inventoryManager->getInventory()));

    // Try to craft should fail
    craftingPanel->setSelectedRecipe(recipeIndex);
    EXPECT_FALSE(craftingPanel->craftSelectedRecipe());

    // Resources should not be consumed
    EXPECT_EQ(inventoryManager->getItemAmount(rpg::ResourceType::Iron), 10.0f);
}

TEST_F(InventoryIntegrationTest, InventoryPanelVisibility)
{
    EXPECT_FALSE(inventoryManager->isInventoryVisible());
    
    inventoryManager->setInventoryVisible(true);
    EXPECT_TRUE(inventoryManager->isInventoryVisible());
    
    inventoryManager->toggleInventory();
    EXPECT_FALSE(inventoryManager->isInventoryVisible());
}

TEST_F(InventoryIntegrationTest, CraftingPanelVisibility)
{
    EXPECT_FALSE(craftingPanel->isVisible());
    
    craftingPanel->setVisible(true);
    EXPECT_TRUE(craftingPanel->isVisible());
    
    craftingPanel->toggleVisibility();
    EXPECT_FALSE(craftingPanel->isVisible());
}

TEST_F(InventoryIntegrationTest, CraftingRecipeFiltering)
{
    // Set search filter
    craftingPanel->setSearchFilter("Shield");
    
    auto recipes = craftingPanel->getAvailableRecipes();
    
    // Should only contain recipes with "Shield" in name
    for (const auto& recipe : recipes) {
        EXPECT_NE(recipe.name.find("Shield"), std::string::npos);
    }
}

TEST_F(InventoryIntegrationTest, EventCallbacks)
{
    bool itemAddedCalled = false;
    bool itemRemovedCalled = false;
    bool itemUsedCalled = false;

    inventoryManager->setOnItemAdded([&](rpg::ResourceType type, float amount) {
        itemAddedCalled = true;
        EXPECT_EQ(type, rpg::ResourceType::Iron);
        EXPECT_EQ(amount, 50.0f);
    });

    inventoryManager->setOnItemRemoved([&](rpg::ResourceType type, float amount) {
        itemRemovedCalled = true;
        EXPECT_EQ(type, rpg::ResourceType::Iron);
        EXPECT_EQ(amount, 10.0f);
    });

    inventoryManager->setOnItemUsed([&](rpg::ResourceType type, float amount) {
        itemUsedCalled = true;
    });

    // Trigger callbacks
    inventoryManager->addItem(rpg::ResourceType::Iron, 50.0f);
    EXPECT_TRUE(itemAddedCalled);

    inventoryManager->removeItem(rpg::ResourceType::Iron, 10.0f);
    EXPECT_TRUE(itemRemovedCalled);

    // Bind and use from hotbar
    inventoryManager->bindToHotbar(0, rpg::ResourceType::Iron);
    inventoryManager->selectHotbarSlot(0);
    inventoryManager->useSelectedHotbarItem(5.0f);
    EXPECT_TRUE(itemUsedCalled);
}

} // anonymous namespace
} // namespace fresh
