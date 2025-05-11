#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "model/request/HttpRequest.hpp"
#include "model/response/HttpResponse.hpp"

class RequestHandler {
    public:
        virtual void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) = 0;
        virtual ~RequestHandler() = default;
};

#endif
