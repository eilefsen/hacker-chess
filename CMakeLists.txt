cmake_minimum_required(VERSION 3.30..3.31)
project(hacker-chess
	VERSION 1.0
	DESCRIPTION "Chess server built around a plain-text protocol. Perfect for playing chess in the terminal!"
	LANGUAGES C
)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
						STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
				"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_subdirectory(src)
