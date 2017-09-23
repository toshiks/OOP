/**
 * @file IniParser.h
 * Interface for parsing file
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 08.09.2017
 * @version 1.0
 */
#ifndef OOP_1_INIPARSER_H
#define OOP_1_INIPARSER_H

#include <fstream>
#include <string>
#include <map>
#include <typeinfo>

#include "../Logger/Logger.h"
#include "../IniParserException/IniParserException.h"

class IniParser final
{
    public:
        IniParser ();

        ~IniParser ();

        //Open ini file and parse it
        void initial (const std::string &fileName) const noexcept(false);

        // Check section name
        bool isHaveSection (const std::string &sectionName) const noexcept(false);

        // Check parameter name
        bool isHaveParam (const std::string &sectionName, const std::string &paramName) const noexcept(false);

        // Return value of parameter from section
        template < class _Tp >
        _Tp getValue (const std::string &sectionName, const std::string &paramName) const noexcept(false);


    protected:
        // Parsing file
        void parseFile (std::ifstream &file) const noexcept(false);

        // Check word
        bool isRightWord (const std::string &word) const noexcept(false);



    private:
        std::string generateStringExceptionMessage(const int &count, std::string type) const;

        std::string getStr(const std::string &sectionName, const std::string &paramName, bool isNumber, const char * typeName) const noexcept(false);

        mutable std::multimap < std::string, std::multimap < std::string, std::string > > _data;
        mutable logger::Logger _parserLogger;

};
#endif //OOP_1_INIPARSER_H
