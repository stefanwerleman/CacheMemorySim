#include <iostream>

#include "../libs/ArgumentWrapper/ArgumentWrapper.h"
#include "../libs/Cache/Cache.h"

int main(int argc, char **argv)
{
    const int ARGUMENT_LOWER_LIMIT = 9;

    if (argc < ARGUMENT_LOWER_LIMIT)
    {
        std::cout << "Insufficient arguments" << std::endl;
        return 1;
    }

    ArgumentWrapper arguments(argc, argv);

    Cache L1(arguments);
    Cache L2(arguments);

	return 0;
}
