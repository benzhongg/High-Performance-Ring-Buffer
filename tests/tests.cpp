#include "hpringbuffer.h"
#include <gtest/gtest.h>

TEST(SizeTest, EmptyBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(SizeTest, NonEmptyBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.push(1);
    ASSERT_EQ(highperformance1.size(), 1);
}


TEST(SizeTest, FullBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.push(1);
    ASSERT_EQ(highperformance1.size(), 2);
}

TEST(PushTest, EmptyBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    ASSERT_EQ(highperformance1.push(1), true);
}

TEST(PushTest, NonEmptyBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.push(1);
    ASSERT_EQ(highperformance1.push(2), true);
}

TEST(PushTest, FullBuffer)
{
    HighPerformanceRingBuffer<int, 3> highperformance1;
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.push(1);
    ASSERT_EQ(highperformance1.push(2), false);
    ASSERT_EQ(highperformance1.size(), 2);
}

TEST(ClearBufferTest, BoolCheckSizeCheck)
{
    HighPerformanceRingBuffer<int, 5> highperformance1;
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.push(1);
    highperformance1.clearBuffer();
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(CapacityTest, ReturnValueMatchesDeclaration)
{
    HighPerformanceRingBuffer<int, 5> highperformance1;
    ASSERT_EQ(highperformance1.capacity(), 4);
}

TEST(PopTest, OnNonEmptyBuffer)
{
    HighPerformanceRingBuffer<int, 5> highperformance1;
    int pop_val1 { 0 };
    int pop_val2 { 0 };
    highperformance1.push(1);
    highperformance1.push(2);
    ASSERT_EQ(highperformance1.pop(pop_val1), true);
    ASSERT_EQ(highperformance1.size(), 1);
    ASSERT_EQ(highperformance1.pop(pop_val2), true);
    ASSERT_EQ(highperformance1.size(), 0);
}

TEST(PopTest, EmptyBuffer)
{
    HighPerformanceRingBuffer<int, 5> highperformance1;
    int pop_val { 0 };
    ASSERT_EQ(highperformance1.pop(pop_val), false);
}

// TEST(PushRangeTest, EmptyBuffer)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     highperformance1.pushRange({1, 2, 3});
//     ASSERT_EQ(highperformance1.size(), 3);
//     ASSERT_EQ(highperformance1.pop(), 1);
//     ASSERT_EQ(highperformance1.pop(), 2);
//     ASSERT_EQ(highperformance1.pop(), 3);
//     ASSERT_EQ(highperformance1.size(), 0);
// }

// TEST(PushRangeTest, NonEmptyBuffer)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     highperformance1.push(100);
//     highperformance1.pushRange({1, 2, 3});
//     ASSERT_EQ(highperformance1.size(), 4);
//     ASSERT_EQ(highperformance1.pop(), 100);
//     ASSERT_EQ(highperformance1.size(), 3);
// }

// TEST(PopRangeTest, EmptyBuffer)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     std::list<int> resulting_list = highperformance1.popRange(2);
//     ASSERT_TRUE(resulting_list.empty());
// }

// TEST(PopRangeTest, NonEmptyBufferValidGetAmount)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     highperformance1.pushRange({1, 2, 3});
//     std::list<int> resulting_list = highperformance1.popRange(3);
//     std::list<int> comparator_list = {1, 2, 3};
//     ASSERT_EQ(resulting_list, comparator_list);
//     ASSERT_EQ(highperformance1.size(), 0);
// }

// TEST(PopRangeTest, NonEmptyBufferInvalidGetAmount)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     highperformance1.pushRange({1, 2, 3});
//     std::list<int> resulting_list = highperformance1.popRange(4);
//     std::list<int> comparator_list = {};
//     ASSERT_EQ(resulting_list, comparator_list);
// }

TEST(ConcurrencyTest, PushElementCorrectOrder)
{
    HighPerformanceRingBuffer<int, 100> highperformance1;
    int iterations = 1'000;

    // Single Producer push values 0 to ITERATIONS
    std::thread producer 
    ([&highperformance1, &iterations] 
    {
        for (int x = 0; x < iterations; ++x)
        {
            while (!highperformance1.push(x))
            {
                // this reschedules the thread
                std::this_thread::yield();
            }
        }
    });

    // consumer should read every x value from 0 to ITERATIONS as they come in from producer and pass EXPECT_EQ
    std::thread consumer
    ([&highperformance1, &iterations]
        {
            for (int x = 0; x < iterations; ++x)
            {
                int pop_val { 0 };

                while (!highperformance1.pop(pop_val))
                {
                    std::this_thread::yield();
                }

                EXPECT_EQ(pop_val, x);
            }
        });
        
    producer.join();
    consumer.join();
}

