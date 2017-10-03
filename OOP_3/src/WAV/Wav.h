/**
 * @file wav.h
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 02.10.2017
 * @version 1.0
 */
#ifndef OOP3_WAV_H
#define OOP3_WAV_H

#include <string>
#include <vector>
#include <bits/unique_ptr.h>
#include "../Logger/Logger.h"

#include "../wav_header.h"

class Wav
{
    public:
        Wav();

        void createFromFile(const std::string &name);

    private:
        auto getFile();

        void readHeader();

        void readData();

        void checkHeader(const long &fileSize) const;

        std::unique_ptr<wav_header_s> _header;
        std::string _fileName;

        logger::Logger _logger;

        std::vector<std::vector<uint_fast16_t>> _data;


};


#endif //OOP3_WAV_H
