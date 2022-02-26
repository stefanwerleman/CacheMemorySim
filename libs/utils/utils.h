#ifndef UTILS
#define UTILS

#include <string>

// Utils will have helper functions
namespace utils
{
    std::string get_separator(int current, int length);

    typedef struct address
    {
        unsigned int addr;
        unsigned int tag;
        unsigned int index;
        unsigned int offset;
    } address;
}

#endif