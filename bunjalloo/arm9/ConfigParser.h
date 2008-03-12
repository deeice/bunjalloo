#ifndef ConfigParser_h_seen
#define ConfigParser_h_seen

#include <string>

class Config;
class ConfigParser
{
  public:
    ConfigParser(Config & config);

    void replaceMarkers(std::string & line, const char marker='%');

  private:
    Config & m_config;

    void parseMiddle(std::string & middle);
};
#endif
