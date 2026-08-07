cmake_minimum_required(VERSION 3.25)

project(BrainFuck LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(brainfuck src/BrainFuck.cpp)
target_include_directories(brainfuck PUBLIC src)

add_executable(brainfuck_app src/main.cpp)
target_link_libraries(brainfuck_app PRIVATE brainfuck)

include(CTest)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
