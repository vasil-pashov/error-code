cmake_minimum_required(VERSION 3.14.0)
project(tests LANGUAGES CXX)

set(CPP cpp/main.cpp)

add_executable(${PROJECT_NAME} ${CPP})
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest error_code)
doctest_discover_tests(${PROJECT_NAME})