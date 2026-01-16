#ifndef LASVSIM_CONFIG_WRAPPER_H
#define LASVSIM_CONFIG_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// 定义一个不透明指针，代表 C++ 中的 HttpConfig 实例
typedef struct Lasvsim_HttpConfig Lasvsim_HttpConfig;

// 创建配置对象 (对应 C++ 构造函数)
Lasvsim_HttpConfig* Lasvsim_HttpConfig_Create(const char* endpoint, const char* token);

// 销毁配置对象 (防止内存泄漏)
void Lasvsim_HttpConfig_Delete(Lasvsim_HttpConfig* config);

// 获取配置信息
const char* Lasvsim_HttpConfig_GetEndpoint(Lasvsim_HttpConfig* config);
const char* Lasvsim_HttpConfig_GetToken(Lasvsim_HttpConfig* config);

#ifdef __cplusplus
}
#endif

#endif