cmake_minimum_required( VERSION 3.20 )

project( qoi CXX C )

option( WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF )
option( ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF )
option( ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF )
option( ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF )

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectSettings.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake )

include( FetchContent )

macro( FetchAddSub #[[LibName FetchOptions...]] )
	FetchContent_Declare( ${ARGV} )
	FetchContent_Populate( ${ARGV0} )
	file( REMOVE ${${ARGV0}_SOURCE_DIR}/CMakeLists.txt )
	file( COPY ${ARGV0}.cmake DESTINATION ${${ARGV0}_SOURCE_DIR} )
	file( RENAME ${${ARGV0}_SOURCE_DIR}/${ARGV0}.cmake ${${ARGV0}_SOURCE_DIR}/CMakeLists.txt )
	add_subdirectory( ${${ARGV0}_SOURCE_DIR} ${${ARGV0}_BINARY_DIR} )
endmacro()
macro( FetchMakeAvail #[[LibName FetchOptions...]] )
	FetchContent_Declare( ${ARGV} )
	FetchContent_MakeAvailable( ${ARGV0} )
endmacro()
