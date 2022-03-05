#include <iostream>
#include <cmath>
#include <climits>
#include <bitset>
#include <sstream>
#include <string>
#include <tuple>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "Cache.h"
#include "../utils/utils.h"

unsigned int Cache::number_of_caches = 0;
unsigned int Cache::block_size = 0;

const unsigned int UPPER_BOUND = UINT_MAX;
const unsigned int DIRECT_BLOCK = 0;
const unsigned int DIRECT_MAP = 1;
const std::string LRU = "LRU";
const std::string PLRU = "PLRU";
const std::string OPTIMAL = "OPTIMAL";

enum Result { HIT = 1, MISS = -1, REPLACE = 0 };

Cache::Cache (std::tuple<std::string, unsigned int, unsigned int> level, unsigned int block_size, std::string replacement_policy)
{   
    this->block_size = block_size;
    this->size = std::get<1>(level);

    // Number of ways in a set
    this->associativity = std::get<2>(level);

    this->number_of_blocks = ceil((double)this->size / this->block_size);
    this->number_of_sets = ceil((double)this->number_of_blocks / this->associativity);

    this->number_of_caches++;
    this->level = this->number_of_caches;

    this->replacement_policy = replacement_policy;

    this->sets = new utils::block* [this->number_of_sets];

    for (int set = 0; set < this->number_of_sets; set++)
    {
        this->sets[set] = new utils::block [this->associativity];
    }

    if (this->associativity <= 1)
    {
        this->victim_cache = new utils::block [2];
    }
}

Cache::~Cache (void)
{
    // NOTE: It is important to use delete[] for any array types.

    if (this->sets != NULL)
    {
        for (int set = 0; set < this->number_of_sets; set++)
        {
            delete[] this->sets[set];
        }

        delete[] this->sets;
    }

    if (this->victim_cache != NULL)
    {
        delete[] this->victim_cache;
    }
}

utils::address* Cache::direct_map(utils::address addr)
{
    utils::address *evictee = NULL;

    if (addr.tag == this->sets[addr.index][DIRECT_BLOCK].tag)
    {
        // HIT
    }
    else if (this->sets[addr.index][DIRECT_BLOCK].tag == -1)
    {
        // MISS but can fill it.
        this->sets[addr.index][DIRECT_BLOCK].tag = addr.tag;
        this->sets[addr.index][DIRECT_BLOCK].tag = true;
        this->sets[addr.index][DIRECT_BLOCK].addr = addr;
    }
    else
    {
        // REPLACEMENT
        evictee = &(this->victim_cache[this->victim_lru].addr); 
        utils::block victim = this->sets[addr.index][DIRECT_BLOCK];
        this->victim_cache[this->victim_lru] = victim;
        this->victim_lru = !this->victim_lru;
        this->sets[addr.index][DIRECT_BLOCK].tag = addr.tag;
    }

    return evictee;
}

utils::address* Cache::run_cache(utils::address addr)
{
    if (this->associativity == DIRECT_MAP)
    {
        return this->direct_map(addr);
    }
    else if (this->replacement_policy == LRU)
    {
        // std::cout << "LRU" << std::endl;
    }
    else if (this->replacement_policy == PLRU)
    {
        // std::cout << "PLRU" << std::endl;
    }
    else if (this->replacement_policy == OPTIMAL)
    {
        // std::cout << "OPTIMAL" << std::endl;
    }
    
    return NULL;
}

utils::address* Cache::run_cache(char operation, std::string input_address)
{
    utils::address addr = utils::parse_address(operation, input_address, this->block_size, this->number_of_sets);
    return this->run_cache(addr);
}

unsigned int Cache::get_size(void)
{
    return this->size;
}

unsigned int Cache::get_associativity(void)
{
    return this->associativity;
}

unsigned int Cache::get_level(void)
{
    return this->level;
}

unsigned int Cache::get_number_of_sets(void)
{
    return this->number_of_sets;
}

unsigned int Cache::get_number_of_blocks(void)
{
    return this->number_of_blocks;
}

unsigned int Cache::get_number_of_caches(void)
{
    return this->number_of_caches;
}

// Output a Cache in a clean way.
std::ostream& operator << (std::ostream &output, Cache cache)
{
    output << "L" << cache.get_level() << ":" << std::endl;
    output << "{" << std::endl;
    output << "\tSIZE: " << cache.get_size() << " bytes," << std::endl;
    output << "\tASSOCIATIVITY: " << cache.get_associativity() << "," << std::endl;
    output << "\tNUMBER OF SETS: " << cache.get_number_of_sets() << "," << std::endl;
    output << "\tNUMBER OF BLOCKS: " << cache.get_number_of_blocks() << "," << std::endl;
    output << "}" << std::endl;

    return output;
}