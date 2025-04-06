#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"

class RequestHandler {
    public:
        virtual void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) = 0;
        virtual ~RequestHandler() = default;
};

#endif
