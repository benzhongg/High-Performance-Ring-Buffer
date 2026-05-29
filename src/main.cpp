#include <iostream>
#include <hpringbuffer.h>

void say_hello_1(){
    std::cout << "hello from thread 1";
}
void say_hello_2(){
    std::cout << "hello from thread 2";
}
void say_hello_3(){
    std::cout << "hello from thread 3";
}

int main(){
    std::thread t1(say_hello_1);
    std::thread t2(say_hello_2);
    std::thread t3(say_hello_3);
    t1.join();
    t2.join();
    t3.join();
    // join waits for the thread to finish its execution
    // detach permits the thread to execute independently from the thread handle
    // swap swaps two thread objects
}