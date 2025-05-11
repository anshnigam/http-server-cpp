#include "model/common/Cookie.hpp"
#include <ctime>
#include <sstream>
#include <string>

Cookie::Cookie(const std::string& name, const std::string& value)
    : name_(name),
      value_(value),
      path_("/"),
      expiry_(0),
      secure_(true),
      httpOnly_(true),
      sameSite_(NONE)
{}

std::string Cookie::getName() const {
    return name_;
}

std::string Cookie::getValue() const {
    return value_;
}


std::string Cookie::getDomain() const {
    return domain_;
}

std::string Cookie::getPath() const {
    return path_;
}


std::time_t Cookie::getExpiry() const {
    return expiry_;
}

bool Cookie::isSecure() const {
    return secure_;
}

bool Cookie::isHttpOnly() const {
    return httpOnly_;
}

SameSite Cookie::getSameSite() const {
    return sameSite_;
}

void Cookie::setDomain(const std::string& domain) {
    domain_ = domain;
}

void Cookie::setPath(const std::string& path) {
    path_ = path;
}

void Cookie::setExpiry(const time_t expiry) {
    expiry_ = expiry;
}

void Cookie::setMaxAge(int seconds) {
    expiry_ = time(nullptr) + seconds;
}

void Cookie::setSecure(bool secure) {
    secure_ = secure;
}

void Cookie::setHttpOnly(bool httpOnly) {
    httpOnly_ = httpOnly;
}

void Cookie::setSameSite(SameSite sameSite) {
    sameSite_ = sameSite;
}

std::string Cookie::toString() const {
    std::stringstream stream;

    stream << name_ << "=" << value_;

    if (!domain_.empty()) {
        stream << "; Domain=" << domain_;
    }

   if (expiry_ != 0) {
       char timeBuffer[32];
       struct tm* timeInfo = gmtime(&expiry_);
       strftime(timeBuffer, sizeof(timeBuffer), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
       stream << "; Expires=" << timeBuffer;
   }

   if (secure_) {
       stream << "; Secure";
   }

   if (httpOnly_) {
       stream << "; HttpOnly";
   }

   stream << "; SameSite=" << sameSite_;

   return stream.str();
}

Cookie Cookie::valueOf(const std::string& cookieStr) {
    size_t equalPosition = cookieStr.find('=');
    if (equalPosition == std::string::npos) {
        return Cookie("INVALID", "INVALID");
    }
    std::string name = cookieStr.substr(0, equalPosition);

    std::string value;
    size_t semiColonPosition = cookieStr.find(';', equalPosition);
    if (semiColonPosition == std::string::npos) {
        value = cookieStr.substr(equalPosition + 1);
    } else {
        value = cookieStr.substr(equalPosition + 1, semiColonPosition - equalPosition - 1);
    }

    return Cookie(name, value);
}
