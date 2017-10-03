/**
 * @file WavException.cpp
 * Realisation classes of exceptions for class Wav
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 14.09.2017
 * @version 1.0
 */
#include "WavException.h"

WavException::WavException (const std::string &message) noexcept : message(message) { }

WavException::~WavException () noexcept { }

const char* WavException::what () const noexcept
{
    return message.c_str();
}

FileDoesNotExistException::FileDoesNotExistException (const std::string &message) noexcept : WavException(message) { }

FileFormatException::FileFormatException (const std::string &message) noexcept : WavException(message) { }

InvalidArgumentException::InvalidArgumentException (const std::string &message) noexcept : WavException(message) { }

InvalidTypeException::InvalidTypeException (const std::string &message) noexcept : WavException(message) { }

WavIOException::WavIOException (const std::string &message) noexcept : WavException(message) { }

WavHeaderException::WavHeaderException (const std::string &message) noexcept : WavIOException(message) { }

HeaderSymbolsException::HeaderSymbolsException (const std::string &message) noexcept : WavHeaderException(message) { }

HeaderSizeException::HeaderSizeException (const std::string &message) noexcept : WavHeaderException(message) { }

AudioFormatException::AudioFormatException (const std::string &message) noexcept : WavHeaderException(message) { }