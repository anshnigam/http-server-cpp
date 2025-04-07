#include "../include/HttpServer.h"
#include <iostream>

int main() {
    HttpServer server(8080);

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    return 0;
}
