/**
 * @file wav.cpp
 * Place for description
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 02.10.2017
 * @version 1.0
 */

#include <iostream>

#include "Wav.h"
#include "../WavException/WavException.h"

Wav::Wav () : _header(nullptr), _logger("LOGGER", "E:\\CLionProjects\\OOP\\OOP\\OOP_3\\logger.txt")
{ }

auto Wav::getFile ()
{
    //For auto-close file
    auto deleter = [ & ] (FILE* f) {
        fclose(f);
        _logger.log(logger::Level::INFO, "File closed.");
    };

    std::unique_ptr < FILE, decltype(deleter) > file(fopen(this->_fileName.c_str(), "rb"), deleter);

    if ( !file.get()) {
        throw FileDoesNotExistException("File " + this->_fileName + " is not exist");
    }

    this->_logger.log(logger::Level::INFO, "Open file " + this->_fileName);

    return file;
}

void Wav::createFromFile (const std::string &name)
{
    this->_logger.log(logger::Level::INFO, ">>>> Read of file ( " + name + " )");
    this->_fileName = name;

    if ( this->_header.get() != nullptr ) {
        this->_header.release();
    }

    this->_header = std::make_unique < wav_header_s >();

    this->_logger.log(logger::Level::INFO, "Read header.");
    readHeader();
    this->_logger.log(logger::Level::INFO, "The header has been read.");


    this->_logger.log(logger::Level::INFO, "Read data.");
    readData();
    this->_logger.log(logger::Level::INFO, "The data has been read.");
}

void Wav::readHeader ()
{
    auto file = this->getFile();

    size_t countByte = fread(this->_header.get(), sizeof(wav_header_s), 1, file.get());

    if ( countByte != 1 ) {
        throw FileFormatException("File " + this->_fileName + " isn't read.");
    }

    fseek(file.get(), 0, SEEK_END); // seek to the end of the file
    long fileSize = ftell(file.get());

    checkHeader(fileSize);
}

void Wav::readData ()
{
    if ( this->_header->bitsPerSample != 16 ) {
        throw AudioFormatException("Only 16-bit samples is supported");
    }

    auto file = getFile();

    std::vector < uint_fast16_t > samples;

    fseek(file.get(), 44, SEEK_SET);

    uint_fast32_t countChannels = this->_header.get()->numChannels;

    uint_fast32_t countSamplesInChannel = ( this->_header.get()->subchunk2Size / sizeof(uint_fast16_t)) / countChannels;

    samples.resize( countSamplesInChannel * countChannels );

    size_t readBytes = fread(samples.data(), 1, this->_header.get()->subchunk2Size, file.get());

    if ( readBytes != _header.get()->subchunk2Size ){
        throw WavIOException("readData() read only " + std::to_string(readBytes) +
                                     " of " + std::to_string(_header.get()->subchunk2Size));
    }


    _data.resize(countChannels);

    for (size_t i = 0; i < _data.size(); i++){
        _data[i].resize(countSamplesInChannel);
    }

    for ( int ch = 0; ch < countChannels; ch++ ) {
        std::vector < uint_fast16_t >& chdata = _data[ ch ];

        for ( size_t i = 0; i < countChannels; i++ ) {
            chdata[ i ] = samples[ countChannels * i + ch ];
        }
    }
}

void Wav::checkHeader (const long &fileSize) const
{
    if ( this->_header.get()->chunkId != 0x46464952 ) {
        throw HeaderSymbolsException("'RIFF' doesn't exist");
    }

    if ( this->_header.get()->chunkSize != fileSize - 8 ) {
        throw HeaderSizeException("Chunk size is error");
    }

    if ( this->_header.get()->format != 0x45564157 ) {
        throw HeaderSymbolsException("'WAVE' doesn't exist");
    }

    if ( this->_header.get()->subchunk1Id != 0x20746d66 ) {
        throw HeaderSymbolsException("'fmt ' doesn't exist");
    }

    if ( this->_header.get()->subchunk1Size != 16 ) {
        std::cout << _header.get()->subchunk1Size << '\n';
        throw HeaderSizeException("Sub chunk 1 size is error");
    }

    if ( this->_header.get()->audioFormat != 1 ) {
        throw AudioFormatException("File isn't PCM");
    }

    if ( this->_header.get()->byteRate !=
         this->_header.get()->sampleRate * this->_header.get()->numChannels * this->_header.get()->bitsPerSample / 8 ) {
        throw HeaderSizeException("Byte Rate is error");
    }

    if ( this->_header.get()->blockAlign !=
         this->_header.get()->numChannels * this->_header.get()->bitsPerSample / 8 ) {
        throw HeaderSizeException("Block align is error");
    }

    if ( this->_header.get()->subchunk2Id != 0x61746164 ) {
        throw HeaderSymbolsException("'data' doesn't exist");
    }

    if ( this->_header.get()->subchunk2Size != fileSize - 44 ) {
        throw HeaderSizeException("Sub chunk 2 size is error");
    }
}