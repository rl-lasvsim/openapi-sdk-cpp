#pragma once
#include <string>
#include <map>

namespace lasvsim {
    struct Request {
        std::string method;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;
        
        Request(const std::string& method, const std::string& path)
            : method(method), path(path) {}
    };
}