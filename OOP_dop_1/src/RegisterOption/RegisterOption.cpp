/**
 * @file RegisterOption.cpp
 * Class for description of register.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#include <algorithm>

#include "RegisterOption.h"
using namespace my_register;

const std::vector <std::string> preambulaRegistersName_ { "R0", "R1", "R2", "R3", "R4",
                                                          "R5", "R6", "R7", "R8", "R9",
                                                          "R10", "R11", "R12", "R13" };

const std::vector <std::string> postambulaRegistersName_ { "R8189", "R8190", "R8191" };


std::string RegisterOption::getName () const
{
    return this->name_;
}

std::string RegisterOption::getAddressRegister_1 () const
{
    return this->addressRegister_[0];
}

std::string RegisterOption::getAddressRegister_2 () const
{
    return this->addressRegister_[1];
}

std::string RegisterOption::getCommand () const
{
    return this->command_;
}

void RegisterOption::setName (const std::string &name)
{
    this->name_ = name;
}

void RegisterOption::setAddressRegister_1 (const std::string &addressRegister_1)
{
    this->addressRegister_[0] = addressRegister_1;
}

void RegisterOption::setAddressRegister_2 (const std::string &addressRegister_2)
{
    this->addressRegister_[1] = addressRegister_2;
}

void RegisterOption::setCommand (const std::string &command)
{
    this->command_ = command;
}

bool RegisterOption::isPreambula () const
{
    const int temp = std::count(preambulaRegistersName_.begin(),
                                preambulaRegistersName_.end(), this->name_);

    return temp > 0;
}

bool RegisterOption::isPostambula () const
{
    const int temp = std::count(postambulaRegistersName_.begin(),
                                postambulaRegistersName_.end(), this->name_);

    return temp > 0;
}