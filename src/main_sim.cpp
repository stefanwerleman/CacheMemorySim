#include <iostream>

#include "../libs/ArgumentWrapper/ArgumentWrapper.h"

int main(int argc, char **argv)
{
    unsigned int block_size = atoi(argv[1]); 
    unsigned int l1_size = atoi(argv[2]);
    unsigned int l1_associativity = atoi(argv[3]);
    unsigned int l2_size = atoi(argv[4]);
    unsigned int l2_associativity = atoi(argv[5]);
    unsigned int replacement_policy = atoi(argv[6]);
    unsigned int inclusion_property = atoi(argv[7]);
    std::string trace_file = argv[8];

    ArgumentWrapper argument_wrapper(
        block_size, 
        l1_size, 
        l1_associativity, 
        l2_size, 
        l2_associativity,
        replacement_policy,
        inclusion_property, 
        trace_file);

    std::cout << argument_wrapper;

	return 0;
}
