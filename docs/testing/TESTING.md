# Testing Guide for Fresh Voxel Engine

This guide explains how to set up, write, and run tests for the Fresh Voxel Engine.

## 🎉 Testing Status

**Testing Infrastructure: ✅ OPERATIONAL**

- **Total Tests:** 75 passing
- **Test Framework:** Google Test 1.12.1
- **Coverage:** Core systems (Chunks, Noise, Terrain, Events)
- **Build Integration:** Enabled by default
- **CI/CD:** Ready for integration

## Quick Start

### Build with Tests (Enabled by Default)

```bash
cd /path/to/fresh
mkdir build && cd build
cmake ..  # Tests are now enabled by default
make -j$(nproc)
```

To disable tests:
```bash
cmake -DBUILD_TESTS=OFF ..
```

### Run Tests

```bash
# Run all tests directly
./FreshVoxelEngineTests

# Or use CTest for detailed output
ctest --output-on-failure

# Run with verbose output
ctest --verbose

# Run specific test suite
./FreshVoxelEngineTests --gtest_filter=ChunkTest.*

# Run specific test
./FreshVoxelEngineTests --gtest_filter=ChunkTest.SetAndGetVoxel_ValidCoordinates_ReturnsCorrectType
```

## Setting Up Testing Environment

### Prerequisites

- CMake 3.20 or higher
- C++20 compatible compiler
- Google Test (automatically fetched if not found)
- OpenGL development libraries (Linux/macOS)

### Linux/macOS Installation

Install required dependencies first:

```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev libglew-dev libglm-dev libopenal-dev

# macOS
brew install glfw glew glm openal-soft

# Then build with tests
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows Installation

Google Test is automatically downloaded and built when you enable BUILD_TESTS:

```batch
cmake -B build
cmake --build build --config Debug
```

#### Manual Installation (Optional)

If you prefer to install Google Test manually:
- Use vcpkg: `vcpkg install gtest:x64-windows`
- Or download from https://github.com/google/googletest and build manually

## Project Structure

```
fresh/
├── tests/                  # All test files
│   ├── README.md          # This file
│   ├── core/              # Core engine tests
│   ├── voxel/             # Voxel system tests
│   ├── generation/        # Terrain generation tests
│   ├── physics/           # Physics tests
│   ├── gameplay/          # Gameplay tests
│   └── assets/            # Asset system tests
├── CMakeLists.txt         # Build configuration (test support added)
└── ...
```

## Writing Tests

### Basic Test Template

```cpp
#include <gtest/gtest.h>
#include "your/header.h"

TEST(TestSuiteName, TestName) {
    // Arrange
    int expected = 5;
    
    // Act
    int actual = yourFunction();
    
    // Assert
    EXPECT_EQ(expected, actual);
}
```

### Test Fixture Template

```cpp
#include <gtest/gtest.h>
#include "your/header.h"

class YourClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
        object = std::make_unique<YourClass>();
    }

    void TearDown() override {
        // Cleanup code after each test
        object.reset();
    }

    std::unique_ptr<YourClass> object;
};

TEST_F(YourClassTest, MethodTest) {
    // Use object here
    EXPECT_TRUE(object->isValid());
}
```

## Test Categories

### 1. Unit Tests
Test individual functions and classes in isolation.

**Example:**
```cpp
TEST(MathTest, AddFunction_TwoNumbers_ReturnsSum) {
    EXPECT_EQ(add(2, 3), 5);
}
```

### 2. Integration Tests
Test multiple components working together.

**Example:**
```cpp
TEST(WorldTest, ChunkAndWorld_Integration_Works) {
    VoxelWorld world;
    auto chunk = std::make_unique<Chunk>(0, 0);
    world.addChunk(std::move(chunk));
    EXPECT_NE(world.getChunk(0, 0), nullptr);
}
```

### 3. Performance Tests
Test that operations complete within time limits.

**Example:**
```cpp
TEST(PerformanceTest, ChunkGeneration_CompletsQuickly) {
    auto start = std::chrono::high_resolution_clock::now();
    
    generateChunk();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LT(duration.count(), 100);  // Should complete in < 100ms
}
```

## Common Assertions

### Equality
```cpp
EXPECT_EQ(expected, actual);      // Equal
EXPECT_NE(val1, val2);            // Not equal
EXPECT_LT(val1, val2);            // Less than
EXPECT_LE(val1, val2);            // Less or equal
EXPECT_GT(val1, val2);            // Greater than
EXPECT_GE(val1, val2);            // Greater or equal
```

### Boolean
```cpp
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);
```

### Floating Point
```cpp
EXPECT_FLOAT_EQ(expected, actual);
EXPECT_DOUBLE_EQ(expected, actual);
EXPECT_NEAR(val1, val2, abs_error);
```

### Strings
```cpp
EXPECT_STREQ(str1, str2);
EXPECT_STRNE(str1, str2);
```

### Exceptions
```cpp
EXPECT_THROW(statement, exception_type);
EXPECT_ANY_THROW(statement);
EXPECT_NO_THROW(statement);
```

## Running Tests

### All Tests
```bash
./FreshVoxelEngineTests
```

### Specific Test Suite
```bash
./FreshVoxelEngineTests --gtest_filter=ChunkTest.*
```

### Specific Test
```bash
./FreshVoxelEngineTests --gtest_filter=ChunkTest.Constructor_CreatesChunk_WithCorrectCoordinates
```

### With Verbose Output
```bash
./FreshVoxelEngineTests --gtest_verbose=1
```

### List All Tests
```bash
./FreshVoxelEngineTests --gtest_list_tests
```

### Repeat Tests
```bash
./FreshVoxelEngineTests --gtest_repeat=10
```

### Run Until Failure
```bash
./FreshVoxelEngineTests --gtest_repeat=-1 --gtest_break_on_failure
```

## Continuous Integration

Tests run automatically on:
- Pull requests
- Commits to main branch
- Nightly builds

See `.github/workflows/ci.yml` for CI configuration.

## Test Coverage

### Generate Coverage Report (GCC/Clang)

```bash
# Build with coverage flags
cmake -DBUILD_TESTS=ON -DCMAKE_CXX_FLAGS="--coverage" ..
make

