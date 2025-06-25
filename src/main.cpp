#include "Lasvsim/Client.h"
#include "lasvsim/HttpClient.h"
#include "Lasvsim/HttpConfig.h"
#include "Lasvsim/ProcessTask.h"
#include <iostream>

int main() {
    try {
        // 1. 创建配置 (替换为你的实际API信息)
        auto config = std::make_shared<Lasvsim::HttpConfig>(
            "http://8.146.201.197:30080/dev",  // 替换为实际baseUrl
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1aWQiOjE3LCJvaWQiOjI1LCJuYW1lIjoiYWRtaW7lr4blr4bpurvmiJEiLCJpZGVudGl0eSI6ImFkbWluIiwicGVybWlzc2lvbnMiOltdLCJpc3MiOiJ1c2VyIiwic3ViIjoiTGFzVlNpbSIsImV4cCI6MTc1MTAwOTk0NiwibmJmIjoxNzUwNDA1MTQ2LCJpYXQiOjE3NTA0MDUxNDYsImp0aSI6IjE3In0.zEgNytOVF04wVrXK1YbgbEdq5onTWKnvJtgnnY_vC4A"      // 替换为实际token
        );

        // 3. 创建业务客户端
        Lasvsim::Client client(config);

        // 4. 调用接口 (替换为实际参数)
        std::cout << "Calling copyRecord(123, 456)..." << std::endl;
        std::shared_ptr<Lasvsim::ProcessTask> p = client.getProcessTask();
        // autp c = p.get()->copyRecord(1,1);
        auto r = p.get()->copyRecord(123, 456);
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