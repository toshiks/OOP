/**
 * @file Logger.cpp
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 07.09.2017
 * @version 1.0
 */
#include "Logger.h"
#include <iostream>
using namespace logger;

Logger::Logger (const std::string &loggerName, const std::string &fileName, Level level) : level_(level),
                                                                             loggerName_(loggerName),
                                                                             file_(fileName) { }

Logger::Logger (const std::string &loggerName, const std::string &fileName): Logger(loggerName, fileName, Level::SEVERE) { }

Logger::~Logger ()
{
    this->file_.close();
}

void Logger::setLevel (Level level)
{
    this->level_ = level;
}

void Logger::setFileName (const std::string &fileName)
{
    openFile(fileName);
}

void Logger::openFile (const std::string &fileName)
{
    if (this->file_.is_open())
        this->file_.close();

    try{
        this->file_.open(fileName);
    }
    catch (std::exception &e){
        std::cerr << "Exception error in logger '" << loggerName_ << "'\nMessage: " << e.what();
    }
}

void Logger::log (Level level, const std::string &message) const
{
    if (level < this->level_)
        return;

    std::string logMessage = this->loggerName_ + "::";

    switch (level){
        case SEVERE:
            logMessage += "SEVERE: ";
            break;
        case WARNING:
            logMessage += "WARNING: ";
            break;
        case INFO:
            logMessage += "INFO: ";
            break;
        case CONFIG:
            logMessage += "CONFIG: ";
            break;
        case FINE:
            logMessage += "FINE: ";
            break;
        case FINER:
            logMessage += "FINER: ";
            break;
        case FINEST:
            logMessage += "FINEST: ";
            break;
        default:
            break;
    }

    logMessage += message;
    logMessage += "\n";

    this->file_ << logMessage;
}