# Fresh Voxel Engine - Tools Directory

This directory contains utilities and tools to simplify development and deployment of the Fresh Voxel Engine.

## Directory Structure

```
tools/
└── build_tools/          Automated build and installation scripts
    ├── install.bat       One-click automated installation for Windows
    ├── build.bat         Quick build script (incremental)
    ├── rebuild.bat       Full clean rebuild
    ├── clean.bat         Clean build artifacts
    └── README.md         Detailed build tools documentation
```

## Available Tools

### Build Tools (`build_tools/`)

Windows batch scripts that automate common build tasks:

#### `install.bat` - Automated Installation ⭐ Recommended
Complete automated setup for first-time users:
- Checks prerequisites (CMake, Visual Studio 2022)
- Installs vcpkg package manager (optional)
- Installs dependencies (GLFW, GLM)
- Generates Visual Studio project files
- Builds the engine
- Creates shortcuts

**Usage**: Simply double-click `tools\build_tools\install.bat`

See [build_tools/README.md](build_tools/README.md) for detailed documentation.

#### `build.bat` - Quick Build
Fast incremental build for development:
- Builds without regenerating project files
- Choose Debug/Release/RelWithDebInfo
- Option to run after building

**Usage**: Double-click or run `tools\build_tools\build.bat`

#### `rebuild.bat` - Clean Rebuild
Full rebuild from scratch:
- Cleans build directory
- Regenerates project files
- Builds in Release mode

**Usage**: Double-click or run `tools\build_tools\rebuild.bat`

#### `clean.bat` - Clean Artifacts
Removes all build artifacts:
- Deletes build directory
- Removes generated shortcuts
- Keeps source code intact

**Usage**: Double-click or run `tools\build_tools\clean.bat`

## Quick Reference

### For First-Time Setup
```batch
# Windows
tools\build_tools\install.bat
```

### For Development Builds
```batch
# Quick incremental build
tools\build_tools\build.bat

# Or use Visual Studio
start build\FreshVoxelEngine.sln
```

### For Clean Rebuild
```batch
# Full rebuild
tools\build_tools\rebuild.bat

# Or clean then install
tools\build_tools\clean.bat
tools\build_tools\install.bat
```

## Platform Support

Fresh Voxel Engine build tools are designed for:
- ✅ **Windows 10+** - Full support with automated batch scripts
- Includes Visual Studio 2022 integration
- DirectX 12/11 and OpenGL support

## Requirements

All build tools require:
- Windows 10 or later (64-bit)
- CMake 3.20+
- Visual Studio 2022 (any edition)
- Git (for vcpkg installation)

See [build_tools/README.md](build_tools/README.md) for detailed prerequisites.

## Troubleshooting

### Scripts Won't Run
**Issue**: Double-clicking batch files does nothing or closes immediately

**Solution**:
1. Right-click the batch file
2. Select "Run as administrator"
3. Or open Command Prompt and run manually:
   ```batch
   cd path\to\fresh
   tools\build_tools\install.bat
   ```

### Permission Errors
**Issue**: "Access denied" or permission errors

**Solution**:
- Run Command Prompt as Administrator
- Check antivirus isn't blocking the scripts
- Ensure you have write permissions in the directory

### Path Issues
**Issue**: Scripts can't find CMake or Visual Studio

**Solution**:
- Add CMake to system PATH
- Reinstall Visual Studio 2022
- Run scripts from repository root directory

For more help, see [build_tools/README.md](build_tools/README.md#troubleshooting).

## Future Tools

Planned additions to this directory:
- `package_tools/` - Distribution packaging scripts
- `test_tools/` - Automated testing utilities
- `deploy_tools/` - Deployment automation
- Additional Windows build optimization tools

## Contributing

We welcome contributions to improve these tools! If you create useful scripts or tools:

1. Add them to the appropriate subdirectory
2. Include clear documentation
3. Test on multiple configurations
4. Submit a pull request

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.

## Additional Resources

- 📖 [Getting Started Guide](../GETTING_STARTED.md)
- 🔨 [Developer Setup](../DEVELOPER_SETUP.md)
- 🏗️ [Visual Studio Setup](../VISUAL_STUDIO_SETUP.md)
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)

---

**Fresh Voxel Engine** - Making development easier, one tool at a time.
