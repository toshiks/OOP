/**
 * @file IniParser.cpp
 * Realization of parsing of ini-file.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 08.09.2017
 * @version 1.0
 */

#include <string>
#include <regex>
#include <iostream>

#include "IniParser.h"

IniParser::IniParser () : parserLogger_("LOGGER", "E:\\CLionProjects\\OOP\\OOP\\OOP_1\\logger.txt")
{ }

IniParser::~IniParser ()
{ }

void IniParser::initial (const std::string &fileName) const noexcept(false)
{
    std::ifstream in(fileName);

    if ( !in.is_open() || !in.good()) {
        std::string message = "File: '" + fileName + "' doesn't exist!\n";

        parserLogger_.log(logger::Level::FINEST, message);
        throw FileDoesNotExistException(message);
    }

    parserLogger_.log(logger::Level::INFO, "File: '" + fileName + "' opened.\n");
    parseFile(in);

    in.close();
    parserLogger_.log(logger::Level::INFO, "File: '" + fileName + "' closed.\n");
}

std::string IniParser::generateStringExceptionMessage(const int &count, std::string type) const
{
    std::string message = "ERROR IN " + std::to_string(count) + " LINE.\n" + type + " INCORRECT!\n";

    parserLogger_.log(logger::Level::FINE, message);

    return message;
}

void IniParser::parseFile (std::ifstream &file) const noexcept(false)
{
    std::regex isComments("^\\s*;.*");
    std::regex isFailComment("^.*;.*");
    std::regex isFailLine("[^\\s]+");
    std::regex isRightSection("\\s*\\[([\\dA-Za-z_]+)\\]\\s*.*");
    std::regex isRightParameter("\\s*([\\dA-Za-z_]+)\\s*=\\s*([\\dA-Za-z_./]+)\\s*(;+.*)*");
    std::regex isAllSection(".*\\[(.*)\\].*");
    std::regex isAllParameter(".*=.*");

    std::string str;
    std::smatch temp;

    int count = 0;

    std::string currentSectionName;
    std::multimap < std::string, std::string > currentSectionParam;


    while ( !file.eof() && file.good() ) {
        std::getline(file, str, '\n');
        parserLogger_.log(logger::Level::INFO, "read line: '" + str + "'\n");
        count++;

        if ( std::regex_match(str, isComments)) {
            continue;
        }
        if ( std::regex_match(str, temp, isRightSection)) {

            if ( !currentSectionParam.empty()) {

                this->data_.insert(make_pair(currentSectionName, currentSectionParam));
                currentSectionParam.clear();
            }

            currentSectionName = temp[ 1 ];

            parserLogger_.log(logger::Level::INFO, "Find section: '" + temp[ 1 ].str() + "'\n");

        } else if ( std::regex_match(str, temp, isRightParameter)) {

            currentSectionParam.insert(make_pair(temp[ 1 ], temp[ 2 ]));

            parserLogger_.log(logger::Level::INFO,
                             "Find pair: '" + temp[ 1 ].str() + "' and '" + temp[ 2 ].str() + "'\n");

        } else if ( std::regex_match(str, isAllSection)) {

            throw FileFormatException(generateStringExceptionMessage(count, "SECTION NAME"));

        } else if ( regex_match(str, isAllParameter)) {

            throw FileFormatException(generateStringExceptionMessage(count, "PARAMETERS"));

        } else if( regex_match(str, isFailComment)){

            throw FileFormatException( generateStringExceptionMessage(count, "COMMENT"));
        } else if( regex_match(str, isFailLine)){

            throw FileFormatException(generateStringExceptionMessage(count, "LINE"));
        }
    }

    if ( !currentSectionParam.empty()) {

        this->data_.insert(make_pair(currentSectionName, currentSectionParam));
        currentSectionParam.clear();
    }
}

bool IniParser::isRightWord (const std::string &word) const
{
    std::regex isSectionRight("[\\dA-Za-z_]+");

    if (!std::regex_match(word, isSectionRight)){
        std::string message = "Argument '" + word + "' incorrect";

        throw InvalidArgumentException(message);
    }

    return true;
}

bool IniParser::isHaveSection (const std::string &sectionName) const
{
    isRightWord(sectionName);

    return this->data_.find(sectionName) != this->data_.end();
}

bool IniParser::isHaveParam (const std::string &sectionName, const std::string &paramName) const
{
    if (!isHaveSection(sectionName)){
        return false;
    }

    isRightWord(paramName);

    std::multimap<std::string, std::string> &temp = this->data_.find(sectionName)->second;

    return temp.find(paramName) != temp.end();
}

std::string IniParser::getStr (const std::string &sectionName, const std::string &paramName, bool isNumber, const char *typeName) const
{
    if ( !isHaveParam(sectionName, paramName)) {
        throw InvalidArgumentException("Section '" + sectionName + "' and parameter '" + paramName + "' not found!");
    }

    std::string str = this->data_.find(sectionName)->second.find(paramName)->second;

    int countPointer = 0;
    bool flag = false;

    for (auto &i: str){
        if (std::isalpha(i) || i == '/') {
            flag = true;
            break;
        }
        if (i == '.'){
            if (countPointer > 1){
                flag = true;
                break;
            }
            countPointer++;
        }
    }

    if (isNumber && flag){
        std::string message(typeName);
        message += " is inavalid type!\n";

        throw InvalidTypeException(message);
    }

    return str;
}

template <>
int IniParser::getValue (const std::string &sectionName, const std::string &paramName) const
{
    return std::stoi(this->getStr(sectionName, paramName, true, "int"));
}

template <>
float IniParser::getValue (const std::string &sectionName, const std::string &paramName) const
{
    return std::stof(this->getStr(sectionName, paramName, true, "float"));
}

template <>
double IniParser::getValue (const std::string &sectionName, const std::string &paramName) const
{
    return std::stod(this->getStr(sectionName, paramName, true, "double"));
}

template <>
std::string IniParser::getValue (const std::string &sectionName, const std::string &paramName) const
{
    return this->getStr(sectionName, paramName, false, "std::string");
}

template <>
const char * IniParser::getValue (const std::string &sectionName, const std::string &paramName) const
{
    return this->getStr(sectionName, paramName, false, "const char *").c_str();
}