#include "lasvsim/HttpClient_wrapper.h"
#include "lasvsim/HttpClient.h"
#include "lasvsim/HttpConfig.h"
#include <map>
#include <string>
#include <memory>

// 内部结构体定义
struct Lasvsim_HttpClient {
    std::unique_ptr<lasvsim::HttpClient> client;
    std::string last_response; // 用于保存返回结果给 C 侧读取
};

// 假设我们在 lasvsim_config_wrapper.cpp 中增加一个内部接口来获取裸指针
// 或者直接在 cpp 里通过 struct 访问
struct Lasvsim_HttpConfig {
    lasvsim::HttpConfig* obj;
};

extern "C" {

Lasvsim_HttpClient* Lasvsim_CurlClient_Create(Lasvsim_HttpConfig* config_wrapper) {
    if (!config_wrapper || !config_wrapper->obj) return nullptr;

    try {
        Lasvsim_HttpClient* wrapper = new Lasvsim_HttpClient();
        
        // 将裸指针包装进 shared_ptr (注意：这里需确保 config 的生命周期)
        // 更好的做法是 SDK 提供直接获取 shared_ptr 的接口
        auto config_ptr = std::shared_ptr<lasvsim::HttpConfig>(
            config_wrapper->obj, [](lasvsim::HttpConfig*){} // 空删除器，防止重复释放
        );

        std::map<std::string, std::string> empty_headers;
        wrapper->client = std::make_unique<lasvsim::CurlHttpClient>(config_ptr, empty_headers);
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

void Lasvsim_HttpClient_AddHeader(Lasvsim_HttpClient* client, const char* key, const char* value) {
    if (client && client->client && key && value) {
        client->client->AddHeader(key, value);
    }
}

const char* Lasvsim_HttpClient_Post(Lasvsim_HttpClient* client, const char* url, const char* body) {
    if (!client || !client->client || !url) return nullptr;

    try {
        // 调用 SDK 的 Post 方法
        client->last_response = client->client->Post(url, body ? body : "");
        return client->last_response.c_str();
    } catch (const std::exception& e) {
        // 1. 将错误信息保存到上次错误变量中（假设你有个全局或 client 相关的 last_error）
        // 这里可以调用你之前定义的 Error_wrapper 接口
        // 2. 直接打印日志
        fprintf(stderr, "[Lasvsim Exception] POST to %s failed: %s\n", url, e.what());
        // 此处可以配合上一步的 Error Wrapper 记录错误
        client->last_response = "";
        return nullptr;
    }
}

void Lasvsim_HttpClient_Delete(Lasvsim_HttpClient* client) {
    delete client;
}

}