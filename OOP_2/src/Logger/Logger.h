/**
 * @file Logger.h
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 07.09.2017
 * @version 1.0
 */
#ifndef OOP_1_LOGGER_H
#define OOP_1_LOGGER_H

#include <fstream>

namespace logger
{
    // Levels of log message
    enum Level{
        // Low level
        SEVERE,
        WARNING,
        INFO,
        CONFIG,
        FINE,
        FINER,
        // Hard level
        FINEST
    };


    class Logger
    {
        public:
            explicit Logger(std::string loggerName, std::string fileName, Level level);
            explicit Logger(std::string loggerName, std::string fileName);
            ~Logger ();

            void setLevel(Level level);
            void setFileName (const std::string &fileName);

            // Write log message in file
            void log(Level level, const std::string &message) const;

        protected:
            // Open file
            void openFile(const std::string &fileName);

        private:
            // Level for
            Level level;
            std::string loggerName;
            mutable std::ofstream file;
    };

}


#endif //OOP_1_LOGGER_H
