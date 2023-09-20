cmake_minimum_required(VERSION 3.24)

find_package(doctest REQUIRED)

set(TEST_SOURCES
    EntryTest.cpp
    FileTest.cpp
    SectionTest.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}_tests doctest::doctest ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME}_tests
    PUBLIC
        DOCTEST_CONFIG_NO_MULTITHREADING
        DOCTEST_CONFIG_USE_STD_HEADERS
        WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}_tests
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${DOCTEST_INCLUDE_DIR}
)

find_file(DOCTEST_CMAKE doctest.cmake)
if(NOT DOCTEST_CMAKE)
    message(FATAL_ERROR "Could not find doctest.cmake")
else()
    include(${DOCTEST_CMAKE})
    doctest_discover_tests(${PROJECT_NAME}_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../src)
endif()
