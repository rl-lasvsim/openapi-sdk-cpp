#include "lasvsim/Error_wrapper.h"
#include "lasvsim/Error.h"
#include <cstring>
#include <algorithm>

// 使用 thread_local 替代 mutex，解决 MinGW 线程模型冲突
// 初始化使用 {0} 即可将整个结构体清零
static thread_local LasvsimErrorInfo last_error = {0};

extern "C" {

void Lasvsim_GetLastError(LasvsimErrorInfo* outInfo) {
    if (outInfo) {
        // C 风格的安全拷贝：直接按位赋值
        *outInfo = last_error;
    }
}

void Lasvsim_ClearError(void) {
    // 修复：不使用大括号赋值，改用 memset 保证 100% 兼容性
    // 这在 C/C++ 混编中是最稳定的做法
    std::memset(&last_error, 0, sizeof(LasvsimErrorInfo));
}

void Lasvsim_HandleException(const lasvsim::SDKException& e) {
    // 先清空，再赋值
    std::memset(&last_error, 0, sizeof(LasvsimErrorInfo));
    
    last_error.code = e.getCode();
    
    // 辅助安全拷贝函数：确保不溢出且末尾有 \0
    auto safe_strncpy = [](char* dest, const std::string& src, size_t size) {
        if (size == 0) return;
        size_t len = std::min(src.size(), size - 1);
        std::memcpy(dest, src.c_str(), len);
        dest[len] = '\0';
    };

    safe_strncpy(last_error.message, e.getMessage(), sizeof(last_error.message));
    safe_strncpy(last_error.reason, e.getReason(), sizeof(last_error.reason));
    safe_strncpy(last_error.url, e.getUrl(), sizeof(last_error.url));
}

}