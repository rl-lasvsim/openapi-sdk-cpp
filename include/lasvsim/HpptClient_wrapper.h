#ifndef LASVSIM_HTTP_CLIENT_WRAPPER_H
#define LASVSIM_HTTP_CLIENT_WRAPPER_H

#include "lasvsim/HttpConfig_wrapper.h" // 引用之前定义的 HttpConfig 指针

#ifdef __cplusplus
extern "C" {
#endif

// HttpClient 的不透明句柄
typedef struct Lasvsim_HttpClient Lasvsim_HttpClient;

/**
 * 创建基于 cURL 实现的 HttpClient
 * @param config_wrapper 之前创建的 HttpConfig 指针
 * @return HttpClient 句柄
 */
Lasvsim_HttpClient* Lasvsim_CurlClient_Create(Lasvsim_HttpConfig* config_wrapper);

/**
 * 销毁 HttpClient
 */
void Lasvsim_HttpClient_Delete(Lasvsim_HttpClient* client);

/**
 * 添加默认请求头
 */
void Lasvsim_HttpClient_AddHeader(Lasvsim_HttpClient* client, const char* key, const char* value);

/**
 * 发送 POST 请求
 * @return 返回响应字符串（注意：调用者需使用 Lasvsim_FreeString 释放内存，或根据实际设计处理）
 */
const char* Lasvsim_HttpClient_Post(Lasvsim_HttpClient* client, const char* url, const char* body);

/**
 * 获取最后一次请求生成的响应（辅助接口）
 */
const char* Lasvsim_HttpClient_GetLastResponse(Lasvsim_HttpClient* client);

#ifdef __cplusplus
}
#endif

#endif