cmake_minimum_required(VERSION 3.7)
project(OOP_2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS -Wall)

set(SOURCE_FILES main.cpp StatisticMultiset.h StatisticMultisetException.cpp StatisticMultisetException.h Logger.cpp Logger.h)
add_executable(OOP_2 ${SOURCE_FILES})