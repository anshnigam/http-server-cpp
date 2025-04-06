#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <map>
#include <string>

class HttpResponse {
    public:
        HttpResponse();

        void setStatus(int code, const std::string& message);
        void setHeader(const std::string& name, const std::string& value);
        void setBody(const std::string& body);

        std::string toString() const;

    private:
        int statusCode_;
        std::string statusMessage_;
        std::map<std::string, std::string> headers_;
        std::string body_;
};

#endif
