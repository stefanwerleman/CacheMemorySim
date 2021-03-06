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
        int tag = -1;
        int index = -1;
        int offset = -1;
        int trace_loc = -1;
    } address;

    typedef struct block
    {
        int tag = EMPTY_BIT;
        bool valid = false;
        bool dirty_bit = false;
        int sequence_number = EMPTY_BIT;    // LRU
        int way = -1;                       // PLRU
        utils::address addr;
    } block;
    
    address parse_address(char operation, 
                          std::string input_address, 
                          unsigned int block_size, 
                          unsigned int number_of_sets);

    address parse_address(std::string address, 
                          unsigned int block_size, 
                          unsigned int number_of_sets);

    void write_back(utils::block *block, unsigned int *write_backs);

    std::string to_hex(unsigned int binary_value);

    // Prints an element in a clean way depending if it is the last element.
    std::string get_separator(int current, int length);
    std::ostream& operator << (std::ostream &output, utils::address addr);
    std::ostream& operator << (std::ostream &output, utils::block block);
}


#endif