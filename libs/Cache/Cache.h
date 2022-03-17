#ifndef CACHE
#define CACHE

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
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

    // LRU Policy
    std::unordered_map<unsigned int, utils::block> **set_maps;
    unsigned int *mru;

    // PLRU Policy
    bool **plru_tree;
    unsigned int *next;
    std::unordered_map<unsigned int, unsigned int> **hash_map;
    
    // Optimal Policy
    static utils::address *traces;

    static unsigned int number_of_caches;
    static unsigned int block_size;    

    public:
        utils::block **sets;
        Cache (std::tuple<std::string, unsigned int, unsigned int>, 
               unsigned int block_size, 
               std::string replacement_policy);

        ~Cache (void);

        utils::address direct_map(utils::address addr,
                                  unsigned int *reads,
                                  unsigned int *read_misses,
                                  unsigned int *writes,
                                  unsigned int *write_misses,
                                  unsigned int *write_backs);

        utils::address lru(utils::address addr,
                                  unsigned int *reads,
                                  unsigned int *read_misses,
                                  unsigned int *writes,
                                  unsigned int *write_misses,
                                  unsigned int *write_backs);

        utils::address plru(utils::address addr,
                                  unsigned int *reads,
                                  unsigned int *read_misses,
                                  unsigned int *writes,
                                  unsigned int *write_misses,
                                  unsigned int *write_backs);

        utils::address optimal(utils::address addr,
                                  unsigned int *reads,
                                  unsigned int *read_misses,
                                  unsigned int *writes,
                                  unsigned int *write_misses,
                                  unsigned int *write_backs);

        utils::address invalidate(utils::address addr);

        utils::address run_cache(utils::address addr,
                                 unsigned int *reads,
                                 unsigned int *read_misses,
                                 unsigned int *writes,
                                 unsigned int *write_misses,
                                 unsigned int *write_backs);

        utils::address run_cache(char operation, 
                                 std::string input_address, 
                                 int trace_loc,
                                 unsigned int *reads,
                                 unsigned int *read_misses,
                                 unsigned int *writes,
                                 unsigned int *write_misses,
                                 unsigned int *write_backs);


        // void cache_write()

        void set_traces(std::string *traces);

        unsigned int get_size(void);
        unsigned int get_associativity(void);
        unsigned int get_level(void);
        unsigned int get_number_of_sets(void);
        unsigned int get_number_of_blocks(void);
        unsigned int get_number_of_caches(void);
        utils::block **get_sets(void);
};

// Output a Cache in a clean way.
std::ostream& operator << (std::ostream &output, Cache cache);

#endif