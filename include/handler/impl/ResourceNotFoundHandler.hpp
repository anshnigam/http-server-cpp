#ifndef RESOURCENOTFOUNDHANDLER_HPP
#define RESOURCENOTFOUNDHANDLER_HPP

#include "handler/RequestHandler.hpp"
#include "model/request/HttpRequest.hpp"
#include "model/response/HttpResponse.hpp"

class ResourceNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override ;
        ~ResourceNotFoundHandler() = default;
};

#endif
