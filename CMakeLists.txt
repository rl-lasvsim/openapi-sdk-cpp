cmake_minimum_required(VERSION 3.14)
project(lasvsim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# 查找依赖
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

file(GLOB_RECURSE SRCFILES 
    "src/*.cpp"
)

# 添加库
add_library(lasvsim ${SRCFILES})

# 添加包含目录
target_include_directories(lasvsim
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 链接依赖
target_link_libraries(lasvsim
    PRIVATE
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)

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
