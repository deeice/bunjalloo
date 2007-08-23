/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef Config_h_seen
#define Config_h_seen

#include <string>

class Document;
class ParameterSet;

class Config
{
  public:

    /** Initialise the config class. Set the document parser and the controller.
     * @param doc the document model of the configuration file.
     * @param controller class that handles the loading, etc, of the config files.
     */
    Config(Document & doc);//, ControllerI & controller);

    /** End of life time. */
    ~Config();

    /** Get the font.
     * @return the base name of the font
     */
    std::string font() const;

    /** Get the proxy.
     * @return the proxy string
     */
    std::string proxy() const;

    /** Get the flag that says whether or not to use the proxy.
     * @return true if we should use a proxy, false otherwise.
     */
    bool useProxy() const;

    /** Reload the config file.
     */
    void reload();

    /** Post a configuration string from a form. The encodedString has the usual
     * cgi format of param1=value&param2=value...
     * @param encodedString contains a CGI-like string containing & seperated key, value pairs.
     */
    void postConfiguration(const std::string & encodedString);

  private:
    static const std::string s_configFile;
    Document & m_document;
    std::string m_font;
    std::string m_cookieList;
    std::string m_proxy;

    void configPathMember(const std::string & value, std::string & member);
    void handleCookies() const;
    void parseLine(ParameterSet & set);

};

#endif
