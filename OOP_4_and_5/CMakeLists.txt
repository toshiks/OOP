cmake_minimum_required(VERSION 3.7)
project(OOP_4_and_5)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2 -pthread)

include_directories(vendor/gmp/include)
link_directories(vendor/gmp/lib)

add_subdirectory(vendor/googletest)
include_directories(vendor/googletest/googletest/include)
include_directories(vendor/googletest/googlemock/include)
include_directories(vendor)
include_directories(src)

#add_compile_options(-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy)
#add_compile_options(-Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op)
#add_compile_options(-Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast)
#add_compile_options(-Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo)
#add_compile_options(-Wstrict-null-sentinel -Wswitch-default)
#add_compile_options(-Werror -Wno-unused)


set(SOURCE_FILES
        main.cpp
        src/AtkinSieve/AtkinSieve.cpp
        src/AtkinSieve/AtkinSieve.h
        tests/TestAtkin.cpp
        src/MathFunctions/MathFunctions.cpp
        src/MathFunctions/MathFunctions.h
        tests/TestMath.cpp
        src/QuadraticSieve/QuadraticSieve.cpp
        src/QuadraticSieve/QuadraticSieve.h
        tests/TestQuadraticSieve.cpp
        src/Worker/Worker.cpp src/Worker/Worker.h)

add_executable(OOP_4_and_5 ${SOURCE_FILES})

target_link_libraries(OOP_4_and_5 gmpxx gmp gtest gtest_main)