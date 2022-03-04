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
        unsigned int tag;
        unsigned int index;
        unsigned int offset;
    } address;

    address parse_address(char operation, 
                          std::string input_address, 
                          unsigned int block_size, 
                          unsigned int number_of_sets);

    typedef struct block
    {
        int tag = EMPTY_BIT;
        bool valid = false;
        utils::address addr;
    } block;

    // Prints an element in a clean way depending if it is the last element.
    std::string get_separator(int current, int length);
    std::ostream& operator << (std::ostream &output, utils::address addr);
    std::ostream& operator << (std::ostream &output, utils::block block);
}


#endif