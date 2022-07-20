cmake_minimum_required(VERSION 3.14.0)
project(error_code
	VERSION 0.1.1
	LANGUAGES CXX
)
cmake_policy(SET CMP0077 NEW)
include(FetchContent)

option(ERROR_CODE_WITH_TESTS "Builds tests for the project" OFF)
option(ERROR_CODE_WITH_INSTALL "Generate install target" OFF)

add_library(${PROJECT_NAME} STATIC
	cpp/error_code.cpp
	include/error_code.h
)

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
	set(doctest_version 2.4.8)
	find_package(doctest ${doctest_version} QUIET)
	set(doctest_cmake_path "doctest")
	if(NOT ${doctest_FOUND})
		message(STATUS "Doctest was not found. Proceeding to download it.")
		FetchContent_Declare(
			doctest
			GIT_REPOSITORY https://github.com/doctest/doctest.git
			GIT_TAG "v${doctest_version}"
		)
		set(DOCTEST_NO_INSTALL ON CACHE BOOL "")
		set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "")
		FetchContent_MakeAvailable(doctest)
		set(doctest_cmake_path ${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
	endif()
	include(${doctest_cmake_path})
	enable_testing()
	add_subdirectory(tests)
endif()

if(${ERROR_CODE_WITH_INSTALL})
	include(CMakePackageConfigHelpers)
	if(UNIX)
		include(GNUInstallDirs)
		set(include_install_dir ${CMAKE_INSTALL_INCLUDEDIR})
		set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
		set(lib_install_dir ${CMAKE_INSTALL_LIBDIR})
	else()
		set(include_install_dir "include")
		set(config_install_dir "lib/cmake/${PROJECT_NAME}")
		set(lib_install_dir "lib")
	endif()
	set(config_install_dir "${config_install_dir}")

	set(targets_export_name "${PROJECT_NAME}Targets")
	install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${targets_export_name}
		ARCHIVE DESTINATION ${lib_install_dir}
		INCLUDES DESTINATION ${include_install_dir}
	)

	install(
		EXPORT ${targets_export_name}
		FILE "${targets_export_name}.cmake"
		NAMESPACE ErrorCode::
		DESTINATION ${config_install_dir}
	)

	install(
		FILES
			include/error_code.h
			DESTINATION ${include_install_dir}
	)

	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		INSTALL_DESTINATION "${config_install_dir}"
		NO_SET_AND_CHECK_MACRO
		NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		COMPATIBILITY SameMinorVersion
	)

	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION ${config_install_dir}
	)
endif()

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${config_install_dir}>
)