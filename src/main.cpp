#include <iostream>
#include <hpringbuffer.h>
#include <numeric>

int main()
{
    // HighPerformanceRingBuffer<int, 100> hp1;
    // const int TEST_COUNT = 1;
    // long long consumer_sum = 0;

    // std::thread producer
    // ([&hp1, &TEST_COUNT]()
    // {
    //     for (auto i = 1; i < TEST_COUNT; ++i)
    //     {
    //         while (!hp1.push(i))
    //         {
    //             std::this_thread::yield();                
    //         }
    //     }
    // });



    // std::thread consumer
    // ([&hp1, &consumer_sum, &TEST_COUNT]()
    // {
    //     int count = 0;
    //     while (count < TEST_COUNT)
    //     {
    //         if (!hp1.isEmpty())
    //         {
    //             consumer_sum += hp1.pop();
    //             count++;
    //         }
    //         else
    //         {
    //             std::this_thread::yield();
    //         }
    //     }
    // });
    
    // producer.join();
    // consumer.join();

    // long long expected_sum = (static_cast<long long>(TEST_COUNT) * (TEST_COUNT + 1)) / 2;
    // std::cout << "Expected Sum: " << expected_sum << std::endl;
    // std::cout << "Actual Sum:   " << consumer_sum << std::endl;

    // if (consumer_sum == expected_sum) 
    // {
    //     std::cout << "SUCCESS: No data loss or corruption." << std::endl;
    // } 
    // else 
    // {
    //     std::cout << "FAILURE: Data race detected." << std::endl;
    // }
}