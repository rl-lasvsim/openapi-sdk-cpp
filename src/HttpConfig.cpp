#include "lasvsim/HttpConfig.h"

namespace lasvsim {
    HttpConfig::HttpConfig(const std::string& endpoint, const std::string& token)
        : endpoint_(endpoint), token_(token) {
        if (endpoint_.empty()) {
            throw std::invalid_argument("Endpoint cannot be empty");
        }
        if (token_.empty()) {
            throw std::invalid_argument("Token cannot be empty");
        }
    }

    // const std::string& HttpConfig::getEndpoint() const {
    //     return endpoint_;
    // }

    // const std::string& HttpConfig::getToken() const {
    //     return token_;
    // }
}