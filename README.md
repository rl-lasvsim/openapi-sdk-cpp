### 使用方引用
cd your_project
cmake -B build -DCMAKE_INSTALL_PREFIX=/your/sdk/install/path
cmake --build build

cmake例子：cmake -B build -DCMAKE_PREFIX_PATH="/Users/sq_luozhihong/data/source/github_rl/openapi-sdk-cpp/lasvsim_install"
### sdk打包安装
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=lasvsim_install
cmake --build build --target install
