#ifndef LASVSIM_SIMULATOR_WRAPPER_H
#define LASVSIM_SIMULATOR_WRAPPER_H

#include "lasvsim/HttpClient_wrapper.h" // 引用之前定义的 HttpConfig 指针


#ifdef __cplusplus
extern "C" {
#endif

// 不透明句柄
typedef struct Lasvsim_SimulatorConfig Lasvsim_SimulatorConfig;
typedef struct Lasvsim_Simulator Lasvsim_Simulator;

// 1. SimulatorConfig 接口
Lasvsim_SimulatorConfig* Lasvsim_SimConfig_Create(const char* scenId, const char* scenVer, const char* simRecordId, int maxStep);
void Lasvsim_SimConfig_Delete(Lasvsim_SimulatorConfig* cfg);
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

// 设置车辆位置
void Lasvsim_Simulator_SetVehiclePosition(Lasvsim_Simulator* sim, const char* vehicle_id, double x, double y,double z, double heading);

typedef struct {
    char** ids;      // Array of string pointers
    int count;       // Number of IDs in the array
} Lasvsim_VehicleIdList;

// 获取车辆id列表 std::vector<std::string> GetVehicleIdList();
int Lasvsim_Simulator_GetVehicleIdList(Lasvsim_Simulator* sim,char **outIds);

// C语言兼容的结构体
typedef struct {
    double fl_torque;  // 左前轮扭矩, 单位:[N*m]
    double fr_torque;  // 右前轮扭矩, 单位:[N*m]
    double lon_acc;    // 纵向加速度, 单位:[m/s^2]
    double rl_torque;  // 左后轮扭矩, 单位:[N*m]
    double rr_torque;  // 右后轮扭矩, 单位:[N*m]
    double ste_wheel;  // 方向盘转角(逆时针为正),单位:[rad(弧度)]
} Lasvsim_ControlInfo;

int Lasvsim_Simulator_GetVehicleControlInfo(Lasvsim_Simulator* sim,const char* vehicle_id, Lasvsim_ControlInfo* out_control_info);

#ifdef __cplusplus
}
#endif

#endif