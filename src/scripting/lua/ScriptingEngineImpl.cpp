/**
 * @file ScriptingEngine.cpp
 * @brief Production-ready Lua scripting engine using Sol2
 * 
 * Architecture:
 * - C++ Core: High-performance voxel operations, mesh generation, physics, rendering
 * - Lua Scripting: Game logic, AI, world generation rules, modding
 * - Sol2 Integration: Modern C++ to Lua binding with type safety
 * - Safe API: Controlled exposure of engine functionality
 * - Modding Support: Extensible API for community content
 */

#ifdef FRESH_LUA_AVAILABLE

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "scripting/lua/ScriptingEngine.h"
#include "core/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace fresh {
namespace scripting {

ScriptingEngine::ScriptingEngine() : luaState(nullptr) {
}

ScriptingEngine::~ScriptingEngine() {
    shutdown();
}

bool ScriptingEngine::initialize() {
    try {
        // Create new Lua state using Sol2
        auto* state = new sol::state();
        luaState = static_cast<void*>(state);
        
        // Open standard Lua libraries
        state->open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::math,
            sol::lib::table,
            sol::lib::io
        );
        
        // Register engine API
        registerEngineAPI();
        
        Logger::getInstance().info("Lua scripting engine initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Failed to initialize Lua: ") + e.what());
        return false;
    }
}

void ScriptingEngine::shutdown() {
    if (luaState) {
        auto* state = static_cast<sol::state*>(luaState);
        delete state;
        luaState = nullptr;
        Logger::getInstance().info("Lua scripting engine shut down");
    }
}

bool ScriptingEngine::loadScript(const std::string& filepath) {
    if (!luaState) {
        reportError("Lua state not initialized");
        return false;
    }
    
    try {
        auto* state = static_cast<sol::state*>(luaState);
        auto result = state->script_file(filepath);
        
        if (!result.valid()) {
            sol::error err = result;
            reportError(std::string("Error loading script: ") + err.what());
            return false;
        }
        
        Logger::getInstance().info("Loaded Lua script: " + filepath);
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Exception loading script: ") + e.what());
        return false;
    }
}

