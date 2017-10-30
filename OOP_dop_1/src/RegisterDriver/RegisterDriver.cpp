/**
 * @file RegisterDriver.cpp
 * Interface for work files with register's information.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */
#include "RegisterDriver.h"
using namespace my_register;

void RegisterDriver::readFile(const std::string &fileName) {
  this->reader_.readFile(fileName, this->storage_);

  writer_.changeRegisterStorage(this->storage_);
}

void RegisterDriver::writeToIniFile(const std::string &fileName) const {
  this->writer_.writeToIniFile(fileName);
}

void RegisterDriver::writeToHeaderFile(const std::string &fileName) const {
  this->writer_.writeToHeaderFile(fileName);
}