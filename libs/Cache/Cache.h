#ifndef CACHE
#define CACHE

#include <iostream>
#include <string>
#include <array>

#include "../ArgumentWrapper/ArgumentWrapper.h"

class Cache
{
    unsigned int size;
    unsigned int associativity;
    unsigned int level;
    unsigned int number_of_sets;
    unsigned int number_of_blocks;

    static unsigned int number_of_caches;

    public:
        Cache (ArgumentWrapper arguments);

        unsigned int get_size(void);
        unsigned int get_associativity(void);
        unsigned int get_level(void);
        unsigned int get_number_of_sets(void);
        unsigned int get_number_of_blocks(void);
        unsigned int get_number_of_caches(void);
};

std::ostream& operator << (std::ostream &output, Cache cache);

#endif