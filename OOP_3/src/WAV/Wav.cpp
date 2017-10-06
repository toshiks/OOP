/**
 * @file wav.cpp
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 02.10.2017
 * @version 1.0
 */

#include <iostream>
#include <cmath>

#include "Wav.h"
#include "../WavException/WavException.h"

Wav::Wav () : _header(nullptr), _logger("LOGGER", "E:\\CLionProjects\\OOP\\OOP\\OOP_3\\logger.txt")
{ }

auto Wav::getFile (bool state, const std::string &fileName)
{
    //For auto-close file
    auto deleter = [ & ] (FILE* f) {
        fclose(f);
        _logger.log(logger::Level::INFO, "File closed.");
    };

    std::string resume = (state ? "rb" : "wb");

    std::unique_ptr < FILE, decltype(deleter) > file(fopen(fileName.c_str(), resume.c_str()), deleter);

    if (state) {
        if ( !file.get()) {
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
    auto file = this->getFile(true, _fileName);

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

    auto file = getFile(true, _fileName);

    std::vector < int_fast16_t > samples;

    fseek(file.get(), 44, SEEK_SET);

    auto header = this->_header.get();

    uint_fast32_t countChannels = header->numChannels;

    uint_fast32_t countSamplesInChannel = ( header->subchunk2Size / sizeof(uint_fast16_t)) / countChannels;

    samples.resize( countSamplesInChannel * countChannels );

    size_t readBytes = fread(samples.data(), 1, header->subchunk2Size, file.get());

    if ( readBytes != _header.get()->subchunk2Size ){
        throw WavIOException("readData() read only " + std::to_string(readBytes) +
                                     " of " + std::to_string(_header.get()->subchunk2Size));
    }

    _logger.log(logger::Level::INFO, "Data size: " + std::to_string(samples.size()));
    _logger.log(logger::Level::INFO, "Data transform start");

    _data.resize(countChannels);

    for (size_t i = 0; i < _data.size(); i++){
        _data[i].resize(countSamplesInChannel);
    }

    for ( int ch = 0; ch < countChannels; ch++ ) {
        std::vector < int_fast16_t >& chdata = _data[ ch ];

        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            chdata[ i ] = samples[ countChannels * i + ch ];
        }
    }

    _logger.log(logger::Level::INFO, "Data transform end");
}

void Wav::printInfo ()
{
    auto header = _header.get();
    std::cout <<  "-------------------------\n";
    std::cout <<  " audioFormat   " <<  header->audioFormat << "\n";
    std::cout <<  " numChannels   " <<  header->numChannels << "\n";
    std::cout <<  " sampleRate    " <<  header->sampleRate << "\n";
    std::cout <<  " bitsPerSample " <<  header->bitsPerSample << "\n";
    std::cout <<  " byteRate      " <<  header->byteRate << "\n";
    std::cout <<  " blockAlign    " <<  header->blockAlign << "\n";
    std::cout <<  " chunkSize     " <<  header->chunkSize << "\n";
    std::cout <<  " subchunk1Size " <<  header->subchunk1Size << "\n";
    std::cout <<  " subchunk2Size " <<  header->subchunk2Size << "\n";
    std::cout <<  "-------------------------\n";
}


void Wav::preFillHeader ()
{
    _logger.log(logger::Level::INFO, "Pre fill header start");
    auto header = _header.get();

    header->chunkId = /*0x52494646; */0x46464952;
    header->format  = /*0x57415645;*/ 0x45564157;

    header->subchunk1Id = /*0x666d7420;*/ 0x20746d66;
    header->subchunk2Id = /*0x64617461;*/ 0x61746164;

    header->audioFormat   = 1;
    header->subchunk1Size = 16;
    header->bitsPerSample = 16;
    _logger.log(logger::Level::INFO, "Pre fill header end");
}

void Wav::fillHeader (uint_fast16_t countChannels, uint_fast16_t bitsPerSample,
                      uint_fast32_t sampleRate, uint_fast32_t countSamplesInChannel)
{
    if (countChannels < 1){
        throw InvalidArgumentException("The number of channels is less than 1.");
    }

    if (bitsPerSample != 16){
        throw AudioFormatException("Only 16-bit samples is supported");
    }

    _logger.log(logger::Level::INFO, "Fill header start");

    if (_header.get() != nullptr)
        _header.release();

    _header = std::make_unique<wav_header_s>();

    auto header = _header.get();

    this->preFillHeader();

    uint_fast32_t fileBytesSize = 44 + countChannels * (bitsPerSample/ sizeof(uint_fast16_t) ) * countSamplesInChannel;

    header->sampleRate = sampleRate;
    header->numChannels = countChannels;

    header->chunkSize = fileBytesSize - 8;
    header->subchunk2Size = fileBytesSize - 44;

    header->byteRate      = header->sampleRate * header->numChannels * header->bitsPerSample / 8;
    header->blockAlign    = header->numChannels * header->bitsPerSample / static_cast<uint_fast16_t>(8);

    _logger.log(logger::Level::INFO, "Fill header end");
}

void Wav::save (const std::string &fileName)
{
    dataChecking();

    uint_fast16_t countChannels = static_cast<uint_fast16_t>(_data.size());
    size_t countSamplesInChannel = _data[0].size();

    _logger.log(logger::Level::INFO, "Start to save wav file");

    this->fillHeader(countChannels, 16, _header.get()->sampleRate, countSamplesInChannel);

    std::vector< int_fast16_t > transformData;

    transformData.resize(countChannels * countSamplesInChannel);

    for ( uint_fast16_t ch = 0; ch < countChannels; ch++ ) {
        const std::vector<int_fast16_t>& chdata = _data[ ch ];
        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            transformData[ countChannels * i + ch ] = chdata[ i ];
        }
    }

    auto file = getFile(false, fileName);


    fwrite( this->_header.get(), sizeof(wav_header_s), 1, file.get() );
    fwrite( transformData.data(), sizeof(int_fast16_t), transformData.size(), file.get() );

    _logger.log(logger::Level::INFO, "Transform data size: " + std::to_string(transformData.size()));
    _logger.log(logger::Level::INFO, "End to save wav file");
}

