/**
 * @file RegisterReader.cpp
 * Create realization of interface for reading registers from file.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 29.10.2017
 * @version 1.0
 */

#include <fstream>
#include <iostream>
#include <algorithm>
#include <bits/unique_ptr.h>

#include "RegisterReader.h"
#include "../Exceptions/RegisterException.h"

using namespace my_register;

auto RegisterReader::openFile (const std::string &fileName) const
{
    const auto deleter = [ & ] (std::ifstream* f) {
        f->close();
        delete f;
    };


    std::unique_ptr < std::ifstream, decltype(deleter) > file(
            new std::ifstream(fileName.c_str()),
            deleter
    );

    if ( !file.get()->is_open()) {
        throw ReaderRegisterException("File '" + fileName + "' doesn't exist");
    }

    return file;
}

void RegisterReader::readFile (const std::string &fileName, RegisterStorage &storage) const
{
    storage.clear();

    const auto file = this->openFile(fileName);
    std::ifstream &file_r = *file.get();

    std::string line;

    while ( file_r.good()) {
        std::getline(file_r, line);
        this->validString(line, storage);
    }
}

inline void RegisterReader::cleanString (std::string &str) const
{
    str.erase(
            std::remove_if(str.begin(), str.end(), isspace),
            str.end()
    );
}

const std::shared_ptr < std::smatch > RegisterReader::parseString (std::string &str, const std::string &oldStr) const
{
    const std::regex regExpr("^(R[0-9]+)(.*)0x([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

    auto temp = std::make_shared < std::smatch >();

    if ( !std::regex_match(str, *temp.get(), regExpr)) {
        throw ReaderRegisterException("Line: '" + oldStr + "' doesn't valid!");
    }

    return temp;
}

void RegisterReader::dataDistribution (const std::smatch &parsedData, const std::string &oldStr,
                                       RegisterStorage &storage) const
{
    if ( parsedData.size() < 5 || parsedData.size() > 6 ) {
        throw ReaderRegisterException("Line: '" + oldStr + "' doesn't valid!");
    }

    std::string nameRegister = parsedData[ 1 ];
    register_option registerOption;

    registerOption.name = nameRegister;

    if ( parsedData.size() == 6 ) {
        if (parsedData[2] != "") {
            if ( parsedData[ 2 ] == "(INIT)" && !this->_readingINIT ) {
                this->_readingINIT = true;
            } else {
                throw ReaderRegisterException("Line: '" + oldStr + "'. Expression (INIT) met more than 1 time.");
            }
        }

        registerOption.addressRegister[ 0 ] = parsedData[ 3 ];
        registerOption.addressRegister[ 1 ] = parsedData[ 4 ];
        registerOption.command = parsedData[ 5 ];
    } else {
        registerOption.addressRegister[ 0 ] = parsedData[ 2 ];
        registerOption.addressRegister[ 1 ] = parsedData[ 3 ];
        registerOption.command = parsedData[ 4 ];
    }

    if ( storage.doesExist(nameRegister) ) {
        std::cerr << "WARNING: Register '" << nameRegister << "' repeated. The last value is written.";
        storage.replace(registerOption);
    } else {
        storage.emplace(registerOption);
    }
}

void RegisterReader::validString (const std::string &str, RegisterStorage &storage) const
{
    std::string workStr = str;

    cleanString(workStr);

    if ( workStr.size() == 0 ) {
        return;
    }

    if ( workStr[ 0 ] != 'R' ) {
        throw ReaderRegisterException("Mistake in line: '" + str + "'. Register name doesn't support.");
    }

    const std::shared_ptr < std::smatch > dataFromStr = this->parseString(workStr, str);

    this->dataDistribution(*dataFromStr.get(), str, storage);
}