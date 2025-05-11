#include "handler/impl/BadRequestHandler.hpp"


void BadRequestHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(400, "Bad Request");
    httpResponse.setBody("<html><body>Bad Request</body></html>");
}
