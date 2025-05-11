#ifndef METHODNOTFOUNDHANDLER_HPP
#define METHODNOTFOUNDHANDLER_HPP

#include "handler/RequestHandler.hpp"
#include "model/request/HttpRequest.hpp"
#include "model/response/HttpResponse.hpp"

class MethodNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
        ~MethodNotFoundHandler() override = default;
};

#endif
