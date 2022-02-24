#include "ArgumentWrapper.h"
#include <string>

ArgumentWrapper::ArgumentWrapper (
    unsigned int block_size, 
    unsigned int l1_size, 
    unsigned int l1_associativity,
    unsigned int l2_size,
    unsigned int l2_associativity,
    unsigned int replacement_policy,
    unsigned int inclusion_property,
    std::string trace_file)
{
    this->block_size = block_size;
    this->l1_size = l1_size;
    this->l1_associativity = l1_associativity;
    this->l2_size = l2_size;
    this->l2_associativity = l2_associativity;
    this->replacement_policy = replacement_policy;
    this->inclusion_property = inclusion_property;
    this->trace_file = trace_file;
}

unsigned int ArgumentWrapper::get_block_size(void)
{
    return this->block_size;
}

unsigned int ArgumentWrapper::get_l1_size(void)
{
    return this->l1_size;
}

unsigned int ArgumentWrapper::get_l1_associativity(void)
{
    return this->l1_associativity;
}

unsigned int ArgumentWrapper::get_l2_size(void)
{
    return this->l2_size;
}

unsigned int ArgumentWrapper::get_l2_associativity(void)
{
    return this->l2_associativity;
}

std::string ArgumentWrapper::get_replacement_policy(void)
{
    if (this->replacement_policy == 0)
    {
        return "LRU";
    }
    else if (this->replacement_policy == 1)
    {
        return "PLRU";
    }
    else
    {
        return "Optimal";
    }
}

std::string ArgumentWrapper::get_inclusion_property(void)
{
    if (this->inclusion_property == 0)
    {
        return "non-inclusive";
    }
    else
    {
        return "inclusive";
    }
}

std::string ArgumentWrapper::get_trace_file(void)
{
    return this->trace_file;
}

std::ostream& operator << (std::ostream &output, ArgumentWrapper argument_wrapper)
{
        output << "ArgumentWrapper: \n{" << std::endl;
        output << "\tBLOCK_SIZE: " << (argument_wrapper.get_block_size()) << " bytes," << std::endl;
        output << "\tL1_SIZE: " << (argument_wrapper.get_l1_size()) << " bytes," << std::endl;
        output << "\tL1_ASSOC: " << (argument_wrapper.get_l1_associativity()) << "," << std::endl;
        output << "\tL2_SIZE: " << (argument_wrapper.get_l2_size()) << " bytes," << std::endl;
        output << "\tL2_ASSOC: " << (argument_wrapper.get_l2_associativity()) << "," << std::endl;
        output << "\tREPLACEMENT_POLICY: " << (argument_wrapper.get_replacement_policy()) << "," << std::endl;
        output << "\tINCLUSION_POLICY: " << (argument_wrapper.get_inclusion_property()) << "," << std::endl;
        output << "\ttrace_file: " << (argument_wrapper.get_trace_file()) << std::endl;
        output << "}" << std::endl;

        return output;
}