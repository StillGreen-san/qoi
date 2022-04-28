cmake_minimum_required( VERSION 3.20 )

project( phoboslab_qoi C )

add_library( phoboslab_qoi phoboslab_qoi.c )
add_library( phoboslab::qoi ALIAS phoboslab_qoi )
target_include_directories( phoboslab_qoi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
