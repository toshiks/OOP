/**
 * @file IniParserExceptions.cpp
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 08.09.2017
 * @version 1.0
 */
#include "IniParserException.h"

IniParserException::IniParserException (const std::string &message) noexcept : message(message) { }

IniParserException::~IniParserException () noexcept { }

const char* IniParserException::what () const noexcept
{
    return message.c_str();
}

FileDoesNotExistException::FileDoesNotExistException (const std::string &message) noexcept : IniParserException(message) { }

FileFormatException::FileFormatException (const std::string &message) noexcept : IniParserException(message) { }

InvalidArgumentException::InvalidArgumentException (const std::string &message) noexcept : IniParserException(message) { }

InvalidTypeException::InvalidTypeException (const std::string &message) noexcept : IniParserException(message) { }