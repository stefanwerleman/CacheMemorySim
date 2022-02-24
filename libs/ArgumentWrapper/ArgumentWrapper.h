#ifndef ARGUMENT_WRAPPER
#define ARGUMENT_WRAPPER

#include <iostream>
#include <string>

// Since there are many arguments, we will have a single place all of them.
class ArgumentWrapper
{
    // These will be private because we do not 
    // want to change after capturing them. 
    unsigned int block_size;
    unsigned int l1_size;
    unsigned int l1_associativity;
    unsigned int l2_size;
    unsigned int l2_associativity;
    unsigned int replacement_policy;
    unsigned int inclusion_property;
    std::string trace_file;

    public:
        ArgumentWrapper(
            unsigned int block_size, 
            unsigned int l1_size, 
            unsigned int l1_associativity,
            unsigned int l2_size,
            unsigned int l2_associativity,
            unsigned int replacement_policy,
            unsigned int inclusion_property,
            std::string trace_file);

        // Using getters instead of directly accessing the members.
        unsigned int get_block_size(void);
        unsigned int get_l1_size(void);
        unsigned int get_l1_associativity(void);
        unsigned int get_l2_size(void);
        unsigned int get_l2_associativity(void);
        std::string get_replacement_policy(void);
        std::string get_inclusion_property(void);
        std::string get_trace_file(void);
};

// Cleaner way to output this class.
std::ostream& operator << (std::ostream &output, ArgumentWrapper argument_wrapper);

#endif