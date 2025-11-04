# Changelog

All notable changes to Fresh Voxel Engine will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **ImGui DirectX Rendering Backends** - Complete DirectX 11/12 support for ImGui
  - Implemented DirectX 11 ImGui rendering backend
  - Implemented DirectX 12 ImGui rendering backend
  - Added SRV descriptor heap to DirectX12RenderContext for ImGui
  - Added public getters to access DirectX device/context for ImGui integration
  - Full support for ImGui initialization, frame rendering, and cleanup

### Changed
- **Removed Vulkan Support** - Simplified graphics stack
  - Removed Vulkan-specific renderer, device, and pipeline classes
  - Updated engine to use OpenGL and DirectX backends only
  - Updated all documentation to reflect OpenGL/DirectX support
  - Simplified build dependencies (no longer requires Vulkan SDK)

### Added
- Comprehensive project documentation
  - CONTRIBUTING.md guide
  - CODE_OF_CONDUCT.md
  - ARCHITECTURE.md with system diagrams
  - LICENSE (MIT)
  - CHANGELOG.md
- GitHub issue templates
- GitHub pull request template
- CI/CD workflow with GitHub Actions
- Example code and tutorials

## [0.2.0] - 2025-11-03

### Added
- **Modular Asset System** - Complete asset pack support with auto-discovery
  - Auto-scan asset directory for packs
  - JSON manifest-based asset definitions
  - 7 placement rules (Random, Clustered, Grid, NearWater, OnHills, InCaves, RiverSide)
  - Biome-specific asset placement
  - Smart placement with distance enforcement
  - Asset pack template generation
- **Enhanced Core Systems**
  - Memory management with pool allocators
  - Event system for decoupled communication
  - Scene graph with transformations
  - Spatial hashing for collision optimization
  - Hot-reloading for resources

### Completed
- All 10 core engine systems (100% foundation)
- Player controller with first-person camera
- Chunk streaming for infinite worlds
- Enhanced physics with comprehensive collision detection
- Input management with rebindable controls
- Resource management with caching
- Audio engine framework (ready for backend)
- Scripting engine framework (ready for backend)

### Documentation
- Created comprehensive documentation suite
  - ROADMAP.md - Development roadmap
  - COMPLETE_STATUS.md - Implementation status
  - GETTING_STARTED.md - Quick start guide
  - CPP_GUIDELINES.md - C++ development standards
  - IMPLEMENTATION_SUMMARY.md - Feature summary
  - docs/TERRAFORMING.md - Terraforming guide

## [0.1.0] - 2025-10-15

### Added
- **Core Engine Foundation**
  - CMake-based build system
  - Engine initialization and main loop
  - Window management with GLFW
  
- **Multi-API Rendering System**
  - OpenGL 4.5+ renderer implementation
  - DirectX 11 renderer (Windows)
  - DirectX 12 renderer (Windows)
  - ShaderManager with API-agnostic support
  - Basic vertex and fragment shaders

- **Voxel World System**
  - Chunk-based world (16x256x16 voxels)
  - 13 voxel types (Air, Stone, Dirt, Grass, Sand, Water, Wood, Leaves, Coal, Iron, Gold, Diamond, Bedrock)
  - Face culling optimization
  - Greedy meshing algorithm

- **Procedural Generation**
  - Perlin noise implementation
  - Multi-octave fractal noise
  - TerrainGenerator with biomes
  - 3D noise for cave generation
  - Height-based biome selection

- **World Editing System**
  - TerraformingSystem with 10 tools
    - Single Block, Brush, Sphere, Filled Sphere
    - Cube, Filled Cube, Line
    - Flatten, Smooth, Paint
  - Undo/Redo system (100 operations)
  - WorldEditor integration
  - EditorGUI framework (console-based)

- **Serialization**
  - WorldSerializer (binary format)
  - Chunk compression
  - World save/load functionality

- **UI Systems**
  - Main Menu (console-based)
  - Create/Load world functionality
  - World management

- **Foundation Systems**
  - PhysicsSystem foundation
  - RaycastSystem foundation
  - AISystem framework with behavior trees

### Technical Details
- C++17 standard
- Modern CMake configuration
- Windows-native with DirectX 12 support
- Modular architecture
- Clean code structure

## Release Types

### Major Releases (X.0.0)
- Significant architectural changes
- Breaking API changes
- Major new features

### Minor Releases (0.X.0)
- New features
- Non-breaking API additions
- Significant improvements

### Patch Releases (0.0.X)
- Bug fixes
- Performance improvements
- Documentation updates
- Minor tweaks

## Upcoming Features

See [ROADMAP.md](ROADMAP.md) for detailed future plans.

### Next Release (0.3.0) - Planned
- ImGui integration for UI
- Complete rendering pipeline for all backends
- Inventory system implementation
- Resource gathering mechanics
- Day/night cycle
- Weather system foundation

### Future Releases
- Multiplayer networking (0.4.0)
- Profession system (0.5.0)
- Advanced AI and NPCs (0.6.0)
- Audio system integration (0.7.0)
- Full release (1.0.0)

## Links

- [Repository](https://github.com/shifty81/fresh)
- [Issues](https://github.com/shifty81/fresh/issues)
- [Discussions](https://github.com/shifty81/fresh/discussions)
- [Contributing Guide](CONTRIBUTING.md)

---

**Note**: This project is in active development. Features and APIs may change between releases.
