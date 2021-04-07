cmake_minimum_required(VERSION 3.10)

#Configuration parameters of cmake
set(CMAKE_CXX_COMPILER g++)#requires g++ to be available on PATH
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PierwszeZadanie)
enable_language(CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

#Source files here
set(SOURCE_FILES
        src/main.cpp)

add_executable(Program ${SOURCE_FILES})

#target_include_directories(Tunel PUBLIC include)

#TESTs
#find_package(Boost 1.65 COMPONENTS "unit_test_framework" REQUIRED)

#Test source files here
#set(SOURCE_TEST_FILES
#        )
#
#add_executable(TunelTesting ${SOURCE_TEST_FILES})
#
#target_include_directories(TunelTesting PUBLIC include ${Boost_INCLUDE_DIRS})
#
#target_link_libraries(TunelTesting
#        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#
#enable_testing()
#
#add_test(NAME Test COMMAND TunelTesting
#        --report_level=detailed
#        --log_level=all
#        --color_output=yes)
#
#add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 BOOST_TEST_LOG_LEVEL=all
#        ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})