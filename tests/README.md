# Fresh Voxel Engine - Test Suite

This directory contains unit tests and integration tests for the Fresh Voxel Engine.

## Test Framework

We use [Google Test](https://github.com/google/googletest) as our testing framework.

## Building Tests

Tests are built automatically when you build the project with testing enabled:

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make
```

## Running Tests

After building, run the test suite:

```bash
# Run all tests
./FreshVoxelEngineTests

# Run specific test suite
./FreshVoxelEngineTests --gtest_filter=ChunkTests.*

# Run with verbose output
./FreshVoxelEngineTests --gtest_verbose=1

# List all tests
./FreshVoxelEngineTests --gtest_list_tests
```

## Test Organization

Tests are organized by system/module:

```
tests/
├── core/               # Core engine tests
│   ├── EngineTests.cpp
│   ├── WindowTests.cpp
│   └── MemoryManagerTests.cpp
├── voxel/             # Voxel system tests
│   ├── ChunkTests.cpp
│   ├── VoxelWorldTests.cpp
│   └── MeshGeneratorTests.cpp
├── generation/        # Procedural generation tests
│   ├── NoiseTests.cpp
│   └── TerrainGeneratorTests.cpp
├── physics/           # Physics system tests
│   └── CollisionTests.cpp
├── gameplay/          # Gameplay tests
│   ├── PlayerTests.cpp
│   └── CameraTests.cpp
└── assets/            # Asset system tests
    └── ModularAssetSystemTests.cpp
```

## Writing Tests

### Test File Template

```cpp
#include <gtest/gtest.h>
#include "system/YourClass.h"

// Test fixture for setup/teardown
class YourClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
    }

    void TearDown() override {
        // Cleanup code after each test
    }

    // Shared test data
    YourClass* testObject;
};

// Test case
TEST_F(YourClassTest, MethodName_Scenario_ExpectedBehavior) {
    // Arrange
    int input = 5;
    
    // Act
    int result = testObject->method(input);
    
    // Assert
    EXPECT_EQ(result, 10);
}

// Simple test (no fixture)
TEST(YourClassTest, SimpleTest) {
    EXPECT_TRUE(true);
}
```

### Common Assertions

```cpp
// Boolean
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);

// Equality
EXPECT_EQ(expected, actual);
EXPECT_NE(val1, val2);

// Comparison
EXPECT_LT(val1, val2);  // Less than
EXPECT_LE(val1, val2);  // Less or equal
EXPECT_GT(val1, val2);  // Greater than
EXPECT_GE(val1, val2);  // Greater or equal

// Floating point
EXPECT_FLOAT_EQ(expected, actual);
EXPECT_DOUBLE_EQ(expected, actual);
EXPECT_NEAR(val1, val2, abs_error);

// Strings
EXPECT_STREQ(str1, str2);
EXPECT_STRNE(str1, str2);

// Exceptions
EXPECT_THROW(statement, exception_type);
EXPECT_NO_THROW(statement);

// Death tests (for crash testing)
EXPECT_DEATH(statement, regex);
```

## Test Coverage

Current test coverage:

| Module | Coverage | Status |
|--------|----------|--------|
| Core Engine | 0% | 🚧 Planned |
| Voxel System | 0% | 🚧 Planned |
| Generation | 0% | 🚧 Planned |
| Physics | 0% | 🚧 Planned |
| Gameplay | 0% | 🚧 Planned |
| Assets | 0% | 🚧 Planned |

## Continuous Integration

Tests run automatically on:
- Every pull request
- Every commit to main branch
- Nightly builds

See `.github/workflows/ci.yml` for CI configuration.

## Best Practices

### DO:
- ✅ Write tests before fixing bugs
- ✅ Test edge cases and error conditions
- ✅ Keep tests focused and simple
- ✅ Use descriptive test names
- ✅ Test public interfaces, not implementation
- ✅ Make tests independent and repeatable

### DON'T:
- ❌ Test internal implementation details
- ❌ Write tests that depend on other tests
- ❌ Use hardcoded paths or absolute paths
- ❌ Leave commented-out test code
- ❌ Test third-party libraries
- ❌ Write tests that are slow without marking them

## Performance Tests

Mark slow tests with:

```cpp
TEST(SlowTest, LongRunningOperation) {
    // This test might take a while
}
```

Run only fast tests:

```bash
./FreshVoxelEngineTests --gtest_filter=-SlowTest.*
```

## Debugging Tests

### Run under debugger:

```bash
gdb --args ./FreshVoxelEngineTests --gtest_filter=FailingTest
```

### Get detailed failure info:

```bash
./FreshVoxelEngineTests --gtest_break_on_failure
```

## Contributing

When adding new features:
1. Write tests first (TDD)
2. Ensure all tests pass
3. Aim for >80% code coverage
4. Add integration tests for complex features

## Resources

- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Google Test Advanced Guide](https://google.github.io/googletest/advanced.html)
- [Best Practices](https://google.github.io/googletest/faq.html)
