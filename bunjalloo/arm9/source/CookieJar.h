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

    /** Check if there is a cookie for this domain already in the jar.
     * @param uri the URL of the cookie.
     * @param name the cookie name.
     * @return true if there is a cookie from the domain with the given name, false otherwise.
     */
    bool hasCookieForDomain(const URI & uri, const std::string & name) const;

    /** Check if we should accept cookies from the given domain.
     * @param domain the domain that has sent the cookie.
     * @return true if we accept cookies from this domain. false otherwise.
     */
    bool acceptCookies(const std::string & domain) const;

    /** Set if we should accept cookies, or not, from the given domain.
     * Bunjalloo uses a white-list cookie management policy - everything is
     * rejected unless it is on the list. Therefore this function is used to
     * add domains from the internal list of allowed sites, or to remove
     * already added sites.
     * @param domain the domain to accept or reject cookies from.
     * @param accept if true, then cookies are accepted for the domain.
     * Otherwise they are rejected.
     */
    void setAcceptCookies(const std::string & domain, bool accept=true) const;

  private:
    //! List of cookies.
    std::vector<Cookie *> m_cookies;


};

#endif
