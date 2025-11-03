#include "scripting/lua/ScriptingEngine.h"
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
    // TODO: Initialize Lua state
    // Example with plain Lua:
    // luaState = luaL_newstate();
    // luaL_openlibs(luaState);
    
    std::cout << "NOTE: Lua scripting engine is a stub implementation.\n";
    std::cout << "To enable Lua scripting:\n";
    std::cout << "  1. Install Lua library (lua5.3-dev or lua5.4-dev)\n";
    std::cout << "  2. Choose binding library (LuaBridge3 or sol2)\n";
    std::cout << "  3. Add to CMakeLists.txt:\n";
    std::cout << "     find_package(Lua REQUIRED)\n";
    std::cout << "     target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})\n";
    std::cout << "  4. Implement the TODOs in ScriptingEngine.cpp\n\n";
    
    return true;
}

void ScriptingEngine::shutdown() {
    // TODO: Close Lua state
    // if (luaState) {
    //     lua_close((lua_State*)luaState);
    //     luaState = nullptr;
    // }
}

bool ScriptingEngine::loadScript(const std::string& filepath) {
    // Read script file
    std::ifstream file(filepath);
    if (!file.is_open()) {
        reportError("Failed to open script file: " + filepath);
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    
    return executeScript(code);
}

bool ScriptingEngine::executeScript(const std::string& code) {
    // TODO: Execute Lua code
    // Example:
    // if (luaL_dostring((lua_State*)luaState, code.c_str()) != LUA_OK) {
    //     reportError(lua_tostring((lua_State*)luaState, -1));
    //     lua_pop((lua_State*)luaState, 1);
    //     return false;
    // }
    
    reportError("Lua scripting not implemented (stub only)");
    return false;
}

bool ScriptingEngine::callFunction(const std::string& functionName) {
    // TODO: Call Lua function
    // Example:
    // lua_getglobal((lua_State*)luaState, functionName.c_str());
    // if (!lua_isfunction((lua_State*)luaState, -1)) {
    //     reportError("Function not found: " + functionName);
    //     lua_pop((lua_State*)luaState, 1);
    //     return false;
    // }
    // if (lua_pcall((lua_State*)luaState, 0, 0, 0) != LUA_OK) {
    //     reportError(lua_tostring((lua_State*)luaState, -1));
    //     lua_pop((lua_State*)luaState, 1);
    //     return false;
    // }
    
    reportError("Lua scripting not implemented (stub only)");
    return false;
}

void ScriptingEngine::registerFunction(const std::string& name,
                                      std::function<void()> func) {
    // TODO: Register C++ function for Lua
    // Example with LuaBridge:
    // luabridge::getGlobalNamespace((lua_State*)luaState)
    //     .addFunction(name.c_str(), func);
}

void ScriptingEngine::setGlobal(const std::string& name, int value) {
    // TODO: Set Lua global
    // lua_pushinteger((lua_State*)luaState, value);
    // lua_setglobal((lua_State*)luaState, name.c_str());
}

void ScriptingEngine::setGlobal(const std::string& name, float value) {
    // TODO: Set Lua global
    // lua_pushnumber((lua_State*)luaState, value);
    // lua_setglobal((lua_State*)luaState, name.c_str());
}

void ScriptingEngine::setGlobal(const std::string& name, const std::string& value) {
    // TODO: Set Lua global
    // lua_pushstring((lua_State*)luaState, value.c_str());
    // lua_setglobal((lua_State*)luaState, name.c_str());
}

int ScriptingEngine::getGlobalInt(const std::string& name) {
    // TODO: Get Lua global
    // lua_getglobal((lua_State*)luaState, name.c_str());
    // int value = lua_tointeger((lua_State*)luaState, -1);
    // lua_pop((lua_State*)luaState, 1);
    // return value;
    return 0;
}

float ScriptingEngine::getGlobalFloat(const std::string& name) {
    // TODO: Get Lua global
    // lua_getglobal((lua_State*)luaState, name.c_str());
    // float value = lua_tonumber((lua_State*)luaState, -1);
    // lua_pop((lua_State*)luaState, 1);
    // return value;
    return 0.0f;
}

std::string ScriptingEngine::getGlobalString(const std::string& name) {
    // TODO: Get Lua global
    // lua_getglobal((lua_State*)luaState, name.c_str());
    // const char* value = lua_tostring((lua_State*)luaState, -1);
    // lua_pop((lua_State*)luaState, 1);
    // return value ? std::string(value) : "";
    return "";
}

bool ScriptingEngine::loadMod(const std::string& modDirectory) {
    // TODO: Implement mod loading
    // 1. Scan modDirectory for mod.lua or init.lua
    // 2. Load manifest (mod.json or similar)
    // 3. Execute mod initialization script
    // 4. Register mod API hooks
    
    reportError("Mod loading not implemented (stub only)");
    return false;
}

std::vector<std::string> ScriptingEngine::getLoadedMods() const {
    return loadedMods;
}

void ScriptingEngine::reportError(const std::string& error) {
    lastError = error;
    std::cerr << "[ScriptingEngine] " << error << std::endl;
}

} // namespace scripting
} // namespace fresh
