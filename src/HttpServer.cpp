#include "../include/HttpServer.h"
#include "../include/HttpRequest.h"
#include "../include/HttpResponse.h"

#include <ostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

HttpServer::HttpServer(int port) : port_(port), socketFD_(-1), alive_(false) {
    std::cout << "Created Http Server" << std::endl;
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::registerHandler(const std::string& requestPath, RequestHandler* handler) {
}

bool HttpServer::start() {
    socketFD_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD_ < 0) {
        std::cerr << "ERROR opening socket" << std::endl;
        return false;
    }

    int opt = 1;
    if (setsockopt(socketFD_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "ERROR setting socket options" << std::endl;
        return false;
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port_);

    if (bind(socketFD_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "ERROR on binding" << std::endl;
        return false;
    }
    
    if (listen(socketFD_, 5) < 0) {
        std::cerr << "ERROR on listen" << std::endl;
        return false;
    }

    alive_ = true;
    
    std::cout << "Server listening on port " << port_ << std::endl;
    
    
    acceptConnection();
    
    return true;

}

void HttpServer::stop() {
    if (socketFD_ > 0) {
        alive_ = false;
        close(socketFD_);
        socketFD_ = -1;
    }
}

void HttpServer::acceptConnection() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    while(alive_) {
        int clientFD = accept(socketFD_, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (clientFD < 0) {
            std::cerr << "ERROR on accept" << std::endl;
            continue;
        }
        handleConnection(clientFD);
    }
}

void HttpServer::handleConnection(int clientFD) {
    char buffer[4096] = {0};
    int readBytes = recv(clientFD, buffer, sizeof(buffer)-1, 0);

    if (readBytes < 0) {
        std::cerr << "ERROR reading from socket" << std::endl;
        close(clientFD);
        return;
    }

    std::string rawRequest(buffer, readBytes);
    HttpRequest request;

    if (!request.parse(rawRequest)) {
        // Send 400 Bad Request if parsing fails
        HttpResponse response;
        response.setStatus(400, "Bad Request");
        response.setBody("<html><body><h1>400 Bad Request</h1></body></html>");
        
        std::string response_str = response.toString();
        write(clientFD, response_str.c_str(), response_str.length());
        close(clientFD);
        return;
    }

    RequestHandler* handler = findHandler(request);
    
    HttpResponse response;
    
    handler->handle(request, response);
    
    std::string response_str = response.toString();   
    std::cout << "Response : " << response_str << std::endl;
    write(clientFD, response_str.c_str(), response_str.length());
    
    close(clientFD);
}


RequestHandler* HttpServer::findHandler(const HttpRequest& httpRequest) {
}
