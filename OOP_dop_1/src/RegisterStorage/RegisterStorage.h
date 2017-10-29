/**
 * @file RegisterStorage.h
 * Storage for saving registers in memory.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 30.10.2017
 * @version 1.0
 */

#ifndef OOP_DOP_1_REGISTERSTORAGE_H
#define OOP_DOP_1_REGISTERSTORAGE_H

#include <string>
#include <unordered_map>
#include <vector>


namespace my_register
{
#pragma pack(push, 1)
    struct register_option
    {
        std::string name;
        std::string addressRegister[2];
        std::string command;
    };
#pragma pack(pop)

    class RegisterStorage
    {
        public:
            explicit RegisterStorage() = default;

            ~RegisterStorage () = default;

            void emplace(const register_option &registerOption);

            void replace(const register_option &newRegisterOption);

            uint_fast32_t size() const;

            register_option operator [](const uint_fast32_t &n) const;

            void clear();

            bool doesExist(const std::string &registerName) const;

        private:
            std::unordered_map<std::string, uint_fast32_t> registersName_;
            std::vector<register_option> optionsOfRegiter_;

    };

}

#endif //OOP_DOP_1_REGISTERSTORAGE_H
