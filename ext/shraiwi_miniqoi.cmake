cmake_minimum_required( VERSION 3.20 )

project( shraiwi_miniqoi C CXX )

add_library( shraiwi_miniqoi_c src/mini_qoi.c )
add_library( shraiwi::miniqoi::c ALIAS shraiwi_miniqoi_c )
target_include_directories( shraiwi_miniqoi_c PUBLIC src )

add_library( shraiwi_miniqoi_cpp src/MiniQOI.cpp )
add_library( shraiwi::miniqoi::cpp ALIAS shraiwi_miniqoi_cpp )
target_include_directories( shraiwi_miniqoi_cpp PUBLIC src )
