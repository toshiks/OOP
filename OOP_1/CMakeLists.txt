cmake_minimum_required(VERSION 3.7)
project(OOP_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wall)

set(SOURCE_FILES
        main.cpp
        src/Logger/Logger.cpp
        src/Logger/Logger.h
        src/IniParser/IniParser.cpp
        src/IniParser/IniParser.h
        src/IniParserException/IniParserException.cpp
        src/IniParserException/IniParserException.h)


add_executable(OOP_1 ${SOURCE_FILES})
