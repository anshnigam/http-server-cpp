#include "handler/impl/ResourceNotFoundHandler.hpp"

void ResourceNotFoundHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(404, "Not Found");
    httpResponse.setBody("<html><body>Not Found</body></html>");
}
