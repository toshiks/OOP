/**
 * @file RegisterDriver.h
 * Interface for work files with register's information.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */
#ifndef OOP_DOP_1_REGISTERDRIVER_H
#define OOP_DOP_1_REGISTERDRIVER_H

#include <string>

#include "../RegisterWriter/RegisterWriter.h"
#include "../RegisterReader/RegisterReader.h"
#include "../RegisterStorage/RegisterStorage.h"
#include "../Exceptions/RegisterException.h"

namespace my_register {
class RegisterDriver final {
 public:
  explicit RegisterDriver() = default;

  ~RegisterDriver() = default;

  RegisterDriver(const RegisterDriver &) = delete;

  void readFile(const std::string &fileName);

  void writeToIniFile(const std::string &fileName) const;

  void writeToHeaderFile(const std::string &fileName) const;
 private:
  RegisterStorage storage_;
  RegisterReader reader_;
  RegisterWriter writer_;

};
}

#endif //OOP_DOP_1_REGISTERDRIVER_H
