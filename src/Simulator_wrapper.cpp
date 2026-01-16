#include "lasvsim/Simulator_wrapper.h"
#include "lasvsim/Simulator.h"
#include <memory>

// 内部结构体定义
struct Lasvsim_SimulatorConfig {
    std::shared_ptr<lasvsim::SimulatorConfig> obj;
};

struct Lasvsim_Simulator {
    std::unique_ptr<lasvsim::Simulator> obj;
};

// 引用 HttpClient 的内部定义
struct Lasvsim_HttpClient {
    std::shared_ptr<lasvsim::HttpClient> client;
};

extern "C" {

// Config 构造
Lasvsim_SimulatorConfig* Lasvsim_SimConfig_Create(const char* scenId, const char* scenVer, const char* simRecordId, int maxStep) {
    auto wrapper = new Lasvsim_SimulatorConfig();
    wrapper->obj = std::make_shared<lasvsim::SimulatorConfig>(scenId, scenVer, simRecordId, maxStep);
    return wrapper;
}

// Simulator 构造
Lasvsim_Simulator* Lasvsim_Simulator_Create(Lasvsim_HttpClient* client_wrap, Lasvsim_SimulatorConfig* cfg_wrap) {
    if (!client_wrap || !cfg_wrap) return nullptr;
    try {
        auto wrapper = new Lasvsim_Simulator();
        // 这里的构造需要 SDK 支持传入 shared_ptr
        wrapper->obj = std::make_unique<lasvsim::Simulator>(client_wrap->client, cfg_wrap->obj);
        return wrapper;
    } catch (...) { return nullptr; }
}

int Lasvsim_Simulator_Step(Lasvsim_Simulator* sim) {
    if (!sim) return 1002;
    auto res = sim->obj->Step();
    return static_cast<int>(res.code);
}

void Lasvsim_Simulator_SetControl(Lasvsim_Simulator* sim, const char* vehicle_id, double lon_acc, double ste_wheel) {
    if (sim && vehicle_id) {
        sim->obj->SetVehicleControlParam(vehicle_id, lon_acc, ste_wheel);
    }
}

int Lasvsim_Simulator_GetVehiclePos(Lasvsim_Simulator* sim, const char* vehicle_id, Lasvsim_SimplePos* outPos) {
    if (!sim || !vehicle_id || !outPos) return 0;
    
    std::vector<std::string> ids = { vehicle_id };
    auto posMap = sim->obj->GetVehiclesPosition(ids);
    
    auto it = posMap.find(vehicle_id);
    if (it != posMap.end()) {
        const auto& p = it->second; // it->second 直接指向 Position 对象
        outPos->x = p.point.x;
        outPos->y = p.point.y;
        outPos->heading = p.heading;
        outPos->type = static_cast<int>(p.type);
        return 1;
    }
    
    return 0;
}

void Lasvsim_Simulator_Delete(Lasvsim_Simulator* sim) { delete sim; }
void Lasvsim_SimConfig_Delete(Lasvsim_SimulatorConfig* cfg) { delete cfg; }

}