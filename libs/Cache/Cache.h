#ifndef CACHE
#define CACHE

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../utils/utils.h"

// Need to forward declare to let the compiler know this exists.
namespace utils
{
    struct address;
    struct block;
}

class Cache
{
    unsigned int size;
    unsigned int level;
    unsigned int number_of_blocks;
    std::string replacement_policy;
    
    // tag_store realated information
    unsigned int number_of_sets;        // AKA the number of rows in tag_store
    unsigned int associativity;         // AKA the number of columns in tag_store

    // Direct mapping members
    unsigned int victim_lru = 1;
    utils::block *victim_cache;
    utils::block **sets;

    // LRU Policy
    std::unordered_map<unsigned int, utils::block> **set_maps;
    unsigned int *mru;

    static unsigned int number_of_caches;
    static unsigned int block_size;    

    public:
        Cache (std::tuple<std::string, unsigned int, unsigned int>, unsigned int block_size, std::string replacement_policy);
        ~Cache (void);

        utils::address direct_map(utils::address addr);
        utils::address lru(utils::address addr);
        utils::address plru(utils::address addr);

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