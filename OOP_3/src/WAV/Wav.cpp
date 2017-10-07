/**
 * @file wav.cpp
 * Realise function for class Wav.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 02.10.2017
 * @version 1.0
 */

#include <cmath>
#include <fstream>
#include <iostream>

#include "Wav.h"
#include "../WavException/WavException.h"

Wav::Wav () : _header(nullptr), _logger("LOGGER", "E:\\CLionProjects\\OOP\\OOP\\OOP_3\\logger.txt")
{ }

/**
 *
 * @param state true - read, false - write
 * @param fileName string of file's name
 * @return unique_ptr with FILE
 */
auto Wav::getFile (bool state, const std::string &fileName)
{
    //For auto-close file
    auto deleter = [ & ] (std::fstream* f) {
        f->close();
        _logger.log(logger::Level::INFO, "File closed.");
    };

    const std::fstream::openmode resume = ( state ?  std::fstream::in :  std::fstream::out )
                                          | std::fstream::binary | std::fstream::ate;

    std::unique_ptr < std::fstream, decltype(deleter) > file(
             new std::fstream(fileName.c_str(),  resume),
             deleter);

    if ( state ) {
        if ( !file.get()->is_open() ) {
            throw FileDoesNotExistException("File " + fileName + " is not exist");
        }
    }

    this->_logger.log(logger::Level::INFO, "Open file " + fileName);

    return file;
}

void Wav::createFromFile (const std::string &name)
{
    this->_logger.log(logger::Level::INFO, ">>>> Read of file ( " + name + " )");
    this->_fileName = name;

    if ( this->_header.get() != nullptr ) {
        this->_header.release();
    }

    this->_header = std::make_unique < WavHeader_s >();

    this->_logger.log(logger::Level::INFO, "Read header.");
    readHeader();
    this->_logger.log(logger::Level::INFO, "The header has been read.");


    this->_logger.log(logger::Level::INFO, "Read data.");
    readData();
    this->_logger.log(logger::Level::INFO, "The data has been read.");
}

void Wav::readHeader ()
{
    const auto file = getFile(true, _fileName);

    file.get()->seekg(0, std::fstream::beg);
    size_t countByte = static_cast<size_t>(file.get()->read(reinterpret_cast<char *>(&(*this->_header.get())),
                                                            sizeof(WavHeader_s)).gcount());

    if ( countByte != 44 ) {
        throw FileFormatException("File " + this->_fileName + " isn't read.");
    }

    file.get()->seekg (0, std::fstream::end);

    checkHeader(static_cast<const long>(file.get()->tellg()));
}

void Wav::readData ()
{
    if ( this->_header->bitsPerSample != 16 ) {
        throw AudioFormatException("Only 16-bit samples is supported");
    }

    const auto file = getFile(true, _fileName);

    std::vector < int_fast16_t > samples;

    try {
        file.get()->seekg(44, std::fstream::beg);
    }
    catch (...){
        throw WavDataException("Wav data isn't valid");
    }

    const auto header = this->_header.get();

    const uint_fast32_t &countChannels        = header->numChannels;
    const uint_fast32_t countSamplesInChannel = ( header->subchunk2Size / sizeof(uint_fast16_t)) / countChannels;

    samples.resize(countSamplesInChannel * countChannels);

    uint_fast32_t readBytes = static_cast<uint_fast32_t>(file.get()->read(reinterpret_cast<char *>(samples.data()),
                                                                          header->subchunk2Size).gcount());

    if ( readBytes != _header.get()->subchunk2Size ) {
        throw WavIOException("readData() read only " + std::to_string(readBytes) +
                             " of " + std::to_string(_header.get()->subchunk2Size));
    }

    _logger.log(logger::Level::INFO, "Data size: " + std::to_string(samples.size()));
    _logger.log(logger::Level::INFO, "Data transform start");

    _data.resize(countChannels);

    for ( size_t i = 0; i < _data.size(); i++ ) {
        _data[ i ].resize(countSamplesInChannel);
    }

    for ( int ch = 0; ch < countChannels; ch++ ) {
        std::deque < int_fast16_t > &chdata = _data[ ch ];

        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            chdata[ i ] = samples[ countChannels * i + ch ];
        }
    }

    _logger.log(logger::Level::INFO, "Data transform end");
}

