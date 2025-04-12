#ifndef ROUTEINFO_HPP
#define ROUTEINFO_HPP

#include "handler/RequestHandler.hpp"
#include "request/HttpMethod.hpp"
#include <string>
#include <regex>
#include <vector>

struct RouteInfo {
    HttpMethod method;
    std::regex pattern;
    RequestHandler* handler;
    std::vector<std::string> paramNames;
};

#endif
