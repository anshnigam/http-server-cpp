#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "model/common/Cookie.hpp"
#include <map>
#include <string>
#include <vector>

class HttpResponse {
    public:
        HttpResponse();

        void setStatus(int code, const std::string& message);
        void setHeader(const std::string& name, const std::string& value);
        void addCookie(const Cookie& cookie);
        void setBody(const std::string& body);

        std::string toString() const;

    private:
        int statusCode_;
        std::string statusMessage_;
        std::map<std::string, std::string> headers_;
        std::string body_;

        std::vector<Cookie> cookies_;
};

#endif