bool ScriptingEngine::executeScript(const std::string& code) {
    if (!luaState) {
        reportError("Lua state not initialized");
        return false;
    }
    
    try {
        auto* state = static_cast<sol::state*>(luaState);
        auto result = state->script(code);
        
        if (!result.valid()) {
            sol::error err = result;
            reportError(std::string("Error executing script: ") + err.what());
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Exception executing script: ") + e.what());
        return false;
    }
}

bool ScriptingEngine::callFunction(const std::string& functionName) {
    if (!luaState) {
        reportError("Lua state not initialized");
        return false;
    }
    
    try {
        auto* state = static_cast<sol::state*>(luaState);
        sol::protected_function func = (*state)[functionName];
        
        if (!func.valid()) {
            reportError("Function not found: " + functionName);
            return false;
        }
        
        auto result = func();
        if (!result.valid()) {
            sol::error err = result;
            reportError(std::string("Error calling function: ") + err.what());
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Exception calling function: ") + e.what());
        return false;
    }
}

void ScriptingEngine::registerFunction(const std::string& name,
                                      std::function<void()> func) {
    if (!luaState) {
        reportError("Lua state not initialized");
        return;
    }
    
    try {
        auto* state = static_cast<sol::state*>(luaState);
        (*state)[name] = func;
    } catch (const std::exception& e) {
        reportError(std::string("Failed to register function: ") + e.what());
    }
}

void ScriptingEngine::setGlobal(const std::string& name, int value) {
    if (!luaState) return;
    auto* state = static_cast<sol::state*>(luaState);
    (*state)[name] = value;
}

void ScriptingEngine::setGlobal(const std::string& name, float value) {
    if (!luaState) return;
    auto* state = static_cast<sol::state*>(luaState);
    (*state)[name] = value;
}

void ScriptingEngine::setGlobal(const std::string& name, const std::string& value) {
    if (!luaState) return;
    auto* state = static_cast<sol::state*>(luaState);
    (*state)[name] = value;
}

int ScriptingEngine::getGlobalInt(const std::string& name) {
    if (!luaState) return 0;
    auto* state = static_cast<sol::state*>(luaState);
    return (*state)[name].get_or(0);
}

float ScriptingEngine::getGlobalFloat(const std::string& name) {
    if (!luaState) return 0.0f;
    auto* state = static_cast<sol::state*>(luaState);
    return (*state)[name].get_or(0.0f);
}

std::string ScriptingEngine::getGlobalString(const std::string& name) {
    if (!luaState) return "";
    auto* state = static_cast<sol::state*>(luaState);
    return (*state)[name].get_or(std::string(""));
}

bool ScriptingEngine::loadMod(const std::string& modDirectory) {
    if (!luaState) {
        reportError("Lua state not initialized");
        return false;
    }
    
    try {
        // Look for init.lua in mod directory
        std::string initScript = modDirectory + "/init.lua";
        
        if (!loadScript(initScript)) {
            reportError("Failed to load mod from: " + modDirectory);
            return false;
        }
        
        // Call mod initialization if it exists
        callFunction("onModLoad");
        
        loadedMods.push_back(modDirectory);
        Logger::getInstance().info("Loaded mod: " + modDirectory);
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Exception loading mod: ") + e.what());
        return false;
    }
}

std::vector<std::string> ScriptingEngine::getLoadedMods() const {
    return loadedMods;
}

void ScriptingEngine::reportError(const std::string& error) {
    lastError = error;
    Logger::getInstance().error("[ScriptingEngine] " + error);
}

void ScriptingEngine::registerEngineAPI() {
    if (!luaState) return;
    
    auto* state = static_cast<sol::state*>(luaState);
    
    // Register safe utility functions for scripts
    (*state)["print"] = [](const std::string& msg) {
        Logger::getInstance().info("[Lua] " + msg);
    };
    
    (*state)["warn"] = [](const std::string& msg) {
        Logger::getInstance().warning("[Lua] " + msg);
    };
    
    (*state)["error"] = [](const std::string& msg) {
        Logger::getInstance().error("[Lua] " + msg);
    };
    
    // Register engine version info
    (*state)["Engine"] = state->create_table_with(
        "name", "Fresh Voxel Engine",
        "version", "0.1.0",
        "lua_enabled", true
    );
    
    Logger::getInstance().info("Registered Lua engine API");
}

} // namespace scripting
} // namespace fresh

#else // FRESH_LUA_AVAILABLE not defined

// Stub implementation when Lua is not available
#include "scripting/lua/ScriptingEngine.h"
#include <iostream>

namespace fresh {
namespace scripting {

ScriptingEngine::ScriptingEngine() : luaState(nullptr) {}
ScriptingEngine::~ScriptingEngine() { shutdown(); }
bool ScriptingEngine::initialize() { return true; }
void ScriptingEngine::shutdown() {}
bool ScriptingEngine::loadScript(const std::string&) { return false; }
bool ScriptingEngine::executeScript(const std::string&) { return false; }
bool ScriptingEngine::callFunction(const std::string&) { return false; }
void ScriptingEngine::registerFunction(const std::string&, std::function<void()>) {}
void ScriptingEngine::setGlobal(const std::string&, int) {}
void ScriptingEngine::setGlobal(const std::string&, float) {}
void ScriptingEngine::setGlobal(const std::string&, const std::string&) {}
int ScriptingEngine::getGlobalInt(const std::string&) { return 0; }
float ScriptingEngine::getGlobalFloat(const std::string&) { return 0.0f; }
std::string ScriptingEngine::getGlobalString(const std::string&) { return ""; }
bool ScriptingEngine::loadMod(const std::string&) { return false; }
std::vector<std::string> ScriptingEngine::getLoadedMods() const { return {}; }

void ScriptingEngine::reportError(const std::string& error) {
    lastError = error;
    std::cerr << "[ScriptingEngine] " << error << std::endl;
}

} // namespace scripting
} // namespace fresh

#endif // FRESH_LUA_AVAILABLE
