#ifndef Document_h_seen
#define Document_h_seen
#include <vector>
#include "UnicodeString.h"
#include "ViewI.h"

class HeaderParser;
class HtmlDocument;
class HtmlElement;

/** This class represents the Document Model and the model in the MVC pattern. */
class Document
{
  public:

    //! Status of the document.
    enum Status {
      NOTHING,   //!< No status set.
      LOADED,    //!< Has fully loaded.
      INPROGRESS //!< Is in the progress of loading.
    };
    //! Constructor.
    Document();

    ~Document();

    /** Set the URI.
     * @param uriString a string representing the URI.
     */
    void setUri(const std::string & uriString);
    
    /** Append data to the model.
     * @param data pointer to the data to add.
     * @param size the amount of data in bytes.
     */
    void appendData(const char * data, int size); 

    /** Append data from a local source to the model. This bypasses the HeaderParser.
     * @param data pointer to the data to add.
     * @param size the amount of data in bytes.
     */
    void appendLocalData(const char * data, int size);
    
    /** Fetch the current URI.
     * @return the URI.
     */
    const std::string & uri() const;

    /** Get the model as text.
     * @return a string of universal characters.
     */
    const UnicodeString & asText() const;

    /** Get the root node of the document model.
     * @return The root node.
     */
    const HtmlElement * rootNode() const;

    /** Get the status of the document.
     * @return the current status.
     */
    Status status() const;
    
    /** Set the status.
     * @param newStatus the new status.
     */
    void setStatus(Status newStatus);
    
    /** Reset the internal state of the model.
     */
    void reset();
    
    /** Get the percentage of the page that has loaded. Note that for chunked pages this returns the
     * percentage of the chunk loaded.
     * @return the percentage of the page (or chunk) loaded
     */
    unsigned int percentLoaded() const;

    /** Register a view to be notified when the model changes.
     * @param v the view to register.
     */
    void registerView(ViewI * v);

    /** Unregister a previously registered view.
     * @param v the view to unregister.
     */
    void unregisterView(ViewI * v);

    void dumpDOM();

    bool hasPreviousHistory() const;
    bool hasNextHistory() const;
    std::string gotoPreviousHistory();
    std::string gotoNextHistory();

  private:
    //std::string m_uri;
    Status m_status;
    int m_amount;
    std::vector<ViewI*> m_views;
    HtmlDocument* m_htmlDocument;
    HeaderParser* m_headerParser;
    std::vector<std::string> m_history;
    std::vector<std::string>::iterator m_historyPosition;

    //! Notify all registered views that there are model changes.
    void notifyAll() const;

    //! No copies.
    Document (const Document&);
    const Document& operator=(const Document&);
};
#endif
