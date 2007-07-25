#ifndef Config_h_seen
#define Config_h_seen


#include <string>
#include "ViewI.h"


class Document;
class ControllerI;

class Config: public ViewI
{
  public:

    /** Initialise the config class. Set the document parser and the controller.
     * @param doc the document model of the configuration file.
     * @param controller class that handles the loading, etc, of the config files.
     */
    Config(Document & doc, ControllerI & controller);

    /** End of life time. */
    ~Config();

    /** Get the font.
     * @return the base name of the font
     */
    std::string font() const;

    /** Reload the config file.
     */
    void reload();

    /** Call back for when the config file has been parsed.  */
    virtual void notify();
    
    /** Post a configuration string from a form. The encodedString has the usual
     * cgi format of param1=value&param2=value...
     * @param encodedString contains a CGI-like string containing & seperated key, value pairs.
     */
    void postConfiguration(const std::string & encodedString);

  private:
    static const std::string s_configFile;
    Document & m_document;
    ControllerI & m_controller;
    bool m_reload;
    std::string m_font;
    std::string m_cookieList;
    std::string m_proxy;
    bool m_useProxy;


    void configMember(const std::string & tag, std::string & member);
    void handleCookies() const;

};

#endif
