#include "lasvsim/Client.h"
#include "lasvsim/ProcessTask.h"
#include "lasvsim/HttpClient.h"
#include <stdexcept>

namespace lasvsim {

Client::Client(std::shared_ptr<HttpConfig> httpConfig)
    : httpConfig(httpConfig){
    if (!httpConfig) {
        throw std::invalid_argument("HttpConfig cannot be null");
    }

    auto headers = std::map<std::string, std::string>();
    httpClient = std::make_shared<CurlHttpClient>(httpConfig,headers);
    processTask = std::make_shared<ProcessTask>(httpClient);
}

Simulator Client::initSimulatorFromConfig(std::shared_ptr<SimulatorConfig> simConfig) {
    if (!simConfig) {
        throw std::invalid_argument("SimulatorConfig cannot be null");
    }
    
    return Simulator(httpClient, simConfig);
}

std::shared_ptr<ProcessTask>&  Client::getProcessTask() {
    return processTask;
}

} // namespace Lasvsim