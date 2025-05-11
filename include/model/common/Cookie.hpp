#ifndef COOKIE_HPP
#define COOKIE_HPP

# include <string>
# include "model/common/SameSite.hpp"

class Cookie {
    public:
        Cookie(const std::string& name, const std::string& value);

        // getter
        std::string getName() const;
        std::string getValue() const;
        std::string getDomain() const;
        std::string getPath() const;
        time_t getExpiry() const;
        bool isSecure() const;
        bool isHttpOnly() const;
        SameSite getSameSite() const;

        // setter
        void setDomain(const std::string& domain);
        void setPath(const std::string& path);
        void setExpiry(const time_t expiry);
        void setMaxAge(int seconds);
        void setSecure(bool secure);
        void setHttpOnly(bool httpOnly);
        void setSameSite(SameSite sameSite);

        // parser
        std::string toString() const;
        static Cookie valueOf(const std::string& cookieStr);

    private:
        std::string name_;
        std::string value_;
        std::string domain_;
        std::string path_;
        time_t expiry_;
        bool secure_;
        bool httpOnly_;
        SameSite sameSite_;
};

#endif
