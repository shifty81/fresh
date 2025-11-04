# GitHub Actions Workflows

This document describes the GitHub Actions workflows used in the Fresh Voxel Engine repository.

## Overview

The repository uses GitHub Actions for continuous integration (CI) on Windows and automated branch cleanup. All workflows are defined in the `.github/workflows/` directory.

**Note:** The CI workflow is currently disabled (renamed to `ci.yml.disabled`). To re-enable it, rename the file back to `ci.yml`.

## Active Workflows

### Branch Cleanup Workflow (`cleanup-branches.yml`)

**Status:** Active

Automatically cleans up old branches to keep the repository organized. This workflow:
- Runs weekly on Sundays at 00:00 UTC
- Can be triggered manually via workflow_dispatch
- Keeps the `main` branch protected
- Keeps the 5 most recent branches (by last commit date)
- Deletes older branches automatically

**Configuration:**
- **Protected Branches:** `main` (never deleted)
- **Keep Count:** 5 most recent branches
- **Schedule:** Weekly (Sundays at 00:00 UTC)

**Manual Trigger:**
1. Go to the GitHub Actions tab
2. Select "Cleanup Old Branches" workflow
3. Click "Run workflow"
4. Select the branch to run from (usually `main`)
5. Click "Run workflow" to execute

The workflow will provide detailed output showing:
- Which branches are kept
- Which branches are deleted
- Success/failure status for each deletion

## CI Workflow (`ci.yml`)

The main CI workflow runs on every push to `main` and `develop` branches, as well as on all pull requests targeting these branches.

### Jobs

#### **build-windows**
Builds and tests the project on Windows with DirectX and OpenGL support.

**Build Matrix:**
- **Build Types:** Debug, Release

**Steps:**
1. Checks out the repository
2. Sets up DirectX SDK (included with Windows SDK on GitHub runners)
3. Installs GLFW via vcpkg
4. Configures CMake with vcpkg toolchain
5. Builds the project using the configured build type
6. Runs CTest test suite with verbose output
7. Uploads build artifacts (Release build only)
   - Artifact name: `fresh-windows-x64`
   - Contents: FreshVoxelEngine.exe and shaders
   - Retention: 7 days

**Purpose:** Ensures the code builds correctly on Windows with DirectX 12/11 and OpenGL support, and passes all tests.

## Workflow Configuration

### Enabling/Disabling Workflows

To enable the CI workflow:
```batch
cd .github\workflows
rename ci.yml.disabled ci.yml
git add .
git commit -m "Enable CI workflow"
git push
```

To disable the CI workflow:
```batch
cd .github\workflows
rename ci.yml ci.yml.disabled
git add .
git commit -m "Disable CI workflow"
git push
```

### Customizing Build Matrix

You can customize the build matrix in `ci.yml`:

```yaml
strategy:
  matrix:
    build_type: [Debug, Release, RelWithDebInfo]
```

### Adding New Jobs

To add a new job to the workflow:

1. Edit `.github/workflows/ci.yml`
2. Add a new job under `jobs:`:
   ```yaml
   my-new-job:
     runs-on: windows-latest
     steps:
       - uses: actions/checkout@v3
       - name: Run custom step
         run: echo "Hello from new job"
   ```

## Build Artifacts

### Accessing Artifacts

Artifacts from successful builds are available for download:
1. Go to the GitHub Actions tab
2. Click on a completed workflow run
3. Scroll to the "Artifacts" section at the bottom
4. Download the artifact you need

### Artifact Structure

Windows artifacts (`fresh-windows-x64.zip`):
```
fresh-windows-x64/
├── FreshVoxelEngine.exe
└── shaders/
    ├── voxel.vert
    └── voxel.frag
```

## Local Testing

To test the workflow steps locally:

### Build and Test

```batch
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON

# Build
cmake --build build --config Release

# Test
cd build
ctest -C Release --output-on-failure
```

### Using vcpkg

```batch
# Set up vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install glfw3:x64-windows glm:x64-windows

# Build with vcpkg
cd ..\fresh
cmake -B build -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

## Troubleshooting

### Common Issues

#### Workflow Fails to Find Dependencies

**Problem:** CMake can't find GLFW or GLM

**Solution:** Ensure vcpkg is properly integrated:
```yaml
- name: Configure CMake
  run: |
    cmake -B build `
      -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_INSTALLATION_ROOT\scripts\buildsystems\vcpkg.cmake
```

#### DirectX Compilation Errors

**Problem:** DirectX headers or libraries not found

**Solution:** Windows runners include Windows SDK by default. Verify CMakeLists.txt links:
```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE 
    d3d11.lib 
    d3d12.lib 
    dxgi.lib 
    d3dcompiler.lib
)
```

#### Test Failures

**Problem:** Tests fail in CI but pass locally

**Solution:** Check for:
- Path separator differences (use CMake's file paths)
- Hardcoded absolute paths
- Missing test data files

### Viewing Logs

To view detailed logs:
1. Click on the failed workflow run
2. Click on the failed job
3. Expand the failed step to see full output

For test failures:
```batch
# Run tests with verbose output locally
cd build
ctest -C Release --output-on-failure --verbose
```

## Best Practices

### Before Pushing

Always test locally before pushing:
```batch
# Full local test
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build build --config Release
cd build
ctest -C Release --output-on-failure
```

### Pull Requests

- Ensure CI passes before requesting review
- Fix any workflow failures promptly
- Update documentation if workflow changes

### Workflow Updates

When modifying workflows:
1. Test changes in a fork first
2. Document any new steps or requirements
3. Update this document if adding new jobs

## Additional Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CMake Documentation](https://cmake.org/documentation/)
- [vcpkg Documentation](https://github.com/Microsoft/vcpkg)
- [Fresh Voxel Engine Build Guide](VISUAL_STUDIO_SETUP.md)
