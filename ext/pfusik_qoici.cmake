cmake_minimum_required( VERSION 3.20 )

project( pfusik_qoici C CXX )

add_library( pfusik_qoici_c transpiled/QOI.c )
add_library( pfusik::qoici::c ALIAS pfusik_qoici_c )
target_include_directories( pfusik_qoici_c PUBLIC transpiled )

add_library( pfusik_qoici_cpp transpiled/QOI.cpp )
add_library( pfusik::qoici::cpp ALIAS pfusik_qoici_cpp )
target_include_directories( pfusik_qoici_cpp PUBLIC transpiled )
