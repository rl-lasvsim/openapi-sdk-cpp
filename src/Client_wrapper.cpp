#include "lasvsim/Client_wrapper.h"
#include "lasvsim/Client.h"
#include "lasvsim/HttpConfig.h"

// 内部持有封装
struct Lasvsim_Client {
    std::unique_ptr<lasvsim::Client> client;
};

// 引用之前定义的 HttpConfig 结构
struct Lasvsim_HttpConfig {
    lasvsim::HttpConfig* obj;
};

extern "C" {

Lasvsim_Client* Lasvsim_Client_Create(Lasvsim_HttpConfig* config_wrapper) {
    if (!config_wrapper || !config_wrapper->obj) return nullptr;

    try {
        // 将裸指针包装成 shared_ptr (不带删除器，因为生命周期由 config_wrapper 管理)
        auto config_ptr = std::shared_ptr<lasvsim::HttpConfig>(
            config_wrapper->obj, [](lasvsim::HttpConfig*){} 
        );

        Lasvsim_Client* wrapper = new Lasvsim_Client();
        wrapper->client = std::make_unique<lasvsim::Client>(config_ptr);
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

int Lasvsim_Client_IsTaskValid(Lasvsim_Client* client) {
    if (!client || !client->client) return 0;
    
    // 调用 GetProcessTask()
    auto& task = client->client->GetProcessTask();
    return (task != nullptr) ? 1 : 0;
}

void Lasvsim_Client_Delete(Lasvsim_Client* client) {
    delete client;
}

}