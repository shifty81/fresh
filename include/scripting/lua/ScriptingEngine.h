#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace fresh
{
namespace scripting
{

/**
 * @brief Scripting engine for Lua integration
 *
 * NOTE: This is a stub implementation. To complete this system:
 * 1. Add Lua library dependency (lua5.3 or lua5.4)
 * 2. Integrate LuaBridge or sol2 for C++ binding
 * 3. Implement the actual Lua state management
 *
 * Required libraries (choose one):
 * - LuaBridge3: https://github.com/kunitoki/LuaBridge3
 * - sol2: https://github.com/ThePhD/sol2
 * - Plain Lua C API with manual bindings
 */
class ScriptingEngine
{
public:
    ScriptingEngine();
    ~ScriptingEngine();

    // Lifecycle
    bool initialize();
    void shutdown();

    // Script execution
    bool loadScript(const std::string& filepath);
    bool executeScript(const std::string& code);
    bool callFunction(const std::string& functionName);

    // Object registration (for Lua access)
    template <typename T>
    void registerClass(const std::string& className);

    template <typename T>
    void registerObject(const std::string& name, T* object);

    void registerFunction(const std::string& name, std::function<void()> func);

    // Global variable access
    void setGlobal(const std::string& name, int value);
    void setGlobal(const std::string& name, float value);
    void setGlobal(const std::string& name, const std::string& value);

    int getGlobalInt(const std::string& name);
    float getGlobalFloat(const std::string& name);
    std::string getGlobalString(const std::string& name);

    // Mod loading system
    bool loadMod(const std::string& modDirectory);
    std::vector<std::string> getLoadedMods() const;

    // Input system registration
    void registerInputManager(void* inputManager);

    // Error handling
    std::string getLastError() const
    {
        return lastError;
    }

private:
    // Lua state (would be lua_State* with actual Lua)
    void* luaState;

    std::string lastError;
    std::vector<std::string> loadedMods;

    void reportError(const std::string& error);
};

// Template implementations (would use Lua binding library)
template <typename T>
void ScriptingEngine::registerClass(const std::string& className)
{
    // TODO: Implement with LuaBridge or sol2
    // Example with LuaBridge:
    // luabridge::getGlobalNamespace(L)
    //     .beginClass<T>(className.c_str())
    //     .addConstructor<void(*)(void)>()
    //     // ... add methods ...
    //     .endClass();
}

template <typename T>
void ScriptingEngine::registerObject(const std::string& name, T* object)
{
    // TODO: Implement with LuaBridge or sol2
    // Example with LuaBridge:
    // luabridge::setGlobal(L, object, name.c_str());
}

} // namespace scripting
} // namespace fresh
