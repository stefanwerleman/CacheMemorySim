#include "../libs/ArgumentWrapper/ArgumentWrapper.h"
#include "../libs/Cache/Cache.h"
#include <iostream>
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

    // TODO: Begin Simulation after you understand replacement policies

    utils::destroy_memory_hierarchy(memory_hierarchy);
	return 0;
}
