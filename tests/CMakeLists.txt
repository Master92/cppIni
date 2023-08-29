cmake_minimum_required(VERSION 3.24)

include(CTest)

set(TEST_DRIVER_NAME TestDriver)

find_package(doctest REQUIRED)

add_executable(${PROJECT_NAME}_tests ConstructionTest.cpp)
target_link_libraries(${PROJECT_NAME}_tests doctest::doctest cppIni)
target_compile_definitions(${PROJECT_NAME}_tests
    PUBLIC
        DOCTEST_CONFIG_NO_MULTITHREADING
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${DOCTEST_INCLUDE_DIR}
)
