# Scene Serialization System

## Overview

The Fresh Voxel Engine includes a comprehensive scene serialization system that allows saving and loading entire scene hierarchies to/from JSON files. This enables saving game states, level design, and dynamic content loading.

## Dependencies

- **nlohmann-json**: JSON parsing and serialization library (already included)

The scene serialization is only available when `FRESH_JSON_AVAILABLE` is defined during compilation.

## Features

### ✅ Implemented

1. **Scene Graph Serialization**
   - Complete scene hierarchy preservation
   - Node transforms (position, rotation, scale)
   - Node active states
   - Nested child relationships

2. **File Format**
   - Human-readable JSON format
   - Version tracking
   - Metadata support
   - Pretty-printed for easy editing

3. **Robust Loading**
   - Error handling and validation
   - Missing file detection
   - Malformed JSON handling
   - Graceful degradation

## Scene File Format

### Basic Structure

```json
{
  "name": "Scene Name",
  "version": "1.0",
  "metadata": {},
  "root": {
    "name": "Root",
    "active": true,
    "position": [0, 0, 0],
    "rotation": [0, 0, 0, 1],
    "scale": [1, 1, 1],
    "children": [...]
  }
}
```

### Field Descriptions

- **name**: Scene name (string)
- **version**: File format version (string)
- **metadata**: Custom scene metadata (object)
- **root**: Root scene node with children

### Node Structure

Each node in the scene graph contains:

- **name**: Node identifier (string)
- **active**: Whether node is active (boolean)
- **position**: [x, y, z] world position (array of 3 floats)
- **rotation**: [x, y, z, w] quaternion rotation (array of 4 floats)
- **scale**: [x, y, z] scale factors (array of 3 floats)
- **children**: Array of child nodes (recursive)

## Usage Examples

### Basic Save/Load

```cpp
#include "core/SceneManager.h"

using namespace fresh;

// Get scene manager instance
SceneManager& manager = SceneManager::getInstance();

// Create a scene
Scene* scene = manager.createScene("MyScene");

// Add some nodes
auto node = std::make_shared<SceneNode>("Object");
node->setPosition(glm::vec3(10.0f, 0.0f, 5.0f));
scene->addNode(node);

// Save scene to file
if (manager.saveScene("scenes/my_scene.json", scene)) {
    std::cout << "Scene saved successfully!" << std::endl;
}

// Load scene from file
Scene* loadedScene = manager.loadScene("scenes/my_scene.json");
if (loadedScene) {
    std::cout << "Scene loaded successfully!" << std::endl;
    manager.setActiveScene(loadedScene->getName());
}
```

### Creating Complex Hierarchies

```cpp
// Create parent-child relationships
auto parent = std::make_shared<SceneNode>("Parent");
parent->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));

auto child = std::make_shared<SceneNode>("Child");
child->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
child->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

parent->addChild(child);
scene->addNode(parent);

// Save - hierarchy will be preserved
manager.saveScene("scenes/hierarchy.json", scene);
```

### Setting Node Transforms

```cpp
auto node = std::make_shared<SceneNode>("TransformedNode");

// Set position
node->setPosition(glm::vec3(10.0f, 20.0f, 30.0f));

// Set rotation (quaternion)
glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
node->setRotation(rotation);

// Set scale
node->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

// Set active state
node->setActive(true);

scene->addNode(node);
```

### Finding Nodes After Loading

```cpp
Scene* scene = manager.loadScene("scenes/my_scene.json");

// Find node by name
SceneNode* node = scene->findNode("Object");
if (node) {
    // Access node properties
    glm::vec3 pos = node->getPosition();
    glm::quat rot = node->getRotation();
    glm::vec3 scl = node->getScale();
    
    std::cout << "Node position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}
```

### Error Handling

```cpp
// Save with null check
if (scene) {
    if (!manager.saveScene("scenes/output.json", scene)) {
        std::cerr << "Failed to save scene" << std::endl;
    }
} else {
    std::cerr << "Scene is null" << std::endl;
}

// Load with error handling
Scene* scene = manager.loadScene("scenes/input.json");
if (!scene) {
    std::cerr << "Failed to load scene" << std::endl;
    // Handle error - create default scene, retry, etc.
}
```

