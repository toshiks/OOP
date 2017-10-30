/**
 * @file RegisterWriter.cpp
 * Class for writing information about registers in headers and ini files.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#include <fstream>
#include <iostream>
#include <bits/unique_ptr.h>

#include "RegisterWriter.h"
#include "../Exceptions/RegisterException.h"
using namespace my_register;

const std::vector<std::vector<std::string> > preambula{{"00", "00", "80"},
                                                       {"00", "00", "00"}};

const std::vector<std::vector<std::string> > postambula{{"01", "43", "D1"},
                                                        {"01", "43", "F1"},
                                                        {"01", "43", "D1"},
                                                        {"01", "44", "FF"},
                                                        {"01", "43", "51"},
                                                        {"01", "43", "52"},
                                                        {"01", "39", "02"}};

RegisterWriter::RegisterWriter(const RegisterStorage &storage) {
  this->changeRegisterStorage(storage);
}

auto RegisterWriter::openFile(const std::string &fileName) const {
  auto deleter = [](std::ofstream *f) {
    f->close();
    delete f;
  };

  std::unique_ptr<std::ofstream, decltype(deleter)> file(
      new std::ofstream(fileName),
      deleter
  );

  if (!file.get()->is_open()) {
    throw WriterRegisterException("File '" + fileName + "' didn't open");
  }

  return file;
}

void RegisterWriter::writeToHeaderFile(const std::string &fileName) const {
  this->writeToFile(fileName, this->writeStringHeader_);
}

void RegisterWriter::writeToIniFile(const std::string &fileName) const {
  this->writeToFile(fileName, this->writeStringIni_);
}

void RegisterWriter::writeToFile(const std::string &fileName, const std::string &str) const {
  const auto file = this->openFile(fileName);
  std::ofstream &out = *file.get();

  out << str;
}

std::string RegisterWriter::stringFromParametrsForHeader(const std::string &address_1,
                                                         const std::string &address_2,
                                                         const std::string &command) const {
  const std::string spaces = "     ";
  std::string tmp = spaces + "{ ";

  tmp += "0x" + address_1 + ", ";
  tmp += "0x" + address_2 + ", ";
  tmp += "0x" + command + " ";

  tmp += "},";

  return tmp;
}

void RegisterWriter::changeRegisterStorage(const RegisterStorage &storage) {
  createStringHeaderFromRegisterStorage(storage);
  createStringIniFromRegisterStorage(storage);
}

void RegisterWriter::createStringHeaderFromRegisterStorage(const RegisterStorage &storage) {
  int count = 0;
  this->writeStringHeader_ += "const uint8_t LmkConfigCmds[numCommands][3]={\n";

  if (storage[0].isPreambula()) {
    for (const auto &reg : preambula) {
      count += 1;
      this->writeStringHeader_ += this->stringFromParametrsForHeader(reg[0], reg[1], reg[2]) + "\n";
    }
  }

  for (size_t i = 0; i < storage.size(); ++i) {
    const auto &temp = storage[i];

    if ((!temp.isPreambula()) && (!temp.isPostambula())) {
      count += 1;
      this->writeStringHeader_ += this->stringFromParametrsForHeader(temp.getAddressRegister_1(),
                                                                     temp.getAddressRegister_2(),
                                                                     temp.getCommand()) + "\n";
    }
  }

  const auto &lastStorageElement = storage[storage.size() - 1];

  if (lastStorageElement.isPostambula()) {
    for (const auto &reg : postambula) {
      count += 1;
      this->writeStringHeader_ += this->stringFromParametrsForHeader(reg[0], reg[1], reg[2]) + "\n";
    }
  }

  this->writeStringHeader_.pop_back();
  this->writeStringHeader_.pop_back();
  this->writeStringHeader_ += "\n};";

  this->writeStringHeader_ = "const uint32_t numCommands = " + std::to_string(count) + ";\n" + this->writeStringHeader_;
}

void RegisterWriter::createStringIniFromRegisterStorage(const RegisterStorage &storage) {
  if (storage[0].isPreambula()) {

    for (const auto &reg : preambula) {

      for (const auto &opt: reg) {
        this->writeStringIni_ += "0x" + opt + ", ";
      }
      this->writeStringIni_ += "\n";
    }
  }

  for (size_t i = 0; i < storage.size(); ++i) {
    const auto &temp = storage[i];

    if ((!temp.isPreambula()) && (!temp.isPostambula())) {
      this->writeStringIni_ += "0x" + temp.getAddressRegister_1() + ", ";
      this->writeStringIni_ += "0x" + temp.getAddressRegister_2() + ", ";
      this->writeStringIni_ += "0x" + temp.getCommand() + ",\n";
    }
  }

  const auto &lastStorageElement = storage[storage.size() - 1];

  if (lastStorageElement.isPostambula()) {

    for (const auto &reg : postambula) {

      for (const auto &opt: reg) {
        this->writeStringIni_ += "0x" + opt + ", ";
      }
      this->writeStringIni_ += "\n";
    }
  }

  this->writeStringIni_.pop_back();
  this->writeStringIni_.pop_back();
  this->writeStringIni_.pop_back();
}