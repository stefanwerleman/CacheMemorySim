#ifndef CACHE_HIERARCHY
#define CACHE_HIERARCHY

#include <string>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"

class CacheHierarchy
{
    std::vector<Cache*> caches;
    std::string inclusion_policy;

    public:
        CacheHierarchy(ArgumentWrapper arguments);
        ~CacheHierarchy();
};

#endif