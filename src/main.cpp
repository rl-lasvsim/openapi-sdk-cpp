#include "Lasvsim/Client.h"
#include "lasvsim/HttpClient.h"
#include "Lasvsim/HttpConfig.h"
#include "Lasvsim/ProcessTask.h"
#include <iostream>

int main() {
    try {
        // 1. 创建配置 (替换为你的实际API信息)
        auto config = std::make_shared<Lasvsim::HttpConfig>(
            "https://api.example.com",  // 替换为实际baseUrl
            "your_api_token_here"      // 替换为实际token
        );

        // 3. 创建业务客户端
        Lasvsim::Client client(config);

        // 4. 调用接口 (替换为实际参数)
        std::cout << "Calling copyRecord(123, 456)..." << std::endl;
        Lasvsim::ProcessTask p = client.getProcessTask();

        auto r = p.copyRecord(123, 456);
        // 5. 打印结果
        std::cout << "\nResult:\n"
                  << "  simRecordId: " << r.simRecordId << "\n"
                  << "  scenId: " << r.scenId << "\n"
                  << "  scenVer: " << r.scenVer << "\n"
                  << "  newRecordId: " << r.newRecordId << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}