cmake_minimum_required(VERSION 3.7)
project(OOP_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wall)

set(SOURCE_FILES main.cpp Logger.cpp Logger.h IniParser.cpp IniParser.h IniParserException.cpp IniParserException.h)
add_executable(OOP_1 ${SOURCE_FILES})