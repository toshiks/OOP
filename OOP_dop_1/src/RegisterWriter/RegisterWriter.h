/**
 * @file RegisterWriter.h
 * Class for writing information about registers in headers and ini files.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#ifndef OOP_DOP_1_REGISTERWRITER_H
#define OOP_DOP_1_REGISTERWRITER_H

#include <string>

#include "../RegisterStorage/RegisterStorage.h"

namespace my_register {
class RegisterWriter final {
 public:
  explicit RegisterWriter(const RegisterStorage &storage);

  ~RegisterWriter() = default;

  RegisterWriter(const RegisterWriter &) = delete;

  void changeRegisterStorage(const RegisterStorage &storage);

  void writeToIniFile(const std::string &fileName) const;

  void writeToHeaderFile(const std::string &fileName) const;

 private:

  auto openFile(const std::string &fileName) const;

  std::string stringFromParametrsForHeader(const std::string &address_1,
                                           const std::string &address_2,
                                           const std::string &command) const ;

  void createStringHeaderFromRegisterStorage(const RegisterStorage &storage);

  void createStringIniFromRegisterStorage(const RegisterStorage &storage);

  void writeToFile(const std::string &fileName, const std::string &str) const;

  std::string writeStringHeader_;
  std::string writeStringIni_;

};

}

#endif //OOP_DOP_1_REGISTERWRITER_H
