#pragma once
#include <string>
#include <map>

namespace Lasvsim {
    struct Response {
        int statusCode;
        std::map<std::string, std::string> headers;
        std::string body;
        
        explicit Response(int code = 0) : statusCode(code) {}
        
        bool IsSuccess() const { 
            return statusCode >= 200 && statusCode < 300; 
        }
    };
}