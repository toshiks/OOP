/**
 * @file WavException.h
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 19.09.2017
 * @version 1.0
 */
#ifndef OOP_3_WAVEXCEPTION_H
#define OOP_3_WAVEXCEPTION_H

#include <exception>
#include <stdexcept>


class WavException : public std::exception
{
    public:
        WavException (const std::string &message) noexcept;

        ~WavException () noexcept;


        const char* what () const noexcept override;

    private:
        std::string message;
};

class FileDoesNotExistException : public WavException
{
    public:
        FileDoesNotExistException (const std::string &message) noexcept;
};

class FileFormatException : public WavException
{
    public:
        FileFormatException (const std::string &message) noexcept;
};

class InvalidArgumentException : public WavException
{
    public:
        InvalidArgumentException (const std::string &message) noexcept;
};

class InvalidTypeException : public WavException
{
    public:
        InvalidTypeException (const std::string &message) noexcept;
};

class WavIOException : public WavException
{
    public:
        WavIOException (const std::string &message) noexcept;
};

class WavHeaderException : public WavIOException
{
    public:
        WavHeaderException (const std::string &message) noexcept;
};

class HeaderSymbolsException : public WavHeaderException
{
    public:
        HeaderSymbolsException (const std::string &message) noexcept;
};

class HeaderSizeException : public WavHeaderException
{
    public:
        HeaderSizeException (const std::string &message) noexcept;
};

class AudioFormatException : public WavHeaderException
{
    public:
        AudioFormatException (const std::string &message) noexcept;
};

class WavDataException : public WavException
{
    public:
        WavDataException (const std::string &message) noexcept;
};




#endif
