#include <iostream>
#include <cmath>
#include <climits>
#include <bitset>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <tuple>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "Cache.h"
#include "../utils/utils.h"

unsigned int Cache::number_of_caches = 0;
unsigned int Cache::block_size = 0;
utils::address *Cache::traces = NULL;

const int NUMBER_OF_TRACES = 100001;

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
        this->sets = new utils::block* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            this->sets[set] = new utils::block [this->associativity];
        }

        this->set_maps = new std::unordered_map<unsigned int, utils::block>* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            std::unordered_map<unsigned int, utils::block> *new_set = new std::unordered_map<unsigned int, utils::block>;
            this->set_maps[set] = new_set;
        }

        this->mru = new unsigned [this->number_of_sets];
    }

    if (this->replacement_policy == PLRU)
    {
        this->set_maps = new std::unordered_map<unsigned int, utils::block>* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            std::unordered_map<unsigned int, utils::block> *new_set = new std::unordered_map<unsigned int, utils::block>;
            this->set_maps[set] = new_set;
        }

        this->sets = new utils::block* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            this->sets[set] = new utils::block [this->associativity];
        }

        this->plru_tree = new bool* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            this->plru_tree[set] = new bool [this->associativity - 1];
        }
    }

    if (this->replacement_policy == OPTIMAL)
    {
        this->sets = new utils::block* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            this->sets[set] = new utils::block [this->associativity];
        }

        this->set_maps = new std::unordered_map<unsigned int, utils::block>* [this->number_of_sets];

        for (int set = 0; set < this->number_of_sets; set++)
        {
            std::unordered_map<unsigned int, utils::block> *new_set = new std::unordered_map<unsigned int, utils::block>;
            this->set_maps[set] = new_set;
        }
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

    if (this->plru_tree != NULL)
    {
        for (int set = 0; set < this->number_of_sets; set++)
        {
            delete [] this->plru_tree[set];
        }

        delete [] this->plru_tree;
    }
}

utils::address Cache::direct_map(utils::address addr, 
                                 unsigned int *reads,
                                 unsigned int *read_misses,
                                 unsigned int *writes,
                                 unsigned int *write_misses,
                                 unsigned int *write_backs)
{
    utils::address evictee;

    if (addr.tag == this->sets[addr.index][DIRECT_BLOCK].tag)
    {
        // HIT
        if (addr.operation == 'w')
        {
            utils::write_back(&(this->sets[addr.index][DIRECT_BLOCK]), write_backs);
        }
    }
    else if (this->sets[addr.index][DIRECT_BLOCK].tag == -1)
    {
        if (addr.operation == 'w')
        {
            (*write_misses)++;
        }
        else if (addr.operation == 'r')
        {
            (*read_misses)++;
        }

        // MISS but can fill it.
        this->sets[addr.index][DIRECT_BLOCK].tag = addr.tag;
        this->sets[addr.index][DIRECT_BLOCK].valid = true;
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
        this->sets[addr.index][DIRECT_BLOCK].addr = addr;
    }

    return evictee;
}

utils::address Cache::lru(utils::address addr,
                          unsigned int *reads,
                          unsigned int *read_misses,
                          unsigned int *writes,
                          unsigned int *write_misses,
                          unsigned int *write_backs)
{
    utils::address evictee;
    std::unordered_map<unsigned int, utils::block> *current_set = this->set_maps[addr.index];
    utils::block *set = this->sets[addr.index];

    if (current_set->find(addr.tag) == current_set->end())
    {
        if (addr.operation == 'w')
        {
            (*write_misses)++;
        }
        else if (addr.operation == 'r')
        {
            (*read_misses)++;
        }

        utils::block new_block;

        new_block.addr = addr;
        new_block.tag = addr.tag;
        new_block.valid = true;
        new_block.sequence_number = this->mru[addr.index];
        this->mru[addr.index]++;
        
        if (current_set->size() < this->associativity)
        {
            // MISS
            new_block.way = current_set->size();
            current_set->emplace(addr.tag, new_block);
        }
        else
        {
            // REPLACE
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

            new_block.way = evictee_block.way;
            current_set->erase(evictee_block.tag);
            current_set->emplace(addr.tag, new_block);
            evictee = evictee_block.addr;
        }

        set[new_block.way] = new_block;
    }
    else
    {
        // HIT
        current_set->at(addr.tag).sequence_number = this->mru[addr.index];
        this->mru[addr.index]++;

        if (addr.operation == 'w')
        {
            utils::write_back(&current_set->at(addr.tag), write_backs);
        }
    }


    return evictee;
}

