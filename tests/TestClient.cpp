#include "Lasvsim/Client.h"
#include "Lasvsim/HttpConfig.h"
#include "Lasvsim/MockHttpClient.h" // 模拟HTTP客户端
#include <gtest/gtest.h>

namespace Lasvsim {
namespace Testing {

// 模拟HTTP响应数据
const std::string kMockResponse = R"({
    "simRecordId": "rec_test_123",
    "scenId": "scen_test_456",
    "scenVer": "1.0.0",
    "newRecordId": 999
})";

// 测试夹具
class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建模拟配置
        config_ = std::make_shared<HttpConfig>("https://api.example.com", "test_token");
        
        // 创建模拟HTTP客户端
        mockHttpClient_ = std::make_shared<MockHttpClient>(config_);
        
        // 设置模拟行为
        EXPECT_CALL(*mockHttpClient_, SendRequest(_, _, _, _))
            .WillRepeatedly(Return(kMockResponse));
        
        // 创建被测客户端
        client_ = std::make_unique<Client>(config_, mockHttpClient_);
    }

    std::shared_ptr<HttpConfig> config_;
    std::shared_ptr<MockHttpClient> mockHttpClient_;
    std::unique_ptr<Client> client_;
};

// 测试正常流程
TEST_F(ClientTest, CopyRecordSuccess) {
    const int kTestTaskId = 123;
    const int kTestRecordId = 456;
    
    // 预期调用参数
    std::string expectedUrl = "/api/v1/tasks/123/records/456/copy";
    std::map<std::string, std::string> expectedHeaders = {
        {"Authorization", "Bearer test_token"},
        {"Content-Type", "application/json"}
    };

    // 设置期望（使用GMock）
    EXPECT_CALL(*mockHttpClient_, 
        SendRequest(
            "POST", 
            expectedUrl,
            expectedHeaders,
            "")
        ).Times(1);
    
    // 执行测试
    auto result = client_->copyRecord(kTestTaskId, kTestRecordId);
    
    // 验证结果
    EXPECT_EQ(result.simRecordId, "rec_test_123");
    EXPECT_EQ(result.scenId, "scen_test_456");
    EXPECT_EQ(result.newRecordId, 999);
}

// 测试错误处理
TEST_F(ClientTest, HandlesHttpError) {
    // 模拟HTTP错误
    EXPECT_CALL(*mockHttpClient_, SendRequest(_, _, _, _))
        .WillOnce(Throw(std::runtime_error("Network error")));
    
    // 验证异常抛出
    EXPECT_THROW({
        client_->copyRecord(1, 1);
    }, std::runtime_error);
}

// 测试无效输入
TEST_F(ClientTest, RejectsInvalidInput) {
    EXPECT_THROW(client_->copyRecord(0, 1), std::invalid_argument);
    EXPECT_THROW(client_->copyRecord(1, -1), std::invalid_argument);
}

} // namespace Testing
} // namespace Lasvsim

// 主函数
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}