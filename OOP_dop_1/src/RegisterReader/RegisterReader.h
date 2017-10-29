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

namespace my_register
{
    #pragma pack(push, 1)
    struct registerOption
    {
        char addressRegister[2];
        char command;
    };
    #pragma pack(pop)


    class RegisterReader final
    {
        public:
            explicit RegisterReader() = default;

            ~RegisterReader () = default;

            RegisterReader(const RegisterReader&) = delete;


            void readFile(const std::string &fileName, std::map<std::string, registerOption> &data) const;

        private:
            auto openFile(const std::string &fileName) const;

            bool _readingINIT;

    };

}


#endif //OOP_DOP_1_REGISTERREADER_H
