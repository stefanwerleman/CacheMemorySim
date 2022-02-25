#include <iostream>

#include "../libs/ArgumentWrapper/ArgumentWrapper.h"

int main(int argc, char **argv)
{
    const int ARGUMENT_LOWER_LIMIT = 9;

    if (argc < ARGUMENT_LOWER_LIMIT)
    {
        std::cout << "Insufficient arguments" << std::endl;
        return 1;
    }

    ArgumentWrapper arguments(argc, argv);

    std::cout << arguments;

	return 0;
}
