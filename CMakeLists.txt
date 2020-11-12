cmake_minimum_required(VERSION 3.17)
project(ifjproj C)

set(CMAKE_C_STANDARD 99)

add_executable(ifjproj main.c scanner.h scanner.c types_definition.c types_definition.h)