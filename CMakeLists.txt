## CMake script for libftl.

PROJECT(algebra.h)

## Global configurations.
cmake_minimum_required(VERSION 3.2)
enable_testing()
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BUILD_STATIC_EXECUTABLE OFF)

## Set up compiler and options.
## set(CMAKE_CXX_COMPILER g++)
add_compile_options(
    -Wall
    -O2
    -std=c++17
    -pedantic-errors
)

## static link standrd C++ library.
if(BUILD_STATIC_EXECUTABLE)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

## C++ header files.
include_directories(
    include/
    third_party/autocheck/include/
    third_party/bandit/
)

## All demos.
add_executable(helloworld
    demo/helloworld.cxx
)

## Unit tests.
add_executable(helloworld_test
    test/helloworld_test.cxx
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

