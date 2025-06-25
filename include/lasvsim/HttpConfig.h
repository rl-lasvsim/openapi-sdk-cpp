#pragma once
#include <string>
#include <memory>

namespace Lasvsim {
    class HttpConfig {
    public:
        HttpConfig(const std::string& endpoint, 
               const std::string& token);
        
        // 基础配置
        const std::string& getEndpoint() const { return endpoint_; }
        const std::string& getToken() const { return token_; }
        
        // SSL验证
        // void SetVerifySSL(bool verify) { verifySSL_ = verify; }
        // bool GetVerifySSL() const { return verifySSL_; }
        
    private:
        std::string endpoint_;
        std::string token_;
        // bool verifySSL_ = true;
    };
}