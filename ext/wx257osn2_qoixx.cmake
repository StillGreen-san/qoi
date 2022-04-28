cmake_minimum_required( VERSION 3.20 )

project( wx257osn2_qoixx CXX )

add_library( wx257osn2_qoixx INTERFACE )
add_library( wx257osn2::qoixx ALIAS wx257osn2_qoixx )
target_include_directories( wx257osn2_qoixx INTERFACE include )
target_compile_features( wx257osn2_qoixx INTERFACE cxx_std_20 )
