cmake_minimum_required(VERSION 3.24)
project(cppIni LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING ON "Build test files")
option(BUILD_SHARED_LIBS ON "Build shared library files")

add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()