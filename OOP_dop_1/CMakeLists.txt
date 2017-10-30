cmake_minimum_required(VERSION 3.7)
project(OOP_dop_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef -Werror -Wno-unused")

set(SOURCE_FILES
        main.cpp
        src/RegisterReader/RegisterReader.cpp
        src/RegisterReader/RegisterReader.h
        src/Exceptions/RegisterException.cpp
        src/Exceptions/RegisterException.h
        src/RegisterStorage/RegisterStorage.cpp
        src/RegisterStorage/RegisterStorage.h
        src/RegisterOption/RegisterOption.cpp
        src/RegisterOption/RegisterOption.h
        src/RegisterWriter/RegisterWriter.cpp
        src/RegisterWriter/RegisterWriter.h
        src/RegisterDriver/RegisterDriver.cpp
        src/RegisterDriver/RegisterDriver.h)

add_executable(OOP_dop_1 ${SOURCE_FILES})