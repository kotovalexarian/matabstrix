cmake_minimum_required (VERSION 3.0)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

project (Matabstrix)
add_definitions ("-std=c++14 -Wall -Wextra")
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--use-preload-plugins --preload-file data")

file (GLOB_RECURSE sources "src/**.cpp"
                           "examples/**.cpp")

add_executable (matabstrix ${sources})
set (CMAKE_EXECUTABLE_SUFFIX ".html")
set_target_properties (matabstrix PROPERTIES OUTPUT_NAME "index")

find_package (GLM REQUIRED)
include_directories (${GLM_INCLUDE_DIRS})
