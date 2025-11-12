#include <gtest/gtest.h>
#include "core/Reflection.h"
#include <string>

using namespace fresh;

// Test class with various property types
class TestObject {
public:
    int intValue = 42;
    float floatValue = 3.14f;
    double doubleValue = 2.718;
    bool boolValue = true;
    std::string stringValue = "Hello";
    
    int getReadOnlyValue() const { return m_readOnly; }
    
private:
    int m_readOnly = 100;
};

// Register the test type
static TypeRegistrar<TestObject> testObjectRegistrar("TestObject")
    .property("intValue", &TestObject::intValue)
    .property("floatValue", &TestObject::floatValue)
    .property("doubleValue", &TestObject::doubleValue)
    .property("boolValue", &TestObject::boolValue)
    .property("stringValue", &TestObject::stringValue)
    .property("readOnlyValue",
        [](const TestObject* obj) { return obj->getReadOnlyValue(); },
        [](TestObject*, int) { /* No-op setter */ },
        PropertyFlags::ReadOnly | PropertyFlags::EditorVisible);

TEST(ReflectionTest, TypeRegistration) {
    auto& registry = ReflectionRegistry::getInstance();
    
    // Get type by type_index
    auto typeInfo = registry.getType(std::type_index(typeid(TestObject)));
    ASSERT_NE(typeInfo, nullptr);
    EXPECT_EQ(typeInfo->getName(), "TestObject");
    
    // Get type by name
    auto typeInfo2 = registry.getType("TestObject");
    ASSERT_NE(typeInfo2, nullptr);
    EXPECT_EQ(typeInfo2, typeInfo);
}

TEST(ReflectionTest, PropertyCount) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    const auto& properties = typeInfo->getProperties();
    EXPECT_EQ(properties.size(), 6); // 6 properties registered
}

TEST(ReflectionTest, PropertyAccess) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    // Check individual properties exist
    EXPECT_NE(typeInfo->getProperty("intValue"), nullptr);
    EXPECT_NE(typeInfo->getProperty("floatValue"), nullptr);
    EXPECT_NE(typeInfo->getProperty("doubleValue"), nullptr);
    EXPECT_NE(typeInfo->getProperty("boolValue"), nullptr);
    EXPECT_NE(typeInfo->getProperty("stringValue"), nullptr);
    EXPECT_NE(typeInfo->getProperty("readOnlyValue"), nullptr);
    
    // Non-existent property
    EXPECT_EQ(typeInfo->getProperty("nonExistent"), nullptr);
}

TEST(ReflectionTest, PropertyTypes) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    auto intProp = typeInfo->getProperty("intValue");
    ASSERT_NE(intProp, nullptr);
    EXPECT_EQ(intProp->getType(), PropertyType::Int);
    
    auto floatProp = typeInfo->getProperty("floatValue");
    ASSERT_NE(floatProp, nullptr);
    EXPECT_EQ(floatProp->getType(), PropertyType::Float);
    
    auto doubleProp = typeInfo->getProperty("doubleValue");
    ASSERT_NE(doubleProp, nullptr);
    EXPECT_EQ(doubleProp->getType(), PropertyType::Double);
    
    auto boolProp = typeInfo->getProperty("boolValue");
    ASSERT_NE(boolProp, nullptr);
    EXPECT_EQ(boolProp->getType(), PropertyType::Bool);
    
    auto stringProp = typeInfo->getProperty("stringValue");
    ASSERT_NE(stringProp, nullptr);
    EXPECT_EQ(stringProp->getType(), PropertyType::String);
}

TEST(ReflectionTest, PropertyReadWrite) {
    TestObject obj;
    obj.intValue = 10;
    obj.floatValue = 1.5f;
    
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    // Read int property
    auto intProp = typeInfo->getProperty("intValue");
    ASSERT_NE(intProp, nullptr);
    std::any intValue = intProp->getValue(&obj);
    EXPECT_EQ(std::any_cast<int>(intValue), 10);
    
    // Write int property
    intProp->setValue(&obj, 20);
    EXPECT_EQ(obj.intValue, 20);
    
    // Read float property
    auto floatProp = typeInfo->getProperty("floatValue");
    ASSERT_NE(floatProp, nullptr);
    std::any floatValue = floatProp->getValue(&obj);
    EXPECT_FLOAT_EQ(std::any_cast<float>(floatValue), 1.5f);
    
    // Write float property
    floatProp->setValue(&obj, 2.5f);
    EXPECT_FLOAT_EQ(obj.floatValue, 2.5f);
}

