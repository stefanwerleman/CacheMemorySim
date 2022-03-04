#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"

#ifndef UTILS
#define UTILS

#include <string>
#include <vector>

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

    address parse_address(char operation, std::string input_address, unsigned int block_size, unsigned int number_of_sets);

    // Prints an element in a clean way depending if it is the last element.
    std::string get_separator(int current, int length);
}

#endif