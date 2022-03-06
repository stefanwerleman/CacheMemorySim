#include <cmath>
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../ArgumentWrapper/ArgumentWrapper.h"
#include "../Cache/Cache.h"
#include "utils.h"

utils::address utils::parse_address(char operation, std::string input_address, unsigned int block_size, unsigned int number_of_sets)
{
    utils::address addr;
    
    addr.operation = operation;
    
    unsigned mask;
    std::stringstream address_stream;
    address_stream << std::hex << input_address;
    address_stream >> mask;
    std::bitset<32> binary_addr(mask);
    
    unsigned int num_index_bits = log2(number_of_sets);
    unsigned int num_offset_bits = log2(block_size);
    unsigned int num_tag_bits = 32 - num_index_bits - num_offset_bits;

    std::string address_string = binary_addr.to_string();

    addr.addr = address_string;
    addr.tag = std::stoi(address_string.substr(0, num_tag_bits), nullptr, 2);
    addr.index = std::stoi(address_string.substr(num_tag_bits, num_index_bits), nullptr, 2);
    addr.offset = std::stoi(address_string.substr(num_index_bits, num_offset_bits), nullptr, 2);

    return addr;
}

utils::address utils::parse_address(std::string address, unsigned int block_size, unsigned int number_of_sets)
{   
    char operation = address[0];
    std::string input_address;

    for (int c = 2; c < address.length(); c++)
        input_address += address[c];
    
    return utils::parse_address(operation, input_address, block_size, number_of_sets);
}

// Prints an element in a clean way depending if it is the last element.
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

std::ostream& utils::operator << (std::ostream &output, utils::address addr)
{
    output << "Address: {" << std::endl;
    output << "\taddress: " << addr.addr << "," << std::endl;
    output << "\toperation: " << addr.operation << "," << std::endl;
    output << "\ttag: " << addr.tag << "," << std::endl;
    output << "\tindex: " << addr.index << "," << std::endl;
    output << "\toffset: " << addr.offset << std::endl;
    output << "}" <<std::endl;
    
    return output;
}

std::ostream& utils::operator << (std::ostream &output, utils::block block)
{
    output << "(V: " << block.valid << ", T: " << block.tag << ", S: " << block.sequence_number << ")";
    return output;
}