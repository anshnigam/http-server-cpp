#ifndef RESOURCENOTFOUNDHANDLER_H
#define RESOURCENOTFOUNDHANDLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "RequestHandler.h"

#include <iostream>

class ResourceNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override {
            std::cout << "API resource not found" << std::endl;
            httpResponse.setStatus(404, "Not Found");
            httpResponse.setBody("<html><body>Not Found</body></html>");
        }
};

#endif
