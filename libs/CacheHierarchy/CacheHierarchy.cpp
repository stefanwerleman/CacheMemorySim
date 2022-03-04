#include <fstream>
#include <string>
#include <tuple>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "CacheHierarchy.h"
#include "../Cache/Cache.h"
#include "../utils/utils.h"

const std::string NON_INCLUSIVE = "NON-INCLUSIVE";
const std::string INCLUSIVE = "INCLUSIVE";

CacheHierarchy::CacheHierarchy (ArgumentWrapper arguments)
{
    this->inclusion_property = arguments.get_inclusion_property();
    this->trace_file = arguments.get_trace_file();

    for (std::tuple<std::string, unsigned int, unsigned int> level : arguments.get_levels())
    {
        Cache *cache = new Cache(level, arguments.get_block_size(), arguments.get_replacement_policy());
        this->caches.push_back(cache);
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
}

void CacheHierarchy::run_cache_hierarchy(void)
{
    std::ifstream file("./data/traces/" + this->trace_file);

    if (!file.is_open())
    {
        std::cout << "File failed to open." << std::endl;
        return;
    }

    //     unsigned int number_of_hits = 0;
    //     unsigned int number_of_misses = 0;
    //     unsigned int number_of_replacements = 0;

    std::string in;
    while (file >> in)
    {
        char operation = in[0];
        std::string input_address;

        file >> input_address;

        // TODO: Refactor for multiple caches.
        utils::address addr = this->caches[0]->run_cache(operation, input_address);
    }

    file.close();
}