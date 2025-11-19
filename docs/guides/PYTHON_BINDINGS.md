# Python Bindings for Tooling and Automation

## Overview

Fresh Engine includes Python bindings via pybind11 for tooling, automation, and asset processing. This allows Python scripts to interact with the engine's C++ core for:

- Asset validation and processing
- Build automation
- World data analysis
- Batch operations
- Testing utilities

## Architecture

```
┌─────────────────────────────────────┐
│   Python Scripts (automation)       │
│   - Asset validation                │
│   - Build pipelines                 │
│   - Data processing                 │
└─────────────────────────────────────┘
              ↓ pybind11
┌─────────────────────────────────────┐
│   Fresh Engine C++ Module           │
│   - AssetProcessing                 │
│   - WorldDataAccess                 │
│   - BuildAutomation                 │
└─────────────────────────────────────┘
```

## Quick Start

### 1. Build with Python Support

Python bindings are optional. To enable them:

```bash
# Ensure Python 3.8+ is installed
python --version

# Build the engine (Python support detected automatically)
cmake -B build -S .
cmake --build build
```

### 2. Import Fresh Engine Module

```python
import fresh_engine as fe

# Asset processing
textures = fe.AssetProcessing.get_supported_texture_formats()
print(f"Supported formats: {textures}")

fe.AssetProcessing.load_texture("textures/dirt.png")

# World data access
fe.WorldDataAccess.load_world("saves/world1")
info = fe.WorldDataAccess.get_world_info("saves/world1")
print(info)

# Build automation
fe.BuildAutomation.validate_assets("Assets/")
manifest = fe.BuildAutomation.generate_manifest("Assets/")
```

## Available Modules

### AssetProcessing

Asset loading and processing utilities.

**Methods:**
- `load_texture(filepath: str) -> bool`: Load and validate texture
- `get_supported_texture_formats() -> List[str]`: Get supported formats
- `process_directory(directory: str, pattern: str) -> int`: Process files

**Example:**
```python
import fresh_engine as fe

# Validate textures
if fe.AssetProcessing.load_texture("grass.png"):
    print("Texture valid")

# Get formats
formats = fe.AssetProcessing.get_supported_texture_formats()
print(f"Formats: {', '.join(formats)}")

# Batch process
count = fe.AssetProcessing.process_directory("textures/", "*.png")
print(f"Processed {count} files")
```

### WorldDataAccess

World and chunk data access.

**Methods:**
- `load_world(world_path: str) -> bool`: Load world data
- `get_world_info(world_path: str) -> str`: Get world information
- `get_chunk_count() -> int`: Get number of loaded chunks

**Example:**
```python
import fresh_engine as fe

# Load world
if fe.WorldDataAccess.load_world("saves/TestWorld"):
    info = fe.WorldDataAccess.get_world_info("saves/TestWorld")
    chunks = fe.WorldDataAccess.get_chunk_count()
    print(f"World loaded: {info}")
    print(f"Chunks: {chunks}")
```

### BuildAutomation

Build pipeline and validation tools.

**Methods:**
- `validate_assets(directory: str) -> bool`: Validate asset directory
- `generate_manifest(directory: str) -> str`: Generate asset manifest (JSON)

**Example:**
```python
import fresh_engine as fe
import json

# Validate assets
if fe.BuildAutomation.validate_assets("Assets/"):
    print("Assets valid")
    
# Generate manifest
manifest_json = fe.BuildAutomation.generate_manifest("Assets/")
manifest = json.loads(manifest_json)
print(f"Manifest: {manifest}")
```

## Example Scripts

### Asset Validator

Validate all assets in a directory:

```python
#!/usr/bin/env python3
import fresh_engine as fe
import os
import sys

def validate_assets(directory):
    """Validate assets in directory"""
    if not os.path.exists(directory):
        print(f"Error: Directory not found: {directory}")
        return False
    
    print(f"Validating assets in: {directory}")
    
    # Use Fresh Engine validator
    result = fe.BuildAutomation.validate_assets(directory)
    
    if result:
        print("✓ All assets valid")
    else:
        print("✗ Asset validation failed")
    
    return result

if __name__ == "__main__":
    directory = sys.argv[1] if len(sys.argv) > 1 else "Assets/"
    validate_assets(directory)
```

### Batch Texture Processor

Process all textures in a directory:

```python
#!/usr/bin/env python3
import fresh_engine as fe
from pathlib import Path

def process_textures(directory):
    """Process all texture files"""
    formats = fe.AssetProcessing.get_supported_texture_formats()
    
    for format in formats:
        count = fe.AssetProcessing.process_directory(
            directory, 
            f"*.{format}"
        )
        if count > 0:
            print(f"Processed {count} {format} files")

if __name__ == "__main__":
    process_textures("textures/")
```

