#ifndef UTILS
#define UTILS

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"
#include <string>
#include <vector>

// Utils will have helper functions
namespace utils
{
    typedef struct address
    {
        unsigned int addr;
        unsigned int tag;
        unsigned int index;
        unsigned int offset;
    } address;

    std::string get_separator(int current, int length);

    std::vector<Cache*> create_memory_hierarchy(ArgumentWrapper arguments);

    void destroy_memory_hierarchy(std::vector<Cache*> memory_hierarchy);
}

#endif