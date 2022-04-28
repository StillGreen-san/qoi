cmake_minimum_required( VERSION 3.20 )

project( shadowmitia_libqoi CXX )

add_library( shadowmitia_libqoi src/qoi.cpp )
add_library( shadowmitia::libqoi ALIAS shadowmitia_libqoi )
target_include_directories( shadowmitia_libqoi PUBLIC include/qoi )
