### 使用方引用
cd your_project
cmake -B build -DCMAKE_INSTALL_PREFIX=/your/sdk/install/path
cmake --build build

例如：
cmake -B samples/build samples -DCMAKE_INSTALL_PREFIX=lasvsim_install
cmake --build samples/build
### sdk打包安装
// -G "MinGW Makefiles"表示使用minWG进行编译
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=lasvsim_install -DCMAKE_TOOLCHAIN_FILE=D:\app\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --target install
### 依赖安装
方式一：
sudo apt-get install -y libcurl4-openssl-dev nlohmann-json3-dev




// mac c打包命令
clang main.c -o sim_test \
    -I./include \
    -L./lib -llasvsim \
    $(curl-config --libs) \
    -lc++

// windows c打包命令，D:\app\vcpkg\packages\curl_x64-windows\lib
$CURL_LIB = "D:\app\vcpkg\packages\curl_x64-windows\lib\libcurl.lib"
$SDK_LIB = "./lib/liblasvsim.a"
$INCLUDE_DIR = "./include"
# 2. 执行编译命令
gcc main.c -o test_sdk.exe `
    -I./include `
    -DCURL_STATICLIB `
    ./lib/liblasvsim.a `
    $CURL_LIB `
    -lstdc++ -lws2_32 -lwldap32 -lcrypt32 -lnormaliz