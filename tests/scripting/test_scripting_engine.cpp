/**
 * @file test_scripting_engine.cpp
 * @brief Unit tests for the Lua scripting engine
 */

#include <gtest/gtest.h>
#include "scripting/lua/ScriptingEngine.h"
#include <filesystem>
#include <fstream>

using namespace fresh::scripting;

class ScriptingEngineTest : public ::testing::Test {
protected:
    ScriptingEngine engine;
    std::string testScriptPath;

    void SetUp() override {
        // Initialize the scripting engine
        ASSERT_TRUE(engine.initialize());

        // Create a temporary test script
        testScriptPath = "test_script.lua";
    }

    void TearDown() override {
        // Clean up test files
        if (std::filesystem::exists(testScriptPath)) {
            std::filesystem::remove(testScriptPath);
        }
    }

    void createTestScript(const std::string& content) {
        std::ofstream file(testScriptPath);
        file << content;
        file.close();
    }
};

TEST_F(ScriptingEngineTest, InitializationTest) {
    // Engine should be initialized in SetUp
    EXPECT_TRUE(true);
}

TEST_F(ScriptingEngineTest, ExecuteSimpleScript) {
    bool result = engine.executeScript("x = 10");
    EXPECT_TRUE(result);
}

TEST_F(ScriptingEngineTest, SetAndGetGlobalInt) {
    engine.setGlobal("test_int", 42);
    int value = engine.getGlobalInt("test_int");
    EXPECT_EQ(value, 42);
}

TEST_F(ScriptingEngineTest, SetAndGetGlobalFloat) {
    engine.setGlobal("test_float", 3.14f);
    float value = engine.getGlobalFloat("test_float");
    EXPECT_NEAR(value, 3.14f, 0.001f);
}

TEST_F(ScriptingEngineTest, SetAndGetGlobalString) {
    engine.setGlobal("test_string", std::string("Hello Lua"));
    std::string value = engine.getGlobalString("test_string");
    EXPECT_EQ(value, "Hello Lua");
}

TEST_F(ScriptingEngineTest, CallFunction) {
    // Define a simple function in Lua
    engine.executeScript(R"(
        function test_func()
            return 123
        end
    )");

    bool result = engine.callFunction("test_func");
    EXPECT_TRUE(result);
}

TEST_F(ScriptingEngineTest, RegisterFunction) {
    bool called = false;
    
    engine.registerFunction("cpp_function", [&called]() {
        called = true;
    });

    // Call the registered function from Lua
    engine.executeScript("cpp_function()");
    
    EXPECT_TRUE(called);
}

TEST_F(ScriptingEngineTest, LoadScriptFile) {
    createTestScript(R"(
        -- Test script
        greeting = "Hello from Lua file!"
    )");

    bool result = engine.loadScript(testScriptPath);
    EXPECT_TRUE(result);

    std::string value = engine.getGlobalString("greeting");
    EXPECT_EQ(value, "Hello from Lua file!");
}

TEST_F(ScriptingEngineTest, ErrorHandling) {
    // Execute invalid Lua code
    bool result = engine.executeScript("invalid lua code !");
    EXPECT_FALSE(result);
    
    // Check that error message is set
    std::string error = engine.getLastError();
    EXPECT_FALSE(error.empty());
}

TEST_F(ScriptingEngineTest, MathOperations) {
    engine.executeScript(R"(
        result = 10 + 20 * 2
    )");

    int value = engine.getGlobalInt("result");
    EXPECT_EQ(value, 50);
}

TEST_F(ScriptingEngineTest, TableAccess) {
    engine.executeScript(R"(
        my_table = {
            x = 100,
            y = 200,
            name = "Test"
        }
    )");

    // Access table values
    engine.executeScript("x_value = my_table.x");
    int x = engine.getGlobalInt("x_value");
    EXPECT_EQ(x, 100);
}

TEST_F(ScriptingEngineTest, MultipleScriptExecution) {
    engine.executeScript("a = 10");
    engine.executeScript("b = 20");
    engine.executeScript("c = a + b");

    int result = engine.getGlobalInt("c");
    EXPECT_EQ(result, 30);
}
