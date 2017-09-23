/**
 * @file IniParserExceptions.cpp
 * Realisation classes of exceptions for class StatisticMultiset
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 14.09.2017
 * @version 1.0
 */
#include "StatisticMultisetException.h"

StatisticMultisetException::StatisticMultisetException (const std::string &message) noexcept : message(message) { }

StatisticMultisetException::~StatisticMultisetException () noexcept { }

const char* StatisticMultisetException::what () const noexcept
{
    return message.c_str();
}

FileDoesNotExistException::FileDoesNotExistException (const std::string &message) noexcept : StatisticMultisetException(message) { }

FileFormatException::FileFormatException (const std::string &message) noexcept : StatisticMultisetException(message) { }

InvalidArgumentException::InvalidArgumentException (const std::string &message) noexcept : StatisticMultisetException(message) { }

InvalidTypeException::InvalidTypeException (const std::string &message) noexcept : StatisticMultisetException(message) { }