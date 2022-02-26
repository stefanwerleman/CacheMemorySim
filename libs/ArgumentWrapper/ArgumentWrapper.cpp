#include <string>
#include <tuple>
#include <vector>

#include "ArgumentWrapper.h"
#include "../utils/utils.h"

ArgumentWrapper::ArgumentWrapper (int argc, char **argv)
{
    const int BLOCKSIZE_ARGUMENT = 1;
    const int REPLACEMENT_POLICY_ARGUMENT = argc - 3;
    const int INCLUSION_PROPERTY_ARGUMENT = argc - 2;
    const int TRACE_FILE_ARGUMENT = argc - 1;

    // Keep all necessary information for our Caches and Addresses.
    this->block_size = atoi(argv[BLOCKSIZE_ARGUMENT]);
    this->replacement_policy = atoi(argv[REPLACEMENT_POLICY_ARGUMENT]);
    this->inclusion_property = atoi(argv[INCLUSION_PROPERTY_ARGUMENT]);
    this->trace_file = argv[TRACE_FILE_ARGUMENT];
    this->number_of_caches = (REPLACEMENT_POLICY_ARGUMENT - BLOCKSIZE_ARGUMENT - 1) / 2;
    
    // Want to store all cache levels in a list of tuples
    int num_level = 1;
    for (
        int level = BLOCKSIZE_ARGUMENT + 1; 
        level < REPLACEMENT_POLICY_ARGUMENT - 1; 
        level += 2)
    {
        std::string level_name = 'L' + std::to_string(num_level);
        unsigned int size = atoi(argv[level]);
        unsigned int associativity = atoi(argv[level + 1]);
        this->levels.push_back(std::make_tuple(level_name, size, associativity));

        num_level++;
    }
}

unsigned int ArgumentWrapper::get_block_size(void)
{
    return this->block_size;
}

// Returns a list of all the level caches.
std::vector<std::tuple<std::string, unsigned int, unsigned int>> ArgumentWrapper::get_levels(void)
{
    return this->levels;
}

// Cleaner to output the names of the policies.
std::string ArgumentWrapper::get_replacement_policy(void)
{
    if (this->replacement_policy == 0)
    {
        return "LRU";
    }
    else if (this->replacement_policy == 1)
    {
        return "PLRU";
    }
    else
    {
        return "Optimal";
    }
}

// Cleaner to output the names of the properties.
std::string ArgumentWrapper::get_inclusion_property(void)
{
    if (this->inclusion_property == 0)
    {
        return "non-inclusive";
    }
    else
    {
        return "inclusive";
    }
}

std::string ArgumentWrapper::get_trace_file(void)
{
    return this->trace_file;
}

unsigned int ArgumentWrapper::get_number_of_caches(void)
{
    return this->number_of_caches;
}

// Parses cache level into a meaningful string to easily read the contents
std::string tuple_to_string(std::tuple<std::string, unsigned int, unsigned int> level)
{
    return ("(" + 
            std::get<0>(level) + 
            ", " + 
            std::to_string(std::get<1>(level)) + 
            ", " + 
            std::to_string(std::get<2>(level)) +
            ")");
}

// Prints out the argument_wrapper in a clean way.
std::ostream& operator << (std::ostream &output, ArgumentWrapper argument_wrapper)
{
        output << "ArgumentWrapper: \n{" << std::endl;
        output << "\tBLOCK_SIZE: " << (argument_wrapper.get_block_size()) << " bytes," << std::endl;
        
        // Print all levels
        std::vector<std::tuple<std::string, unsigned int, unsigned int>> levels = argument_wrapper.get_levels();
        int current_level = 0;
        int number_of_levels = levels.size();

        output << "\tLEVELS: [ ";
        for (std::tuple<std::string, unsigned int, unsigned int> level: levels)
        {
            output << tuple_to_string(level) << utils::get_separator(current_level, number_of_levels);
            current_level++;
        }
        output << " ]," <<std::endl;

        output << "\tREPLACEMENT_POLICY: " << (argument_wrapper.get_replacement_policy()) << "," << std::endl;
        output << "\tINCLUSION_POLICY: " << (argument_wrapper.get_inclusion_property()) << "," << std::endl;
        output << "\ttrace_file: " << (argument_wrapper.get_trace_file()) << std::endl;
        output << "}" << std::endl;

        return output;
}

