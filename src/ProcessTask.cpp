#include "lasvsim/ProcessTask.h"
#include <stdexcept>
#include "lasvsim/HttpClient.h"
#include <nlohmann/json.hpp> // 需要集成JSON库

using json = nlohmann::json;

namespace lasvsim {

ProcessTask::ProcessTask(std::shared_ptr<HttpClient> httpClient) 
    : http_client_(httpClient) {
    if (!http_client_) {
        throw std::invalid_argument("HttpClient cannot be null");
    }
}

CopyRecordRes ProcessTask::CopyRecord(int taskId, int recordId) {
    if (taskId <= 0 || recordId <= 0) {
        throw std::invalid_argument("Invalid taskId or recordId");
    }
    
    try {

        // 构建json 字符串
        json reqJson;
        reqJson["task_id"] = taskId;
        reqJson["record_id"] = recordId;
        std::string jsonStr = reqJson.dump();

        // // 发送POST请求(空body)
        std::string response = http_client_->Post("/openapi/process_task/v2/record/copy", jsonStr);
        
        // 解析响应
        json resJson = json::parse(response);

        CopyRecordRes res;
        res.new_record_id = resJson["new_record_id"];
        res.sim_record_id = resJson["sim_record_id"];
        res.scen_id = resJson["scen_id"];
        res.scen_ver = resJson["scen_ver"];

        return res;
    } catch (const std::exception& e) {
        throw std::runtime_error("Copy record failed: " + std::string(e.what()));
    }
}

} // namespace Lasvsim