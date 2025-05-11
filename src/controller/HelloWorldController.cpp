#include "model/common/Cookie.hpp"
#include "model/request/HttpRequest.hpp"
#include "model/response/HttpResponse.hpp"
#include "controller/HelloWorldController.hpp"

#include <iostream>
#include <optional>

void HelloWorldController::handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) {
    std::cout << "Received request for hello world" << std::endl;
    std::optional<Cookie> cookie = httpRequest.getCookie("name");
    if (cookie) {
        std::cout << "Cookie found with value:" << cookie->toString() << ". Deleting it" << std::endl;
        cookie->setMaxAge(-1);
        httpResponse.addCookie(cookie.value());
    } else {
        std::cout << "Cookie not found, setting cookie in Http Response" << std::endl;
        httpResponse.addCookie(Cookie("name", "test-value"));
    }
    httpResponse.setStatus(200, "OK");
    httpResponse.setBody("<html><body>Hello from server</body></html>");
}
