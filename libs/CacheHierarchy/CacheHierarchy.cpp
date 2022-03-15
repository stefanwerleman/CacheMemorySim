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
        
        if (this->caches[0]->get_number_of_caches() == 1)
        {
            utils::address addr = this->caches[0]->run_cache(operation, input_address, trace_loc);
        }
        else
        {
            utils::address addr = this->caches[0]->run_cache(operation, input_address, trace_loc);

            addr = this->caches[1]->run_cache(operation, input_address, trace_loc);
            
            if (addr.tag != -1)
            {
                utils::address invalid_addr = this->caches[0]->invalidate(addr);
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