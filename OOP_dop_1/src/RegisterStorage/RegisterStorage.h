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
        std::string addressRegister[2];
        std::string command;
    };
#pragma pack(pop)

    class RegisterStorage
    {
            explicit RegisterStorage() = default;

            ~RegisterStorage () = default;

            void emplace(const std::string &registerName, const register_option &registerOption);

            void replace(const std::string &registerName, const register_option &newRegisterOption);

            const uint_fast32_t size() const;

            const register_option operator [](const uint_fast32_t &n);

            void clear();
        private:
            std::unordered_map<std::string, uint_fast32_t> registersName_;
            std::vector<register_option> optionsOfRegiter_;

    };

}

#endif //OOP_DOP_1_REGISTERSTORAGE_H