### World Analyzer

Analyze world save files:

```python
#!/usr/bin/env python3
import fresh_engine as fe
import sys

def analyze_world(world_path):
    """Analyze world data"""
    print(f"Analyzing world: {world_path}")
    
    if not fe.WorldDataAccess.load_world(world_path):
        print("Failed to load world")
        return
    
    info = fe.WorldDataAccess.get_world_info(world_path)
    chunks = fe.WorldDataAccess.get_chunk_count()
    
    print(f"\nWorld Information:")
    print(f"  {info}")
    print(f"  Chunks loaded: {chunks}")

if __name__ == "__main__":
    world_path = sys.argv[1] if len(sys.argv) > 1 else "saves/world1"
    analyze_world(world_path)
```

## Integration with Existing Tools

The Fresh Engine repository includes Python tools that can use these bindings:

- `tools/build_verifier.py`: Build environment checker
- `tools/texture_atlas_generator.py`: Texture atlas generation
- `tools/world_analyzer.py`: World data analysis
- `tools/automation/asset_validator.py`: Comprehensive asset validation

## Build Pipeline Integration

### Pre-Build Validation

```python
# pre_build.py
import fresh_engine as fe
import sys

def pre_build_check():
    """Run pre-build validation"""
    print("Running pre-build checks...")
    
    # Validate assets
    if not fe.BuildAutomation.validate_assets("Assets/"):
        print("Asset validation failed!")
        sys.exit(1)
    
    print("Pre-build checks passed!")

if __name__ == "__main__":
    pre_build_check()
```

Add to CMakeLists.txt:
```cmake
add_custom_target(PreBuildCheck
    COMMAND python ${CMAKE_SOURCE_DIR}/tools/pre_build.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running pre-build validation"
)

add_dependencies(${PROJECT_NAME} PreBuildCheck)
```

### Post-Build Packaging

```python
# post_build.py
import fresh_engine as fe
import json

def post_build_package():
    """Create asset package after build"""
    print("Generating asset manifest...")
    
    manifest_json = fe.BuildAutomation.generate_manifest("Assets/")
    manifest = json.loads(manifest_json)
    
    # Write manifest file
    with open("build/asset_manifest.json", "w") as f:
        json.dump(manifest, f, indent=2)
    
    print("Asset manifest created!")

if __name__ == "__main__":
    post_build_package()
```

## Performance Considerations

### Best Practices

1. **Minimize C++/Python crossings**: Batch operations
2. **Use native Python for file I/O**: Only call C++ for complex operations
3. **Cache results**: Store frequently accessed data in Python
4. **Profile scripts**: Use Python profiler for optimization

### Example: Efficient Batch Processing

```python
# Bad: Multiple C++ calls
for texture in texture_files:
    fe.AssetProcessing.load_texture(texture)

# Good: Single C++ call
fe.AssetProcessing.process_directory("textures/", "*.png")
```

## Troubleshooting

### Module Not Found

If you see `ModuleNotFoundError: No module named 'fresh_engine'`:

1. Check Python bindings were compiled:
   ```bash
   # Look for fresh_engine.pyd (Windows) or fresh_engine.so (Linux)
   find build -name "fresh_engine.*"
   ```

2. Add to Python path:
   ```python
   import sys
   sys.path.append("path/to/build")
   import fresh_engine
   ```

3. Verify Python version matches build:
   ```bash
   python --version
   # Should match Python used during CMake configuration
   ```

### Import Errors

- Ensure pybind11 is available
- Check C++ library dependencies are in PATH
- Verify Python architecture matches (x64 vs x86)

## Extending the Bindings

To add new functions to the Python bindings:

1. Add C++ function to appropriate class in `PythonBindings.h`
2. Implement function in `PythonBindings.cpp`
3. Add pybind11 binding in `PYBIND11_MODULE` section
4. Rebuild and test

Example:
```cpp
// In PythonBindings.h
class AssetProcessing {
public:
    static bool optimizeTexture(const std::string& filepath);
};

// In PythonBindings.cpp
bool AssetProcessing::optimizeTexture(const std::string& filepath) {
    // Implementation...
}

// In PYBIND11_MODULE
py::class_<fresh::python::AssetProcessing>(m, "AssetProcessing")
    .def_static("optimize_texture", &fresh::python::AssetProcessing::optimizeTexture);
```

## See Also

- [pybind11 Documentation](https://pybind11.readthedocs.io/)
- [Python Tools README](../tools/README.md)
- [Build System Documentation](../BUILD.md)
- [Asset Management Guide](../ASSET_PACK_GUIDE.md)
