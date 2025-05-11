#ifndef BADREQUESTHANDLER_HPP
#define BADREQUESTHANDLER_HPP

#include "handler/RequestHandler.hpp"
#include "model/request/HttpRequest.hpp"
#include "model/response/HttpResponse.hpp"

class BadRequestHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
        ~BadRequestHandler() override = default;
};

#endif
