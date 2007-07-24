#ifndef URI_h_seen
#define URI_h_seen
#include "UnicodeString.h"
/** Encapsulate a URI.*/
class URI
{
  public:

    enum Protocol_t
    {
      UNKNOWN_PROTOCOL,
      HTTP_PROTOCOL,
      FILE_PROTOCOL,
      CONFIG_PROTOCOL
    };

    static UnicodeString escape(const UnicodeString & input);

    /** Create an empty URI.*/
    URI();

    /** Create an URI initialised to the given string.
     * @param uriString the URI location
     */
    URI(const std::string & uriString, bool fix=true);

    /** Set the URI location.
     * @param uriString the new location.
     */
    void setUri(const std::string & uriString);

    /** Get the protocol of this URI.
     * @return the URI protocol type, e.g. if it represents a file:// URI returns FILE_PROTOCOL.
     */
    Protocol_t protocol() const;

    /** Is the URI valid (syntactically). It is entirely possible too have a
     * valid URI that describes a non existent file or location.
     * @return true if the URI looks good.
     */
    bool isValid() const;

    /** Get the server part of the URI.
     * @return the server part of http uris.
     */
    std::string server() const;

    /** Get the port part of the URI.
     * @return the port part of http uris.
     */
    int port() const;

    /** Get the file part of the URI.
     * @return the file part of the location.
     */
    const std::string fileName() const;

    /** Get the lot.
     * @return the whole URI.
     */
    const std::string asString() const;

    /** Get the method (GET or POST).
     * @return the HTTP method.
     */
    const std::string method() const;

    /** Set the method (GET or POST).
     * @param the HTTP method.
     */
    void setMethod(const std::string & method);

    /** Navigate to a new URI and return the new address.
     * @param newFile the relative or absolute URI.
     * @return the new URI address.
     */
    URI navigateTo(const std::string & newFile) const;

    /** Set an extra request header to be sent as part of the HTTP request.
     * @param headerData the header data to set
     */
    void setRequestHeader(const std::string & headerData);

    /** Retrieve any extra headers for this request.
     * @return the headers to send.
     */
    std::string requestHeader() const;

    /** Compare this URI to the @a other one. URIs are the same if they have
     * the same protocol and address.
     * @param other the other URI to compare.
     */
    bool operator==(const URI & other);
    /** Inverse compare. @see operator==().
     * @param other the other URI to compare.
     */
    bool operator!=(const URI & other);

  private:
    std::string m_method;
    std::string m_protocol;
    std::string m_address;
    std::string m_requestHeader;
    bool m_fix;
};
#endif
