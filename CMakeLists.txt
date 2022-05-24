cmake_minimum_required(VERSION 3.14.0)
project(error_code
	VERSION 0.1.1
	LANGUAGES CXX
)

option(WITH_TESTS "Builds tests for the project" OFF)

add_library(${PROJECT_NAME} STATIC
	cpp/error_code.cpp
	include/error_code.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_compile_definitions(
	ERROR_CODE_MAJOR_VERSION=${CMAKE_PROJECT_VERSION_MAJOR}
	ERROR_CODE_MINOR_VERSION=${CMAKE_PROJECT_VERSION_MINOR}
	ERROR_CODE_PATCH_VERSION=${CMAKE_PROJECT_VERSION_PATCH}
)

if(${WITH_TESTS})
	include("vendor/doctest/scripts/cmake/doctest.cmake")
	set(DOCTEST_NO_INSTALL ON)
	enable_testing()
	add_subdirectory(vendor/doctest)
	add_subdirectory(tests)
endif()