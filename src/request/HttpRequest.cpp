#include "request/HttpRequest.hpp"
#include <sstream>

HttpRequest::HttpRequest() : method_(UNKNOWN) {
    
}

// expecting: GET /path HTTP/1.1
bool HttpRequest::parse(const std::string& rawRequest) {
    std::istringstream requestStream(rawRequest);
    std::string line;

    if (!std::getline(requestStream, line)) {
        return false;
    }

    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    std::istringstream lineStream(line);
    std::string methodStr;

    if (!(lineStream >> methodStr >> path_ >> version_)) {
        return false;
    }

    parseMethod(methodStr);
    parseHeaders(requestStream);
    parseBody(requestStream);

    return true;
}

HttpRequest::Method HttpRequest::getMethod() const {
    return method_;
}

std::string HttpRequest::getPath() const {
    return path_;
}

std::string HttpRequest::getPathParam(const std::string& name) const {
    auto it = pathParams_.find(name);
    return (it != pathParams_.end()) ? it->second : "";
}

std::string HttpRequest::getVersion() const {
    return version_;
}

std::string HttpRequest::getHeader(const std::string& name) const {
    auto it = headers_.find(name);
    if (it != headers_.end()) {
        return it->second;
    }
    return "";
}

std::string HttpRequest::getBody() const {
    return body_;
}

void HttpRequest::setPathParam(const std::string& name, const std::string& value) {
    pathParams_[name] = value;
}

void HttpRequest::parseMethod(std::string methodStr) {
    if (methodStr == "GET") {
        method_ = HttpRequest::GET;
    } else if (methodStr == "POST") {
        method_ = HttpRequest::POST;
    } else if (methodStr == "PUT") {
        method_ = HttpRequest::PUT;
    } else if (methodStr == "DELETE") {
        method_ = HttpRequest::DELETE;
    } else if (methodStr == "HEAD") {
        method_ = HttpRequest::HEAD;
    } else if (methodStr == "OPTIONS") {
        method_ = HttpRequest::OPTIONS;
    } else if (methodStr == "PATCH") {
        method_ = HttpRequest::PATCH;
    } else if (methodStr == "TRACE") {
        method_ = HttpRequest::TRACE;
    } else {
        method_ = HttpRequest::UNKNOWN;
    }
}

void HttpRequest::parseHeaders(std::istringstream& requestStream) {
    std::string line;
   while (std::getline(requestStream, line) && line != "\r" && line != "") {
        if (line.back() == '\r') {
            line.pop_back();  // Remove carriage return
        }
        
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string header_name = line.substr(0, colon_pos);
            std::string header_value = line.substr(colon_pos + 1);
            
            header_value.erase(0, header_value.find_first_not_of(" \t"));
            header_value.erase(header_value.find_last_not_of(" \t") + 1);
            
            headers_[header_name] = header_value;
        }
    } 
}

void HttpRequest::parseBody(std::istringstream& requestStream) {
    if (method_ == POST || method_ == PUT) {
        // Read the entire remaining content as body
        std::stringstream body_stream;
        body_stream << requestStream.rdbuf();
        body_ = body_stream.str();
    }
}

