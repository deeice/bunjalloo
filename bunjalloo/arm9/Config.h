/*
  Copyright (C) 2007,2008 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef Config_h_seen
#define Config_h_seen

#include <string>
#include "ParameterSet.h"

class Document;
class ParameterSet;

class Config
{
  public:
    static const char PROXY_STR[];
    static const char FONT_STR[];
    static const char COOKIE_STR[];
    static const char CERT_FILE[];
    static const char SEARCHFILE_STR[];
    static const char MAX_CONNECT[];
    static const char TOOLBAR_TIME[];
    static const char USECACHE[];
    static const char CLEARCACHE[];

    /** Initialise the config class. Set the document parser and the controller.
     * @param doc the document model of the configuration file.
     */
    Config(Document & doc);

    /** End of life time. */
    ~Config();

    /** Get a configuration value as a string.
     * @param name the name of the parameter.
     * @param value the value of the parameter.
     * @return true if the value is setm, false otherwise.
     */
    bool resource(const std::string & name, std::string & value) const;

    /** Get a configuration value as a bool. */
    bool resource(const std::string & name, bool & value) const;

    /** Get a configuration value as an int. */
    bool resource(const std::string & name, int & value) const;

    /** Reload the config file.
     */
    void reload();

  private:
    static const std::string s_configFile;
    Document & m_document;
    KeyValueMap m_resources;

    void configPathMember(const std::string & value, std::string & member);
    void handleCookies() const;
    void parseLine(ParameterSet & set);

};

#endif
