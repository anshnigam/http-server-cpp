#include "response/HttpResponse.hpp"
#include <sstream>

HttpResponse::HttpResponse() : statusCode_(200), statusMessage_("OK") {
    headers_["Content-Type"] = "text/html";
}

void HttpResponse::setStatus(int code, const std::string& message) {
    statusCode_ = code;
    statusMessage_ = message;
}

void HttpResponse::setHeader(const std::string& name, const std::string& value) {
    headers_[name] = value;
}

void HttpResponse::setBody(const std::string& body) {
    body_ = body;
    setHeader("Content-Length", std::to_string(body_.length()));
}

std::string HttpResponse::toString() const {
    std::ostringstream response;
    response << "HTTP/1.1 " << statusCode_ << " " << statusMessage_ << "\r\n";
    
    for (const auto& header : headers_) {
        response << header.first << ": " << header.second << "\r\n";
    }
    
    response << "\r\n";
    
    if (!body_.empty()) {
        response << body_;
    }
    
    return response.str();
}

