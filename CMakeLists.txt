cmake_minimum_required(VERSION 3.17)
project(CMPE380 C)
enable_language(CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 98)

option(SANITIZER_BUILD "Build with ASan" OFF)

if (SANITIZER_BUILD)
    add_compile_options(-fsanitize=address -fsanitize=leak)
    add_link_options(-fsanitize=address)
endif()

add_compile_options(-O1 -g -Wall -std=c99 -pedantic)

add_subdirectory(Lab02)
add_subdirectory(Lab03)
add_subdirectory(Lab04)
add_subdirectory(Lab06)
add_subdirectory(Lab07)
add_subdirectory(Lab08)
add_subdirectory(Lab09)
