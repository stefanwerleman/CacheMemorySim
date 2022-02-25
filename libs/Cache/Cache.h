#ifndef CACHE
#define CACHE

#include <iostream>
#include <string>
#include <array>

#include "../ArgumentWrapper/ArgumentWrapper.h"

class Cache
{
    static unsigned int number_of_caches;

    unsigned int size;
    unsigned int associativity;
    unsigned int level;
    unsigned int number_of_sets;
    unsigned int number_of_blocks;

    public:
        Cache (ArgumentWrapper arguments);
};

#endif