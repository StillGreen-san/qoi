# based on https://github.com/cpp-best-practices/cmake_template

function( target_try_enable_cppcheck target )
	if( ENABLE_CPPCHECK )
		find_program( CPPCHECK cppcheck )
		if( CPPCHECK )
			if( CMAKE_GENERATOR MATCHES ".*Visual Studio.*" )
				set( CPPCHECK_TEMPLATE "vs" )
			else( )
				set( CPPCHECK_TEMPLATE "gcc" )
			endif( )
			set( CPPCHECK_OPTIONS
				 ${CPPCHECK}
				 --template=${CPPCHECK_TEMPLATE}
				 --enable=style,performance,warning,portability
				 --inline-suppr
				 # We cannot act on a bug/missing feature of cppcheck
				 --suppress=cppcheckError
				 --suppress=internalAstError
				 # if a file does not have an internalAstError, we get an unmatchedSuppression error
				 --suppress=unmatchedSuppression
				 # noisy and incorrect sometimes
				 --suppress=passedByValue
				 # ignores code that cppcheck thinks is invalid C++
				 --suppress=syntaxError
				 --suppress=preprocessorErrorDirective
				 --inconclusive )
			set_property( TARGET ${target} PROPERTY CXX_CPPCHECK ${CPPCHECK_OPTIONS} )
		else( )
			message( SEND_ERROR "cppcheck requested but executable not found" )
		endif( )
	endif( )
endfunction()

function( target_try_enable_clang_tidy target )
	if( ENABLE_CLANG_TIDY )
		find_program( CLANGTIDY clang-tidy )
		if( CLANGTIDY )
			set( CLANG_TIDY_OPTIONS
				 ${CLANGTIDY}
				 -extra-arg=-Wno-unknown-warning-option
				 -extra-arg=-Wno-ignored-optimization-argument
				 -extra-arg=-Wno-unused-command-line-argument
				 -p )
			if( NOT "${CMAKE_CXX_STANDARD}" STREQUAL "" )
				if( "${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl" )
					set( CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD} )
				else( )
					set( CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD} )
				endif( )
			endif( )
			set_property( TARGET ${target} PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS} )
		else( )
			message( SEND_ERROR "clang-tidy requested but executable not found" )
		endif( )
	endif( )
endfunction()

function( target_try_enable_include_what_you_use target )
	if( ENABLE_INCLUDE_WHAT_YOU_USE )
		find_program( INCLUDE_WHAT_YOU_USE include-what-you-use )
		if( INCLUDE_WHAT_YOU_USE )
			set_property( TARGET ${target} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE} )
		else( )
			message( SEND_ERROR "include-what-you-use requested but executable not found" )
		endif( )
	endif( )
endfunction()
