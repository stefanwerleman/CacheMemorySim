#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"
#include <string>
#include "utils.h"
#include <vector>

// Prints an element in a clean way depending if it is the last element.
std::string utils::get_separator(int current, int length)
{
    if (current < length - 1)
    {
        return ", ";
    }
    else
    {
        return "";
    }
}

std::vector<Cache*> utils::create_memory_hierarchy(ArgumentWrapper arguments)
{
    std::vector<Cache*> memory_hierarchy;

    for (int level = 0; level < arguments.get_number_of_caches(); level++)
    {
        // Checking if this is an existing cache.
        unsigned int size = std::get<1>(arguments.get_levels()[level]);
        unsigned int associativity = std::get<2>(arguments.get_levels()[level]);

        if (size > 0 && associativity > 0)
        {
            Cache *cache = new Cache(arguments);
            memory_hierarchy.push_back(cache);
        }

    }

    return memory_hierarchy;
}

void utils::destroy_memory_hierarchy(std::vector<Cache*> memory_hierarchy)
{
    for (Cache *cache: memory_hierarchy)
    {
        delete cache;
    }
}