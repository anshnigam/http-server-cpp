#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "HttpRequest.h"
#include "RequestHandler.h"
#include "RouteInfo.h"
#include <string>
#include <vector>

class HttpServer {
    public:

        HttpServer(int port);
        ~HttpServer();

        void registerHandler(const std::string& requestPath, RequestHandler* handler);
        
        bool start();
        void stop();
        
    private:
        int port_;
        int socketFD_;
        bool alive_;
        
        std::vector<RouteInfo> routes_;

        void acceptConnection();
        void handleConnection(int clientFD);
        RequestHandler* findHandler(const HttpRequest& httpRequest);
};

#endif
