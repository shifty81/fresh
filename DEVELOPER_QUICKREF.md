# Developer Quick Reference

Quick commands and workflows for Fresh Voxel Engine development.

## 🔧 Common Commands

### Build & Test
```bash
# Build Release
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

# Build with Tests
cmake -B build -DBUILD_TESTS=ON && cmake --build build

# Run Tests
./build/FreshVoxelEngineTests
```

### Code Formatting
```bash
# Format single file
clang-format -i src/core/Engine.cpp

# Format all C++ files
find include/ src/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Documentation
```bash
# Generate API docs
doxygen Doxyfile

# View generated docs
open docs/api/html/index.html  # macOS
xdg-open docs/api/html/index.html  # Linux
```

## 📝 Naming Conventions

| Type | Style | Example |
|------|-------|---------|
| Class/Struct | PascalCase | `VoxelWorld` |
| Function/Method | camelCase | `generateChunk()` |
| Variable | camelCase | `chunkSize` |
| Constant | UPPER_SNAKE_CASE | `MAX_CHUNK_SIZE` |
| Private Member | m_ prefix | `m_vertexBuffer` |

## 🏷️ Release Process

```bash
# 1. Update versions (CMakeLists.txt, vcpkg.json, Doxyfile)
# 2. Update CHANGELOG.md
# 3. Commit and push
git add . && git commit -m "Bump version to X.Y.Z" && git push

# 4. Create and push tag
git tag -a vX.Y.Z -m "Release X.Y.Z" && git push origin vX.Y.Z
```

## ✅ Pre-Commit Checklist

- [ ] Code formatted with clang-format
- [ ] Build succeeds without warnings
- [ ] Tests pass
- [ ] Doxygen comments added for public APIs
- [ ] CHANGELOG.md updated (if needed)

## 📚 Documentation

- [Full Development Tools Guide](docs/DEVELOPMENT_TOOLS.md)
- [Contributing Guidelines](CONTRIBUTING.md)
- [API Reference](QUICK_REFERENCE.md)

---

For detailed information, see [docs/DEVELOPMENT_TOOLS.md](docs/DEVELOPMENT_TOOLS.md)
