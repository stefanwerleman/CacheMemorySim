#ifndef CACHE_HIERARCHY
#define CACHE_HIERARCHY

#include <string>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"

class CacheHierarchy
{
    std::string inclusion_property;
    std::string trace_file;
    std::vector<Cache*> caches;

    public:
        CacheHierarchy(ArgumentWrapper arguments);
        ~CacheHierarchy(void);

        void run_cache_hierarchy(void);
};

#endif