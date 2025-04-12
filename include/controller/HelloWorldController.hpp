#ifndef HELLOWORLDCONTROLLER_HPP
#define HELLOWORLDCONTROLLER_HPP

#include "handler/RequestHandler.hpp"

class HelloWorldController: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
};
#endif
