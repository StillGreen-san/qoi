# based on https://github.com/cpp-best-practices/cmake_template

# treat the value of the CXX_STANDARD target property as a requirement
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# generate compile_commands.json to make it easier to work with clang based tools
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

if( ENABLE_IPO )
    # enable link time optimization
    include( CheckIPOSupported )
    check_ipo_supported( RESULT ipo_supported OUTPUT ipo_output )
    if( ipo_supported )
        set( CMAKE_INTERPROCEDURAL_OPTIMIZATION ON )
    else( )
        message( ERROR "IPO is not supported: ${ipo_output}" )
    endif( )
endif( )

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
