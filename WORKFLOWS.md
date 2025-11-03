# GitHub Actions Workflows

This document describes the GitHub Actions workflows used in the Fresh Voxel Engine repository.

## Overview

The repository uses GitHub Actions for continuous integration (CI) and quality assurance. All workflows are defined in the `.github/workflows/` directory.

## CI Workflow (`ci.yml`)

The main CI workflow runs on every push to `main` and `develop` branches, as well as on all pull requests targeting these branches.

### Jobs

#### 1. **build-linux**
Builds and tests the project on Ubuntu Linux with multiple compiler configurations.

**Build Matrix:**
- **Build Types:** Debug, Release
- **Compilers:**
  - GCC 11 (gcc-11/g++-11)
  - GCC 12 (gcc-12/g++-12)
  - Clang 14 (clang-14/clang++-14)

**Steps:**
1. Checks out the repository
2. Installs dependencies (CMake, Ninja, Vulkan SDK, GLFW, GLM, GoogleTest, Vulkan tools)
3. Configures CMake with Ninja generator
4. Builds the project using the configured build type
5. Runs CTest test suite with verbose output
6. Uploads build artifacts (Release build with GCC 12 only)
   - Artifact name: `fresh-linux-x64`
   - Contents: FreshVoxelEngine binary and shaders
   - Retention: 7 days

**Purpose:** Ensures the code builds correctly on Linux with multiple compilers and passes all tests.

#### 2. **build-windows**
Builds and tests the project on Windows.

**Build Matrix:**
- **Build Types:** Debug, Release

**Steps:**
1. Checks out the repository
2. Sets up Vulkan SDK using humbletim/setup-vulkan-sdk action
3. Installs GLFW via vcpkg
4. Configures CMake with vcpkg toolchain
5. Builds the project using the configured build type
6. Runs CTest test suite with verbose output
7. Uploads build artifacts (Release build only)
   - Artifact name: `fresh-windows-x64`
   - Contents: FreshVoxelEngine.exe and shaders
   - Retention: 7 days

**Purpose:** Ensures the code builds correctly on Windows and passes all tests.

#### 3. **build-macos**
Builds and tests the project on macOS.

**Build Matrix:**
- **Build Types:** Debug, Release

**Steps:**
1. Checks out the repository
2. Installs dependencies via Homebrew (CMake, Ninja, GLFW, Vulkan headers/loader, MoltenVK, GLM, GoogleTest)
3. Configures CMake with Ninja generator
4. Builds the project using the configured build type
5. Runs CTest test suite with verbose output
6. Uploads build artifacts (Release build only)
   - Artifact name: `fresh-macos-arm64`
   - Contents: FreshVoxelEngine binary and shaders
   - Retention: 7 days

**Purpose:** Ensures the code builds correctly on macOS (including Apple Silicon) and passes all tests.

#### 4. **code-quality**
Performs static code analysis and formatting checks.

**Steps:**
1. Checks out the repository
2. Installs analysis tools (clang-format, clang-tidy, cppcheck)
3. Runs clang-format to check code formatting compliance
4. Runs cppcheck for static analysis (checks for potential bugs, undefined behavior, etc.)

**Purpose:** Maintains code quality standards and catches potential issues early.

**Note:** Steps are configured with `continue-on-error: true`, meaning failures won't block the workflow.

#### 5. **documentation**
Validates documentation files and structure.

**Steps:**
1. Checks out the repository
2. Checks for broken links in Markdown files
   - Scans all `.md` files
   - Validates relative links to other Markdown files
3. Validates presence of required documentation files:
   - README.md
   - CONTRIBUTING.md
   - CODE_OF_CONDUCT.md
   - LICENSE
   - CHANGELOG.md
   - ARCHITECTURE.md

**Purpose:** Ensures documentation is complete and all internal links are valid.

**Note:** Steps are configured with `continue-on-error: true`, meaning failures won't block the workflow.

## Workflow Artifacts

The CI workflow produces downloadable artifacts for Release builds on all three platforms:

- **Linux (GCC 12):** `fresh-linux-x64`
- **Windows:** `fresh-windows-x64`
- **macOS:** `fresh-macos-arm64`

All artifacts include the compiled executable and shader files, and are retained for 7 days.

## Triggers

All jobs in the CI workflow are triggered by:
- **Push events** to `main` or `develop` branches
- **Pull request events** targeting `main` or `develop` branches

## Dependencies

The workflows require the following external dependencies:
- **Linux:** Vulkan SDK, GLFW, GLM, GoogleTest, various compilers
- **Windows:** Vulkan SDK, GLFW (via vcpkg)
- **macOS:** Vulkan headers/loader, MoltenVK, GLFW, GLM, GoogleTest (via Homebrew)

## Status Badges

You can add status badges to your README.md to show the current state of the CI workflow:

```markdown
![CI](https://github.com/shifty81/fresh/workflows/CI/badge.svg)
```

## Running Workflows Manually

Workflows can be triggered manually from the GitHub Actions tab in the repository, though the current configuration only includes automatic triggers.

## Extending the Workflows

To add new workflows or modify existing ones:

1. Create or edit YAML files in `.github/workflows/`
2. Follow GitHub Actions syntax
3. Test changes in a pull request before merging to main branches
4. Consider adding status checks in branch protection rules

## See Also

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contributing guidelines
- [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) - Development environment setup
