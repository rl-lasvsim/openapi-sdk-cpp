#include "lasvsim/ProcessTask_wrapper.h"
#include "lasvsim/ProcessTask.h"
#include <cstring>
#include <algorithm>

// 内部结构定义（关联 Client Wrapper 中获取的 shared_ptr）
struct Lasvsim_ProcessTask {
    std::shared_ptr<lasvsim::ProcessTask> obj;
};

extern "C" {

int Lasvsim_ProcessTask_CopyRecord(Lasvsim_ProcessTask* task, int taskId, int recordId, Lasvsim_CopyRecordRes* outRes) {
    if (!task || !task->obj || !outRes) return -1;

    try {
        // 调用 C++ 接口
        lasvsim::CopyRecordRes res = task->obj->CopyRecord(taskId, recordId);

        // 填充 C 结构体，使用 strncpy 确保安全
        std::strncpy(outRes->sim_record_id, res.sim_record_id.c_str(), sizeof(outRes->sim_record_id) - 1);
        std::strncpy(outRes->scen_id, res.scen_id.c_str(), sizeof(outRes->scen_id) - 1);
        std::strncpy(outRes->scen_ver, res.scen_ver.c_str(), sizeof(outRes->scen_ver) - 1);
        outRes->new_record_id = res.new_record_id;

        return 0; // 成功
    } catch (...) {
        // 这里可以配合之前的 Error Wrapper 记录具体的异常信息
        return -2; // 失败
    }
}

}