/**
 * @file MemoryManagerTests.cpp
 * @brief Unit tests for the MemoryManager system
 */

#include <gtest/gtest.h>
#include "core/MemoryManager.h"

/**
 * Test fixture for MemoryManager tests
 */
class MemoryManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        MemoryManager::getInstance().reset();
    }

    void TearDown() override {
        MemoryManager::getInstance().reset();
    }
};

/**
 * Test memory tracking
 */
TEST_F(MemoryManagerTest, TrackAllocation_AddsToTotal_CorrectSize) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    size_t allocSize = 1024;
    
    // Act
    memMgr.trackAllocation(allocSize, "Test");
    
    // Assert
    EXPECT_EQ(memMgr.getTotalAllocated(), allocSize);
}

/**
 * Test memory deallocation tracking
 */
TEST_F(MemoryManagerTest, TrackDeallocation_SubtractsFromTotal_CorrectSize) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    size_t allocSize = 2048;
    
    memMgr.trackAllocation(allocSize, "Test");
    ASSERT_EQ(memMgr.getTotalAllocated(), allocSize);
    
    // Act
    memMgr.trackDeallocation(allocSize, "Test");
    
    // Assert
    EXPECT_EQ(memMgr.getTotalAllocated(), 0);
}

/**
 * Test multiple allocations
 */
TEST_F(MemoryManagerTest, MultipleAllocations_AccumulatesCorrectly_TotalSize) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    
    // Act
    memMgr.trackAllocation(1000, "Test1");
    memMgr.trackAllocation(2000, "Test2");
    memMgr.trackAllocation(3000, "Test3");
    
    // Assert
    EXPECT_EQ(memMgr.getTotalAllocated(), 6000);
}

/**
 * Test tagged allocations
 */
TEST_F(MemoryManagerTest, TaggedAllocation_TracksPerTag_CorrectSizes) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    
    // Act
    memMgr.trackAllocation(1024, "Chunk");
    memMgr.trackAllocation(2048, "Chunk");
    memMgr.trackAllocation(512, "Texture");
    
    // Assert
    EXPECT_EQ(memMgr.getAllocatedByTag("Chunk"), 3072);
    EXPECT_EQ(memMgr.getAllocatedByTag("Texture"), 512);
}

/**
 * Test peak memory tracking
 */
TEST_F(MemoryManagerTest, PeakMemory_TracksMaximum_CorrectValue) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    
    // Act
    memMgr.trackAllocation(1000, "Test");
    memMgr.trackAllocation(2000, "Test");  // Peak = 3000
    memMgr.trackDeallocation(1500, "Test"); // Now 1500, but peak is still 3000
    
    // Assert
    EXPECT_EQ(memMgr.getPeakAllocated(), 3000);
}

/**
 * Test memory statistics
 */
TEST_F(MemoryManagerTest, GetStatistics_ReturnsCorrectInfo_AllFields) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    
    memMgr.trackAllocation(1024, "Test");
    memMgr.trackAllocation(2048, "Test");
    
    // Act
    auto stats = memMgr.getStatistics();
    
    // Assert
    EXPECT_EQ(stats.totalAllocated, 3072);
    EXPECT_EQ(stats.peakAllocated, 3072);
    EXPECT_GE(stats.allocationCount, 2);
}

/**
 * Test reset functionality
 */
TEST_F(MemoryManagerTest, Reset_ClearsAllTracking_ZeroAllocations) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    memMgr.trackAllocation(5000, "Test");
    ASSERT_GT(memMgr.getTotalAllocated(), 0);
    
    // Act
    memMgr.reset();
    
    // Assert
    EXPECT_EQ(memMgr.getTotalAllocated(), 0);
}

/**
 * Test memory leak detection
 */
TEST_F(MemoryManagerTest, LeakDetection_WithLeaks_ReportsLeaks) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    memMgr.trackAllocation(1000, "Leaked");
    
    // Act
    bool hasLeaks = memMgr.hasLeaks();
    
    // Assert
    EXPECT_TRUE(hasLeaks);
}

TEST_F(MemoryManagerTest, LeakDetection_NoLeaks_ReportsClean) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    memMgr.trackAllocation(1000, "NotLeaked");
    memMgr.trackDeallocation(1000, "NotLeaked");
    
    // Act
    bool hasLeaks = memMgr.hasLeaks();
    
    // Assert
    EXPECT_FALSE(hasLeaks);
}

/**
 * Test memory pool
 */
TEST(MemoryPoolTest, Allocate_CreatesObject_ValidPointer) {
    // Arrange
    MemoryPool<int> pool(10);
    
    // Act
    int* ptr = pool.allocate();
    
    // Assert
    EXPECT_NE(ptr, nullptr);
    
    // Cleanup
    pool.deallocate(ptr);
}

TEST(MemoryPoolTest, AllocateDeallocate_ReuseMemory_SamePointer) {
    // Arrange
    MemoryPool<int> pool(10);
    
    // Act
    int* ptr1 = pool.allocate();
    pool.deallocate(ptr1);
    int* ptr2 = pool.allocate();
    
    // Assert - should reuse the same memory
    EXPECT_EQ(ptr1, ptr2);
    
    // Cleanup
    pool.deallocate(ptr2);
}

TEST(MemoryPoolTest, MultipleAllocations_WithinCapacity_AllSucceed) {
    // Arrange
    MemoryPool<int> pool(5);
    std::vector<int*> pointers;
    
    // Act
    for (int i = 0; i < 5; i++) {
        pointers.push_back(pool.allocate());
    }
    
    // Assert
    for (auto* ptr : pointers) {
        EXPECT_NE(ptr, nullptr);
    }
    
    // Cleanup
    for (auto* ptr : pointers) {
        pool.deallocate(ptr);
    }
}

/**
 * Test stack allocator
 */
TEST(StackAllocatorTest, Allocate_ReturnsMemory_ValidPointer) {
    // Arrange
    StackAllocator allocator(1024);
    
    // Act
    void* ptr = allocator.allocate(64);
    
    // Assert
    EXPECT_NE(ptr, nullptr);
}

TEST(StackAllocatorTest, SequentialAllocations_PacksTogether_NoGaps) {
    // Arrange
    StackAllocator allocator(1024);
    
    // Act
    void* ptr1 = allocator.allocate(64);
    void* ptr2 = allocator.allocate(64);
    
    // Assert - pointers should be 64 bytes apart
    size_t offset = reinterpret_cast<char*>(ptr2) - reinterpret_cast<char*>(ptr1);
    EXPECT_EQ(offset, 64);
}

TEST(StackAllocatorTest, Reset_ClearsAllocations_ReuseMemory) {
    // Arrange
    StackAllocator allocator(1024);
    void* ptr1 = allocator.allocate(64);
    
    // Act
    allocator.reset();
    void* ptr2 = allocator.allocate(64);
    
    // Assert - after reset, should get same pointer
    EXPECT_EQ(ptr1, ptr2);
}

/**
 * Thread safety test (if applicable)
 */
TEST(MemoryManagerThreadTest, ConcurrentAccess_MultipleThreads_NoDataRace) {
    // Arrange
    auto& memMgr = MemoryManager::getInstance();
    memMgr.reset();
    const int numThreads = 4;
    const int allocationsPerThread = 100;
    
    // Act
    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; t++) {
        threads.emplace_back([&memMgr, allocationsPerThread]() {
            for (int i = 0; i < allocationsPerThread; i++) {
                memMgr.trackAllocation(1024, "ThreadTest");
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Assert
    size_t expected = numThreads * allocationsPerThread * 1024;
    EXPECT_EQ(memMgr.getTotalAllocated(), expected);
}
