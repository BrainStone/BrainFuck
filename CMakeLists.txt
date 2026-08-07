cmake_minimum_required(VERSION 3.25)

project(BrainFuck LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(brainfuck_lib src/BrainFuck.cpp)
target_include_directories(brainfuck_lib PUBLIC src)

add_executable(brainfuck src/main.cpp)
target_link_libraries(brainfuck PRIVATE brainfuck_lib)

include(CTest)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
