/**
 * @file RegisterReader.cpp
 * Create realization of interface for reading registers from file.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 29.10.2017
 * @version 1.0
 */
#include <fstream>
#include <bits/unique_ptr.h>

#include "RegisterReader.h"
#include "../Exceptions/RegisterException.h"

using namespace my_register;

auto RegisterReader::openFile (const std::string &fileName) const
{
    auto deleter = [] (std::fstream *f) {
        f->close();
        delete f;
    };

    const std::fstream::openmode resume = std::fstream::in | std::fstream::binary | std::fstream::ate;

    std::unique_ptr< std::fstream, decltype(deleter) > file (
            new std::fstream(fileName.c_str(), resume),
            deleter);

    if (!file.get()->is_open()){
        throw new ReaderRegisterException("File '" + fileName + "' doesn't exist");
    }

    return file;
}