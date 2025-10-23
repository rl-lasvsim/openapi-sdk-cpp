cmake_minimum_required(VERSION 3.14)
project(lasvsim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加调试标志
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    # 对于Clang/Linux环境，可以添加LLDB特定的调试信息格式
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fstandalone-debug")
    endif()
endif()

include(GNUInstallDirs)

# 查找依赖
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# 添加库
add_library(lasvsim
    src/Client.cpp
    src/HttpClient.cpp
    src/HttpConfig.cpp
    src/ProcessTask.cpp
    src/Simulator.cpp
)

# 添加包含目录
target_include_directories(lasvsim
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 链接依赖
target_link_libraries(lasvsim
    PRIVATE
    CURL::libcurl  # 使用现代CMake目标名称
    nlohmann_json::nlohmann_json
)

# 为调试版本设置属性
target_compile_options(lasvsim PRIVATE $<$<CONFIG:Debug>:-g -O0>)
target_compile_definitions(lasvsim PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)

# 导出 target 名字空间
add_library(lasvsim::lasvsim ALIAS lasvsim)

# 安装目标和头文件
install(TARGETS lasvsim EXPORT lasvsimTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 导出 CMake 配置文件
install(EXPORT lasvsimTargets
    FILE lasvsimTargets.cmake
    NAMESPACE lasvsim::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lasvsim
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/lasvsimConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/lasvsimConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/lasvsimConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lasvsim
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lasvsimConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lasvsimConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lasvsim
)