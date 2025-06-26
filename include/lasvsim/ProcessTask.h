
#pragma once
#include "lasvsim/HttpClient.h"

namespace lasvsim {
    class CopyRecordRes {
    public:
        std::string sim_record_id;
        std::string scen_id;
        std::string scen_ver;
        int new_record_id;
    };

    class ProcessTask {
    public:
        ProcessTask(std::shared_ptr<HttpClient> httpClient);
        virtual ~ProcessTask() = default;
        //     def copy_record(self, task_id: int, record_id: int):
        CopyRecordRes CopyRecord(int taskId, int recordId);

    private:
        std::shared_ptr<HttpClient> http_client_;
    };
}