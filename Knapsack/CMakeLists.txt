cmake_minimum_required(VERSION 3.10)

#Configuration parameters of cmake
set(CMAKE_CXX_COMPILER g++)#requires g++ to be available on PATH
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DrugieZadanie)
enable_language(CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

#Source files here
set(SOURCE_FILES
        src/main.cpp)

add_executable(Program ${SOURCE_FILES})