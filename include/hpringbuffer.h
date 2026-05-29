#pragma once
#include <array>
#include <list>
#include <iostream>
#include <stdexcept>
#include <atomic>
#include <thread>


template<typename T, size_t CAPACITY>
class HighPerformanceRingBuffer
{
private:
    std::array<T, CAPACITY> m_buffer { default_value() };
    

    // thing to note
        // False Sharing -> if head and tail are atomics and placed next to each other in mem
        // They may end up on the same cache line, meaning either one of them might invalidate
            // the other for trying to read/update
            // this is solved through "padding" like a char padding[64];
            
    //head is next write
    std::atomic<int> m_head { 0 };
    
    //tail is next read
    std::atomic<int> m_tail { 0 };

protected:
    T default_value()
    {
        return T{};
    }

    
    
    public:
    
    bool isFull()
    {
        return size() + 1 == CAPACITY;    
    }
    
    bool isEmpty()
    {
        return size() == 0;
    }
    HighPerformanceRingBuffer() = default;
    
    ~HighPerformanceRingBuffer() = default;

    bool push(T v)
    //are either atomics affected here?
    //yes
    { 
        
        // if this is false then does the compiler/cpu decide to reorder
        if (!isFull())
        {
            // acquire prevents any loads any further load can't be before this one or any behind
            // release pertains to storing past or any following 
            
            // consume a specific version of acquire
            // relaxed -> all memory reorderings are ok
            
            // in this case we'll be choosing acq rel
            
            // memory_order_acq_rel is for RMW events/operations
            
            // seq_cst is the strongest memory ordering in c++
            // program order among every individual thread
            // single total order amongst all threads
            
            // atomic operations are all about synchronizing memory between threads
            // avoid read/write and overwriting each other
            
            // acquire memory published by other threads making it available to us
            // release updates and publishes changes made to all other threads
            // prevents compiler from moving anything below
            // 
            
            // when you use acquire and release there is no such thing as global ordering
            // no total order of events
            // but from the perspective of each thread, the ordering they follow is consistent to itself
            // sequentially consistent
            
            // relaxed -> guarantees one variable is visible and guaranteed at every single time
            // no ordering
            // no other memory is synchronized
            // no ordering with other relaxed operations
            // used if your counting the ocurrences of some event in your program
            
            // x86 coherent cache model look into
            
            // volatile doesn't do synchronization
            // related to IPC
            
            // I care about having the most up to date m_head so an acquire is necessary
            
            auto temp_head = m_head.load(std::memory_order_acquire);
            auto temp_tail = m_tail.load(std::memory_order_acquire);
            
            m_buffer[temp_head] = v;
            // computation
            temp_head = (temp_head + 1) % CAPACITY;
            
            // I care about releasing the most up to date m_head so release is necessary
            m_head.store(temp_head, std::memory_order_release);
            
            return true;
        } 
        else 
        {
            return false;
        }
    }
    
    // bool pushRange(std::initializer_list<T> list)
    // {
    //     if (list.size() > (CAPACITY - size()))
    //     {
    //         return false;
    //     }

    //     for (auto item : list) 
    //     {
    //         push(item);
    //     }

    //     return true;
    // }

    bool pop(T& out_value)
    {
        auto temp_head = m_head.load(std::memory_order_acquire);
        auto temp_tail = m_tail.load(std::memory_order_acquire);
        
        if (isEmpty())
        {
            return false;
        }
        
        // see push() for same reasoning
        // in an SPSC scenario consider using relaxed for less overhead than acquire
        // acquire works but there's no need to sync with external states since SPSC
        
        out_value = m_buffer[temp_tail];
        temp_tail = (temp_tail + 1) % CAPACITY;
        
        m_tail.store(temp_tail, std::memory_order_release);

        return true;
    }
    
    // std::list<T> popRange(int element_read_count)
    // {
    //     if (element_read_count > size() || element_read_count > CAPACITY)
    //     {
    //         return {};
    //     } 
    //     else 
    //     {
    //         std::list<T> res;

    //         auto currentSize = size();
            
    //         for (auto x = 0; x < currentSize; x++)
    //         {
    //             res.push_back(pop());
    //         }
            
    //         return res;
    //     }
    // }

    //returns element count
    size_t size() const
    {

        // there is a potential error here -> even though they are safeguarded individually, what
        // happens when head and tail is read, head gets affected via a push(), and tail is loaded?
            // we'd be calculating a size that is no longer valid

        // if they're both able to call size()
        // acquire is needed to check the true m_head value

        auto temp_head = m_head.load(std::memory_order_acquire);
        auto temp_tail = m_tail.load(std::memory_order_acquire);

        if (temp_tail < temp_head){
            return temp_head - temp_tail;
        }
        
        if (temp_head < temp_tail){
            return CAPACITY - temp_tail - temp_head;
        }
        
        //H == T when empty, our FULL case won't occur -> see isFull()
        return 0;
    }
    
    // these are both stores -> debating between .store or .exchange
    // .store is the straightforward release after setting to 0 (faster)
    // .exchange is the acq (sync) with all threads then setting to 0 (slower, more hardware, but performant if
        // knowledge of the past is required)
    void clearBuffer()
    {
        m_tail.store(0, std::memory_order_release);
        m_head.store(0, std::memory_order_release);
    }
    
    size_t capacity()
    {
        return CAPACITY - 1;
    }
};