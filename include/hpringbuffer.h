#pragma once
#include <array>
#include <list>
#include <iostream>
#include <stdexcept>

//A ring buffer is a datastructure that provides optimized read and write speeds with no additional memory allocation overhead
    //requirements are in the comments below for a high performance ring buffer
    //requirement targetted methods are public -> minimal guaranteed features of this class
template<typename T, size_t CAPACITY>
class HighPerformanceRingBuffer {
private:
    //compare benefits of which data struct to set for m_buffer
    std::array<T, CAPACITY> m_buffer { };
    
    //tail is next write
    int m_tail { 0 };
    
    //head is next read
    int m_head { 0 };


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

    // //default constructor
    // HighPerformanceRingBuffer();
    
    // //default destructor
    // ~HighPerformanceRingBuffer();

    //push an element   
    bool push(T v)
    { 
        if (!isFull())
        {
            m_buffer[m_tail] = v;

            m_tail = (m_tail + 1) % CAPACITY;
            
            return true;
        } 
        else 
        {
            return false;
        }
    }
    
    //push multiple elements
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

    //read an element
    const T pop()
    {
        if (isEmpty())
        {
            return default_value();
        }
        
        T res = m_buffer[m_head];
        m_head = (m_head + 1) % CAPACITY;
        
        return res;
    }
    
    //read multiple elements
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
            for (auto x = 0; x < currentSize; x++){
                res.push_back(pop());
            }
            return res;
        }
    }

    //returns element count in ring buffer
    size_t size() const
    {
        if (m_head < m_tail){
            return m_tail - m_head;
        }
        
        if (m_tail < m_head){
            return CAPACITY - m_head - m_tail;
        }
        
        //H == T when empty, our FULL case won't occur -> see isFull()
        return 0;
    }
    
    void clearBuffer()
    {
        m_head = 0;
        m_tail = 0;
    }
    
    //get max capacity of ring buffer
    size_t capacity()
    {
        return CAPACITY - 1;
    }
};