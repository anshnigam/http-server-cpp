#include "HttpServer.hpp"
#include "handler/RequestHandler.hpp"
#include "handler/impl/BadRequestHandler.hpp"
#include "handler/impl/MethodNotFoundHandler.hpp"
#include "pool/executor/ThreadPoolExecutor.hpp"
#include "request/HttpMethod.hpp"
#include "request/HttpRequest.hpp"
#include "response/HttpResponse.hpp"
#include "handler/impl/ResourceNotFoundHandler.hpp"

#include <ostream>
#include <regex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

HttpServer::HttpServer(int port) : port_(port), socketFD_(-1), alive_(false), threadPool_(2) {
    badRequestHandler_ = new BadRequestHandler();
    resourceNotFoundHandler_ = new ResourceNotFoundHandler();
    methodNotFoundHandler_ = new MethodNotFoundHandler();
    std::cout << "Created Http Server" << std::endl;
}

HttpServer::~HttpServer() {
    stop();
    delete badRequestHandler_;
    delete resourceNotFoundHandler_;
    delete methodNotFoundHandler_;
    std::cout << "Stopped server running on port " << port_ << std::endl; 
}

// convert '/path/:id/*/api' -> '^/path/([^/]+)/.*/api$' and tie to RequestHandler
void HttpServer::registerHandler(const HttpMethod method, const std::string& requestURI, RequestHandler* handler) {
    RouteInfo route;

    std::string pattern = "^";
    size_t start = 0, colon = -1;

    while ((colon = requestURI.find(':', start)) != std::string::npos) {
        pattern += requestURI.substr(start, colon - start);
        
        size_t paramEnd = requestURI.find('/', colon);
        if (paramEnd == std::string::npos) {
            paramEnd = requestURI.length();
        }

        std::string paramName = requestURI.substr(colon + 1, paramEnd - colon -1);
        route.paramNames.push_back(paramName);

        pattern += "([^/]+)";
        start = paramEnd;
    }
    
    pattern += requestURI.substr(start) + "$";

    size_t asterisk;
    while ((asterisk = pattern.find('*')) != std::string::npos) {
        pattern.replace(asterisk, 1, ".*");
    }
    
    route.method = method;
    route.pattern = std::regex(pattern);
    route.handler = std::move(handler);
    routes_.push_back(route);
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

        threadPool_.enqueue([this, clientFD]() {
            this->handleConnection(clientFD);
        });
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
    write(clientFD, response_str.c_str(), response_str.length());
    
    close(clientFD);
}


RequestHandler* HttpServer::findHandler(HttpRequest& httpRequest) {
    std::string path = httpRequest.getPath();
    
    bool patternMatchingHandlerExists = false;

    for (const auto& route : routes_) {
        std::smatch matches;
        if (std::regex_match(path, matches, route.pattern)) {
            patternMatchingHandlerExists = true;

            for (size_t i = 0 ; i < route.paramNames.size() && i + 1 < matches.size(); ++i) {
                httpRequest.setPathParam(route.paramNames[i], matches[i + 1].str());
            }
            
            if (route.method == httpRequest.getMethod()) {
                return route.handler;
            }
        }
    }

    return (patternMatchingHandlerExists) ? methodNotFoundHandler_ : resourceNotFoundHandler_;
}
