#include "lasvsim/ProcessTask_wrapper.h"
#include "lasvsim/Client_wrapper.h"
#include "lasvsim/Client.h"
#include "lasvsim/HttpConfig.h"
#include "Simulator_wrapper.cpp"

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
    } catch (const std::exception& e) {
        fprintf(stderr, "ERROR: Exception caught in Lasvsim_Client_Create: %s\n", e.what());
        return nullptr; // 失败
    } catch (...) {
        fprintf(stderr, "ERROR: Unknown exception caught in Lasvsim_Client_Create\n");
        return nullptr; // 失败
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

// 获取ProcessTask对象
Lasvsim_ProcessTask* Lasvsim_Client_GetProcessTask(Lasvsim_Client* client) {
    if (!client || !client->client) {
        return nullptr;
    }
    
    try {
        auto& process_task_ref = client->client->GetProcessTask();
        if (!process_task_ref) {
            return nullptr;
        }
        
        // 创建ProcessTask包装器
        Lasvsim_ProcessTask* task_wrapper = Lasvsim_ProcessTask_Create(&process_task_ref);
        
        return task_wrapper;
    } catch (const std::exception& e) {
        fprintf(stderr, "Failed to get process task: %s\n", e.what());
        return nullptr;
    }
}

// Simulator 构造
Lasvsim_Simulator* Lasvsim_Simulator_Create_From_Client(Lasvsim_Client* client_wrap, Lasvsim_SimulatorConfig* cfg_wrap) {
    if (!client_wrap || !cfg_wrap) return nullptr;
    try {
        auto wrapper = new Lasvsim_Simulator();

        lasvsim::Simulator simor = client_wrap->client->InitSimulatorFromConfig(cfg_wrap->obj);

        // 这里的构造需要 SDK 支持传入 shared_ptr
        wrapper->obj = std::make_unique<lasvsim::Simulator>(simor);
        return wrapper;
    } catch (...) { return nullptr; }
}

}