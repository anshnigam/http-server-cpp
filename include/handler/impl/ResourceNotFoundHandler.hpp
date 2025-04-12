#ifndef RESOURCENOTFOUNDHANDLER_HPP
#define RESOURCENOTFOUNDHANDLER_HPP

#include "request/HttpRequest.hpp"
#include "response/HttpResponse.hpp"
#include "handler/RequestHandler.hpp"

class ResourceNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override ;
        ~ResourceNotFoundHandler() = default;
};

#endif
