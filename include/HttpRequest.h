#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <sstream>
#include <map>

class HttpRequest {
    public:
        enum Method {
            GET, POST, PUT, DELETE, HEAD, OPTIONS, PATCH, TRACE,
            UNKNOWN
        };

        HttpRequest();

        bool parse(const std::string& raw_request);

        Method getMethod() const;
        std::string getPath() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& name) const;
        std::string getBody() const;

    private:
        Method method_;
        std::string path_;
        std::string version_;
        std::map <std::string, std::string> headers_;
        std::string body_;
        void parseMethod(std::string methodStr);
        void parseHeaders(std::istringstream& requestStream);
        void parseBody(std::istringstream& requestStream);
};

#endif
