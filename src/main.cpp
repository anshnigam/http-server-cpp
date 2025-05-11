#include "controller/HelloWorldController.hpp"
#include "HttpServer.hpp"
#include "model/request/HttpMethod.hpp"
#include <iostream>

int main() {
    HttpServer server(8080);
    
    server.registerHandler(HttpMethod::GET, "/hello", new HelloWorldController());

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    return 0;
}
