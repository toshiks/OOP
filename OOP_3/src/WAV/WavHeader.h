#ifndef WAV_HEADER_H
#define WAV_HEADER_H

struct WavHeader_s
{
    uint_fast32_t chunkId;
    uint_fast32_t chunkSize;
    uint_fast32_t format;
    uint_fast32_t subchunk1Id;
    uint_fast32_t subchunk1Size;
    uint_fast16_t audioFormat;
    uint_fast16_t numChannels;
    uint_fast32_t sampleRate;
    uint_fast32_t byteRate;
    uint_fast16_t blockAlign;
    uint_fast16_t bitsPerSample;
    uint_fast32_t subchunk2Id;
    uint_fast32_t subchunk2Size;
};

#endif // WAV_HEADER_H
