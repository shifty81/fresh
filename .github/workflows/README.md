# GitHub Actions Workflows

This directory contains GitHub Actions workflows for automated CI/CD.

## Active Workflows

### ci.yml - Windows-Only Build and Test
**Status**: ✅ Active  
**Trigger**: Push to main/develop, Pull requests to main/develop

Builds and tests the Fresh Voxel Engine on Windows with DirectX support:
- **Windows 10/11** (latest) - DirectX 12/11 primary, OpenGL optional

**Build Matrix**:
- Debug build
- Release build

**Steps**:
1. Install dependencies via vcpkg (DirectX SDK included with Windows SDK)
2. Configure with CMake
3. Build the project
4. Run test suite (FreshVoxelEngineTests.exe)
5. Upload Release build artifacts

**Artifacts**: 
- Windows: `fresh-windows-x64` (7 days retention)

## Archived Cross-Platform Support

**Note:** Fresh Voxel Engine is now Windows-exclusive. Previous cross-platform CI configurations have been archived.

## Dependencies

### Windows (vcpkg)
Uses vcpkg for dependency management:
```powershell
vcpkg install glfw3:x64-windows glew:x64-windows glm:x64-windows imgui:x64-windows openal-soft:x64-windows
```

DirectX 11/12 support is provided by the Windows 10 SDK (included with Visual Studio).

## Test Handling

Tests are run as part of the build process. The CI will fail if tests fail, maintaining code quality.

## Adding New Workflows

When adding new workflows:
1. Use GitHub Actions v4 syntax
2. Use `actions/checkout@v4` and `actions/upload-artifact@v4`
3. Focus on Windows platform only
4. Document dependencies and purpose
5. Test YAML syntax: `python3 -c "import yaml; f = open('.github/workflows/new.yml'); yaml.safe_load(f); f.close(); print('Valid')"`
6. Update this README

## Future Workflows

Planned additions:
- Code coverage reporting (Windows)
- Static analysis (MSVC analyzer, clang-tidy)
- Security scanning (Windows-specific)
- Documentation generation
- Release automation (Windows builds)
- Nightly builds (Windows)
- DirectX shader compilation validation

