#include <iostream>
#include <string>

#include "../libs/ArgumentWrapper/ArgumentWrapper.h"
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


	return 0;
}
