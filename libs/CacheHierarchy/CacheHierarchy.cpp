#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <tuple>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "CacheHierarchy.h"
#include "../Cache/Cache.h"
#include "../utils/utils.h"

const std::string NON_INCLUSIVE = "NON-INCLUSIVE";
const std::string INCLUSIVE = "INCLUSIVE";
const unsigned int L1_CACHE = 0;
const unsigned int L2_CACHE = 1;

const int NUMBER_OF_TRACES = 100001;

CacheHierarchy::CacheHierarchy (ArgumentWrapper arguments)
{
    this->inclusion_property = arguments.get_inclusion_property();
    this->trace_file = arguments.get_trace_file();

    if (arguments.get_replacement_policy() == "OPTIMAL")
    {
        this->traces = new std::string [NUMBER_OF_TRACES];
    }

    for (std::tuple<std::string, unsigned int, unsigned int> level : arguments.get_levels())
    {
        if (std::get<1>(level) > 0 && std::get<2>(level) > 0)
        {
            Cache *cache = new Cache(level, arguments.get_block_size(), arguments.get_replacement_policy());
            this->caches.push_back(cache);
        }
    }
}

CacheHierarchy::~CacheHierarchy(void)
{
    if (this->caches.size() > 0)
    {
        for (Cache *cache : this->caches)
        {
            if (cache != NULL)
            {
                delete cache;
            }
        }
    }

    if (this->traces != NULL)
    {
        delete [] this->traces;
    }
}

void CacheHierarchy::run_cache_hierarchy(void)
{
    if (this->traces != NULL)
    {
        this->get_traces();
        this->caches[0]->set_traces(this->traces);
    }

    std::ifstream file("./data/traces/" + this->trace_file);

    if (!file.is_open())
    {
        std::cout << "File failed to open." << std::endl;
        return;
    }

    std::string in;
    int trace_loc = 0;

    while (file >> in)
    {
        char operation = in[0];
        std::string input_address;

        file >> input_address;

        // TODO: Refactor for multiple caches.
        
        if (this->caches[L1_CACHE]->get_number_of_caches() == 1)
        {
            utils::address addr = this->caches[L1_CACHE]->run_cache(operation, 
                                                                    input_address, 
                                                                    trace_loc,
                                                                    &(this->l1_reads),
                                                                    &(this->l1_read_misses),
                                                                    &(this->l1_writes),
                                                                    &(this->l1_write_misses),
                                                                    &(this->l1_write_backs));
        }
        else
        {
            utils::address addr = this->caches[L1_CACHE]->run_cache(operation, 
                                                                    input_address, 
                                                                    trace_loc,
                                                                    &(this->l1_reads),
                                                                    &(this->l1_read_misses),
                                                                    &(this->l1_writes),
                                                                    &(this->l1_write_misses),
                                                                    &(this->l1_write_backs));

            addr = this->caches[L2_CACHE]->run_cache(operation, 
                                                     input_address, 
                                                     trace_loc,
                                                     &(this->l2_reads),
                                                    &(this->l2_read_misses),
                                                    &(this->l2_writes),
                                                    &(this->l2_write_misses),
                                                    &(this->l2_write_backs));
            
            if (this->inclusion_property == INCLUSIVE && addr.tag != -1)
            {
                utils::address invalid_addr = this->caches[L1_CACHE]->invalidate(addr);
            }
        }

        trace_loc++;

        // TODO: Non-returned address will have -1 as values. Make sure you ignore them.
    }

    file.close();
}

// Have to call a separate method outside of the constructor for read file to work.
void CacheHierarchy::get_traces(void)
{
    std::ifstream file("./data/traces/" + this->trace_file);

    if (!file.is_open())
    {
        std::cout << "File failed to open." << std::endl;
        exit(EXIT_FAILURE);
        return;
    }

        // std::cout << in << std::endl;
    std::string in;
    for (int input = 0; getline(file, in); input++)
    {   
        this->traces[input] = in;
    }

    file.close();
}

