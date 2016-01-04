## CMake script for libftl.

PROJECT(libftl)

## Global configurations.
cmake_minimum_required(VERSION 3.2)
enable_testing()
set(CMAKE_VERBOSE_MAKEFILE off)

## Set up compiler and options.
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(
    -Wall
    ## -Wextra
    ## -Werror
    -std=c++14
    -O1
)

## C++ header files.
include_directories(
    include/
    contrib/autocheck/include/
    contrib/bandit/
)

## Common source files.
set(COMMON)

## Executables
add_executable(helloworld
    ${COMMON} app/helloworld.cpp
)

## Unit tests.
add_executable(helloworld_test
    ${COMMON} test/helloworld_test.cpp
)
add_test(helloworld_test helloworld_test)

## Add "--output-on-failure" via custom target.
if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure
        --build-config "$<CONFIGURATION>")
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure)
endif()

