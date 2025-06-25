cmake_minimum_required(VERSION 3.10)
project(LasvsimClientDemo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 查找依赖
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)

# 添加可执行文件
add_executable(demo
    src/main.cpp
    src/Client.cpp
    src/ProcessTask.cpp
    src/HttpClient.cpp
    src/HttpConfig.cpp
)

# 包含目录
target_include_directories(demo PRIVATE include)

# 链接库
target_link_libraries(demo
    PRIVATE 
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)