## Example Scene Files

See `examples/scenes/` directory for example scene files:

- `example_scene.json` - Basic scene with ground, player, camera, and lighting

## Manual Editing

Scene files are human-readable JSON and can be edited manually:

1. Open the .json file in any text editor
2. Modify node names, transforms, or hierarchy
3. Ensure JSON syntax is valid
4. Load the scene in your application

**Tips:**
- Use a JSON validator to check syntax
- Keep indentation for readability
- Back up files before editing
- Test changes incrementally

## API Reference

### SceneManager

```cpp
// Create new scene
Scene* createScene(const std::string& name);

// Load scene from file
Scene* loadScene(const std::string& path);

// Save scene to file
bool saveScene(const std::string& path, Scene* scene);

// Set active scene
void setActiveScene(const std::string& name);

// Get active scene
Scene* getActiveScene();
```

### Scene

```cpp
// Get scene name
const std::string& getName() const;

// Get root node
SceneNode* getRoot();

// Add node to scene
void addNode(std::shared_ptr<SceneNode> node);

// Find node by name
SceneNode* findNode(const std::string& name);
```

### SceneNode

```cpp
// Transform
void setPosition(const glm::vec3& pos);
void setRotation(const glm::quat& rot);
void setScale(const glm::vec3& scl);

glm::vec3 getPosition() const;
glm::quat getRotation() const;
glm::vec3 getScale() const;

// Hierarchy
void addChild(std::shared_ptr<SceneNode> child);
void removeChild(std::shared_ptr<SceneNode> child);
SceneNode* getParent() const;
const std::vector<std::shared_ptr<SceneNode>>& getChildren() const;

// Properties
void setActive(bool active);
bool isActive() const;
const std::string& getName() const;
```

## Testing

Unit tests are provided in `tests/core/test_scene_serialization.cpp`:

```bash
cd build
ctest -R test_scene_serialization
```

Tests cover:
- Creating and saving scenes
- Loading scenes from files
- Transform preservation
- Hierarchy preservation
- Active state preservation
- Error handling

## Performance Considerations

1. **File Size**: Scene files are human-readable but can be large for complex scenes
2. **Load Time**: JSON parsing is fast but linear with scene complexity
3. **Memory**: Entire scene is loaded into memory at once

### Optimization Tips

- Use binary formats for very large scenes (future enhancement)
- Stream loading for massive worlds (future enhancement)
- Compress scene files for distribution
- Cache frequently-accessed scenes

## Future Enhancements

### Planned Features

- [ ] Component serialization (attach game components to nodes)
- [ ] Resource references (meshes, textures, materials)
- [ ] Binary scene format for faster loading
- [ ] Incremental/streaming scene loading
- [ ] Scene diff and merging tools
- [ ] Asset dependency tracking
- [ ] Scene validation tools
- [ ] Scene conversion utilities

## Best Practices

1. **Naming**: Use descriptive, unique names for nodes
2. **Organization**: Group related nodes under parent nodes
3. **Versioning**: Track scene file versions for compatibility
4. **Backups**: Keep backups of important scenes
5. **Testing**: Test save/load frequently during development
6. **Documentation**: Comment complex hierarchies in metadata

## Troubleshooting

### Build Errors

**Problem**: Undefined reference to nlohmann::json
```
Solution: Ensure nlohmann-json is installed via vcpkg
```

**Problem**: FRESH_JSON_AVAILABLE not defined
```
Solution: Check CMakeLists.txt and rebuild project
```

### Runtime Errors

**Problem**: "Failed to open scene file"
```
Solution: Check file path and permissions
```

**Problem**: "Error loading scene" with JSON error
```
Solution: Validate JSON syntax with a JSON validator
```

**Problem**: Nodes missing after loading
```
Solution: Check that node names are unique and properly nested
```

## License

The scene serialization system follows the same MIT license as the Fresh Voxel Engine.

---

**Last Updated**: 2026-01-03
**Status**: ✅ Fully Implemented
**Version**: 1.0.0
