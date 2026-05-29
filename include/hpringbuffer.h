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

    bool isFull()
    {
        return size() + 1 == CAPACITY;    
    }

    bool isEmpty()
    {
        return size() == 0;
    }

public:

    HighPerformanceRingBuffer() = default;
    
    ~HighPerformanceRingBuffer() = default;

    bool push(T v)
    //are either atomics affected here?
    //yes
    { 
        if (!isFull())
        {
            // acquire pertains to loads any further load can't be before this one or any behind
            // release pertains to storing past or any following 
            
            // consume a specific version of acquire
            // relaxed -> all memory reorderings are ok

            // in this case we'll be choosing acq rel

            // memory_order_acq_rel is for RMW events/operations
            int tempHead = m_head.load(std::memory_order_acquire);

            m_buffer[tempHead] = v;

            tempHead = (tempHead + 1) % CAPACITY;
            
            m_head.store(tempHead, std::memory_order_release);
            return true;
        } 
        else 
        {
            return false;
        }
    }
    
    bool pushRange(std::initializer_list<T> list)
    {
        if (list.size() > (CAPACITY - size()))
        {
            return false;
        }

        for (auto item : list) 
        {
            push(item);
        }

        return true;
    }

    const T pop()
    {
        if (isEmpty())
        {
            return default_value();
        }
        
        int tempTail = m_tail.load(std::memory_order_acquire);
        
        T res = m_buffer[tempTail];
        tempTail = (tempTail + 1) % CAPACITY;
        
        m_tail.store(tempTail, std::memory_order_release);

        return res;
    }
    
    std::list<T> popRange(int element_read_count)
    {
        if (element_read_count > size() || element_read_count > CAPACITY)
        {
            return {};
        } 
        else 
        {
            std::list<T> res;

            auto currentSize = size();
            
            for (auto x = 0; x < currentSize; x++)
            {
                res.push_back(pop());
            }
            
            return res;
        }
    }

    //returns element count
    size_t size() const
    {

        // there is a potential error here -> even though they are safeguarded individually, what
        // happens when tail is head is read, head gets affected via a push(), and tail is loaded?
            // we'd be calculating a size that is no longer valid
        int temp_head = m_head.load(std::memory_order_acquire);
        int temp_tail = m_tail.load(std::memory_order_acquire);

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