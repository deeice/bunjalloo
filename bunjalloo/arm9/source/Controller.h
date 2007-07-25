#ifndef Controller_h_seen
#define Controller_h_seen

class Document;
class View;
class Config;
class URI;

#include "ControllerI.h"

class HttpClient;
/** Controller maps user actions to model updates. For example, responds to setting the URI.
 */
class Controller : public ControllerI
{
  friend class HttpClient;
  public:
    //!Constructor.
    Controller();
    //! Destructor
    ~Controller();

    /** Handle the user request for setting a URI.
     * @param uri the URI to use.
     */
    virtual void doUri(const URI & uri);
    
    virtual void previous();
    virtual void next();

    virtual const Config & config() const;

    /** Show the software licence.*/
    void showLicence();

    /** Loops forever.*/
    void mainLoop();



  private:
    Document * m_document;
    View * m_view;
    Config * m_config;

    void localFile(const std::string &);
    void fetchHttp(const URI &);
    void configureUrl(const std::string & fileName);
    void loadError();
    // helper to avoid code dupe
    void handleUri(const URI & uri);
};
#endif
