#include "lasvsim/ProcessTask.h"
#include <stdexcept>
#include "lasvsim/HttpClient.h"
#include <nlohmann/json.hpp> // 需要集成JSON库

using json = nlohmann::json;

namespace Lasvsim {

ProcessTask::ProcessTask(std::shared_ptr<HttpClient> httpClient) 
    : httpClient_(httpClient) {
    if (!httpClient_) {
        throw std::invalid_argument("HttpClient cannot be null");
    }
}

CopyRecordRes ProcessTask::copyRecord(int taskId, int recordId) {
    if (taskId <= 0 || recordId <= 0) {
        throw std::invalid_argument("Invalid taskId or recordId");
    }
    
    try {

        // 构建json 字符串
        json j;
        j["task_id"] = taskId;
        j["record_id"] = recordId;
        std::string jsonStr = j.dump();
        // 构建API路径
        // std::string path = buildCopyRecordPath(taskId, recordId);
        

        // auto j = nlohmann::json::parse(jsonStr);
        // a =  j.get<std::vector<T>>();

        // // 发送POST请求(空body)
        std::string response = httpClient_->Post("/openapi/process_task/v2/record/copy", jsonStr);
        
        // 解析响应
        json jRes = json::parse(response);

        CopyRecordRes res;
        res.newRecordId = jRes["new_record_id"];
        res.simRecordId = jRes["sim_record_id"];
        res.scenId = jRes["scen_id"];
        res.scenVer = jRes["scen_ver"];

        return res;
    } catch (const std::exception& e) {
        throw std::runtime_error("Copy record failed: " + std::string(e.what()));
    }
}

} // namespace Lasvsim