TEST(ReflectionTest, PropertyFlags) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    // Normal property (serializable and editor visible by default)
    auto intProp = typeInfo->getProperty("intValue");
    ASSERT_NE(intProp, nullptr);
    EXPECT_TRUE(intProp->isSerializable());
    EXPECT_TRUE(intProp->isEditorVisible());
    EXPECT_FALSE(intProp->isReadOnly());
    
    // Read-only property
    auto readOnlyProp = typeInfo->getProperty("readOnlyValue");
    ASSERT_NE(readOnlyProp, nullptr);
    EXPECT_TRUE(readOnlyProp->isReadOnly());
    EXPECT_TRUE(readOnlyProp->isEditorVisible());
}

TEST(ReflectionTest, ReadOnlyPropertyCannotBeWritten) {
    TestObject obj;
    
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    auto readOnlyProp = typeInfo->getProperty("readOnlyValue");
    ASSERT_NE(readOnlyProp, nullptr);
    
    // Read the value
    std::any value = readOnlyProp->getValue(&obj);
    EXPECT_EQ(std::any_cast<int>(value), 100);
    
    // Attempt to write (should be ignored due to read-only flag)
    readOnlyProp->setValue(&obj, 200);
    
    // Value should remain unchanged
    std::any newValue = readOnlyProp->getValue(&obj);
    EXPECT_EQ(std::any_cast<int>(newValue), 100);
}

TEST(ReflectionTest, StringProperty) {
    TestObject obj;
    obj.stringValue = "Initial";
    
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    auto stringProp = typeInfo->getProperty("stringValue");
    ASSERT_NE(stringProp, nullptr);
    
    // Read string
    std::any value = stringProp->getValue(&obj);
    EXPECT_EQ(std::any_cast<std::string>(value), "Initial");
    
    // Write string
    stringProp->setValue(&obj, std::string("Modified"));
    EXPECT_EQ(obj.stringValue, "Modified");
}

TEST(ReflectionTest, InstanceCreation) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    // Create instance using factory
    void* instance = typeInfo->createInstance();
    ASSERT_NE(instance, nullptr);
    
    TestObject* obj = static_cast<TestObject*>(instance);
    EXPECT_EQ(obj->intValue, 42); // Default value
    EXPECT_FLOAT_EQ(obj->floatValue, 3.14f); // Default value
    
    // Clean up
    delete obj;
}

TEST(ReflectionTest, MultipleInstances) {
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    // Create multiple instances
    TestObject* obj1 = static_cast<TestObject*>(typeInfo->createInstance());
    TestObject* obj2 = static_cast<TestObject*>(typeInfo->createInstance());
    
    ASSERT_NE(obj1, nullptr);
    ASSERT_NE(obj2, nullptr);
    EXPECT_NE(obj1, obj2); // Different instances
    
    // Modify one instance
    obj1->intValue = 100;
    obj2->intValue = 200;
    
    EXPECT_EQ(obj1->intValue, 100);
    EXPECT_EQ(obj2->intValue, 200);
    
    // Clean up
    delete obj1;
    delete obj2;
}

TEST(ReflectionTest, IterateAllProperties) {
    TestObject obj;
    obj.intValue = 42;
    obj.floatValue = 3.14f;
    obj.boolValue = false;
    
    auto typeInfo = ReflectionRegistry::getInstance().getType("TestObject");
    ASSERT_NE(typeInfo, nullptr);
    
    int propertyCount = 0;
    for (const auto& [name, prop] : typeInfo->getProperties()) {
        EXPECT_FALSE(name.empty());
        EXPECT_NE(prop, nullptr);
        
        // Try to read the property
        std::any value = prop->getValue(&obj);
        EXPECT_TRUE(value.has_value());
        
        propertyCount++;
    }
    
    EXPECT_EQ(propertyCount, 6);
}
