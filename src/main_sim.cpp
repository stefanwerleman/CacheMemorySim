#include "../libs/ArgumentWrapper/ArgumentWrapper.h"
#include "../libs/Cache/Cache.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../libs/utils/utils.h"

int main(int argc, char **argv)
{
    const int ARGUMENT_LOWER_LIMIT = 7;

    // Need to be given at-least one cache
    if (argc < ARGUMENT_LOWER_LIMIT)
    {
        std::cout << "Insufficient arguments" << std::endl;
        return 1;
    }


    // Parses all arguments
    ArgumentWrapper arguments(argc, argv);

    std::vector<Cache*> memory_hierarchy = utils::create_memory_hierarchy(arguments);

    std::ifstream file("./data/traces/" + arguments.get_trace_file());

    std::string in;

    while (file >> in)
    {
        char operation = in[0];
        file >> in;
        std::string address = in;

        std::cout << memory_hierarchy[0]->parse_address(operation, address, arguments.get_block_size());
    }

    // TODO: Begin Simulation
    
    file.close();
    utils::destroy_memory_hierarchy(memory_hierarchy);
	return 0;
}
