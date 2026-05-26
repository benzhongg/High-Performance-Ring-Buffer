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

    size_t m_size { 0 };

protected:
    //placeholder values for unused cells in m_buffer
    T default_value()
    {
        return T{};
    }

    bool isFull()
    {
        return size() == CAPACITY;    
    }

    bool isEmpty()
    {
        return size() == 0;
    }

    bool headBeforeTail()
    {
        return m_head < m_tail;
    }

    //repetitive function but makes code clear so this might stay
    bool tailBeforeHead()
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
        //should insert condition on isFull or this below
        if (m_tail != m_head) {
            m_buffer[m_tail] = v;
            m_tail = (m_tail + 1) % CAPACITY;
            m_size++;
            return true;
        } else {
            return false;
        }
    }
    
    //insert multiple elements
    bool insertRange(std::initializer_list<T> list)
    {
        if (list.size() > (CAPACITY - size())){
            std::cout << "Memory overwritten, data loss, max # of elements you can put in without overwriting in the ring buffer is x\n";
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
    std::list<T> get(int element_read_count)
    {
        if (element_read_count > CAPACITY){
            std::cout << "requested more elements than CAPACITY of the ring buffer\n";
        } else {
            std::list<T> res;
            for (int x = 0; x < m_size; x++){
                res.push_back(get());
            }
            return res;
        }
    }

    //size
        //element count
    const size_t size() const
    {
        if(m_tail >= m_head){
            return m_tail - m_head;
        } else {
            return CAPACITY - m_head - m_tail;
        }

        // //check 3 cases
        // //H before T
        // if (headBeforeTail()){

        // }
        
        // //T before H
        // if (tailBeforeHead()){
        
        // }
        
        // //H == T
        // if (m_head == m_tail){
        //     return m_size;
        // }
    }
    
    //clear buffer (all)  
    void clearBuffer()
    {
        m_size = 0;
        m_head = 0;
        m_tail = 0;
    }
    
    //remove
    void remove(int index)
    {   
        //bounds check
        if (index >= CAPACITY){
            std::cout << "out of range" << std::endl;
        }
        
        //check validity of index
        if (headBeforeTail() && (index >= m_head && index < m_tail)){
           
            m_size--;

            while(index < (m_tail - 1)){
                m_buffer[index] = m_buffer[index + 1];
                index++;
            }

            m_tail = index;

        } else if (tailBeforeHead() && (index >= m_head || index < m_tail)){
            
            m_size--;

            while(index < (m_tail - 1)){
                m_buffer[index] = m_buffer[(index + 1) % CAPACITY];
                index = (index + 1) % CAPACITY;
            }

            m_tail = index;

        } else {
            //User enters index = m_tail
            std::cout << "address alread y empty" << std::endl;
        }
    }
    
    //get capacity of ring buffer
    size_t capacity()
    {
        return CAPACITY;
    }
};