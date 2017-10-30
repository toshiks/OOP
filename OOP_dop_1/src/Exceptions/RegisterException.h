/**
 * @file RegisterExceptions.h
 * Create interface of exceptions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 29.10.2017
 * @version 1.0
 */

#ifndef OOP_DOP_1_REGISTEREXCEPTION_H
#define OOP_DOP_1_REGISTEREXCEPTION_H

#include <exception>
#include <string>

namespace my_register {
class RegisterException : public std::exception {
 public:
  explicit RegisterException(const std::string &message) noexcept;

  ~RegisterException() noexcept;

  const char *what() const noexcept override;

 private:
  std::string message_;

};

class ReaderRegisterException final : public RegisterException {
 public:
  explicit ReaderRegisterException(const std::string &message) noexcept;
};

class StorageRegisterException final : public RegisterException {
 public:
  explicit StorageRegisterException(const std::string &message) noexcept;
};

}

#endif //OOP_DOP_1_REGISTEREXCEPTION_H
