### 使用方引用
cd your_project
cmake -B build -DCMAKE_INSTALL_PREFIX=/your/sdk/install/path
cmake --build build

例如：
cmake -B samples/build samples -DCMAKE_INSTALL_PREFIX=lasvsim_install
cmake --build samples/build
### sdk打包安装
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=lasvsim_install
cmake --build build --target install
### 依赖安装
方式一：
sudo apt-get install -y libcurl4-openssl-dev nlohmann-json3-dev