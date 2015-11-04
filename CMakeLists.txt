cmake_minimum_required (VERSION 3.0)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project (Matabstrix)
add_definitions ("-std=c++14 -Wall -Wextra")

file (GLOB sources *.cpp)

add_executable (matabstrix ${sources})
set (CMAKE_EXECUTABLE_SUFFIX ".html")
