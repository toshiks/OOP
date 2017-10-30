/**
 * @file RegisterStorage.h
 * Storage for saving registers in memory.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#ifndef OOP_DOP_1_REGISTERSTORAGE_H
#define OOP_DOP_1_REGISTERSTORAGE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../RegisterOption/RegisterOption.h"

namespace my_register {
class RegisterStorage {
 public:
  explicit RegisterStorage() = default;

  ~RegisterStorage() = default;

  void emplace(const RegisterOption &registerOption);

  void replace(const RegisterOption &newRegisterOption);

  uint_fast32_t size() const;

  RegisterOption operator[](const uint_fast32_t &n) const;

  void clear();

  bool doesExist(const std::string &registerName) const;

 private:
  std::unordered_map<std::string, uint_fast32_t> registersName_;
  std::vector<RegisterOption> optionsOfRegiter_;

};

}

#endif //OOP_DOP_1_REGISTERSTORAGE_H
