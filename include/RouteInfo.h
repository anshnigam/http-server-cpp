#ifndef ROUTEINFO_H
#define ROUTEINFO_H

#include "RequestHandler.h"
#include <string>
#include <regex>
#include <vector>

struct RouteInfo {
    std::regex pattern;
    RequestHandler* handler;
    std::vector<std::string> paramNames;
};

#endif
