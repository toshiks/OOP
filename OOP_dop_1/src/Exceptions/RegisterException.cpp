/**
 * @file RegisterExceptions.cpp
 * Create realization of exceptions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 29.10.2017
 * @version 1.0
 */

#include "RegisterException.h"
using namespace my_register;

RegisterException::RegisterException(const std::string &message) noexcept : message_(message) {}

RegisterException::~RegisterException() noexcept {}

const char *RegisterException::what() const noexcept {
  return message_.c_str();
}

ReaderRegisterException::ReaderRegisterException(const std::string &message) noexcept : RegisterException(message) {}

StorageRegisterException::StorageRegisterException(const std::string &message) noexcept : RegisterException(message) {}
