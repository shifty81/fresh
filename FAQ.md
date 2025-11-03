# Frequently Asked Questions (FAQ)

## General Questions

### What is Fresh Voxel Engine?

Fresh Voxel Engine is a modern, open-source voxel game engine built with C++17 and Vulkan. It features procedural terrain generation, a powerful world editor, and supports creating voxel-based games and applications.

### What can I build with it?

- Minecraft-like games
- Voxel-based sandbox games
- Terrain visualization tools
- Procedural world generators
- Educational projects
- Game prototypes

### Is it free?

Yes! Fresh Voxel Engine is released under the MIT License, which means:
- ✅ Free to use
- ✅ Free to modify
- ✅ Free for commercial projects
- ✅ Open source

### What platforms are supported?

- **Linux**: Ubuntu 20.04+, Fedora, Arch, etc.
- **Windows**: Windows 10+
- **macOS**: macOS 11+ (with MoltenVK)

### What are the system requirements?

**Minimum:**
- Vulkan 1.2 capable GPU
- 4-core CPU
- 8 GB RAM
- 2 GB storage

**Recommended:**
- Modern GPU (RTX 2060, RX 5700, or better)
- 8-core CPU
- 16 GB RAM
- 10 GB storage

## Getting Started

### How do I install it?

See [GETTING_STARTED.md](GETTING_STARTED.md) for detailed instructions:

```bash
git clone https://github.com/shifty81/fresh.git
cd fresh
mkdir build && cd build
cmake ..
make
./FreshVoxelEngine
```

### I'm getting build errors. What should I do?

