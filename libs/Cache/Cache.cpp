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
}

utils::address Cache::run_cache(utils::address addr)
{
    return addr;
}

utils::address Cache::run_cache(char operation, std::string input_address)
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