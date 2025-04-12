#include "request/HttpRequest.hpp"
#include "response/HttpResponse.hpp"
#include "controller/HelloWorldController.hpp"

#include <iostream>

void HelloWorldController::handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) {
    std::cout << "Received request for hello world" << std::endl;
    httpResponse.setStatus(200, "OK");
    httpResponse.setBody("<html><body>Hello from server</body></html>");
}
