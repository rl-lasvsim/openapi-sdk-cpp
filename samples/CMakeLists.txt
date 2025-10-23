cmake_minimum_required(VERSION 3.14)
project(sample_main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置默认构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 调试配置
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fstandalone-debug")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX flags debug: ${CMAKE_CXX_FLAGS_DEBUG}")

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(lasvsim REQUIRED)

add_executable(sample_main main.cpp)
target_link_libraries(sample_main PRIVATE lasvsim::lasvsim)

# 为 Debug 构建明确设置选项
target_compile_options(sample_main PRIVATE 
    $<$<CONFIG:Debug>:-g -O0 -fstandalone-debug>
)

get_target_property(_inc_dirs lasvsim::lasvsim INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "lasvsim include dirs: ${_inc_dirs}")