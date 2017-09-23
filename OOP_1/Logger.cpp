/**
 * @file Logger.cpp
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 07.09.2017
 * @version 1.0
 */
#include "Logger.h"
#include <iostream>
using namespace logger;

Logger::Logger (std::string loggerName, std::string fileName, Level level) : level(level),
                                                                             loggerName(loggerName),
                                                                             file(fileName) { }

Logger::Logger (std::string loggerName, std::string fileName): Logger(loggerName, fileName, Level::SEVERE) { }

Logger::~Logger ()
{
    try {
        this->file.close();
    }
    catch (...)
    { }
}

void Logger::setLevel (Level level)
{
    this->level = level;
}

void Logger::setFileName (const std::string &fileName)
{
    openFile(fileName);
}

void Logger::openFile (const std::string &fileName)
{
    if (this->file.is_open())
        this->file.close();

    try{
        this->file.open(fileName);
    }
    catch (std::ifstream::failure e){
        std::cerr << "Exception error in logger '" << loggerName << "'\nMessage: " << e.what();
    }
}

void Logger::log (Level level, const std::string &message) const
{
    if (level < this->level)
        return;

    std::string logMessage = this->loggerName + "\n";

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
    }

    logMessage += message;

    this->file << logMessage;
}