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
#ifndef Language_h_seen
#define Language_h_seen

#include <map>
#include "UnicodeString.h"

/** Define to make translation a bit easier.
 * @code
 *   print(T("url"));
 * @endcode
 * This would print the translation to "human" of url, which may be address,
 * dirección, etc.
 */
#define T(s) Language::instance().translate(s)
#include "FileParser.h"

class Language: public FileParser
{
  public:
    /** Name of the directory where language settings are kept.
     * Settings are kept in lang.txt, e.g. en.txt */
    const static char LANGUAGE_DIR[];

    const static char * BUILTIN_LANGS[6];
    /** Singleton pattern used. Again. */
    static Language & instance();

    /** Translate a string to the locale specific translation. If a
     * translation doesn't exist, then s is unicodified and returned.
     * @param s the programmer string
     * @return the user string in the current language.
     */
    UnicodeString translate(const std::string & s);

    /** Set the language */
    void setLanguage(const std::string & l);

    /** Get the current language name.
     * @return the current language name.
     */
    std::string currentLanguage() const;

    void callback(const std::string & first, const std::string & second);
  private:
    typedef std::map<std::string, std::string> TranslateMap;
    TranslateMap m_strings;
    std::string m_lang;

    /** Read language settings.
     */
    Language();
    ~Language();

    void loadLanguageFile();
};

#endif
