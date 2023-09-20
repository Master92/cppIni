cmake_minimum_required(VERSION 3.24)

set(CMAKE_DEBUG_POSTFIX d)

set(SOURCES
    Entry.cpp
    File.cpp
    Section.cpp
)

set(API_HEADERS
    cppIni.h
)
LIST(TRANSFORM API_HEADERS PREPEND ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/)

set(HEADERS
    Entry.h
    File.h
    Section.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${API_HEADERS} ${HEADERS})

include(GenerateExportHeader)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
generate_export_header(${PROJECT_NAME}
    EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
