#include <string>

#include "utils.h"

// Returns the right format for iterables
std::string utils::get_separator(int current, int length)
{
    if (current < length - 1)
    {
        return ", ";
    }
    else
    {
        return "";
    }
}