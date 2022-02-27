#ifndef CACHE
#define CACHE

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include <iostream>
#include <string>

// Denotes that there is nothing in a block
const int EMPTY_BIT =  -1;

typedef struct address
{
    std::string addr;
    char operation;
    unsigned int tag;
    unsigned int index;
    unsigned int offset;
} address;

typedef struct set
{
    int valid = EMPTY_BIT;
    int tag = EMPTY_BIT;

    // For LRU only.
    int sequence_number = EMPTY_BIT;
} set;

class Cache
{
    unsigned int size;
    unsigned int level;
    unsigned int number_of_blocks;
    
    // tag_store realated information
    unsigned int number_of_sets;        // AKA the number of rows in tag_store
    unsigned int associativity;         // AKA the number of columns in tag_store
    set **tag_store;
    
    int *mru;

    static unsigned int number_of_caches;

    set **create_tag_store(void);

    public:
        Cache (ArgumentWrapper arguments);
        ~Cache ();

        unsigned int get_size(void);
        unsigned int get_associativity(void);
        unsigned int get_level(void);
        unsigned int get_number_of_sets(void);
        unsigned int get_number_of_blocks(void);
        unsigned int get_number_of_caches(void);
        void run_cache(ArgumentWrapper arguments);
        address parse_address(char operation, std::string input_address, unsigned int block_size);
        
        // For now, returns if it's HIT, MISS, or REPLACE
        void LRU(address addr);
};

// Output a Cache in a clean way.
std::ostream& operator << (std::ostream &output, Cache cache);

// Output a set in a clean way.
std::ostream& operator << (std::ostream &output, set s);

std::ostream& operator << (std::ostream &output, address addr);

#endif