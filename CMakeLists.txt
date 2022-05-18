cmake_minimum_required( VERSION 3.20 )

project( qoi CXX C )
# TODO make tests & benchmark optional
# TODO check if cxx_std can be lowered

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
macro( PostBuildImageCopy #[[LibTarget ImageTarget]])
	add_custom_command(
		TARGET ${ARGV0} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/wikipedia_008.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/dice.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/kodim10.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/kodim23.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/qoi_logo.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/testcard.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${${ARGV1}_SOURCE_DIR}/testcard_rgba.qoi"
			$<TARGET_FILE_DIR:${ARGV0}>
	)
endmacro()

FetchAddSub( phoboslab_qoi
	GIT_REPOSITORY https://github.com/phoboslab/qoi.git
	GIT_TAG 6d7eadd28c0be90e36a853a7bbb1782e50a0e08f # 2022.04.25
)
FetchAddSub( pfusik_qoici
	GIT_REPOSITORY https://github.com/pfusik/qoi-ci.git
	GIT_TAG d146811f3c9b9598b3bf6ab50fa5272256733b77 # 1.1.1 release.
)
FetchAddSub( shadowmitia_libqoi
	GIT_REPOSITORY https://github.com/ShadowMitia/libqoi.git
	GIT_TAG cc2b8967a9d2c4754e35d67d9ee9d9613e7cbbe0 # 2022.01.13
)
FetchAddSub( wx257osn2_qoixx
	GIT_REPOSITORY https://github.com/wx257osn2/qoixx.git
	GIT_TAG 72cbaa530df7c7bdecf8b13b49060f485c8c661b # 2022.02.23
)
#FetchAddSub( shraiwi_miniqoi
#	GIT_REPOSITORY https://github.com/shraiwi/mini-qoi.git
#	GIT_TAG 281ffd0ee20a4d85cad4b156be07181a6e8b3a60 # v1.0.0
#)
FetchMakeAvail( Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG 62fd660583d3ae7a7886930b413c3c570e89786c # v2.13.9
)
FetchAddSub( qoi_test_images
	URL https://qoiformat.org/qoi_test_images.zip
)

AddLib( phoboslab_qoi_impl test/phoboslab_qoi.cpp phoboslab::qoi )
AddLib( pfusik_qoici_c_impl test/pfusik_qoici_c.cpp pfusik::qoici::c )
AddLib( pfusik_qoici_cpp_impl test/pfusik_qoici_cpp.cpp pfusik::qoici::cpp )
AddLib( shadowmitia_libqoi_impl test/shadowmitia_libqoi.cpp shadowmitia::libqoi )
AddLib( wx257osn2_qoixx_impl test/wx257osn2_qoixx.cpp wx257osn2::qoixx )
#AddLib( shraiwi_miniqoi_c_impl test/shraiwi_miniqoi_c.cpp shraiwi::miniqoi::c )
#AddLib( shraiwi_miniqoi_cpp_impl test/shraiwi_miniqoi_cpp.cpp shraiwi::miniqoi::cpp )
# TODO unresolved symbols mqoi_desc_done, mqoi_dec_done & __builtin_bswap32

add_library( sgs_qoi INTERFACE )
add_library( sgs::qoi ALIAS sgs_qoi )
target_compile_features( sgs_qoi INTERFACE cxx_std_17 )
target_compile_definitions( sgs_qoi INTERFACE $<$<STREQUAL:${CMAKE_CXX_BYTE_ORDER},BIG_ENDIAN>:CMAKE_CXX_BIG_ENDIAN> )
target_include_directories( sgs_qoi INTERFACE lib )

AddLib( stillgreensan_qoi_impl test/stillgreensan_qoi.cpp sgs::qoi )

AddLib( test_data test/testdata.cpp phoboslab_qoi_impl )

add_executable( sgs_qoi_test test/catchmain.cpp test/testmain.cpp )
target_link_libraries( sgs_qoi_test PRIVATE sgs::qoi test_data Catch2::Catch2)
PostBuildImageCopy( sgs_qoi_test qoi_test_images )

add_executable( bench test/catchmain.cpp test/benchmain.cpp )
target_link_libraries( bench PRIVATE
	stillgreensan_qoi_impl
#	shraiwi_miniqoi_c_impl
#	shraiwi_miniqoi_cpp_impl
	wx257osn2_qoixx_impl
	shadowmitia_libqoi_impl
	pfusik_qoici_c_impl
	pfusik_qoici_cpp_impl
	phoboslab_qoi_impl )
target_link_libraries( bench PRIVATE Catch2::Catch2 qoi_test_images test_data )
PostBuildImageCopy( bench qoi_test_images )