#include <iostream>
#include <hpringbuffer.h>
#include <list>

int main(){

    HighPerformanceRingBuffer<int, 5> highperformance1;
    std::cout << highperformance1.capacity();

    highperformance1.insert(1);
    std::cout << highperformance1.size();

    highperformance1.insert({1, 2});
    std::cout << highperformance1.size();
    
    //buffer is [1, 1, 2...]
    std::cout << highperformance1.get();
    std::list<int> multi_get_result = highperformance1.get(2);
    for (auto index : multi_get_result){
        std::cout << index << " ";
    }
    
    // highperformance1.remove(1);
    // std::cout << highperformance1.size();
    // std::cout << highperformance1.get();

    // highperformance1.clear_buffer();
    
}