#include "lasvsim/HttpClient.h"
#include "lasvsim/Error.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // 需要集成JSON库

using json = nlohmann::json;

namespace lasvsim {


    HttpClient::HttpClient(
        std::shared_ptr<HttpConfig> config,std::map<std::string, std::string>& headers
    ) : config_(std::move(config)){
        if (!config_) {
            throw std::invalid_argument("HttpConfig cannot be null");
        }

        default_headers_ = headers;
        if (default_headers_.empty()) {
            // 如果没有提供默认头，则初始化一个空的map
            default_headers_ = std::map<std::string, std::string>();
        }

        // Add necessary default headers (without overriding custom ones)
        if (default_headers_.find("Authorization") == default_headers_.end() && !config_->GetToken().empty()) {
            default_headers_["Authorization"] = "Bearer " + config_->GetToken();
        }
        
        if (default_headers_.find("Content-Type") == default_headers_.end()) {
            default_headers_["Content-Type"] = "application/json";
        }
        
        if (default_headers_.find("Connection") == default_headers_.end()) {
            default_headers_["Connection"] = "keep-alive";
        }
    }


    CurlHttpClient::CurlHttpClient(std::shared_ptr<HttpConfig> config,std::map<std::string, std::string>& headers)
        : HttpClient(config,headers) {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    CurlHttpClient::~CurlHttpClient() {
        curl_global_cleanup();
    }

    size_t CurlHttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void CurlHttpClient::SetCurlOptions(CURL* curl, const std::string& method, const std::string& url) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, config_->getTimeout());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        // 设置SSL选项(如果需要)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
   
    }

    void CurlHttpClient::SetRequestHeaders(CURL* curl, const std::map<std::string, std::string>& headers) {
        struct curl_slist* headerList = nullptr;
        for (const auto& [key, value] : headers) {
            headerList = curl_slist_append(headerList, (key + ": " + value).c_str());
        }
        
        if (!default_headers_.empty()) {
            for (const auto& [key, value] : default_headers_) {
                headerList = curl_slist_append(headerList, (key + ": " + value).c_str());
            }
        }
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }

    std::string CurlHttpClient::SendRequest(
        const std::string& method,
        const std::string& url,
        const std::map<std::string, std::string>& headers,
        const std::string& body) {
        
        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize cURL");
        }
        
        std::string response;
        // 如果url以/开头，则添加默认的baseUrl
        std::string fullUrl = url;
        if (fullUrl.front() == '/') {
            fullUrl = config_->GetEndpoint() + fullUrl;
        }
        
        try {
            // 设置基本选项
            SetCurlOptions(curl, method, fullUrl);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            
            // 设置请求头
            SetRequestHeaders(curl, headers);
            
            // 设置请求体
            if (!body.empty() && (method == "POST" || method == "PUT")) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
            }
            
            // 执行请求
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                throw std::runtime_error("cURL error: " + std::string(curl_easy_strerror(res)));
            }
            
            // 检查HTTP状态码
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            if (httpCode != 200) {
                /**
                           try:
                error_data = ujson.loads(response.data)
            except Exception as e:
                error_data = {"message": f'client parse json error:{e},data:{response.data}'}

            reason = error_data.get('reason') if isinstance(error_data, dict) else None
            raise APIError(
                status_code=response.status,
                message=error_data.get('message'),
                reason=reason,
            )
                 */

                json error_json = json::parse(response);
                throw SDKException(httpCode, error_json["message"], error_json["reason"],fullUrl);
            }
            
        } catch (...) {
            curl_easy_cleanup(curl);
            throw;
        }
        
        curl_easy_cleanup(curl);
        return response;
    }

    HttpClient* CurlHttpClient::Clone() {
        return new CurlHttpClient(config_, default_headers_);
    }
} // namespace Lasvsim