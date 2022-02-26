#include <iostream>
#include <cmath>
#include <string>
#include <tuple>

#include "Cache.h"
#include "../ArgumentWrapper/ArgumentWrapper.h"

unsigned int Cache::number_of_caches = 0;

Cache::Cache (ArgumentWrapper arguments)
{
    std::tuple<std::string, unsigned int, unsigned int> level = arguments.get_levels()[this->number_of_caches];
    
    this->size = std::get<1>(level);

    // Number of ways in a set
    this->associativity = std::get<2>(level);

    this->number_of_blocks = ceil((double)this->size / arguments.get_block_size());
    this->number_of_sets = ceil((double)this->number_of_blocks / this->associativity);

    this->number_of_caches++;
    this->level = this->number_of_caches;

    this->tag_store = this->create_tag_store();
}

Cache::~Cache (void)
{
    for (int way = 0; way < this->number_of_sets; way++)
    {
        delete this->tag_store[way];
    }

    delete this->tag_store;
}

set **Cache::create_tag_store(void)
{
    set **new_tag_store = new set* [this->number_of_sets];
    
    for (int way = 0; way < this->number_of_sets; way++)
    {
        new_tag_store[way] = new set [this->associativity];
    }

    return new_tag_store;
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

std::ostream& operator << (std::ostream &output, set s)
{
    output << "[V: " << s.valid << "; T: " << s.tag << "]";

    return output;
}