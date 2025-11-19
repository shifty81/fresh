# Contributing to Fresh Voxel Engine

Thank you for your interest in contributing to Fresh Voxel Engine! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Features](#suggesting-features)

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## Getting Started

### Prerequisites

Before you begin, ensure you have the following installed:

- **Windows 10/11** (x64) - Primary development platform
- **Visual Studio 2022** with "Desktop development with C++" workload
- **CMake 3.20 or higher**
- **Git for Windows**
- **vcpkg** (for dependency management)

**Note**: The engine is Windows-exclusive. On Windows, native Win32 APIs are used for windowing (GLFW is only used for Linux CI testing).

### Setting Up Development Environment

1. **Fork the Repository**
   ```bash
   # Fork via GitHub UI, then clone your fork
   git clone https://github.com/YOUR_USERNAME/fresh.git
   cd fresh
   ```

2. **Add Upstream Remote**
   ```bash
   git remote add upstream https://github.com/shifty81/fresh.git
   git fetch upstream
   ```

3. **Build the Project**
   ```bash
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ```

4. **Run Tests** (when available)
   ```bash
   ctest
   ```

## Development Process

### Branch Naming Convention

Use descriptive branch names that indicate the purpose of your changes:

- `feature/` - New features (e.g., `feature/inventory-system`)
- `fix/` - Bug fixes (e.g., `fix/chunk-loading-crash`)
- `docs/` - Documentation updates (e.g., `docs/improve-readme`)
- `refactor/` - Code refactoring (e.g., `refactor/physics-system`)
- `test/` - Adding tests (e.g., `test/terrain-generation`)

### Workflow

1. **Create a branch** from `main`
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes**
   - Write clear, concise commit messages
   - Follow the coding standards
   - Add tests for new functionality
   - Update documentation as needed

3. **Keep your branch updated**
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

4. **Test your changes**
   ```bash
   cd build
   make
   ./FreshVoxelEngine
   ```

5. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request**

## Coding Standards

### C++ Style Guide

We follow modern C++ best practices. See [CPP_GUIDELINES.md](CPP_GUIDELINES.md) for detailed guidelines.

**Key Points:**

- Use C++20 features appropriately
- Prefer `std::unique_ptr` and `std::shared_ptr` over raw pointers
- Use RAII for resource management
- Follow the Rule of Zero/Five
- Prefer STL algorithms over raw loops
- Use meaningful variable and function names
- Keep functions small and focused (single responsibility)

### Formatting

- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Maximum 100 characters
- **Braces**: Opening brace on same line (K&R style)
- **Naming Conventions**:
  - Classes/Structs: `PascalCase` (e.g., `VoxelWorld`)
  - Functions/Methods: `camelCase` (e.g., `generateChunk()`)
  - Variables: `camelCase` (e.g., `chunkSize`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_CHUNK_SIZE`)
  - Private members: prefix with `m_` (e.g., `m_vertexBuffer`)

### Example Code

```cpp
#pragma once

#include <memory>
#include <vector>

namespace fresh {

// Good class example
class ChunkManager {
public:
    ChunkManager(int viewDistance);
    ~ChunkManager() = default;
    
    // No copy, move only
    ChunkManager(const ChunkManager&) = delete;
    ChunkManager& operator=(const ChunkManager&) = delete;
    ChunkManager(ChunkManager&&) = default;
    ChunkManager& operator=(ChunkManager&&) = default;
    
    void loadChunk(int x, int z);
    void unloadChunk(int x, int z);
    
private:
    int m_viewDistance;
    std::vector<std::unique_ptr<Chunk>> m_loadedChunks;
    
    bool isChunkInRange(int x, int z) const;
};

} // namespace fresh
```

### Documentation

- Use Doxygen-style comments for public APIs
- Document complex algorithms and non-obvious code
- Update relevant documentation files when changing functionality

```cpp
/**
 * @brief Generates terrain for a chunk using procedural generation
 * 
 * Uses multi-octave Perlin noise to create realistic terrain with biomes,
 * caves, and height variations.
 * 
 * @param chunkX The X coordinate of the chunk in world space
 * @param chunkZ The Z coordinate of the chunk in world space
 * @param seed The world seed for reproducible generation
 * @return std::unique_ptr<Chunk> The generated chunk
 */
std::unique_ptr<Chunk> generateChunk(int chunkX, int chunkZ, uint32_t seed);
```

## Pull Request Process

### Before Submitting

- [ ] Code builds without errors or warnings
- [ ] All tests pass (when test infrastructure is available)
- [ ] Code follows style guidelines
- [ ] Documentation is updated
- [ ] Commit messages are clear and descriptive
- [ ] Branch is up-to-date with `main`

### PR Description Template

Your PR description should include:

1. **Summary**: Brief description of changes
2. **Motivation**: Why is this change needed?
3. **Implementation**: How did you implement it?
4. **Testing**: How did you test the changes?
5. **Screenshots**: If applicable (UI/rendering changes)
6. **Breaking Changes**: Any breaking API changes?
7. **Related Issues**: Link to related issues

### Review Process

1. Maintainers will review your PR
2. Address feedback by pushing new commits
3. Once approved, a maintainer will merge your PR
4. Your contribution will be included in the next release

### PR Checklist

```markdown
## Description
[Describe your changes]

## Type of Change
- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Testing
- [ ] Built and tested locally
- [ ] Added/updated tests (if applicable)
- [ ] All tests pass

## Documentation
- [ ] Updated README.md (if needed)
- [ ] Updated relevant documentation
- [ ] Added inline code comments

## Screenshots (if applicable)
[Add screenshots]
```

## Reporting Bugs

### Before Reporting

1. Check existing issues to avoid duplicates
2. Try to reproduce the bug on the latest version
3. Gather relevant information (OS, compiler, versions)

### Bug Report Template

When filing a bug report, include:

```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '...'
3. See error

**Expected behavior**
What you expected to happen.

**Screenshots**
If applicable, add screenshots.

**Environment:**
 - OS: [e.g., Windows 11]
 - Compiler: [e.g., MSVC 19.35 / Visual Studio 2022]
 - Graphics API: [DirectX 12/DirectX 11/OpenGL version]
 - GPU: [e.g., NVIDIA RTX 3060]

**Additional context**
Add any other context about the problem.
```

## Suggesting Features

We welcome feature suggestions! Here's how to propose new features:

### Feature Request Template

```markdown
**Is your feature request related to a problem?**
A clear description of the problem.

**Describe the solution you'd like**
A clear and concise description of what you want to happen.

**Describe alternatives you've considered**
Other solutions or features you've considered.

**Additional context**
Add any other context or screenshots about the feature request.

**Implementation ideas (optional)**
If you have ideas about how to implement this feature.
```

### Feature Discussion

1. Open an issue with the `feature-request` label
2. Discuss the feature with maintainers and community
3. If approved, you can start working on it
4. Reference the issue in your PR

## Contributing to Different Areas

### Engine Systems

- **Rendering**: OpenGL/DirectX rendering, shaders, graphics pipeline
- **Voxel World**: Chunk management, meshing, world generation
- **Physics**: Collision detection, physics simulation
- **AI**: Behavior trees, pathfinding, NPC systems
- **Networking**: Multiplayer, client-server architecture
- **Audio**: Sound effects, music, 3D audio

### Documentation

- Improve existing documentation
- Write tutorials and guides
- Create example projects
- Record video tutorials
- Translate documentation

### Testing

- Write unit tests
- Create integration tests
- Perform manual testing
- Report bugs and edge cases

### Asset Creation

- Create example asset packs
- Design voxel models
- Create textures
- Compose music/sound effects

## Community

### Communication Channels

- **GitHub Issues**: Bug reports and feature requests
- **GitHub Discussions**: General questions and discussions
- **Pull Requests**: Code contributions

### Getting Help

If you need help:

1. Check the documentation in `docs/`
2. Search existing issues
3. Ask in GitHub Discussions
4. Reach out to maintainers

## License

By contributing to Fresh Voxel Engine, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).

## Recognition

Contributors will be acknowledged in:
- CONTRIBUTORS.md file
- Release notes
- Project README

Thank you for contributing to Fresh Voxel Engine! 🎮
