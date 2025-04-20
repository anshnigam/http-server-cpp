#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "pool/executor/ThreadPoolExecutor.hpp"
#include "request/HttpMethod.hpp"
#include "request/HttpRequest.hpp"
#include "handler/RequestHandler.hpp"
#include "request/RouteInfo.hpp"
#include <string>
#include <vector>

class HttpServer {
    public:

        HttpServer(int port);
        ~HttpServer();

        void registerHandler(const HttpMethod requestMethod, const std::string& requestURI, RequestHandler* handler);
        
        bool start();
        void stop();
        
    private:
        int port_;
        int socketFD_;
        bool alive_;

        RequestHandler* badRequestHandler_;
        RequestHandler* resourceNotFoundHandler_;
        RequestHandler* methodNotFoundHandler_;
        ThreadPoolExecutor threadPool_;
        
        std::vector<RouteInfo> routes_;
        

        void acceptConnection();
        void handleConnection(int clientFD);
        // non const arg, as it enriches the request params based on matching route
        RequestHandler* findHandler(HttpRequest& httpRequest);
};

#endif