void Wav::checkHeader (const long &fileSize) const
{
    const auto header = this->_header.get();

    if ( header->chunkId != 0x46464952 ) {
        throw HeaderSymbolsException("'RIFF' doesn't exist");
    }

    if ( header->chunkSize != fileSize - 8 ) {
        throw HeaderSizeException("Chunk size is error");
    }

    if ( header->format != 0x45564157 ) {
        throw HeaderSymbolsException("'WAVE' doesn't exist");
    }

    if ( header->subchunk1Id != 0x20746d66 ) {
        throw HeaderSymbolsException("'fmt ' doesn't exist");
    }

    if ( header->subchunk1Size != 16 ) {
        throw HeaderSizeException("Sub chunk 1 size is error");
    }

    if ( header->audioFormat != 1 ) {
        throw AudioFormatException("File isn't PCM");
    }

    if ( header->byteRate !=
         header->sampleRate * header->numChannels * header->bitsPerSample / 8 ) {
        throw HeaderSizeException("Byte Rate is error");
    }

    if ( header->blockAlign !=
         header->numChannels * header->bitsPerSample / 8 ) {
        throw HeaderSizeException("Block align is error");
    }

    if ( header->subchunk2Id != 0x61746164 ) {
        throw HeaderSymbolsException("'data' doesn't exist");
    }

    if ( header->subchunk2Size != fileSize - 44 ) {
        throw HeaderSizeException("Sub chunk 2 size is error");
    }
}

void Wav::save (const std::string &fileName)
{
    dataChecking();

    const uint_fast16_t countChannels = static_cast<uint_fast16_t>(_data.size());
    const size_t &countSamplesInChannel = _data[ 0 ].size();

    _logger.log(logger::Level::INFO, "Start to save wav file");

    this->fillHeader(countChannels, 16, _header.get()->sampleRate, countSamplesInChannel);

    std::vector < int_fast16_t > transformData;

    transformData.resize(countChannels * countSamplesInChannel);

    for ( uint_fast16_t ch = 0; ch < countChannels; ch++ ) {

        const std::deque < int_fast16_t > &chdata = _data[ ch ];

        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            transformData[ countChannels * i + ch ] = chdata[ i ];
        }
    }

    auto file = getFile(false, fileName);

    file.get()->write(reinterpret_cast<char *>(this->_header.get()), sizeof(WavHeader_s));
    file.get()->write(reinterpret_cast<char *>(transformData.data()), sizeof(int_fast16_t) * transformData.size());

    _logger.log(logger::Level::INFO, "Transform data size: " + std::to_string(transformData.size()));
    _logger.log(logger::Level::INFO, "End to save wav file");
}

void Wav::preFillHeader ()
{
    _logger.log(logger::Level::INFO, "Pre fill header start");

    const auto header = _header.get();

    header->chunkId = 0x46464952;
    header->format  = 0x45564157;

    header->subchunk1Id = 0x20746d66;
    header->subchunk2Id = 0x61746164;

    header->audioFormat   = 1;
    header->subchunk1Size = 16;
    header->bitsPerSample = 16;

    _logger.log(logger::Level::INFO, "Pre fill header end");
}

void Wav::fillHeader (const uint_fast16_t &countChannels, const uint_fast16_t &bitsPerSample,
                      const uint_fast32_t &sampleRate, const uint_fast32_t &countSamplesInChannel)
{
    if ( countChannels < 1 ) {
        throw InvalidArgumentException("The number of channels is less than 1.");
    }

    if ( bitsPerSample != 16 ) {
        throw AudioFormatException("Only 16-bit samples is supported");
    }

    _logger.log(logger::Level::INFO, "Fill header start");

    if ( _header.get() != nullptr )
        _header.release();

    _header = std::make_unique < WavHeader_s >();

    auto header = _header.get();

    this->preFillHeader();

    const uint_fast32_t fileBytesSize = 44 + countChannels * sizeof(uint_fast16_t) * countSamplesInChannel;

    header->sampleRate  = sampleRate;
    header->numChannels = countChannels;

    header->chunkSize     = fileBytesSize - 8;
    header->subchunk2Size = fileBytesSize - 44;

    header->byteRate   = header->sampleRate * header->numChannels * header->bitsPerSample / 8;
    header->blockAlign = header->numChannels * header->bitsPerSample / static_cast<uint_fast16_t>(8);

    _logger.log(logger::Level::INFO, "Fill header end");
}

void Wav::dataChecking () const
{
    const auto header = this->_header.get();

    if ( header == nullptr ) {
        throw WavHeaderException("Header doesn't exist");
    }

    if ( _data.empty()) {
        throw WavDataException("Data don't exist");
    }

    if ( _data[ 0 ].empty()) {
        throw WavDataException("Data is wrong");
    }

    const size_t countChannels = _data.size();
    const size_t countSamplesInChannel = _data[ 0 ].size();

    for ( size_t i = 1; i < countChannels; i++ ) {
        if ( _data[ i ].size() != countSamplesInChannel ) {
            throw WavDataException("Data is wrong");
        }
    }

    if ( header->numChannels < 1 ) {
        throw InvalidArgumentException("The number of channels is less then 1.");
    }
}

