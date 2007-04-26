#ifndef CookieJar_h_seen
#define CookieJar_h_seen

#include <string>
#include <vector>

class Cookie;
class CookieJar
{
  public:
    CookieJar();

    void addCookieHeader(const std::string & request);

    void clear(const std::string & domain="",
               const std::string & path="",
               const std::string & name="");

    void cookiesForRequest(const std::string & request,
        std::string & headers);

  private:
    std::vector<Cookie *> m_cookies;

};

#endif
