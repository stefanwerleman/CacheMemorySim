#include <iostream>
#include <cmath>
#include <climits>
#include <bitset>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "Cache.h"
#include "../utils/utils.h"

unsigned int Cache::number_of_caches = 0;
const unsigned int UPPER_BOUND = UINT_MAX;

enum Result { HIT = 1, MISS = -1, REPLACE = 0 };

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

    this->mru = new int [this->number_of_sets];
}

Cache::~Cache (void)
{
    for (int way = 0; way < this->number_of_sets; way++)
    {
        delete this->tag_store[way];
    }

    delete this->tag_store;

    delete this->mru;
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

void Cache::run_cache(ArgumentWrapper arguments)
{
    std::ifstream file("./data/traces/" + arguments.get_trace_file());

    if (!file.is_open())
    {
        std::cout << "File failed to open." << std::endl;
        return;
    }

    unsigned int number_of_hits = 0;
    unsigned int number_of_misses = 0;
    unsigned int number_of_replacements = 0;

    std::string in;
    while (file >> in)
    {
        char operation = in[0];
        std::string input_address;

        file >> input_address;

        utils::address addr = utils::parse_address(operation, input_address, arguments.get_block_size(), this->number_of_sets);

        if (this->associativity == 1)
        {
            // Run Direct Mapping. No replacement policy
            std::cout << "Direct Mapping Not Ready" <<std::endl;
            return;
        }
        else
        {
            // Running LRU policy for now
            this->LRU(addr);
        }
    }
}

void Cache::LRU(utils::address addr)
{
    // Needed for replacements
    bool need_replacement = true;
    unsigned int min_lru = UPPER_BOUND;
    unsigned min_lru_location = 0;

    // Needed to continuously update the MRU for any sequence number.
    int *mru = &(this->mru[addr.index]);
 
    set *current_set = this->tag_store[addr.index];

    for (int way = 0; way < this->associativity; way++)
    {
        // Immediately find the LRU and the location of the way that is the LRU
        if (current_set[way].sequence_number != -1 && current_set->sequence_number < min_lru)
        {
            min_lru = current_set[way].sequence_number;
            min_lru_location = way;
        }

        if (addr.tag == current_set[way].tag)       // If there is a hit, update MRU only.
        {
            (*mru)++;
            current_set[way].sequence_number = (*mru);

            // If we already updated the sequence, no need to replace
            need_replacement = false;
        }
        else if (current_set[way].tag == -1)        // If MISS but there is room, just fill it in
        {
            current_set[way].tag = addr.tag;
            (*mru)++;
            current_set[way].sequence_number = (*mru);

            // If we already filled in any empty block, no need to replace.
            need_replacement = false;
        }
    }

    if (need_replacement)
    {
        current_set[min_lru_location].tag = addr.tag;
        (*mru)++;
        current_set[min_lru_location].sequence_number = (*mru);
    }

    return;
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

// Output a set in a clean way.
std::ostream& operator << (std::ostream &output, set s)
{
    output << "[V: " << s.valid << "; T: " << s.tag << "]";

    return output;
}