cmake_minimum_required(VERSION 2.8)
project( swooce )
set(OpenCV_STATIC ON)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( swooce swooce.cpp )
target_link_libraries( swooce ${OpenCV_LIBS} )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fopenmp -Ofast" )
# Use --as-needed to fix opencv's crap ton of useless (to us) libraries
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")