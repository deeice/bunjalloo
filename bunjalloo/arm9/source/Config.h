#ifndef Config_h_seen
#define Config_h_seen


#include <string>
#include "ViewI.h"


class Document;
class ControllerI;

class Config: public ViewI
{
  public:
    static Config & instance();

    std::string font() const;
    void initialise(Document * doc, ControllerI * controller);
    void reload();

    void notify();
    
    /** Post a configuration string from a form. The encodedString has the usual
     * cgi format of param1=value&param2=value...
     * @param encodedString contains a CGI-like string containing & seperated key, value pairs.
     */
    void postConfiguration(const std::string & encodedString);

  private:
    static const std::string s_configFile;
    Document * m_document;
    ControllerI * m_controller;
    bool m_reload;
    std::string m_font;
    std::string m_cookieList;
    std::string m_proxy;
    bool m_useProxy;

    Config();
    ~Config();

    void configMember(const std::string & tag, std::string & member);
    void handleCookies() const;

};

#endif
