cmake_minimum_required(VERSION 3.14.0)
project(error_code
	VERSION 0.1.1
	LANGUAGES CXX
)
cmake_policy(SET CMP0077 NEW)
include(FetchContent)

option(ERROR_CODE_WITH_TESTS "Builds tests for the project" OFF)

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

if(${ERROR_CODE_WITH_TESTS})
	# Most projects don't need to call find_package and check <name>_FOUND
	# variable when working with FetchContent, because fetch content has
	# FIND_PACKAGE_ARGS which if passed will first try to execute find_package
	# with those arguments and then fallback on FetchContent. However for this
	# project we need to include a file doctest.cmake. If find_package finds
	# the package we can do it with include(doctest) because it is installed
	# into <package>/lib/cmake and cmake knows where to search for it. If
	# find_package fails and FetchContent does a fallback the file would be
	# in ${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake. It seems like there
	# is no way to tell if FetchContent_Declare with FIND_PACKAGE_ARGS did
	# find_package successfully or it had to fallback, because FetchContent_Declare
	# does not set <package>_FOUND even if the package was found.
	set(_doctest_version 2.4.8)
	find_package(doctest ${_doctest_version} QUIET)
	set(_doctest_cmake_path "doctest")
	if(NOT ${doctest_FOUND})
		message(STATUS "Doctest was not found. Proceeding to download it.")
		FetchContent_Declare(
			doctest
			GIT_REPOSITORY https://github.com/doctest/doctest.git
			GIT_TAG "v${_doctest_version}"
		)
		set(DOCTEST_NO_INSTALL ON CACHE BOOL "")
		set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "")
		FetchContent_MakeAvailable(doctest)
		set(_doctest_cmake_path ${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
	endif()
	include(${_doctest_cmake_path})
	enable_testing()
	add_subdirectory(tests)
endif()