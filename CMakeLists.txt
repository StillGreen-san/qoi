cmake_minimum_required( VERSION 3.20 )
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set( PROJECT_NAMESPACE sgs )
project( qoi
         VERSION 0.5.3
         HOMEPAGE_URL "https://github.com/StillGreen-san/qoi"
         LANGUAGES CXX )
set( PROJECT_QUALIFIED_NAME ${PROJECT_NAMESPACE}${PROJECT_NAME} )
set( PROJECT_IMPORTED_NAME ${PROJECT_NAMESPACE}::${PROJECT_NAME} )

option( ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF )
option( ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF )
option( ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF )

option( ENABLE_IPO "enable link time optimization" ON )
option( ENABLE_TESTS "build tests" OFF )
option( ENABLE_BENCHMARK "build benchmark" OFF )

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectSettings.cmake )
add_project_option_interface( project_options )

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
macro( AddLib #[[LibName LibSource LinkLibName]] )
    add_library( ${ARGV0} ${ARGV1} )
    target_link_libraries( ${ARGV0} PRIVATE ${ARGV2} )
    target_compile_features( ${ARGV0} PUBLIC cxx_std_17 )
endmacro()
macro( PostBuildImageCopy #[[LibTarget ImageTarget]] )
    add_custom_command(
        TARGET ${ARGV0} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${${ARGV1}_SOURCE_DIR}/wikipedia_008.qoi"
            "${${ARGV1}_SOURCE_DIR}/dice.qoi"
            "${${ARGV1}_SOURCE_DIR}/kodim10.qoi"
            "${${ARGV1}_SOURCE_DIR}/kodim23.qoi"
            "${${ARGV1}_SOURCE_DIR}/qoi_logo.qoi"
            "${${ARGV1}_SOURCE_DIR}/testcard.qoi"
            "${${ARGV1}_SOURCE_DIR}/testcard_rgba.qoi"
            $<TARGET_FILE_DIR:${ARGV0}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/diff.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/luma.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/rgb.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/rgba.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/rgbidx.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/run.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/rng01.qoi"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/qoi/rng02.qoi"
            $<TARGET_FILE_DIR:${ARGV0}>
    )
endmacro()

if( ENABLE_TESTS OR ENABLE_BENCHMARK )
    FetchAddSub( phoboslab_qoi
        GIT_REPOSITORY https://github.com/phoboslab/qoi.git
        GIT_TAG 6d7eadd28c0be90e36a853a7bbb1782e50a0e08f # 2022.04.25
    )
    FetchMakeAvail( Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 62fd660583d3ae7a7886930b413c3c570e89786c # v2.13.9
    )
    FetchAddSub( qoi_test_images
        URL https://qoiformat.org/qoi_test_images.zip
    )
endif()
if( ENABLE_BENCHMARK )
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
    FetchAddSub( shraiwi_miniqoi
        GIT_REPOSITORY https://github.com/shraiwi/mini-qoi.git
        GIT_TAG 40c43fec296a1ff10b4ea4a7c502de14c6d9745d # 2022.06.01
    )
endif()

include( GNUInstallDirs )

add_library( ${PROJECT_NAME} INTERFACE )
add_library( ${PROJECT_IMPORTED_NAME} ALIAS ${PROJECT_NAME} )
target_compile_features( ${PROJECT_NAME} INTERFACE cxx_std_17 )
target_include_directories( ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
set_target_properties( ${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION} )

if( ENABLE_TESTS OR ENABLE_BENCHMARK )
    AddLib( phoboslab_qoi_impl test/phoboslab_qoi.cpp phoboslab::qoi )
    AddLib( test_data test/testdata.cpp phoboslab_qoi_impl )
endif()
if( ENABLE_BENCHMARK )
    AddLib( stillgreensan_qoi_impl test/stillgreensan_qoi.cpp ${PROJECT_NAME} )
    AddLib( pfusik_qoici_c_impl test/pfusik_qoici_c.cpp pfusik::qoici::c )
    AddLib( pfusik_qoici_cpp_impl test/pfusik_qoici_cpp.cpp pfusik::qoici::cpp )
    AddLib( shadowmitia_libqoi_impl test/shadowmitia_libqoi.cpp shadowmitia::libqoi )
    AddLib( wx257osn2_qoixx_impl test/wx257osn2_qoixx.cpp wx257osn2::qoixx )
    AddLib( shraiwi_miniqoi_c_impl test/shraiwi_miniqoi_c.cpp shraiwi::miniqoi::c )
    AddLib( shraiwi_miniqoi_cpp_impl test/shraiwi_miniqoi_cpp.cpp shraiwi::miniqoi::cpp )
endif()

if( ENABLE_TESTS )
    add_executable( ${PROJECT_NAME}_test test/catchmain.cpp test/testmain.cpp )
    target_link_libraries( ${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} test_data Catch2::Catch2 project_options )
    PostBuildImageCopy( ${PROJECT_NAME}_test qoi_test_images )
endif()

if( ENABLE_BENCHMARK )
    add_executable( bench test/catchmain.cpp test/benchmain.cpp )
    target_link_libraries( bench PRIVATE
        stillgreensan_qoi_impl
        shraiwi_miniqoi_c_impl shraiwi_miniqoi_cpp_impl
        wx257osn2_qoixx_impl
        shadowmitia_libqoi_impl
        pfusik_qoici_c_impl pfusik_qoici_cpp_impl
        phoboslab_qoi_impl )
    target_link_libraries( bench PRIVATE Catch2::Catch2 qoi_test_images test_data project_options )
    PostBuildImageCopy( bench qoi_test_images )
endif()

include( CMakePackageConfigHelpers )

configure_file( ${PROJECT_SOURCE_DIR}/cmake/pkg-config.pc.in ${PROJECT_BINARY_DIR}/${PROJECT_QUALIFIED_NAME}.pc @ONLY )
configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
        ${PROJECT_BINARY_DIR}/${PROJECT_QUALIFIED_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_QUALIFIED_NAME}
        NO_SET_AND_CHECK_MACRO )
write_basic_package_version_file( ${PROJECT_QUALIFIED_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion )
configure_file( ${PROJECT_SOURCE_DIR}/LICENCE.MD ${PROJECT_BINARY_DIR}/copyright COPYONLY )

install( TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_QUALIFIED_NAME}_Targets )
install( EXPORT ${PROJECT_QUALIFIED_NAME}_Targets
         FILE ${PROJECT_QUALIFIED_NAME}Targets.cmake
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_QUALIFIED_NAME}
         NAMESPACE ${PROJECT_NAMESPACE}:: )

install( DIRECTORY ${PROJECT_SOURCE_DIR}/inc/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
install( FILES ${PROJECT_BINARY_DIR}/${PROJECT_QUALIFIED_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig )
install( FILES
         ${PROJECT_BINARY_DIR}/${PROJECT_QUALIFIED_NAME}Config.cmake
         ${PROJECT_BINARY_DIR}/${PROJECT_QUALIFIED_NAME}ConfigVersion.cmake
         ${PROJECT_BINARY_DIR}/copyright
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_QUALIFIED_NAME} )