1. Check [DEVELOPER_SETUP.md](DEVELOPER_SETUP.md) for prerequisites
2. Ensure Vulkan SDK is installed
3. Check that GLFW is available
4. See [Troubleshooting](#troubleshooting) section below
5. Search existing [GitHub Issues](https://github.com/shifty81/fresh/issues)
6. Create a new issue if not found

### How do I create my first world?

Follow [Tutorial 1: Your First Voxel World](docs/tutorials/tutorial_01_first_world.md) for a step-by-step guide.

Basic steps:
1. Initialize engine and window
2. Create VoxelWorld
3. Generate terrain with TerrainGenerator
4. Add player and camera
5. Run main loop

### Where can I find examples?

Check the [examples/](examples/) directory for:
- Basic setup
- Terrain generation
- Player controller
- World editing
- Asset loading

## Features

### Does it support multiplayer?

Multiplayer is planned for v0.4.0 (Q1 2026). The architecture supports it, but implementation is not yet complete.

### Can I create custom voxel types?

Yes! Edit `include/voxel/VoxelTypes.h` to add new voxel types. The engine supports up to 256 types.

### How does terrain generation work?

The engine uses multi-octave Perlin noise for realistic terrain:
- Height maps for surface
- 3D noise for caves
- Biome-based block selection
- Seed-based reproducibility

See [TerrainGenerator](docs/tutorials/tutorial_01_first_world.md#step-4-generate-initial-chunks) for details.

### What's the world size limit?

Technically unlimited! The engine uses chunk streaming:
- Chunks are 16×256×16 voxels
- Loaded dynamically around player
- Only visible chunks in memory
- Limited by available storage

### Can I import custom models?

Yes, through the modular asset system:
- Supports OBJ, FBX, GLTF
- Create asset packs with JSON manifests
- Auto-discovery on engine start
- See [ModularAssetSystem](docs/api/README.md)

### Does it have physics?

Yes, the physics system includes:
- Gravity
- Collision detection (AABB, sphere, ray)
- Voxel-world collision
- Spatial partitioning
- More physics features coming in future updates

## Development

### How can I contribute?

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

We welcome:
- Code contributions
- Documentation improvements
- Bug reports
- Feature suggestions
- Asset creation

### What coding standards do you follow?

See [CPP_GUIDELINES.md](CPP_GUIDELINES.md) for complete guidelines:
- C++17 standard
- 4-space indentation
- PascalCase for classes
- camelCase for functions
- RAII for resource management
- Modern C++ practices

### How do I run tests?

Test infrastructure is under development. When available:

```bash
cd build
ctest --output-on-failure
```

### Can I use this in a commercial project?

Yes! The MIT License allows commercial use. You can:
- Sell games built with it
- Modify and sell the engine
- Use it in proprietary projects
- No royalties required

Just keep the license notice in your project.

### How do I add a new feature?

1. Check [ROADMAP.md](ROADMAP.md) to see if it's planned
2. Open an issue to discuss the feature
3. Fork and create a feature branch
4. Implement the feature
5. Add tests and documentation
6. Submit a pull request

## Technical Questions

### Why Vulkan instead of OpenGL?

Vulkan provides:
- Better performance and lower overhead
- Multi-threaded rendering
- Modern GPU features
- Better control over GPU
- Future-proof API

### How is performance?

On modern hardware:
- 60+ FPS at 1080p
- Hundreds of chunks loaded
- Smooth chunk streaming
- Optimized mesh generation

Performance optimizations:
- Greedy meshing (90% face reduction)
- Frustum culling
- Chunk streaming
- Spatial partitioning

### What about memory usage?

Typical memory usage:
- ~200-500 KB per chunk (with mesh)
- ~200 chunks loaded = ~100 MB
- Texture atlas: ~64 MB
- Total: ~200-500 MB typical

Memory management features:
- Object pooling
- Smart pointers
- Custom allocators
- Chunk streaming limits

### How does chunk streaming work?

1. Track player position
2. Calculate required chunks (circular pattern)
3. Priority queue by distance
4. Background thread generates chunks
5. Upload to GPU when ready
6. Unload distant chunks

See [ChunkStreamer](include/voxel/ChunkStreamer.h) for implementation.

### Can I use a different renderer?

The architecture is designed to be modular. You could:
- Create a new renderer class
- Implement the Renderer interface
- Swap in your implementation

Though Vulkan is the primary target.

## Troubleshooting

### Black screen / nothing renders

**Common causes:**
- Shaders not found
- Vulkan validation errors
- GPU not Vulkan-capable
- Camera position issues

**Solutions:**
1. Check console for errors
2. Ensure shaders are in `build/shaders/`
3. Run with validation layers:
   ```bash
   export VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation
   ./FreshVoxelEngine
   ```
4. Check camera position (should be above ground)

### "Failed to create Vulkan instance"

**Solutions:**
1. Install Vulkan SDK
2. Update GPU drivers
3. Check GPU supports Vulkan 1.2:
   ```bash
   vulkaninfo | grep apiVersion
   ```

### Low FPS

**Solutions:**
1. Build in Release mode:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
2. Disable validation layers
3. Reduce chunk count
4. Check GPU usage in task manager

### Memory leaks

**Solutions:**
1. Build with debug symbols
2. Run with Valgrind:
   ```bash
   valgrind --leak-check=full ./FreshVoxelEngine
   ```
3. Check for missing smart pointer usage
4. Verify proper cleanup in destructors

### Build errors with C++17

**Solution:**
```bash
cmake .. -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
```

Ensure compiler supports C++17:
- GCC 7+
- Clang 5+
- MSVC 2017+

## Community

### Where can I get help?

- 📖 [Documentation](docs/)
- 💬 [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- 🐛 [GitHub Issues](https://github.com/shifty81/fresh/issues)
- 📧 Email: support@freshvoxelengine.org
- 💬 Discord: Coming soon!

### How do I report a bug?

1. Check existing issues
2. Use the bug report template
3. Include:
   - Description
   - Steps to reproduce
   - Environment (OS, GPU, etc.)
   - Error messages/logs
   - Screenshots if applicable

See [Bug Report Template](.github/ISSUE_TEMPLATE/bug_report.md)

### Can I request features?

Yes! Use the feature request template:
1. Describe the feature
2. Explain use cases
3. Suggest implementation ideas
4. Discuss benefits

See [Feature Request Template](.github/ISSUE_TEMPLATE/feature_request.md)

### Is there a Discord/forum?

Discord server is planned for future. Currently use:
- GitHub Discussions for questions
- GitHub Issues for bugs/features

### How do I stay updated?

- ⭐ Star the repository
- 👁️ Watch for releases
- 📰 Check [CHANGELOG.md](CHANGELOG.md)
- 📊 View [PROJECT_STATUS.md](PROJECT_STATUS.md)

## License & Legal

### Can I modify the engine?

Yes! The MIT License allows you to:
- Modify the source code
- Create derivative works
- Distribute modified versions

Just keep the original license notice.

### Do I need to credit the engine?

Not required, but appreciated! You can:
- Mention in credits
- Link to the repository
- Use the logo (when available)

### Can I sell games made with it?

Absolutely! No royalties or fees required.

### What about assets I create?

Assets you create are yours:
- Your ownership
- Your license choice
- Your distribution rights

## Future Plans

### When is version 1.0?

Target: Mid 2026

See [ROADMAP.md](ROADMAP.md) for detailed timeline.

### What features are coming next?

**v0.3.0 (Dec 2025):**
- Inventory system
- Resource gathering
- Crafting
- Day/night cycle

**v0.4.0 (Q1 2026):**
- Multiplayer networking
- Chat system
- Lobby system

See full roadmap in [ROADMAP.md](ROADMAP.md).

### Will you support VR?

VR support is being considered for post-1.0. Vulkan makes this feasible.

### Mobile support?

Not currently planned. The engine targets desktop platforms due to Vulkan requirements.

## Additional Resources

- 📖 [Getting Started](GETTING_STARTED.md)
- 🏗️ [Architecture](ARCHITECTURE.md)
- 📚 [Tutorials](docs/tutorials/)
- 🔧 [Developer Setup](DEVELOPER_SETUP.md)
- 🗺️ [Roadmap](ROADMAP.md)
- 📝 [Contributing](CONTRIBUTING.md)

---

**Didn't find your answer?**

- Ask in [GitHub Discussions](https://github.com/shifty81/fresh/discussions)
- Create an [Issue](https://github.com/shifty81/fresh/issues)
- Email: support@freshvoxelengine.org

We're here to help! 🚀
