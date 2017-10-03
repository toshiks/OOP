cmake_minimum_required(VERSION 3.7)
project(OOP3)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        src/wav_header.h
        src/WAV/Wav.cpp
        src/WAV/Wav.h
        src/WavException/WavException.cpp
        src/WavException/WavException.h
        src/Logger/Logger.cpp
        src/Logger/Logger.h)
add_executable(OOP3 ${SOURCE_FILES})