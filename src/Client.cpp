#include "lasvsim/Client.h"
#include "lasvsim/ProcessTask.h"
#include "lasvsim/HttpClient.h"
#include <stdexcept>

namespace Lasvsim {

Client::Client(std::shared_ptr<HttpConfig> httpConfig)
    : httpConfig(httpConfig),
      httpClient(std::make_shared<CurlHttpClient>(httpConfig)),
      processTask(httpClient)  // 正确初始化
{
    if (!httpConfig) {
        throw std::invalid_argument("HttpConfig cannot be null");
    }
}

void Client::initCommonClient() {
    try {
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to initialize common client: " + std::string(e.what()));
    }
}

ProcessTask Client::getProcessTask() {
    return processTask;
}

} // namespace Lasvsim