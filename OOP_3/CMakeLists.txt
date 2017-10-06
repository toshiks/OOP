cmake_minimum_required(VERSION 3.7)
project(OOP_3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(SOURCE_FILES
        main.cpp
        src/wav_header.h
        src/WAV/Wav.cpp
        src/WAV/Wav.h
        src/WavException/WavException.cpp
        src/WavException/WavException.h
        src/Logger/Logger.cpp
        src/Logger/Logger.h)
add_executable(OOP_3 ${SOURCE_FILES})