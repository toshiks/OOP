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

        void save(const std::string &fileName);

        void createFromFile(const std::string &name);

        void convertStereoToMono();

        void makeReverb(const double &delaySeconds, const float &decay);

        void printInfo() const;

        void cutBegin(const double &second);

    private:
        auto getFile(bool state, const std::string &fileName);

        void readHeader();

        void readData();

        void checkHeader(const long &fileSize) const;

        void preFillHeader();

        void fillHeader(const uint_fast16_t &countChannels,const uint_fast16_t &bitsPerSample,
                        const uint_fast32_t &sampleRate, const uint_fast32_t &countSamplesInChannel);

        void dataChecking() const;

        std::unique_ptr<wav_header_s> _header;
        std::string _fileName;

        logger::Logger _logger;

        std::vector<std::vector<int_fast16_t>> _data;


};


#endif //OOP3_WAV_H
