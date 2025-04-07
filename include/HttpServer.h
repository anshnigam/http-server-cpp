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

        void registerHandler(const std::string& requestURI, RequestHandler* handler);
        
        bool start();
        void stop();
        
    private:
        int port_;
        int socketFD_;
        bool alive_;
        RequestHandler* defaultHandler_;
        
        std::vector<RouteInfo> routes_;
        

        void acceptConnection();
        void handleConnection(int clientFD);
        // non const arg, as it enriches the request params based on matching route
        RequestHandler* findHandler(HttpRequest& httpRequest);
};

#endif