utils::address Cache::plru(utils::address addr,
                           unsigned int *reads,
                           unsigned int *read_misses,
                           unsigned int *writes,
                           unsigned int *write_misses,
                           unsigned int *write_backs)
{
    utils::address evictee;
    utils::block *current_set = this->sets[addr.index];
    std::unordered_map<unsigned int, utils::block> *set_map = this->set_maps[addr.index];
    bool *current_plru_tree = this->plru_tree[addr.index];
    bool needs_update = false;

    unsigned int high = this->associativity - 2;
    unsigned int low = 0;
    unsigned int mid = low + ((high - low) / 2);


    utils::block current_block;

    if (set_map->find(addr.tag) != set_map->end())
    {
        // HIT
        current_block = set_map->at(addr.tag);
        needs_update = true;

        if (addr.operation == 'w')
        {
            utils::write_back(&(set_map->at(addr.tag)), write_backs);
        }
    }
    else
    {
        if (addr.operation == 'w')
        {
            (*write_misses)++;
        }
        else if (addr.operation == 'r')
        {
            (*read_misses)++;
        }

        utils::block new_block;
        if (set_map->size() < this->associativity)
        {
            // MISS FILL
            new_block.way = set_map->size();
            needs_update = true;
        }
        else
        {
            // REPLACE
            utils::block evictee_block;

            while (high != low)
            {
                current_plru_tree[mid] = !current_plru_tree[mid];

                if (current_plru_tree[mid] == true)
                {
                    low = mid;
                    mid = low + ceil((high - low) / 2);
                }
                else
                {
                    high = mid;
                    mid = low + ((high - low) / 2);
                }
            }

            if (current_plru_tree[high] == true)
            {
                evictee_block = current_set[high + 1];
            }
            else
            {
                evictee_block = current_set[high];
            }

            new_block.way = evictee_block.way;
            set_map->erase(evictee_block.tag);
            evictee = evictee_block.addr;
            needs_update = false;
        }

        new_block.addr = addr;
        new_block.tag = addr.tag;
        new_block.valid = true;
        set_map->emplace(addr.tag, new_block);
        current_set[new_block.way] = new_block;

        current_block = new_block;
    }

    if (needs_update)
    {
        while (high != low)
        {
            if (current_block.way <= mid)
            {
                current_plru_tree[mid] = false;
                high = mid;
                mid = low + ((high - low) / 2);
            }
            else
            {
                current_plru_tree[mid] = true;
                low = mid;
                mid = low + ceil((high - low) / 2);
            }

            // We flipped the last bit. Done.
            if (mid == low || mid == high)
            {
                break;
            }
        }
    }

    return evictee;
}

utils::address Cache::optimal(utils::address addr,
                              unsigned int *reads,
                              unsigned int *read_misses,
                              unsigned int *writes,
                              unsigned int *write_misses,
                              unsigned int *write_backs)
{
    utils::address evictee;
    utils::block *current_set = this->sets[addr.index];
    std::unordered_map<unsigned int, utils::block> *set_map = this->set_maps[addr.index];
    std::unordered_set<unsigned int> current_opt;

    if (set_map->find(addr.tag) != set_map->end())
    {
        // HIT
        if (addr.operation == 'w')
        {
            utils::write_back(&(set_map->at(addr.tag)), write_backs);
        }
    }
    else
    {
        if (addr.operation == 'w')
        {
            (*write_misses)++;
        }
        else if (addr.operation == 'r')
        {
            (*read_misses)++;
        }

        utils::block new_block;
        if (set_map->size() < this->associativity)
        {
            // MISS FILL
            new_block.way = set_map->size();
        }
        else
        {
            // REPLACE
            utils::block evictee_block;

            // Check RHS
            for (int i = addr.trace_loc; i < NUMBER_OF_TRACES && current_opt.size() < this->associativity; i++)
            {
                if ((set_map->find(this->traces[i].tag) != set_map->end()) && (current_opt.find(traces[i].tag) == current_opt.end()))
                {
                    evictee_block = set_map->at(this->traces[i].tag);
                    current_opt.emplace(traces[i].tag);
                }
            }

            if (current_opt.size() < this->associativity)
            {
                // Check LHS
                for (int i = 0; i < addr.trace_loc && current_opt.size() < this->associativity; i++)
                {
                    if ((set_map->find(this->traces[i].tag) != set_map->end()) && (current_opt.find(traces[i].tag) == current_opt.end()))
                    {
                        evictee_block = set_map->at(this->traces[i].tag);
                        current_opt.emplace(traces[i].tag);
                    }
                }
            }

            new_block.way = evictee_block.way;
            set_map->erase(evictee_block.tag);
            evictee = evictee_block.addr;
        }

        new_block.addr = addr;
        new_block.tag = addr.tag;
        new_block.valid = true;
        set_map->emplace(addr.tag, new_block);
        current_set[new_block.way] = new_block;
    }

    return evictee;
}

