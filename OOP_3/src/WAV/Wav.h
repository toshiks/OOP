/**
 * @file wav.h
 * Class for work with wav-files.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 02.10.2017
 * @version 1.0
 */

#ifndef OOP3_WAV_H
#define OOP3_WAV_H

#include <string>
#include <bits/unique_ptr.h>
#include <deque>
#include <vector>

#include "../Logger/Logger.h"
#include "WavHeader.h"

class Wav
{
    public:
        Wav ();

        void createFromFile (const std::string &name);

        void save (const std::string &fileName);

        void makeReverb (const float &delaySeconds, const float &decay);

        void convertStereoToMono ();

        void cutBegin (const float &second);
        void cutEnd   (const float &second);

        std::string   getInfo          () const;
        uint_fast16_t getNumChannels   () const;
        uint_fast32_t getSampleRate    () const;
        uint_fast32_t getByteRate      () const;
        uint_fast16_t getBitsPerSample () const;

    private:
        auto getFile (bool state, const std::string &fileName) const;

        void readHeader ();

        void readData ();

        void checkHeader (const long &fileSize) const;

        void preFillHeader ();
        void fillHeader    (const uint_fast16_t &countChannels, const uint_fast16_t &bitsPerSample,
                            const uint_fast32_t &sampleRate, const uint_fast32_t &countSamplesInChannel);

        void dataChecking () const;

        void cut (const float &second, bool state);

        std::unique_ptr < WavHeader_s > header_;

        std::string fileName_;

        logger::Logger logger_;

        std::deque < std::deque < int_fast16_t>> data_;


};


#endif //OOP3_WAV_H
