#include "lasvsim/Error_wrapper.h"
#include "lasvsim/Error.h" // 包含你提供的原始头文件
#include <cstring>
#include <mutex>

// 使用线程局部变量存储错误，确保多线程安全
static thread_local LasvsimErrorInfo last_error = {0, "", "", ""};
static std::mutex error_mutex;

extern "C" {

void Lasvsim_GetLastError(LasvsimErrorInfo* outInfo) {
    if (outInfo) {
        std::lock_guard<std::mutex> lock(error_mutex);
        *outInfo = last_error;
    }
}

void Lasvsim_ClearError(void) {
    std::lock_guard<std::mutex> lock(error_mutex);
    last_error.code = 0;
    std::memset(last_error.message, 0, sizeof(last_error.message));
    std::memset(last_error.reason, 0, sizeof(last_error.reason));
    std::memset(last_error.url, 0, sizeof(last_error.url));
}

// 这是一个内部辅助函数，供其他 C++ Wrapper 调用
void Lasvsim_HandleException(const lasvsim::SDKException& e) {
    std::lock_guard<std::mutex> lock(error_mutex);
    last_error.code = e.getCode();
    
    // 安全拷贝字符串到 C 字符数组
    std::strncpy(last_error.message, e.getMessage().c_str(), sizeof(last_error.message) - 1);
    std::strncpy(last_error.reason, e.getReason().c_str(), sizeof(last_error.reason) - 1);
    std::strncpy(last_error.url, e.getUrl().c_str(), sizeof(last_error.url) - 1);
}

}