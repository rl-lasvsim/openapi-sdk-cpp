#ifndef LASVSIM_PROCESS_TASK_WRAPPER_H
#define LASVSIM_PROCESS_TASK_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// 定义与 C++ CopyRecordRes 对应的 C 结构体
typedef struct {
    char sim_record_id[128];
    char scen_id[128];
    char scen_ver[64];
    int new_record_id;
} Lasvsim_CopyRecordRes;

// ProcessTask 不透明句柄
typedef struct Lasvsim_ProcessTask Lasvsim_ProcessTask;

// 创建函数
Lasvsim_ProcessTask* Lasvsim_ProcessTask_Create(void* process_task_ptr);

/**
 * 销毁实例
 */
void Lasvsim_ProcessTask_Delete(Lasvsim_ProcessTask* task);

/**
 * 复制记录
 * @param task 任务句柄
 * @param taskId 任务ID
 * @param recordId 记录ID
 * @param outRes 用于接收结果的结构体指针
 * @return int 0表示成功，非0表示失败
 */
int Lasvsim_ProcessTask_CopyRecord(Lasvsim_ProcessTask* task, int taskId, int recordId, Lasvsim_CopyRecordRes* outRes);

#ifdef __cplusplus
}
#endif

#endif