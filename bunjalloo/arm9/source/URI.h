#ifndef URI_h_seen
#define URI_h_seen
#include <string>
/** Encapsulate a URI.*/
class URI
{
  public:
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

    /** Is this URI a file?.
     * @return true if the URI represents a file:// URI.
     */
    bool isFile() const;
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


    /** Navigate to a new URI.
     * @param newFile the relative or absolute URI.
     */
    void navigateTo(const std::string & newFile);

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
    std::string m_protocol;
    std::string m_address;
    bool m_fix;
};
#endif
