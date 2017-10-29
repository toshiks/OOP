/**
 * @file RegisterReader.h
 * Create interface for reading registers from file.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 29.10.2017
 * @version 1.0
 */
#ifndef OOP_DOP_1_REGISTERREADER_H
#define OOP_DOP_1_REGISTERREADER_H

#include <string>
#include <regex>
#include <bits/shared_ptr.h>

#include "../RegisterStorage/RegisterStorage.h"

namespace my_register
{
    class RegisterReader final
    {
        public:
            explicit RegisterReader () = default;

            ~RegisterReader () = default;

            RegisterReader (const RegisterReader &) = delete;


            void readFile (const std::string &fileName, RegisterStorage &storage) const;

        private:
            auto openFile (const std::string &fileName) const;

            void validString (const std::string &str, RegisterStorage &storage) const;

            inline void cleanString (std::string &str) const;

            const std::shared_ptr < std::smatch > parseString (std::string &str, const std::string &oldStr) const;

            void dataDistribution (const std::smatch &parsedData, const std::string &oldStr,
                                   RegisterStorage &storage) const;


            mutable bool _readingINIT = false;

    };

}


#endif //OOP_DOP_1_REGISTERREADER_H
