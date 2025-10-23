cmake_minimum_required(VERSION 3.14)
project(sample_main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 只写 find_package，不写路径
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(lasvsim REQUIRED)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fstandalone-debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
endif()

add_executable(sample_main main.cpp)
target_link_libraries(sample_main PRIVATE lasvsim::lasvsim)

# 可选调试输出
get_target_property(_inc_dirs lasvsim::lasvsim INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "lasvsim include dirs: ${_inc_dirs}")
