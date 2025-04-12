#include "handler/impl/MethodNotFoundHandler.hpp"


void MethodNotFoundHandler::handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) {
    httpResponse.setStatus(405, "Method Not Found");
    httpResponse.setBody("<html><body>Method Not Found</body></html>");
}
