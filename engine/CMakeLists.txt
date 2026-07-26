cmake_minimum_required(VERSION 3.20)

project(CompoundEngine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(compound STATIC
    src/compound.c
    src/skill.c
)

add_executable(test_skill
    tests/test_skill.c
)

target_link_libraries(test_skill compound)