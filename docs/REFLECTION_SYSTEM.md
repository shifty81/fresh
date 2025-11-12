# C++ Reflection System

## Overview

The Fresh Engine includes a lightweight reflection system that allows runtime type introspection, property access, and serialization. This system is essential for:

- **Editor Integration**: Property inspector can generically display and edit object properties
- **Serialization**: Save and load objects without manual serialization code
- **Scripting**: Expose C++ objects to Lua and Python automatically
- **Debugging**: Runtime inspection of object state

## Architecture

The reflection system consists of several key components:

### TypeInfo
Stores metadata about a type including:
- Type name
- Type index for C++ RTTI
- List of properties
- Factory function for creating instances

### PropertyInfo
Stores metadata about a property including:
- Property name
- Property type (bool, int, float, string, etc.)
- Flags (read-only, serializable, editor visible)
- Getter and setter functions

### ReflectionRegistry
Central registry for all reflected types:
- Singleton pattern for global access
- Type lookup by type_index or name
- Stores all registered TypeInfo objects

## Basic Usage

### Registering a Type

```cpp
#include "core/Reflection.h"

namespace fresh {

class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    
    // Member function properties
    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3& pos) { position = pos; }
};

// Register the type and its properties
static TypeRegistrar<Transform> transformRegistrar("Transform")
    .property("position", &Transform::position)
    .property("rotation", &Transform::rotation)
    .property("scale", &Transform::scale);

} // namespace fresh
```

### Using Property Flags

```cpp
// Read-only property (visible in editor but not editable)
static TypeRegistrar<Player> playerRegistrar("Player")
    .property("health", &Player::health)
    .property("maxHealth", &Player::maxHealth, PropertyFlags::ReadOnly | PropertyFlags::EditorVisible);

// Property not visible in editor
static TypeRegistrar<InternalState> stateRegistrar("InternalState")
    .property("debugValue", &InternalState::debugValue, PropertyFlags::Serializable);

// Advanced property (hidden by default in editor)
static TypeRegistrar<PhysicsBody> physicsRegistrar("PhysicsBody")
    .property("mass", &PhysicsBody::mass)
    .property("inertia", &PhysicsBody::inertia, PropertyFlags::Serializable | PropertyFlags::Advanced);
```

### Registering with Getter/Setter Methods

For properties that use methods instead of public members:

```cpp
class Camera {
private:
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

public:
    float getFov() const { return m_fov; }
    void setFov(float fov) { m_fov = fov; }
    
    float getNearPlane() const { return m_nearPlane; }
    void setNearPlane(float near) { m_nearPlane = near; }
};

static TypeRegistrar<Camera> cameraRegistrar("Camera")
    .property("fov", 
        [](const Camera* cam) { return cam->getFov(); },
        [](Camera* cam, float value) { cam->setFov(value); })
    .property("nearPlane",
        [](const Camera* cam) { return cam->getNearPlane(); },
        [](Camera* cam, float value) { cam->setNearPlane(value); });
```

## Runtime Property Access

### Getting Type Information

```cpp
#include "core/Reflection.h"

// Get type info by type
auto& registry = ReflectionRegistry::getInstance();
auto typeInfo = registry.getType(std::type_index(typeid(Transform)));

// Get type info by name
auto typeInfo2 = registry.getType("Transform");

if (typeInfo) {
    std::cout << "Type: " << typeInfo->getName() << std::endl;
    
    // Iterate properties
    for (const auto& [name, prop] : typeInfo->getProperties()) {
        std::cout << "  Property: " << name << std::endl;
        if (prop->isReadOnly()) {
            std::cout << "    (read-only)" << std::endl;
        }
    }
}
```

### Reading and Writing Properties

```cpp
Transform transform;
transform.position = glm::vec3(1, 2, 3);

// Get type info
auto typeInfo = ReflectionRegistry::getInstance().getType(std::type_index(typeid(Transform)));

// Get property
auto posProperty = typeInfo->getProperty("position");

if (posProperty) {
    // Read property value
    std::any value = posProperty->getValue(&transform);
    glm::vec3 pos = std::any_cast<glm::vec3>(value);
    
    std::cout << "Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    
    // Write property value
    glm::vec3 newPos(10, 20, 30);
    posProperty->setValue(&transform, newPos);
}
```

### Creating Instances

```cpp
// Get type info
auto typeInfo = ReflectionRegistry::getInstance().getType("Transform");

if (typeInfo) {
    // Create an instance using the registered factory
    void* instance = typeInfo->createInstance();
    Transform* transform = static_cast<Transform*>(instance);
    
    // Use the instance...
    transform->position = glm::vec3(5, 5, 5);
    
    // Clean up
    delete transform;
}
```

## Integration with Editor

The reflection system integrates seamlessly with the editor's property inspector:

