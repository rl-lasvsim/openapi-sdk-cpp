#pragma once
#include <stdexcept>
#include <string>

namespace lasvsim {
    // 错误原因定义
    using Reason =  std::string;
    const Reason SDK_UNKNOWN = "SDK_UNKNOWN"; // SDK侧未知错误
    const Reason NOT_PERMISSION = "NOT_PERMISSION"; // 没有操作权限
    const Reason PARAM_UNVAILABLE = "PARAM_UNVAILABLE"; // 参数不正确
    const Reason NOT_EXIST = "NOT_EXIST"; // 数据不可用

    // 自定义异常类
    class SDKException : public std::runtime_error {
    public:
        SDKException(int code, std::string message, std::string reason,std::string url)
            : std::runtime_error(formatMessage(code,message,reason,url)), code_(code), message_(message), reason_(reason),url_(url) {}

        int getCode() const {
            return code_;
        }

        std::string getMessage() const {
            return message_;
        }

        std::string getReason() const {
            return reason_;
        }

        std::string getUrl() const {
            return url_;
        }

    private:
        static std::string formatMessage(int code, const std::string& message, 
                                    const std::string& reason, const std::string& url) {
            return "SDK Error [" + std::to_string(code) + "]: " + message + 
                " (Reason: " + reason + ", URL: " + url + ")";
        }

        int code_;
        std::string message_;
        std::string reason_;
        std::string url_;
    };
}