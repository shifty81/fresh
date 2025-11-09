# Fresh Voxel Engine - Python Development Tools

This directory contains Python utilities and tools for the Fresh Voxel Engine development workflow. These tools support C++20, .NET 9, and Lua development with asset generation, analysis, and verification capabilities.

## 🐍 Available Tools

### Build & Verification Tools

#### 1. build_verifier.py
**Purpose:** Verify build environment and dependencies before compiling

**Usage:**
```bash
python tools/build_verifier.py
```

**Checks:**
- Python 3.8+ installation
- CMake 3.20+ availability
- Git installation (for vcpkg)
- .NET 9 SDK (optional, for C# bindings)
- Visual Studio 2022 installation
- vcpkg setup

**Benefits:**
- Catch missing dependencies early
- Clear error messages for missing tools
- Saves time debugging build issues

---

### Asset Generation Tools

#### 2. texture_atlas_generator.py
**Purpose:** Generate optimized texture atlases from individual texture files

**Usage:**
```bash
# Basic usage
python tools/texture_atlas_generator.py textures/blocks textures/atlas.png

# With custom sizes
python tools/texture_atlas_generator.py textures/blocks textures/atlas.png --tile-size 32 --atlas-size 8192
```

**Features:**
- Combines multiple textures into single atlas
- Reduces draw calls and improves performance
- Generates metadata file with UV coordinates
- Configurable tile and atlas sizes

**Requirements:**
- Pillow library: `pip install Pillow`

**Output:**
- Atlas PNG file
- Metadata TXT file with texture coordinates

---

#### 3. generate_test_sounds.py
**Purpose:** Generate test WAV audio files for the audio system

**Usage:**
```bash
python tools/generate_test_sounds.py
```

**Generates:**
- `sounds/test_tone.wav` - 440 Hz A note (1 second)
- `sounds/beep.wav` - 880 Hz beep (0.2 seconds)
- `sounds/thud.wav` - 220 Hz thud (0.3 seconds)

**Use Cases:**
- Test audio system without external audio files
- Placeholder sounds during development
- Audio system debugging

---

### Analysis Tools

#### 4. world_analyzer.py
**Purpose:** Analyze saved world files and provide statistics

**Usage:**
```bash
python tools/world_analyzer.py saves/MyWorld.world
```

**Features:**
- File size analysis
- World statistics (when format spec available)
- Debugging save/load issues
- Understanding world structure

**Note:** Full analysis requires world file format specification. Currently provides file size and structure overview.

---

## 🚀 Quick Start

### Prerequisites

Install Python 3.8 or higher:
```bash
python --version  # Should show 3.8+
```

Install optional dependencies:
```bash
# For texture atlas generation
pip install Pillow

# All tools have minimal dependencies
```

### Running Tools

From the repository root directory:
```bash
# Verify build environment
python tools/build_verifier.py

# Generate test sounds
python tools/generate_test_sounds.py

# Generate texture atlas
python tools/texture_atlas_generator.py textures/blocks textures/atlas.png

# Analyze world file
python tools/world_analyzer.py saves/MyWorld.world
```

## 📋 Tool Categories

### Development Support
- ✅ `build_verifier.py` - Environment verification
- 🔄 Future: Dependency updater
- 🔄 Future: Project scaffolding tool

### Asset Pipeline
- ✅ `texture_atlas_generator.py` - Texture atlas creation
- ✅ `generate_test_sounds.py` - Audio file generation
- 🔄 Future: Model converter
- 🔄 Future: Shader compiler
- 🔄 Future: Asset pack validator

### Analysis & Debugging
- ✅ `world_analyzer.py` - World file analysis
- 🔄 Future: Performance profiler
- 🔄 Future: Memory leak detector
- 🔄 Future: Log analyzer

### Build & Deploy
- 🔄 Future: Automated build script
- 🔄 Future: Package creator
- 🔄 Future: Release automation
- 🔄 Future: Documentation generator

## 🛠️ Integration with Engine

### C++20 Engine
Python tools support C++ development:
- Verify build environment before compilation
- Generate assets used by C++ code
- Analyze runtime data and logs

### .NET 9 Bindings
Tools work with managed development:
- Same asset generation workflow
- Compatible with C# game projects
- Shared analysis tools

### Lua Scripting
Python tools complement Lua development:
- Generate assets loaded by Lua scripts
- Validate Lua configuration files
- Analyze Lua script performance

## 🎯 Creating Custom Tools

### Template for New Tools

```python
#!/usr/bin/env python3
"""
Fresh Voxel Engine - Your Tool Name

Brief description of what the tool does.
"""

import sys
from pathlib import Path


class YourTool:
    """Main tool class."""
    
    def __init__(self):
        """Initialize the tool."""
        pass
    
    def process(self):
        """Main processing logic."""
        print("Processing...")
        return True


def main():
    """Main entry point."""
    if len(sys.argv) < 2:
        print("Usage: python your_tool.py <args>")
        return 1
    
    tool = YourTool()
    if tool.process():
        print("Success!")
        return 0
    else:
        print("Failed!")
        return 1


if __name__ == '__main__':
    sys.exit(main())
```

### Adding Your Tool

1. Create script in `tools/` directory
2. Make it executable: `chmod +x tools/your_tool.py`
3. Add documentation to this README
4. Test with multiple scenarios
5. Submit PR with examples

## 📖 Additional Resources

### Engine Documentation
- 📖 [BUILD.md](../BUILD.md) - Complete build instructions
- 🏗️ [ARCHITECTURE.md](../ARCHITECTURE.md) - System architecture
- 🎮 [examples/README.md](../examples/README.md) - Gameplay demos
- 🔷 [DOTNET_INTEGRATION.md](../DOTNET_INTEGRATION.md) - .NET 9 bindings
- 🌙 [LUA_INTEGRATION_GUIDE.md](../LUA_INTEGRATION_GUIDE.md) - Lua scripting

### Development
- 🤝 [CONTRIBUTING.md](../CONTRIBUTING.md) - Contribution guidelines
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [Report Issues](https://github.com/shifty81/fresh/issues)

## 🔧 Platform Support

All Python tools support:
- ✅ **Windows 10/11** (x64) - Full support
- ✅ **Linux** - Most tools (file paths may need adjustment)
- ✅ **macOS** - Most tools (file paths may need adjustment)

Python 3.8+ required on all platforms.

## 📊 Tool Status

| Tool | Status | Python Version | Dependencies |
|------|--------|----------------|--------------|
| build_verifier.py | ✅ Complete | 3.8+ | stdlib |
| texture_atlas_generator.py | ✅ Complete | 3.8+ | Pillow |
| generate_test_sounds.py | ✅ Complete | 3.8+ | stdlib |
| world_analyzer.py | 🟨 Basic | 3.8+ | stdlib |

**Legend:**
- ✅ Complete and tested
- 🟨 Basic functionality, needs enhancement
- 🔄 Planned for future release

---

**Fresh Voxel Engine** - Modern Python tooling for C++20, .NET 9, and Lua development!
