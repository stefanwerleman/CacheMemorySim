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
    std::string *traces;

    // L1 Results
    unsigned int l1_reads = 0;
    unsigned int l1_read_misses = 0;
    unsigned int l1_writes = 0;
    unsigned int l1_write_misses = 0;
    double l1_miss_rate = 0.0;
    unsigned int l1_write_backs = 0;

    // L2 Results
    unsigned int l2_reads = 0;
    unsigned int l2_read_misses = 0;
    unsigned int l2_writes = 0;
    unsigned int l2_write_misses = 0;
    double l2_miss_rate = 0.0;
    unsigned int l2_write_backs = 0; 

    unsigned int total_memory_traffic = 0;

    public:
        CacheHierarchy(ArgumentWrapper arguments);
        ~CacheHierarchy(void);

        void run_cache_hierarchy(void);
        void get_traces();
};

#endif