#ifndef LASVSIM_SIMULATOR_WRAPPER_H
#define LASVSIM_SIMULATOR_WRAPPER_H

#include "lasvsim/HpptClient_wrapper.h" // 引用之前定义的 HttpConfig 指针


#ifdef __cplusplus
extern "C" {
#endif

// 不透明句柄
typedef struct Lasvsim_SimulatorConfig Lasvsim_SimulatorConfig;
typedef struct Lasvsim_Simulator Lasvsim_Simulator;

// 1. SimulatorConfig 接口
Lasvsim_SimulatorConfig* Lasvsim_SimConfig_Create(const char* scenId, const char* scenVer, const char* simRecordId, int maxStep);
void Lasvsim_SimConfig_Delete(Lasvsim_SimulatorConfig* cfg);

// 2. Simulator 核心接口
Lasvsim_Simulator* Lasvsim_Simulator_Create(Lasvsim_HttpClient* client, Lasvsim_SimulatorConfig* cfg);
void Lasvsim_Simulator_Delete(Lasvsim_Simulator* sim);

int Lasvsim_Simulator_Step(Lasvsim_Simulator* sim); // 返回 StepCode (0: RUNNING, 1001: FINISHED, 1002: FAILED)
void Lasvsim_Simulator_Stop(Lasvsim_Simulator* sim);

// 3. 车辆控制接口
void Lasvsim_Simulator_SetControl(Lasvsim_Simulator* sim, const char* vehicle_id, double lon_acc, double ste_wheel);

// 4. 获取车辆位置 (简化版：只获取 X, Y, Heading)
typedef struct {
    double x, y, heading;
    int type; // PositionType
} Lasvsim_SimplePos;

int Lasvsim_Simulator_GetVehiclePos(Lasvsim_Simulator* sim, const char* vehicle_id, Lasvsim_SimplePos* outPos);

#ifdef __cplusplus
}
#endif

#endif