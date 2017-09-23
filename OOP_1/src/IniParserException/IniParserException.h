/**
 * @file IniParserExceptions.h
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 08.09.2017
 * @version 1.0
 */
#ifndef OOP_1_INIPARSEREXCEPTIONS_H
#define OOP_1_INIPARSEREXCEPTIONS_H

#include <exception>
#include <stdexcept>


class IniParserException : public std::exception
{
    public:
        IniParserException(const std::string &message) noexcept ;
        ~IniParserException () noexcept;


        const char* what() const noexcept override;

    private:
        std::string message;
};

class FileDoesNotExistException : public IniParserException
{
    public:
        FileDoesNotExistException(const std::string &message) noexcept;
};


class FileFormatException : public IniParserException
{
    public:
        FileFormatException(const std::string &message) noexcept;
};

class InvalidArgumentException : public IniParserException
{
    public:
        InvalidArgumentException(const std::string &message) noexcept;
};

class InvalidTypeException : public IniParserException
{
    public:
        InvalidTypeException(const std::string &message) noexcept;
};


#endif //OOP_1_INIPARSEREXCEPTIONS_H