# Run tests
./FreshVoxelEngineTests

# Generate report
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_report

# View report
open coverage_report/index.html
```

## Debugging Tests

### Debug Single Test with GDB

```bash
gdb --args ./FreshVoxelEngineTests --gtest_filter=ChunkTest.FailingTest
```

### Break on Failure

```bash
./FreshVoxelEngineTests --gtest_break_on_failure
```

### Verbose Failure Info

```bash
./FreshVoxelEngineTests --gtest_verbose=1
```

## Best Practices

### DO:
- ✅ Write tests for new features
- ✅ Write tests when fixing bugs
- ✅ Keep tests independent
- ✅ Use descriptive test names (MethodName_Scenario_ExpectedBehavior)
- ✅ Follow Arrange-Act-Assert pattern
- ✅ Test edge cases and error conditions
- ✅ Clean up resources in TearDown()

### DON'T:
- ❌ Test private methods directly
- ❌ Make tests depend on execution order
- ❌ Use hardcoded file paths
- ❌ Leave debug code or comments
- ❌ Test external dependencies
- ❌ Write slow tests without marking them

## Test Naming Convention

```
MethodName_Scenario_ExpectedBehavior

Examples:
- SetVoxel_ValidCoordinates_ReturnsTrue
- GetChunk_InvalidCoords_ReturnsNull
- GenerateTerrain_WithSeed_IsDeterministic
```

## Example Tests

See existing test files:
- `tests/voxel/ChunkTests.cpp` - Chunk system tests
- `tests/generation/NoiseTests.cpp` - Noise generation tests
- `tests/core/MemoryManagerTests.cpp` - Memory management tests

## Adding New Tests

1. Create test file in appropriate directory
2. Include necessary headers
3. Write test fixtures if needed
4. Write test cases
5. Add test file to CMakeLists.txt (in TEST_SOURCES)
6. Build and run tests
7. Fix any failures
8. Commit tests with code changes

## Troubleshooting

### Tests Won't Build

**Problem:** CMake can't find Google Test

**Solution:**
```bash
cmake -DBUILD_TESTS=ON ..
# Google Test will be fetched automatically
```

### Tests Crash

**Problem:** Segmentation fault in tests

**Solution:**
- Run under debugger: `gdb --args ./FreshVoxelEngineTests`
- Check for null pointers
- Verify setup/teardown is correct
- Check memory management

### Tests Fail

**Problem:** Test assertions fail

**Solution:**
- Check test logic
- Verify expected vs actual values
- Add debug output: `std::cout << "Debug: " << value << std::endl;`
- Run specific test in isolation

### Slow Tests

**Problem:** Tests take too long

**Solution:**
- Profile tests
- Mock expensive operations
- Reduce iteration counts
- Mark as slow test if necessary

## Resources

- [Google Test Documentation](https://google.github.io/googletest/)
- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Google Test FAQ](https://google.github.io/googletest/faq.html)
- [Test Driven Development](https://en.wikipedia.org/wiki/Test-driven_development)

## Contributing

When contributing tests:
1. Follow existing test structure
2. Use consistent naming
3. Add comments for complex tests
4. Ensure tests pass before submitting PR
5. Aim for high code coverage
6. Document any special test requirements

## Support

For help with testing:
- Check existing tests for examples
- Read Google Test documentation
- Ask in project discussions
- Open an issue with `testing` label