void CacheHierarchy::print_sim_configs(ArgumentWrapper arguments)
{
    std::cout << "BLOCKSIZE:             " << arguments.get_block_size() << std::endl;
    std::cout << "L1_SIZE:               " << arguments.get_levels()[L1_CACHE] << std::endl;
    std::cout << "L1_ASSOC:              " << arguments.get_levels()[L1_CACHE] << std::endl;
    std::cout << "L2_SIZE:               " << arguments.get_levels()[L2_CACHE] << std::endl;
    std::cout << "L2_ASSOC:              " << arguments.get_levels()[L2_CACHE] << std::endl;
    std::cout << "REPLACEMENT POLICY:    " << arguments.get_replacement_policy() << std::endl;
    
    // TODO: CHANGE TO LOWERCASE FOR ALL.
    std::cout << "INCLUSION PROPERTY:    " << arguments.get_inclusion_property() << std::endl;
    std::cout << "trace_file:            " << arguments.get_trace_file() << std::endl;
}

void CacheHierarchy::print_sim_results(void)
{
    std::cout << "a. number of L1 reads:        " << this->l1_reads << std::endl;
    std::cout << "b. number of L1 read misses:  " << this->l1_read_misses << std::endl;
    std::cout << "c. number of L1 writes:       " << this->l1_writes << std::endl;
    std::cout << "d. number of L1 write misses: " << this->l1_write_misses << std::endl;
    std::cout << "e. L1 miss rate:              " << this->l1_miss_rate << std::endl;
    std::cout << "f. number of L1 writebacks:   " << this->l1_write_backs << std::endl;
    std::cout << "g. number of L2 reads:        " << this->l2_reads << std::endl;
    std::cout << "h. number of L2 read misses:  " << this->l2_read_misses << std::endl;
    std::cout << "i. number of L2 writes:       " << this->l2_writes << std::endl;
    std::cout << "j. number of L2 write misses: " << this->l2_write_misses << std::endl;
    std::cout << "k. L2 miss rate:              " << this->l2_miss_rate << std::endl;
    std::cout << "l. number of L2 writebacks:   " << this->l2_write_backs << std::endl;
    std::cout << "m. total memory traffic:      " << this->total_memory_traffic << std::endl;
}

void CacheHierarchy::print_final_cache(utils::block **cache, 
                                       unsigned int number_of_sets, 
                                       unsigned int number_of_ways)
{
    for (int set = 0; set < number_of_sets; set++)
    {
        // TODO: WILL THIS BREAK?
        std::cout << "Set" << "\t" << set << ":" << "\t";
        for (int way = 0; way < number_of_ways; way++)
        {
            std::cout << utils::to_hex(cache[set][way].tag);

            if (cache[set][way].dirty_bit)
            {
                std::cout << " " << "D\t";
            }
            else
            {
                std::cout << "\t";
            }
        }
        std::cout << std::endl;
    }
}

void CacheHierarchy::print_results(ArgumentWrapper arguments)
{
    std::cout << "===== Simulator configuration =====" << std::endl;

    this->print_sim_configs(arguments);

    // TODO: PRINT 2D SETS OF BOTH CACHES.
    if (this->caches[L1_CACHE]->get_number_of_caches() == 1)
    {
        std::cout << "===== L1 contents =====" << std::endl;
        this->print_final_cache(this->caches[L1_CACHE]->get_sets(), 
                                this->caches[L1_CACHE]->get_number_of_sets(), 
                                this->caches[L1_CACHE]->get_associativity());
    }
    else
    {
        std::cout << "===== L1 contents =====" << std::endl;
        this->print_final_cache(this->caches[L1_CACHE]->get_sets(), 
                                this->caches[L1_CACHE]->get_number_of_sets(), 
                                this->caches[L1_CACHE]->get_associativity());

        std::cout << "===== L2 contents =====" << std::endl;
        this->print_final_cache(this->caches[L2_CACHE]->get_sets(), 
                                this->caches[L2_CACHE]->get_number_of_sets(), 
                                this->caches[L2_CACHE]->get_associativity());
    }

    std::cout << "===== Simulation results (raw) =====" << std::endl;
    this->print_sim_results();
}