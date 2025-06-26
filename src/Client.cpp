#include "lasvsim/Client.h"
#include "lasvsim/ProcessTask.h"
#include "lasvsim/HttpClient.h"
#include <stdexcept>

namespace lasvsim {

Client::Client(std::shared_ptr<HttpConfig> httpConfig)
    : http_config_(httpConfig){
    if (!httpConfig) {
        throw std::invalid_argument("HttpConfig cannot be null");
    }

    auto headers = std::map<std::string, std::string>();
    http_client_ = std::make_shared<CurlHttpClient>(httpConfig,headers);
    process_task_ = std::make_shared<ProcessTask>(http_client_);
}

Simulator Client::InitSimulatorFromConfig(std::shared_ptr<SimulatorConfig> simConfig) {
    if (!simConfig) {
        throw std::invalid_argument("SimulatorConfig cannot be null");
    }
    
    return Simulator(http_client_, simConfig);
}

std::shared_ptr<ProcessTask>&  Client::GetProcessTask() {
    return process_task_;
}

} // namespace Lasvsim