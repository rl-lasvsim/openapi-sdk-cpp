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

void Lasvsim_SimConfig_Delete(Lasvsim_SimulatorConfig* cfg) { delete cfg; }

void Lasvsim_Simulator_Delete(Lasvsim_Simulator* sim) { delete sim; }

int Lasvsim_Simulator_Step(Lasvsim_Simulator* sim) {
    if (!sim) return 1002;
    auto res = sim->obj->Step();
    return static_cast<int>(res.code);
}

void Lasvsim_Simulator_Stop(Lasvsim_Simulator* sim) {
    if (sim) sim->obj->Stop();
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


void Lasvsim_Simulator_SetVehiclePosition(Lasvsim_Simulator* sim, const char* vehicle_id, double x, double y,double z, double heading) {
    if (sim && vehicle_id) {
        lasvsim::Point p(x, y, z);
        sim->obj->SetVehiclePosition(vehicle_id, heading,p);
    }
}

int Lasvsim_Simulator_GetVehicleIdList(Lasvsim_Simulator* sim,char **outIds)  {
    if (!sim || !outIds) {
        return -1; // Error: invalid input parameters
    }
    

    try {
        std::vector<std::string> ids = sim->obj->GetVehicleIdList();
        size_t count = ids.size();
        
        if (count == 0) {
            *outIds = nullptr;
            return 0; 
        }
        
        char** result = (char**)malloc(sizeof(char*) * (count + 1));
        
        if (!result) {
            return -1; 
        }
        
        for (size_t i = 0; i < count; ++i) {
            result[i] = (char*)malloc(sizeof(char) * (ids[i].length() + 1));
            if (!result[i]) {
                for (size_t j = 0; j < i; ++j) {
                    free(result[j]);
                }
                free(result);
                return -1; 
            }
            strcpy(result[i], ids[i].c_str());
        }
        
        result[count] = nullptr;
        
        *outIds = result[0]; 
        *outIds = (char*)result;
        
        return 0;
    } catch (...) {
        return -1; // Error occurred
    }
}

int Lasvsim_Simulator_GetVehicleControlInfo(Lasvsim_Simulator* sim,const char* vehicle_id, Lasvsim_ControlInfo* out_control_info){
    if (!vehicle_id || !out_control_info) {
        return -1;
    }
    
    try {
        // 调用原始的C++函数
        std::string vehicle_id_str(vehicle_id);
        lasvsim::ControlInfo cpp_result = sim->obj->GetVehicleControlInfo(vehicle_id_str);
        
        if (!out_control_info) {
            return -1;
        }
    
        // 将C++结果复制到C结构体
        out_control_info->fl_torque = cpp_result.fl_torque;
        out_control_info->fr_torque = cpp_result.fr_torque;
        out_control_info->lon_acc = cpp_result.lon_acc;
        out_control_info->rl_torque = cpp_result.rl_torque;
        out_control_info->rr_torque = cpp_result.rr_torque;
        out_control_info->ste_wheel = cpp_result.ste_wheel;
    } catch (const std::exception& e) {
        fprintf(stderr, "ERROR: Exception caught in Lasvsim_Simulator_GetVehicleControlInfo: %s\n", e.what());
        return -2; // 失败
    } catch (...) {
        fprintf(stderr, "ERROR: Unknown exception caught in Lasvsim_Simulator_GetVehicleControlInfo \n");
        return -2; // 失败
    }
    return 0;
}

}