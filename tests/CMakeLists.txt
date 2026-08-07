find_package(GTest REQUIRED)

project(${PROJECT_NAME}_test CXX)

file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_executable(${PROJECT_NAME} ${test_sources})
target_link_libraries(${PROJECT_NAME} PRIVATE brainfuck_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
