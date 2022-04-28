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
	file( COPY ext/ DESTINATION ${${ARGV0}_SOURCE_DIR} FILES_MATCHING PATTERN ${ARGV0}* )
	file( RENAME ${${ARGV0}_SOURCE_DIR}/${ARGV0}.cmake ${${ARGV0}_SOURCE_DIR}/CMakeLists.txt )
	add_subdirectory( ${${ARGV0}_SOURCE_DIR} ${${ARGV0}_BINARY_DIR} )
endmacro()
macro( FetchMakeAvail #[[LibName FetchOptions...]] )
	FetchContent_Declare( ${ARGV} )
	FetchContent_MakeAvailable( ${ARGV0} )
endmacro()
macro( AddLib #[[LibName LibSource LinkLibName]])
	add_library( ${ARGV0} ${ARGV1} )
	target_link_libraries( ${ARGV0} PRIVATE ${ARGV2} )
	target_compile_features( ${ARGV0} PUBLIC cxx_std_17 )
endmacro()

FetchAddSub( phoboslab_qoi
	GIT_REPOSITORY https://github.com/phoboslab/qoi.git
	GIT_TAG 6d7eadd28c0be90e36a853a7bbb1782e50a0e08f # 2022.04.25
)
FetchAddSub( pfusik_qoici
	GIT_REPOSITORY https://github.com/pfusik/qoi-ci.git
	GIT_TAG d146811f3c9b9598b3bf6ab50fa5272256733b77 # 1.1.1 release.
)
FetchMakeAvail( Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG 62fd660583d3ae7a7886930b413c3c570e89786c # v2.13.9
)

AddLib( phoboslab_qoi_impl test/phoboslab_qoi.cpp phoboslab::qoi )
AddLib( pfusik_qoici_c_impl test/pfusik_qoici_c.cpp pfusik::qoici::c )
AddLib( pfusik_qoici_cpp_impl test/pfusik_qoici_cpp.cpp pfusik::qoici::cpp )

add_executable( bench test/benchmain.cpp )
target_link_libraries( bench PRIVATE
	pfusik_qoici_c_impl
	pfusik_qoici_cpp_impl
	phoboslab_qoi_impl )
target_link_libraries( bench PRIVATE Catch2::Catch2 )
