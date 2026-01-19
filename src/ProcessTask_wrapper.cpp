#include "lasvsim/ProcessTask_wrapper.h"
#include "lasvsim/ProcessTask.h"
#include <cstring>
#include <algorithm>

// 内部结构定义（关联 Client Wrapper 中获取的 shared_ptr）
struct Lasvsim_ProcessTask {
    std::shared_ptr<lasvsim::ProcessTask> obj;

    // 构造函数
    Lasvsim_ProcessTask(std::shared_ptr<lasvsim::ProcessTask> ptr)
        : obj(ptr) {}
};

extern "C" {

Lasvsim_ProcessTask* Lasvsim_ProcessTask_Create(void* process_task_ptr) {
    if (!process_task_ptr) return nullptr;
    try {
        // 关键：将 void* 转换回实际的类型
        // process_task_ptr 是一个 shared_ptr<ProcessTask>* 指针
        std::shared_ptr<lasvsim::ProcessTask>* real_ptr = 
            reinterpret_cast<std::shared_ptr<lasvsim::ProcessTask>*>(process_task_ptr);
        
        // 解引用获取 shared_ptr，然后构造对象
        return new Lasvsim_ProcessTask(*real_ptr);
    } catch (...) {
        return nullptr;
    }
}

// 释放函数
void Lasvsim_ProcessTask_Delete(Lasvsim_ProcessTask* task) {
    if (task) {
        delete task;
    }
}

int Lasvsim_ProcessTask_CopyRecord(Lasvsim_ProcessTask* task, int taskId, int recordId, Lasvsim_CopyRecordRes* outRes) {
        // Add logging to identify which object is null
    if (!task) {
        fprintf(stderr, "DEBUG: task is NULL\n");
        return -1;
    }
    
    if (!task->obj) {
        fprintf(stderr, "DEBUG: task->obj is NULL\n");
        return -1;
    }
    
    if (!outRes) {
        fprintf(stderr, "DEBUG: outRes is NULL\n");
        return -1;
    }

    if (!task || !task->obj || !outRes) return -1;

    try {
        
        // 调用 C++ 接口
        lasvsim::CopyRecordRes res = task->obj->CopyRecord(taskId, recordId);

        fprintf(stderr, "DEBUG: CopyRecord called with taskId=%d, recordId=%d\n", taskId, recordId);

        // 填充 C 结构体，使用 strncpy 确保安全
        std::strncpy(outRes->sim_record_id, res.sim_record_id.c_str(), sizeof(outRes->sim_record_id) - 1);
        std::strncpy(outRes->scen_id, res.scen_id.c_str(), sizeof(outRes->scen_id) - 1);
        std::strncpy(outRes->scen_ver, res.scen_ver.c_str(), sizeof(outRes->scen_ver) - 1);
        outRes->new_record_id = res.new_record_id;

        return 0; // 成功
    } catch (const std::exception& e) {
        fprintf(stderr, "ERROR: Exception caught in Lasvsim_ProcessTask_CopyRecord: %s\n", e.what());
        return -2; // 失败
    } catch (...) {
        fprintf(stderr, "ERROR: Unknown exception caught in Lasvsim_ProcessTask_CopyRecord - taskId=%d, recordId=%d\n", taskId, recordId);
        return -2; // 失败
    }
}

}