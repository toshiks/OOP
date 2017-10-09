cmake_minimum_required(VERSION 3.7)
project(OOP_3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef -Werror -Wno-unused")

set(SOURCE_FILES
        main.cpp
        src/WAV/WavHeader.h
        src/WAV/Wav.cpp
        src/WAV/Wav.h
        src/WavException/WavException.cpp
        src/WavException/WavException.h
        src/Logger/Logger.cpp
        src/Logger/Logger.h)

add_executable(OOP_3 ${SOURCE_FILES})