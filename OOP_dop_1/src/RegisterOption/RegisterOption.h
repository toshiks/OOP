/**
 * @file RegisterOption.h
 * Class for description of register.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#ifndef OOP_DOP_1_REGISTEROPTION_H
#define OOP_DOP_1_REGISTEROPTION_H

#include <string>
#include <vector>

namespace my_register {
class RegisterOption {
 public:
  explicit RegisterOption();

  ~RegisterOption() = default;

  bool isPreambula() const;
  bool isPostambula() const;

  std::string getName() const;
  std::string getAddressRegister_1() const;
  std::string getAddressRegister_2() const;
  std::string getCommand() const;

  void setName(const std::string &name);
  void setAddressRegister_1(const std::string &addressRegister_1);
  void setAddressRegister_2(const std::string &addressRegister_1);
  void setCommand(const std::string &command);

 private:
  std::string name_;
  std::vector<std::string> addressRegister_;
  std::string command_;
};

}

#endif //OOP_DOP_1_REGISTEROPTION_H
