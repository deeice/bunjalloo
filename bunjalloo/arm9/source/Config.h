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
  private:
    static const std::string s_configFile;
    Document * m_document;
    ControllerI * m_controller;
    bool m_reload;
    std::string m_font;

    Config();
    ~Config();

};

#endif
