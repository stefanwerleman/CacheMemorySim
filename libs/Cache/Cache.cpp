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

    if (this->associativity <= 1)
    {
        this->sets = new utils::block* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            this->sets[set] = new utils::block [this->associativity];
        }

        this->victim_cache = new utils::block [2];
    }

    if (this->replacement_policy == LRU)
    {
        this->set_maps = new std::unordered_map<unsigned int, utils::block>* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            std::unordered_map<unsigned int, utils::block> *new_set = new std::unordered_map<unsigned int, utils::block>;
            this->set_maps[set] = new_set;
        }

        this->mru = new unsigned [this->number_of_sets];
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

        delete [] this->sets;
    }

    if (this->victim_cache != NULL)
    {
        delete [] this->victim_cache;
    }

    if (this->set_maps != NULL)
    {
        for (int set = 0; set < this->number_of_sets; set++)
        {
            delete this->set_maps[set];
        }

        delete [] this->set_maps;
    }

    if (this->mru != NULL)
    {
        delete [] this->mru;
    }
}

utils::address Cache::direct_map(utils::address addr)
{
    utils::address evictee;

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
        evictee = this->victim_cache[this->victim_lru].addr; 
        utils::block victim = this->sets[addr.index][DIRECT_BLOCK];
        this->victim_cache[this->victim_lru] = victim;
        this->victim_lru = !this->victim_lru;
        this->sets[addr.index][DIRECT_BLOCK].tag = addr.tag;
    }

    return evictee;
}

utils::address Cache::lru(utils::address addr)
{
    utils::address evictee;
    std::unordered_map<unsigned int, utils::block> *current_set = this->set_maps[addr.index];

    if (current_set->find(addr.tag) == current_set->end())
    {
        utils::block new_block;
        
        if (current_set->size() < this->associativity)
        {
            // MISS
            new_block.addr = addr;
            new_block.tag = addr.tag;
            new_block.valid = true;
            new_block.sequence_number = this->mru[addr.index];
            this->mru[addr.index]++;

            current_set->emplace(addr.tag, new_block);
        }
        else
        {
            // REPLACE
            new_block.addr = addr;
            new_block.tag = addr.tag;
            new_block.valid = true;
            new_block.sequence_number = this->mru[addr.index];
            this->mru[addr.index]++;

            unsigned int lru = UINT16_MAX;
            utils::block evictee_block;

            for (auto& block: (*current_set))
            {
                if (block.second.sequence_number < lru)
                {
                    lru = block.second.sequence_number;
                    evictee_block = block.second;
                }
            }

            current_set->erase(evictee_block.tag);
            current_set->emplace(addr.tag, new_block);
            evictee = evictee_block.addr;
            return evictee;
        }
    }
    else
    {
        // HIT
        current_set->at(addr.tag).sequence_number = this->mru[addr.index];
        this->mru[addr.index]++;
    }

    return evictee;
}

utils::address Cache::run_cache(utils::address addr)
{
    utils::address evictee;
    if (this->associativity == DIRECT_MAP)
    {
        evictee = this->direct_map(addr);
    }
    else if (this->replacement_policy == LRU)
    {
        evictee = this->lru(addr);
    }
    else if (this->replacement_policy == PLRU)
    {
        // std::cout << "PLRU" << std::endl;
    }
    else if (this->replacement_policy == OPTIMAL)
    {
        // std::cout << "OPTIMAL" << std::endl;
    }

    return evictee;
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