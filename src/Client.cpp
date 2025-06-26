#include "lasvsim/Client.h"
#include "lasvsim/ProcessTask.h"
#include "lasvsim/HttpClient.h"
#include <stdexcept>

namespace Lasvsim {

Client::Client(std::shared_ptr<HttpConfig> httpConfig)
    : httpConfig(httpConfig){
    if (!httpConfig) {
        throw std::invalid_argument("HttpConfig cannot be null");
    }

    auto headers = std::map<std::string, std::string>();
    httpClient = std::make_shared<CurlHttpClient>(httpConfig,headers);
    processTask = std::make_shared<ProcessTask>(httpClient);
}

void Client::initCommonClient() {
    try {
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to initialize common client: " + std::string(e.what()));
    }
}

std::shared_ptr<ProcessTask>&  Client::getProcessTask() {
    return processTask;
}

} // namespace Lasvsim