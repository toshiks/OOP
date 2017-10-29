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
#include <map>
#include <unordered_map>
#include <regex>
#include <bits/shared_ptr.h>

namespace my_register
{
#pragma pack(push, 1)
    struct register_option
    {
        std::string addressRegister[2];
        std::string command;
    };
#pragma pack(pop)


    class RegisterReader final
    {
        public:
            explicit RegisterReader () = default;

            ~RegisterReader () = default;

            RegisterReader (const RegisterReader &) = delete;


            void readFile (const std::string &fileName, std::unordered_map < std::string, register_option > &data) const;

        private:
            auto openFile (const std::string &fileName) const;

            void validString (const std::string &str, std::unordered_map < std::string, register_option > &data) const;

            inline void cleanString (std::string &str) const;

            const std::shared_ptr < std::smatch > parseString (std::string &str, const std::string &oldStr) const;

            void dataDistribution (const std::smatch &parsedData, const std::string &oldStr,
                                   std::unordered_map < std::string, register_option > &data) const;


            mutable bool _readingINIT = false;

    };

}


#endif //OOP_DOP_1_REGISTERREADER_H
