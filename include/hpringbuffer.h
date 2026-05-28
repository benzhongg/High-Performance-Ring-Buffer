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
    
    //head is next write
    int m_head { 0 };
    
    //tail is next read
    int m_tail { 0 };

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
    { 
        if (!isFull())
        {
            m_buffer[m_head] = v;

            m_head = (m_head + 1) % CAPACITY;
            
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
        
        T res = m_buffer[m_tail];
        m_tail = (m_tail + 1) % CAPACITY;
        
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
        if (m_tail < m_head){
            return m_head - m_tail;
        }
        
        if (m_head < m_tail){
            return CAPACITY - m_tail - m_head;
        }
        
        //H == T when empty, our FULL case won't occur -> see isFull()
        return 0;
    }
    
    void clearBuffer()
    {
        m_tail = 0;
        m_head = 0;
    }
    
    size_t capacity()
    {
        return CAPACITY - 1;
    }
};