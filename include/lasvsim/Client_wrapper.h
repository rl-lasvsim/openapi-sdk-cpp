#ifndef LASVSIM_CLIENT_WRAPPER_H
#define LASVSIM_CLIENT_WRAPPER_H

#include "lasvsim/HttpConfig_wrapper.h"
#include "lasvsim/Simulator_wrapper.h" // 假设已定义 Simulator 的 Wrapper

#ifdef __cplusplus
extern "C" {
#endif

// Client 不透明句柄
typedef struct Lasvsim_Client Lasvsim_Client;

/**
 * 创建 Client 实例
 */
Lasvsim_Client* Lasvsim_Client_Create(Lasvsim_HttpConfig* config_wrapper);

/**
 * 销毁 Client 实例
 */
void Lasvsim_Client_Delete(Lasvsim_Client* client);

/**
 * 获取任务状态 (示例：获取 ProcessTask 的某些属性)
 * 返回 1 表示任务有效，0 表示无效
 */
int Lasvsim_Client_IsTaskValid(Lasvsim_Client* client);

/**
 * 从配置初始化仿真器
 * 注意：这里假设 Simulator 也被包装成了指针
 */
// Lasvsim_Simulator* Lasvsim_Client_InitSimulator(Lasvsim_Client* client, Lasvsim_SimConfig* sim_cfg);

#ifdef __cplusplus
}
#endif

#endif