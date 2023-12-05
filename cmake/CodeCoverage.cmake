cmake_minimum_required(VERSION 3.24)

# Code coverage configuration
add_library(coverage_config INTERFACE)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("Enabling code coverage")
    target_compile_options(coverage_config INTERFACE
            -O0             # no optimization
            -g              # generate debug info
            --coverage      # sets all required flags
    )
    target_link_options(coverage_config INTERFACE --coverage)
endif()
install(TARGETS coverage_config EXPORT ${PROJECT_NAME}-targets)