void Wav::dataChecking ()
{
    auto header = this->_header.get();

    if ( header == nullptr ){
        throw WavHeaderException("Header doesn't exist");
    }

    if ( _data.empty() ){
        throw  WavDataException("Data don't exist");
    }

    if ( _data[0].empty() ){
        throw  WavDataException("Data is wrong");
    }

    uint_fast16_t countChannels = static_cast<uint_fast16_t>(_data.size());
    size_t countSamplesInChannel = _data[0].size();

    for (uint_fast16_t i = 1; i < countChannels; i++){
        if ( _data[i].size() != countSamplesInChannel ){
            throw  WavDataException("Data is wrong");
        }
    }

    if (header->numChannels < 1){
        throw InvalidArgumentException("The number of channels is less then 1.");
    }
}

void Wav::makeReverb (double delaySeconds, float decay)
{
    if (decay <= 0 || delaySeconds <= 0){
        throw InvalidArgumentException("Negate argument!");
    }

    this->dataChecking();

    size_t countChannels = _data.size();
    size_t countSamplesInChannel = _data[0].size();

    auto header = _header.get();

    int delay_samples = (int)(delaySeconds * header->sampleRate);


    for ( size_t ch = 0; ch < countChannels; ch++ ) {
        std::vector<float> tmp;
        tmp.resize(countSamplesInChannel);

        // Convert signal from short to float
        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            tmp[ i ] = _data[ ch ][ i ];
        }

        // Add a reverb
        for ( size_t i = 0; i < static_cast<int>(countSamplesInChannel) - delay_samples; i++ ) {
            tmp[ i + delay_samples ] += decay * tmp[ i ];
        }

        // Find maximum signal's magnitude
        float max_magnitude = 0.0f;
        for ( size_t i = 0; i < static_cast<int>(countSamplesInChannel) - delay_samples; i++ ) {
            if ( std::abs(tmp[ i ]) > max_magnitude ) {
                max_magnitude = std::abs(tmp[ i ]);
            }
        }

        float norm_coef = 30000.0f / max_magnitude;
        printf( "max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef );

        // Scale back and transform floats to shorts.
        for ( size_t i = 0; i < countSamplesInChannel; i++ ) {
            _data[ ch ][ i ] = (short)(norm_coef * tmp[ i ]);
        }
    }
}

void Wav::convertStereoToMono ()
{
    _logger.log(logger::Level::INFO, "Start convert stereo to mono");
    dataChecking();

    if (_header.get()->numChannels != 2){
        throw InvalidArgumentException("The number of channels isn't 2.");
    }

    uint_fast32_t countSamplesInChannel = _data[0].size();

    std::vector<int_fast16_t> newData;
    newData.resize( countSamplesInChannel );

    for (uint_fast32_t i = 0; i < countSamplesInChannel; i++){
        newData[i] = (_data[0][i] + _data[1][i]) / static_cast<uint_fast16_t>(2);
    }

    _data[0].clear();
    _data[1].clear();
    _data.clear();

    _data.push_back(newData);
    _logger.log(logger::Level::INFO, "End convert stereo to mono");
}

void Wav::checkHeader (const long &fileSize) const
{
    auto header = this->_header.get();
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
        std::cout << _header.get()->subchunk1Size << '\n';
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