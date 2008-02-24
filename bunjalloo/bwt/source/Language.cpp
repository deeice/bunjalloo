/*
  Copyright (C) 2008 Richard Quirk

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
#include "System.h"
#include "File.h"
#include "Language.h"

using namespace std;
const char Language::LANGUAGE_DIR[] = DATADIR"/docs/";

/** Names of the built in languages, ordered the same as user settings */
const char * Language::BUILTIN_LANGS[6] = {
  "jp",
  "en",
  "fr",
  "de",
  "it",
  "es",
};

Language & Language::instance()
{
  static Language s_language;
  return s_language;
}

UnicodeString Language::translate(const std::string & s)
{
  // find s, if exists return m_strings[s] else return string2unicode(s);
  TranslateMap::const_iterator it(m_strings.find(s));
  if (it != m_strings.end())
  {
    return string2unicode(it->second);
  }
  return string2unicode(s);
}

void Language::setLanguage(const std::string & l)
{
  // load the language file
  loadLanguageFile();
}

std::string Language::currentLanguage() const
{
  // switch m_lang, case... 
  return m_lang;
}

Language::Language()
{
  // read user data, load language that applies
  int lang = nds::System::language();
  if (lang > 6 or lang < 0)
  {
    lang = 0;
  }
  m_lang = BUILTIN_LANGS[lang];
  loadLanguageFile();
}

Language::~Language()
{
}

void Language::callback(const std::string & first, const std::string & second)
{
  m_strings[first] = second;
}

void Language::loadLanguageFile()
{
  std::string lf(LANGUAGE_DIR);
  lf += m_lang;
  lf += ".txt";
  if (nds::File::exists(lf.c_str()) == nds::File::F_NONE)
  {
    // default to english...
    lf = LANGUAGE_DIR;
    lf += "en.txt";
  }
  parseFile(lf.c_str());
}