void Wav::makeReverb (const float &delaySeconds, const float &decay)
{
    if ( decay <= 0 || delaySeconds <= 0 ) {
        throw InvalidArgumentException("Negate argument!");
    }

    this->dataChecking();

    const size_t &countChannels        = _data.size();
    const size_t countSamplesInChannel = static_cast<int>(_data[ 0 ].size());

    const auto header = _header.get();

    const int delay_samples = ( int ) ( delaySeconds * header->sampleRate );


    for ( size_t ch = 0; ch < countChannels; ch++ ) {
        std::vector < float > tmp;
        tmp.resize(countSamplesInChannel);

        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            tmp[ i ] = static_cast<float>(_data[ ch ][ i ]);
        }

        for ( int i = 0; i < countSamplesInChannel - delay_samples; i++ ) {
            tmp[ i + delay_samples ] += decay * tmp[ i ];
        }

        // Find maximum signal's magnitude
        float max_magnitude = 0.0f;

        for ( int i = 0; i < countSamplesInChannel - delay_samples; i++ ) {
            if ( std::abs(tmp[ i ]) > max_magnitude ) {
                max_magnitude = std::abs(tmp[ i ]);
            }
        }

        const float norm_coef = 30000.0f / max_magnitude;
        printf("max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef);

        // Scale back and transform floats to shorts.
        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            _data[ ch ][ i ] = static_cast<int_fast16_t>(norm_coef * tmp[ i ]);
        }
    }
}

void Wav::convertStereoToMono ()
{
    _logger.log(logger::Level::INFO, "Start convert stereo to mono");
    dataChecking();

    if ( _header.get()->numChannels != 2 ) {
        throw InvalidArgumentException("The number of channels isn't 2.");
    }

    const size_t &countSamplesInChannel = _data[ 0 ].size();

    std::deque < int_fast16_t > newData;
    newData.resize(countSamplesInChannel);

    for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
        newData[ i ] = ( _data[ 0 ][ i ] + _data[ 1 ][ i ] ) / static_cast<int_fast16_t>(2);
    }

    _data[ 0 ].clear();
    _data[ 1 ].clear();
    _data.clear();

    _data.push_back(newData);
    _logger.log(logger::Level::INFO, "End convert stereo to mono");
}

void Wav::cutBegin (const float &second)
{
    this->cut(second, false);
}

void Wav::cutEnd (const float &second)
{
    this->cut(second, true);
}

/**
 * Cut audio file.
 *
 * @param second
 * @param state false - cut at the beginning, true - cut at the end
 */
void Wav::cut (const float &second, bool state)
{
    _logger.log(logger::Level::INFO, "Start cut audio. Seconds: " + std::to_string(second));

    this->dataChecking();

    const auto header = _header.get();

    const float numChannels = static_cast<float>(header->numChannels);

    const float byteRate = static_cast<float>(header->byteRate) / numChannels;

    const float trackDuration = ( _data[ 0 ].size() * sizeof(int_fast16_t)) / byteRate - 1;
    _logger.log(logger::Level::INFO, "Byte rate: " + std::to_string(byteRate));

    if ( trackDuration <= second ) {
        throw InvalidArgumentException(
                "Time (" + std::to_string(second) + ") exceeds track length (" + std::to_string(trackDuration + 1) +
                ")");
    }

    const uint_fast32_t byteForCut = static_cast<uint_fast32_t>(( byteRate * second ) /
                                                                static_cast<float>(sizeof(int_fast16_t)));
    _logger.log(logger::Level::INFO, "Byte for cut: " + std::to_string(byteForCut));

    for ( size_t ch = 0; ch < _data.size(); ch++ ) {

        std::deque < int_fast16_t > &tmp = _data[ ch ];

        for ( uint_fast32_t i = 0; i < byteForCut; i++ ) {
            if ( state ) {
                tmp.pop_back();
            } else {
                tmp.pop_front();
            }
        }
    }

    _logger.log(logger::Level::INFO, "End cut audio. Seconds: " + std::to_string(second));
}

std::string Wav::getInfo () const
{
    const auto header = _header.get();

    std::string info;

    info += "-------------------------\n";
    info += " numChannels   " + std::to_string(header->numChannels) + "\n";
    info += " sampleRate    " + std::to_string(header->sampleRate) + "\n";
    info += " bitsPerSample " + std::to_string(header->bitsPerSample)+ "\n";
    info += " byteRate      " + std::to_string(header->byteRate) + "\n";
    info += " blockAlign    " + std::to_string(header->blockAlign) + "\n";
    info += " chunkSize     " + std::to_string(header->chunkSize) + "\n";
    info += " subchunk1Size " + std::to_string(header->subchunk1Size) + "\n";
    info += " subchunk2Size " + std::to_string(header->subchunk2Size) + "\n";
    info += "-------------------------\n";

    return info;
}

uint_fast16_t Wav::getNumChannels () const
{
    dataChecking();

    return this->_header.get()->numChannels;
}

uint_fast32_t Wav::getSampleRate () const
{
    dataChecking();

    return this->_header.get()->sampleRate;
}

uint_fast32_t Wav::getByteRate () const
{
    dataChecking();

    return this->_header.get()->byteRate;
}

uint_fast16_t Wav::getBitsPerSample () const
{
    dataChecking();

    return this->_header.get()->bitsPerSample;
}