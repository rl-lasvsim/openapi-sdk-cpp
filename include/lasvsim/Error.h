#pragma once
#include <stdexcept>
#include <string>

namespace Lasvsim {
    // 错误码枚举
    enum class ErrorCode {
        OK = 0,
        // NETWORK_ERROR,
        // INVALID_CONFIG,
        // API_ERROR,
        // UNKNOWN_ERROR
    };

    // 自定义异常类
    class SDKException : public std::runtime_error {
    public:
        ErrorCode code;
        
    };
}