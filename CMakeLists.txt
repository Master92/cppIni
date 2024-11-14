cmake_minimum_required(VERSION 3.24)
project(cppIni LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build test files" OFF)
option(BUILD_SHARED_LIBS "Build shared library files" ON)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

include(cmake/CodeCoverage.cmake)
add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()