#pragma once
template<typename T>

class HighPerformanceRingBuffer {
    
//requirements
    //buffer
        //raw ptr or array or vector
        //decide on array for now    
            //benefits what are they? list them all later
        //can't use list need a block of memory
    //tail
        //int
        //(ptr complicates) - we are paying the cost of ptrs we are trying to avoid
    //head
        //int
    //insert single
    //insert multiple elements
    //get single
    //get multiple elements
    //size
        //element count
    //clear buffer (all)
    //remove item
    //return capacity
        //size of memory

//targetted methods are public
};