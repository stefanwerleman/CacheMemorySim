#ifndef CACHE
#define CACHE

#include <iostream>
#include <string>
#include <tuple>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../utils/utils.h"

// Denotes that there is nothing in a block
const int EMPTY_BIT =  -1;

// Need to forward declare to let the compiler know this exists.
namespace utils
{
    struct address;
}

class Cache
{
    unsigned int size;
    unsigned int level;
    unsigned int number_of_blocks;
    
    // tag_store realated information
    unsigned int number_of_sets;        // AKA the number of rows in tag_store
    unsigned int associativity;         // AKA the number of columns in tag_store

    static unsigned int number_of_caches;
    static unsigned int block_size;

    public:
        Cache (std::tuple<std::string, unsigned int, unsigned int>, unsigned int block_size, std::string replacement_policy);

        utils::address run_cache(utils::address addr);
        utils::address run_cache(char operation, std::string input_address);

        unsigned int get_size(void);
        unsigned int get_associativity(void);
        unsigned int get_level(void);
        unsigned int get_number_of_sets(void);
        unsigned int get_number_of_blocks(void);
        unsigned int get_number_of_caches(void);
};

// Output a Cache in a clean way.
std::ostream& operator << (std::ostream &output, Cache cache);

#endif