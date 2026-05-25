#pragma once
#include <array>
#include <list>

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

    //cleans up m_buffer after removal calls
    void reorganize_buffer_starting_at(int index)
    {
        int walk_ptr = index + 1;
        int write_ptr = index;
        while (write_ptr < (m_size || CAPACITY)){
            m_buffer[write_ptr++] = m_buffer[walk_ptr++];
        }
    }

public:

    // //default constructor
    // HighPerformanceRingBuffer();
    
    // //default destructor
    // ~HighPerformanceRingBuffer();

    //insert single 
        //should we do pass by value or pass by reference and is it ever pass by pointer
    void insert(T v)
    { 
        m_buffer[m_tail] = v;
        m_tail = (m_tail + 1) % CAPACITY;
        m_size++;
    }
    
    //insert multiple elements
    void insert(std::initializer_list<T> list)
    {
        if (list.size() > CAPACITY){
            std::cout << "Memory overwritten, data loss, max # of elements you can put in without overwriting in the ring buffer is x\n";
        } else {
            for (auto item : list) {
                insert(item);
            }
        }
    }

    //get single
    const T get()
    {
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
        return m_size;
    }
    
    //clear buffer (all)
    void clear_buffer()
    {
        for(auto index : m_buffer){
            index = default_value();
        }
        m_size = 0;
        m_head = 0;
        m_tail = 0;
    }
    
    //remove
    void remove(int index)
    {   
        if (index <= CAPACITY && index <= m_size){
            m_buffer[index] = default_value();
            m_size--;
            reorganize_buffer_starting_at(index);
        }
    }
    
    //return CAPACITY
        //size of memory
    size_t capacity()
    {
        return CAPACITY;
    }
};