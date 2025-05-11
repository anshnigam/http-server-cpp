#include "model/request/HttpRequest.hpp"

HttpRequest::HttpRequest() : method_(HttpMethod::UNKNOWN) {
    
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
    parseCookies();
    parseBody(requestStream);

    return true;
}

HttpMethod HttpRequest::getMethod() const {
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

std::optional<Cookie> HttpRequest::getCookie(const std::string& name) const {
    auto it = cookies_.find(name);
    if (it != cookies_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::string HttpRequest::getBody() const {
    return body_;
}

void HttpRequest::setPathParam(const std::string& name, const std::string& value) {
    pathParams_[name] = value;
}

void HttpRequest::parseMethod(std::string methodStr) {
    if (methodStr == "GET") {
        method_ = HttpMethod::GET;
    } else if (methodStr == "POST") {
        method_ = HttpMethod::POST;
    } else if (methodStr == "PUT") {
        method_ = HttpMethod::PUT;
    } else if (methodStr == "DELETE") {
        method_ = HttpMethod::DELETE;
    } else if (methodStr == "HEAD") {
        method_ = HttpMethod::HEAD;
    } else if (methodStr == "OPTIONS") {
        method_ = HttpMethod::OPTIONS;
    } else if (methodStr == "PATCH") {
        method_ = HttpMethod::PATCH;
    } else if (methodStr == "TRACE") {
        method_ = HttpMethod::TRACE;
    } else {
        method_ = HttpMethod::UNKNOWN;
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

void HttpRequest::parseCookies() {
    auto it = headers_.find("Cookie");
    if (it == headers_.end()) {
        return;
    }

    const std::string& cookieHeader = it->second;
    std::istringstream cookieStream(cookieHeader);
    std::string nameValuePair;

    while (std::getline(cookieStream, nameValuePair, ';')) {
        nameValuePair.erase(0, nameValuePair.find_first_not_of(" \t"));
        nameValuePair.erase(nameValuePair.find_last_not_of(" \t") + 1);
        Cookie cookie = Cookie::valueOf(nameValuePair);
        cookies_.emplace(cookie.getName(), cookie);
    }
}

void HttpRequest::parseBody(std::istringstream& requestStream) {
    if (method_ == HttpMethod::POST || method_ == HttpMethod::PUT) {
        // Read the entire remaining content as body
        std::stringstream body_stream;
        body_stream << requestStream.rdbuf();
        body_ = body_stream.str();
    }
}

