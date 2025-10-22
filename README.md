### 使用方引用
cd your_project
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/your/sdk/install/path/lib/cmake/lasvsim
make

cmake例子：cmake ../ -DCMAKE_PREFIX_PATH="/Users/sq_luozhihong/data/source/github_rl/openapi-sdk-cpp/lasvsim_install/lib/cmake/lasvsim"
### sdk打包安装
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=lasvsim_install
cmake --build build --target install
