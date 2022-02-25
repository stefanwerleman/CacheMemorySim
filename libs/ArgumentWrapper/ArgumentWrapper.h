#ifndef ARGUMENT_WRAPPER
#define ARGUMENT_WRAPPER

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// Since there are many arguments, we will have a single place all of them.
class ArgumentWrapper
{
    // These will be private because we do not 
    // want to change after capturing them. 
    unsigned int block_size;
    unsigned int replacement_policy;
    unsigned int inclusion_property;

    std::vector<std::tuple<std::string, unsigned int, unsigned int>> levels;
    std::string trace_file;

    public:
        ArgumentWrapper (int argc, char **argv);

        // Using getters instead of directly accessing the members.
        unsigned int get_block_size(void);
        std::vector<std::tuple<std::string, unsigned int, unsigned int>> get_levels(void);
        std::string get_replacement_policy(void);
        std::string get_inclusion_property(void);
        std::string get_trace_file(void);
};

// Returns Level tuple as a string.
std::string tuple_to_string(std::tuple<std::string, unsigned int, unsigned int> level);

// Cleaner way to output this class.
std::ostream& operator << (std::ostream &output, ArgumentWrapper argument_wrapper);

#endif