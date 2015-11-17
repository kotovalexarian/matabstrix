cmake_minimum_required (VERSION 3.0)

project (libmatabstrix)
add_definitions ("-std=c++14 -Wall -Wextra")

file (GLOB_RECURSE sources  "**.cpp")

add_library (matabstrix STATIC ${sources})

find_package (GLM REQUIRED)
include_directories (${GLM_INCLUDE_DIRS})

find_package (STB REQUIRED)
include_directories (${STB_INCLUDE_DIRS})
