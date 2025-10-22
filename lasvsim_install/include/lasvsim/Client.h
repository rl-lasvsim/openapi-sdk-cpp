#pragma once
#include <memory>
#include "lasvsim/HttpConfig.h"
#include "lasvsim/HttpClient.h"
#include "lasvsim/Request.h"
#include "lasvsim/Response.h"
#include "lasvsim/ProcessTask.h"
#include "lasvsim/Simulator.h"

namespace lasvsim {
    class Client {
        public:
            /**
             * 构造函数
             * @param httpConfig HTTP客户端配置
             */
            Client(std::shared_ptr<HttpConfig> httpConfig);
            
            virtual ~Client() = default;
            
            /**
             * 获取处理任务
             * @return ProcessTask 对象
             */
            std::shared_ptr<ProcessTask>& GetProcessTask();

            /**
             * 初始化仿真器
             * @return Simulator 对象
             */
            Simulator InitSimulatorFromConfig(std::shared_ptr<SimulatorConfig> simConfig);
        private:
            std::shared_ptr<HttpConfig> http_config_;
            std::shared_ptr<HttpClient> http_client_;
            std::shared_ptr<ProcessTask> process_task_;
    };
}