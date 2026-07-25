cmake_minimum_required(VERSION 3.20)

project(CompoundEngine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(compound STATIC
    src/compound.c
)

add_executable(test_version
    tests/test_version.c
)

target_link_libraries(test_version compound)