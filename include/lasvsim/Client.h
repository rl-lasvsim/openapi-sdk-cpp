#pragma once
#include <memory>
#include "lasvsim/HttpConfig.h"
#include "lasvsim/HttpClient.h"
#include "lasvsim/Request.h"
#include "lasvsim/Response.h"
#include "lasvsim/ProcessTask.h"

namespace Lasvsim {
class Client {
    public:
        /**
         * 构造函数
         * @param httpConfig HTTP客户端配置
         */
        Client(std::shared_ptr<HttpConfig> httpConfig);
        
        virtual ~Client() = default;
        
        /**
         * 初始化通用客户端
         * @throws std::runtime_error 初始化失败时抛出
         */
        void initCommonClient();
        
        /**
         * 获取处理任务
         * @return ProcessTask 对象
         */
        std::shared_ptr<ProcessTask>& getProcessTask();

    private:
        std::shared_ptr<HttpConfig> httpConfig;
        std::shared_ptr<HttpClient> httpClient;
        std::shared_ptr<ProcessTask> processTask;
};
}