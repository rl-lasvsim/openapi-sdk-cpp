#include "lasvsim/HttpConfig_wrapper.h"
#include "lasvsim/HttpConfig.h" // SDK 原始头文件
#include <string>

// 实际的结构体定义只在 .cpp 中可见
struct Lasvsim_HttpConfig {
    lasvsim::HttpConfig* obj;
};

extern "C" {

Lasvsim_HttpConfig* Lasvsim_HttpConfig_Create(const char* endpoint, const char* token) {
    if (endpoint == nullptr || token == nullptr) return nullptr;
    
    Lasvsim_HttpConfig* wrapper = new Lasvsim_HttpConfig();
    // 显式调用 C++ 构造函数，std::string 会处理字符拷贝
    wrapper->obj = new lasvsim::HttpConfig(std::string(endpoint), std::string(token));
    return wrapper;
}

void Lasvsim_HttpConfig_Delete(Lasvsim_HttpConfig* config) {
    if (config) {
        delete config->obj;
        delete config;
    }
}

const char* Lasvsim_HttpConfig_GetEndpoint(Lasvsim_HttpConfig* config) {
    if (config && config->obj) {
        // 返回 std::string 的内部指针。注意：生命周期由 config 对象管理
        return config->obj->GetEndpoint().c_str();
    }
    return nullptr;
}

const char* Lasvsim_HttpConfig_GetToken(Lasvsim_HttpConfig* config) {
    if (config && config->obj) {
        return config->obj->GetToken().c_str();
    }
    return nullptr;
}

}