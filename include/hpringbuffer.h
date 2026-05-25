#pragma once
#include <array>

//requirements for a high performance ring buffer
    //A buffer datastructure that provides optimized read and write speeds with no additional memory allocation overhead
template<typename T, int buffer_dimension>
class HighPerformanceRingBuffer {
private:
    //raw ptr or array or vector    
    //decide on array for now    
        //benefits what are they? list them all later
            //can't use list need a block of memory
    std::array<T, buffer_dimension> m_buffer { };
    
    //tail int
        //(ptr complicates) - we are paying the cost of ptrs we are trying to avoid
    int m_tail { 0 };
    
    //head int
    int m_head { 0 };

    size_t m_size { 0 };
protected:

public:

    //insert single 
        //should we do pass by value or pass by reference and is it ever pass by pointer
    void insert(T& v)
    { 
        //should this be written this or separate into two lines
        m_buffer[m_tail++] = *v;
        m_size++;
    }
    //insert multiple elements
    //get single
    const T get() const
    {
        return m_buffer[m_head++];
    }
    //get multiple elements
    //size
        //element count
    const size_t size() const
    {
        return m_size;
    }
    //clear buffer (all)
    void clearBuffer()
    {
        for(auto index : m_buffer){
            *index = std::default(T);
        }
        m_size = 0;
        m_head = 0;
        m_tail = 0;
    }
    //remove item
    void remove(int index){
        //boundscheck
            m_buffer[index] = std::default(T);
    }
    //return capacity
        //size of memory
    size_t capacity(){
        return buffer_dimension;
    }
//requirement targetted methods are public -> it's in the name because that's the guaranteed service that's provided
};