```cpp
// In EditorGUI or InspectorPanel
void InspectorPanel::displayObject(void* object, std::type_index typeIndex) {
    auto typeInfo = ReflectionRegistry::getInstance().getType(typeIndex);
    if (!typeInfo) return;
    
    ImGui::Text("Type: %s", typeInfo->getName().c_str());
    ImGui::Separator();
    
    for (const auto& [name, prop] : typeInfo->getProperties()) {
        if (!prop->isEditorVisible()) continue;
        
        std::any value = prop->getValue(object);
        
        switch (prop->getType()) {
            case PropertyType::Float: {
                float f = std::any_cast<float>(value);
                if (ImGui::DragFloat(name.c_str(), &f)) {
                    prop->setValue(object, f);
                }
                break;
            }
            case PropertyType::Int: {
                int i = std::any_cast<int>(value);
                if (ImGui::DragInt(name.c_str(), &i)) {
                    prop->setValue(object, i);
                }
                break;
            }
            case PropertyType::Vec3: {
                glm::vec3 v = std::any_cast<glm::vec3>(value);
                if (ImGui::DragFloat3(name.c_str(), &v.x)) {
                    prop->setValue(object, v);
                }
                break;
            }
            // ... handle other types
        }
    }
}
```

## Integration with Serialization

The reflection system simplifies serialization:

```cpp
#include <nlohmann/json.hpp>

// Serialize any reflected object to JSON
nlohmann::json serializeObject(void* object, std::type_index typeIndex) {
    nlohmann::json j;
    
    auto typeInfo = ReflectionRegistry::getInstance().getType(typeIndex);
    if (!typeInfo) return j;
    
    j["type"] = typeInfo->getName();
    
    for (const auto& [name, prop] : typeInfo->getProperties()) {
        if (!prop->isSerializable()) continue;
        
        std::any value = prop->getValue(object);
        
        switch (prop->getType()) {
            case PropertyType::Float:
                j[name] = std::any_cast<float>(value);
                break;
            case PropertyType::Int:
                j[name] = std::any_cast<int>(value);
                break;
            case PropertyType::String:
                j[name] = std::any_cast<std::string>(value);
                break;
            // ... handle other types
        }
    }
    
    return j;
}

// Deserialize JSON to any reflected object
void deserializeObject(void* object, const nlohmann::json& j, std::type_index typeIndex) {
    auto typeInfo = ReflectionRegistry::getInstance().getType(typeIndex);
    if (!typeInfo) return;
    
    for (const auto& [name, prop] : typeInfo->getProperties()) {
        if (!prop->isSerializable() || !j.contains(name)) continue;
        
        switch (prop->getType()) {
            case PropertyType::Float:
                prop->setValue(object, j[name].get<float>());
                break;
            case PropertyType::Int:
                prop->setValue(object, j[name].get<int>());
                break;
            case PropertyType::String:
                prop->setValue(object, j[name].get<std::string>());
                break;
            // ... handle other types
        }
    }
}
```

## Performance Considerations

### Overhead
- Property registration: One-time cost at startup
- Type lookup: O(1) hash map lookup
- Property access: One virtual function call + any_cast
- Recommended: Cache TypeInfo pointers for hot paths

### Best Practices
1. Register types at startup, not in hot paths
2. Cache TypeInfo and PropertyInfo pointers when possible
3. Use direct access for performance-critical code
4. Use reflection for editor/tools/serialization only

### Example: Caching for Performance

```cpp
class Component {
private:
    // Cache type info at construction
    std::shared_ptr<TypeInfo> m_cachedTypeInfo;
    
public:
    Component() {
        m_cachedTypeInfo = ReflectionRegistry::getInstance()
            .getType(std::type_index(typeid(*this)));
    }
    
    // Fast property access using cached type info
    std::any getPropertyValue(const std::string& name) {
        auto prop = m_cachedTypeInfo->getProperty(name);
        return prop ? prop->getValue(this) : std::any();
    }
};
```

## Advanced Features

### Custom Property Types

To support custom types (like glm::vec3), implement conversion functions:

```cpp
// Specialization for glm::vec3
template<>
PropertyType TypeRegistrar<MyClass>::getPropertyType<glm::vec3>() {
    return PropertyType::Vec3;
}

// Then register normally
static TypeRegistrar<MyClass> registrar("MyClass")
    .property("position", &MyClass::position); // glm::vec3
```

### Inheritance

For types with inheritance, register base class properties separately:

```cpp
class Entity {
public:
    std::string name;
};

class Player : public Entity {
public:
    int health;
};

// Register base class
static TypeRegistrar<Entity> entityRegistrar("Entity")
    .property("name", &Entity::name);

// Register derived class (base properties must be accessed through casting)
static TypeRegistrar<Player> playerRegistrar("Player")
    .property("health", &Player::health)
    .property("name", 
        [](const Player* p) { return static_cast<const Entity*>(p)->name; },
        [](Player* p, const std::string& v) { static_cast<Entity*>(p)->name = v; });
```

## Future Enhancements

Planned improvements to the reflection system:

- [ ] Support for arrays and containers (std::vector, std::map)
- [ ] Method reflection (not just properties)
- [ ] Attributes/annotations for properties
- [ ] Inheritance hierarchy tracking
- [ ] Automatic property grouping
- [ ] Property value constraints (min, max, enum)

## See Also

- [Serialization Guide](SERIALIZATION.md)
- [Editor Integration](EDITOR_INTEGRATION.md)
- [Lua Scripting](../LUA_INTEGRATION_GUIDE.md)
