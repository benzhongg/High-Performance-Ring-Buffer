#include "hpringbuffer.h"
#include <gtest/gtest.h>

TEST(SizeInsertTest, InsertItemCountMatchesSizeMethod){
    HighPerformanceRingBuffer<int, 5> highperformance1;

    highperformance1.insert(1);
    ASSERT_EQ(highperformance1.size(), 1);
}

TEST(SizeRemoveTest, RemovingItemsMatchesSizeMethod){
    HighPerformanceRingBuffer<int, 5> highperformance1;
    
    highperformance1.insert(1);
    highperformance1.insert(2);
    highperformance1.remove(1);
    ASSERT_EQ(highperformance1.size(), 1);
}

// TEST(OverSizeInsertTest, descriptor)
// {
//     HighPerformanceRingBuffer<int, 5> highperformance1;
//     for(int x = 0; x < 100; x++){
//         highperformance1.insert(x);
//     }
//     ASSERT_EQ(highperformance1.size(), 5);
// }