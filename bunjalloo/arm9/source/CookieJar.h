#ifndef CookieJar_h_seen
#define CookieJar_h_seen

#include <string>
#include <vector>

class Cookie;
class URI;

class CookieJar
{
  public:
    /** Create a cookie holder. */
    CookieJar();

    ~CookieJar();

    /** Add cookies based on the request header.
     * @param request Set-Cookie request header value.
     */
    void addCookieHeader(const URI & uri, const std::string & request);

    /** Clear cookies for the given domain/path/name.
     * @param domain the domain.
     * @param path the path within the domain.
     * @param name the cookie name.
     */
    void clear(const std::string & domain="",
               const std::string & path="",
               const std::string & name="");

    /** Given a request, fill the headers with the defined cookies.
     * @param request the request string.
     * @param headers the output headers.
     */
    void cookiesForRequest(const URI & request,
        std::string & headers) const;

    bool hasCookieForDomain(const URI & uri, const std::string & name) const;

  private:
    //! List of cookies.
    std::vector<Cookie *> m_cookies;


};

#endif
