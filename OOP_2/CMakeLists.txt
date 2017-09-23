cmake_minimum_required(VERSION 3.7)
project(OOP_2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wall)

set(SOURCE_FILES
        main.cpp
        src/StatisticMultiset.h
        src/StatisticMultisetException/StatisticMultisetException.cpp
        src/StatisticMultisetException/StatisticMultisetException.h
        src/Logger/Logger.cpp
        src/Logger/Logger.h)

add_executable(OOP_2 ${SOURCE_FILES})