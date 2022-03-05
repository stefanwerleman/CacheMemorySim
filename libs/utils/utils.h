#ifndef UTILS
#define UTILS

#include <iostream>
#include <string>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"

const int EMPTY_BIT = -1;

// Utils will have helper functions
namespace utils
{
    typedef struct address
    {
        std::string addr;
        char operation;
        unsigned int tag = -1;
        unsigned int index = -1;
        unsigned int offset = -1;
    } address;

    typedef struct block
    {
        int tag = EMPTY_BIT;
        bool valid = false;
        int sequence_number = EMPTY_BIT;
        utils::address addr;
    } block;
    
    address parse_address(char operation, 
                          std::string input_address, 
                          unsigned int block_size, 
                          unsigned int number_of_sets);


    // Prints an element in a clean way depending if it is the last element.
    std::string get_separator(int current, int length);
    std::ostream& operator << (std::ostream &output, utils::address addr);
    std::ostream& operator << (std::ostream &output, utils::block block);
}


#endif