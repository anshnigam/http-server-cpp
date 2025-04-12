#ifndef BADREQUESTHANDLER_HPP
#define BADREQUESTHANDLER_HPP

#include "handler/RequestHandler.hpp"
#include "request/HttpRequest.hpp"
#include "response/HttpResponse.hpp"

class BadRequestHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
        ~BadRequestHandler() override = default;
};

#endif
