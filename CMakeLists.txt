cmake_minimum_required(VERSION 3.3)
project(hide_stuff_in_images)

#[[
clang++ \
    -Rpass=loop-vectorize \
    -Rpass-missed=loop-vectorize \
    -Rpass-analysis=loop-vectorize \
    -O3 -ffast-math -std=c++14 -lboost_timer -lboost_system -lpthread \

]]

#project(assembler C ASM) # this enables building the assembly part of the code
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lz") # for zlib
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lfltk -lfltk_images")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lginac -lcln") # for ginac

set(SOURCE_FILES
#    extract_least_significant_bits.cpp
    hide_file.cpp
    lib/lib.cpp
#    lib.h
    lib/lodepng.cpp
#    lodepng.h
)

add_executable(hide_stuff_in_images ${SOURCE_FILES})

add_executable(test_lzma_stream tests/test_lzma_stream.cpp lib/lzma.cpp)