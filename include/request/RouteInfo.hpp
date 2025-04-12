#ifndef ROUTEINFO_HPP
#define ROUTEINFO_HPP

#include "handler/RequestHandler.hpp"
#include <string>
#include <regex>
#include <vector>

struct RouteInfo {
    std::regex pattern;
    RequestHandler* handler;
    std::vector<std::string> paramNames;
};

#endif
