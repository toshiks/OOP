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
    // Levels of log message_
    enum Level{
        // Low level_
        SEVERE,
        WARNING,
        INFO,
        CONFIG,
        FINE,
        FINER,
        // Hard level_
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

            // Write log message_ in file_
            void log(Level level, const std::string &message) const;

        protected:
            // Open file_
            void openFile(const std::string &fileName);

        private:
            // Level for
            Level level_;
            std::string loggerName_;
            mutable std::ofstream file_;
    };

}


#endif //OOP_1_LOGGER_H