utils::address Cache::invalidate(utils::address addr)
{
    if (this->associativity <= 1)
    {
        utils::block *current_set = this->sets[addr.index];

        if (current_set != NULL && addr.index < this->number_of_sets && addr.tag == current_set->addr.tag)
        {
            current_set->valid = false;
        }
    }
    else if (this->replacement_policy == LRU)
    {
        std::unordered_map<unsigned int, utils::block> *current_set = this->set_maps[addr.index];

        if (current_set != NULL && addr.index < this->number_of_sets && (current_set->find(addr.tag) != current_set->end()))
        {
             current_set->at(addr.tag).valid = false;
        }
    }
    else if (this->replacement_policy == PLRU || this->replacement_policy == OPTIMAL)
    {
        std::unordered_map<unsigned int, utils::block> *current_set_map = this->set_maps[addr.index];

        if (current_set_map != NULL && addr.index < this->number_of_sets && (current_set_map->find(addr.tag) != current_set_map->end()))
        {
             current_set_map->at(addr.tag).valid = false;
        }

        utils::block *current_set = this->sets[addr.index];

        if (current_set != NULL && addr.index < this->number_of_sets && addr.tag == current_set->addr.tag)
        {
            current_set->valid = false;
        }
    }

    return addr;
}

utils::address Cache::run_cache(utils::address addr,
                                unsigned int *reads,
                                unsigned int *read_misses,
                                unsigned int *writes,
                                unsigned int *write_misses,
                                unsigned int *write_backs)
{
    utils::address evictee;
    if (this->associativity == DIRECT_MAP)
    {
        evictee = this->direct_map(addr, reads, read_misses, writes, write_misses, write_backs);
    }
    else if (this->replacement_policy == LRU)
    {
        evictee = this->lru(addr, reads, read_misses, writes, write_misses, write_backs);
    }
    else if (this->replacement_policy == PLRU)
    {
        evictee = this->plru(addr, reads, read_misses, writes, write_misses, write_backs);
    }
    else if (this->replacement_policy == OPTIMAL)
    {
        evictee = this->optimal(addr, reads, read_misses, writes, write_misses, write_backs);
    }

    return evictee;
}

utils::address Cache::run_cache(char operation, 
                                std::string input_address, 
                                int trace_loc,
                                unsigned int *reads,
                                unsigned int *read_misses,
                                unsigned int *writes,
                                unsigned int *write_misses,
                                unsigned int *write_backs)
{
    utils::address addr = utils::parse_address(operation, input_address, this->block_size, this->number_of_sets);
    addr.trace_loc = trace_loc;

    if (addr.operation == 'w')
    {
        (*writes)++;
    }
    else if (addr.operation == 'r')
    {
        (*reads)++;
    }

    return this->run_cache(addr, reads, read_misses, writes, write_misses, write_backs);
}

void Cache::set_traces(std::string *traces)
{
    if (this->replacement_policy == OPTIMAL)
    {
        if (traces != NULL)
        {
            this->traces = new utils::address [NUMBER_OF_TRACES];
            for (int trace = 0; trace < NUMBER_OF_TRACES; trace++)
            {
                utils::address trace_addr = utils::parse_address(traces[trace], block_size, this->number_of_sets);
                trace_addr.trace_loc = trace;
                this->traces[trace] = trace_addr;
            }
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
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

utils::block **Cache::get_sets(void)
{
    return this->sets;
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