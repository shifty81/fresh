# Fresh Voxel Engine - Tools Directory

This directory contains utilities and tools for the Fresh Voxel Engine.

## Directory Structure

```
tools/
└── (Future tools and utilities will be added here)
```

## Building the Engine

**All installer scripts have been removed in favor of a direct, simpler build approach.**

For step-by-step build instructions, see [BUILD.md](../BUILD.md) in the repository root.

### Quick Build Reference

```batch
# Starting from the project root directory (fresh/)

# 1. Set up vcpkg (one-time)
cd ..
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..\fresh

# 2. Generate Visual Studio solution
generate_vs2022.bat

# 3. Build
cd build
cmake --build . --config Release
```

## Platform Support

Fresh Voxel Engine is designed for:
- ✅ **Windows 10+** - Full support with Visual Studio 2022
- DirectX 12/11 rendering

## Requirements

- Windows 10 or later (64-bit)
- CMake 3.20+
- Visual Studio 2022 (any edition) with "Desktop development with C++"
- Git (for vcpkg)

See [BUILD.md](../BUILD.md) for complete details.

## Future Tools

Planned additions to this directory:
- `package_tools/` - Distribution packaging scripts
- `test_tools/` - Automated testing utilities
- `deploy_tools/` - Deployment automation

## Additional Resources

- 📖 [BUILD.md](../BUILD.md) - Complete build instructions
- 🏗️ [ARCHITECTURE.md](../ARCHITECTURE.md) - System architecture
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)

---

**Fresh Voxel Engine** - Direct and simple Windows development with Visual Studio 2022.
