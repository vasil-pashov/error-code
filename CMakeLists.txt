cmake_minimum_required(VERSION 3.14.0)
project(error_code
	VERSION 0.1.0
	LANGUAGES CXX
)

add_library(${PROJECT_NAME} STATIC
	cpp/error_code.cpp
	include/error_code.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
add_compile_definitions(${PROJECT_NAME}
	ERROR_CODE_MAJOR_VERSION=${CMAKE_PROJECT_VERSION_MAJOR}
	ERROR_CODE_MINOR_VERSION=${CMAKE_PROJECT_VERSION_MINOR}
	ERROR_CODE_PATCH_VERSION=${CMAKE_PROJECT_VERSION_PATCH}
)