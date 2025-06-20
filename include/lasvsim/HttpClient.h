#pragma once
#include <string>
#include <map>
#include <memory>
#include "lasvsim/Error.h"
#include "lasvsim/HttpConfig.h"
#include <curl/curl.h>

namespace Lasvsim {
    class HttpClient {
    public:
        /**
         * 构造函数
         * @param httpConfig HTTP配置(超时、认证等)
         */
        HttpClient(std::shared_ptr<HttpConfig> httpConfig,std::map<std::string, std::string>& headers);
        
        virtual ~HttpClient() = default;
        
        /**
         * 发送HTTP请求
         * @param method HTTP方法(GET/POST/PUT/DELETE等)
         * @param url 请求URL(可包含query参数)
         * @param headers 请求头
         * @param body 请求体内容
         * @return 响应体内容
         * @throws std::runtime_error 请求失败时抛出
         */
        virtual std::string SendRequest(
            const std::string& method,
            const std::string& url,
            const std::map<std::string, std::string>& headers,
            const std::string& body = "") = 0;
        
        // 便捷方法(内联实现)
        std::string Get(const std::string& url, 
                    const std::map<std::string, std::string>& headers = {}) {
            return SendRequest("GET", url, headers);
        }
        
        std::string Post(const std::string& url,
                        const std::string& body,
                        const std::map<std::string, std::string>& headers = {}) {
            return SendRequest("POST", url, headers, body);
        }

    protected:
        std::shared_ptr<HttpConfig> config_;
        std::map<std::string, std::string>& defaultHeaders_;
    };

    // 基于cURL的具体实现
    class CurlHttpClient : public HttpClient {
    public:
        CurlHttpClient(std::shared_ptr<HttpConfig> config,std::map<std::string, std::string>& headers);
        ~CurlHttpClient() override;
        
        std::string SendRequest(
            const std::string& method,
            const std::string& url,
            const std::map<std::string, std::string>& headers,
            const std::string& body = "") override;

    private:
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
        void SetCurlOptions(CURL* curl, const std::string& method, const std::string& url);
        void SetRequestHeaders(CURL* curl, const std::map<std::string, std::string>& headers);
    };
}