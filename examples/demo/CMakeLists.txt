set (DATA "${CMAKE_CURRENT_LIST_DIR}/data@/")

project (demo)
add_definitions ("-std=c++14 -Wall -Wextra")
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--embed-file ${DATA} -s TOTAL_MEMORY=67108864")

file (GLOB_RECURSE sources "**.cpp")

add_executable (demo ${sources})
set (CMAKE_EXECUTABLE_SUFFIX ".html")
set_target_properties (demo PROPERTIES OUTPUT_NAME "index")
target_link_libraries (demo matabstrix)

find_package (GLM REQUIRED)
include_directories (${GLM_INCLUDE_DIRS})
