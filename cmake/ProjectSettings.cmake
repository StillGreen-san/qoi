# based on https://github.com/cpp-best-practices/cmake_template

# generate compile_commands.json to make it easier to work with clang based tools
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

function(target_try_enable_ipo target)
	if( ENABLE_IPO )
		# enable link time optimization
		include( CheckIPOSupported )
		check_ipo_supported( RESULT ipo_supported OUTPUT ipo_output )
		if( ipo_supported )
			set_property( TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION ON )
		else( )
			message( ERROR "IPO is not supported: ${ipo_output}" )
		endif( )
	endif( )
endfunction()

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake )

# Enhance error reporting and compiler messages
if( CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" )
	add_compile_options( -fcolor-diagnostics )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	add_compile_options( -fdiagnostics-color=always )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900 )
	add_compile_options( /diagnostics:column )
else( )
	message( STATUS "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler." )
endif( )

macro( add_project_option_interface target )
	add_library( ${target} INTERFACE )
	# treat the value of the CXX_STANDARD target property as a requirement
	set_property( TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON )
	target_try_enable_include_what_you_use( ${target} )
	target_try_enable_ipo( ${target} )
	target_try_enable_clang_tidy( ${target} )
	target_try_enable_cppcheck( ${target} )
	target_try_enable_compiler_warnings( ${target} )
endmacro( )
