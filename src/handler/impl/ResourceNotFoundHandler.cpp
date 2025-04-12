#include "handler/impl/ResourceNotFoundHandler.hpp"

void ResourceNotFoundHandler::handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) {
    httpResponse.setStatus(404, "Not Found");
    httpResponse.setBody("<html><body>Not Found</body></html>");
}
