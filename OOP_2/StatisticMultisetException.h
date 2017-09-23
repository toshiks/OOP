/**
 * @file StatisticMultisetException.h
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 19.09.2017
 * @version 1.0
 */
#ifndef OOP_2_STATISTICMULTISETEXCEPTION_H
#define OOP_2_STATISTICMULTISETEXCEPTION_H

#include <exception>
#include <stdexcept>


class StatisticMultisetException : public std::exception
{
    public:
        StatisticMultisetException (const std::string &message) noexcept;

        ~StatisticMultisetException () noexcept;


        const char* what () const noexcept override;

    private:
        std::string message;
};

class FileDoesNotExistException : public StatisticMultisetException
{
    public:
        FileDoesNotExistException (const std::string &message) noexcept;
};


class FileFormatException : public StatisticMultisetException
{
    public:
        FileFormatException (const std::string &message) noexcept;
};

class InvalidArgumentException : public StatisticMultisetException
{
    public:
        InvalidArgumentException (const std::string &message) noexcept;
};

class InvalidTypeException : public StatisticMultisetException
{
    public:
        InvalidTypeException (const std::string &message) noexcept;
};


#endif
