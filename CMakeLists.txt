cmake_minimum_required(VERSION 3.17)
project(CMPE380 C)
set(CMAKE_C_STANDARD 99)

option(SANITIZER_BUILD "Build with ASan" OFF)

if (SANITIZER_BUILD)
    add_compile_options(-fsanitize=address -fsanitize=leak)
    add_link_options(-fsanitize=address)
endif()

add_compile_options(-O1 -g -Wall -std=c99 -pedantic)

add_subdirectory(Lab02)
add_subdirectory(Lab03)
add_subdirectory(Lab04)