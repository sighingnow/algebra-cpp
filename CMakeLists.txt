PROJECT(algebra.h)

## Global configurations.
cmake_minimum_required(VERSION 3.2)
enable_testing()
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BUILD_STATIC_EXECUTABLE OFF)

## Set up compiler and options.
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(
    -Wall
    -Werror
    -O2
    -std=c++14
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
add_executable(hello-algebra
    demo/hello-algebra.cxx
    demo/monoid-demo.cxx
    demo/string-demo.cxx
    demo/list-demo.cxx
    demo/functor-applicative-monad-demo.cxx
)

## Unit tests.
add_executable(algebra-test test/algebra-test.cxx)
add_test(algebra-test algebra-test)
add_executable(monoid-test test/monoid-test.cxx)
add_test(monoid-test monoid-test)
add_executable(string-test test/string-test.cxx)
add_test(string-test string-test)
add_executable(list-test test/list-test.cxx)
add_test(list-test list-test)

## Add "--output-on-failure" via custom target.
if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure
        --build-config "$<CONFIGURATION>")
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --output-on-failure)
endif()
