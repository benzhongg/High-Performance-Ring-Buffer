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
    //placeholder values for unused cells in m_buffer
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

    bool headBeforeTail() const
    {
        return m_head < m_tail;
    }

    //repetitive function but makes code clear so this might stay
    bool tailBeforeHead() const
    {
        return m_tail < m_head;
    }

public:

    // //default constructor
    // HighPerformanceRingBuffer();
    
    // //default destructor
    // ~HighPerformanceRingBuffer();

    //insert single 
    bool insert(T v)
    { 
        if (!isFull()) {
            m_buffer[m_tail] = v;
            m_tail = (m_tail + 1) % CAPACITY;
            return true;
        } else {
            return false;
        }
    }
    
    //insert multiple elements
    bool multiInsert(std::initializer_list<T> list)
    {
        if (list.size() > (CAPACITY - size())){
            return false;
        }

        for (auto item : list) {
            insert(item);
        }

        return true;
    }

    //get single
    const T get()
    {
        if (isEmpty()){
            return default_value();
        }
        
        T res = m_buffer[m_head];
        m_head = (m_head + 1) % CAPACITY;
        
        return res;
    }
    
    //get multiple elements
    std::list<T> multiGet(int element_read_count)
    {
        if (element_read_count > size() || element_read_count > CAPACITY){
            return {};
        } else {
            std::list<T> res;

            auto currentSize = size();
            for (auto x = 0; x < currentSize; x++){
                res.push_back(get());
            }
            return res;
        }
    }

    //size - buffer element count
    size_t size() const
    {
        //H before T
        if (headBeforeTail()){
            return m_tail - m_head;
        }
        
        //T before H
        if (tailBeforeHead()){
            return CAPACITY - m_head - m_tail;
        }
        
        //H == T on empty / full
        return 0;
    }
    
    //clear buffer (all)  
    void clearBuffer()
    {
        m_head = 0;
        m_tail = 0;
    }
    
    //remove
    bool remove(int index)
    {   
        //bounds check
        if (index >= CAPACITY){
            return false;
        }
        
        m_buffer[index] = default_value();

        //is this readable?
        //buffer reorganization
        if (headBeforeTail() && (index >= m_head && index < m_tail)){
            //clean up buffer from the removed index
            while(index < (m_tail - 1)){
                m_buffer[index] = m_buffer[index + 1];
                index++;
            }
        } else if (tailBeforeHead() && (index >= m_head || index < m_tail)){
            //clean up buffer from the removed index
            while(index < (m_tail - 1)){
                m_buffer[index] = m_buffer[(index + 1) % CAPACITY];
                index = (index + 1) % CAPACITY;
            }
        } else {
            //valid index but invalid capacity
            return false;
        }
 
        m_tail = index;

        return true;
    }
    
    //get capacity of ring buffer
    size_t capacity()
    {
        return CAPACITY;
    }
};