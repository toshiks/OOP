/**
 * @file RegisterStorage.cpp
 * Storage for saving registers in memory.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#include "RegisterStorage.h"
#include "../Exceptions/RegisterException.h"
using namespace my_register;

void RegisterStorage::emplace (const std::string &registerName, const register_option &registerOption)
{
    if ( this->registersName_.find(registerName) == this->registersName_.end() ){
        return;
    }

    const auto index = this->optionsOfRegiter_.size();

    this->registersName_.emplace(registerName, index);

    this->optionsOfRegiter_.emplace_back(registerOption);
}

void RegisterStorage::replace (const std::string &registerName, const register_option &newRegisterOption)
{
    const auto temp = this->registersName_.find(registerName);

    if ( temp == this->registersName_.end() ){
        return;
    }

    this->optionsOfRegiter_[temp->second] = newRegisterOption;
}

const uint_fast32_t RegisterStorage::size () const
{
    return this->optionsOfRegiter_.size();
}

const register_option RegisterStorage::operator[] (const uint_fast32_t &n)
{
    if (n >= this->optionsOfRegiter_.size()){
        throw StorageRegisterException("Array index out of bounds");
    }

    return this->optionsOfRegiter_[n];
}

void RegisterStorage::clear ()
{
    this->optionsOfRegiter_.clear();
    this->registersName_.clear();
}