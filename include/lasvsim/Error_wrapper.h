#ifndef LASVSIM_ERROR_WRAPPER_H
#define LASVSIM_ERROR_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// 定义 C 风格的错误结构体
typedef struct {
    int code;
    char message[256];
    char reason[64];
    char url[256];
} LasvsimErrorInfo;

// 获取最后一次发生的错误信息
void Lasvsim_GetLastError(LasvsimErrorInfo* outInfo);

// 清除错误信息
void Lasvsim_ClearError(void);

#ifdef __cplusplus
}
#endif

#endif