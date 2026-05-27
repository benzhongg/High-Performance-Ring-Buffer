#include "hpringbuffer.h"
#include <gtest/gtest.h>

TEST(SizeTest, EmptyBuffer){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(SizeTest, NonEmptyBuffer){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.size(), 1);
}


TEST(SizeTest, FullBuffer){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.size(), 2);
}

TEST(InsertTest, EmptyBufferInsert){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    ASSERT_EQ(highperformance1.insert(1), true);
}

TEST(InsertTest, NonEmptyBufferInsert){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.insert(2), true);
}

TEST(InsertTest, FullBufferInsert){
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.insert(2), false);
    ASSERT_EQ(highperformance1.size(), 2);
}

TEST(RemoveTest, ValidIndexValidCapacity){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.remove(0), true);
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(RemoveTest, InvalidIndexValidCapacity){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.remove(20), false);
    
}

TEST(RemoveTest, InvalidIndexInvalidCapacity){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.remove(3), false);
}

TEST(ClearBufferTest, BoolCheckSizeCheck){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.insert(1);
    highperformance1.clearBuffer();
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(CapacityTest, ReturnValueMatchesDeclaration){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    ASSERT_EQ(highperformance1.capacity(), 5);
}

TEST(GetTest, OnNonEmptyBuffer){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(1);
    highperformance1.insert(2);
    ASSERT_EQ(highperformance1.get(), 1);
    ASSERT_EQ(highperformance1.size(), 1);
    ASSERT_EQ(highperformance1.get(), 2);
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(GetTest, EmptyBuffer){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    ASSERT_EQ(highperformance1.get(), 0);
}

TEST(MultiInsertTest, EmptyBuffer){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.multiInsert({1, 2, 3});
    ASSERT_EQ(highperformance1.size(), 3);
    ASSERT_EQ(highperformance1.get(), 1);
    ASSERT_EQ(highperformance1.get(), 2);
    ASSERT_EQ(highperformance1.get(), 3);
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(MultiInsertTest, NonEmptyBuffer){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.insert(100);
    highperformance1.multiInsert({1, 2, 3});
    ASSERT_EQ(highperformance1.size(), 4);
    ASSERT_EQ(highperformance1.get(), 100);
    ASSERT_EQ(highperformance1.size(), 3);
}

TEST(MultiGetTest, EmptyBuffer){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    std::list<int> resulting_list = highperformance1.multiGet(2);
    ASSERT_TRUE(resulting_list.empty());
}

TEST(MultiGetTest, NonEmptyBufferValidGetAmount){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.multiInsert({1, 2, 3});
    std::list<int> resulting_list = highperformance1.multiGet(3);
    std::list<int> comparator_list = {1, 2, 3};
    ASSERT_EQ(resulting_list, comparator_list);
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(MultiGetTest, NonEmptyBufferInvalidGetAmount){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.multiInsert({1, 2, 3});
    std::list<int> resulting_list = highperformance1.multiGet(4);
    std::list<int> comparator_list = {};
    ASSERT_EQ(resulting_list, comparator_list);
}



