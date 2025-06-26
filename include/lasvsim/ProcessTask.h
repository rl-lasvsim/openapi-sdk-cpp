
#pragma once
#include "lasvsim/HttpClient.h"

namespace lasvsim {
    class CopyRecordRes {
    public:
        std::string simRecordId;
        std::string scenId;
        std::string scenVer;
        int newRecordId;
    };

    class ProcessTask {
    public:
        ProcessTask(std::shared_ptr<HttpClient> httpClient);
        virtual ~ProcessTask() = default;
        //     def copy_record(self, task_id: int, record_id: int):
        CopyRecordRes copyRecord(int taskId, int recordId);

    private:
        std::shared_ptr<HttpClient> httpClient_;
    };
}